//disableVerboseLogging();
//
//require("./tests/testWeakRef");
//require("./tests/tests");
//require("./tests/testsForRuntimeBindingGenerator");
//require("./tests/propertyAccessTests");
//require("./tests/numericConversionTests");
//require("./tests/inheritanceChainResolutionTest");
//require("./tests/exceptionHandlingTests");
//require("./tests/dispatchCallbacksOnUiThreadTests");
//require("./tests/stringConversionTests");
//require("./tests/testsForTypescript");
//require("./tests/testGC");
//require("./tests/testsMemoryManagement");
//require("./tests/testIfAbleToRunExternalFile");
//require("./tests/finalFieldsSetTests");
//require("./tests/extendedClassesTests");
//require("./tests/extendClassNameTests");


//var MainActivity = com.tns.NativeScriptActivity.extend("MainActivity", {
//			   onCreate: function() {
//					this.super.onCreate(null);
//					
//					require("./tests/testsWithContext").run(this);
//					
//					var layout = new android.widget.LinearLayout(this);
//					layout.setOrientation(1);
//					this.setContentView(layout);
//					
//					var textView = new android.widget.TextView(this);
//					textView.setText("Hit that sucker");
//					layout.addView(textView);
//					
//					var button = new android.widget.Button(this);
//					button.setText("Hit me");
//					layout.addView(button);
//					var counter = 0;
//					button.setOnClickListener(new android.view.View.OnClickListener("AppClickListener", {
//						onClick:  function() {
//			              Log("onClick called");  
//			              button.setText("Hit that sucker one more time " + ++counter);
//			            }}));
//          }});



var MainActivity = (function (_super) {
    __extends(MainActivity, _super);
    function MainActivity() {
    }
    
    MainActivity.prototype.onCreate = function () {
    	Log("onCreate from js");
    	var k = this.toString();
    	Log("this.toString " + k);
    	_super.prototype.onCreate.call(this, null);
    	//this.super.onCreate(null);
    	
//    	require("./tests/testsWithContext").run(this);
    	
    	var buttonTest = new android.widget.Button(this);
    	buttonTest.asd();

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
    			Log("onClick called");  
    			button.setText("Hit that sucker one more time " + ++counter);
    		}}));
    };
    return MainActivity;
})(com.tns.NativeScriptActivity); 

app.init({
	
	getActivity: function(intent) {
		Log("intent=" + intent)
		if (intent.getAction() == android.content.Intent.ACTION_MAIN) {
			return MainActivity;
		}
		else {
			fail("Unknown action");
		}
	},
	
	
	onCreate: function() {
		Log("Application on create called");
	} 
});