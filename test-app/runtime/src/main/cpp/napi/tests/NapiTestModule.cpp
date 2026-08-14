#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "NapiTestSupport.h"

typedef struct {
  double value;
} NapiTestPayload;

static int sFinalizerRuns = 0;
static int sWrapCount = 0;
static napi_ref sHeldRef = NULL;

static napi_value EchoString(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  size_t length = 0;
  NAPI_CALL(env, napi_get_value_string_utf8(env, args[0], NULL, 0, &length));

  char* buffer = (char*)malloc(length + 1);
  if (buffer == NULL) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }

  napi_value result = NULL;
  napi_status status = napi_get_value_string_utf8(env, args[0], buffer, length + 1, &length);
  if (status == napi_ok) {
    status = napi_create_string_utf8(env, buffer, length, &result);
  }
  free(buffer);

  if (status != napi_ok) {
    NapiThrowLastError(env);
    return NULL;
  }

  return result;
}

static napi_value DoubleNumber(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double value = 0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &value));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_double(env, value * 2, &result));
  return result;
}

static napi_value NegateBool(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool value = false;
  NAPI_CALL(env, napi_get_value_bool(env, args[0], &value));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_boolean(env, !value, &result));
  return result;
}

static napi_value TransformObject(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value input = NULL;
  NAPI_CALL(env, napi_get_named_property(env, args[0], "value", &input));

  double value = 0;
  NAPI_CALL(env, napi_get_value_double(env, input, &value));

  napi_value doubled = NULL;
  NAPI_CALL(env, napi_create_double(env, value * 2, &doubled));

  napi_value tag = NULL;
  NAPI_CALL(env, napi_create_string_utf8(env, "napi", NAPI_AUTO_LENGTH, &tag));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "value", doubled));
  NAPI_CALL(env, napi_set_named_property(env, result, "tag", tag));
  return result;
}

static napi_value TransformArray(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  uint32_t length = 0;
  NAPI_CALL(env, napi_get_array_length(env, args[0], &length));

  napi_value first = NULL;
  double firstValue = 0;
  if (length > 0) {
    NAPI_CALL(env, napi_get_element(env, args[0], 0, &first));
    NAPI_CALL(env, napi_get_value_double(env, first, &firstValue));
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_array_with_length(env, 2, &result));

  napi_value lengthValue = NULL;
  NAPI_CALL(env, napi_create_double(env, (double)length, &lengthValue));
  NAPI_CALL(env, napi_set_element(env, result, 0, lengthValue));

  napi_value firstOut = NULL;
  NAPI_CALL(env, napi_create_double(env, firstValue, &firstOut));
  NAPI_CALL(env, napi_set_element(env, result, 1, firstOut));
  return result;
}

static napi_value ThrowError(napi_env env, napi_callback_info info) {
  (void)info;
  napi_throw_error(env, "ERR_TEST_CODE", "napi test failure");
  return NULL;
}

static void FinalizePayload(napi_env env, void* data, void* hint) {
  (void)env;
  (void)hint;
  free(data);
  sFinalizerRuns++;
}

static napi_value WrapValue(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double value = 0;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &value));

  NapiTestPayload* payload = (NapiTestPayload*)malloc(sizeof(NapiTestPayload));
  if (payload == NULL) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }
  payload->value = value;

  napi_status status = napi_wrap(env, args[0], payload, FinalizePayload, NULL, NULL);
  if (status != napi_ok) {
    free(payload);
    NapiThrowLastError(env);
    return NULL;
  }

  sWrapCount++;
  return args[0];
}

static napi_value UnwrapValue(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  void* payload = NULL;
  NAPI_CALL(env, napi_unwrap(env, args[0], &payload));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_double(env, ((NapiTestPayload*)payload)->value, &result));
  return result;
}

static napi_value FinalizerRan(napi_env env, napi_callback_info info) {
  (void)info;
  napi_value result = NULL;
  NAPI_CALL(env, napi_get_boolean(env, sFinalizerRuns > 0, &result));
  return result;
}

