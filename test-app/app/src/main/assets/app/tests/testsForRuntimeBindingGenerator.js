describe("Tests for runtime binding generator", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("When_generating_a_proxy_of_nested_interface_at_runtime", function () {

		__log("TEST: When_generating_a_proxy_of_nested_interface_at_runtime");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton10", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button = new MyButton();
		
		var called = false;
		button.setOnClickListener(new android.view.View.OnClickListener("ClickListener19", {
			onClick:  function() {
		          called = true;
		    }
		}));
		
	    button.click(null);
		
		expect(called).toBe(true);
	});
	
	it("When_generating_a_proxy_of_android_class_at_runtime", function () {
		
		__log("TEST: When_generating_a_proxy_of_android_class_at_runtime");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton36", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button1 = new MyButton();
		var dummyObject = button1.DummyClassAsObjectField;
		
		var isInstanceOf = dummyObject instanceof com.tns.tests.DummyClass;
		expect(isInstanceOf).toEqual(true);
	});
});