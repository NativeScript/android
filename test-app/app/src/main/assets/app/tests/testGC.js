describe("Tests garbage collection", function () {
	var myCustomEquality = function(first, second) {
		return first == second;
	};

	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});

	xit("TestGarbageCollection", function (done) {
		var normalTest = function () {

			__log("TEST: TestGarbageCollection");

			var obj = new com.tns.tests.ClassX();

			obj.dummy();

			obj = null;

			gc();
			java.lang.System.gc();
			gc();
			java.lang.System.gc();
			gc();
			java.lang.System.gc();

			new java.lang.Thread(new java.lang.Runnable("ThreadFunc", {
				run: function() {
					var isCollected = com.tns.tests.ClassX.IsCollected;
					__log('----------> isCollected: ' + isCollected);
					expect(isCollected).toBe(true);
					done();
				}
			})).start();
		};
		normalTest();
	});

	it("testAccessingStringFieldWontLeak", function () {

		__log("TEST: testAccessingStringFieldWontLeak");

		var dummy = new com.tns.tests.DummyClass();

		for (var i=0; i<10000; i++)
		{
			var name = dummy.nameField;

			expect(name).toBe("dummy");
		}
	});

	xit("should persist JavaScript object when it reappears after GC", function () {

		function getObject() {
			var o = new java.lang.Object();
			o.x = 123;
			var arr = java.lang.reflect.Array.newInstance(java.lang.Object.class, 1);
			arr[0] = o;
			return arr;
		}

		var arr = getObject();
		gc();
		var o = arr[0];

		expect(o.x).toBe(123);
	});

	it("should properly reintroduce Java object back in a callback", function () {
		function getTestObject() {
			return new com.tns.tests.BadEqualsTest(
				new com.tns.tests.BadEqualsTest.BadEqualsObject(),
				new com.tns.tests.BadEqualsTest.Callback({
					onFinish: function(o) {
						__log(">>o=" + o.toString());
					}
				}));
		}

		var test = getTestObject();

		// flush LRU cache
		for (var i=0; i<65536; i++) {
			new java.lang.Object().hashCode();
		}

		gc();
		java.lang.Runtime.getRuntime().gc();
		gc();
		java.lang.Runtime.getRuntime().gc();
		gc();
		java.lang.Runtime.getRuntime().gc();

		test.postCallback();
	});
	describe("collected wrappers waiting for a flush", function () {
		var Holder = com.tns.tests.WrapperHolder;

		it("keep a callback's error when wrappers were collected after its last Java call", function () {
			var callable = new java.util.concurrent.Callable({
				call: function () {
					for (var i = 0; i < 2000; i++) {
						new java.lang.Object();
					}
					gc();
					throw new Error("thrown with a batch pending");
				}
			});
			var task = new java.util.concurrent.FutureTask(callable);
			task.run();

			expect(task.isDone()).toBe(true);
			expect(function () { task.get(); }).toThrow();
		});

		function collectJava() {
			for (var i = 0; i < 3; i++) {
				java.lang.Runtime.getRuntime().gc();
				java.lang.System.runFinalization();
			}
		}

		afterEach(function () {
			Holder.held = null;
		});

		it("keep the Java object of a wrapper handed back to JS before the flush", function () {
			(function () {
				Holder.held = new java.lang.StringBuilder("reacquired");
			})();
			gc();
			var again = Holder.held;
			Holder.held = null;
			collectJava();

			expect(again.toString()).toBe("reacquired");
		});

		it("keep a wrapper handed back to JS while its liveness check is queued", function () {
			(function () {
				Holder.held = new java.lang.StringBuilder("checked");
			})();
			gc();
			collectJava();
			gc();
			var again = Holder.held;
			collectJava();

			expect(again.toString()).toBe("checked");
			again = null;
			gc();
			collectJava();
		});

		it("release an object Java registered for a call that failed before reaching it", function () {
			// Not a function: the call fails before its argument is converted.
			var target = new com.tns.tests.FreshArgs.Target({ take: 42 });
			var ref = com.tns.tests.FreshArgs.callWithNewObject(target);
			collectJava();

			expect(ref.get()).toBe(null);
		});

		it("give threads passing the same new object at once one wrapper", function (done) {
			var FreshArgs = com.tns.tests.FreshArgs;
			var received = [];
			var target = new FreshArgs.Target({
				take: function (o) {
					received.push(o);
				}
			});
			FreshArgs.startFromThreadsAtOnce(target, 4);
			(function poll() {
				if (FreshArgs.finishedCalls() < 4) {
					setTimeout(poll, 10);
					return;
				}
				expect(received.length).toBe(4);
				for (var i = 1; i < received.length; i++) {
					expect(received[i] === received[0]).toBe(true);
				}
				done();
			})();
		});

		it("let Java reclaim objects during a synchronous loop that never yields", function () {
			// Far more than the Java heap: every buffer must become collectable while the loop runs.
			var total = 0;
			for (var i = 0; i < 160; i++) {
				var stream = new java.io.ByteArrayOutputStream(4 * 1024 * 1024);
				total += stream.size() + 1;
				if (i % 8 === 7) {
					gc();
				}
			}
			expect(total).toBe(160);
		});
	});
});