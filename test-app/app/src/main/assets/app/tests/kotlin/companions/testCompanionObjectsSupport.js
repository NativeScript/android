describe("Tests Kotlin companion objects support", function () {
	it("Test Kotlin companion object without a name should be supported", function () {
	    var stringFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.Companion.getStringFromCompanion();
	    expect(stringFromCompanion).toBe("testCompanion");

	    var providedStringFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.Companion.getProvidedStringFromCompanion("providedString");
	    expect(providedStringFromCompanion).toBe("providedString");

	    var simpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.Companion.getSimpleObjectFromCompanion();
	    expect(simpleObjectFromCompanion.getSomeString()).toBe("test");

	    var simpleKotlinObject = new com.tns.tests.kotlin.SimpleKotlinObject();
	    var providedSimpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.Companion.getProvidedSimpleObjectFromCompanion(simpleKotlinObject);
	    expect(simpleKotlinObject.equals(providedSimpleObjectFromCompanion)).toBe(true);

	    var stringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.getStringJvmStaticFromCompanion();
	    expect(stringJvmStaticFromCompanion).toBe("testCompanion");

	    var providedStringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithCompanion.getProvidedStringJvmStaticFromCompanion("providedString");
	    expect(providedStringJvmStaticFromCompanion).toBe("providedString");
	});

	it("Test Kotlin named companion object without a name should be supported", function () {
	    var stringFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.NamedCompanion.getStringFromNamedCompanion();
        expect(stringFromCompanion).toBe("testCompanion");

        var providedStringFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.NamedCompanion.getProvidedStringFromNamedCompanion("providedString");
        expect(providedStringFromCompanion).toBe("providedString");

        var simpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.NamedCompanion.getSimpleObjectFromNamedCompanion();
        expect(simpleObjectFromCompanion.getSomeString()).toBe("test");

        var simpleKotlinObject = new com.tns.tests.kotlin.SimpleKotlinObject();
        var providedSimpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.NamedCompanion.getProvidedSimpleObjectFromNamedCompanion(simpleKotlinObject);
        expect(simpleKotlinObject.equals(providedSimpleObjectFromCompanion)).toBe(true);

        var stringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.getStringJvmStaticFromNamedCompanion();
        expect(stringJvmStaticFromCompanion).toBe("testCompanion");

        var providedStringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinClassWithNamedCompanion.getProvidedStringJvmStaticFromNamedCompanion("providedString");
        expect(providedStringJvmStaticFromCompanion).toBe("providedString");
	});


	it("Test Kotlin interface companion object without a name should be supported", function () {
    	var stringFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.Companion.getStringFromCompanion();
    	expect(stringFromCompanion).toBe("testCompanion");

    	var providedStringFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.Companion.getProvidedStringFromCompanion("providedString");
    	expect(providedStringFromCompanion).toBe("providedString");

    	var simpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.Companion.getSimpleObjectFromCompanion();
    	expect(simpleObjectFromCompanion.getSomeString()).toBe("test");

    	var simpleKotlinObject = new com.tns.tests.kotlin.SimpleKotlinObject();
    	var providedSimpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.Companion.getProvidedSimpleObjectFromCompanion(simpleKotlinObject);
    	expect(simpleKotlinObject.equals(providedSimpleObjectFromCompanion)).toBe(true);

    	var stringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.getStringJvmStaticFromCompanion();
    	expect(stringJvmStaticFromCompanion).toBe("testCompanion");

    	var providedStringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithCompanion.getProvidedStringJvmStaticFromCompanion("providedString");
    	expect(providedStringJvmStaticFromCompanion).toBe("providedString");
    });

    it("Test Kotlin interface named companion object without a name should be supported", function () {
    	var stringFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.NamedCompanion.getStringFromNamedCompanion();
        expect(stringFromCompanion).toBe("testCompanion");

        var providedStringFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.NamedCompanion.getProvidedStringFromNamedCompanion("providedString");
        expect(providedStringFromCompanion).toBe("providedString");

        var simpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.NamedCompanion.getSimpleObjectFromNamedCompanion();
        expect(simpleObjectFromCompanion.getSomeString()).toBe("test");

        var simpleKotlinObject = new com.tns.tests.kotlin.SimpleKotlinObject();
        var providedSimpleObjectFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.NamedCompanion.getProvidedSimpleObjectFromNamedCompanion(simpleKotlinObject);
        expect(simpleKotlinObject.equals(providedSimpleObjectFromCompanion)).toBe(true);

        var stringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.getStringJvmStaticFromNamedCompanion();
        expect(stringJvmStaticFromCompanion).toBe("testCompanion");

        var providedStringJvmStaticFromCompanion = com.tns.tests.kotlin.companions.KotlinInterfaceWithNamedCompanion.getProvidedStringJvmStaticFromNamedCompanion("providedString");
        expect(providedStringJvmStaticFromCompanion).toBe("providedString");
    });

    it("Test Kotlin class whose parent class contains a companion should work", function () {
    	var child = new com.tns.tests.kotlin.companions.ChildKotlinClass();
    	var str = child.getStringFromCompanion();
    	expect(str).toBe("someString");
    });

});