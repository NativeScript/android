//
// Created by pkanev on 4/24/2017.
//

#include <NativeScriptAssert.h>
#include "v8-dom-agent-impl.h"
#include <ArgConverter.h>
#include <Runtime.h>

namespace v8_inspector {

    using tns::Runtime;
    using tns::ArgConverter;

    namespace DOMAgentState {
        static const char domEnabled[] = "domEnabled";
    }

    V8DOMAgentImpl::V8DOMAgentImpl(V8InspectorSessionImpl *session,
                                   protocol::FrontendChannel *frontendChannel,
                                   protocol::DictionaryValue *state)
            : m_session(session),
              m_frontend(frontendChannel),
              m_state(state),
              m_enabled(false) {
        Instance = this;
    }

    V8DOMAgentImpl::~V8DOMAgentImpl() { }

    void V8DOMAgentImpl::enable(ErrorString*) {
        if (m_enabled) {
            return;
        }

        m_state->setBoolean(DOMAgentState::domEnabled, true);

        m_enabled = true;
    }

    void V8DOMAgentImpl::disable(ErrorString*) {
        if (!m_enabled) {
            return;
        }

        m_state->setBoolean(DOMAgentState::domEnabled, false);

        m_enabled = false;
    }

    void V8DOMAgentImpl::getDocument(ErrorString *errorString, std::unique_ptr<protocol::DOM::Node>* out_root) {
        std::unique_ptr<protocol::DOM::Node> defaultNode = protocol::DOM::Node::create()
                .setNodeId(0)
                .setNodeType(9)
                .setNodeName("Frame")
                .setLocalName("Frame")
                .setNodeValue("")
                .build();

        *out_root = std::move(defaultNode);
    }

    void V8DOMAgentImpl::removeNode(ErrorString*, int in_nodeId) {
        // TODO: Pete: call modules' View class methods to remove view from parent view
    }

    // Pete: return empty resolved object - prevents crashes when opening the 'properties', 'event listeners' tabs
    void V8DOMAgentImpl::resolveNode(ErrorString*, int in_nodeId, const Maybe<String>& in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_object) {
        auto resolvedNode = protocol::Runtime::RemoteObject::create()
                .setType("View")
                .build();

        *out_object = std::move(resolvedNode);
    }

    V8DOMAgentImpl* V8DOMAgentImpl::Instance = 0;

    void V8DOMAgentImpl::setAttributeValue(ErrorString *, int in_nodeId, const String &in_name,
                                           const String &in_value) {
        // TODO: Pete: call modules' View class methods to set view's attribute
    }

    void V8DOMAgentImpl::setAttributesAsText(ErrorString *, int in_nodeId, const String &in_text,
                                             const Maybe<String> &in_name) {
        // TODO: Pete: call modules' View class methods to set view's attribute
    }

    void V8DOMAgentImpl::removeAttribute(ErrorString *, int in_nodeId, const String &in_name) {
        // TODO: Pete: call modules' View class methods to modify view's attribute
    }

    void V8DOMAgentImpl::performSearch(ErrorString *, const String &in_query,
                                       const Maybe<protocol::Array<int>> &in_nodeIds,
                                       String *out_searchId, int *out_resultCount) {

    }

    void
    V8DOMAgentImpl::getSearchResults(ErrorString *, const String &in_searchId, int in_fromIndex,
                                     int in_toIndex,
                                     std::unique_ptr<protocol::Array<int>> *out_nodeIds) {

    }

    void V8DOMAgentImpl::discardSearchResults(ErrorString *, const String &in_searchId) {

    }

    void V8DOMAgentImpl::highlightNode(ErrorString *,
                                       std::unique_ptr<protocol::DOM::HighlightConfig> in_highlightConfig,
                                       const Maybe<int> &in_nodeId,
                                       const Maybe<String> &in_objectId) {

    }

    void V8DOMAgentImpl::hideHighlight(ErrorString *) {

    }
}
