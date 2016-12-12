/*
 *	Code takes care of static analysis and generates "out_parsed_typescript.txt"
 *	The output file consists of information about custom and common bindings that should be generated.
 */

///////////////// CONFIGURATION /////////////////

var disableLogger = true;
if (process.env.AST_PARSER_DISABLE_LOGGING && process.env.AST_PARSER_DISABLE_LOGGING.trim() === "true") {
	disableLogger = false;
}

loggingSettings = {
	"logPath": require("path").dirname(require.main.filename) + "/logs/i.txt",
	"strategy": "console",
	"APP_NAME": "ast_parser",
	"disable": disableLogger
};

var fs = require("fs"),
	babelParser = require("babylon"),
	traverse = require("babel-traverse"),
	logger = require('./helpers/logger')(loggingSettings),
	fileHelpers = require("./helpers/file_helpers")({ logger: logger }),
	path = require("path"),
	stringify = require("./helpers/json_extension"),
	es5_visitors = require("./visitors/es5-visitors"),
	t = require("babel-types"),
	lazy = require("lazy"),
	eol = require('os').EOL,

	arguments = process.argv,
	appDir = path.dirname(require.main.filename),
	extendDecoratorName = "JavaProxy",
	interfacesDecoratorName = "Interfaces",
	outFile = "out/out_parsed_typescript.txt", // default out file
	inputDir = "input_parced_typescript", // default input folder
	interfacesNamesFilePath = "../interfaces-names.txt", //default interace_names file path
	interfaceNames = [],
	inputFiles = [];


//env variables
if (process.env.AST_PARSER_OUT_FILE) {
	outFile = process.env.AST_PARSER_OUT_FILE.trim();
}
if (process.env.AST_PARSER_INPUT_DIR) {
	inputDir = process.env.AST_PARSER_INPUT_DIR.trim();
}
if (process.env.AST_PARSER_INTERFACE_FILE_PATH) {
	interfacesNamesFilePath = process.env.AST_PARSER_INTERFACE_FILE_PATH.trim();
}


//console variables have priority
if (arguments && arguments.length >= 3) {
	inputDir = arguments[2]
	// console.log("inputDir: " + inputDir)
}
if (arguments && arguments.length >= 4) {
	outFile = arguments[3]
	// console.log("outFile: " + outFile)
}
if (arguments && arguments.length >= 5) {
	interfacesNamesFilePath = arguments[4]
	// console.log("interfacesNamesFilePath: " + interfacesNamesFilePath)
}
if (arguments && arguments.length >= 6) {
	inputFilesPath = arguments[5]
}


/////////////// PREPARATION ////////////////
// fileHelpers.createFile(outFile)

/////////////// EXECUTE ////////////////
var tsHelpersFilePath = path.join(inputDir, "..", "internal", "ts_helpers.js");

// ENTRY POINT!
readLinesFromFile(inputFilesPath, inputFiles, tsHelpersFilePath)
	.then(getFileAst)
	.then(getExtendsLineColumn) //config
	.then(readInterfaceNames) //config
	.then(traverseAndAnalyseFilesDir) //start
	.catch(exceptionHandler);

/*
*	Get's the javascript files that need traversing
*/
function readLinesFromFile(filePath, outArr, resolveParameter) {
	return new Promise(function (resolve, reject) {
		new lazy(fs.createReadStream(filePath))
			.lines
			.forEach(function (line) {
				outArr.push(line.toString().trim());
			}).on('pipe', function (err) {
				if (err) {
					return reject(err);
				}

				console.log("finished with reading lines with js files");

				return resolve(resolveParameter)
			});
	});
}


/*
*	Get line and column of the __extends function from ts_helpers file
*/
function getFileAst(tsHelpersFilePath) {	
	return new Promise(function (resolve, reject) {
		fs.readFile(tsHelpersFilePath, 'utf8', function(err, fileContent) {
			if (err) {				
				logger.warn("+DIDN'T parse ast from file " + tsHelpersFilePath);
				return reject(err);
			}

			logger.info("+parsing ast from " + tsHelpersFilePath);

			var ast = babelParser.parse(fileContent, {
				minify: false,
				plugins: ["decorators"]
			});			

			return resolve(ast);
		});
	});
};

/*
*	Get line and column of the extend function in the tsHelpers.js file
* 	(Line and column are used as identifiers for the typescript extended classes!)
*/
function getExtendsLineColumn(ast) {
	return new Promise(function (resolve, reject) {

		var tsHelpersInfo = {};
		traverse.default(ast, {
			enter: function(path) {

				if(t.isAssignmentExpression(path.parent) &&
					t.isCallExpression(path) &&
					 path.node.callee.property &&
					 path.node.callee.property.name === "extend" &&
					 path.node.callee.object.name === "parent") {
						tsHelpersInfo.line = path.node.callee.property.loc.start.line
						tsHelpersInfo.column = path.node.callee.property.loc.start.column + 1
				}
			}
		})

		es5_visitors.setLineAndColumn(tsHelpersInfo);
		return resolve(true);
	});
};

