// Addons see NAPI_VERSION 8 unless they ask for more; asking must come before
// the headers. The env still reports module API version 8 either way — this
// only decides which declarations exist.
#define NAPI_VERSION 9

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "NapiTestSupport.h"

// A second addon, kept apart from NapiTestModule so that module stays a
// readable example of the embedding surface while this one sweeps the value,
// property and error APIs the way Node's test/js-native-api suites do.

// Reads a short ASCII selector argument ("utf8", "range", ...). Longer input is
// truncated, which can only turn into a "no such selector" throw.
static bool ReadSelector(napi_env env, napi_value value, char* buffer, size_t size) {
  size_t copied = 0;
  return napi_get_value_string_utf8(env, value, buffer, size, &copied) == napi_ok;
}

static napi_value UnknownSelector(napi_env env, const char* what) {
  napi_throw_error(env, NULL, what);
  return NULL;
}

//=== Strings ==============================================================

// The three encodings measure a string differently: utf8 counts bytes, utf16
// counts code units, latin1 counts characters (V8's String::Length).
static napi_value StringLengths(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  size_t utf8 = 0;
  size_t utf16 = 0;
  size_t latin1 = 0;
  NAPI_CALL(env, napi_get_value_string_utf8(env, args[0], NULL, 0, &utf8));
  NAPI_CALL(env, napi_get_value_string_utf16(env, args[0], NULL, 0, &utf16));
  NAPI_CALL(env, napi_get_value_string_latin1(env, args[0], NULL, 0, &latin1));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "utf8", NapiDouble(env, (double)utf8)));
  NAPI_CALL(env, napi_set_named_property(env, result, "utf16", NapiDouble(env, (double)utf16)));
  NAPI_CALL(env, napi_set_named_property(env, result, "latin1", NapiDouble(env, (double)latin1)));
  return result;
}

// Copies into a buffer of exactly `bufsize`, so a spec can pin the truncation
// contract: at most bufsize - 1 units are written and the rest is the
// terminator.
static napi_value CopyString(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char encoding[16];
  if (!ReadSelector(env, args[1], encoding, sizeof(encoding))) {
    NapiThrowLastError(env);
    return NULL;
  }

  int32_t requested = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &requested));
  size_t bufsize = requested < 0 ? 0 : (size_t)requested;

  size_t copied = 0;
  napi_value text = NULL;
  napi_status status = napi_ok;

  if (strcmp(encoding, "utf16") == 0) {
    char16_t* buffer = (char16_t*)calloc(bufsize + 1, sizeof(char16_t));
    if (buffer == NULL) {
      napi_throw_error(env, NULL, "out of memory");
      return NULL;
    }
    status = napi_get_value_string_utf16(env, args[0], buffer, bufsize, &copied);
    if (status == napi_ok) {
      status = napi_create_string_utf16(env, buffer, copied, &text);
    }
    free(buffer);
  } else {
    char* buffer = (char*)calloc(bufsize + 1, sizeof(char));
    if (buffer == NULL) {
      napi_throw_error(env, NULL, "out of memory");
      return NULL;
    }
    if (strcmp(encoding, "utf8") == 0) {
      status = napi_get_value_string_utf8(env, args[0], buffer, bufsize, &copied);
      if (status == napi_ok) {
        status = napi_create_string_utf8(env, buffer, copied, &text);
      }
    } else if (strcmp(encoding, "latin1") == 0) {
      status = napi_get_value_string_latin1(env, args[0], buffer, bufsize, &copied);
      if (status == napi_ok) {
        status = napi_create_string_latin1(env, buffer, copied, &text);
      }
    } else {
      free(buffer);
      return UnknownSelector(env, "unknown string encoding");
    }
    free(buffer);
  }

  if (status != napi_ok) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "copied", NapiDouble(env, (double)copied)));
  NAPI_CALL(env, napi_set_named_property(env, result, "text", text));
  return result;
}

static napi_value CreateStrings(napi_env env, napi_callback_info info) {
  (void)info;

  static const char16_t kUtf16[] = u"utf16 ü \U0001F600";
  // Bytes above 0x7F are single latin1 characters, not UTF-8 sequences.
  static const char kLatin1[] = {'A', (char)0xE9, (char)0xFF, '\0'};

  napi_value utf8 = NULL;
  napi_value utf8Sized = NULL;
  napi_value utf16 = NULL;
  napi_value latin1 = NULL;
  napi_value empty = NULL;
  NAPI_CALL(env, napi_create_string_utf8(env, "utf8 ü ☃", NAPI_AUTO_LENGTH, &utf8));
  NAPI_CALL(env, napi_create_string_utf8(env, "abcdef", 3, &utf8Sized));
  NAPI_CALL(env, napi_create_string_utf16(env, kUtf16, NAPI_AUTO_LENGTH, &utf16));
  NAPI_CALL(env, napi_create_string_latin1(env, kLatin1, 3, &latin1));
  NAPI_CALL(env, napi_create_string_utf8(env, "", 0, &empty));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "utf8", utf8));
  NAPI_CALL(env, napi_set_named_property(env, result, "utf8Sized", utf8Sized));
  NAPI_CALL(env, napi_set_named_property(env, result, "utf16", utf16));
  NAPI_CALL(env, napi_set_named_property(env, result, "latin1", latin1));
  NAPI_CALL(env, napi_set_named_property(env, result, "empty", empty));
  return result;
}

