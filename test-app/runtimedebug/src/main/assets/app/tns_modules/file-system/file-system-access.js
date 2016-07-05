var textModule = require("text");
var utils = require("utils/utils");
var FileSystemAccess = (function () {
    function FileSystemAccess() {
        this._pathSeparator = "/";
    }
    FileSystemAccess.prototype.getLastModified = function (path) {
        var javaFile = new java.io.File(path);
        return new Date(javaFile.lastModified());
    };
    FileSystemAccess.prototype.getParent = function (path, onError) {
        try {
            var javaFile = new java.io.File(path);
            var parent = javaFile.getParentFile();
            return { path: parent.getAbsolutePath(), name: parent.getName() };
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
            return undefined;
        }
    };
    FileSystemAccess.prototype.getFile = function (path, onError) {
        return this.ensureFile(new java.io.File(path), false, onError);
    };
    FileSystemAccess.prototype.getFolder = function (path, onError) {
        var javaFile = new java.io.File(path);
        var dirInfo = this.ensureFile(javaFile, true, onError);
        if (!dirInfo) {
            return undefined;
        }
        return { path: dirInfo.path, name: dirInfo.name };
    };
    FileSystemAccess.prototype.eachEntity = function (path, onEntity, onError) {
        if (!onEntity) {
            return;
        }
        this.enumEntities(path, onEntity, onError);
    };
    FileSystemAccess.prototype.getEntities = function (path, onError) {
        var fileInfos = new Array();
        var onEntity = function (entity) {
            fileInfos.push(entity);
            return true;
        };
        var errorOccurred;
        var localError = function (error) {
            if (onError) {
                onError(error);
            }
            errorOccurred = true;
        };
        this.enumEntities(path, onEntity, localError);
        if (!errorOccurred) {
            return fileInfos;
        }
        return null;
    };
    FileSystemAccess.prototype.fileExists = function (path) {
        var file = new java.io.File(path);
        return file.exists();
    };
    FileSystemAccess.prototype.folderExists = function (path) {
        var file = new java.io.File(path);
        return file.exists() && file.isDirectory();
    };
    FileSystemAccess.prototype.deleteFile = function (path, onError) {
        try {
            var javaFile = new java.io.File(path);
            if (!javaFile.isFile()) {
                if (onError) {
                    onError({ message: "The specified parameter is not a File entity." });
                }
                return;
            }
            if (!javaFile.delete()) {
                if (onError) {
                    onError({ message: "File deletion failed" });
                }
            }
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.deleteFolder = function (path, onError) {
        try {
            var javaFile = new java.io.File(path);
            if (!javaFile.getCanonicalFile().isDirectory()) {
                if (onError) {
                    onError({ message: "The specified parameter is not a Folder entity." });
                }
                return;
            }
            this.deleteFolderContent(javaFile);
            if (!javaFile.delete()) {
                if (onError) {
                    onError({ message: "Folder deletion failed." });
                }
            }
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.emptyFolder = function (path, onError) {
        try {
            var javaFile = new java.io.File(path);
            if (!javaFile.getCanonicalFile().isDirectory()) {
                if (onError) {
                    onError({ message: "The specified parameter is not a Folder entity." });
                }
                return;
            }
            this.deleteFolderContent(javaFile);
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.rename = function (path, newPath, onError) {
        var javaFile = new java.io.File(path);
        if (!javaFile.exists()) {
            if (onError) {
                onError(new Error("The file to rename does not exist"));
            }
            return;
        }
        var newFile = new java.io.File(newPath);
        if (newFile.exists()) {
            if (onError) {
                onError(new Error("A file with the same name already exists."));
            }
            return;
        }
        if (!javaFile.renameTo(newFile)) {
            if (onError) {
                onError(new Error("Failed to rename file '" + path + "' to '" + newPath + "'"));
            }
        }
    };
    FileSystemAccess.prototype.getDocumentsFolderPath = function () {
        var dir = utils.ad.getApplicationContext().getFilesDir();
        return dir.getAbsolutePath();
    };
    FileSystemAccess.prototype.getLogicalRootPath = function () {
        var dir = utils.ad.getApplicationContext().getFilesDir();
        return dir.getCanonicalPath();
    };
    FileSystemAccess.prototype.getTempFolderPath = function () {
        var dir = utils.ad.getApplicationContext().getCacheDir();
        return dir.getAbsolutePath();
    };
    FileSystemAccess.prototype.getCurrentAppPath = function () {
        return this.getLogicalRootPath() + "/app";
    };
    FileSystemAccess.prototype.read = function (path, onError) {
        try {
            var javaFile = new java.io.File(path);
            var stream = new java.io.FileInputStream(javaFile);
            var bytes = Array.create("byte", javaFile.length());
            var dataInputStream = new java.io.DataInputStream(stream);
            dataInputStream.readFully(bytes);
            return bytes;
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.write = function (path, bytes, onError) {
        try {
            var javaFile = new java.io.File(path);
            var stream = new java.io.FileOutputStream(javaFile);
            stream.write(bytes, 0, bytes.length);
            stream.close();
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.readText = function (path, onError, encoding) {
        try {
            var types = require("utils/types");
            var javaFile = new java.io.File(path);
            var stream = new java.io.FileInputStream(javaFile);
            var actualEncoding = encoding;
            if (!actualEncoding) {
                actualEncoding = textModule.encoding.UTF_8;
            }
            var reader = new java.io.InputStreamReader(stream, actualEncoding);
            var bufferedReader = new java.io.BufferedReader(reader);
            var line = undefined;
            var result = "";
            while (true) {
                line = bufferedReader.readLine();
                if (types.isNullOrUndefined(line)) {
                    break;
                }
                if (result.length > 0) {
                    result += "\n";
                }
                result += line;
            }
            if (actualEncoding === textModule.encoding.UTF_8) {
                result = FileSystemAccess._removeUtf8Bom(result);
            }
            bufferedReader.close();
            return result;
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess._removeUtf8Bom = function (s) {
        if (s.charCodeAt(0) === 0xFEFF) {
            s = s.slice(1);
        }
        return s;
    };
    FileSystemAccess.prototype.writeText = function (path, content, onError, encoding) {
        try {
            var javaFile = new java.io.File(path);
            var stream = new java.io.FileOutputStream(javaFile);
            var actualEncoding = encoding;
            if (!actualEncoding) {
                actualEncoding = textModule.encoding.UTF_8;
            }
            var writer = new java.io.OutputStreamWriter(stream, actualEncoding);
            writer.write(content);
            writer.close();
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.deleteFolderContent = function (file) {
        var filesList = file.listFiles();
        if (filesList.length === 0) {
            return true;
        }
        var i, childFile, success = false;
        for (i = 0; i < filesList.length; i++) {
            childFile = filesList[i];
            if (childFile.getCanonicalFile().isDirectory()) {
                success = this.deleteFolderContent(childFile);
                if (!success) {
                    break;
                }
            }
            success = childFile.delete();
        }
        return success;
    };
    FileSystemAccess.prototype.ensureFile = function (javaFile, isFolder, onError) {
        try {
            if (!javaFile.exists()) {
                var created;
                if (isFolder) {
                    created = javaFile.mkdirs();
                }
                else {
                    javaFile.getParentFile().mkdirs();
                    created = javaFile.createNewFile();
                }
                if (!created) {
                    if (onError) {
                        onError("Failed to create new java File for path " + javaFile.getAbsolutePath());
                    }
                    return undefined;
                }
                else {
                    javaFile.setReadable(true);
                    javaFile.setWritable(true);
                }
            }
            var path = javaFile.getAbsolutePath();
            return { path: path, name: javaFile.getName(), extension: this.getFileExtension(path) };
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
            return undefined;
        }
    };
    FileSystemAccess.prototype.getFileExtension = function (path) {
        var dotIndex = path.lastIndexOf(".");
        if (dotIndex && dotIndex >= 0 && dotIndex < path.length) {
            return path.substring(dotIndex);
        }
        return "";
    };
    FileSystemAccess.prototype.enumEntities = function (path, callback, onError) {
        try {
            var javaFile = new java.io.File(path);
            if (!javaFile.getCanonicalFile().isDirectory()) {
                if (onError) {
                    onError("There is no folder existing at path " + path);
                }
                return;
            }
            var filesList = javaFile.listFiles();
            var length = filesList.length;
            var i;
            var info;
            var retVal;
            for (i = 0; i < length; i++) {
                javaFile = filesList[i];
                info = {
                    path: javaFile.getAbsolutePath(),
                    name: javaFile.getName()
                };
                if (javaFile.isFile()) {
                    info.extension = this.getFileExtension(info.path);
                }
                retVal = callback(info);
                if (retVal === false) {
                    break;
                }
            }
        }
        catch (exception) {
            if (onError) {
                onError(exception);
            }
        }
    };
    FileSystemAccess.prototype.getPathSeparator = function () {
        return this._pathSeparator;
    };
    FileSystemAccess.prototype.normalizePath = function (path) {
        var file = new java.io.File(path);
        return file.getAbsolutePath();
    };
    FileSystemAccess.prototype.joinPath = function (left, right) {
        var file1 = new java.io.File(left);
        var file2 = new java.io.File(file1, right);
        return file2.getAbsolutePath();
    };
    FileSystemAccess.prototype.joinPaths = function (paths) {
        if (!paths || paths.length === 0) {
            return "";
        }
        if (paths.length === 1) {
            return paths[0];
        }
        var i, result = paths[0];
        for (i = 1; i < paths.length; i++) {
            result = this.joinPath(result, paths[i]);
        }
        return this.normalizePath(result);
    };
    return FileSystemAccess;
}());
exports.FileSystemAccess = FileSystemAccess;
