var es5_visitors = (function () {

	var t = require("babel-types"),

		defaultExtendDecoratorName = "JavaProxy",
		columnOffset = 1,
		FILE_SEPARATOR = "_f",
		LINE_SEPARATOR = "_l",
		COLUMN_SEPARATOR = "_c",
		DECLARED_CLASS_SEPARATOR = "__",
		TYPESCRIPT_EXTEND_STRING = FILE_SEPARATOR + "rnal_ts_helpers_l47_c38",
		customExtendsArr = [],
		normalExtendsArr = [],
		interfacesArr = [];

	/* 	ENTRY POINT!
	*	Traverses each passed node with several visitors.
	*	Result from visit can be got from static methods.
	*
	*	Input parameters: 
	*		path - node to visit
	*		config - filename, decorator name ...
	*/
	function es5Visitor(path, config) {

		if(!config.filePath) {
			config.filePath = "No file path provided";
		}

		if(path.node.skipMeOnVisit) {
			return;
		}

	// ES5 Syntax
		// anchor is extend (normal extend pattern + custom extend pattern) 
		if (t.isMemberExpression(path) && path.node.property.name === "extend") {
			traverseEs5Extend(path, config);
		}

		//anchor is new keyword (interface pattern)
		if(t.isNewExpression(path)) {
			traverseInterface(path, config);
		}

	// // Parsed Typescript to ES5 Syntax (normal extend pattern + custom extend pattern)
	// 	// anchor is __extends
		if(t.isIdentifier(path) && path.node.name === "__extends") {
			traverseTsExtend(path, config);
		}
		// Maybe it's not a good idea to expose this scenario because it can be explicitly covered
		// //anchor is JavaProxy (optional)
		// var customDecoratorName = config.extendDecoratorName === undefined ? defaultExtendDecoratorName : config.extendDecoratorName;
		// if(t.isIdentifier(path) && path.node.name === customDecoratorName) {
			// if(path.node.skipMeOnVisit) {
			// 	return;
			// }
			// console.log("enters because there is a java proxy down the way")
			// traverseJavaProxyExtend(path, config, customDecoratorName);
		// }

	}

	/*
	*	Returns the custom extends array generated from visitor
	*/
	es5Visitor.getProxyExtendInfo = function () {
		var res = customExtendsArr.slice();
		customExtendsArr = [];
		return res;
	}

	/*
	*	Returns the common extends array generated from visitor
	*/
	es5Visitor.getCommonExtendInfo = function () {
		var res = normalExtendsArr.slice().filter(function (p) {
			if(p.startsWith("*")) {
				return false;
			}
			return p;
		});
		normalExtendsArr = [];
		return res;
	}

	/*
	*	Returns the extended interfaces array generated from visitor
	*/
	es5Visitor.getInterfaceInfo = function() {
		var res = interfacesArr.slice();
		interfacesArr = [];
		return res;
	}

	/* 
	*	Traverses the typescript extend case (__extends())
	*	Write results in "normalExtendsArr" or "customExtendsArr".
	*/
	function traverseTsExtend(path, config) {

		//this is the information for a normal extend
		var extendClass;
		try {
			extendClass = _getArgumentFromNodeAsString(path, 5, config)
		} catch (e) {
			config.logger.info(e.message)
			return;
		}
		var overriddenMethodNames = _getOverriddenMethodsTypescript(path, 3)
		var extendParent = _getParrent(path, 1, config);
		var declaredClassName = "";
		if(t.isCallExpression(extendParent)) {
			declaredClassName = extendParent.node.arguments[0].name;
		}

		// todo: check the found names in some list with predefined classes

		// check for _decorate (normal typescript extend + java proxy)

		var isDecorated = traverseToFindDecorate(path, config, extendClass, overriddenMethodNames);
		if(!isDecorated) {
			var lineToWrite = _generateLineToWrite("", extendClass, overriddenMethodNames, TYPESCRIPT_EXTEND_STRING + DECLARED_CLASS_SEPARATOR + declaredClassName, "");
			if(config.logger) {
				config.logger.info(lineToWrite)
			}
			normalExtendsArr.push(lineToWrite);
		}
	}

	/* 
	*	Traverses the passed node to find if there is a __decorate inside it.
	*	If there is __decorate identifier, then the whole node is treated as a custom extend
	*	This node is skipped next time because it's already traversed once here.
	*/
	function traverseToFindDecorate(path, config, extendClass, overriddenMethodNames) {
		var iifeRoot = _getParrent(path, 3)
		var body = iifeRoot.node.body;
		for(var index in body) {
			var ci = body[index];
			if(t.isExpressionStatement(ci) && 
					t.isAssignmentExpression(ci.expression) &&
					ci.expression.right.callee && 
					ci.expression.right.callee.name === "__decorate" &&
					ci.expression.right.arguments &&
					t.isArrayExpression(ci.expression.right.arguments[0])) {

				for(var i in ci.expression.right.arguments[0].elements) {
					var currentDecorator = ci.expression.right.arguments[0].elements[i]

					if(t.isCallExpression(currentDecorator)) {
						if(currentDecorator.callee.name === config.extendDecoratorName) {
							currentDecorator.callee.skipMeOnVisit = true;
							var customDecoratorName = config.extendDecoratorName === undefined ? defaultExtendDecoratorName : config.extendDecoratorName;
							traverseJavaProxyExtend(currentDecorator.arguments[0].value, config, customDecoratorName,  extendClass, overriddenMethodNames);
							return true;
						}
					}
				}
			}
		}
	}

	/* 
	*	Traverses the node, which is a "new" expression and find if it's a native interface or not.
	*	Write results in "interfacesArr".
	*/
	function traverseInterface(path, config) {
		if(!config.interfaceNames) {
			throw "No interface names are provided! You can pass them in config.interfaceNames as an array!"
		}

		var o = path.node.callee,
			interfaceArr = _getWholeName(o),
			foundInterface = false,
			interfaceNames = config.interfaceNames

		var currentInterface = interfaceArr.reverse().join(".");
		for(var i in interfaceNames) {
			var interfaceName = interfaceNames[i].trim();
			if(interfaceName === currentInterface) {
				currentInterface = interfaceName;
				foundInterface = true;
				break;
			}
		}

		if(foundInterface) {
			var arg0 = "",
				arg1;
			if(path.node.arguments.length === 1) {
				arg1 = path.node.arguments[0];
			}
			else if(path.node.arguments.length === 2) {
				arg0 = path.node.arguments[0];
				arg1 = path.node.arguments[1];
			}
			else {
				throw {
					message: "Not enough or too many arguments passed(" + path.node.arguments.length + ") when trying to extend interface in file: " + config.filePath,
					errCode: 1
				}
			}

			var isCorrectInterfaceName = _testClassName(arg0.value);
			var overriddenInterfaceMethods = _getOverriddenMethods(arg1, config);
			var extendInfo = 	FILE_SEPARATOR + config.filePath + LINE_SEPARATOR + path.node.loc.start.line + COLUMN_SEPARATOR + (path.node.loc.start.column + columnOffset) + DECLARED_CLASS_SEPARATOR + (isCorrectInterfaceName ? arg0.value : "");
			var lineToWrite = _generateLineToWrite("", currentInterface, overriddenInterfaceMethods.join(","), extendInfo, "");
			if(config.logger) {
				config.logger.info(lineToWrite)
			}
			interfacesArr.push(lineToWrite)
		}
	}

	/* 
	*	Finds the java proxy name from custom class decorator.
	*	Write results in "customExtendsArr"
	*/
	function traverseJavaProxyExtend(path, config, customDecoratorName, extendClass, overriddenMethodNames) {
		if(config.logger) {
			config.logger.info("\t+in "+customDecoratorName+" anchor");
		}

		var classNameFromDecorator = path;//_getDecoratorArgument(path, config, customDecoratorName);

		var lineToWrite = _generateLineToWrite(classNameFromDecorator, extendClass, overriddenMethodNames, "", config.fullPathName);
		if(config.logger) {
			config.logger.info(lineToWrite)
		}
		customExtendsArr.push(lineToWrite)
	}

	/* 
	*	Finds the normal extend name, overridden methods and possibly java proxy name from passed node.
	*	Writes to "customExtendsArr" or "normalExtendsArr".
	*	Left whole for readability.
	*/
	function traverseEs5Extend(path, config) {
		var callee = path.parent.callee;

		if(callee) {
			var o = callee.object
			extendClass = _getWholeName(o);

			var extendArguments = path.parent.arguments;
			var arg0,
				arg1;
			if (extendArguments.length === 1 && t.isObjectExpression(arg0)) {
				arg0 = extendArguments[0];
			}
			else if(t.isStringLiteral(arg0)){

			}

			var arg0 = "",
				arg1;
			if(extendArguments.length) {
				if(extendArguments.length === 1 && t.isObjectExpression(extendArguments[0])) {
					arg1 = extendArguments[0];
				}
				else if(extendArguments.length === 2) {
					if(t.isStringLiteral(extendArguments[0]) && t.isObjectExpression(extendArguments[1])) {
						arg0 = extendArguments[0];
						arg1 = extendArguments[1];
					}
				}
				else {
					// don't throw here, because there can be a valid js extend that has nothing to do with NS
					return;
					throw {
						message: "Not enough or too many arguments passed(" + extendArguments.length + ") when trying to extend class in file: " + config.filePath,
						errCode: 1
					}
				}
			}
			else {
				// don't throw here, because there can be a valid js extend that has nothing to do with NS
				return;
				throw {
					message: "You need to call the extend with parameters. Example: '...extend(\"a.b.C\", {...overrides...})') in file: " + config.filePath,
					errCode: 1
				}
			}

			className = arg0.value ? arg0.value : "";
			overriddenMethodNames = _getOverriddenMethods(arg1, config);

			var isCorrectExtendClassName = _testJavaProxyName(className);
			var isCorrectClassName = _testClassName(className);
			if(className && !isCorrectClassName && !isCorrectExtendClassName)  {
				throw {
					message: "The 'extend' you are trying to make has an invalid name. Example: '...extend(\"a.b.C\", {...overrides...})'), file: " + config.filePath,
					errCode: 1
				}
			}

			var lineToWrite = "";
			if(isCorrectExtendClassName) {
				if(config.logger) {
					config.logger.info(lineToWrite)
				}
				lineToWrite =  _generateLineToWrite(isCorrectExtendClassName ? className : "", extendClass.reverse().join("."), overriddenMethodNames, "", config.fullPathName);
				customExtendsArr.push(lineToWrite)
				return;
			}

			if(config.logger) {
				config.logger.info(lineToWrite)
			}
			var extendInfo = FILE_SEPARATOR + config.filePath + LINE_SEPARATOR + path.node.property.loc.start.line + COLUMN_SEPARATOR + (path.node.property.loc.start.column + columnOffset) + DECLARED_CLASS_SEPARATOR + className;
			lineToWrite =  _generateLineToWrite(isCorrectExtendClassName ? className : "", extendClass.reverse().join("."), overriddenMethodNames, extendInfo, "");
			normalExtendsArr.push(lineToWrite)
		}
		else {
				// don't throw here, because there can be a valid js extend that has nothing to do with NS
				return;
				throw {
					message: "You need to call the extend '...extend(\"extend_name\", {...overrides...})'), file: " + config.filePath,
					errCode: 1
				}
		}
	}

/* 
*	HELPER METHODS
*/
	function _getOverriddenMethods(node, config) {
		var overriddenMethodNames = [];
		if(t.isObjectExpression(node)) {
			var objectProperties = node.properties;
			for(var index in objectProperties) {
				overriddenMethodNames.push(objectProperties[index].key.name)
			}
		}

		return overriddenMethodNames;
	}

	function _getWholeName(node) {
		var arr = [],
			isAndroidInterface = false;

		while (node !== undefined) {
			if (!t.isMemberExpression(node)) {					
				if(isAndroidInterface) {
					arr.push(node.name)
				}
				break;
			}

			isAndroidInterface = true;
			arr.push(node.property.name)
			node = node.object
		}

		return arr;
	}

	function _getArgumentFromNodeAsString(path, count, config) {

		var extClassArr = [];
		var extendedClass =  _getParrent(path, count, config);

		if(extendedClass) {
			if(t.isCallExpression(extendedClass.node)) {
				var o = extendedClass.node.arguments[0];
			}
			else {
				throw {
					message: "Node type is not a call expression. File=" + config.filePath  + " line=" + path.node.loc.start.line,
					errCode: 1
				}
			}
		}

		extClassArr = _getWholeName(o);

		return extClassArr.reverse().join(".");
	}

	function _getDecoratorArgument(path, config, customDecoratorName) {
		if(path.parent && t.isCallExpression(path.parent)) {

			if(path.parent.arguments && path.parent.arguments.length > 0) {

				var classNameFromDecorator = path.parent.arguments[0].value
				var isCorrectExtendClassName = _testJavaProxyName(classNameFromDecorator);
				if(isCorrectExtendClassName) {
					return path.parent.arguments[0].value;
				}
				else {
					throw {
						message: "The first argument '" + classNameFromDecorator + "' of the "+customDecoratorName+" decorator is not following the right pattern which is: '[namespace.]ClassName'. Example: '"+customDecoratorName+"(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
						errCode: 1
					}
				}
			}
			else {
				throw {
					message: "No arguments passed to "+customDecoratorName+" decorator. Example: '"+customDecoratorName+"(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
					errCode: 1
				}
			}
		}
		else {
			throw { 
				message: "Decorator "+customDecoratorName+" must be called with parameters: Example: '"+customDecoratorName+"(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
				errCode: 1
			}
		}
		return undefined;
	}

	function _getOverriddenMethodsTypescript(path, count) {
		var methods = [];

		var cn = _getParrent(path, count)

		// this pattern follows typescript generated syntax
		for(var item in cn.node.body) {
			var ci = cn.node.body[item];
			if(t.isExpressionStatement(ci)) {
				if(t.isAssignmentExpression(ci.expression)) {
					if(ci.expression.left.property) {
						methods.push(ci.expression.left.property.name)
					}
				}
			}
		}

		return methods;
	}

	function _getParrent(node, numberOfParrents, config) {
		if(!node) {
			throw {
				message: "No parent found for node in file: " + config.filePath,
				errCode: 1
			}
		}
		if(numberOfParrents === 0) {
			return node;
		}

		return _getParrent(node.parentPath, --numberOfParrents)
	}

	function _testJavaProxyName(name) {
		if(name) {
			return /^((\w+\.)+\w+)$/.test(name)
		}
		return false;
	}

	function _testClassName(name) {
		if(name && name != "") {
			return /^(\w+)$/.test(name)
		}
		return false;
	}

	function _generateLineToWrite(classNameFromDecorator, extendClass, overriddenMethodNames, extendInfo, filePath) {
		var lineToWrite = extendClass + "*" + extendInfo.replace(/[\\/\\-]/g, "_") + "*" + overriddenMethodNames + "*" + classNameFromDecorator + "*" + filePath;
		return lineToWrite;
	}

	return {
		es5Visitor: es5Visitor
	}
})();

module.exports = es5_visitors;
