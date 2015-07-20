__disableVerboseLogging();

require("./tests/testWeakRef"); 
require("./tests/tests");
require("./tests/testArrays");
require("./tests/testsForRuntimeBindingGenerator");
require("./tests/testPrimitiveTypeConversion");
require("./tests/numericConversionTests"); 
require("./tests/inheritanceChainResolutionTest"); 
require("./tests/exceptionHandlingTests");
require("./tests/dispatchCallbacksOnUiThreadTests");
require("./tests/stringConversionTests");
require("./tests/testsForTypescript"); 
require("./tests/testGC");
require("./tests/testsMemoryManagement");
require("./tests/testIfAbleToRunExternalFile");
require("./tests/finalFieldsSetTests");
require("./tests/extendedClassesTests");
require("./tests/extendClassNameTests");
require("./tests/testJniReferenceLeak");

var MainActivity = {
    onCreate: function (bundle) {
    	__log("onCreate from js");
    	var k = this.toString();
    	__log("this.toString " + k);
    	this.super.onCreate(bundle);
    	//this.super.onCreate(null);

    	require("./tests/testsWithContext").run(this);
    	execute(); //run jasmine
    	
    	var layout = new android.widget.LinearLayout(this);
    	layout.setOrientation(1);
    	this.setContentView(layout);
    	
    	var textView = new android.widget.TextView(this);
    	textView.setText("Hit that sucker");
    	layout.addView(textView);
    	
    	var button = new android.widget.Button(this);
    	button.setText("Hit me");
    	layout.addView(button);
    	var counter = 0;
    	button.setOnClickListener(new android.view.View.OnClickListener("AppClickListener", {
    		onClick:  function() {
    			__log("onClick called");  
    			button.setText("Hit that sucker one more time " + ++counter);
    		}}));
    }
}; 

app.init({
	
	getActivity: function(activity) {
		var intent = activity.getIntent();
		__log("intent=" + intent)
		var action = intent.getAction();
		__log("action=" + action)
		return MainActivity;
		if (action == android.content.Intent.ACTION_MAIN) {
			return MainActivity;
		}
		else {
			__exit("Unknown action");
		}
	},
	
	
	onCreate: function() {
		__log("Application on create called");
	} 
});