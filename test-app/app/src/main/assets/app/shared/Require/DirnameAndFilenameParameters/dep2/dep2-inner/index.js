var dirname = __dirname.split('/app/SharedRuntimeTests/Require/DirnameAndFilenameParameters')[1];
var filename = __filename.split('/app/SharedRuntimeTests/Require/DirnameAndFilenameParameters')[1];
var module_filename = module.filename.split('/app/SharedRuntimeTests/Require/DirnameAndFilenameParameters')[1];

TNSLog(dirname + ' ' + filename + ' ' + module_filename + ';');
