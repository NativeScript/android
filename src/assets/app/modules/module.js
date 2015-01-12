var tnsExtends = this.__extends;

(function When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function() {
	
	Log("TEST: When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function");
	
	if (!tnsExtends)
	{
		fail("FAILED: When_running_code_inside_a_module_it_should_have_acces_to_tns__exports_function. __extends is not available in the module");
	}
})();


module.exports = { 
	accessGlobalObject: function(s) {
		Log("global is working " + global);
	}
}