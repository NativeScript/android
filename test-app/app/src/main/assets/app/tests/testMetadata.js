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
describe("Java members", function () {
	it("are not constructors", function () {
		var list = new java.util.ArrayList();

		expect(function () { return new list.size(); }).toThrowError(TypeError);
		expect(list.size.prototype).toBeUndefined();
	});

	it("keep static methods constructible, so new on a static factory returns its object", function () {
		var id = new java.util.UUID.randomUUID();

		expect(id instanceof java.util.UUID).toBe(true);
	});

	it("leave class constructors constructible", function () {
		var MyObject = java.lang.Object.extend({ toString: function () { return "mine"; } });

		expect(new java.util.ArrayList().size()).toBe(0);
		expect(new MyObject().toString()).toBe("mine");
		expect(java.util.UUID.randomUUID() instanceof java.util.UUID).toBe(true);
	});
});
