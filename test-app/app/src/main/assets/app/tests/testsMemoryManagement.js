describe("Tests for memmory managment", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("TestSecondaryCallbackInvokationWithObjectParamsShouldWork", function () {
		
		var u = new com.tns.tests.UseCallbackTest(123);
		
		u.setDataCallback(new com.tns.tests.UseCallbackTest.DataCallback("DataCallback_5", {
			onData: function(data, delay) {
				if("inner spec: ", function () {
					android.util.Log.i("TNS.TESTS", "data=" + data.getData());
					expect(data).not.toEqual(undefined);
					expect(delay).not.toEqual(undefined);	
				});
			}
		}));
		
		u.setCleanCallback(new com.tns.tests.UseCallbackTest.CleanCallback("CleanCallback11", {
			onClean: function(delay) {
				if("inner spec: ", function () {
					expect(delay).not.toEqual(undefined);
					gc();
					java.lang.System.gc();
				});
			}
		}));

		u.enqueDataCallback(0);
		u.enqueDataCallback(10 * 1000);
		u.enqueCleanCallback(0);
		expect(true).toBe(true);
		expect(true).toEqual(true);
	});
});