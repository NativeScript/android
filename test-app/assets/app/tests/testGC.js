///START: Extends tests
var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}


// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
function test1() {
  
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
	
	Assert(createObjects("Callback5") === true, "test1 FAILED: to enqueue first message");
	Assert(createObjects("Callback26") === true, "test1 FAILED: to enqueue second message");
	gc();
	java.lang.System.gc();
}


// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
function test2() {

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
	
	Assert(createObjects("Callback55") === true, "test2 FAILED: to enqueue first message");
	Assert(createObjects("Callback56") === true, "test2 FAILED: to enqueue second message");
	gc();
	java.lang.System.gc();
}


// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
function test3() {

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
	
	Assert(createObjects("Callback91") === true, "test3 FAILED: to enqueue first message");
	Assert(createObjects("Callback92") === true, "test3 FAILED: to enqueue second message");
	gc();
	java.lang.System.gc();
}


// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
function test4() {

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
	
	Assert(createObjects("Callback1133") === true, "test4 FAILED: to enqueue first message");
	Assert(createObjects("Callback1134") === true, "test4 FAILED: to enqueue second message");
	gc();
	java.lang.System.gc();
}


// this test has implicit assert in com.tns.Platform.getJavaObjectByID method
function test5() {

	function indref4() {
		this.helper = new indref4helper();
	}
	indref4.prototype.getMessage = function() {
		return "&&&" + this.helper.getMessageZZZ();
	}
	function indref4helper() {	
		var _class1 = new com.tns.tests.Class1();
		
		Log("indref4helper _class1=" + _class1);
	
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
	
	Assert(createObjects("Callback1178") === true, "test5 FAILED: to enqueue first message");
	Assert(createObjects("Callback1179") === true, "test5 FAILED: to enqueue second message");
	gc();
	java.lang.System.gc();
}

function testAccessingStringFieldWontLeak() {

	Log("TEST: testAccessingStringFieldWontLeak");

	var dummy = new com.tns.tests.DummyClass();
	
	for (var i=0; i<10000; i++)
	{
		var name = dummy.nameField;
		
		Assert(name === "dummy", "FAILED testAccessingStringFieldWontLeak: Expected vaule='dummy', actual value=" + name);
	}
}


test1()
test2()
test3()
test4()
//test5()
testAccessingStringFieldWontLeak();