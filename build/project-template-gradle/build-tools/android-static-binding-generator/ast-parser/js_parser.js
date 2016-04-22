/*
*	Code takes care of static analysis and generates "out_parsed_typescript.txt"
*	The output file consists of information about custom and common bindings that should be generated.
* 	
*	test command:
*		node transpiled_ts_parser.js "input\dir\path" "output\file\path" "interface\names\file\path"
*		node transpiled_ts_parser.js "D:\work\android-static-binding-generator\project\input_parced_typescript" "D:\work\android-static-binding-generator\project\out\out_parsed_typescript.txt" "D:\work\android-static-binding-generator\project\interface-name-generator\interfaces-names.txt"
*/

///////////////// CONFIGURATION /////////////////

var disableLogger = true;
if(process.env.AST_PARSER_DISABLE_LOGGING && process.env.AST_PARSER_DISABLE_LOGGING.trim() === "true") {
	disableLogger = true;
}

loggingSettings = {
	"logPath" : require("path").dirname(require.main.filename) + "/logs/i.txt",
	"strategy" : "console",
	"APP_NAME" : "ast_parser",
	"disable": disableLogger
};

var fs = require("fs"),
	babelParser = require("babylon"),
	traverse = require("babel-traverse"),
	logger = require('./helpers/logger')(loggingSettings),
	fileHelpers = require("./helpers/file_helpers")({logger: logger}),
	path = require("path"),
	stringify = require("./helpers/json_extension"),
	es5_visitors = require("./visitors/es5-visitors"),
	t = require("babel-types"),
	filewalker = require('filewalker'),
	lazy = require("lazy"),
	eol = require('os').EOL,

	arguments = process.argv,
	appDir = path.dirname(require.main.filename),
	extendDecoratorName = "JavaProxy",
	outFile = "out/out_parsed_typescript.txt", //default out file
	inputDir = "input_parced_typescript", //default input folder
	interfacesNamesFilePath = "../interfaces-names.txt", //default interace_names file path
	interfaceNames = [];

//env variables
if(process.env.AST_PARSER_OUT_FILE) {
	outFile = process.env.AST_PARSER_OUT_FILE.trim();
}
if(process.env.AST_PARSER_INPUT_DIR) {
	inputDir = process.env.AST_PARSER_INPUT_DIR.trim();
}
if(process.env.AST_PARSER_INTERFACE_FILE_PATH) {
	interfacesNamesFilePath = process.env.AST_PARSER_INTERFACE_FILE_PATH.trim();
}


//console variables have priority
if(arguments && arguments.length >= 3) {
	inputDir = arguments[2]
	console.log("inputDir: " + inputDir)
}
if(arguments && arguments.length >= 4) {
	outFile = arguments[3]
	console.log("outFile: " + outFile)
}
if(arguments && arguments.length >= 5) {
	interfacesNamesFilePath = arguments[4]
	console.log("interface names path: " + interfacesNamesFilePath)
}

/////////////// PREPARATION ////////////////
fileHelpers.createFile(outFile)

/////////////// EXECUTE ////////////////

/*
*	Traverses a given input directory and attempts to visit every ".js" file.
*	It passes each found file down the line.
*/
var traverseFilesDir = function(filesDir) {

	if(!fs.existsSync(filesDir)) {
		throw "The input dir: " + filesDir + " does not exist!";
	}

	filewalker(filesDir)
		.on("file", function (file, info) {
			if(file.substring(file.length - 3, file.length) === '.js') {
				var currentFileName = path.join(filesDir, file);

				readFile(currentFileName)
					.then(astFromFileContent)
					// .then(writeToFile)
					.then(visitAst)
					.then(writeToFile)
					.catch(exceptionHandler)
			}
		})
		.on('error', function(err) {
			reject(err);
		})
		.walk();
}

// ENTRY POINT!
/*
*	Get's pregenerated interface names from "interfacesNamesFilePath"
*	After reading interface names runs the visiting api
*/
function readInterfaceNames() {
	return new Promise(function (resolve, reject) {		
		new lazy(fs.createReadStream(interfacesNamesFilePath))
		.lines
		.forEach(function(line){
			 interfaceNames.push(line.toString());
			 // console.log(line.toString());
	    }).on('pipe', function (err) {
	    	if(err) {
	    		reject(false);
	    	}
	 		resolve(inputDir);
		});
	})
}
readInterfaceNames().then(traverseFilesDir)


/*
*	Gets the file content as text and passes it down the line.
*/
var readFile = function (filePath, err) {
	return new Promise(function (resolve, reject) {

		fs.readFile(filePath, function (err, data) {

			if(err) {
				logger.warn("+DIDN'T get content of file!");
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

		if(err) {
			logger.warn("+DIDN'T parse ast from file!");
			return reject(err);
		}
		
		logger.info("+parsing ast from file!");
		// console.log("data: " + data.data);
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
	return new Promise (function (resolve, reject) {
		if(err) {
			logger.warn("+DIDN'T visit ast!");
			return reject(err);
		}

		logger.info("+visiting ast with given visitor library!");

		traverse.default(data.ast, {
			enter(path) {

				var decoratorConfig = {
					logger: logger,
					extendDecoratorName: extendDecoratorName,
					filePath: data.filePath.substring(inputDir.length + 1, (data.filePath.length - 3)),
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

var writeToFile = function(data, err) {

	return new Promise (function (resolve, reject) {

		if(data.trim() != "") {

			// fs.appendFile(outFile, stringify(data), function (writeFileError) {
			fs.appendFile(outFile, data + eol, function (writeFileError) {
				if(err) {
					logger.warn("Error from writeToFile: " + err);
					return reject(err);
				}
				if(writeFileError) {				
					logger.warn("Error writing file: " + writeFileError);
					return reject(writeFileError);
				}

				logger.info("+appended '" + data + "' to file: " + outFile);
				return resolve(data);
				
			});
		}
	});
}

/*
*	If there is an exception anywhere down the line it's caught here
*	If the error is criticalthe process is exited.
*/
var exceptionHandler = function (reason) {
	if(reason.errCode && reason.errCode === 1) {
		logger.error("(*)(*)(*)Error: Exception Handler Caught: " + reason.message);
		logger.error("PROCESS EXITING...");
		process.stderr.write(reason.message);
		process.exit(4);
	}
	else {
		logger.error("(*)(*)(*)Error: Exception Handler Caught: " + reason);
	}
}