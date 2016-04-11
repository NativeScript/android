TNSLog(' and dependency1 started');

var module2 = require('./dependency2');
TNSLog(' ' + module2.publicString);
TNSLog(' ' + module2.publicNumber);
module2.publicFunction();

exports.publicString = 'dependency1 string';
exports.publicNumber = 1;
exports.publicFunction = function () {
    TNSLog(' dependency1 function');
};

TNSLog(' and dependency1 executed');
