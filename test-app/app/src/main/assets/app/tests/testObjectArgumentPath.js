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

	it("constructs through each overload on a warm constructor cache", function () {
		for (var i = 0; i < 3; i++) {
			expect(new java.lang.StringBuilder("ab").toString()).toBe("ab");
			expect(new java.lang.StringBuilder(32).capacity()).toBe(32);
			expect(new java.lang.StringBuilder(new java.lang.StringBuilder("cs")).toString()).toBe("cs");
			var plain = new java.lang.Object();
			expect(plain.getClass().getName()).toBe("java.lang.Object");
		}
	});

	it("constructs extended classes repeatedly, interleaved and with arguments", function () {
		var Counting = java.util.ArrayList.extend({
			size: function () {
				return this.super.size() + 100;
			}
		});
		var Named = java.lang.Object.extend({
			toString: function () {
				return "named";
			}
		});
		var source = new java.util.ArrayList();
		source.add("a");

		var seen = [];
		for (var i = 0; i < 20; i++) {
			var empty = new Counting();
			empty.add("x");
			expect(empty.size()).toBe(101);
			expect(new Counting(10).size()).toBe(100);
			expect(new Counting(source).size()).toBe(101);
			var named = new Named();
			expect(named.toString()).toBe("named");
			expect(seen.indexOf(empty)).toBe(-1);
			seen.push(empty);
		}
	});

	describe("cached Java objects of linked wrappers", function () {
		var Holder = com.tns.tests.WrapperHolder;

		function collectJava() {
			for (var i = 0; i < 3; i++) {
				java.lang.Runtime.getRuntime().gc();
				java.lang.System.runFinalization();
			}
		}

		afterEach(function () {
			Holder.held = null;
		});

		it("never pass null for a wrapper whose Java object was collected after Java weakened it", function () {
			(function () {
				Holder.held = new java.lang.StringBuilder("weakened");
			})();
			gc();
			// Any Java call flushes the collected wrapper, weakening its Java object.
			collectJava();
			var again = Holder.held;
			Holder.held = null;
			collectJava();

			var passed;
			try {
				passed = java.util.Collections.singletonList(again).get(0);
			} catch (e) {
				passed = "threw";
			}
			expect(passed === again || passed === "threw").toBe(true);
		});

		it("resolve a reachable wrapper after warm instance calls and a collection", function () {
			var sb = new java.lang.StringBuilder("warm");
			for (var i = 0; i < 50; i++) {
				expect(sb.length()).toBe(4);
			}
			for (var j = 0; j < 2000; j++) {
				new java.lang.Object();
			}
			gc();
			collectJava();
			expect(sb.toString()).toBe("warm");
			expect(java.util.Collections.singletonList(sb).get(0)).toBe(sb);
		});

		it("resolve a constructed instance after Java made another of its class during the constructor", function () {
			var Outer = com.tns.tests.SelfNesting.extend({
				touch: function () {
					this.super.touch();
				},
				selfNestingSpec: true
			});
			var outer = new Outer(true);
			expect(outer.getTag()).toBe(2);
			collectJava();
			expect(outer.getTag()).toBe(2);
		});
	});
});
