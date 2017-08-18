//
// Created by pkanev on 3/8/2017.
//

#include <sstream>
#include "NetworkDomainCallbackHandlers.h"
#include "NativeScriptAssert.h"

using namespace tns;

void NetworkDomainCallbackHandlers::ResponseReceivedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto networkAgentInstance = V8NetworkAgentImpl::Instance;

        if (!networkAgentInstance) {
            return;
        }

        if (args.Length() != 0 && !args[0]->IsObject()) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        auto context = isolate->GetCurrentContext();

        v8::Local<v8::Object> argsObj = args[0]->ToObject();

        if ((!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "type")).FromMaybe(false)) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "response")).FromMaybe(false)) {
            throw NativeScriptException(
                "Not all parameters are present in the object argument in the call to ResponseReceived! Required params: 'requestId', `timestamp`, `type`, `response`");
        }

        auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString();
        auto type = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "type")).ToLocalChecked()->ToString();
        auto response = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "response")).ToLocalChecked();
        auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber()->IntegerValue();

        auto responseAsObj = response->ToObject();
        v8::Local<v8::String> responseJson;
        auto maybeResponseJson = v8::JSON::Stringify(context, responseAsObj);

        if (!maybeResponseJson.ToLocal(&responseJson)) {
            throw NativeScriptException("`response` parameter not in the correct format.");
        }

        auto responseJsonString = ArgConverter::ConvertToUtf16String(responseJson);
        auto responseUtf16Data = responseJsonString.data();
        auto protocolResponseJson = protocol::parseJSON(String16((const uint16_t*) responseUtf16Data));

        protocol::ErrorSupport errorSupport;

        auto protocolResponseObj = protocol::Network::Response::parse(protocolResponseJson.get(),
                                   &errorSupport);

        auto errorString = errorSupport.errors().utf8();

        if (!errorString.empty()) {
            auto errorMessage = "Error while parsing debug `response` object. ";
            DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorString.c_str());

            throw NativeScriptException(errorMessage + errorString);
        }

        auto requestIdString = ArgConverter::ConvertToString(requestId).c_str();
        auto networkRequestData = new v8_inspector::utils::NetworkRequestData();
        networkAgentInstance->m_responses.insert(std::make_pair(requestIdString, networkRequestData));

        networkAgentInstance->m_frontend.responseReceived(requestIdString,
                FrameId,
                LoaderId,
                timeStamp,
                ArgConverter::ConvertToString(
                    type).c_str(),
                std::move(protocolResponseObj));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

void NetworkDomainCallbackHandlers::RequestWillBeSentCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto networkAgentInstance = V8NetworkAgentImpl::Instance;

        if (!networkAgentInstance) {
            return;
        }

        if (args.Length() != 0 && !args[0]->IsObject()) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        auto context = isolate->GetCurrentContext();

        v8::Local<v8::Object> argsObj = args[0]->ToObject();

        if ((!(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false)) ||
                !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "url")).FromMaybe(false)) ||
                !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "request")).FromMaybe(false)) ||
                !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false)) ||
                !(argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "type")).FromMaybe(false)))) {
            throw NativeScriptException("Not all parameters are present in the object argument in the call to RequestWillBeSent! Required params: 'requestId', `url`, `timestamp`, `type`, `request`, `timestamps`");
        }

        auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString();
        auto url = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "url")).ToLocalChecked()->ToString();
        auto request = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "request")).ToLocalChecked();
        auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber()->IntegerValue();
        auto typeArg = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "type")).ToLocalChecked()->ToString();

        auto requestAsObj = request->ToObject();
        v8::Local<v8::String> requestJson;
        auto maybeRequestJson = v8::JSON::Stringify(context, requestAsObj);

        if (!maybeRequestJson.ToLocal(&requestJson)) {
            throw NativeScriptException("`request` parameter not in the correct format.");
        }

        auto requestJsonString = ArgConverter::ConvertToUtf16String(requestJson);
        auto requestUtf16Data = requestJsonString.data();
        auto protocolRequestJson = protocol::parseJSON(String16((const uint16_t*)  requestUtf16Data));

        protocol::ErrorSupport errorSupport;

        auto protocolRequestObj = protocol::Network::Request::parse(protocolRequestJson.get(), &errorSupport);
        auto initiator = protocol::Network::Initiator::create().setType(protocol::Network::Initiator::TypeEnum::Script).build();

        auto errorString = errorSupport.errors().utf8();

        if (!errorString.empty()) {
            auto errorMessage = "Error while parsing debug `request` object. ";
            DEBUG_WRITE_FORCE("%s Error: %s", errorMessage, errorString.c_str());

            throw NativeScriptException(errorMessage + errorString);
        }

        protocol::Maybe<String16> type(ArgConverter::ConvertToString(typeArg).c_str());
        protocol::Maybe<protocol::Network::Response> emptyRedirect;
        networkAgentInstance->m_frontend.requestWillBeSent(ArgConverter::ConvertToString(requestId).c_str(),
                FrameId,
                LoaderId,
                ArgConverter::ConvertToString(url).c_str(),
                std::move(protocolRequestObj),
                timeStamp,
                std::move(initiator),
                emptyRedirect,
                type);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

