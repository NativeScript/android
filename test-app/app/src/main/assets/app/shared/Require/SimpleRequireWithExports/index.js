TNSLog('main started');
var module = require('./dependency');
TNSLog(' ' + module.publicString);
TNSLog(' ' + module.publicNumber);
module.publicFunction();
TNSLog(' and main executed');