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

	// this test has implicit assert in com.tns.Runtime.getJavaObjectByID method
	it("test1", function () {

	  	function createObjects(name) {
		  	var c1 = new com.tns.tests.Class1();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback5")).toBe(true);
		expect(createObjects("Callback26")).toBe(true);

		gc();
		java.lang.System.gc();
	});

	// this test has implicit assert in com.tns.Runtime.getJavaObjectByID method
	it("test2", function () {

		function indref1() {
			this.class1 = new com.tns.tests.Class1();
		}
		indref1.prototype.getMessage = function() {
			return "~~~" + this.class1.getMessage();
		}

	  	function createObjects(name) {
			var c1 = new indref1();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback55")).toBe(true);
		expect(createObjects("Callback56")).toBe(true);
		gc();
		java.lang.System.gc();
	});

	// this test has implicit assert in com.tns.Runtime.getJavaObjectByID method
	it("test3", function () {

		function indref2() {
			this.helper = new indref2helper();
		}
		indref2.prototype.getMessage = function() {
			return "---" + this.helper.getMessage();
		}
		function indref2helper() {
			this.class1 = new com.tns.tests.Class1();
		}
		indref2helper.prototype.getMessage = function() {
			return "***" + this.class1.getMessage();
		}

	  	function createObjects(name) {
			var c1 = new indref2();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback91")).toBe(true);
		expect(createObjects("Callback92")).toBe(true);
		gc();
		java.lang.System.gc();
	});

	// this test has implicit assert in com.tns.Runtime.getJavaObjectByID method
	it("test4", function () {

		function indref3() {
			this.helper = new indref3helper();
		}
		indref3.prototype.getMessage = function() {
			return "+++" + this.helper.getMessage();
		}
		function indref3helper() {
			this._class1 = new com.tns.tests.Class1();

			Object.defineProperty(this, "class1", {
				get: function() {
			  		return this._class1
				}
			});
		}
		indref3helper.prototype.getMessage = function() {
			return "^^^" + this.class1.getMessage();
		}

	  	function createObjects(name) {
			var c1 = new indref3();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback1133")).toBe(true);
		expect(createObjects("Callback1134")).toBe(true);
		gc();
		java.lang.System.gc();
	});

	// this test has implicit assert in com.tns.Runtime.getJavaObjectByID method
	//originally test was commented out
	xit("test5", function () {

		function indref4() {
			this.helper = new indref4helper();
		}
		indref4.prototype.getMessage = function() {
			return "&&&" + this.helper.getMessageZZZ();
		}
		function indref4helper() {
			var _class1 = new com.tns.tests.Class1();

			__log("indref4helper _class1=" + _class1);

			Object.defineProperty(this, "class1", {
				get: function() {
			  		return _class1
				}
				,enumerable: false
			});
		}
		indref4helper.prototype.getMessageZZZ = function() {
			return "```" + this.class1.getMessage();
		}

	  	function createObjects(name) {
			var c1 = new indref4();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback1178")).toBe(true);
		expect(createObjects("Callback1179")).toBe(true);
		gc();
		java.lang.System.gc();
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

	it("should keep extended instances alive after GC", function () {
		var C1 = com.tns.tests.Class1.extend({});

		function createObjects(name) {
			var c1 = new C1();

			var cb1 = new com.tns.tests.Class1.Callback1(name, {
				getMessage: function() {
					var msg = c1.getMessage();
					return msg;
				}
			});

			return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 5 * 1000);
		}

		expect(createObjects("Callback452356")).toBe(true);
		expect(createObjects("Callback417892")).toBe(true);

		gc();
		java.lang.System.gc();
	});

	it("should keep array-enclosed objects alive after GC", function () {
        function createObjects(name) {
            var arr = new Array();
            arr.push(new com.tns.tests.Class1());

            var cb1 = new com.tns.tests.Class1.Callback1(name, {
                getMessage: function() {
                    var msg = arr[0].getMessage();
                    return msg;
                }
            });

            return com.tns.tests.Class1.Class2.printMessageWithDelay(cb1, 2 * 1000);
        }

        expect(createObjects("Callback1")).toBe(true);
        expect(createObjects("Callback2")).toBe(true);
        expect(createObjects("Callback3")).toBe(true);

        gc();
        java.lang.System.gc();
    })

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