void NetworkDomainCallbackHandlers::DataForRequestIdCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto networkAgentInstance = V8NetworkAgentImpl::Instance;

        if (!networkAgentInstance) {
            return;
        }

        if (args.Length() != 0 && !args[0]->IsObject()) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        auto context = isolate->GetCurrentContext();

        v8::Local<v8::Object> argsObj = args[0]->ToObject();

        if (!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "data")).FromMaybe(false) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "hasTextContent")).FromMaybe(false)) {
            throw NativeScriptException(
                "Not all parameters are present in the object argument in the call to DataForRequestId! Required params: 'requestId', `data`, `hasTextContent`");
        }

        auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString();
        auto data = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "data")).ToLocalChecked()->ToString();
        auto hasTextContent = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "hasTextContent")).ToLocalChecked()->ToBoolean();

        auto requestIdString = ArgConverter::ConvertToString(requestId).c_str();
        auto dataString = ArgConverter::ConvertToUtf16String(data);
        auto hasTextContentBool = hasTextContent->BooleanValue();

        auto responses = networkAgentInstance->m_responses;
        auto it = responses.find(requestIdString);

        if (it == responses.end()) {
            throw NativeScriptException("Response not found for requestId = " +
                                        ArgConverter::ConvertToString(requestId));
        } else {
            v8_inspector::utils::NetworkRequestData* response = it->second;

            response->setData(dataString);

            response->setHasTextContent(hasTextContentBool);
        }
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

void NetworkDomainCallbackHandlers::LoadingFinishedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto networkAgentInstance = V8NetworkAgentImpl::Instance;

        if (!networkAgentInstance) {
            return;
        }

        if (args.Length() != 0 && !args[0]->IsObject()) {
            return;
        }

        auto isolate = args.GetIsolate();

        v8::HandleScope scope(isolate);

        auto context = isolate->GetCurrentContext();

        v8::Local<v8::Object> argsObj = args[0]->ToObject();

        if (!argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "requestId")).FromMaybe(false) ||
                !argsObj->Has(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).FromMaybe(false)) {
            throw NativeScriptException(
                "Not all parameters are present in the object argument in the call to LoadingFinished! Required params: 'requestId', `timeStamp`");
        }

        auto requestId = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "requestId")).ToLocalChecked()->ToString();
        auto timeStamp = argsObj->Get(context, ArgConverter::ConvertToV8String(isolate, "timestamp")).ToLocalChecked()->ToNumber()->IntegerValue();

        networkAgentInstance->m_frontend.loadingFinished(ArgConverter::ConvertToString(requestId).c_str(), timeStamp);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c exception!"));
        nsEx.ReThrowToV8();
    }
}

const char* NetworkDomainCallbackHandlers::FrameId = "NSFrameIdentifier";
const char* NetworkDomainCallbackHandlers::LoaderId = "NSLoaderIdentifier";