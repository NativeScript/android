//com.telerik.kimera.Button.setText("hello");
//var clazz = com.telerik.kimera.Button.class;
//var name = clazz.getName();
//Log("the class name is " + name);

//var myButton = new com.telerik.kimera.Button();
//var dummy = myButton.getDummy();
//dummy.dummyMethod();
//myButton.setOnClickListener(new com.telerik.kimera.View.OnClickListener({
//   onClick : function() {
//      Log("button clicked");
//   }
//}));
 
//myButton.click(new com.telerik.kimera.View());
//myButton = null;

//App = {
//  onCreate:  function() {
//  	Log("Application created");
//  }
//  
//  onTerminate: function()
//}

//module.exports = App;

//app.onCreate = function(){
//  Log("Application created");
//}

//app.onTerminate = function(){
//  Log("Application created");
//}



//primitives tests
var arg  = 5;
var button = new telerik.kimera.Button1();

arg = 5;
var result = button.LongArg(long(arg));
if (result == arg) {
	Log("LongArg with number SUCCESS");
}
else {
	Log("LongArg with number FAIL");
}

arg = "123";
var result = button.LongArg(long(arg));
if (result == arg) {
	Log("LongArg with string SUCCESS");
}
else {
	Log("LongArg with string FAIL");
}

arg = 123;
var result = button.IntArg(arg);
if (result == arg) {
	Log("IntArg SUCCESS");
}
else {
	Log("IntArg FAIL");
}

var arg  = 5;
var result = button.ByteArg(byte(arg));
if (result == arg) {
	Log("ByteArg SUCCESS");
}
else {
	Log("ByteArg FAIL");
}

var result = button.ShortArg(short(arg));
if (result == arg) {
	Log("ShortArg SUCCESS");
}
else {
	Log("ShortArg FAIL");
}


arg = true;
var result = button.BooleanArg(arg);
if (result == arg) {
	Log("BooleanArg SUCCESS");
}
else {
	Log("BooleanArg FAIL");
}

arg = false;
var result = button.BooleanArg(arg);
if (result == arg) {
	Log("BooleanArg SUCCESS");
}
else {
	Log("BooleanArg FAIL");
}

arg = "C";
var result = button.CharArg(char(arg));
if (result == arg) {
	Log("CharArg SUCCESS " + arg);
}
else {
	Log("CharArg FAIL arg:" + arg + " result:" + result);
}


var b = new telerik.kimera.Button1();
var s = b.toString();
if (s.contains("Button1"))
{
	Log("contains is working");
}

var k = s.valueOf();
Log("k is " + k);
	
fail("OK");	





//var d = new telerik.kimera.DummyClass()
//var b = new telerik.kimera.Button()

//b.method1("test")
//b.method1(d)
//b.method1(12345)
//b.method1(new java.lang.Byte(123))
//b.method1(new java.lang.Byte("45"))

//var LinearLayout_VERTICAL = 1;
//var Intent_FLAG_ACTIVITY_NEW_TASK = 268435456;

//var activity = new telerik.kimera.KimeraActivity(overrides({
//			   onCreate: function() {
//					var layout = new android.widget.LinearLayout(this);
//					layout.setOrientation(LinearLayout_VERTICAL);
//					this.setContentView(layout);
//					
//					var textView = new android.widget.TextView(this);
//					textView.setTag("Hit that sucker");
//					layout.addView(textView);
//					
//					var button = new android.widget.Button(this);
//					button.setText("Hit me");
//					layout.addView(button); 
//			   }})
//);

//var clickListener = new telerik.telerik.kimera.OnClickListener({
//	onClick:  function() {
//	    //button.setText("Hit me again");
//	}});