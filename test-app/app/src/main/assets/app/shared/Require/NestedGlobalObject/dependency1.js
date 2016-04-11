var currentFile = 'dependency1';
var nextFile = 'dependency2';

// set variables
current = currentFile;
global.next = nextFile;

TNSLog('(' + global.current + ' -> ' + next + ')');

require('./dependency2');

// set variables
global.next = global.current;
current = currentFile;

TNSLog('(' + global.next + ' -> ' + current + ')');

// set variables
current = currentFile;
global.next = nextFile;
