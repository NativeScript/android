var fs = require("fs"),
	path = require("path");
module.exports = function Helpers(config) {

	function cleanOutFile(filePath) {
		fs.truncateSync(filePath, 0);
		if(config && config.logger) {
			config.logger.info("+cleared out file: " + filePath);
		}
	}

	function createFile(filePath) {
		if(ensureDirectories(filePath)) {
			fs.writeFileSync(filePath, "");
			if(config && config.logger) {
				config.logger.info("+created ast output file: ");
			}
		}
		cleanOutFile(filePath)
	}

	function ensureDirectories(filePath) {
		var parentDir = path.dirname(filePath);
		if(fs.existsSync(parentDir)) {
			return true;
		}

		ensureDirectories(parentDir);
		fs.mkdirSync(parentDir);
		return true;
	}

	return {
		cleanOutFile: cleanOutFile,
		createFile: createFile,
		ensureDirectories: ensureDirectories
	}
};