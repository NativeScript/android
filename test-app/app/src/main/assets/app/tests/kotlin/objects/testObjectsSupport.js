describe("Tests Kotlin objects support", function () {
    it("Test Kotlin object instances should be the same reference", function () {
    	var kotlinClass = com.tns.tests.kotlin.objects.KotlinSingleton.INSTANCE;
    	var kotlinClass2 = com.tns.tests.kotlin.objects.KotlinSingleton.INSTANCE;
    	expect(com.tns.EqualityComparator.areReferencesEqual(kotlinClass, kotlinClass2)).toBe(true);
    });
});