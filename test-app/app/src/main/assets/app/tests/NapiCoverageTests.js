// The addons are compiled into libNativeScript.so for local Debug builds
// only; on any other runtime flavor the suite skips rather than fails.
var napiCoverageModuleAvailable = true;
try {
    require("napicoveragemodule");
} catch (e) {
    napiCoverageModuleAvailable = false;
}

(napiCoverageModuleAvailable ? describe : xdescribe)("Node-API value surface", function () {
    var napi = require("napicoveragemodule");

    it("is a separate addon from napitestmodule, cached per env", function () {
        expect(napi).not.toBe(require("napitestmodule"));
        expect(require("napicoveragemodule")).toBe(napi);
    });

    describe("versions", function () {
        it("reports the highest supported Node-API version", function () {
            expect(napi.napiVersion()).toBe(10);
        });

        it("reports the Node release the surface was vendored from", function () {
            expect(napi.nodeVersion()).toEqual({
                major: 26,
                minor: 7,
                patch: 0,
                release: "node"
            });
        });

        it("refuses the APIs that have no equivalent here", function () {
            expect(napi.unsupportedApiStatuses()).toEqual({
                uvEventLoop: "generic_failure",
                moduleFileName: "generic_failure"
            });
        });
    });

    describe("strings", function () {
        it("measures a string differently per encoding", function () {
            expect(napi.stringLengths("abc")).toEqual({ utf8: 3, utf16: 3, latin1: 3 });
            expect(napi.stringLengths("")).toEqual({ utf8: 0, utf16: 0, latin1: 0 });
            // U+2603 is three UTF-8 bytes but a single code unit.
            expect(napi.stringLengths("☃")).toEqual({ utf8: 3, utf16: 1, latin1: 1 });
            // U+1F600 is four UTF-8 bytes and a surrogate pair.
            expect(napi.stringLengths("😀")).toEqual({ utf8: 4, utf16: 2, latin1: 2 });
        });

        it("creates strings in each encoding", function () {
            var created = napi.createStrings();
            expect(created.utf8).toBe("utf8 ü ☃");
            expect(created.utf16).toBe("utf16 ü 😀");
            expect(created.latin1).toBe("Aéÿ");
            expect(created.empty).toBe("");
        });

        it("honours an explicit length when creating", function () {
            expect(napi.createStrings().utf8Sized).toBe("abc");
        });

        it("copies at most bufsize - 1 units and terminates the rest", function () {
            expect(napi.copyString("hello", "utf8", 6)).toEqual({ copied: 5, text: "hello" });
            expect(napi.copyString("hello", "utf8", 3)).toEqual({ copied: 2, text: "he" });
            expect(napi.copyString("hello", "utf8", 1)).toEqual({ copied: 0, text: "" });

            expect(napi.copyString("hello", "utf16", 6)).toEqual({ copied: 5, text: "hello" });
            expect(napi.copyString("hello", "utf16", 3)).toEqual({ copied: 2, text: "he" });

            expect(napi.copyString("héllo", "latin1", 6)).toEqual({ copied: 5, text: "héllo" });
            expect(napi.copyString("héllo", "latin1", 3)).toEqual({ copied: 2, text: "hé" });
        });

        it("reports zero copied for an empty buffer", function () {
            expect(napi.copyString("hello", "utf8", 0)).toEqual({ copied: 0, text: "" });
            expect(napi.copyString("hello", "utf16", 0)).toEqual({ copied: 0, text: "" });
        });

        it("never splits a UTF-8 sequence when truncating", function () {
            // V8 stops short of the buffer rather than emitting a partial
            // sequence, so two bytes of room hold none of the three-byte U+2603.
            expect(napi.copyString("☃x", "utf8", 3)).toEqual({ copied: 0, text: "" });
            expect(napi.copyString("☃x", "utf8", 4)).toEqual({ copied: 3, text: "☃" });
            expect(napi.copyString("☃x", "utf8", 5)).toEqual({ copied: 4, text: "☃x" });
        });

        it("rejects a non-string", function () {
            expect(napi.stringStatus("x")).toBe("ok");
            expect(napi.stringStatus(42)).toBe("string_expected");
            expect(napi.stringStatus(null)).toBe("string_expected");
            expect(napi.stringStatus({})).toBe("string_expected");
        });
    });

    describe("numbers", function () {
        it("round-trips a fractional value with truncation toward zero", function () {
            expect(napi.numberParts(1.5)).toEqual({
                double: 1.5,
                int32: 1,
                uint32: 1,
                int64: 1
            });
            expect(napi.numberParts(-1.5)).toEqual({
                double: -1.5,
                int32: -1,
                uint32: 4294967295,
                int64: -1
            });
        });

        it("wraps out-of-range values the way ToInt32/ToUint32 do", function () {
            expect(napi.numberParts(-1)).toEqual({
                double: -1,
                int32: -1,
                uint32: 4294967295,
                int64: -1
            });
            expect(napi.numberParts(2147483648)).toEqual({
                double: 2147483648,
                int32: -2147483648,
                uint32: 2147483648,
                int64: 2147483648
            });
            expect(napi.numberParts(4294967297)).toEqual({
                double: 4294967297,
                int32: 1,
                uint32: 1,
                int64: 4294967297
            });
        });

        it("turns every non-finite value into zero", function () {
            var nan = napi.numberParts(NaN);
            expect(isNaN(nan.double)).toBe(true);
            expect(nan.int32).toBe(0);
            expect(nan.uint32).toBe(0);
            expect(nan.int64).toBe(0);

            expect(napi.numberParts(Infinity)).toEqual({
                double: Infinity,
                int32: 0,
                uint32: 0,
                int64: 0
            });
            expect(napi.numberParts(-Infinity)).toEqual({
                double: -Infinity,
                int32: 0,
                uint32: 0,
                int64: 0
            });
        });

        it("creates the edges of each integer width", function () {
            var created = napi.createNumbers();
            expect(created.int32Min).toBe(-2147483648);
            expect(created.int32Max).toBe(2147483647);
            expect(created.uint32Max).toBe(4294967295);
            expect(created.int64Max).toBe(9007199254740991);
            expect(created.int64Min).toBe(-9007199254740991);
            expect(isNaN(created.nan)).toBe(true);
            expect(created.posInf).toBe(Infinity);
            expect(created.negInf).toBe(-Infinity);
            expect(Object.is(created.negZero, -0)).toBe(true);
        });

        it("rejects a non-number without coercing it", function () {
            expect(napi.numberStatus(1)).toBe("ok");
            expect(napi.numberStatus("1")).toBe("number_expected");
            expect(napi.numberStatus(true)).toBe("number_expected");
            expect(napi.numberStatus(null)).toBe("number_expected");
        });
    });

    describe("symbols", function () {
        it("creates a unique symbol carrying its description", function () {
            var symbol = napi.createSymbol("napi-desc");
            expect(typeof symbol).toBe("symbol");
            expect(symbol.description).toBe("napi-desc");
            expect(napi.createSymbol("napi-desc")).not.toBe(symbol);
        });

        it("creates a symbol without a description", function () {
            var symbol = napi.createSymbol(undefined);
            expect(typeof symbol).toBe("symbol");
            expect(symbol.description).toBeUndefined();
        });

        it("requires a string description", function () {
            expect(napi.createSymbolStatus("ok-desc")).toBe("ok");
            expect(napi.createSymbolStatus(42)).toBe("string_expected");
            expect(napi.createSymbolStatus({})).toBe("string_expected");
        });

        it("resolves registered symbols from the same table as Symbol.for", function () {
            expect(napi.symbolFor("napi.coverage.key")).toBe(Symbol.for("napi.coverage.key"));
        });

        it("uses a symbol as a property key", function () {
            var target = {};
            var key = Symbol("napi-key");
            expect(napi.symbolKeyRoundTrip(target, key, 7)).toEqual({
                hasBefore: false,
                hasAfter: true,
                read: 7,
                deleted: true,
                hasAfterDelete: false
            });
            expect(target[key]).toBeUndefined();
        });
    });

    describe("array buffers and views", function () {
        it("fills a new array buffer through its data pointer", function () {
            var buffer = napi.createArrayBuffer(8);
            expect(buffer instanceof ArrayBuffer).toBe(true);
            expect(buffer.byteLength).toBe(8);
            expect(Array.prototype.slice.call(new Uint8Array(buffer))).toEqual([
                0, 1, 2, 3, 4, 5, 6, 7
            ]);
        });

        it("reads back the same memory JS sees", function () {
            var buffer = napi.createArrayBuffer(4);
            new Uint8Array(buffer)[3] = 99;
            expect(napi.arrayBufferInfo(buffer)).toEqual({
                byteLength: 4,
                hasData: true,
                firstByte: 0,
                lastByte: 99
            });

            napi.writeByte(buffer, 0, 200);
            expect(new Uint8Array(buffer)[0]).toBe(200);
        });

        it("creates a typed array over a JS array buffer at an offset", function () {
            var buffer = new ArrayBuffer(8);
            new Uint8Array(buffer).set([0, 0, 5, 6, 7, 8, 9, 10]);

            var view = napi.createTypedArray("uint16", buffer, 3, 2);
            expect(view instanceof Uint16Array).toBe(true);
            expect(view.length).toBe(3);
            expect(view.byteOffset).toBe(2);
            expect(view.byteLength).toBe(6);
            expect(view.buffer).toBe(buffer);
        });

        it("describes a typed array from the native side", function () {
            var buffer = napi.createArrayBuffer(8);
            var view = napi.createTypedArray("uint8", buffer, 5, 3);

            var info = napi.typedArrayInfo(view);
            expect(info.type).toBe("uint8");
            expect(info.length).toBe(5);
            expect(info.byteOffset).toBe(3);
            expect(info.buffer).toBe(buffer);
            // The data pointer already has the offset applied.
            expect(info.firstByte).toBe(3);
        });

        it("keeps every element type distinguishable", function () {
            var buffer = new ArrayBuffer(8);
            expect(napi.typedArrayInfo(napi.createTypedArray("int8", buffer, 8, 0)).type).toBe(
                "int8"
            );
            expect(napi.typedArrayInfo(napi.createTypedArray("float64", buffer, 1, 0)).type).toBe(
                "float64"
            );
            expect(napi.typedArrayInfo(napi.createTypedArray("biguint64", buffer, 1, 0)).type).toBe(
                "biguint64"
            );
        });

        it("wraps the addon's own allocation without copying it", function () {
            var view = napi.createExternalTypedArray();
            expect(view instanceof Uint8Array).toBe(true);
            expect(view.byteLength).toBe(8);
            expect(Array.prototype.slice.call(view)).toEqual([10, 11, 12, 13, 14, 15, 16, 17]);

            // Writes land in the addon's buffer, which is the one JS is reading.
            napi.writeByte(view.buffer, 0, 42);
            expect(view[0]).toBe(42);
        });

        it("creates and describes a DataView", function () {
            var buffer = napi.createArrayBuffer(8);
            var view = napi.createDataView(buffer, 4, 2);
            expect(view instanceof DataView).toBe(true);
            expect(view.byteLength).toBe(4);
            expect(view.byteOffset).toBe(2);
            expect(view.buffer).toBe(buffer);
            expect(view.getUint8(0)).toBe(2);

            expect(napi.dataViewInfo(view)).toEqual({
                byteLength: 4,
                byteOffset: 2,
                buffer: buffer,
                firstByte: 2
            });
        });

        it("detaches an array buffer", function () {
            var buffer = napi.createArrayBuffer(4);
            expect(napi.detachArrayBuffer(buffer)).toEqual({
                status: "ok",
                before: false,
                after: true
            });
            expect(buffer.byteLength).toBe(0);
        });

        it("refuses to detach something that is not an array buffer", function () {
            expect(napi.detachArrayBuffer({})).toEqual({
                status: "arraybuffer_expected",
                before: false,
                after: false
            });
        });
    });

    describe("promises", function () {
        // The addon holds one deferred at a time, so a spec that fails before
        // settling would make every later createPromise() throw.
        beforeEach(function () {
            napi.settlePromise(true, undefined);
        });

        it("resolves a deferred", function (done) {
            var promise = napi.createPromise();
            expect(promise instanceof Promise).toBe(true);
            expect(napi.predicates(promise).promise).toBe(true);

            promise.then(function (value) {
                expect(value).toBe(42);
                done();
            });

            expect(napi.settlePromise(true, 42)).toBe(true);
        });

        it("rejects a deferred with the value it was given", function (done) {
            var promise = napi.createPromise();
            var reason = new Error("rejected by the addon");

            promise.then(
                function () {
                    expect("resolved").toBe("rejected");
                    done();
                },
                function (caught) {
                    expect(caught).toBe(reason);
                    done();
                }
            );

            expect(napi.settlePromise(false, reason)).toBe(true);
        });

        it("reports that there is nothing left to settle", function () {
            expect(napi.settlePromise(true, 1)).toBe(false);
        });

        it("does not see a plain thenable as a promise", function () {
            expect(napi.predicates({ then: function () {} }).promise).toBe(false);
        });

        it("sees every kind of JS promise as a promise", function () {
            expect(napi.predicates(Promise.resolve()).promise).toBe(true);
            expect(napi.predicates(new Promise(function () {})).promise).toBe(true);
            expect(napi.predicates((async function () {})()).promise).toBe(true);
        });
    });

    describe("errors", function () {
        it("creates each error type with its code", function () {
            var error = napi.createError("error", "ECODE", "plain");
            expect(error instanceof Error).toBe(true);
            expect(error.name).toBe("Error");
            expect(error.message).toBe("plain");
            expect(error.code).toBe("ECODE");

            var typeError = napi.createError("type", "ETYPE", "typed");
            expect(typeError instanceof TypeError).toBe(true);
            expect(typeError.code).toBe("ETYPE");

            var rangeError = napi.createError("range", "ERANGE", "ranged");
            expect(rangeError instanceof RangeError).toBe(true);
            expect(rangeError.code).toBe("ERANGE");

            var syntaxError = napi.createError("syntax", "ESYNTAX", "syntactic");
            expect(syntaxError instanceof SyntaxError).toBe(true);
            expect(syntaxError.code).toBe("ESYNTAX");
        });

        it("leaves out the code when none is given", function () {
            var error = napi.createError("type", null, "no code");
            expect(error instanceof TypeError).toBe(true);
            expect(error.message).toBe("no code");
            expect("code" in error).toBe(false);
        });

        it("requires a string message", function () {
            expect(napi.createErrorStatus("fine")).toBe("ok");
            expect(napi.createErrorStatus(42)).toBe("string_expected");
        });

        it("throws each error type", function () {
            var caught;
            try {
                napi.throwErrorKind("type", "ETHROWN", "thrown type error");
            } catch (e) {
                caught = e;
            }
            expect(caught instanceof TypeError).toBe(true);
            expect(caught.message).toBe("thrown type error");
            expect(caught.code).toBe("ETHROWN");

            caught = undefined;
            try {
                napi.throwErrorKind("range", null, "thrown range error");
            } catch (e) {
                caught = e;
            }
            expect(caught instanceof RangeError).toBe(true);
            expect(caught.message).toBe("thrown range error");
            expect("code" in caught).toBe(false);

            caught = undefined;
            try {
                napi.throwErrorKind("syntax", null, "thrown syntax error");
            } catch (e) {
                caught = e;
            }
            expect(caught instanceof SyntaxError).toBe(true);
        });

        it("throws a value that is not an Error", function () {
            var caught;
            try {
                napi.throwValue("just a string");
            } catch (e) {
                caught = e;
            }
            expect(caught).toBe("just a string");

            var sentinel = { tag: "thrown object" };
            caught = undefined;
            try {
                napi.throwValue(sentinel);
            } catch (e) {
                caught = e;
            }
            expect(caught).toBe(sentinel);
        });

        it("recognises errors and only errors", function () {
            expect(napi.predicates(new Error("x")).error).toBe(true);
            expect(napi.predicates(new TypeError("x")).error).toBe(true);
            expect(napi.predicates({ message: "x" }).error).toBe(false);
            expect(napi.predicates("x").error).toBe(false);
        });
    });

    describe("exceptions", function () {
        it("surfaces, blocks on, and clears a pending exception", function () {
            var result = napi.callAndCatch(function () {
                throw new Error("boom");
            });

            expect(result.callStatus).toBe("pending_exception");
            expect(result.pendingBefore).toBe(true);
            // A second call refuses to run while the first exception is pending.
            expect(result.blockedStatus).toBe("pending_exception");
            expect(result.clearStatus).toBe("ok");
            expect(result.pendingAfter).toBe(false);
            expect(result.caught instanceof Error).toBe(true);
            expect(result.caught.message).toBe("boom");
        });

        it("leaves nothing pending after a clean call", function () {
            var result = napi.callAndCatch(function () {
                return 1;
            });

            expect(result.callStatus).toBe("ok");
            expect(result.pendingBefore).toBe(false);
            expect(result.blockedStatus).toBe("ok");
            expect(result.pendingAfter).toBe(false);
            expect(result.caught).toBeUndefined();
        });

        it("clears to undefined when nothing was thrown", function () {
            expect(napi.clearWithoutException()).toEqual({ status: "ok", isUndefined: true });
        });

        it("rethrows the identical value it caught", function () {
            var sentinel = new Error("rethrown");
            var caught;
            try {
                napi.callAndRethrow(function () {
                    throw sentinel;
                });
            } catch (e) {
                caught = e;
            }
            expect(caught).toBe(sentinel);
        });
    });

    describe("references", function () {
        it("counts up and down and hands back the value", function () {
            var target = { id: "counted" };
            var ref = napi.refCreate(target, 1);

            expect(napi.refGet(ref)).toBe(target);
            expect(napi.refRef(ref)).toBe(2);
            expect(napi.refRef(ref)).toBe(3);
            expect(napi.refUnref(ref)).toBe(2);
            expect(napi.refUnref(ref)).toBe(1);
            expect(napi.refUnref(ref)).toBe(0);
            expect(napi.refGet(ref)).toBe(target);

            expect(napi.refDelete(ref)).toBe(true);
            expect(napi.refDelete(ref)).toBe(false);
        });

        it("only references objects, functions and symbols at module API version 8", function () {
            expect(napi.refCreateStatus({})).toBe("ok");
            expect(napi.refCreateStatus(function () {})).toBe("ok");
            expect(napi.refCreateStatus(Symbol("ref"))).toBe("ok");

            expect(napi.refCreateStatus("a string")).toBe("invalid_arg");
            expect(napi.refCreateStatus(42)).toBe("invalid_arg");
            expect(napi.refCreateStatus(true)).toBe("invalid_arg");
            expect(napi.refCreateStatus(null)).toBe("invalid_arg");
            expect(napi.refCreateStatus(undefined)).toBe("invalid_arg");
        });

        it("keeps a weak reference alive once it is strengthened", function () {
            var target = { tag: "strengthened" };
            var ref = napi.refCreate(target, 0);
            expect(napi.refRef(ref)).toBe(1);
            target = null;

            // Conservative stack scanning keeps the dropped value alive until
            // the loop below overwrites the frame that held it.
            __collect();
            var sink = 0;
            for (var i = 0; i < 200000; i++) {
                sink += i % 7;
            }
            __collect();

            expect(sink).toBeGreaterThan(0);
            expect(napi.refGet(ref).tag).toBe("strengthened");
            expect(napi.refDelete(ref)).toBe(true);
        });

        it("drops a weak reference once its value is collected", function (done) {
            var target = { tag: "weak" };
            var ref = napi.refCreate(target, 0);
            // Deliberately not refGet: handing the object back to JS would put
            // it on the stack again, where conservative scanning keeps it alive.
            expect(napi.refIsLive(ref)).toBe(true);
            target = null;

            __collect();
            var sink = 0;
            for (var i = 0; i < 200000; i++) {
                sink += i % 7;
            }
            __collect();

            expect(sink).toBeGreaterThan(0);

            setTimeout(function () {
                expect(napi.refIsLive(ref)).toBe(false);
                expect(napi.refGet(ref)).toBeUndefined();
                expect(napi.refDelete(ref)).toBe(true);
                done();
            }, 0);
        });
    });

    describe("conversions", function () {
        it("coerces to boolean", function () {
            expect(napi.coerce("bool", 0)).toBe(false);
            expect(napi.coerce("bool", NaN)).toBe(false);
            expect(napi.coerce("bool", "")).toBe(false);
            expect(napi.coerce("bool", null)).toBe(false);
            expect(napi.coerce("bool", undefined)).toBe(false);
            expect(napi.coerce("bool", "x")).toBe(true);
            expect(napi.coerce("bool", {})).toBe(true);
        });

        it("coerces to number", function () {
            expect(napi.coerce("number", "42")).toBe(42);
            expect(napi.coerce("number", "")).toBe(0);
            expect(napi.coerce("number", true)).toBe(1);
            expect(napi.coerce("number", null)).toBe(0);
            expect(isNaN(napi.coerce("number", undefined))).toBe(true);
            expect(isNaN(napi.coerce("number", "not a number"))).toBe(true);
        });

        it("coerces to string", function () {
            expect(napi.coerce("string", 42)).toBe("42");
            expect(napi.coerce("string", null)).toBe("null");
            expect(napi.coerce("string", undefined)).toBe("undefined");
            expect(napi.coerce("string", true)).toBe("true");
            expect(napi.coerce("string", [1, 2])).toBe("1,2");
            expect(napi.coerce("string", {})).toBe("[object Object]");
        });

        it("coerces to object", function () {
            var boxed = napi.coerce("object", 42);
            expect(typeof boxed).toBe("object");
            expect(boxed instanceof Number).toBe(true);
            expect(boxed.valueOf()).toBe(42);

            expect(napi.coerce("object", "s") instanceof String).toBe(true);
            expect(napi.coerce("object", true) instanceof Boolean).toBe(true);

            var target = { already: "an object" };
            expect(napi.coerce("object", target)).toBe(target);
        });

        it("propagates the TypeError a coercion throws", function () {
            expect(function () {
                napi.coerce("number", Symbol("no number"));
            }).toThrowError(TypeError);

            expect(function () {
                napi.coerce("object", null);
            }).toThrowError(TypeError);
        });
    });

    describe("properties", function () {
        it("distinguishes own from inherited", function () {
            var own = { a: 1 };
            expect(napi.propertyOps(own, "a")).toEqual({
                has: true,
                hasOwn: true,
                read: 1,
                deleted: true,
                hasAfterDelete: false
            });

            var child = Object.create({ p: 5 });
            expect(napi.propertyOps(child, "p")).toEqual({
                has: true,
                hasOwn: false,
                read: 5,
                // Deleting a non-own property succeeds without touching the
                // prototype, so the lookup still finds it.
                deleted: true,
                hasAfterDelete: true
            });
        });

        it("reports a missing property", function () {
            expect(napi.propertyOps({}, "missing")).toEqual({
                has: false,
                hasOwn: false,
                read: undefined,
                deleted: true,
                hasAfterDelete: false
            });
        });

        it("operates on array elements by index", function () {
            var values = [10, 20, 30];
            expect(napi.elementOps(values, 1)).toEqual({
                has: true,
                read: 20,
                deleted: true,
                hasAfterDelete: false
            });
            expect(values.length).toBe(3);
            expect(values[1]).toBeUndefined();
        });

        it("lists enumerable string keys including inherited ones", function () {
            var target = Object.create({ inherited: 1 });
            target.own = 2;
            Object.defineProperty(target, "hidden", { value: 3, enumerable: false });
            target[Symbol("skipped")] = 4;

            expect(napi.propertyNames(target)).toEqual(["own", "inherited"]);
        });

        it("filters keys with napi_get_all_property_names", function () {
            var target = Object.create({ inherited: 1 });
            target.own = 2;
            Object.defineProperty(target, "hidden", { value: 3, enumerable: false });
            var symbolKey = Symbol("kept");
            target[symbolKey] = 4;

            var ownAll = napi.allPropertyNames(target, true, "all");
            expect(ownAll.indexOf("own")).toBeGreaterThan(-1);
            expect(ownAll.indexOf("hidden")).toBeGreaterThan(-1);
            expect(ownAll.indexOf(symbolKey)).toBeGreaterThan(-1);
            expect(ownAll.indexOf("inherited")).toBe(-1);

            var ownStrings = napi.allPropertyNames(target, true, "skip_symbols");
            expect(ownStrings.indexOf(symbolKey)).toBe(-1);
            expect(ownStrings.indexOf("hidden")).toBeGreaterThan(-1);

            var enumerable = napi.allPropertyNames(target, false, "enumerable");
            expect(enumerable.indexOf("own")).toBeGreaterThan(-1);
            expect(enumerable.indexOf("inherited")).toBeGreaterThan(-1);
            expect(enumerable.indexOf("hidden")).toBe(-1);

            var symbolsOnly = napi.allPropertyNames(target, true, "skip_strings");
            expect(symbolsOnly).toEqual([symbolKey]);
        });

        it("keeps or stringifies index keys per the conversion mode", function () {
            var keys = napi.indexKeyTypes([1, 2]);
            expect(typeof keys.kept[0]).toBe("number");
            expect(keys.kept[0]).toBe(0);
            expect(typeof keys.converted[0]).toBe("string");
            expect(keys.converted[0]).toBe("0");
            expect(keys.kept.length).toBe(keys.converted.length);
        });

        it("defines an accessor pair sharing one callback", function () {
            var target = napi.defineOnTarget({});

            target.base = 5;
            expect(target.base).toBe(5);
            // Same getter, different descriptor `data`.
            expect(target.offset).toBe(105);

            var base = Object.getOwnPropertyDescriptor(target, "base");
            expect(typeof base.get).toBe("function");
            expect(typeof base.set).toBe("function");
            expect(base.enumerable).toBe(true);

            var offset = Object.getOwnPropertyDescriptor(target, "offset");
            expect(typeof offset.get).toBe("function");
            expect(offset.set).toBeUndefined();
        });

        it("maps napi_property_attributes onto the descriptor", function () {
            var target = napi.defineOnTarget({});

            expect(Object.getOwnPropertyDescriptor(target, "locked")).toEqual({
                value: 1,
                writable: false,
                enumerable: false,
                configurable: false
            });
            expect(Object.getOwnPropertyDescriptor(target, "open")).toEqual({
                value: 1,
                writable: true,
                enumerable: true,
                configurable: true
            });
        });
    });

    describe("types and identity", function () {
        it("names every value type", function () {
            expect(napi.typeOf(undefined)).toBe("undefined");
            expect(napi.typeOf(null)).toBe("null");
            expect(napi.typeOf(true)).toBe("boolean");
            expect(napi.typeOf(1)).toBe("number");
            expect(napi.typeOf("s")).toBe("string");
            expect(napi.typeOf(Symbol("s"))).toBe("symbol");
            expect(napi.typeOf({})).toBe("object");
            expect(napi.typeOf([])).toBe("object");
            expect(napi.typeOf(function () {})).toBe("function");
            expect(napi.typeOf(BigInt(1))).toBe("bigint");
            expect(napi.typeOf(napi.createExternal(1))).toBe("external");
        });

        it("classifies built-in object kinds", function () {
            expect(napi.predicates([]).array).toBe(true);
            expect(napi.predicates({ length: 0 }).array).toBe(false);
            expect(napi.predicates(new Date()).date).toBe(true);
            expect(napi.predicates(new ArrayBuffer(1)).arraybuffer).toBe(true);
            expect(napi.predicates(new Uint8Array(1)).typedarray).toBe(true);
            expect(napi.predicates(new DataView(new ArrayBuffer(1))).dataview).toBe(true);
            expect(napi.predicates(new ArrayBuffer(1)).typedarray).toBe(false);
        });

        it("treats a Uint8Array, and only a Uint8Array, as a buffer", function () {
            // There is no node::Buffer here, so napi_is_buffer is exactly
            // "is this a Uint8Array".
            expect(napi.predicates(new Uint8Array(1)).buffer).toBe(true);
            expect(napi.predicates(new Int8Array(1)).buffer).toBe(false);
            expect(napi.predicates(new ArrayBuffer(1)).buffer).toBe(false);
        });

        it("compares with strict equality semantics", function () {
            var target = {};
            expect(napi.strictEquals(target, target)).toBe(true);
            expect(napi.strictEquals({}, {})).toBe(false);
            expect(napi.strictEquals("a", "a")).toBe(true);
            expect(napi.strictEquals(1, "1")).toBe(false);
            expect(napi.strictEquals(NaN, NaN)).toBe(false);
            expect(napi.strictEquals(0, -0)).toBe(true);
        });

        it("walks the prototype chain for instanceof", function () {
            expect(napi.instanceOf(new TypeError("x"), Error)).toBe(true);
            expect(napi.instanceOf(new Error("x"), TypeError)).toBe(false);
            expect(napi.instanceOf([], Array)).toBe(true);
            expect(napi.instanceOf({}, Error)).toBe(false);
        });

        it("constructs and reads a prototype", function () {
            function Point(x) {
                this.x = x;
            }

            var point = napi.newInstance(Point, 7);
            expect(point instanceof Point).toBe(true);
            expect(point.x).toBe(7);
            expect(napi.getPrototype(point)).toBe(Point.prototype);
            expect(napi.getPrototype({})).toBe(Object.prototype);
        });

        it("freezes and seals", function () {
            var frozen = napi.freezeObject({ a: 1 });
            expect(Object.isFrozen(frozen)).toBe(true);
            frozen.a = 2;
            expect(frozen.a).toBe(1);

            var sealed = napi.sealObject({ a: 1 });
            expect(Object.isSealed(sealed)).toBe(true);
            expect(Object.isFrozen(sealed)).toBe(false);
            sealed.a = 2;
            expect(sealed.a).toBe(2);
            sealed.b = 3;
            expect(sealed.b).toBeUndefined();
        });

        it("round-trips a date", function () {
            var date = napi.createDate(1234567);
            expect(date instanceof Date).toBe(true);
            expect(date.getTime()).toBe(1234567);
            expect(napi.dateValue(date)).toBe(1234567);
            expect(napi.predicates(date).date).toBe(true);
            expect(napi.dateValue({})).toBe("date_expected");
        });

        it("round-trips an external pointer", function () {
            var external = napi.createExternal(2.5);
            expect(napi.externalValue(external)).toBe(2.5);
            expect(napi.externalValue({})).toBe("invalid_arg");
        });
    });
});