static napi_value ResetFinalizerFlag(napi_env env, napi_callback_info info) {
  (void)info;
  sFinalizerRuns = 0;

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

static napi_value HoldRef(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  if (sHeldRef != NULL) {
    napi_delete_reference(env, sHeldRef);
    sHeldRef = NULL;
  }
  NAPI_CALL(env, napi_create_reference(env, args[0], 1, &sHeldRef));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

static napi_value GetRef(napi_env env, napi_callback_info info) {
  (void)info;
  napi_value result = NULL;
  if (sHeldRef != NULL) {
    NAPI_CALL(env, napi_get_reference_value(env, sHeldRef, &result));
  }

  // A weak or already-collected reference yields NULL rather than an error.
  if (result == NULL) {
    NAPI_CALL(env, napi_get_undefined(env, &result));
  }
  return result;
}

static napi_value ReleaseRef(napi_env env, napi_callback_info info) {
  (void)info;
  bool released = false;
  if (sHeldRef != NULL) {
    NAPI_CALL(env, napi_delete_reference(env, sHeldRef));
    sHeldRef = NULL;
    released = true;
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_boolean(env, released, &result));
  return result;
}

static napi_value GetWrapCount(napi_env env, napi_callback_info info) {
  (void)info;
  napi_value result = NULL;
  NAPI_CALL(env, napi_create_double(env, (double)sWrapCount, &result));
  return result;
}

//=== Async work ===========================================================

typedef struct {
  napi_async_work work;
  napi_ref callbackRef;
  double input;
  double result;
  bool ranOffJsThread;
  bool sleepBeforeWork;
  pthread_t jsThread;
} NapiAsyncWorkContext;

static void ExecuteAsyncWork(napi_env env, void* data) {
  (void)env;
  NapiAsyncWorkContext* context = (NapiAsyncWorkContext*)data;
  if (context->sleepBeforeWork) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  context->ranOffJsThread = !pthread_equal(pthread_self(), context->jsThread);
  context->result = context->input * 2;
}

static void CompleteAsyncWork(napi_env env, napi_status status, void* data) {
  NapiAsyncWorkContext* context = (NapiAsyncWorkContext*)data;

  napi_value callback = NULL;
  napi_get_reference_value(env, context->callbackRef, &callback);
  if (callback != NULL) {
    napi_value recv = NULL;
    napi_value args[3];
    napi_get_undefined(env, &recv);
    args[0] = NapiStatusValue(env, status);
    napi_create_double(env, context->result, &args[1]);
    napi_get_boolean(env, context->ranOffJsThread, &args[2]);
    napi_call_function(env, recv, callback, 3, args, NULL);
  }

  napi_delete_reference(env, context->callbackRef);
  napi_delete_async_work(env, context->work);
  free(context);
}

static NapiAsyncWorkContext* CreateAsyncWorkContext(napi_env env, napi_value callback, double input,
                                                    bool sleepBeforeWork) {
  NapiAsyncWorkContext* context = (NapiAsyncWorkContext*)calloc(1, sizeof(NapiAsyncWorkContext));
  if (context == NULL) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }

  context->input = input;
  context->sleepBeforeWork = sleepBeforeWork;
  context->jsThread = pthread_self();

  napi_value name = NULL;
  if (napi_create_string_utf8(env, "napi-test-work", NAPI_AUTO_LENGTH, &name) != napi_ok ||
      napi_create_reference(env, callback, 1, &context->callbackRef) != napi_ok ||
      napi_create_async_work(env, NULL, name, ExecuteAsyncWork, CompleteAsyncWork, context,
                             &context->work) != napi_ok) {
    if (context->callbackRef != NULL) {
      napi_delete_reference(env, context->callbackRef);
    }
    free(context);
    NapiThrowLastError(env);
    return NULL;
  }

  return context;
}

static napi_value StartAsyncWork(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double input = 0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &input));

  NapiAsyncWorkContext* context = CreateAsyncWorkContext(env, args[1], input, false);
  if (context == NULL) {
    return NULL;
  }

  NAPI_CALL(env, napi_queue_async_work(env, context->work));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

// Cancellation is inherently racy: the work may already be running by the time
// the cancel lands. The status is returned so the spec can hold both outcomes
// to their own contract instead of guessing which one happened.
static napi_value StartCancelledWork(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  NapiAsyncWorkContext* context = CreateAsyncWorkContext(env, args[0], 1, true);
  if (context == NULL) {
    return NULL;
  }

  NAPI_CALL(env, napi_queue_async_work(env, context->work));
  return NapiStatusValue(env, napi_cancel_async_work(env, context->work));
}

//=== Threadsafe functions =================================================

// Shared between the producer thread and the finalizer, which runs on the JS
// thread and may fire while the producer is still unwinding — env teardown
// closes the function out from under it. Whichever side finishes last frees.
struct NapiTsfnContext {
  napi_threadsafe_function tsfn = NULL;
  napi_ref doneRef = NULL;
  int count = 0;
  std::atomic<int> owners{1};
  std::atomic<int> lastCallStatus{napi_ok};
};

static void ReleaseTsfnContext(NapiTsfnContext* context) {
  if (context->owners.fetch_sub(1) == 1) {
    delete context;
  }
}

// Only ever touched on the JS thread: set when a run starts, cleared by the
// finalizer, read by the reentrant push below.
static napi_threadsafe_function sActiveTsfn = NULL;