/*
*	Get's pregenerated interface names from "interfacesNamesFilePath"
*	After reading interface names runs the visiting api
*/
function readInterfaceNames(data, err) {
	return new Promise(function (resolve, reject) {
		new lazy(fs.createReadStream(interfacesNamesFilePath))
			.lines
			.forEach(function (line) {
				interfaceNames.push(line.toString());
			}).on('pipe', function (err) {
				if (err) {
					return reject(false);
				}

				inputDir = path.normalize(inputDir);

				return resolve(inputDir);
			});
	})
}

/*
*	Traverses a given input directory and attempts to visit every ".js" file.
*	It passes each found file down the line.
*/
function traverseAndAnalyseFilesDir(inputDir, err) {
	if (!fs.existsSync(inputDir)) {
		throw "The input dir: " + inputDir + " does not exist!";
	}

	traverseFiles(inputFiles);
}

function traverseFiles(filesToTraverse) {

	var filesLength = filesToTraverse.length;
	for(var i = 0; i < filesLength; i += 1) {
		var fp = filesToTraverse[i];
		logger.info("Visiting JavaScript file: " + fp);

		readFile(fp)
		.then(astFromFileContent)
		.then(visitAst)
		.then(writeToFile)
		.catch(exceptionHandler)
	}
}

/*
*	Gets the file content as text and passes it down the line.
*/
var readFile = function (filePath, err) {
	
	return new Promise(function (resolve, reject) {
		fs.readFile(filePath, function (err, data) {
			if (err) {
				logger.warn("+DIDN'T get content of file: " + filePath);
				return reject(err);
			}

			logger.info("+got content of file!");
			var fileInfo = {
				filePath: filePath,
				data: data.toString()
			}
			return resolve(fileInfo);
		});
	});
}

/*
*	Get's the AST (https://en.wikipedia.org/wiki/Abstract_syntax_tree) from the file content and passes it down the line.
*/
var astFromFileContent = function (data, err) {
	return new Promise(function (resolve, reject) {		
		if (err) {
			logger.warn("+DIDN'T parse ast from file!");
			return reject(err);
		}

		logger.info("+parsing ast from file!");

		var ast = babelParser.parse(data.data, {
			minify: false,
			plugins: ["decorators"]
		});
		data.ast = ast;
		return resolve(data);
	});
};

//only unique filter
function onlyUnique(value, index, self) {
	return self.indexOf(value) === index;
}

/*
*	Visist's the passed AST with a given visitor and extracts nativescript speciffic data.
*	Passes the extracted bindings data down the line.
*/
var visitAst = function (data, err) {
	return new Promise(function (resolve, reject) {
		if (err) {
			logger.warn("+DIDN'T visit ast!");
			return reject(err);
		}

		traverse.default(data.ast, {
			enter: function (path) {
				var decoratorConfig = {
					logger: logger,
					extendDecoratorName: extendDecoratorName,
					interfacesDecoratorName: interfacesDecoratorName,
					filePath: data.filePath.substring(inputDir.length + 1, (data.filePath.length - 3)) || "",
					fullPathName: data.filePath.substring(inputDir.length + 1).replace(/[\\]/g, "/"),
					interfaceNames: interfaceNames
				};
				es5_visitors.es5Visitor(path, decoratorConfig);
			}
		})

		var customExtendsArr = es5_visitors.es5Visitor.getProxyExtendInfo()
		var normalExtendsArr = es5_visitors.es5Visitor.getCommonExtendInfo()
		var interfacesArr = es5_visitors.es5Visitor.getInterfaceInfo()

		var res = customExtendsArr.concat(normalExtendsArr).concat(interfacesArr).filter(onlyUnique).join(eol);
		return resolve(res)
	});
}

var writeToFile = function (data, err) {	
	return new Promise(function (resolve, reject) {
		if (data.trim() != "") {
			// fs.appendFile(outFile, stringify(data), function (writeFileError) {
			fs.appendFile(outFile, data + eol, function (writeFileError) {
				if (err) {
					logger.warn("Error from writeToFile: " + err);
					return reject(err);
				}
				if (writeFileError) {
					logger.warn("Error writing file: " + writeFileError);
					return reject(writeFileError);
				}

				logger.info("+appended '" + data + "' to file: " + outFile);
				return resolve(data);

			});
		}
		else {
			logger.info("No need to generate anything. (UP-TO-DATE)");
		}
	});
}

/*
*	If there is an exception anywhere down the line it's caught here
*	If the error is criticalthe process is exited.
*/
var exceptionHandler = function (reason) {
	if (reason.errCode && reason.errCode === 1) {
		logger.error("(*)(*)(*)Error: Exception Handler Caught: " + reason.message);
		logger.error("PROCESS EXITING...");
		process.stderr.write(reason.message);
		process.exit(4);
	}
	else {
		logger.error("(*)(*)(*)Error: Exception Handler Caught: " + reason);
	}
}
