describe("Tests Kotlin delegation support", function () {
    it("Test Kotlin class implementation delegation should work", function () {
    	var delegationObject = new com.tns.tests.kotlin.delegation.DelegationClass();
    	expect(delegationObject.getString()).toBe("some string");
    });
});