//
// Created by pkanev on 3/8/2017.
//

#ifndef NETWORKDOMAINCALLBACKHANDLERS_H
#define NETWORKDOMAINCALLBACKHANDLERS_H


#include <include/v8.h>
#include <v8_inspector/src/inspector/v8-network-agent-impl.h>
#include "JsV8InspectorClient.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"

namespace tns {
class NetworkDomainCallbackHandlers {

    public:
        static const char* FrameId;
        static const char* LoaderId;

        /*
         * Fired when device is about to send HTTP request.
         * @param args - JS object : { requestId, timestamp, type, response }
         */
        static void ResponseReceivedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * Fired when HTTP response is available.
         * @param args - JS object { requestId, url, request, timestamp, type }
         */
        static void RequestWillBeSentCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * Called after response is received to save the response data for the provided requestId
         * @param args - JS object { requestId, data, hasTextContent }
         */
        static void DataForRequestIdCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * Fired when HTTP request has finished loading.
         * @param args - JS object { requestId, timestamp }
         */
        static void LoadingFinishedCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
};
}


#endif //NETWORKDOMAINCALLBACKHANDLERS_H
