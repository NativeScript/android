var tnsExtends = __extends;

(function When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function() {
	
	__log("TEST: When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function");
	
	if (!tnsExtends)
	{
		fail("FAILED: When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function. __extends is not available in the module");
	}
})();


module.exports = { 
	accessGlobalObject: function(s) {
		__log("global is working " + global);
		return true;
	}
}

module.exports.value123 = 123;