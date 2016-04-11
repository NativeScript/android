var currentFile = 'dependency2';
var nextFile = 'dependency3';

// set variables
current = currentFile;
global.next = nextFile;

TNSLog('(' + global.current + ' -> ' + next + ')');

require('./dependency3');

// set variables
global.next = global.current;
current = currentFile;

TNSLog('(' + global.next + ' -> ' + current + ')');

// set variables
current = currentFile;
global.next = nextFile;
