describe("Tests garbage collection", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
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
	
	// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
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
	
	// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
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
	
	// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
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
	
	// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
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
});