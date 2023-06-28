//
// Created by pkanev on 3/8/2017.
//

// #include <sstream>

// #include <v8_inspector/src/inspector/protocol/Protocol.h>
// #include <v8_inspector/src/inspector/string-util.h>
// #include <v8_inspector/third_party/inspector_protocol/crdtp/error_support.h>
// #include <v8_inspector/third_party/inspector_protocol/crdtp/json.h>
#include "NetworkDomainCallbackHandlers.h"
// #include "NativeScriptAssert.h"
// #include "utils/NetworkRequestData.h"

using namespace tns;

void NetworkDomainCallbackHandlers::ResponseReceivedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    // try {
    //     auto networkAgentInstance = NetworkAgentImpl::Instance;
    //     const std::string wrongParametersError = "Not all parameters are present in the object argument in the call to ResponseReceived! Required params: 'requestId', `timestamp`, `type`, `response`";

    //     if (!networkAgentInstance) {
    //         return;
    //     }

    //     if (args.Length() == 0 || !args[0]->IsObject()) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto isolate = args.GetIsolate();

    //     v8::HandleScope scope(isolate);

    //     auto context = isolate->GetCurrentContext();

    //     v8::Local<v8::Object> argsObj = args[0]->ToObject(context).ToLocalChecked();

    //     if ((!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "type")).FromMaybe(false)) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "response")).FromMaybe(false)) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto type = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "type")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto response = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "response")).ToLocalChecked();
    //     auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber(context).ToLocalChecked()->IntegerValue(context).ToChecked();

    //     auto responseAsObj = response->ToObject(context).ToLocalChecked();
    //     auto connectionReusedProp = ArgConverter::ConvertToV8String(isolate, "connectionReused");
    //     if (!responseAsObj->Has(context, connectionReusedProp).FromMaybe(false)) {
    //         responseAsObj->Set(context, connectionReusedProp, v8::Boolean::New(isolate, true));
    //     }
    //     auto connectionIdProp = ArgConverter::ConvertToV8String(isolate, "connectionId");
    //     if (!responseAsObj->Has(context, connectionIdProp).FromMaybe(false)) {
    //         responseAsObj->Set(context, connectionIdProp, v8::Number::New(isolate, 0));
    //     }
    //     auto encodedDataLengthProp = ArgConverter::ConvertToV8String(isolate, "encodedDataLength");
    //     if (!responseAsObj->Has(context, encodedDataLengthProp).FromMaybe(false)) {
    //         responseAsObj->Set(context, encodedDataLengthProp, v8::Number::New(isolate, 0));
    //     }
    //     auto securityStateProp = ArgConverter::ConvertToV8String(isolate, "securityState");
    //     if (!responseAsObj->Has(context, securityStateProp).FromMaybe(false)) {
    //         responseAsObj->Set(context, securityStateProp, ArgConverter::ConvertToV8String(isolate, "info"));
    //     }
    //     v8::Local<v8::String> responseJson;
    //     auto maybeResponseJson = v8::JSON::Stringify(context, responseAsObj);

    //     if (!maybeResponseJson.ToLocal(&responseJson)) {
    //         throw NativeScriptException("`response` parameter not in the correct format.");
    //     }

    //     const v8_inspector::String16 responseJsonString = v8_inspector::toProtocolString(isolate, responseJson);
    //     std::vector<uint8_t> cbor;
    //     v8_crdtp::json::ConvertJSONToCBOR(v8_crdtp::span<uint16_t>(responseJsonString.characters16(), responseJsonString.length()), &cbor);
    //     std::unique_ptr<protocol::Value> protocolResponseJson = protocol::Value::parseBinary(cbor.data(), cbor.size());

    //     v8_crdtp::ErrorSupport errorSupport;
    //     auto protocolResponseObj = protocol::Network::Response::fromValue(protocolResponseJson.get(), &errorSupport);

    //     std::vector<uint8_t> json;
    //     v8_crdtp::json::ConvertCBORToJSON(errorSupport.Errors(), &json);
    //     auto errorString = String16(reinterpret_cast<const char*>(json.data()), json.size()).utf8();

    //     if (!errorString.empty()) {
    //         auto errorMessage = "Error while parsing debug `response` object. ";
    //         DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorString.c_str());

    //         throw NativeScriptException(errorMessage + errorString);
    //     }

    //     auto requestIdString = ArgConverter::ConvertToString(requestId);
    //     auto networkRequestData = new utils::NetworkRequestData();
    //     networkAgentInstance->m_responses.insert(std::make_pair(requestIdString, networkRequestData));

    //     auto id = ArgConverter::ConvertToV8String(isolate, FrameId);
    //     protocol::Maybe<String16> frameId(ArgConverter::ConvertToString(id).c_str());

    //     networkAgentInstance->m_frontend.responseReceived(
    //         requestIdString.c_str(),
    //         LoaderId,
    //         timeStamp,
    //         ArgConverter::ConvertToString(type).c_str(),
    //         std::move(protocolResponseObj),
    //         std::move(frameId));
    // } catch (NativeScriptException& e) {
    //     e.ReThrowToV8();
    // } catch (std::exception e) {
    //     std::stringstream ss;
    //     ss << "Error: c exception: " << e.what() << std::endl;
    //     NativeScriptException nsEx(ss.str());
    //     nsEx.ReThrowToV8();
    // } catch (...) {
    //     NativeScriptException nsEx(std::string("Error: c exception!"));
    //     nsEx.ReThrowToV8();
    // }
}

