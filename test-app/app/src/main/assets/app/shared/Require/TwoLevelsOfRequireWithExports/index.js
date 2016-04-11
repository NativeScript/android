TNSLog('main started');
var module1 = require('./dependency1');
TNSLog(' ' + module1.publicString);
TNSLog(' ' + module1.publicNumber);
module1.publicFunction();
TNSLog(' and main executed');