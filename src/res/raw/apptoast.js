telerik.kimera.KimeraActivity.extend({
			   onCreate: function() {
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
					var activity = this;
					var context  = this.getApplicationContext();
					button.setOnClickListener(new android.view.View.OnClickListener(overrides({
						onClick:  function() {
							Log("AndroidOnClickListener called js onClick");  
							button.setText("Hit me baby one more time " + counter++);
							
							android.widget.Toast.makeText(context, "OMG! you really hit it", 0).show();
					}})));
					
					this.super.onCreate(null);
			   }});

app.init({
	onCreate: function() {
	   Log("Application created. Calling toString");
	   Log("Application toString() returns "  + this.toString());
	   this.startActivity(new android.content.Intent(this, telerik.kimera.KimeraActivity.class).setFlags(268435456));
	}
});


