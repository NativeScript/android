describe("Tests string conversion ", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("TestCanConvertNonTrivalJavaString1", function () {
		
		__log("TEST: TestCanConvertNonTrivalJavaString1");
		
		var strConvTest = new com.tns.tests.StringConversionTest();
		
		var s = strConvTest.getString();
		
		var len = strConvTest.getLength();
		
		var isEqualsLength = strConvTest.equalsLength(s);
		
		expect(isEqualsLength).toBe(true);
		
		var isEqualsString = strConvTest.equalsString(s);
		
		expect(isEqualsString).toBe(true);
		
	});
	
	it("TestCanConvertNonTrivalJavaString2", function () {
		

		__log("TEST: TestCanConvertNonTrivalJavaString2");

		var strConvTest = new com.tns.tests.StringConversionTest();
		
		var s = strConvTest.getString(1);
		
		var arr = strConvTest.getStringArr();
		
		arr[0] = s;
		
		var newS = arr[0];
		
		var isEqualsLength = strConvTest.equalsLength(newS);
		
		expect(isEqualsLength).toBe(true);
		
		var isEqualsString = strConvTest.equalsString(newS);
		
		expect(isEqualsString).toBe(true);
	});
	
	it("TestCanConvertNonTrivalJavaString3", function () {
		
		__log("TEST: TestCanConvertNonTrivalJavaString3");

		var strConvTest = new com.tns.tests.StringConversionTest();
		
		var s = strConvTest.s;
		
		var len = strConvTest.getLength();
		
		var isEqualsLength = strConvTest.equalsLength(s);
		
		expect(isEqualsLength).toBe(true);
		
		var isEqualsString = strConvTest.equalsString(s);
		
		expect(isEqualsString).toBe(true);
	});
	
	it("TestCanConvertNonTrivalJavaString4", function () {
		
		__log("TEST: TestCanConvertNonTrivalJavaString4");
		
		var s = "";
		var len = 0;
		var isEqualsLength = false;
		var isEqualsString = false;
		
		var MyStringConversionTest = com.tns.tests.StringConversionTest.extend("StringConversionTest77", {
			callback: function(str) {
				s = str;
				len = this.getLength();
				isEqualsLength = this.equalsLength(s);
				isEqualsString = this.equalsString(s);
			}
		});
		
		var strConvTest = new MyStringConversionTest();
		
		strConvTest.triggerCallback();
		
		expect(isEqualsLength).toBe(true);
		
		expect(isEqualsString).toBe(true);
	});
});
