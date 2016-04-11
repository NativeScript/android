/* 
	// demonstrates how to extend class in TypeScript with prebuilt Java proxy
	
	declare module android {
		export module app {
			export class Activity {
				onCreate(bundle: android.os.Bundle);			
			}
		}
		export module os {
			export class Bundle {}
		}
	}
	
	@JavaProxy("com.tns.NativeScriptActivity")
	class MyActivity extends android.app.Activity
	{
		onCreate(bundle: android.os.Bundle) 
		{
			super.onCreate(bundle);
		}
	}
*/
var MyActivity = (function (_super) {
    __extends(MyActivity, _super);
    function MyActivity() {
        _super.apply(this, arguments);
    }
    MyActivity.prototype.onCreate = function (bundle) {
        _super.prototype.onCreate.call(this, bundle);
        
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
    MyActivity = __decorate([
        JavaProxy("com.tns.NativeScriptActivity")
    ], MyActivity);
    return MyActivity;
})(android.app.Activity);