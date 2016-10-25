describe("Tests array operations", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});

	xit("TestWorkingWithJavaArrayDoesNotMakeMemoryLeak", function () {
		__log("TEST: TestWorkingWithJavaArrayDoesNotMakeMemoryLeak");
		
		var size = 10 * 1024 * 1024;
		
		for (var i = 0; i < 100; i++) {
		
			var arr = java.lang.reflect.Array.newInstance(java.lang.Byte.class.getField("TYPE").get(null), size);
			
			var length = arr.length;
			
			expect(length).toEqual(size);
			
			arr[0] = 123;
			
			var el = arr[0];
			
			expect(el).toEqual(123);
		}
	});

	it("TestArraySize", function () {
		
		var size = 12345;
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Object.class, size);
		
		expect(arr.length).toBe(size);
	});
	
	it("TestArrayOperations", function () {
		
		// keep 'size' value large (>512) in order to test for JNI reference leaks as well
		var size = 10000;
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Integer.class, size);
		
		expect(arr.length).toBe(size);
		
		for (var i = 0; i < size; i++) {
			arr[i] = java.lang.Integer.valueOf(i);
		}
		
		for (var i = 0; i < size; i++) {
			var n = arr[i].intValue();
			expect(n).toBe(i);
		}
	});

	it("TestArrays", function () {
		
		__log("TEST: TestArrays");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton639", {
			toString : function() {
			  	return "button1";	
			}
		});
		var tester = new MyButton(); 
		var instances = tester.getDummyInstances();
		
		var instanceFound = false;

		for (var i = 0; i < instances.length; i++)
		{
			if (instances[i].getName() == "second");
			{
				instanceFound = true;
			}
		}
		
		expect(instanceFound).toEqual(true);
		
		instances[0] = instances[1];
		
		var instances0name = instances[0].getName();
		var instances1name = instances[1].getName();
		
		expect(instances0name).toEqual(instances1name);
	});
	
	it("TestArrayLengthPropertyIsNumber", function () {
		
		__log("TEST: TestArrayLengthPropertyIsNumber");
		
		var expectedLength = 10;

		function getLength(x)
		{
			var arr = x.getIntArray1(expectedLength);
			
			return arr ? arr.length : 123456;
		}
		
		var MyButton = com.tns.tests.Button1.extend("MyButton680", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var count = getLength(new MyButton());
		
		expect(count).toBe(expectedLength);
		
	});
	
});