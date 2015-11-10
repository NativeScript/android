/**
* example command:
*		node traverse_files.js [interface-names.txt] [dir_to_traverse] [out_dir_name]
*
* This file generates AST (http://en.wikipedia.org/wiki/Abstract_syntax_tree) on given js file and
* extracts class and interface full names
* e.g "android.widget.button.(extend)" -> extended class
* "android.view.View.OnClickListener" -> interface
* necessary to create bindings and
* outputs them into a file
*/

var uglifyjs = require('uglify-js');
var filewalker = require('filewalker');
var lazy = require("lazy");
var fs = require('fs');

var arguments = process.argv;
var outFile;
var LINE_OFFSET = 2; //line offset because of module wrapper

//no idea why exactly 5 and 3 (maybe because of tabs difference in parsers)
var CLASS_COLUMN_OFFSET = 5;
var INTERFACE_COLUMN_OFFSET = 3;

if(arguments.length == 3) {
	console.log('you need to pass input file to traverse e.g. "node traverse_files.js [interface-names.txt] [dir_to_traverse]"');
	return;
}
if(arguments.length == 4) {
	// console.log('you can pass an out file name as a parameter! e.g. "node traverse_files.js [interface-names.txt] [dir_to_traverse] [out_dir_name]"');
	outFile = 'bindings.txt';
}
else {
	outFile = arguments[4];
}

var interfacesNamesFileName = arguments[2];
var inputDir = arguments[3];

var classNamesNeedExtending = [];
var filesToTraverse = [];
var interfaceNames = [];
var javaSeparator = '_';
var fileSeparator = javaSeparator + 'f';// + javaSeparator + javaSeparator;
var lineSeparator = javaSeparator + 'l';
var columnSeparator = javaSeparator + 'c';
var customClassNameSeparator = javaSeparator + javaSeparator;
var resultJson;

new lazy(fs.createReadStream(interfacesNamesFileName)).lines
	.forEach(function(line){
		 interfaceNames.push(line.toString());
    }).on('pipe', function (err) {
		var filename = inputDir;
		traverseFolder(filename);
	
		if (err) throw err;
	  
		console.log('OK: ' + filename + '\n');
	});
 
function traverseFolder(inputFolder){

	filewalker(inputFolder)
	  .on('file', function(p, s) {
		  
		  //if in the folder there is a file with .js extensions
		if(p.substring(p.length - 3, p.length) === '.js' && p.indexOf("ios.js") == -1) {
			var currentFileName = inputFolder + '/' + p;
			
			//TODO: think how to fix circular reference problem 
			if(currentFileName.indexOf('easysax.js') == -1) { //exception because of maximum call stack exceeded (circular reference)
				filesToTraverse.push(currentFileName);
				// console.log(currentFileName);
			}
		}
	  })
	  .on('error', function(err) {
		console.error(err);
	  })
	  .on('done', function() {
		createClearFile();
		
		for(var index in filesToTraverse) {
			parseFile(filesToTraverse[index]);
		}
	  })
	.walk();
}

function parseFile(fullFilename) {
	fs.readFile(fullFilename, 'utf8', function(err, data) {
		if (err) {
			throw err; 
		}
		
		classNamesNeedExtending = [];
		
		parseDataFromFile(data, fullFilename);
	});
}

function parseDataFromFile(data, fullFilename) {
	
	var ast = uglifyjs.parse(data.toString());
	
	traverseAstTree(ast, checkPattern, fullFilename);
}

//traverse nodes with some visitor
function traverseAstTree(node, visitor, fullFilename){

	//visitor checks for patterns
    visitor(node, fullFilename);
	
	// if(typeof node === 'object') {
	
        for(var item in node) {
		
			if(typeof node[item] === 'object' && node[item] !== null && node[item] != 0) {
			
				traverseAstTree(node[item], visitor, fullFilename);
			}
		}
	// }
}

