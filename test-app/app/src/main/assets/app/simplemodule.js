var summer = require('./simplesubmodule')

exports.myLog = function(s)
{
	if (arguments.length == 1)
	{
		__log("1+1=" + summer.add(1, 1))
		
		__log("global is working" + global)
	}
	__log(s);
}