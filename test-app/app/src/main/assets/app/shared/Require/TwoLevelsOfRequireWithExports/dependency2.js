TNSLog(' and dependency2 started');

exports.publicString = 'dependency2 string';
exports.publicNumber = 2;
exports.publicFunction = function () {
    TNSLog(' dependency2 function');
};

TNSLog(' and dependency2 executed');
