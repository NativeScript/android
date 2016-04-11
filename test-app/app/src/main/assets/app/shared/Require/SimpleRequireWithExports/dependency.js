TNSLog(' and dependency started');

exports.publicString = "public string";
exports.publicNumber = 42;
exports.publicFunction = function () {
    TNSLog(' public function');
};

TNSLog(' and dependency executed');
