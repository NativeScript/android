TNSLog(' and dependency3 started');
var requiredModule = require('./dependency1');
TNSLog(' ' + requiredModule.visible);
TNSLog(' ' + requiredModule.invisible);
TNSLog(' and dependency3 ended');