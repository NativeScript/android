TNSLog('main started');

// define varliables
appVisible1 = 'appVisible1';
global.appVisible2 = 'appVisible2';
var appInvisible = 'appInvisible';

require('./dependency');

TNSLog(' ' + global.depVisible1);
TNSLog(' ' + depVisible1);
TNSLog(' ' + global.depVisible2);
TNSLog(' ' + global.depInvisible);

TNSLog(' ' + appVisible1); // should be chanched

TNSLog(' and main ended');
