//waitForDebugger();

var callFromCtorOfInstanceCreatedByJavaReflection = false;
var callFromCtorOfInstanceCreatedByJavascript = false;


telerik.kimera.KimeraActivity.extend({
				constructor: function() {
					callFromCtorOfInstanceCreatedByJavaReflection = true;
				},
				
			   onCreate: function() {
					this.super.onCreate(null);
					
					var layout = new android.widget.LinearLayout(this);
					layout.setOrientation(1);
					this.setContentView(layout);
					
					var textView = new android.widget.TextView(this);
					textView.setText("Hit that sucker");
					layout.addView(textView);
					
					var button = new android.widget.Button(this, overrides({
						constructor: function() {
							callFromCtorOfInstanceCreatedByJavascript = true;
						},
						
						setEnabled: function(enabled) {
							Log("FROM_SETENABLED_OVERRIDE(1): " + enabled);
							var newValue = true;
							this.super.setEnabled(newValue);
							Log("FROM_SETENABLED_OVERRIDE(2): " + newValue);
						}
					}));

					button.setEnabled(false);
					button.setText("Hit me");
					layout.addView(button);
					var counter = 0;
					button.setOnClickListener(new android.view.View.OnClickListener(overrides({
						onClick:  function() {
			              Log("onClick called");  
			              button.setText("Hit that sucker one more time " + counter++);
			            }})));
			            
		            // run tests
			            
					if (callFromCtorOfInstanceCreatedByJavaReflection === false)
					{
						fail("callFromCtorOfInstanceCreatedByJavaReflection is not set");
					}
					
					if (callFromCtorOfInstanceCreatedByJavascript === false)
					{
						fail("callFromCtorOfInstanceCreatedByJavascript is not set");
					}
          }});

app.init({
	onCreate: function() {
	   Log("Application created. Calling toString");
	   Log("Application toString() returns "  + this.toString());
	   this.startActivity(new android.content.Intent(this, telerik.kimera.KimeraActivity.class).setFlags(android.content.Intent.FLAG_ACTIVITY_NEW_TASK));
	}
});