static napi_value StringStatus(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  size_t length = 0;
  return NapiStatusValue(env, napi_get_value_string_utf8(env, args[0], NULL, 0, &length));
}

//=== Numbers ==============================================================

static napi_value NumberParts(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double asDouble = 0;
  int32_t asInt32 = 0;
  uint32_t asUint32 = 0;
  int64_t asInt64 = 0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &asDouble));
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &asInt32));
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &asUint32));
  NAPI_CALL(env, napi_get_value_int64(env, args[0], &asInt64));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "double", NapiDouble(env, asDouble)));
  NAPI_CALL(env, napi_set_named_property(env, result, "int32", NapiDouble(env, (double)asInt32)));
  NAPI_CALL(env, napi_set_named_property(env, result, "uint32", NapiDouble(env, (double)asUint32)));
  NAPI_CALL(env, napi_set_named_property(env, result, "int64", NapiDouble(env, (double)asInt64)));
  return result;
}

static napi_value CreateNumbers(napi_env env, napi_callback_info info) {
  (void)info;

  napi_value int32Min = NULL;
  napi_value int32Max = NULL;
  napi_value uint32Max = NULL;
  napi_value int64Max = NULL;
  napi_value int64Min = NULL;
  NAPI_CALL(env, napi_create_int32(env, INT32_MIN, &int32Min));
  NAPI_CALL(env, napi_create_int32(env, INT32_MAX, &int32Max));
  NAPI_CALL(env, napi_create_uint32(env, UINT32_MAX, &uint32Max));
  NAPI_CALL(env, napi_create_int64(env, 9007199254740991LL, &int64Max));
  NAPI_CALL(env, napi_create_int64(env, -9007199254740991LL, &int64Min));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "int32Min", int32Min));
  NAPI_CALL(env, napi_set_named_property(env, result, "int32Max", int32Max));
  NAPI_CALL(env, napi_set_named_property(env, result, "uint32Max", uint32Max));
  NAPI_CALL(env, napi_set_named_property(env, result, "int64Max", int64Max));
  NAPI_CALL(env, napi_set_named_property(env, result, "int64Min", int64Min));
  NAPI_CALL(env, napi_set_named_property(env, result, "nan", NapiDouble(env, NAN)));
  NAPI_CALL(env, napi_set_named_property(env, result, "posInf", NapiDouble(env, INFINITY)));
  NAPI_CALL(env, napi_set_named_property(env, result, "negInf", NapiDouble(env, -INFINITY)));
  NAPI_CALL(env, napi_set_named_property(env, result, "negZero", NapiDouble(env, -0.0)));
  return result;
}

static napi_value NumberStatus(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double value = 0;
  return NapiStatusValue(env, napi_get_value_double(env, args[0], &value));
}

//=== Symbols ==============================================================

static napi_value CreateSymbol(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_valuetype type = napi_undefined;
  NAPI_CALL(env, napi_typeof(env, args[0], &type));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_symbol(env, type == napi_undefined ? NULL : args[0], &result));
  return result;
}

static napi_value CreateSymbolStatus(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value symbol = NULL;
  return NapiStatusValue(env, napi_create_symbol(env, args[0], &symbol));
}

// Resolves in the same global registry Symbol.for uses.
static napi_value SymbolFor(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char key[64];
  if (!ReadSelector(env, args[0], key, sizeof(key))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_value result = NULL;
  NAPI_CALL(env, node_api_symbol_for(env, key, NAPI_AUTO_LENGTH, &result));
  return result;
}

// A symbol is a valid property key everywhere a string is; the four operations
// have to agree about that.
static napi_value SymbolKeyRoundTrip(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool hasBefore = false;
  bool hasAfter = false;
  bool deleted = false;
  bool hasAfterDelete = false;
  napi_value read = NULL;
  NAPI_CALL(env, napi_has_property(env, args[0], args[1], &hasBefore));
  NAPI_CALL(env, napi_set_property(env, args[0], args[1], args[2]));
  NAPI_CALL(env, napi_has_property(env, args[0], args[1], &hasAfter));
  NAPI_CALL(env, napi_get_property(env, args[0], args[1], &read));
  NAPI_CALL(env, napi_delete_property(env, args[0], args[1], &deleted));
  NAPI_CALL(env, napi_has_property(env, args[0], args[1], &hasAfterDelete));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "hasBefore", NapiBool(env, hasBefore)));
  NAPI_CALL(env, napi_set_named_property(env, result, "hasAfter", NapiBool(env, hasAfter)));
  NAPI_CALL(env, napi_set_named_property(env, result, "read", read));
  NAPI_CALL(env, napi_set_named_property(env, result, "deleted", NapiBool(env, deleted)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "hasAfterDelete", NapiBool(env, hasAfterDelete)));
  return result;
}

//=== ArrayBuffers, typed arrays and DataViews =============================

static const struct {
  const char* name;
  napi_typedarray_type type;
} kTypedArrayTypes[] = {
    {"int8", napi_int8_array},
    {"uint8", napi_uint8_array},
    {"uint8clamped", napi_uint8_clamped_array},
    {"int16", napi_int16_array},
    {"uint16", napi_uint16_array},
    {"int32", napi_int32_array},
    {"uint32", napi_uint32_array},
    {"float32", napi_float32_array},
    {"float64", napi_float64_array},
    {"bigint64", napi_bigint64_array},
    {"biguint64", napi_biguint64_array},
};

static const size_t kTypedArrayTypeCount = sizeof(kTypedArrayTypes) / sizeof(kTypedArrayTypes[0]);

