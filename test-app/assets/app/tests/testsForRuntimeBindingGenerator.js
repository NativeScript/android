var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var When_generating_a_proxy_of_nested_interface_at_runtime = function() {
	Log("TEST: When_generating_a_proxy_of_nested_interface_at_runtime");
	
	var MyButton = new com.tns.tests.Button1.extend("MyButton10", {
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
	
	Assert(called === true, "FAILED: When_generating_a_proxy_of_nested_interface_at_runtime.");
}


var When_generating_a_proxy_of_android_class_at_runtime = function() {
	Log("TEST: When_generating_a_proxy_of_android_class_at_runtime");
	
	var MyButton = new com.tns.tests.Button1.extend("MyButton36", {
		toString : function() {
		  	return "button1";	
		},
	});
	
	var button1 = new MyButton();
	var dummyObject = button1.DummyClassAsObjectField;
	
	var isInstanceOf = dummyObject instanceof com.tns.tests.DummyClass;
	Assert(isInstanceOf == true, "FAILED: When_generating_a_proxy_of_android_class_at_runtime.");
}

When_generating_a_proxy_of_nested_interface_at_runtime();
When_generating_a_proxy_of_android_class_at_runtime();