static void TsfnCallJs(napi_env env, napi_value js_callback, void* context, void* data) {
  (void)context;

  // A null env means the call was dropped: the function closed before it could
  // be delivered, and this is the addon's chance to free `data`.
  if (env == NULL || js_callback == NULL) {
    return;
  }

  napi_value recv = NULL;
  napi_value arg = NULL;
  napi_get_undefined(env, &recv);
  napi_create_int32(env, (int32_t)(intptr_t)data, &arg);
  napi_call_function(env, recv, js_callback, 1, &arg, NULL);
}

static void TsfnFinalize(napi_env env, void* data, void* hint) {
  (void)hint;
  NapiTsfnContext* context = (NapiTsfnContext*)data;
  sActiveTsfn = NULL;

  if (context->doneRef != NULL) {
    napi_value done = NULL;
    napi_get_reference_value(env, context->doneRef, &done);
    if (done != NULL) {
      napi_value recv = NULL;
      napi_value arg = NapiStatusValue(env, (napi_status)context->lastCallStatus.load());
      napi_get_undefined(env, &recv);
      napi_call_function(env, recv, done, 1, &arg, NULL);
    }
    napi_delete_reference(env, context->doneRef);
  }

  ReleaseTsfnContext(context);
}

static void TsfnProducer(NapiTsfnContext* context) {
  napi_threadsafe_function tsfn = context->tsfn;

  for (int i = 1; i <= context->count; i++) {
    napi_status status =
        napi_call_threadsafe_function(tsfn, (void*)(intptr_t)i, napi_tsfn_blocking);
    context->lastCallStatus.store(status);
    if (status != napi_ok) {
      break;
    }
  }

  napi_release_threadsafe_function(tsfn, napi_tsfn_release);
  ReleaseTsfnContext(context);
}

static napi_value StartTsfn(napi_env env, napi_callback_info info) {
  size_t argc = 4;
  napi_value args[4];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t count = 0;
  int32_t maxQueueSize = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &count));
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &maxQueueSize));

  NapiTsfnContext* context = new NapiTsfnContext();
  context->count = count;

  napi_value name = NULL;
  if (napi_create_string_utf8(env, "napi-test-tsfn", NAPI_AUTO_LENGTH, &name) != napi_ok ||
      napi_create_reference(env, args[3], 1, &context->doneRef) != napi_ok ||
      napi_create_threadsafe_function(env, args[2], NULL, name, (size_t)maxQueueSize, 1, context,
                                      TsfnFinalize, NULL, TsfnCallJs, &context->tsfn) != napi_ok) {
    if (context->doneRef != NULL) {
      napi_delete_reference(env, context->doneRef);
    }
    delete context;
    NapiThrowLastError(env);
    return NULL;
  }

  // Handed to the producer thread; the finalizer holds the reference the
  // context was created with.
  context->owners.fetch_add(1);
  sActiveTsfn = context->tsfn;
  std::thread(TsfnProducer, context).detach();

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

// Pushes from the JS thread, which is legal while a call is being delivered:
// the function cannot close underneath its own callback.
static napi_value PushTsfn(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t value = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &value));

  if (sActiveTsfn == NULL) {
    return NapiStatusValue(env, napi_closing);
  }

  return NapiStatusValue(env, napi_call_threadsafe_function(sActiveTsfn, (void*)(intptr_t)value,
                                                            napi_tsfn_nonblocking));
}

static napi_value ProbeTsfnAbort(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value name = NULL;
  NAPI_CALL(env, napi_create_string_utf8(env, "napi-test-abort", NAPI_AUTO_LENGTH, &name));

  napi_threadsafe_function tsfn = NULL;
  NAPI_CALL(env, napi_create_threadsafe_function(env, args[0], NULL, name, 0, 1, NULL, NULL, NULL,
                                                 TsfnCallJs, &tsfn));

  napi_status queued =
      napi_call_threadsafe_function(tsfn, (void*)(intptr_t)1, napi_tsfn_nonblocking);
  napi_status released = napi_release_threadsafe_function(tsfn, napi_tsfn_abort);
  napi_status afterAbort =
      napi_call_threadsafe_function(tsfn, (void*)(intptr_t)2, napi_tsfn_nonblocking);

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "queued", NapiStatusValue(env, queued)));
  NAPI_CALL(env, napi_set_named_property(env, result, "released", NapiStatusValue(env, released)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "afterAbort", NapiStatusValue(env, afterAbort)));
  return result;
}

//=== Callback scopes ======================================================