static bool LookupTypedArrayType(const char* name, napi_typedarray_type* type) {
  for (size_t i = 0; i < kTypedArrayTypeCount; i++) {
    if (strcmp(kTypedArrayTypes[i].name, name) == 0) {
      *type = kTypedArrayTypes[i].type;
      return true;
    }
  }
  return false;
}

static const char* TypedArrayTypeName(napi_typedarray_type type) {
  for (size_t i = 0; i < kTypedArrayTypeCount; i++) {
    if (kTypedArrayTypes[i].type == type) {
      return kTypedArrayTypes[i].name;
    }
  }
  return "unknown";
}

// Bytes are written through the raw pointer, so JS reading them back proves the
// handle and the memory refer to the same buffer.
static napi_value CreateArrayBuffer(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t byteLength = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &byteLength));
  if (byteLength < 0) {
    napi_throw_error(env, NULL, "negative byte length");
    return NULL;
  }

  void* data = NULL;
  napi_value result = NULL;
  NAPI_CALL(env, napi_create_arraybuffer(env, (size_t)byteLength, &data, &result));

  uint8_t* bytes = (uint8_t*)data;
  for (int32_t i = 0; i < byteLength; i++) {
    bytes[i] = (uint8_t)i;
  }
  return result;
}

static napi_value ArrayBufferInfo(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  void* data = NULL;
  size_t byteLength = 0;
  NAPI_CALL(env, napi_get_arraybuffer_info(env, args[0], &data, &byteLength));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(
      env, napi_set_named_property(env, result, "byteLength", NapiDouble(env, (double)byteLength)));
  NAPI_CALL(env, napi_set_named_property(env, result, "hasData", NapiBool(env, data != NULL)));
  if (byteLength > 0) {
    NAPI_CALL(env, napi_set_named_property(env, result, "firstByte",
                                           NapiDouble(env, ((uint8_t*)data)[0])));
    NAPI_CALL(env, napi_set_named_property(env, result, "lastByte",
                                           NapiDouble(env, ((uint8_t*)data)[byteLength - 1])));
  }
  return result;
}

static napi_value WriteByte(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  void* data = NULL;
  size_t byteLength = 0;
  NAPI_CALL(env, napi_get_arraybuffer_info(env, args[0], &data, &byteLength));

  int32_t index = 0;
  int32_t value = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &index));
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &value));
  if (index < 0 || (size_t)index >= byteLength) {
    napi_throw_range_error(env, NULL, "byte index out of range");
    return NULL;
  }

  ((uint8_t*)data)[index] = (uint8_t)value;

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

static napi_value CreateTypedArray(napi_env env, napi_callback_info info) {
  size_t argc = 4;
  napi_value args[4];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char typeName[24];
  if (!ReadSelector(env, args[0], typeName, sizeof(typeName))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_typedarray_type type = napi_uint8_array;
  if (!LookupTypedArrayType(typeName, &type)) {
    return UnknownSelector(env, "unknown typed array type");
  }

  int32_t length = 0;
  int32_t byteOffset = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &length));
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &byteOffset));

  napi_value result = NULL;
  NAPI_CALL(
      env, napi_create_typedarray(env, type, (size_t)length, args[1], (size_t)byteOffset, &result));
  return result;
}

static napi_value TypedArrayInfo(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_typedarray_type type = napi_uint8_array;
  size_t length = 0;
  void* data = NULL;
  napi_value buffer = NULL;
  size_t byteOffset = 0;
  NAPI_CALL(env,
            napi_get_typedarray_info(env, args[0], &type, &length, &data, &buffer, &byteOffset));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(
      env, napi_set_named_property(env, result, "type", NapiString(env, TypedArrayTypeName(type))));
  NAPI_CALL(env, napi_set_named_property(env, result, "length", NapiDouble(env, (double)length)));
  NAPI_CALL(
      env, napi_set_named_property(env, result, "byteOffset", NapiDouble(env, (double)byteOffset)));
  NAPI_CALL(env, napi_set_named_property(env, result, "buffer", buffer));
  // Reading the first element as raw bytes needs no knowledge of the element
  // type, and still proves `data` points past `byteOffset`.
  NAPI_CALL(env, napi_set_named_property(env, result, "firstByte",
                                         NapiDouble(env, length > 0 ? ((uint8_t*)data)[0] : -1)));
  return result;
}

static void FinalizeExternalBytes(napi_env env, void* data, void* hint) {
  (void)env;
  (void)hint;
  free(data);
}

// napi_create_external_arraybuffer hands V8 the addon's own allocation without
// copying, so the bytes JS reads back are the ones written below.
static napi_value CreateExternalTypedArray(napi_env env, napi_callback_info info) {
  (void)info;

  const size_t byteLength = 8;
  uint8_t* bytes = (uint8_t*)malloc(byteLength);
  if (bytes == NULL) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }
  for (size_t i = 0; i < byteLength; i++) {
    bytes[i] = (uint8_t)(10 + i);
  }

  napi_value buffer = NULL;
  napi_status status = napi_create_external_arraybuffer(env, bytes, byteLength,
                                                        FinalizeExternalBytes, NULL, &buffer);
  if (status != napi_ok) {
    free(bytes);
    NapiThrowLastError(env);
    return NULL;
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_typedarray(env, napi_uint8_array, byteLength, buffer, 0, &result));
  return result;
}

