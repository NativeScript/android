TNSLog('main started');
var result;
try {
    result = require('./dependency');
}
catch (e) {
    TNSLog(' ' + e.name);
}
TNSLog(' main ended');
