TNSLog('main started');
var result;
try {
    result = require('/notExistingFile');
}
catch (e) {
    TNSLog(' ' + e.name);
}
TNSLog(' main ended');
