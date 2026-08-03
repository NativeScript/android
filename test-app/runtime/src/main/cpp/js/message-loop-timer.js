const { messageLoopTimerStart, messageLoopTimerStop } = binding;

// We proxy the WebAssembly's compile, compileStreaming, instantiate and
// instantiateStreaming methods so that they can start and stop a
// MessageLoopTimer inside the promise callbacks. This timer will call
// the v8::platform::PumpMessageLoop method at regular intervals.
// https://github.com/NativeScript/android-runtime/issues/1558

global.WebAssembly = new Proxy(WebAssembly, {
    get: (target, name) => {
        let origMethod = target[name];
        let proxyMethods = [
            "compile",
            "compileStreaming",
            "instantiate",
            "instantiateStreaming"
        ];

        if (proxyMethods.indexOf(name) < 0) {
            return origMethod;
        }

        return function (...args) {
            messageLoopTimerStart();
            let result = origMethod.apply(this, args);
            return result.then(x => {
                messageLoopTimerStop();
                return x;
            }).catch(e => {
                messageLoopTimerStop();
                throw e;
            });
        };
    }
});
