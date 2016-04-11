var currentFile = 'dependency4';
var nextFile = 'dependency5';

// set variables
current = currentFile;
global.next = nextFile;

TNSLog('(' + global.current + ' -> ' + next + ')');

require('./dependency5');

// set variables
global.next = global.current;
current = currentFile;

TNSLog('(' + global.next + ' -> ' + current + ')');