void NetworkDomainCallbackHandlers::RequestWillBeSentCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    // try {
    //     auto networkAgentInstance = NetworkAgentImpl::Instance;
    //     const std::string wrongParametersError = "Not all parameters are present in the object argument in the call to RequestWillBeSent! Required params: 'requestId', `url`, `timestamp`, `type`, `request`, `timestamps`";

    //     if (!networkAgentInstance) {
    //         return;
    //     }

    //     if (args.Length() == 0 || !args[0]->IsObject()) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto isolate = args.GetIsolate();

    //     v8::HandleScope scope(isolate);

    //     auto context = isolate->GetCurrentContext();

    //     v8::Local<v8::Object> argsObj = args[0]->ToObject(context).ToLocalChecked();

    //     if ((!(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false)) ||
    //             !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "url")).FromMaybe(false)) ||
    //             !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "request")).FromMaybe(false)) ||
    //             !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false)) ||
    //             !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "type")).FromMaybe(false)))) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto url = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "url")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto request = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "request")).ToLocalChecked();
    //     auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber(context).ToLocalChecked()->IntegerValue(context).ToChecked();
    //     auto typeArg = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "type")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     long long int wallTime = 0;
    //     if (argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "wallTime")).FromMaybe(true)) {
    //         wallTime = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "wallTime")).ToLocalChecked()->ToNumber(context).ToLocalChecked()->IntegerValue(context).ToChecked();
    //     }

    //     auto requestAsObj = request->ToObject(context).ToLocalChecked();
    //     auto initialPriorityProp = ArgConverter::ConvertToV8String(isolate, "initialPriority");
    //     auto referrerPolicyProp = ArgConverter::ConvertToV8String(isolate, "referrerPolicy");
    //     if (!argsObj->Has(context, initialPriorityProp).FromMaybe(false)) {
    //         requestAsObj->Set(context, initialPriorityProp, ArgConverter::ConvertToV8String(isolate, "Medium"));
    //     }
    //     if (!argsObj->Has(context, referrerPolicyProp).FromMaybe(false)) {
    //         requestAsObj->Set(context, referrerPolicyProp, ArgConverter::ConvertToV8String(isolate, "no-referrer-when-downgrade"));
    //     }

    //     v8::Local<v8::String> requestJson;
    //     auto maybeRequestJson = v8::JSON::Stringify(context, requestAsObj);

    //     if (!maybeRequestJson.ToLocal(&requestJson)) {
    //         throw NativeScriptException("`request` parameter not in the correct format.");
    //     }

    //     const String16& requestJsonString16 = toProtocolString(isolate, requestJson);
    //     std::vector<uint8_t> cbor;
    //     v8_crdtp::json::ConvertJSONToCBOR(v8_crdtp::span<uint16_t>(requestJsonString16.characters16(), requestJsonString16.length()), &cbor);
    //     std::unique_ptr<protocol::Value> protocolRequestJson = protocol::Value::parseBinary(cbor.data(), cbor.size());

    //     protocol::ErrorSupport errorSupport;
    //     auto protocolRequestObj = protocol::Network::Request::fromValue(protocolRequestJson.get(), &errorSupport);
    //     auto initiator = protocol::Network::Initiator::create().setType(protocol::Network::Initiator::TypeEnum::Script).build();

    //     std::vector<uint8_t> json;
    //     v8_crdtp::json::ConvertCBORToJSON(errorSupport.Errors(), &json);
    //     auto errorString = String16(reinterpret_cast<const char*>(json.data()), json.size()).utf8();

    //     if (!errorString.empty()) {
    //         auto errorMessage = "Error while parsing debug `request` object. ";
    //         DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorString.c_str());

    //         throw NativeScriptException(errorMessage + errorString);
    //     }

    //     protocol::Maybe<String16> frameId(ArgConverter::ConvertToString(ArgConverter::ConvertToV8String(isolate, FrameId)).c_str());
    //     protocol::Maybe<String16> type(ArgConverter::ConvertToString(typeArg).c_str());
    //     protocol::Maybe<protocol::Network::Response> emptyRedirect;
    //     networkAgentInstance->m_frontend.requestWillBeSent(
    //         ArgConverter::ConvertToString(requestId).c_str(),
    //         LoaderId,
    //         ArgConverter::ConvertToString(url).c_str(),
    //         std::move(protocolRequestObj),
    //         timeStamp,
    //         wallTime,
    //         std::move(initiator),
    //         std::move(emptyRedirect),
    //         std::move(type),
    //         std::move(frameId));
    // } catch (NativeScriptException& e) {
    //     e.ReThrowToV8();
    // } catch (std::exception e) {
    //     std::stringstream ss;
    //     ss << "Error: c exception: " << e.what() << std::endl;
    //     NativeScriptException nsEx(ss.str());
    //     nsEx.ReThrowToV8();
    // } catch (...) {
    //     NativeScriptException nsEx(std::string("Error: c exception!"));
    //     nsEx.ReThrowToV8();
    // }
}

