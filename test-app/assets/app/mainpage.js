__disableVerboseLogging();

require("./tests/testWeakRef"); 
require("./tests/tests"); 
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


var MainActivity = (function (_super) {
    __extends(MainActivity, _super);
    function MainActivity() {
    }
    
    MainActivity.prototype.onCreate = function () {
    	__log("onCreate from js");
    	var k = this.toString();
    	__log("this.toString " + k);
    	_super.prototype.onCreate.call(this, null);
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
    };
    return MainActivity;
})(com.tns.NativeScriptActivity); 

app.init({
	
	getActivity: function(intent) {
		__log("intent=" + intent)
		if (intent.getAction() == android.content.Intent.ACTION_MAIN) {
			return MainActivity;
		}
		else {
			fail("Unknown action");
		}
	},
	
	
	onCreate: function() {
		__log("Application on create called");
	} 
});