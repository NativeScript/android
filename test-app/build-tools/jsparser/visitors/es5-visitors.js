var es5_visitors = (function () {

    var types = require("babel-types"),

        defaultExtendDecoratorName = "JavaProxy",
        columnOffset = 1,
        ASTERISK_SEPARATOR = "*",
        customExtendsArr = [],
        normalExtendsArr = [],
        interfacesArr = [],
        UNSUPPORTED_TYPESCRIPT_EXTEND_FORMAT_MESSAGE = "[WARN] TypeScript-extended class has a super() call in an unsupported format.",

        customExtendsArrGlobal = [];

	/* 	ENTRY POINT!
	*	Traverses each passed node with several visitors.
	*	Result from visit can be got from static methods.
	*
	*	Input parameters: 
	*		path - node to visit
	*		config - filename, decorator name ...
	*/
    function es5Visitor(path, config) {

        if (!config.filePath) {
            config.filePath = "No file path provided";
        }

        if (path.node.skipMeOnVisit) {
            return;
        }

        // ES5 Syntax
        // anchor is extend (normal extend pattern + custom extend pattern) 
        if (types.isMemberExpression(path) && path.node.property.name === "extend") {
            traverseEs5Extend(path, config);
        }

        //anchor is new keyword (interface pattern)
        if (types.isNewExpression(path)) {
            traverseInterface(path, config);
        }

        // // Parsed Typescript to ES5 Syntax (normal extend pattern + custom extend pattern)
        // 	// anchor is __extends
        if (types.isIdentifier(path) && path.node.name === "__extends") {
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
   *       Returns the common extends array generated from visitor
   */
    es5Visitor.getCommonExtendInfo = function () {
        var res = [];
        for (var index in normalExtendsArr) {
            if (normalExtendsArr[index][0] !== "*") {
                res.push(normalExtendsArr[index]);
            }
        }

        normalExtendsArr = [];
        return res;
    }

	/*
	*	Returns the extended interfaces array generated from visitor
	*/
    es5Visitor.getInterfaceInfo = function () {
        var res = interfacesArr.slice();
        interfacesArr = [];
        return res;
    }

	/* 
	*	Traverses the typescript extend case (__extends())
	*	Write results in "normalExtendsArr" or "customExtendsArr".
	*/
    function traverseTsExtend(path, config) {
        // information for normal extend (unnamed)
        var extendClass;
        try {
            extendClass = _getArgumentFromNodeAsString(path, 5, config)
        } catch (e) {
            config.logger.info(e.message)
            return;
        }

        var overriddenMethodNames = _getOverriddenMethodsTypescript(path, 3);

        var extendPath = _getParent(path, 3, config);
        var declaredClassName = "";

        var typescriptClassExtendSuperCallLocation = getTypeScriptExtendSuperCallLocation(extendPath, config);
        var extendParent = _getParent(path, 1, config);

        if (types.isCallExpression(extendParent)) {
            declaredClassName = extendParent.node.arguments[0].name;
        }

        var decorateNodes = traverseForDecorate(path, config, 3);

        var isDecoratedWithExtend = false,
            customExtendDecoratorName,
            customExtendDecoratorValue,
            implementedInterfaces = [];

        if (!decorateNodes) {
            // 7 -> Takes 7 levels up to get to the scope where the class is declared
            decorateNodes = traverseForDecorateSpecial(path, config, 7);
        }

        if (decorateNodes) {
            for (var i in decorateNodes) {
                var currentDecorator = decorateNodes[i];
                if (types.isCallExpression(currentDecorator)) {
                    // Interfaces/Implements
                    if (currentDecorator.callee.name === config.interfacesDecoratorName) {
                        currentDecorator.callee.skipMeOnVisit = true;

                        var interfaces = currentDecorator.arguments[0].elements;

                        for (var i in interfaces) {
                            var interfaceName = _getWholeInterfaceNameFromInterfacesNode(interfaces[i]);
                            implementedInterfaces.push(interfaceName);
                        }
                    }

                    // JavaProxy
                    if (currentDecorator.callee.name === config.extendDecoratorName) {
                        currentDecorator.callee.skipMeOnVisit = true;

                        isDecoratedWithExtend = true;

                        customExtendDecoratorName = config.extendDecoratorName === undefined ? defaultExtendDecoratorName : config.extendDecoratorName;
                        customExtendDecoratorValue = currentDecorator.arguments[0].value;
                    }
                }
            }
        }

        if (isDecoratedWithExtend) {
            traverseJavaProxyExtend(customExtendDecoratorValue, config, customExtendDecoratorName, extendClass, overriddenMethodNames, implementedInterfaces);
        } else {
            var lineToWrite;

            lineToWrite = _generateLineToWrite("", extendClass, overriddenMethodNames, { file: config.filePath, line: typescriptClassExtendSuperCallLocation.line, column: typescriptClassExtendSuperCallLocation.column, className: declaredClassName }, "", implementedInterfaces);

            if (config.logger) {
                config.logger.info(lineToWrite)
            }

            normalExtendsArr.push(lineToWrite);
        }
    }

    function getTypeScriptExtendSuperCallLocation(extendPath, config) {
        var constructorFunctionName;
        var returnIdentifierName;
        var superCalleeStartColumn;
        var superCalleeLine;
        var locationAssigned = false;

        // checks if constructor function and return identifiers match in a transpiled typescript class extend
        if (extendPath.node.body && extendPath.node.body.length >= 3) {
            if (types.isFunctionDeclaration(extendPath.node.body[1]) && extendPath.node.body[1].id)
                constructorFunctionName = extendPath.node.body[1].id.name;
            if (types.isReturnStatement(extendPath.node.body[extendPath.node.body.length - 1]))
                returnIdentifierName = extendPath.node.body[extendPath.node.body.length - 1].argument.name;
        }

        // checks the typescript-extended class for a strict format, and a super call/apply inside
        /**
         *      function MyBtn() {
                    var _this = _super.call(this) || this;
                    return global.__native(_this);
                }

                **UGLIFIED extended class**

                function t(t) {
                    var r = e.call(this) || this;
                    r.textBase = t;
                    return global.__native(r);
                }
         */
        if (constructorFunctionName === returnIdentifierName && !!constructorFunctionName) {
            var constructorFunctionScope = extendPath.node.body[1];
            // the 'super' variable will always be passed as the second argument to the __extends call
            // try to find the 'super' identifier which may be uglified
            var superVariableIdentifier = getSuperVariableIdentifier(extendPath.node.body[0]);

            //// helper functions to find the correct super.call(this) node
            function getSuperVariableIdentifier(__extendsNode) {
                if (types.isExpressionStatement(__extendsNode) && types.isCallExpression(__extendsNode.expression)) {
                    var __extendsCallArguments = __extendsNode.expression.arguments;
                    if (__extendsCallArguments.length == 2) {
                        return __extendsCallArguments[1].name;
                    }
                }
            }

            /** Getting the super.call node from assignment
                if expressionStatement => check possibleExpressionStatements[0]
                if possibleExpressionStatements[0].expression is assignment expression
                if possibleExpressionStatements[0].expression.right is logical expression
                if possibleExpressionStatements[0].expression.right.left is Call expression
                if possibleExpressionStatements[0].expression.right.left.callee.object.name === superVariableIdentifier
                if the above is valid, then variableRHS = possibleVariableDeclarations[0].expression.right.left 
             */
            function getThisAssignmentSuperCallLineNode(nodes, superIdentifier) {
                var matchingNodes = nodes.filter(
                    (node) => {
                        return types.isAssignmentExpression(node.expression)
                            && types.isLogicalExpression(node.expression.right)
                            && types.isCallExpression(node.expression.right.left)
                            && node.expression.right.left.callee.object.name === superIdentifier;
                    });

                return matchingNodes.length > 0 ? matchingNodes[0].expression.right.left : null;
            }

            /** Getting the super.call node from declaration
                if variableDeclaration => check possibleVariableDeclarations[0].declarations[0].init  isn't null
                if possibleNodesForSuperCall[0].declarations[0].init is logical expression
                if possibleNodesForSuperCall[0].declarations[0].init.left is Call Expression
                if possibleNodesForSuperCall[0].declarations[0].init.left.callee.object.name === superVariableIdentifier
                if the above is valid, then variableRHS = possibleVariableDeclarations[0].declarations[0].init.left
             */
            function getThisDeclarationSuperCallLineNode(nodes, superIdentifier) {
                var matchingNodes = nodes.filter(
                    (node) => {
                        return types.isLogicalExpression(node.declarations[0].init)
                            && types.isCallExpression(node.declarations[0].init.left)
                            && node.declarations[0].init.left.callee.object.name === superIdentifier;
                    });

                return matchingNodes.length > 0 ? matchingNodes[0].declarations[0].init.left : null;
            }
            ////

            if (superVariableIdentifier) {
                var possibleVariableDeclarations = [];
                var possibleExpressionStatements = [];

                constructorFunctionScope.body.body.forEach(node => {
                    if (types.isVariableDeclaration(node)) {
                        possibleVariableDeclarations.push(node);
                    } else if (types.isExpressionStatement(node)) {
                        possibleExpressionStatements.push(node);
                    }
                });

                if (possibleVariableDeclarations.length > 0 || possibleExpressionStatements.length > 0) {
                    var superCallRHS = getThisDeclarationSuperCallLineNode(possibleVariableDeclarations, superVariableIdentifier)
                        || getThisAssignmentSuperCallLineNode(possibleExpressionStatements, superVariableIdentifier);

                    if (superCallRHS) {
                        var superCallee = superCallRHS.callee.property;
                        superCalleeStartColumn = superCallee.loc.start.column + 1;
                        superCalleeLine = superCallee.loc.start.line;

                        locationAssigned = true;
                    }
                }
            }
        }

        if (!locationAssigned) {
            config.logger.info(UNSUPPORTED_TYPESCRIPT_EXTEND_FORMAT_MESSAGE + " ctor function name: " + constructorFunctionName);
        }

        return {
            line: superCalleeLine,
            column: superCalleeStartColumn
        };
    }

    function traverseForDecorate(path, config, depth) {
        var iifeRoot = _getParent(path, depth)
        var body = iifeRoot.node.body;
        for (var index in body) {
            var ci = body[index];
            if (isDecorateStatement(ci)) {
                // returns the node of the decorate (node.expression.right.callee)
                // __decorate([..])
                return ci.expression.right.arguments[0].elements;
            }
        }

        return null;
    }

    function traverseForDecorateSpecial(path, config, depth) {
        var iifeRoot = _getParent(path, depth);

        var sibling = iifeRoot.getSibling(iifeRoot.key + 1).node;
        if (sibling) {
            if (isDecorateStatement(sibling)) {
                // returns the node of the decorate (node.expression.right.callee)
                // __decorate([..])
                return sibling.expression.right.arguments[0].elements;
            }
        }

        return null;
    }

    function isDecorateStatement(node) {
        return types.isExpressionStatement(node) &&
            types.isAssignmentExpression(node.expression) &&
            node.expression.right.callee &&
            node.expression.right.callee.name === "__decorate" &&
            node.expression.right.arguments &&
            types.isArrayExpression(node.expression.right.arguments[0])
    }

	/* 
	*	Traverses the node, which is a "new" expression and find if it's a native interface or not.
	*	Write results in "interfacesArr".
	*/
    function traverseInterface(path, config) {
        if (!config.interfaceNames) {
            throw "No interface names are provided! You can pass them in config.interfaceNames as an array!"
        }

        var o = path.node.callee,
            interfaceArr = _getWholeName(o),
            foundInterface = false,
            interfaceNames = config.interfaceNames

        var currentInterface = interfaceArr.reverse().join(".");
        for (var i in interfaceNames) {
            var interfaceName = interfaceNames[i].trim();
            if (interfaceName === currentInterface) {
                currentInterface = interfaceName;
                foundInterface = true;
                break;
            }
        }

        if (foundInterface) {
            var arg0 = "",
                arg1;
            if (path.node.arguments.length === 1) {
                arg1 = path.node.arguments[0];
            }
            else if (path.node.arguments.length === 2) {
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
            var extendInfo = {
                file: config.filePath,
                line: path.node.loc.start.line,
                column: path.node.loc.start.column + columnOffset,
                className: (isCorrectInterfaceName ? arg0.value : "")
            }
            var lineToWrite = _generateLineToWrite("", currentInterface, overriddenInterfaceMethods.join(","), extendInfo, "");
            if (config.logger) {
                config.logger.info(lineToWrite)
            }

            interfacesArr.push(lineToWrite)
        }
    }

	/* 
	*	Finds the java proxy name from custom class decorator.
	*	Write results in "customExtendsArr"
	*/
    function traverseJavaProxyExtend(path, config, customDecoratorName, extendClass, overriddenMethodNames, implementedInterfaces) {
        if (config.logger) {
            config.logger.info("\t+in " + customDecoratorName + " anchor");
        }

        var classNameFromDecorator = path;//_getDecoratorArgument(path, config, customDecoratorName);

        var lineToWrite = _generateLineToWrite(classNameFromDecorator, extendClass, overriddenMethodNames, "", config.fullPathName, implementedInterfaces);
        if (config.logger) {
            config.logger.info(lineToWrite)
        }
        addCustomExtend(classNameFromDecorator, config.fullPathName, lineToWrite)
    }

	/* 
	*	Finds the normal extend name, overridden methods and possibly java proxy name from passed node.
	*	Writes to "customExtendsArr" or "normalExtendsArr".
	*	Left whole for readability.
	*/
    function traverseEs5Extend(path, config) {
        var callee = path.parent.callee;

        if (callee) {
            var o = callee.object
            extendClass = _getWholeName(o);

            var extendArguments = path.parent.arguments;
            var arg0,
                arg1;
            if (extendArguments.length === 1 && types.isObjectExpression(arg0)) {
                arg0 = extendArguments[0];
            }
            else if (types.isStringLiteral(arg0)) {

            }

            var arg0 = "",
                arg1;
            if (extendArguments.length) {
                // Get implementation object when there is only 1 argument
                if (extendArguments.length === 1 && types.isObjectExpression(extendArguments[0])) {
                    arg1 = extendArguments[0];
                }
                // Get the name of the extended class and the implementation object when both arguments are present
                else if (extendArguments.length === 2) {
                    if (types.isStringLiteral(extendArguments[0]) && types.isObjectExpression(extendArguments[1])) {
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

            // Get all methods from the implementation object
            var methodsAndInterfaces = _getOverridenMethodsAndImplementedInterfaces(arg1, config);
            var overriddenMethodNames = methodsAndInterfaces[0];
            var implementedInterfaces = methodsAndInterfaces[1];

            var isCorrectExtendClassName = _testJavaProxyName(className);
            var isCorrectClassName = _testClassName(className);
            if (className && !isCorrectClassName && !isCorrectExtendClassName) {
                throw {
                    message: "The 'extend' you are trying to make has an invalid name. Example: '...extend(\"a.b.C\", {...overrides...})'), file: " + config.filePath,
                    errCode: 1
                }
            }

            var lineToWrite = "";
            if (isCorrectExtendClassName) {
                if (config.logger) {
                    config.logger.info(lineToWrite)
                }

                var classNameFromDecorator = isCorrectExtendClassName ? className : "";
                lineToWrite = _generateLineToWrite(classNameFromDecorator, extendClass.reverse().join("."), overriddenMethodNames, "", config.fullPathName, implementedInterfaces);
                addCustomExtend(classNameFromDecorator, config.fullPathName, lineToWrite);

                return;
            }

            if (config.logger) {
                config.logger.info(lineToWrite)
            }
            var extendInfo = {
                file: config.filePath,
                line: path.node.property.loc.start.line,
                column: path.node.property.loc.start.column + columnOffset,
                className: className
            };
            lineToWrite = _generateLineToWrite(isCorrectExtendClassName ? className : "", extendClass.reverse().join("."), overriddenMethodNames, extendInfo, "", implementedInterfaces);
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
        if (types.isObjectExpression(node)) {
            var objectProperties = node.properties;

            for (var index in objectProperties) {
                overriddenMethodNames.push(objectProperties[index].key.name);
            }
        }

        return overriddenMethodNames;
    }

    // NOTE: It's a near-identical method to _getOverridenMethods for optimisation reasons
    // we do not want to check for interfaces while creating an interface
    // and likewise, we do not want to iterate twice through the impl. object's properties to read the interfaces 
    function _getOverridenMethodsAndImplementedInterfaces(node, config) {
        var result = [];
        var overriddenMethodNames = [];
        var implementedInterfaces = [];

        var interfacesFound = false;

        if (types.isObjectExpression(node)) {
            var objectProperties = node.properties;

			/*
				Iterates through all properties of the implementation object, e.g.
				
					{
						method1: function() {
							
						},
						method3: function() {
							
						}
					}
				
				will get 'method1' and 'method3'	 
			*/
            for (var index in objectProperties) {
                // if the user has declared interfaces that he is implementing
                if (!interfacesFound
                    && objectProperties[index].key.name.toLowerCase() === "interfaces"
                    && types.isArrayExpression(objectProperties[index].value)) {
                    interfacesFound = true;
                    var interfaces = objectProperties[index].value.elements;

                    for (var i in interfaces) {
                        var interfaceName = _getWholeInterfaceNameFromInterfacesNode(interfaces[i]);
                        implementedInterfaces.push(interfaceName);
                    }
                } else {
                    overriddenMethodNames.push(objectProperties[index].key.name)
                }
            }
        }

        result.push(overriddenMethodNames);
        result.push(implementedInterfaces);

        return result;
    }

    function _getWholeInterfaceNameFromInterfacesNode(node) {
        var interfaceName = "";

        if (types.isMemberExpression(node)) {
            interfaceName += _resolveInterfacePath(node.object);
            interfaceName += node.property.name;
        }

        return interfaceName;
    }

    function _resolveInterfacePath(node) {
        var subNode = "";

        if (types.isMemberExpression(node)) {
            if (types.isMemberExpression(node.object)) {
                subNode += _resolveInterfacePath(node.object);
                subNode += node.property.name + ".";
            } else {
                subNode += node.object.name + ".";
                subNode += node.property.name + ".";
            }
        }

        return subNode;
    }

    function _getWholeName(node) {
        var arr = [],
            isAndroidInterface = false;

        while (node !== undefined) {
            if (!types.isMemberExpression(node)) {
                if (isAndroidInterface) {
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
        var extendedClass = _getParent(path, count, config);

        if (extendedClass) {
            if (types.isCallExpression(extendedClass.node)) {
                var o = extendedClass.node.arguments[0];
            }
            else {
                throw {
                    message: "Node type is not a call expression. File=" + config.filePath + " line=" + path.node.loc.start.line,
                    errCode: 1
                }
            }
        }

        extClassArr = _getWholeName(o);

        return extClassArr.reverse().join(".");
    }

    function _getDecoratorArgument(path, config, customDecoratorName) {
        if (path.parent && types.isCallExpression(path.parent)) {

            if (path.parent.arguments && path.parent.arguments.length > 0) {

                var classNameFromDecorator = path.parent.arguments[0].value
                var isCorrectExtendClassName = _testJavaProxyName(classNameFromDecorator);
                if (isCorrectExtendClassName) {
                    return path.parent.arguments[0].value;
                }
                else {
                    throw {
                        message: "The first argument '" + classNameFromDecorator + "' of the " + customDecoratorName + " decorator is not following the right pattern which is: '[namespace.]ClassName'. Example: '" + customDecoratorName + "(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
                        errCode: 1
                    }
                }
            }
            else {
                throw {
                    message: "No arguments passed to " + customDecoratorName + " decorator. Example: '" + customDecoratorName + "(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
                    errCode: 1
                }
            }
        }
        else {
            throw {
                message: "Decorator " + customDecoratorName + " must be called with parameters: Example: '" + customDecoratorName + "(\"a.b.ClassName\", {overrides...})', file: " + config.filePath,
                errCode: 1
            }
        }
        return undefined;
    }

    function _getOverriddenMethodsTypescript(path, count) {
        var overriddenMethods = [];

        var cn = _getParent(path, count)

        // this pattern follows typescript generated syntax
        for (var item in cn.node.body) {
            var ci = cn.node.body[item];
            if (types.isExpressionStatement(ci)) {
                if (types.isAssignmentExpression(ci.expression)) {
                    if (ci.expression.left.property) {
                        overriddenMethods.push(ci.expression.left.property.name)
                    }
                }
            }
        }

        return overriddenMethods;
    }

    function _getParent(node, numberOfParents, config) {
        if (!node) {
            throw {
                message: "No parent found for node in file: " + config.filePath,
                errCode: 1
            }
        }
        if (numberOfParents === 0) {
            return node;
        }

        return _getParent(node.parentPath, --numberOfParents)
    }

    function _testJavaProxyName(name) {
        if (name) {
            return /^((\w+\.)+\w+)$/.test(name)
        }
        return false;
    }

    function _testClassName(name) {
        if (name && name != "") {
            return /^(\w+)$/.test(name)
        }
        return false;
    }

    function _generateLineToWrite(classNameFromDecorator, extendClass, overriddenMethodNames, extendInfo, filePath, implementedInterfaces = "") {
        const extendInfoFile = extendInfo.file ? extendInfo.file.replace(/[-\\/\\. ]/g, "_") : "";
        const extendInfoLine = extendInfo.line ? extendInfo.line : "";
        const extendInfoColumn = extendInfo.column ? extendInfo.column : "";
        const extendInfoNewClassName = extendInfo.className ? extendInfo.className : "";

        var lineToWrite = `${extendClass}${ASTERISK_SEPARATOR}`
            + `${extendInfoFile}${ASTERISK_SEPARATOR}`
            + `${extendInfoLine}${ASTERISK_SEPARATOR}`
            + `${extendInfoColumn}${ASTERISK_SEPARATOR}`
            + `${extendInfoNewClassName}${ASTERISK_SEPARATOR}`
            + `${overriddenMethodNames}${ASTERISK_SEPARATOR}`
            + `${classNameFromDecorator}${ASTERISK_SEPARATOR}`
            + `${filePath}${ASTERISK_SEPARATOR}`
            + `${implementedInterfaces}`

        return lineToWrite;
    }

    function addCustomExtend(param, extendPath, lineToWrite) {
        if (customExtendsArrGlobal.indexOf(param) === -1) {
            customExtendsArr.push(lineToWrite)
            customExtendsArrGlobal.push(param)
        } else {
            console.log("Warning: there already is an extend called " + param + ".")
            if (extendPath.indexOf("tns_modules") === -1) {
                // app folder will take precedence over tns_modules
                console.log("Warning: The static binding generator will generate extend from:" + extendPath + " implementation")
                customExtendsArr.push(lineToWrite)
                customExtendsArrGlobal.push(param)
            }
        }
    }

    return {
        es5Visitor: es5Visitor
    }
})();

module.exports = es5_visitors;
