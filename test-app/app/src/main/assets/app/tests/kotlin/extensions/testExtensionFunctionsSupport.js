describe("Tests Kotlin extension functions support", function () {

    it("Test Kotlin extension functions with no arguments and no return value should work", function () {
        var obj = new java.lang.Object();
        var hasException = false;

        try {
            obj.extensionFunctionWithNoArgumentsAndNoReturnValue();
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with no arguments should work", function () {
        var obj = new java.lang.Object();
        var hasException = false;

        try {
            var res = obj.extensionFunctionWithNoArgumentsAndStringReturnValue();
            expect(res).toBe("some data");
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with one primitive argument and no return value should work", function () {
        var obj = new java.lang.Object();
        var hasException = false;

        try {
            obj.extensionFunctionWithOnePrimitiveArgumentAndNoReturnValue(42);
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with one primitive argument should work", function () {
        var obj = new java.lang.Object();
        var hasException = false;

        try {
            var res = obj.extensionFunctionWithOnePrimitiveArgumentAndStringReturnValue(42);
            expect(res).toBe("some data 42");
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with one object type argument and no return value should work", function () {
        var obj = new java.lang.Object();
        var arg = new java.lang.Object();
        var hasException = false;

        try {
            obj.extensionFunctionWithOneObjectTypeArgumentAndNoReturnValue(arg);
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with one object type argument should work", function () {
        var obj = new java.lang.Object();
        var arg = new java.lang.Object();
        var argAsString = arg.toString();
        var hasException = false;

        try {
            var res = obj.extensionFunctionWithOneObjectTypeArgumentAndStringReturnValue(arg);
            expect(res).toBe("some data " + argAsString);
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with multiple object type arguments and no return value should work", function () {
        var obj = new java.lang.Object();
        var arg1 = new java.lang.Object();
        var arg2 = "test";
        var hasException = false;

        try {
            obj.extensionFunctionWithMultipleObjectTypeArgumentsAndNoReturnValue(arg1, arg2);
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    it("Test Kotlin extension functions with multiple object type arguments should work", function () {
        var obj = new java.lang.Object();
        var arg1 = new java.lang.Object();
        var arg1AsString = arg1.toString();
        var arg2 = "test";
        var hasException = false;

        try {
            var res = obj.extensionFunctionWithMultipleObjectTypeArgumentsAndStringReturnValue(arg1, arg2);
            expect(res).toBe("some data " + arg1AsString + " test");
        } catch (error) {
            hasException = true;
        }

        expect(hasException).toBe(false);
    });

    describe("Kotlin extension functions that shadow Java functions", function () {
        let handler;
        beforeEach(function () {
           handler = new android.os.Handler(android.os.Looper.getMainLooper());
        });

        it("should call the Java function", function (done) {
            const run = jasmine.createSpy("java.lang.Runnable.run").and.callFake(function () {
                done();
            });
            const javaRunnable = new java.lang.Runnable({run});
            handler.postAtTime(javaRunnable, 1);
        })

        it("should call the Kotlin function", function (done) {
            const invoke = jasmine.createSpy("kotlin.jvm.functions.Function0.invoke").and.callFake(function () {
                done();
            });
            const cancelToken = new java.lang.Object();
            const kotlinFunc = new kotlin.jvm.functions.Function0({invoke});
            handler.postAtTime(1, cancelToken, kotlinFunc);
        })
    });
});