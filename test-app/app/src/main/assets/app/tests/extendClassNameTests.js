// extend() is called through a computed member so the static binding generator skips
// these call sites: it would try, and fail, to compile a class for each name.
describe("Tests extend class name ", function () {

	function extendObject(name, implementation) {
		return java.lang.Object["extend"](name, implementation);
	}

	function expectRejected(name) {
		var message = null;
		try {
			extendObject(name, {});
		} catch (e) {
			message = e.message;
		}
		expect(message).not.toBeNull();
		expect(message).toContain(name);
	}

	// the class name valid symbols are [a-z , A-Z , 0-9, _]
	it("When_naming_extension_class_user_should_give_valid_name", function () {
		expect(function () {
			extendObject("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", {});
		}).not.toThrow();
	});

	it("When_naming_extension_contains_invalid_symbols_should_throw_exception", function () {
		expect(function () {
			extendObject("1235!", {});
		}).toThrowError(/invalid symbols/);
	});

	it("rejects a dotted name with an empty part", function () {
		expectRejected("com.tns.tests.extendname..Empty");
		expectRejected(".com.tns.tests.extendname.Leading");
		expectRejected("com.tns.tests.extendname/.Slash");
		expectRejected("com.tns.tests.extendname.Trailing.");
	});

	it("rejects a dotted name with a character no class name can contain", function () {
		expectRejected("com.tns.tests.extendname.Has Space");
		expectRejected("com.tns.tests.extendname.Has;Semicolon");
		expectRejected("com.tns.tests.extendname.Has[Bracket");
		expectRejected("com.tns.tests.extendname.Has<Angle>");
		expectRejected("com.tns.tests.extendname.No\u00A0Break");
		expectRejected("com.tns.tests.extendname.Zero\u200BWidth");
	});

	function expectWorkingClass(name, className) {
		className = className || name;
		var Extended = extendObject(name, {
			toString: function () {
				return "from " + name;
			}
		});
		var instance = new Extended();

		expect(instance.getClass().getName()).toBe(className);
		// Objects.toString calls toString() from Java, through the generated class.
		expect(java.util.Objects.toString(instance)).toBe("from " + name);
	}

	it("generates a class for a dotted name whose part starts with a digit", function () {
		expectWorkingClass("com.tns.tests.extendname.1LeadingDigit");
	});

	it("generates a class for a dotted name whose part contains a dash", function () {
		expectWorkingClass("com.tns.tests.extend-name.Dashed");
	});

	it("generates a class for a dotted name whose part is a Java reserved word", function () {
		expectWorkingClass("com.tns.tests.extendname.int");
	});

	it("generates a class for a dotted name with a non-ASCII letter", function () {
		expectWorkingClass("com.tns.tests.extendname.Caf\u00E9");
	});

	it("does not reject a dotted name with a supplementary-plane letter", function () {
		var message = null;
		try {
			extendObject("com.tns.tests.extendname.Script\uD835\uDC9C", {});
		} catch (e) {
			message = e.message;
		}
		expect(message === null ? "" : message).not.toContain("Invalid extend() name");
	});

	it("reads a '/' in a dotted name as a package separator", function () {
		expectWorkingClass("com.tns.tests.extendname/Slashed", "com.tns.tests.extendname.Slashed");
	});
});
