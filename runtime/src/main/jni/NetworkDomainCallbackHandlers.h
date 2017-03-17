//
// Created by pkanev on 3/8/2017.
//

#ifndef NETWORKDOMAINCALLBACKHANDLERS_H
#define NETWORKDOMAINCALLBACKHANDLERS_H


#include <include/v8.h>
#include <v8_inspector/src/inspector/v8-network-agent-impl.h>
#include "JsV8InspectorClient.h"
#include "ArgConverter.h"

namespace tns {
namespace NetworkDomainCallbackHandlers {

static const char* FrameId = "NSFrameIdentifier";
static const char* LoaderId = "NSLoaderIdentifier";

static void ResponseReceivedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
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

        auto responseJsonString = ArgConverter::ConvertToString(responseJson).c_str();
        auto protocolResponseJson = protocol::parseJSON(responseJsonString);

        protocol::ErrorSupport errorSupport;

        auto protocolResponseObj = protocol::Network::Response::parse(protocolResponseJson.get(),
                                   &errorSupport);

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

static void RequestWillBeSentCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
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

        auto requestJsonString = ArgConverter::ConvertToString(requestJson).c_str();
        auto protocolRequestJson = protocol::parseJSON(requestJsonString);

        protocol::ErrorSupport errorSupport;

        auto protocolResponseObj = protocol::Network::Request::parse(protocolRequestJson.get(), &errorSupport);
        auto initiator = protocol::Network::Initiator::create().setType(protocol::Network::Initiator::TypeEnum::Script).build();

        protocol::Maybe<String16> type(ArgConverter::ConvertToString(typeArg).c_str());
        protocol::Maybe<protocol::Network::Response> emptyRedirect;
        networkAgentInstance->m_frontend.requestWillBeSent(ArgConverter::ConvertToString(requestId).c_str(),
                FrameId,
                LoaderId,
                ArgConverter::ConvertToString(url).c_str(),
                std::move(protocolResponseObj),
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

static void DataForRequestId(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
        auto dataString = ArgConverter::ConvertToString(data);
        auto hasTextContentBool = hasTextContent->BooleanValue();

        auto responses = networkAgentInstance->m_responses;
        auto it = responses.find(requestIdString);

        if (it == responses.end()) {
            throw NativeScriptException("Response not found for requestId = " +
                                        ArgConverter::ConvertToString(requestId));
        } else {
            v8_inspector::utils::NetworkRequestData* response = it->second;

            if (!hasTextContentBool) {
                response->setData(dataString);
            } else {
                response->setData(ArgConverter::ConvertToString(data));
            }

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

static void LoadingFinishedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
}
}


#endif //NETWORKDOMAINCALLBACKHANDLERS_H