void NetworkDomainCallbackHandlers::DataForRequestIdCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    // try {
    //     auto networkAgentInstance = NetworkAgentImpl::Instance;
    //     const std::string wrongParametersError = "Not all parameters are present in the object argument in the call to DataForRequestId! Required params: 'requestId', `data`, `hasTextContent`";

    //     if (!networkAgentInstance) {
    //         return;
    //     }

    //     if (args.Length() == 0 || !args[0]->IsObject()) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto isolate = args.GetIsolate();

    //     v8::HandleScope scope(isolate);

    //     auto context = isolate->GetCurrentContext();

    //     v8::Local<v8::Object> argsObj = args[0]->ToObject(context).ToLocalChecked();

    //     if (!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "data")).FromMaybe(false) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "hasTextContent")).FromMaybe(false)) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto data = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "data")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto hasTextContent = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "hasTextContent")).ToLocalChecked()->ToBoolean(isolate);

    //     auto requestIdString = ArgConverter::ConvertToString(requestId);
    //     auto dataString = ArgConverter::ConvertToUtf16String(data);
    //     auto hasTextContentBool = hasTextContent->BooleanValue(isolate);

    //     auto responses = networkAgentInstance->m_responses;
    //     auto it = responses.find(requestIdString);

    //     if (it == responses.end()) {
    //         throw NativeScriptException("Response not found for requestId = " + requestIdString);
    //     } else {
    //         utils::NetworkRequestData* response = it->second;

    //         response->setData(dataString);

    //         response->setHasTextContent(hasTextContentBool);
    //     }
    // } catch (NativeScriptException& e) {
    //     e.ReThrowToV8();
    // } catch (std::exception e) {
    //     std::stringstream ss;
    //     ss << "Error: c exception: " << e.what() << std::endl;
    //     NativeScriptException nsEx(ss.str());
    //     nsEx.ReThrowToV8();
    // } catch (...) {
    //     NativeScriptException nsEx(std::string("Error: c exception!"));
    //     nsEx.ReThrowToV8();
    // }
}

void NetworkDomainCallbackHandlers::LoadingFinishedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    // try {
    //     auto networkAgentInstance = NetworkAgentImpl::Instance;
    //     const std::string wrongParametersError = "Not all parameters are present in the object argument in the call to LoadingFinished! Required params: 'requestId', `timeStamp`";

    //     if (!networkAgentInstance) {
    //         return;
    //     }

    //     if (args.Length() == 0 || !args[0]->IsObject()) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto isolate = args.GetIsolate();

    //     v8::HandleScope scope(isolate);

    //     auto context = isolate->GetCurrentContext();

    //     v8::Local<v8::Object> argsObj = args[0]->ToObject(context).ToLocalChecked();

    //     if (!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
    //             !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false)) {
    //         throw NativeScriptException(wrongParametersError);
    //     }

    //     auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString(context).ToLocalChecked();
    //     auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber(context).ToLocalChecked()->IntegerValue(context).ToChecked();
    //     long long int encodedDataLength = 0;
    //     auto encodedDataLengthProp = ArgConverter::ConvertToV8String(isolate, "encodedDataLength");
    //     if (argsObj->Has(context, encodedDataLengthProp).FromMaybe(true)) {
    //         encodedDataLength = argsObj->Get(context, encodedDataLengthProp).ToLocalChecked()->ToNumber(context).ToLocalChecked()->IntegerValue(context).ToChecked();
    //     }

    //     networkAgentInstance->m_frontend.loadingFinished(ArgConverter::ConvertToString(requestId).c_str(), timeStamp, encodedDataLength);
    // } catch (NativeScriptException& e) {
    //     e.ReThrowToV8();
    // } catch (std::exception e) {
    //     std::stringstream ss;
    //     ss << "Error: c exception: " << e.what() << std::endl;
    //     NativeScriptException nsEx(ss.str());
    //     nsEx.ReThrowToV8();
    // } catch (...) {
    //     NativeScriptException nsEx(std::string("Error: c exception!"));
    //     nsEx.ReThrowToV8();
    // }
}

// const char* NetworkDomainCallbackHandlers::FrameId = "";
// const char* NetworkDomainCallbackHandlers::LoaderId = "NSLoaderIdentifier";
