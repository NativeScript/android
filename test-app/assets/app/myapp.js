// demonstrates how to extend class in JavaScript with prebuilt Java proxy
var MyApp = android.app.Application.extend("com.tns.MyApp", 
{
	onCreate: function()
	{
		__log("Hello MyApp::onCreate()");
		
		require("./boot.js");
	}
});