static napi_value CreateDataView(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t byteLength = 0;
  int32_t byteOffset = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &byteLength));
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &byteOffset));

  napi_value result = NULL;
  NAPI_CALL(env,
            napi_create_dataview(env, (size_t)byteLength, args[0], (size_t)byteOffset, &result));
  return result;
}

static napi_value DataViewInfo(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  size_t byteLength = 0;
  void* data = NULL;
  napi_value buffer = NULL;
  size_t byteOffset = 0;
  NAPI_CALL(env, napi_get_dataview_info(env, args[0], &byteLength, &data, &buffer, &byteOffset));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(
      env, napi_set_named_property(env, result, "byteLength", NapiDouble(env, (double)byteLength)));
  NAPI_CALL(
      env, napi_set_named_property(env, result, "byteOffset", NapiDouble(env, (double)byteOffset)));
  NAPI_CALL(env, napi_set_named_property(env, result, "buffer", buffer));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "firstByte",
                                    NapiDouble(env, byteLength > 0 ? ((uint8_t*)data)[0] : -1)));
  return result;
}

static napi_value DetachArrayBuffer(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool before = false;
  NAPI_CALL(env, napi_is_detached_arraybuffer(env, args[0], &before));
  napi_status status = napi_detach_arraybuffer(env, args[0]);
  bool after = false;
  NAPI_CALL(env, napi_is_detached_arraybuffer(env, args[0], &after));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "status", NapiStatusValue(env, status)));
  NAPI_CALL(env, napi_set_named_property(env, result, "before", NapiBool(env, before)));
  NAPI_CALL(env, napi_set_named_property(env, result, "after", NapiBool(env, after)));
  return result;
}

//=== Promises =============================================================

static napi_deferred sDeferred = NULL;

static napi_value CreatePromise(napi_env env, napi_callback_info info) {
  (void)info;
  if (sDeferred != NULL) {
    napi_throw_error(env, NULL, "a deferred is already outstanding");
    return NULL;
  }

  napi_value promise = NULL;
  NAPI_CALL(env, napi_create_promise(env, &sDeferred, &promise));
  return promise;
}

// Settling frees the deferred, so the handle is cleared before the call rather
// than after: a second settle must not reach the same pointer.
static napi_value SettlePromise(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  if (sDeferred == NULL) {
    return NapiBool(env, false);
  }

  bool resolve = false;
  NAPI_CALL(env, napi_get_value_bool(env, args[0], &resolve));

  napi_deferred deferred = sDeferred;
  sDeferred = NULL;
  NAPI_CALL(env, resolve ? napi_resolve_deferred(env, deferred, args[1])
                         : napi_reject_deferred(env, deferred, args[1]));
  return NapiBool(env, true);
}

//=== Errors ===============================================================

static napi_value CreateError(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char kind[16];
  if (!ReadSelector(env, args[0], kind, sizeof(kind))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_valuetype codeType = napi_undefined;
  NAPI_CALL(env, napi_typeof(env, args[1], &codeType));
  napi_value code = codeType == napi_string ? args[1] : NULL;

  napi_value result = NULL;
  napi_status status;
  if (strcmp(kind, "error") == 0) {
    status = napi_create_error(env, code, args[2], &result);
  } else if (strcmp(kind, "type") == 0) {
    status = napi_create_type_error(env, code, args[2], &result);
  } else if (strcmp(kind, "range") == 0) {
    status = napi_create_range_error(env, code, args[2], &result);
  } else if (strcmp(kind, "syntax") == 0) {
    status = node_api_create_syntax_error(env, code, args[2], &result);
  } else {
    return UnknownSelector(env, "unknown error kind");
  }

  if (status != napi_ok) {
    NapiThrowLastError(env);
    return NULL;
  }
  return result;
}

static napi_value CreateErrorStatus(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value error = NULL;
  return NapiStatusValue(env, napi_create_error(env, NULL, args[0], &error));
}

static napi_value ThrowErrorKind(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char kind[16];
  char code[32];
  char message[64];
  if (!ReadSelector(env, args[0], kind, sizeof(kind)) ||
      !ReadSelector(env, args[2], message, sizeof(message))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_valuetype codeType = napi_undefined;
  NAPI_CALL(env, napi_typeof(env, args[1], &codeType));
  const char* codePtr = NULL;
  if (codeType == napi_string) {
    if (!ReadSelector(env, args[1], code, sizeof(code))) {
      NapiThrowLastError(env);
      return NULL;
    }
    codePtr = code;
  }

  if (strcmp(kind, "error") == 0) {
    napi_throw_error(env, codePtr, message);
  } else if (strcmp(kind, "type") == 0) {
    napi_throw_type_error(env, codePtr, message);
  } else if (strcmp(kind, "range") == 0) {
    napi_throw_range_error(env, codePtr, message);
  } else if (strcmp(kind, "syntax") == 0) {
    node_api_throw_syntax_error(env, codePtr, message);
  } else {
    return UnknownSelector(env, "unknown error kind");
  }
  return NULL;
}

// napi_throw takes any value, not only an Error.
static napi_value ThrowValue(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_throw(env, args[0]);
  return NULL;
}

//=== Exceptions ===========================================================

static napi_value CallAndCatch(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value recv = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &recv));

  napi_status callStatus = napi_call_function(env, recv, args[0], 0, NULL, NULL);

  bool pendingBefore = false;
  napi_is_exception_pending(env, &pendingBefore);

  // Anything with a preamble refuses to run while an exception is pending; the
  // status is what an addon sees if it ignores the first failure.
  napi_status blockedStatus = napi_call_function(env, recv, args[0], 0, NULL, NULL);

  napi_value caught = NULL;
  napi_status clearStatus = napi_get_and_clear_last_exception(env, &caught);

  bool pendingAfter = true;
  napi_is_exception_pending(env, &pendingAfter);

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "callStatus", NapiStatusValue(env, callStatus)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "pendingBefore", NapiBool(env, pendingBefore)));
  NAPI_CALL(env, napi_set_named_property(env, result, "blockedStatus",
                                         NapiStatusValue(env, blockedStatus)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "clearStatus", NapiStatusValue(env, clearStatus)));
  NAPI_CALL(env, napi_set_named_property(env, result, "pendingAfter", NapiBool(env, pendingAfter)));
  NAPI_CALL(env, napi_set_named_property(env, result, "caught", caught));
  return result;
}

