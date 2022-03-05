describe("WeakRef", function () {
    it("should exist", function () {
        expect(WeakRef).toBeDefined();
    });

    // deref/get tests have been removed since they are now coming from v8 and not our code
    // we can safely assume it's well-tested.
    // we only check to make sure we have a `get` alias to `deref` since NativeScript
    // has used `get()` long before `deref()` was standardized.
    it("get should work", function () {
        expect(WeakRef.prototype.get).toBeDefined();
        expect(WeakRef.prototype.get).toEqual(WeakRef.prototype.deref);
    });

    it("should throw when constructed with zero parameters", function () {
        expect(function () {
            new WeakRef();
        }).toThrow();
    });

    it("should throw when constructed with primitive parameters", function () {
        for (var primitive of [null, undefined, 0]) {
            expect(function () {
                new WeakRef(primitive);
            }).toThrow();
        }
    });

    it("clear should exist", function () {
        expect(WeakRef.prototype.clear).toBeDefined();

        const warn = console.warn
        console.warn = (message) => {
            warn(message);
            expect(message).toEqual("WeakRef.clear() is non-standard and has been deprecated. It does nothing and the call can be safely removed.")
        }

        const obj = {}
        const weakRef = new WeakRef(obj);

        weakRef.clear();

        // reset console.warn to it's original
        console.warn = warn;
    });

});
