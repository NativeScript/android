var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var Instance_with_no_extension_shouldnt_use_previously_defined_implementation_object = function() {
	var button = new com.tns.tests.Button1.extend({
		toString: function () {
			return "overriden toString method of chronometer instance";
		},
		getIMAGE_ID_PROP: function () {
			return "overriden getIMAGE_ID_PROP method on button";
		}
	});
	var labelToString = button.toString();
	var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
	//
	
	var button1 = new com.tns.tests.Button1();
	var labelToString1 = button1.toString();
	var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();
	
	
	Assert((labelToString !== labelToString1) && (labelgetIMAGE_ID_PROP !== labelgetIMAGE_ID_PROP1), "Instance_with_no_extension_it_shouldnt_use_previously_defined_implementation_object FAILED! Labels need to be different");
}

var Instance_with_extension_shouldnt_use_previously_defined_implementation_object = function() {
	var button = new com.tns.tests.Button1.extend({
		toString: function () {
			return "overriden toString method of button instance";
		},
		getIMAGE_ID_PROP: function () {
			return "overriden getIMAGE_ID_PROP method on button";
		}
	});
	var labelToString = button.toString();
	var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
	//
	
	var button1 = new com.tns.tests.Button1.extend({
		toString: function () {
			return "overriden toString method of button1 instance ";
		},
		getIMAGE_ID_PROP: function () {
			return "overriden getIMAGE_ID_PROP method on button1";
		}
	});
	var labelToString1 = button1.toString();
	var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();
	
	
	Assert((labelToString !== labelToString1) && (labelgetIMAGE_ID_PROP !== labelgetIMAGE_ID_PROP1), "Instance_with_no_extension_it_shouldnt_use_previously_defined_implementation_object FAILED! Labels need to be different");
}

var Newly_created_instances_should_behave_the_same_and_not_use_previously_defined_implementation_objects = function() {
	
	var button1 = new com.tns.tests.Button1();
	var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();
	
	//
	var button = new com.tns.tests.Button1.extend({
		getIMAGE_ID_PROP: function () {
			return "overriden getIMAGE_ID_PROP method on button";
		}
	});
	var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
	//
	
	var button2 = new com.tns.tests.Button1();
	var labelgetIMAGE_ID_PROP2 = button2.getIMAGE_ID_PROP();
	
	Assert(labelgetIMAGE_ID_PROP1 === labelgetIMAGE_ID_PROP2, "Instance_with_no_extension_it_shouldnt_use_previously_defined_implementation_object FAILED! Labels need to be different");
	
}

Instance_with_no_extension_shouldnt_use_previously_defined_implementation_object();
Instance_with_extension_shouldnt_use_previously_defined_implementation_object();
Newly_created_instances_should_behave_the_same_and_not_use_previously_defined_implementation_objects();