static napi_value CallAndRethrow(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value recv = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &recv));

  if (napi_call_function(env, recv, args[0], 0, NULL, NULL) == napi_ok) {
    napi_throw_error(env, NULL, "the callback was expected to throw");
    return NULL;
  }

  napi_value caught = NULL;
  NAPI_CALL(env, napi_get_and_clear_last_exception(env, &caught));
  napi_throw(env, caught);
  return NULL;
}

// A clean call must leave nothing pending and yield no exception to clear.
static napi_value ClearWithoutException(napi_env env, napi_callback_info info) {
  (void)info;

  napi_value cleared = NULL;
  napi_status status = napi_get_and_clear_last_exception(env, &cleared);

  napi_valuetype type = napi_object;
  NAPI_CALL(env, napi_typeof(env, cleared, &type));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "status", NapiStatusValue(env, status)));
  NAPI_CALL(env, napi_set_named_property(env, result, "isUndefined",
                                         NapiBool(env, type == napi_undefined)));
  return result;
}

//=== References ===========================================================

#define NAPI_COVERAGE_REF_SLOTS 16

static napi_ref sRefs[NAPI_COVERAGE_REF_SLOTS];

static bool ReadRefSlot(napi_env env, napi_value value, int32_t* slot) {
  if (napi_get_value_int32(env, value, slot) != napi_ok) {
    NapiThrowLastError(env);
    return false;
  }
  if (*slot < 0 || *slot >= NAPI_COVERAGE_REF_SLOTS || sRefs[*slot] == NULL) {
    napi_throw_error(env, NULL, "no reference in that slot");
    return false;
  }
  return true;
}

static napi_value RefCreate(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t initialCount = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &initialCount));

  int32_t slot = 0;
  while (slot < NAPI_COVERAGE_REF_SLOTS && sRefs[slot] != NULL) {
    slot++;
  }
  if (slot == NAPI_COVERAGE_REF_SLOTS) {
    napi_throw_error(env, NULL, "no free reference slot");
    return NULL;
  }

  NAPI_CALL(env, napi_create_reference(env, args[0], (uint32_t)initialCount, &sRefs[slot]));
  return NapiDouble(env, slot);
}

// The reference is deleted again straight away: only the status is under test,
// and a leaked slot would starve the table.
static napi_value RefCreateStatus(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_ref ref = NULL;
  napi_status status = napi_create_reference(env, args[0], 1, &ref);
  if (status == napi_ok) {
    napi_delete_reference(env, ref);
  }
  return NapiStatusValue(env, status);
}

static napi_value RefRef(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t slot = 0;
  if (!ReadRefSlot(env, args[0], &slot)) {
    return NULL;
  }

  uint32_t count = 0;
  NAPI_CALL(env, napi_reference_ref(env, sRefs[slot], &count));
  return NapiDouble(env, count);
}

static napi_value RefUnref(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t slot = 0;
  if (!ReadRefSlot(env, args[0], &slot)) {
    return NULL;
  }

  uint32_t count = 0;
  NAPI_CALL(env, napi_reference_unref(env, sRefs[slot], &count));
  return NapiDouble(env, count);
}

static napi_value RefGet(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t slot = 0;
  if (!ReadRefSlot(env, args[0], &slot)) {
    return NULL;
  }

  napi_value value = NULL;
  NAPI_CALL(env, napi_get_reference_value(env, sRefs[slot], &value));

  // A weak reference whose value has been collected yields NULL, not an error.
  if (value == NULL) {
    NAPI_CALL(env, napi_get_undefined(env, &value));
  }
  return value;
}

// Answers "is the referent still there" without handing the value to JS, which
// would put it back on the stack and defeat the collection the caller is
// trying to observe.
static napi_value RefIsLive(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t slot = 0;
  if (!ReadRefSlot(env, args[0], &slot)) {
    return NULL;
  }

  napi_value value = NULL;
  NAPI_CALL(env, napi_get_reference_value(env, sRefs[slot], &value));
  return NapiBool(env, value != NULL);
}

static napi_value RefDelete(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t slot = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &slot));
  if (slot < 0 || slot >= NAPI_COVERAGE_REF_SLOTS || sRefs[slot] == NULL) {
    return NapiBool(env, false);
  }

  NAPI_CALL(env, napi_delete_reference(env, sRefs[slot]));
  sRefs[slot] = NULL;
  return NapiBool(env, true);
}

//=== Conversions ==========================================================

