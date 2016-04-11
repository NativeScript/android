TNSLog(' and dependency2 started');

var main = require('./index');
var dep1 = require('./dependency1');
TNSLog(' ' + main.visible + ' ' + main.invisible);
TNSLog(' ' + dep1.visible + ' ' + dep1.invisible);

TNSLog(' and dependency2 ended');