TNSLog('main started');
var resource = require('./dependency');
var message = resource();
TNSLog(' ' + message);
TNSLog(' and main executed');
