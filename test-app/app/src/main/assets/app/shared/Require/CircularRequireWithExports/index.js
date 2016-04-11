TNSLog('main started');
try {
    require('./dependency1');
}
catch (e) {
    TNSLog(' ' + e.name);
}
TNSLog(' and main ended');
