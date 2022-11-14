describe("Test WebAssembly ", () => {
    // https://wasdk.github.io/WasmFiddle/?15acre
    //
    // #include <stdio.h>
    // #include <sys/uio.h>
    //
    // #define WASM_EXPORT __attribute__((visibility("default")))
    //
    // extern double logarithm(double value);
    //
    // WASM_EXPORT int log(double value) {
    //   return logarithm(value);
    // }
    let wasmCode = new Uint8Array([
        0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x8b, 0x80, 0x80, 0x80, 0x00, 0x02,
        0x60, 0x01, 0x7c, 0x01, 0x7c, 0x60, 0x01, 0x7c, 0x01, 0x7f, 0x02, 0x91, 0x80, 0x80, 0x80,
        0x00, 0x01, 0x03, 0x65, 0x6e, 0x76, 0x09, 0x6c, 0x6f, 0x67, 0x61, 0x72, 0x69, 0x74, 0x68,
        0x6d, 0x00, 0x00, 0x03, 0x82, 0x80, 0x80, 0x80, 0x00, 0x01, 0x01, 0x04, 0x84, 0x80, 0x80,
        0x80, 0x00, 0x01, 0x70, 0x00, 0x00, 0x05, 0x83, 0x80, 0x80, 0x80, 0x00, 0x01, 0x00, 0x01,
        0x06, 0x81, 0x80, 0x80, 0x80, 0x00, 0x00, 0x07, 0x90, 0x80, 0x80, 0x80, 0x00, 0x02, 0x06,
        0x6d, 0x65, 0x6d, 0x6f, 0x72, 0x79, 0x02, 0x00, 0x03, 0x6c, 0x6f, 0x67, 0x00, 0x01, 0x0a,
        0x8d, 0x80, 0x80, 0x80, 0x00, 0x01, 0x87, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x00, 0x10,
        0x00, 0xaa, 0x0b
    ]);

    it("Handle compilation failures", done => {
        WebAssembly.compile(new Uint8Array([ 1, 2, 3, 4 ])).then(moduleInstance => {
            expect(true).toBe(false, "The success callback of the compilation promise was called");
            done();
        }).catch(e => {
            expect(e.name).toEqual("CompileError");
            expect(e.message).toEqual("WebAssembly.compile(): expected magic word 00 61 73 6d, found 01 02 03 04 @+0");
            done();
        });
    });

	it("Compile and instantiate a WebAssembly module asynchronously", done => {
        let importsObj = {
            env: {
                logarithm: Math.log
            }
        };

        WebAssembly.compile(wasmCode).then(wasmModule => {
            WebAssembly.instantiate(wasmModule, importsObj).then(moduleInstance => {
                expect(moduleInstance).toBeDefined();
                expect(moduleInstance.exports).toBeDefined();
                expect(moduleInstance.exports.log).toEqual(jasmine.any(Function));
                let actual = moduleInstance.exports.log(Math.E);
                expect(actual).toEqual(1);
                done();
            }).catch(e => {
                expect(true).toBe(false, "An unexpected error occurred while instantiating the WebAssembly module: " + e.toString());
                done();
            });
        }).catch(e => {
            expect(true).toBe(false, "An unexpected error occurred while compiling the WebAssembly module: " + e.toString());
            done();
        });
	});

	it("Compile and instantiate a WebAssembly module inside a worker", done => {
        let worker = new Worker("./testWebAssemblyWorker");

        worker.onmessage = msg => {
            expect(msg.data).toEqual(1);
            worker.terminate();
            done();
        };

        worker.postMessage(Array.from(wasmCode));
	});
});
