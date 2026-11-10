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

describe("Java object wrappers", function () {
	it("resolve constructor through the class prototype, with no own properties", function () {
		var id = java.util.UUID.randomUUID();

		expect(Object.getPrototypeOf(id)).toBe(java.util.UUID.prototype);
		expect(id.constructor).toBe(java.util.UUID);
		expect(Object.prototype.hasOwnProperty.call(id, "constructor")).toBe(false);
		expect(Object.keys(id)).toEqual([]);
	});

	it("share a prototype between Java-created and JS-created instances", function () {
		var created = new java.lang.StringBuilder("a");
		var returned = new java.lang.StringBuilder().append("b").append(created);

		expect(Object.getPrototypeOf(returned)).toBe(Object.getPrototypeOf(created));
		expect(returned instanceof java.lang.StringBuilder).toBe(true);
		expect(String(returned.toString())).toBe("ba");
	});

	it("read super through an implementation object that is not a Java instance", function () {
		var impl = { toString: function () { return "impl"; } };
		java.lang.Object.extend(impl);

		expect(function () { return impl.super; }).not.toThrow();
	});

	it("leave an extended instance Java creates during a JS construction its own JS object", function () {
		var Inner = java.lang.Object.extend({ toString: function () { return "inner"; } });
		var Outer = com.tns.tests.NestedConstruction.extend({ toString: function () { return "outer"; } });
		var outer = new Outer(new Inner().getClass(), null);

		expect(String(outer.toString())).toBe("outer");
		expect(String(outer.getCreated().toString())).toBe("inner");
	});

	it("keep a JS construction's instance when a callback constructs another during it", function () {
		var Inner = java.lang.Object.extend({ toString: function () { return "inner"; } });
		var Outer = com.tns.tests.NestedConstruction.extend({ toString: function () { return "outer:" + this.tag; } });
		var made = null;
		var outer = new Outer(null, new java.lang.Runnable({
			run: function () {
				made = new Inner();
			}
		}));
		outer.tag = "mine";

		expect(String(outer.toString())).toBe("outer:mine");
		expect(String(made.toString())).toBe("inner");
	});

	it("hand back the same wrapper for the same Java object", function () {
		var builder = new java.lang.StringBuilder();

		expect(builder.append("x")).toBe(builder);
	});

	it("stay linked while thousands of later wrappers are collected", function () {
		var list = new java.util.ArrayList();
		var first = java.util.UUID.randomUUID();
		list.add(first);
		for (var round = 0; round < 3; round++) {
			for (var i = 0; i < 3000; i++) {
				java.util.UUID.randomUUID();
			}
			gc();
			java.lang.System.gc();
		}

		expect(list.get(0)).toBe(first);
		expect(String(list.get(0).toString())).toBe(String(first.toString()));
	});
});
