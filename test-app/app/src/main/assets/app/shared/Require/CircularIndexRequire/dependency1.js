TNSLog(' and dependency1 started');
exports.visible = 'visible';
require('./dependency2');
exports.invisible = 'invisible';
TNSLog(' and dependency1 ended');