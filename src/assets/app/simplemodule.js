var summer = require('./simplesubmodule')

exports.myLog = function(s)
{
	if (arguments.length == 1)
	{
		Log("1+1=" + summer.add(1, 1))
		
		Log("global is working" + global)
	}
	Log(s);
}