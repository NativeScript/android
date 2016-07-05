var file_access_module = require("file-system/file-system-access");
var fileAccess;
var getFileAccess = function () {
    if (!fileAccess) {
        fileAccess = new file_access_module.FileSystemAccess();
    }
    return fileAccess;
};
var nameProperty = "_name";
var pathProperty = "_path";
var isKnownProperty = "_isKnown";
var fileLockedProperty = "_locked";
var extensionProperty = "_extension";
var lastModifiedProperty = "_lastModified";
var createFile = function (info) {
    var file = new File();
    file[pathProperty] = info.path;
    file[nameProperty] = info.name;
    file[extensionProperty] = info.extension;
    return file;
};
var createFolder = function (info) {
    var documents = knownFolders.documents();
    if (info.path === documents.path) {
        return documents;
    }
    var temp = knownFolders.temp();
    if (info.path === temp.path) {
        return temp;
    }
    var folder = new Folder();
    folder[pathProperty] = info.path;
    folder[nameProperty] = info.name;
    return folder;
};
var FileSystemEntity = (function () {
    function FileSystemEntity() {
    }
    Object.defineProperty(FileSystemEntity.prototype, "parent", {
        get: function () {
            var onError = function (error) {
                throw error;
            };
            var folderInfo = getFileAccess().getParent(this.path, onError);
            if (!folderInfo) {
                return undefined;
            }
            return createFolder(folderInfo);
        },
        enumerable: true,
        configurable: true
    });
    FileSystemEntity.prototype.remove = function () {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var localError = function (error) {
                hasError = true;
                reject(error);
            };
            _this.removeSync(localError);
            if (!hasError) {
                resolve();
            }
        });
    };
    FileSystemEntity.prototype.removeSync = function (onError) {
        if (this[isKnownProperty]) {
            if (onError) {
                onError({ message: "Cannot delete known folder." });
            }
            return;
        }
        var fileAccess = getFileAccess();
        if (this instanceof File) {
            fileAccess.deleteFile(this.path, onError);
        }
        else if (this instanceof Folder) {
            fileAccess.deleteFolder(this.path, onError);
        }
    };
    FileSystemEntity.prototype.rename = function (newName) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var localError = function (error) {
                hasError = true;
                reject(error);
            };
            _this.renameSync(newName, localError);
            if (!hasError) {
                resolve();
            }
        });
    };
    FileSystemEntity.prototype.renameSync = function (newName, onError) {
        if (this[isKnownProperty]) {
            if (onError) {
                onError(new Error("Cannot rename known folder."));
            }
            return;
        }
        var parentFolder = this.parent;
        if (!parentFolder) {
            if (onError) {
                onError(new Error("No parent folder."));
            }
            return;
        }
        var fileAccess = getFileAccess();
        var path = parentFolder.path;
        var newPath = fileAccess.joinPath(path, newName);
        var hasError = false;
        var localError = function (error) {
            hasError = true;
            if (onError) {
                onError(error);
            }
            return null;
        };
        fileAccess.rename(this.path, newPath, localError);
        this[pathProperty] = newPath;
        this[nameProperty] = newName;
        if (this instanceof File) {
            this[extensionProperty] = fileAccess.getFileExtension(newPath);
        }
    };
    Object.defineProperty(FileSystemEntity.prototype, "name", {
        get: function () {
            return this[nameProperty];
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FileSystemEntity.prototype, "path", {
        get: function () {
            return this[pathProperty];
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FileSystemEntity.prototype, "lastModified", {
        get: function () {
            var value = this[lastModifiedProperty];
            if (!this[lastModifiedProperty]) {
                value = this[lastModifiedProperty] = getFileAccess().getLastModified(this.path);
            }
            return value;
        },
        enumerable: true,
        configurable: true
    });
    return FileSystemEntity;
}());
exports.FileSystemEntity = FileSystemEntity;
var File = (function (_super) {
    __extends(File, _super);
    function File() {
        _super.apply(this, arguments);
    }
    File.fromPath = function (path) {
        var onError = function (error) {
            throw error;
        };
        var fileInfo = getFileAccess().getFile(path, onError);
        if (!fileInfo) {
            return undefined;
        }
        return createFile(fileInfo);
    };
    File.exists = function (path) {
        return getFileAccess().fileExists(path);
    };
    Object.defineProperty(File.prototype, "extension", {
        get: function () {
            return this[extensionProperty];
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(File.prototype, "isLocked", {
        get: function () {
            return !!this[fileLockedProperty];
        },
        enumerable: true,
        configurable: true
    });
    File.prototype.readSync = function (onError) {
        this.checkAccess();
        this[fileLockedProperty] = true;
        var that = this;
        var localError = function (error) {
            that[fileLockedProperty] = false;
            if (onError) {
                onError(error);
            }
        };
        var content = getFileAccess().read(this.path, localError);
        this[fileLockedProperty] = false;
        return content;
    };
    File.prototype.writeSync = function (content, onError) {
        this.checkAccess();
        try {
            this[fileLockedProperty] = true;
            var that = this;
            var localError = function (error) {
                that[fileLockedProperty] = false;
                if (onError) {
                    onError(error);
                }
            };
            getFileAccess().write(this.path, content, localError);
        }
        finally {
            this[fileLockedProperty] = false;
        }
    };
    File.prototype.readText = function (encoding) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var localError = function (error) {
                hasError = true;
                reject(error);
            };
            var content = _this.readTextSync(localError, encoding);
            if (!hasError) {
                resolve(content);
            }
        });
    };
    File.prototype.readTextSync = function (onError, encoding) {
        this.checkAccess();
        this[fileLockedProperty] = true;
        var that = this;
        var localError = function (error) {
            that[fileLockedProperty] = false;
            if (onError) {
                onError(error);
            }
        };
        var content = getFileAccess().readText(this.path, localError, encoding);
        this[fileLockedProperty] = false;
        return content;
    };
    File.prototype.writeText = function (content, encoding) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var localError = function (error) {
                hasError = true;
                reject(error);
            };
            _this.writeTextSync(content, localError, encoding);
            if (!hasError) {
                resolve();
            }
        });
    };
    File.prototype.writeTextSync = function (content, onError, encoding) {
        this.checkAccess();
        try {
            this[fileLockedProperty] = true;
            var that = this;
            var localError = function (error) {
                that[fileLockedProperty] = false;
                if (onError) {
                    onError(error);
                }
            };
            getFileAccess().writeText(this.path, content, localError, encoding);
        }
        finally {
            this[fileLockedProperty] = false;
        }
    };
    File.prototype.checkAccess = function () {
        if (this.isLocked) {
            throw new Error("Cannot access a locked file.");
        }
    };
    return File;
}(FileSystemEntity));
exports.File = File;
var Folder = (function (_super) {
    __extends(Folder, _super);
    function Folder() {
        _super.apply(this, arguments);
    }
    Folder.fromPath = function (path) {
        var onError = function (error) {
            throw error;
        };
        var folderInfo = getFileAccess().getFolder(path, onError);
        if (!folderInfo) {
            return undefined;
        }
        return createFolder(folderInfo);
    };
    Folder.exists = function (path) {
        return getFileAccess().folderExists(path);
    };
    Folder.prototype.contains = function (name) {
        var fileAccess = getFileAccess();
        var path = fileAccess.joinPath(this.path, name);
        if (fileAccess.fileExists(path)) {
            return true;
        }
        return fileAccess.folderExists(path);
    };
    Folder.prototype.clear = function () {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var onError = function (error) {
                hasError = true;
                reject(error);
            };
            _this.clearSync(onError);
            if (!hasError) {
                resolve();
            }
        });
    };
    Folder.prototype.clearSync = function (onError) {
        getFileAccess().emptyFolder(this.path, onError);
    };
    Object.defineProperty(Folder.prototype, "isKnown", {
        get: function () {
            return this[isKnownProperty];
        },
        enumerable: true,
        configurable: true
    });
    Folder.prototype.getFile = function (name) {
        var fileAccess = getFileAccess();
        var path = fileAccess.joinPath(this.path, name);
        var onError = function (error) {
            throw error;
        };
        var fileInfo = fileAccess.getFile(path, onError);
        if (!fileInfo) {
            return undefined;
        }
        return createFile(fileInfo);
    };
    Folder.prototype.getFolder = function (name) {
        var fileAccess = getFileAccess();
        var path = fileAccess.joinPath(this.path, name);
        var onError = function (error) {
            throw error;
        };
        var folderInfo = fileAccess.getFolder(path, onError);
        if (!folderInfo) {
            return undefined;
        }
        return createFolder(folderInfo);
    };
    Folder.prototype.getEntities = function () {
        var _this = this;
        return new Promise(function (resolve, reject) {
            var hasError = false;
            var localError = function (error) {
                hasError = true;
                reject(error);
            };
            var entities = _this.getEntitiesSync(localError);
            if (!hasError) {
                resolve(entities);
            }
        });
    };
    Folder.prototype.getEntitiesSync = function (onError) {
        var fileInfos = getFileAccess().getEntities(this.path, onError);
        if (!fileInfos) {
            return null;
        }
        var entities = new Array();
        var i;
        for (i = 0; i < fileInfos.length; i++) {
            if (fileInfos[i].extension) {
                entities.push(createFile(fileInfos[i]));
            }
            else {
                entities.push(createFolder(fileInfos[i]));
            }
        }
        return entities;
    };
    Folder.prototype.eachEntity = function (onEntity) {
        if (!onEntity) {
            return;
        }
        var onSuccess = function (fileInfo) {
            var entity;
            if (fileInfo.extension) {
                entity = createFile(fileInfo);
            }
            else {
                entity = createFolder(fileInfo);
            }
            return onEntity(entity);
        };
        var onError = function (error) {
            throw error;
        };
        getFileAccess().eachEntity(this.path, onSuccess, onError);
    };
    return Folder;
}(FileSystemEntity));
exports.Folder = Folder;
var knownFolders;
(function (knownFolders) {
    var _documents;
    var _temp;
    var _app;
    knownFolders.documents = function () {
        if (!_documents) {
            var path = getFileAccess().getDocumentsFolderPath();
            _documents = new Folder();
            _documents[pathProperty] = path;
            _documents[isKnownProperty] = true;
        }
        return _documents;
    };
    knownFolders.temp = function () {
        if (!_temp) {
            var path = getFileAccess().getTempFolderPath();
            _temp = new Folder();
            _temp[pathProperty] = path;
            _temp[isKnownProperty] = true;
        }
        return _temp;
    };
    knownFolders.currentApp = function () {
        if (!_app) {
            var path = getFileAccess().getCurrentAppPath();
            _app = new Folder();
            _app[pathProperty] = path;
            _app[isKnownProperty] = true;
        }
        return _app;
    };
})(knownFolders = exports.knownFolders || (exports.knownFolders = {}));
var path;
(function (path_1) {
    function normalize(path) {
        return getFileAccess().normalizePath(path);
    }
    path_1.normalize = normalize;
    function join() {
        var paths = [];
        for (var _i = 0; _i < arguments.length; _i++) {
            paths[_i - 0] = arguments[_i];
        }
        var fileAccess = getFileAccess();
        return fileAccess.joinPaths(paths);
    }
    path_1.join = join;
    path_1.separator = getFileAccess().getPathSeparator();
})(path = exports.path || (exports.path = {}));
