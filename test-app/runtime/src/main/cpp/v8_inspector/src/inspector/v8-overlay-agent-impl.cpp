//
// Created by pkanev on 11/15/2017.
//

#include <ArgConverter.h>
#include <v8_inspector/src/inspector/utils/v8-inspector-common.h>
#include "v8-overlay-agent-impl.h"

namespace v8_inspector {

using tns::ArgConverter;

namespace OverlayAgentState {
static const char overlayEnabled[] = "overlayEnabled";
}

V8OverlayAgentImpl::V8OverlayAgentImpl(V8InspectorSessionImpl* session,
                                       protocol::FrontendChannel* frontendChannel,
                                       protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state),
      m_enabled(false) {
    Instance = this;
}

V8OverlayAgentImpl::~V8OverlayAgentImpl() { }

DispatchResponse V8OverlayAgentImpl::enable() {
    if (m_enabled) {
        return DispatchResponse::Error("Overlay Agent already enabled!");
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, true);
    m_enabled = true;

    return DispatchResponse::OK();
}

DispatchResponse V8OverlayAgentImpl::disable() {
    if (!m_enabled) {
        return DispatchResponse::OK();
    }

    m_state->setBoolean(OverlayAgentState::overlayEnabled, false);

    m_enabled = false;

    return DispatchResponse::OK();
}

DispatchResponse V8OverlayAgentImpl::setShowFPSCounter(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setPausedInDebuggerMessage(const Maybe<String> in_message) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setShowAdHighlights(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig,
        Maybe<int> in_nodeId,
        Maybe<int> in_backendNodeId,
        Maybe<String> in_objectId,
        Maybe<String> in_selector) {

    std::string highlightNodeFunctionString = "highlightNode";
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> highlightNode;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, highlightNodeFunctionString)).ToLocal(&highlightNode);

        if (!highlightNode.IsEmpty() && highlightNode->IsFunction()) {
            auto highlightNodeFunc = highlightNode.As<v8::Function>();

            v8::Local<v8::Value> args[] = {
                    v8::Number::New(isolate, in_nodeId.fromMaybe(-1)),
                    v8_inspector::toV8String(isolate, in_selector.fromMaybe(""))
            };
            v8::TryCatch tc(isolate);

            highlightNodeFunc->Call(context, global, 2, args);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(highlightNodeFunctionString, tc.Message()->Get()).c_str();
                return DispatchResponse::Error(error);
            }

            return DispatchResponse::OK();
        }
    }

    return DispatchResponse::Error("Could not highlight node. Global Inspector object not found.");
}

DispatchResponse V8OverlayAgentImpl::highlightFrame(const String& in_frameId,
        Maybe<protocol::DOM::RGBA> in_contentColor,
        Maybe<protocol::DOM::RGBA> in_contentOutlineColor) {



    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::hideHighlight() {

    std::string hideHighlightFunctionString = "hideHighlight";
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> hideHighlight;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, hideHighlightFunctionString)).ToLocal(&hideHighlight);

        if (!hideHighlight.IsEmpty() && hideHighlight->IsFunction()) {
            auto highlightNodeFunc = hideHighlight.As<v8::Function>();

            v8::TryCatch tc(isolate);

            highlightNodeFunc->Call(context, global, 0, NULL);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(hideHighlightFunctionString, tc.Message()->Get()).c_str();
                return DispatchResponse::Error(error);
            }

            return DispatchResponse::OK();
        }
    }


    return DispatchResponse::Error("Could not hide highlight. Global Inspector object not found.");
}

DispatchResponse V8OverlayAgentImpl::getHighlightObjectForTest(int in_nodeId,
        std::unique_ptr<protocol::DictionaryValue>* out_highlight) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::highlightQuad(std::unique_ptr<protocol::Array<double>> in_quad, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::highlightRect(int in_x, int in_y, int in_width, int in_height, Maybe<protocol::DOM::RGBA> in_color, Maybe<protocol::DOM::RGBA> in_outlineColor) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setInspectMode(const String& in_mode, Maybe<protocol::Overlay::HighlightConfig> in_highlightConfig) {
    std::string setInspectModeFunctionString = "setInspectMode";
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    auto global = context->Global();

    auto globalInspectorObject = utils::Common::getGlobalInspectorObject(isolate);

    if (!globalInspectorObject.IsEmpty()) {
        v8::Local<v8::Value> setInspectMode;
        globalInspectorObject->Get(context, ArgConverter::ConvertToV8String(isolate, setInspectModeFunctionString)).ToLocal(&setInspectMode);

        if (!setInspectMode.IsEmpty() && setInspectMode->IsFunction()) {
            auto setInspectModeFunc = setInspectMode.As<v8::Function>();
            v8::Local<v8::Value> args[] = { v8_inspector::toV8String(isolate, in_mode) };
            v8::TryCatch tc(isolate);

            setInspectModeFunc->Call(context, global, 1, args);

            if (tc.HasCaught()) {
                auto error = utils::Common::getJSCallErrorMessage(setInspectModeFunctionString, tc.Message()->Get()).c_str();
                return DispatchResponse::Error(error);
            }

            return DispatchResponse::OK();
        }
    }

    return DispatchResponse::Error("Could not set inspect mode. Global Inspector object not found.");
}

DispatchResponse V8OverlayAgentImpl::setShowDebugBorders(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setShowPaintRects(bool in_result) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setShowScrollBottleneckRects(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setShowHitTestBorders(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setShowViewportSizeOnResize(bool in_show) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

DispatchResponse V8OverlayAgentImpl::setSuspended(bool in_suspended) {
    return utils::Common::protocolCommandNotSupportedDispatchResponse();
}

V8OverlayAgentImpl* V8OverlayAgentImpl::Instance = 0;
}
