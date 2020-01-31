describe("Tests Kotlin objects support", function () {
    it("Test Kotlin object instances should be the same reference", function () {
    	var kotlinClass = com.tns.tests.kotlin.objects.KotlinSingleton.INSTANCE;
    	var kotlinClass2 = com.tns.tests.kotlin.objects.KotlinSingleton.INSTANCE;

    	expect(kotlinClass).not.toBe(null);
    	expect(kotlinClass).not.toBe(undefined);
    	expect(kotlinClass2).not.toBe(null);
    	expect(kotlinClass2).not.toBe(undefined);

    	expect(com.tns.EqualityComparator.areReferencesEqual(kotlinClass, kotlinClass2)).toBe(true);
    });
});