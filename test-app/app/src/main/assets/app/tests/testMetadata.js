describe("Tests metadata", function () {

	it("Can_access_protected_static_members_of_protected_interface", function () {
		var accountType = android.provider.ContactsContract.SyncColumns.ACCOUNT_TYPE;
		expect(accountType).toBeTruthy(accountType !== undefined);
	});

	it("should access public methods of non-public base classes", function () {
		var c = new com.tns.tests.MyTestDerivedClass();
		var exceptionCaught = false;
		try {
			var v1 = c.getValue1();
			expect(v1).toBe(1);
			var v2 = c.getValue2();
			expect(v2).toBe(2);
			var v3 = c.getValue3();
			expect(v3).toBe(3);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(false);
	});

	it("should be able to access static fields declared in interface from implementing class", function () {
		var c = new com.tns.tests.MyTestDerivedClass();

		var staticFieldFromInterface = android.app.Activity.TRIM_MEMORY_RUNNING_MODERATE;
		var expected = 5;

		expect(staticFieldFromInterface).toBe(expected);
	});

	it("should be able to access keyword namespace bu prefixing the namespace with $", function () {
		var keywordClass = new $in.tns.tests.JavascriptKeywordClass();
		var expected = 5;
		expect(keywordClass.getValue5()).toBe(expected);
	});
});