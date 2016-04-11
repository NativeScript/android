var currentFile = 'dependency3';
var nextFile = 'dependency4';

// set variables
current = currentFile;
global.next = nextFile;

TNSLog('(' + global.current + ' -> ' + next + ')');

require('./dependency4');

// set variables
global.next = global.current;
current = currentFile;

TNSLog('(' + global.next + ' -> ' + current + ')');
