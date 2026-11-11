describe("Tests dispatch callbacks on UI thread ", function () {
	
	it("TestProcessDataCallbackShouldBeExecutedOnUiThread", function (done) {

		__log("TEST: TestProcessDataCallbackShouldBeExecutedOnUiThread");

		var received = [];
		var D = com.tns.tests.DispatchAsyncOpOnUIThreadTest.extend("DispatchAsyncOpOnUIThreadTest", {
			processData: function(index, data) {
				received.push([index, data]);
				if (received.length === 3) {
					expect(received).toEqual([[0, 123], [1, 456], [2, 789]]);
					done();
				}
			}
		});

		var d = new D();

		d.processDataAsync([123, 456, 789]);
	});
});
