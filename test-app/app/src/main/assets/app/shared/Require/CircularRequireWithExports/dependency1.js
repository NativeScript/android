TNSLog(' and dependency1 started');
exports.visible = "visible from dependency3";
require('./dependency2');
exports.invisible = "invisible from dependency3";
TNSLog(' and dependency1 ended');