static napi_value InvokeViaMakeCallback(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value resourceName = NULL;
  NAPI_CALL(env,
            napi_create_string_utf8(env, "napi-test-callback", NAPI_AUTO_LENGTH, &resourceName));

  napi_value resource = NULL;
  NAPI_CALL(env, napi_create_object(env, &resource));

  napi_async_context context = NULL;
  NAPI_CALL(env, napi_async_init(env, resource, resourceName, &context));

  napi_value recv = NULL;
  NAPI_CALL(env, napi_get_global(env, &recv));

  napi_value result = NULL;
  napi_status status = napi_make_callback(env, context, recv, args[0], 1, &args[1], &result);
  napi_async_destroy(env, context);

  if (status != napi_ok) {
    NapiThrowLastError(env);
    return NULL;
  }

  return result;
}

//=== Cleanup hooks ========================================================

static void FreeCleanupToken(void* arg) { free(arg); }

static void RemoveAsyncCleanupHook(napi_async_cleanup_hook_handle handle, void* data) {
  free(data);
  napi_remove_async_cleanup_hook(handle);
}

static napi_value ExerciseCleanupHooks(napi_env env, napi_callback_info info) {
  (void)info;

  void* token = malloc(1);
  NAPI_CALL(env, napi_add_env_cleanup_hook(env, FreeCleanupToken, token));
  NAPI_CALL(env, napi_remove_env_cleanup_hook(env, FreeCleanupToken, token));

  // Removing twice must be rejected, not silently accepted.
  napi_status removedTwice = napi_remove_env_cleanup_hook(env, FreeCleanupToken, token);

  napi_async_cleanup_hook_handle handle = NULL;
  NAPI_CALL(env, napi_add_async_cleanup_hook(env, RemoveAsyncCleanupHook, malloc(1), &handle));
  if (handle == NULL) {
    napi_throw_error(env, NULL, "expected an async cleanup hook handle");
    return NULL;
  }
  NAPI_CALL(env, napi_remove_async_cleanup_hook(handle));

  // Both kinds are left registered so env teardown has something to run; the
  // tokens they free are what would leak if it never did.
  NAPI_CALL(env, napi_add_env_cleanup_hook(env, FreeCleanupToken, token));
  NAPI_CALL(env, napi_add_async_cleanup_hook(env, RemoveAsyncCleanupHook, malloc(1), NULL));

  return NapiStatusValue(env, removedTwice);
}

static napi_value InitNapiTestModule(napi_env env, napi_value exports) {
  napi_value moduleName = NULL;
  NAPI_CALL(env, napi_create_string_utf8(env, "napitestmodule", NAPI_AUTO_LENGTH, &moduleName));

  napi_property_descriptor properties[] = {
      NAPI_METHOD("echoString", EchoString),
      NAPI_METHOD("doubleNumber", DoubleNumber),
      NAPI_METHOD("negateBool", NegateBool),
      NAPI_METHOD("transformObject", TransformObject),
      NAPI_METHOD("transformArray", TransformArray),
      NAPI_METHOD("throwError", ThrowError),
      NAPI_METHOD("wrapValue", WrapValue),
      NAPI_METHOD("unwrapValue", UnwrapValue),
      NAPI_METHOD("finalizerRan", FinalizerRan),
      NAPI_METHOD("resetFinalizerFlag", ResetFinalizerFlag),
      NAPI_METHOD("holdRef", HoldRef),
      NAPI_METHOD("getRef", GetRef),
      NAPI_METHOD("releaseRef", ReleaseRef),
      NAPI_METHOD("startAsyncWork", StartAsyncWork),
      NAPI_METHOD("startCancelledWork", StartCancelledWork),
      NAPI_METHOD("startTsfn", StartTsfn),
      NAPI_METHOD("pushTsfn", PushTsfn),
      NAPI_METHOD("probeTsfnAbort", ProbeTsfnAbort),
      NAPI_METHOD("invokeViaMakeCallback", InvokeViaMakeCallback),
      NAPI_METHOD("exerciseCleanupHooks", ExerciseCleanupHooks),
      NAPI_GETTER("wrapCount", GetWrapCount),
      NAPI_VALUE("moduleName", moduleName),
  };

  NAPI_CALL(env, napi_define_properties(env, exports, sizeof(properties) / sizeof(properties[0]),
                                        properties));
  return exports;
}

// A statically linked addon cannot use NAPI_MODULE_INIT: the generated symbol
// carries no name and only one can exist per image.
static napi_module sNapiTestModule = {
    NAPI_MODULE_VERSION, 0, __FILE__, InitNapiTestModule, "napitestmodule", NULL, {0},
};

__attribute__((constructor)) static void RegisterNapiTestModule(void) {
  napi_module_register(&sNapiTestModule);
}