static napi_value Coerce(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  char kind[16];
  if (!ReadSelector(env, args[0], kind, sizeof(kind))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_value result = NULL;
  if (strcmp(kind, "bool") == 0) {
    NAPI_CALL(env, napi_coerce_to_bool(env, args[1], &result));
  } else if (strcmp(kind, "number") == 0) {
    NAPI_CALL(env, napi_coerce_to_number(env, args[1], &result));
  } else if (strcmp(kind, "string") == 0) {
    NAPI_CALL(env, napi_coerce_to_string(env, args[1], &result));
  } else if (strcmp(kind, "object") == 0) {
    NAPI_CALL(env, napi_coerce_to_object(env, args[1], &result));
  } else {
    return UnknownSelector(env, "unknown coercion kind");
  }
  return result;
}

//=== Properties ===========================================================

static napi_value PropertyOps(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool has = false;
  bool hasOwn = false;
  bool deleted = false;
  bool hasAfterDelete = false;
  napi_value read = NULL;
  NAPI_CALL(env, napi_has_property(env, args[0], args[1], &has));
  NAPI_CALL(env, napi_has_own_property(env, args[0], args[1], &hasOwn));
  NAPI_CALL(env, napi_get_property(env, args[0], args[1], &read));
  NAPI_CALL(env, napi_delete_property(env, args[0], args[1], &deleted));
  NAPI_CALL(env, napi_has_property(env, args[0], args[1], &hasAfterDelete));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "has", NapiBool(env, has)));
  NAPI_CALL(env, napi_set_named_property(env, result, "hasOwn", NapiBool(env, hasOwn)));
  NAPI_CALL(env, napi_set_named_property(env, result, "read", read));
  NAPI_CALL(env, napi_set_named_property(env, result, "deleted", NapiBool(env, deleted)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "hasAfterDelete", NapiBool(env, hasAfterDelete)));
  return result;
}

static napi_value ElementOps(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t index = 0;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &index));

  bool has = false;
  bool deleted = false;
  bool hasAfterDelete = false;
  napi_value read = NULL;
  NAPI_CALL(env, napi_has_element(env, args[0], (uint32_t)index, &has));
  NAPI_CALL(env, napi_get_element(env, args[0], (uint32_t)index, &read));
  NAPI_CALL(env, napi_delete_element(env, args[0], (uint32_t)index, &deleted));
  NAPI_CALL(env, napi_has_element(env, args[0], (uint32_t)index, &hasAfterDelete));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "has", NapiBool(env, has)));
  NAPI_CALL(env, napi_set_named_property(env, result, "read", read));
  NAPI_CALL(env, napi_set_named_property(env, result, "deleted", NapiBool(env, deleted)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "hasAfterDelete", NapiBool(env, hasAfterDelete)));
  return result;
}

static napi_value PropertyNames(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_property_names(env, args[0], &result));
  return result;
}

static napi_value AllPropertyNames(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool ownOnly = false;
  NAPI_CALL(env, napi_get_value_bool(env, args[1], &ownOnly));

  char filterName[24];
  if (!ReadSelector(env, args[2], filterName, sizeof(filterName))) {
    NapiThrowLastError(env);
    return NULL;
  }

  napi_key_filter filter = napi_key_all_properties;
  if (strcmp(filterName, "all") == 0) {
    filter = napi_key_all_properties;
  } else if (strcmp(filterName, "writable") == 0) {
    filter = napi_key_writable;
  } else if (strcmp(filterName, "enumerable") == 0) {
    filter = napi_key_enumerable;
  } else if (strcmp(filterName, "configurable") == 0) {
    filter = napi_key_configurable;
  } else if (strcmp(filterName, "skip_strings") == 0) {
    filter = napi_key_skip_strings;
  } else if (strcmp(filterName, "skip_symbols") == 0) {
    filter = napi_key_skip_symbols;
  } else {
    return UnknownSelector(env, "unknown key filter");
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_all_property_names(
                     env, args[0], ownOnly ? napi_key_own_only : napi_key_include_prototypes,
                     filter, napi_key_numbers_to_strings, &result));
  return result;
}

// Index keys survive napi_key_keep_numbers as numbers; the other conversion
// stringifies them.
static napi_value IndexKeyTypes(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value kept = NULL;
  napi_value converted = NULL;
  NAPI_CALL(env,
            napi_get_all_property_names(env, args[0], napi_key_own_only, napi_key_all_properties,
                                        napi_key_keep_numbers, &kept));
  NAPI_CALL(env,
            napi_get_all_property_names(env, args[0], napi_key_own_only, napi_key_all_properties,
                                        napi_key_numbers_to_strings, &converted));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "kept", kept));
  NAPI_CALL(env, napi_set_named_property(env, result, "converted", converted));
  return result;
}

static double sAccessorValue = 0;

static napi_value AccessorGet(napi_env env, napi_callback_info info) {
  void* data = NULL;
  NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, NULL, &data));
  // `data` distinguishes two properties sharing one callback.
  return NapiDouble(env, sAccessorValue + (double)(intptr_t)data);
}

static napi_value AccessorSet(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));
  NAPI_CALL(env, napi_get_value_double(env, args[0], &sAccessorValue));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

