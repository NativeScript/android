describe("Object arguments and cached call signatures", function () {
	function roundTrip(obj) {
		return java.util.Collections.singletonList(obj).get(0);
	}

	it("passes a Java wrapper as an object argument on every call", function () {
		var list = new java.util.ArrayList();
		for (var i = 0; i < 3; i++) {
			expect(roundTrip(list)).toBe(list);
		}
	});

	it("passes an extended class instance as an object argument", function () {
		var Extended = java.lang.Object.extend({
			toString: function () {
				return "extended arg";
			}
		});
		var instance = new Extended();
		for (var i = 0; i < 3; i++) {
			expect(roundTrip(instance)).toBe(instance);
			expect(new java.lang.StringBuilder().append(instance).toString()).toBe("extended arg");
		}
	});

	it("passes a TypeScript subclass instance as an object argument", function () {
		var TsButton = (function (_super) {
			__extends(TsButton, _super);
			function TsButton() {
				var _this = _super.call(this) || this;
				_this.label = "ts arg";
				return _this;
			}
			TsButton.prototype.toString = function () {
				return this.label;
			};
			return TsButton;
		})(com.tns.tests.Button1);

		var button = new TsButton();
		for (var i = 0; i < 3; i++) {
			expect(roundTrip(button)).toBe(button);
			expect(new java.lang.StringBuilder().append(button).toString()).toBe("ts arg");
		}
	});

	it("rejects a released wrapper as an object argument", function () {
		var released = new java.lang.Object();
		global.__releaseNativeCounterpart(released);
		expect(function () {
			roundTrip(released);
		}).toThrow();
	});

	it("resolves numeric casts, typed nulls and wrappers on a warm overload cache", function () {
		var list = new java.util.ArrayList();
		var sb = new java.lang.StringBuilder();
		for (var i = 0; i < 3; i++) {
			sb.append(long(5));
			sb.append(char("x"));
			sb.append(double(1));
			sb.append(float(2));
			sb.append(java.lang.String.null);
			sb.append(java.lang.Object.null);
			sb.append(list);
			sb.append("|");
		}
		var round = "5x1.02.0nullnull[]|";
		expect(sb.toString()).toBe(round + round + round);
	});

	it("passes a numeric cast to a method with a single signature", function () {
		for (var i = 0; i < 3; i++) {
			expect(java.lang.Long.toHexString(long("4294967296"))).toBe("100000000");
		}
	});
});
