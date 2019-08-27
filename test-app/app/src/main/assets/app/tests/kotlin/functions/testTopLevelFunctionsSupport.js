describe("Tests Kotlin top level functions support", function () {
    it("Test Kotlin top level functions in an unnamed class should work", function () {
    	var res = com.tns.tests.kotlin.functions.TopLevelFunctionsKt.getRandomNumber();
            	expect(res).toBe(42);
    });

    it("Test Kotlin top level functions in a named class should work", function () {
    	var res = com.tns.tests.kotlin.functions.DemoUtils.getRandomString();
    	expect(res).toBe("42");
    });

    it("Test Kotlin top level functions in a named multifile class should work", function () {
    	var res1 = com.tns.tests.kotlin.multifiles.Utils.getNewString();
    	expect(res1).toBe("new string");

    	var res2 = com.tns.tests.kotlin.multifiles.Utils.getOldString();
    	expect(res2).toBe("old string");
    });
});