static napi_value DefineOnTarget(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value one = NULL;
  NAPI_CALL(env, napi_create_double(env, 1, &one));

  napi_property_descriptor properties[] = {
      {"base", NULL, NULL, AccessorGet, AccessorSet, NULL, napi_enumerable, (void*)(intptr_t)0},
      {"offset", NULL, NULL, AccessorGet, NULL, NULL, napi_enumerable, (void*)(intptr_t)100},
      {"locked", NULL, NULL, NULL, NULL, one, napi_default, NULL},
      {"open", NULL, NULL, NULL, NULL, one,
       (napi_property_attributes)(napi_writable | napi_enumerable | napi_configurable), NULL},
  };

  NAPI_CALL(env, napi_define_properties(env, args[0], sizeof(properties) / sizeof(properties[0]),
                                        properties));
  return args[0];
}

//=== Types, identity and object shape =====================================

static const char* ValueTypeName(napi_valuetype type) {
  switch (type) {
    case napi_undefined:
      return "undefined";
    case napi_null:
      return "null";
    case napi_boolean:
      return "boolean";
    case napi_number:
      return "number";
    case napi_string:
      return "string";
    case napi_symbol:
      return "symbol";
    case napi_object:
      return "object";
    case napi_function:
      return "function";
    case napi_external:
      return "external";
    case napi_bigint:
      return "bigint";
    default:
      return "unknown";
  }
}

static napi_value TypeOf(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_valuetype type = napi_undefined;
  NAPI_CALL(env, napi_typeof(env, args[0], &type));
  return NapiString(env, ValueTypeName(type));
}

static napi_value Predicates(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool isArray = false;
  bool isDate = false;
  bool isError = false;
  bool isPromise = false;
  bool isArrayBuffer = false;
  bool isTypedArray = false;
  bool isDataView = false;
  bool isBuffer = false;
  NAPI_CALL(env, napi_is_array(env, args[0], &isArray));
  NAPI_CALL(env, napi_is_date(env, args[0], &isDate));
  NAPI_CALL(env, napi_is_error(env, args[0], &isError));
  NAPI_CALL(env, napi_is_promise(env, args[0], &isPromise));
  NAPI_CALL(env, napi_is_arraybuffer(env, args[0], &isArrayBuffer));
  NAPI_CALL(env, napi_is_typedarray(env, args[0], &isTypedArray));
  NAPI_CALL(env, napi_is_dataview(env, args[0], &isDataView));
  NAPI_CALL(env, napi_is_buffer(env, args[0], &isBuffer));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "array", NapiBool(env, isArray)));
  NAPI_CALL(env, napi_set_named_property(env, result, "date", NapiBool(env, isDate)));
  NAPI_CALL(env, napi_set_named_property(env, result, "error", NapiBool(env, isError)));
  NAPI_CALL(env, napi_set_named_property(env, result, "promise", NapiBool(env, isPromise)));
  NAPI_CALL(env, napi_set_named_property(env, result, "arraybuffer", NapiBool(env, isArrayBuffer)));
  NAPI_CALL(env, napi_set_named_property(env, result, "typedarray", NapiBool(env, isTypedArray)));
  NAPI_CALL(env, napi_set_named_property(env, result, "dataview", NapiBool(env, isDataView)));
  NAPI_CALL(env, napi_set_named_property(env, result, "buffer", NapiBool(env, isBuffer)));
  return result;
}

static napi_value StrictEquals(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool equal = false;
  NAPI_CALL(env, napi_strict_equals(env, args[0], args[1], &equal));
  return NapiBool(env, equal);
}

static napi_value InstanceOf(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  bool result = false;
  NAPI_CALL(env, napi_instanceof(env, args[0], args[1], &result));
  return NapiBool(env, result);
}

static napi_value NewInstance(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value result = NULL;
  NAPI_CALL(env, napi_new_instance(env, args[0], 1, &args[1], &result));
  return result;
}

static napi_value GetPrototype(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_prototype(env, args[0], &result));
  return result;
}

static napi_value FreezeObject(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  NAPI_CALL(env, napi_object_freeze(env, args[0]));
  return args[0];
}

static napi_value SealObject(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  NAPI_CALL(env, napi_object_seal(env, args[0]));
  return args[0];
}

static napi_value CreateDate(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double millis = 0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &millis));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_date(env, millis, &result));
  return result;
}

static napi_value DateValue(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double millis = 0;
  napi_status status = napi_get_date_value(env, args[0], &millis);
  if (status != napi_ok) {
    return NapiStatusValue(env, status);
  }
  return NapiDouble(env, millis);
}

static void FinalizeExternalData(napi_env env, void* data, void* hint) {
  (void)env;
  (void)hint;
  free(data);
}

static napi_value CreateExternal(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double* payload = (double*)malloc(sizeof(double));
  if (payload == NULL) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }
  NAPI_CALL(env, napi_get_value_double(env, args[0], payload));

  napi_value result = NULL;
  napi_status status = napi_create_external(env, payload, FinalizeExternalData, NULL, &result);
  if (status != napi_ok) {
    free(payload);
    NapiThrowLastError(env);
    return NULL;
  }
  return result;
}

static napi_value ExternalValue(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  void* payload = NULL;
  napi_status status = napi_get_value_external(env, args[0], &payload);
  if (status != napi_ok) {
    return NapiStatusValue(env, status);
  }
  return NapiDouble(env, *(double*)payload);
}

static napi_value NapiVersion(napi_env env, napi_callback_info info) {
  (void)info;

  uint32_t version = 0;
  NAPI_CALL(env, napi_get_version(env, &version));
  return NapiDouble(env, version);
}

