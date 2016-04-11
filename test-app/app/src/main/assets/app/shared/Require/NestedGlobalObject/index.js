TNSLog('main started');

var currentFile = 'app';
var nextFile = 'dependency1';

// set variables
current = currentFile;
global.next = nextFile;

TNSLog('(' + global.current + ' -> ' + next + ')');

require('./dependency1');

// set variables
global.next = global.current;
current = currentFile;

TNSLog('(' + global.next + ' -> ' + current + ')');

// set variables
current = currentFile;
global.next = nextFile;

TNSLog(' and main ended'); 