//extract information from classes and interfaces necessary to generate proxy
function checkPattern(node, fullFilename){
	
	if(node.start) {
	
		var className;
		var customExtendClassName;
		var overridenMethods;
		
		if(node.expression) {
		
			className  = extractFullNodeName(node);
			overridenMethods = getOverridenMethods(node);
			
			var fullLocationOfFile = fileSeparator + getFilename(fullFilename);
			fullLocationOfFile += lineSeparator + (parseInt(node.expression.end.line));// + LINE_OFFSET);
			
			//check if this expression is an interface (PATTERN)
			if(node.start.value == 'new') {
				
				for(var index in interfaceNames) {
					
					if(interfaceNames[index].trim() == className) {
						customExtendClassName = getExtendedClassName(node);
						fullLocationOfFile += columnSeparator + (parseInt(node.expression.start.col) -  INTERFACE_COLUMN_OFFSET);
						className += fullLocationOfFile;
						className += customClassNameSeparator + customExtendClassName;
						
						if(!nameContainsInvalidSymbols(customExtendClassName)) {
							// console.log(className);
							var lineToWrite = className + ' ' + overridenMethods.join();
							appendToFile(lineToWrite);
							return;
						}
					}
				}
			}
				
			//check if expression is extended class (PATTERN)
			if(node.expression.property == 'extend') {
				customExtendClassName = getExtendedClassName(node);
				fullLocationOfFile += columnSeparator + (parseInt(node.expression.end.endcol) - CLASS_COLUMN_OFFSET);
				className += fullLocationOfFile;
				className += customClassNameSeparator + customExtendClassName;
				
				if(!nameContainsInvalidSymbols(customExtendClassName)) {// && className.indexOf('com.tns.tests') == -1) { //binding generator takes care of this
					var lineToWrite = className + ' ' + overridenMethods.join();
					// console.log(className);
					appendToFile(lineToWrite);
					return;
				}
			}
			
			// check if expression is extended typescript class (PATTERN)
			var isValidExtendCandidate = false;
			if(node.expression.body) {
				for(var index in node.expression.body){
					var currentAstToken = node.expression.body[index];
					if(currentAstToken.body) {
						if(currentAstToken.body.expression) {
							
							// the function "__extends" is the way typescript extensions are done
							if( currentAstToken.body.expression.name == '__extends') {						
								
								// get class name to extend from what's passed to iife
								className = extractFullNodeName(node.args[0]);
								
								// get return type of iife
								for(var i in node.expression.body) {			
									var possibleOverridenMethod = node.expression.body[i];
									if(currentAstToken.start) {
										if(possibleOverridenMethod.start.value == 'return') {
											customExtendClassName = possibleOverridenMethod.value.name;
											isValidExtendCandidate = true;
										}
									}
								}
								
								// find overriden methods from top node
								for(var i in node.expression.body) {			
									var possibleOverridenMethod = node.expression.body[i];
									if(possibleOverridenMethod.body) {
										if(possibleOverridenMethod.body.left) {
											var overridenMethodName = getExtendingCtorName(possibleOverridenMethod.body.left);
											if(overridenMethodName == customExtendClassName) {
												var possibleOverridenProp = possibleOverridenMethod.body.left.property;
												
												if(typeof possibleOverridenProp == 'string') {
													overridenMethods.push(possibleOverridenProp);
												}												
											}
										}
									}
								}
							}
						}
					}
				}
				if(isValidExtendCandidate) {
					var extendLocation = '_frnal_prepareExtend_l60_c37__';
					className += extendLocation;
					className += customExtendClassName;
					if(!nameContainsInvalidSymbols(customExtendClassName)) {
						if(customExtendClassName) {
							var lineToWrite = className + ' ' + overridenMethods.join();
							// console.log(className);
							appendToFile(lineToWrite);
							return;
						}
					}
				}
			}
		}
	}
}

function getExtendingCtorName(node) {
	if(node.expression) {
		return getExtendingCtorName(node.expression);
	}
	if(node.name) {
		return node.name;
	}
}

function nameContainsInvalidSymbols(name) {
	for(var index in name) {
		if(!(name[index] >= 'a' && name[index] <= 'z') &&
			!(name[index] >= 'A' && name[index] <= 'Z') && 
			!(name[index] >= '0' && name[index] <= '9') && 
			name[index] != javaSeparator) {
			return true;
		}
	}
	return false;
}

function getFilename(fullFilename){
	var finalFilename;
	var startIndex = inputDir.length + 1;
	var endIndexWithAndroid = fullFilename.indexOf('.android.js');
	var endIndexWithoutAndroid = fullFilename.indexOf('.js');
	if(endIndexWithAndroid != -1) {
		finalFilename = fullFilename.substring(startIndex, endIndexWithAndroid);
	}
	else if(endIndexWithoutAndroid != -1) {
		finalFilename = fullFilename.substring(startIndex, endIndexWithoutAndroid);
	}
	
	finalFilename = finalFilename.replace(/[-/]/g, javaSeparator); //replace ['/', '-'] ---> with '_'
	
	return finalFilename;
}

function extractFullNodeName(node) {
	var expressionArr = [];
	
	// if(node) {
		getNameInfo(node, expressionArr);
	// }
	// else {
		// getNameInfo(node.expression, expressionArr);
	// }
	
	expressionArr.reverse();
	var resultString = expressionArr.join('.');
	
	return resultString;
}

function getOverridenMethods(node) {
	var overridenMethods = [];
	
	if(node.args ){// && (node.start.value == 'new' || node.property !== 'extend')) {
		getOverridenMethodsNames(node.args, overridenMethods);
	}
	
	return overridenMethods;
}

function getOverridenMethodsNames(argumentsNode, overridenMethods) {
	for(var index in argumentsNode) {
		var functionArgument = argumentsNode[index];
		
		if(functionArgument.properties){
			var props = functionArgument.properties;
			for(var index in props) {
				var nameOfOverriddenMethod = props[index].key;
				if(typeof nameOfOverriddenMethod == 'string') {
					overridenMethods.push(nameOfOverriddenMethod);
				}
			}
		}
	}
}

function getExtendedClassName(node) {
	
	var customExtendClassName = "";
	
	if(node.args) {
		
		if(node.args.length == 2) {
			var functionArgument = node.args[0];
			customExtendClassName = functionArgument.value;
		}
	}
	
	return customExtendClassName;
}

function getNameInfo(node, expressionArr) {

	if(node.property && node.property !== 'extend'){
		expressionArr.push(node.property);
	}
	else if(node.name) {
		expressionArr.push(node.name);
	}
	
	if(node.expression) {
		getNameInfo(node.expression, expressionArr);
	}
}

function createClearFile() {
	// create and clear file
	if (fs.existsSync(outFile)) {
		fs.truncate(outFile, 0, function(err) {
		
			if(err) throw err;
			
			console.log('created \ cleared file')
		});	
	}
}

function appendToFile(line) {
	fs.appendFile(outFile, line + '\n', function (err) {
		if (err) throw err;
	});
}