static napi_value NodeVersion(napi_env env, napi_callback_info info) {
  (void)info;

  const napi_node_version* version = NULL;
  NAPI_CALL(env, napi_get_node_version(env, &version));

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env, napi_set_named_property(env, result, "major", NapiDouble(env, version->major)));
  NAPI_CALL(env, napi_set_named_property(env, result, "minor", NapiDouble(env, version->minor)));
  NAPI_CALL(env, napi_set_named_property(env, result, "patch", NapiDouble(env, version->patch)));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "release", NapiString(env, version->release)));
  return result;
}

// The two APIs with no runtime-event-loop-shaped answer report failure rather than
// improvising one.
static napi_value UnsupportedApiStatuses(napi_env env, napi_callback_info info) {
  (void)info;

  struct uv_loop_s* loop = NULL;
  napi_status eventLoop = napi_get_uv_event_loop(env, &loop);

  const char* fileName = NULL;
  napi_status moduleFileName = node_api_get_module_file_name(env, &fileName);

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_object(env, &result));
  NAPI_CALL(env,
            napi_set_named_property(env, result, "uvEventLoop", NapiStatusValue(env, eventLoop)));
  NAPI_CALL(env, napi_set_named_property(env, result, "moduleFileName",
                                         NapiStatusValue(env, moduleFileName)));
  return result;
}

//=== Registration =========================================================

static napi_value InitNapiCoverageModule(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      NAPI_METHOD("stringLengths", StringLengths),
      NAPI_METHOD("copyString", CopyString),
      NAPI_METHOD("createStrings", CreateStrings),
      NAPI_METHOD("stringStatus", StringStatus),
      NAPI_METHOD("numberParts", NumberParts),
      NAPI_METHOD("createNumbers", CreateNumbers),
      NAPI_METHOD("numberStatus", NumberStatus),
      NAPI_METHOD("createSymbol", CreateSymbol),
      NAPI_METHOD("createSymbolStatus", CreateSymbolStatus),
      NAPI_METHOD("symbolFor", SymbolFor),
      NAPI_METHOD("symbolKeyRoundTrip", SymbolKeyRoundTrip),
      NAPI_METHOD("createArrayBuffer", CreateArrayBuffer),
      NAPI_METHOD("arrayBufferInfo", ArrayBufferInfo),
      NAPI_METHOD("writeByte", WriteByte),
      NAPI_METHOD("createTypedArray", CreateTypedArray),
      NAPI_METHOD("typedArrayInfo", TypedArrayInfo),
      NAPI_METHOD("createExternalTypedArray", CreateExternalTypedArray),
      NAPI_METHOD("createDataView", CreateDataView),
      NAPI_METHOD("dataViewInfo", DataViewInfo),
      NAPI_METHOD("detachArrayBuffer", DetachArrayBuffer),
      NAPI_METHOD("createPromise", CreatePromise),
      NAPI_METHOD("settlePromise", SettlePromise),
      NAPI_METHOD("createError", CreateError),
      NAPI_METHOD("createErrorStatus", CreateErrorStatus),
      NAPI_METHOD("throwErrorKind", ThrowErrorKind),
      NAPI_METHOD("throwValue", ThrowValue),
      NAPI_METHOD("callAndCatch", CallAndCatch),
      NAPI_METHOD("callAndRethrow", CallAndRethrow),
      NAPI_METHOD("clearWithoutException", ClearWithoutException),
      NAPI_METHOD("refCreate", RefCreate),
      NAPI_METHOD("refCreateStatus", RefCreateStatus),
      NAPI_METHOD("refRef", RefRef),
      NAPI_METHOD("refUnref", RefUnref),
      NAPI_METHOD("refGet", RefGet),
      NAPI_METHOD("refIsLive", RefIsLive),
      NAPI_METHOD("refDelete", RefDelete),
      NAPI_METHOD("coerce", Coerce),
      NAPI_METHOD("propertyOps", PropertyOps),
      NAPI_METHOD("elementOps", ElementOps),
      NAPI_METHOD("propertyNames", PropertyNames),
      NAPI_METHOD("allPropertyNames", AllPropertyNames),
      NAPI_METHOD("indexKeyTypes", IndexKeyTypes),
      NAPI_METHOD("defineOnTarget", DefineOnTarget),
      NAPI_METHOD("typeOf", TypeOf),
      NAPI_METHOD("predicates", Predicates),
      NAPI_METHOD("strictEquals", StrictEquals),
      NAPI_METHOD("instanceOf", InstanceOf),
      NAPI_METHOD("newInstance", NewInstance),
      NAPI_METHOD("getPrototype", GetPrototype),
      NAPI_METHOD("freezeObject", FreezeObject),
      NAPI_METHOD("sealObject", SealObject),
      NAPI_METHOD("createDate", CreateDate),
      NAPI_METHOD("dateValue", DateValue),
      NAPI_METHOD("createExternal", CreateExternal),
      NAPI_METHOD("externalValue", ExternalValue),
      NAPI_METHOD("napiVersion", NapiVersion),
      NAPI_METHOD("nodeVersion", NodeVersion),
      NAPI_METHOD("unsupportedApiStatuses", UnsupportedApiStatuses),
  };

  NAPI_CALL(env, napi_define_properties(env, exports, sizeof(properties) / sizeof(properties[0]),
                                        properties));
  return exports;
}

static napi_module sNapiCoverageModule = {
    NAPI_MODULE_VERSION, 0, __FILE__, InitNapiCoverageModule, "napicoveragemodule", NULL, {0},
};

__attribute__((constructor)) static void RegisterNapiCoverageModule(void) {
  napi_module_register(&sNapiCoverageModule);
}
