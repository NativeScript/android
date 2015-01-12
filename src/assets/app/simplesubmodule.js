//circular reference
var logger = require('./simplemodule')

module.exports.add = function(x, y)
{
	logger.myLog("Summing " + x + " and " + y, true)
	return x + y;
}