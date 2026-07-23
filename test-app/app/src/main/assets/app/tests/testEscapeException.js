describe("interop.escapeException", function () {
    it("exists on the interop global", function () {
        expect(typeof interop).toBe("object");
        expect(typeof interop.escapeException).toBe("function");
    });

    it("returns a throwable Error preserving the message", function () {
        var wrapped = interop.escapeException(new Error("boom"));
        expect(wrapped instanceof Error).toBe(true);
        expect(wrapped.message).toBe("boom");

        var caught = null;
        try {
            throw wrapped;
        } catch (e) {
            caught = e;
        }
        expect(caught).toBe(wrapped);
    });

    it("throws TypeError when called with no arguments", function () {
        expect(function () {
            interop.escapeException();
        }).toThrowError(TypeError);
    });

    it("is idempotent (double-wrap returns the same object)", function () {
        var once = interop.escapeException(new Error("once"));
        var twice = interop.escapeException(once);
        expect(twice).toBe(once);
    });

    it("rethrows the ORIGINAL Java exception to a native caller", function () {
        var caught = null;
        try {
            com.tns.tests.EscapeExceptionTest.throwIOException();
        } catch (e) {
            caught = e;
        }
        expect(caught).not.toBeNull();
        expect(caught.nativeException).toBeDefined();

        var runnable = new java.lang.Runnable({
            run: function () {
                throw interop.escapeException(caught);
            }
        });
        var ret = com.tns.tests.EscapeExceptionTest.invokeCatchingThrowable(runnable);

        // The native caller caught the original java.io.IOException - not a
        // com.tns.NativeScriptException wrapper - so a concrete
        // `catch (IOException e)` in native code would match.
        expect(ret).not.toBeNull();
        expect(ret.getClass().getName()).toBe("java.io.IOException");
        expect(ret.getMessage()).toBe("original-io-exception");
        expect(ret.equals(caught.nativeException)).toBe(true);

        // JS is still alive after the escape round-trip.
        expect(1 + 1).toBe(2);
    });

    it("an unbranded rethrow keeps today's wrapping semantics", function () {
        var caught = null;
        try {
            com.tns.tests.EscapeExceptionTest.throwIOException();
        } catch (e) {
            caught = e;
        }
        expect(caught).not.toBeNull();

        var runnable = new java.lang.Runnable({
            run: function () {
                throw caught;
            }
        });
        var ret = com.tns.tests.EscapeExceptionTest.invokeCatchingThrowable(runnable);

        // Without the brand the caller receives the NativeScriptException
        // wrapper, with the original exception preserved as its cause.
        expect(ret).not.toBeNull();
        expect(ret.getClass().getName()).toBe("com.tns.NativeScriptException");
        expect(ret.getCause().equals(caught.nativeException)).toBe(true);
    });

    it("a branded plain JS error escapes with the default NativeScriptException shape", function () {
        var runnable = new java.lang.Runnable({
            run: function () {
                throw interop.escapeException(new Error("plain-escape"));
            }
        });
        var ret = com.tns.tests.EscapeExceptionTest.invokeCatchingThrowable(runnable);

        // No underlying Java throwable to unwrap, so the standard escape path
        // applies: the caller gets a com.tns.NativeScriptException carrying
        // the JS message.
        expect(ret).not.toBeNull();
        expect(ret.getClass().getName()).toBe("com.tns.NativeScriptException");
        expect(ret.getMessage()).toContain("plain-escape");
    });
});
