describe("Tests dispatch callbacks on UI thread ", function () {
	
	it("TestProcessDataCallbackShouldBeExecutedOnUiThread", function () {

		__log("TEST: TestProcessDataCallbackShouldBeExecutedOnUiThread"); 

		var D = com.tns.tests.DispatchAsyncOpOnUIThreadTest.extend("DispatchAsyncOpOnUIThreadTest", {
			processData: function(index, data) {
				it("inner spec", function () {
					if (index === 0) {
						expect(data).toBe(123);
					} else if (index === 1) {
						expect(data).toBe(456);
					} else if (index === 2) {
						expect(data).toBe(789);
					} else {
						//one of the above has to be called ... else fail
						expect(true).toBe(false);
					}	
				});
			}
		});
		
		var d = new D();
		
		d.processDataAsync([123, 456, 789]);
	});
});
