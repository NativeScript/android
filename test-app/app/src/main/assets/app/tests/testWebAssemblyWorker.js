self.onmessage = function(msg) {
    let wasmCode = new Uint8Array(msg.data);

    let importsObj = {
        env: {
            logarithm: Math.log
        }
    };

    WebAssembly.compile(wasmCode).then(wasmModule => {
        WebAssembly.instantiate(wasmModule, importsObj).then(moduleInstance => {
            let result = moduleInstance.exports.log(Math.E);
            self.postMessage(result);
        });
    });
}
