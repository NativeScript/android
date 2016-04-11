// define variables
depVisible1 = 'depVisible1';
global.depVisible2 = 'depVisible2';
var depInvisible = 'depInvisible';

// console.log vglobal variables from the previous module
TNSLog(' ' + global.appVisible1);
TNSLog(' ' + appVisible1);
TNSLog(' ' + global.appVisible2);
TNSLog(' ' + global.appInvisible);

// change a global variable
global.appVisible1 = 'changed';
