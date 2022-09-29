describe("Tests Kotlin properties support", function () {
    it("Test Kotlin JvmField properties should work", function () {
        var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

        expect(kotlinClass.jvmField).toBe(0);

        kotlinClass.jvmField = 1;

        expect(kotlinClass.jvmField).toBe(1);
    });


    it("Test Kotlin public properties should work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

    	expect(kotlinClass.immutableProperty).toBe("someImmutableProperty");
    	try{
    	    kotlinClass.immutableProperty = "SHOULD NOT WORK";
    	    fail();
    	} catch{}

    	expect(kotlinClass.mutableProperty).toBe("someMutableProperty");
    	kotlinClass.mutableProperty = "someOtherMutableProperty";
    	expect(kotlinClass.mutableProperty).toBe("someOtherMutableProperty");
    });

    it("Test Kotlin private properties should not work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

    	expect(kotlinClass.privateMutableProperty).toBe(undefined);
    	expect(kotlinClass.privateImmutableProperty).toBe(undefined);
    });

    it("Test Kotlin internal properties should not work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

    	expect(kotlinClass.internalMutableProperty).toBe(undefined);
    	expect(kotlinClass.internalImmutableProperty).toBe(undefined);
    });

    it("Test Kotlin protected properties should work", function () {
    	var kotlinClass = new (com.tns.tests.kotlin.properties.KotlinClassWithProperties.extend({
    	    getProtectedMutableProperty: function(){
    	        expect(this.super.protectedMutableProperty).toBe("someProtectedMutableProperty");
    	        this.super.protectedMutableProperty = "someOtherProtectedMutableProperty";
    	        expect(this.super.protectedMutableProperty).toBe("someOtherProtectedMutableProperty");
    	    },
    	    getProtectedImmutableProperty: function(){
    	        expect(this.super.protectedImmutableProperty).toBe("someProtectedImmutableProperty");
    	        try{
    	            this.super.protectedImmutableProperty = "SHOULD NOT WORK";
    	            fail();
    	        } catch {}
    	    }
    	}))();

    	kotlinClass.getProtectedMutableProperty();
    	kotlinClass.getProtectedImmutableProperty();
    });


    it("Test Kotlin property private should not work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();
    	try{
    	    kotlinClass.privateSetterProperty = "SHOULD NOT WORK";
    	    fail();
    	} catch {}
    });

    it("Test Kotlin boolean property should work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

    	expect(kotlinClass.isMutableBooleanProperty()).toBe(true);
    	kotlinClass.setMutableBooleanProperty(false);
    	expect(kotlinClass.isMutableBooleanProperty()).toBe(false);
    });

    it("Test Kotlin property with complext type should work", function () {
    	var kotlinClass = new com.tns.tests.kotlin.properties.KotlinClassWithProperties();

    	expect(kotlinClass.mutablePropertyWithComplexType.someString).toBe("test");

    	var simpleObject = new com.tns.tests.kotlin.SimpleKotlinObject();
    	kotlinClass.mutablePropertyWithComplexType = simpleObject;
    	expect(kotlinClass.mutablePropertyWithComplexType.equals(simpleObject)).toBe(true);
    });
});