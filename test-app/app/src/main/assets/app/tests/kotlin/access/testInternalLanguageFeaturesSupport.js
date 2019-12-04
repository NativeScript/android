describe("Tests Kotlin internal language features should not be visible at runtime", function () {
	it("Test Kotlin internal functions in a kt file should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinFileWithInternalFunctionsKt.someInternalFunction).toBe(undefined)
	});

	it("Test Kotlin internal properties in a kt file should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinFileWithInternalPropertiesKt.someInternalProperty).toBe(undefined)
	});

	it("Test Kotlin internal class should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinInternalClass).toBe(undefined)
	});

	it("Test Kotlin internal companion in a class should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinPublicClassWithInternalCompanion.Companion).toBe(undefined)
	});

	it("Test Kotlin internal methods in a class should not be exposed", function () {
	    var ktClass = new com.tns.tests.kotlin.access.KotlinPublicClassWithInternalMethods()
        expect(ktClass.someInternalMethod).toBe(undefined)
	});

	it("Test Kotlin internal named companion in a class should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinPublicClassWithInternalCompanion.SomeInternalCompanion).toBe(undefined)
	});

	it("Test Kotlin internal nested class should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinPublicClassWithInternalNestedClass.SomeNestedClass).toBe(undefined)
	});

	it("Test Kotlin internal properties in a class should not be exposed", function () {
	    var ktClass = new com.tns.tests.kotlin.access.KotlinPublicClassWithInternalProperties()
        expect(ktClass.someInternalProperty).toBe(undefined)
	});

	it("Test Kotlin internal object should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinInternalObject).toBe(undefined)
	});

	it("Test Kotlin internal interface should not be exposed", function () {
	    expect(com.tns.tests.kotlin.access.KotlinInternalInterface).toBe(undefined)
	});

	it("Test Kotlin internal enum should not be exposed", function () {
        expect(com.tns.tests.kotlin.access.KotlinInternalEnum).toBe(undefined)
	});

});
