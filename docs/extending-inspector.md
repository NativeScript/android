# How to extend the V8 inspector

## Overview

> Note: Check out the blog post [Chrome DevTools Integration from a Technical Perspective](https://www.nativescript.org/blog/chrome-devtools-integration) for an overview of how the v8 inspector in NativeScript works.

The default inspector of the V8 project comes with only a [handful of implemented inspector domains](https://chromedevtools.github.io/devtools-protocol/v8). This is more than enough to be able to debug JavaScript, but what if you want to take it a step further and enable inspection of any arbitrary files of your application, need to spoof the network requests, or inspect the visual elements?

## Getting additional protocol domain definitions

This step involves fetching and building the V8 projectory, explanation for which can be found at [Building V8 with GN](https://github.com/v8/v8/wiki/Building-with-GN). 

> Note: When passing arguments to gn to have a project generate, make sure to include the inspector sources. For an overview of all available gn arguments run `gn args project-dir --list`

1. Modify `v8/src/inspector/js_protocol.json` to only contain the domains that will be used in the runtime project. The right way to go about this is to copy-paste the `js_protocol` from the runtime project, and add on top of it. For a complete list of all protocol domains supported by the Chrome browser refer to [browser_protocol.json](https://chromium.googlesource.com/chromium/src/+log/master/third_party/WebKit/Source/core/inspector/browser_protocol.json) in the Chromium project. Example browser_protocol.json file can be found [here](https://chromedevtools.github.io/devtools-protocol/tot/) 
2. Modify `v8/src/inspector/inspector_protocol_config.json` and include the names for the additional protocol domain definitions that would need to be generated.
```json
{
    "protocol": {
        ...
        "options": [
            {
                "domain": "Schema",
                "exported": ["Domain"]
            },
            ...
            {
                "domain": "Console"
            },
            {
                "domain": "Log"
            },
            {
                "domain": "Overlay"
            }
        ]
    },

    "exported": {
        ...
    },

    "lib": {
        ...
    }
}
```
3. Figure out which parts of the big `js_inspector.json` protocol you will want to keep for the inspector implementation. It is likely that a lot of it will be browser-specific, so you don't need to spend extra time doing stub implementations for methods and events the application will not be making notifications for.

4. Run the ninja build. Upon completion, the inspector protocol files would be at `project-dir/arch.release/gen/src/inspector/protocol`.

5. Copy-Paste all `.cpp` and `.h` files in the runtime project at `runtime/src/main/cpp/v8_inspector/src/inspector/protocol`

6. Create a new C++ class extending the desired Domain (e.g. DOM). Name it according to the convention already established by the V8 team - `v8-<domain>-agent-impl.h/cpp` - See [v8-dom-agent-impl.h](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-dom-agent-impl.h#L18). Implement the Backend::<Domain>'s methods in the `.cpp` file.

7. To implement event handlers of a certain domain, check out <Protocol>DomainCallbackHandlers ([DOMDomainCallbackHandlers](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/DOMDomainCallbackHandlers.h#L14)) which are registered in [JsV8InspectorClient.cpp](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/JsV8InspectorClient.cpp#L237)

8. Register the newly created agent implementations in [v8-inspector-session-impl.h/cc](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.h#L19) - `V8InspectorSessionImpl` class:

8.1. Changes in v8-inspector-session-impl.`h`
 - `#include "src/inspector/protocol/<Domain>.h`
 - add forward class declaration of the agent implementation - `class V8<Domain>AgentImpl;`
 - create a private `std::unique_ptr<V8<Domain>AgentImpl> m_<domain>Agent;` field
 - create a public getter method `<domain>Agent()`

8.2. Changes in v8-inspector-session-impl.`cc`
 - `#include "src/inspector/protocol/v8-<domain>-agent-impl.h`
 - register protocol domain command prefix in [canDispatchMethod()](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L31)
 - in the V8InspectorSessionImpl constructor initialize the new agent implementation with a [nullptr](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L86)
 - [create a unique pointer wrapper for your new agent impl instance](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L141), and call the static domain dispatcher's [wire](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L143) method
 - make sure to call [agent.disable](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L167) in V8InspectorSessionImpl's destructor.
 - register the newly implemented domain as a supported one inside [ V8InspectorSessionImpl::supportedDomainsImpl()](https://github.com/NativeScript/android-runtime/blob/5a04e09439e2bc6a201577895b9ac6538441e758/test-app/runtime/src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc#L389)

9. Don't forget to add the new classes to the CMakeLists!
10. Test whether the new domain agent is registered by opening Chrome DevTools. In order to debug the Chrome DevTools frontend, you could open the Chrome DevTools inside an Android Chrome DevTools session - Ctrl(Cmd) + Shift + I.
