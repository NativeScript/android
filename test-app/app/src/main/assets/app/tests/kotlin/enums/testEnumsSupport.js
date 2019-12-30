describe("Tests Kotlin extension functions support", function () {

    it("Test Kotlin enum entries are accessible", function () {
        var enumEntry = com.tns.tests.kotlin.enums.KotlinEnum.TEST_ENTRY;

        expect(enumEntry).not.toBe(undefined);
        expect(enumEntry).not.toBe(null);
    });

});