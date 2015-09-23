describe("Tests JNI reference leak", function () {
	
//	var myCustomEquality = function(first, second) {
//		return first == second;
//	};
//	
//	beforeEach(function() {
//		jasmine.addCustomEqualityTester(myCustomEquality);
//	});
	
	it("test_if_method_param_marshalling_leaks", function () {
		
		var test = new com.tns.tests.JniReferenceLeakTest(null);
		
		var N = 10000;
		
		for (var n=0; n<N; n++)
		{
			var c = 'x';
			expect(test.echoChar(char(c))).toBe(c);
			
			var i = 5;
			expect(test.echoInt(i)).toBe(i);
	
			var s = "hello";
			expect(test.echoString(s)).toBe(s);
			
			var o = new java.lang.Object();
			expect(test.echoObject(o)).toBe(o);
		}
		
		expect(n).toBe(N);
	});
	
	it("test_if_field_access_marshalling_leaks", function () {
		
		var test = new com.tns.tests.JniReferenceLeakTest(null);
		
		var N = 10000;
		
		for (var n=0; n<N; n++)
		{
			var c = 'x';
			test.charField = c;
			expect(test.charField).toBe(c);
			
			test.intField = n;
			expect(test.intField).toBe(n);
	
			var s = "hello" + n;
			test.stringField = s;
			expect(test.stringField).toBe(s);
			
			var o = new java.lang.Object();
			test.objectField = o;
			expect(test.objectField).toBe(o);
		}
		
		expect(n).toBe(N);
	});
	
	it("test_if_callback_parameter_marshalling_leaks", function () {
		
		var p = new com.tns.tests.JniReferenceLeakTest.Processor({
			echoChar: function(c) { return char(c); },
			echoInt: function(i) { return i; },
			echoString: function(s) { return s; },
			echoObject: function(o) { return o; },
			echoCharArray: function(arr) { return arr; },
			echoIntArray: function(arr) { return arr; },
			echoStringArray: function(arr) { return arr; },
			echoObjectArray: function(arr) { return arr; }
		});
		
		
		var test = new com.tns.tests.JniReferenceLeakTest(p);
		
		var N = 10000;
		
		for (var n=0; n<N; n++)
		{
			var c = 'x';
			expect(test.triggerEchoChar(char(c))).toBe(c);
			
			var i = 5;
			expect(test.triggerEchoInt(i)).toBe(i);
	
			var s = "hello";
			expect(test.triggerEchoString(s)).toBe(s);
			
			var o = new java.lang.Object();
			expect(test.triggerEchoObject(o)).toBe(o);
		}
		
		expect(n).toBe(N);
	});
	
	it("test_if_global_reference_leaks_when_interface_implementation_is_created", function () {
		
		var n = 0;
		var N = 100 * 1000;
		
		for (; n<N; n++)
		{
			var r = new java.lang.Runnable({ run: function(){} });
		}
		
		expect(n).toBe(N);
	});
	
	
	it("test_passing_javascript_array_should_not_leak", function () {
		
		var n = 0;
		var N = 100 * 1000;
		
		var arr = [1, 2, 3];
		
		var btn = new com.tns.tests.Button1();
		
		for (var i=0; i<N; i++) {
		   var len = btn.getArrayLength(arr);
		   if (len != 3) {
			   break;
		   }
		   n = i;
		}
		
		expect(n).toBe(N-1);
	});

});