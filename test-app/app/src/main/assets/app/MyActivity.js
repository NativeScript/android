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

//    	require("./tests/testsWithContext").run(this);
    	execute(); //run jasmine

    	var layout = new android.widget.LinearLayout(this);
    	layout.setOrientation(1);
    	this.setContentView(layout);

    	var textView = new android.widget.TextView(this);
    	textView.setText("It's a button!");
    	layout.addView(textView);

    	var button = new android.widget.Button(this);
    	button.setText("Hit me");
    	layout.addView(button);
    	var counter = 0;

    	var Color = android.graphics.Color;
    	var colors = [Color.BLUE, Color.RED, Color.MAGENTA, Color.YELLOW, Color.parseColor("#FF7F50")];
    	var taps = 0;

    	var dum = com.tns.tests.DummyClass.null;

    	button.setOnClickListener(new android.view.View.OnClickListener("AppClickListener", {
    		onClick:  function() {
    			button.setBackgroundColor(colors[taps % colors.length]);
    			taps++;
    			var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
                                workAbstract: function() {
                                    console.log("korrrr----->dddd")
                                    throw new Error("ErrorMessage");
                                },
                                workAbstract: function(arg) {
                                    console.log("korrrr111----->" + arg)
                                    throw new Error(arg);
                                },
                                workAbstract: function(arg, numberValue) {
                                    console.log("ddddddd----->" + arg)
                                    var ErrorMessage = arg + numberValue.toString()
                                    throw new Error(ErrorMessage);
                                }
                            });

                var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
//                nonGenericAbstractClassBase.workAbstract();
                var user = new com.tns.sgbtestclasses.User();
                user.work(nonGenericAbstractClassBase);
    		}}));

    };
    MyActivity = __decorate([
        JavaProxy("com.tns.NativeScriptActivity")
    ], MyActivity);
    return MyActivity;
})(android.app.Activity);