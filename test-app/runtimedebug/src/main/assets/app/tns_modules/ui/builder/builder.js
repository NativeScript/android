var debug_1 = require("utils/debug");
var xml = require("xml");
var view_1 = require("ui/core/view");
var file_system_1 = require("file-system");
var types_1 = require("utils/types");
var component_builder_1 = require("ui/builder/component-builder");
var platform_1 = require("platform");
var page_1 = require("ui/page");
var file_name_resolver_1 = require("file-system/file-name-resolver");
var defaultNameSpaceMatcher = /tns\.xsd$/i;
var trace;
function ensureTrace() {
    if (!trace) {
        trace = require("trace");
    }
}
function parse(value, context) {
    if (types_1.isString(value)) {
        var viewToReturn;
        if (context instanceof view_1.View) {
            context = getExports(context);
        }
        var componentModule = parseInternal(value, context);
        if (componentModule) {
            viewToReturn = componentModule.component;
        }
        return viewToReturn;
    }
    else if (types_1.isFunction(value)) {
        return value();
    }
}
exports.parse = parse;
function parseInternal(value, context, uri) {
    var start;
    var ui;
    var errorFormat = (debug_1.debug && uri) ? xml2ui.SourceErrorFormat(uri) : xml2ui.PositionErrorFormat;
    var componentSourceTracker = (debug_1.debug && uri) ? xml2ui.ComponentSourceTracker(uri) : function () {
    };
    (start = new xml2ui.XmlStringParser(errorFormat))
        .pipe(new xml2ui.PlatformFilter())
        .pipe(new xml2ui.XmlStateParser(ui = new xml2ui.ComponentParser(context, errorFormat, componentSourceTracker)));
    start.parse(value);
    return ui.rootComponentModule;
}
function loadCustomComponent(componentPath, componentName, attributes, context, parentPage) {
    var result;
    componentPath = componentPath.replace("~/", "");
    var moduleName = componentPath + "/" + componentName;
    var fullComponentPathFilePathWithoutExt = componentPath;
    if (!file_system_1.File.exists(componentPath) || componentPath === "." || componentPath === "./") {
        fullComponentPathFilePathWithoutExt = file_system_1.path.join(file_system_1.knownFolders.currentApp().path, componentPath, componentName);
    }
    var xmlFilePath = file_name_resolver_1.resolveFileName(fullComponentPathFilePathWithoutExt, "xml");
    if (xmlFilePath) {
        var jsFilePath = file_name_resolver_1.resolveFileName(fullComponentPathFilePathWithoutExt, "js");
        var subExports = context;
        if (global.moduleExists(moduleName)) {
            subExports = global.loadModule(moduleName);
        }
        else {
            if (jsFilePath) {
                subExports = global.loadModule(jsFilePath);
            }
        }
        result = loadInternal(xmlFilePath, subExports);
        if (types_1.isDefined(result) && types_1.isDefined(result.component) && types_1.isDefined(attributes)) {
            var attr;
            for (attr in attributes) {
                component_builder_1.setPropertyValue(result.component, subExports, context, attr, attributes[attr]);
            }
        }
    }
    else {
        result = component_builder_1.getComponentModule(componentName, componentPath, attributes, context);
    }
    var cssFilePath = file_name_resolver_1.resolveFileName(fullComponentPathFilePathWithoutExt, "css");
    if (cssFilePath) {
        if (parentPage) {
            parentPage.addCssFile(cssFilePath);
        }
        else {
            ensureTrace();
            trace.write("CSS file found but no page specified. Please specify page in the options!", trace.categories.Error, trace.messageType.error);
        }
    }
    return result;
}
function load(pathOrOptions, context) {
    var viewToReturn;
    var componentModule;
    if (!context) {
        if (!types_1.isString(pathOrOptions)) {
            var options = pathOrOptions;
            componentModule = loadCustomComponent(options.path, options.name, options.attributes, options.exports, options.page);
        }
        else {
            var path_1 = pathOrOptions;
            componentModule = loadInternal(path_1);
        }
    }
    else {
        var path_2 = pathOrOptions;
        componentModule = loadInternal(path_2, context);
    }
    if (componentModule) {
        viewToReturn = componentModule.component;
    }
    return viewToReturn;
}
exports.load = load;
function loadInternal(fileName, context) {
    var componentModule;
    if (file_system_1.File.exists(fileName)) {
        var file = file_system_1.File.fromPath(fileName);
        var onError = function (error) {
            throw new Error("Error loading file " + fileName + " :" + error.message);
        };
        var text = file.readTextSync(onError);
        componentModule = parseInternal(text, context, fileName);
    }
    if (componentModule && componentModule.component) {
        componentModule.component.exports = context;
    }
    return componentModule;
}
function getExports(instance) {
    var parent = instance.parent;
    while (parent && parent.exports === undefined) {
        parent = parent.parent;
    }
    return parent ? parent.exports : undefined;
}
var xml2ui;
(function (xml2ui) {
    var XmlProducerBase = (function () {
        function XmlProducerBase() {
        }
        XmlProducerBase.prototype.pipe = function (next) {
            this._next = next;
            return next;
        };
        XmlProducerBase.prototype.next = function (args) {
            this._next.parse(args);
        };
        return XmlProducerBase;
    }());
    xml2ui.XmlProducerBase = XmlProducerBase;
    var XmlStringParser = (function (_super) {
        __extends(XmlStringParser, _super);
        function XmlStringParser(error) {
            _super.call(this);
            this.error = error || PositionErrorFormat;
        }
        XmlStringParser.prototype.parse = function (value) {
            var _this = this;
            var xmlParser = new xml.XmlParser(function (args) {
                try {
                    _this.next(args);
                }
                catch (e) {
                    throw _this.error(e, args.position);
                }
            }, function (e, p) {
                throw _this.error(e, p);
            }, true);
            if (types_1.isString(value)) {
                xmlParser.parse(value);
            }
        };
        return XmlStringParser;
    }(XmlProducerBase));
    xml2ui.XmlStringParser = XmlStringParser;
    function PositionErrorFormat(e, p) {
        return new debug_1.ScopeError(e, "Parsing XML at " + p.line + ":" + p.column);
    }
    xml2ui.PositionErrorFormat = PositionErrorFormat;
    function SourceErrorFormat(uri) {
        return function (e, p) {
            var source = p ? new debug_1.Source(uri, p.line, p.column) : new debug_1.Source(uri, -1, -1);
            e = new debug_1.SourceError(e, source, "Building UI from XML.");
            return e;
        };
    }
    xml2ui.SourceErrorFormat = SourceErrorFormat;
    function ComponentSourceTracker(uri) {
        return function (component, p) {
            if (!debug_1.Source.get(component)) {
                var source = p ? new debug_1.Source(uri, p.line, p.column) : new debug_1.Source(uri, -1, -1);
                debug_1.Source.set(component, source);
            }
        };
    }
    xml2ui.ComponentSourceTracker = ComponentSourceTracker;
    var PlatformFilter = (function (_super) {
        __extends(PlatformFilter, _super);
        function PlatformFilter() {
            _super.apply(this, arguments);
        }
        PlatformFilter.prototype.parse = function (args) {
            if (args.eventType === xml.ParserEventType.StartElement) {
                if (PlatformFilter.isPlatform(args.elementName)) {
                    if (this.currentPlatformContext) {
                        throw new Error("Already in '" + this.currentPlatformContext + "' platform context and cannot switch to '" + args.elementName + "' platform! Platform tags cannot be nested.");
                    }
                    this.currentPlatformContext = args.elementName;
                    return;
                }
            }
            if (args.eventType === xml.ParserEventType.EndElement) {
                if (PlatformFilter.isPlatform(args.elementName)) {
                    this.currentPlatformContext = undefined;
                    return;
                }
            }
            if (this.currentPlatformContext && !PlatformFilter.isCurentPlatform(this.currentPlatformContext)) {
                return;
            }
            this.next(args);
        };
        PlatformFilter.isPlatform = function (value) {
            return value && (value.toLowerCase() === platform_1.platformNames.android.toLowerCase()
                || value.toLowerCase() === platform_1.platformNames.ios.toLowerCase());
        };
        PlatformFilter.isCurentPlatform = function (value) {
            return value && value.toLowerCase() === platform_1.device.os.toLowerCase();
        };
        return PlatformFilter;
    }(XmlProducerBase));
    xml2ui.PlatformFilter = PlatformFilter;
    var XmlArgsReplay = (function (_super) {
        __extends(XmlArgsReplay, _super);
        function XmlArgsReplay(args, errorFormat) {
            _super.call(this);
            this.args = args;
            this.error = errorFormat;
        }
        XmlArgsReplay.prototype.replay = function () {
            var _this = this;
            this.args.forEach(function (args) {
                try {
                    _this.next(args);
                }
                catch (e) {
                    throw _this.error(e, args.position);
                }
            });
        };
        return XmlArgsReplay;
    }(XmlProducerBase));
    xml2ui.XmlArgsReplay = XmlArgsReplay;
    var XmlStateParser = (function () {
        function XmlStateParser(state) {
            this.state = state;
        }
        XmlStateParser.prototype.parse = function (args) {
            this.state = this.state.parse(args);
        };
        return XmlStateParser;
    }());
    xml2ui.XmlStateParser = XmlStateParser;
    var TemplateParser = (function () {
        function TemplateParser(parent, templateProperty) {
            this.parent = parent;
            this._context = templateProperty.context;
            this._recordedXmlStream = new Array();
            this._templateProperty = templateProperty;
            this._nestingLevel = 0;
            this._state = 0;
        }
        TemplateParser.prototype.parse = function (args) {
            if (args.eventType === xml.ParserEventType.StartElement) {
                this.parseStartElement(args.prefix, args.namespace, args.elementName, args.attributes);
            }
            else if (args.eventType === xml.ParserEventType.EndElement) {
                this.parseEndElement(args.prefix, args.elementName);
            }
            this._recordedXmlStream.push(args);
            return this._state === 2 ? this.parent : this;
        };
        Object.defineProperty(TemplateParser.prototype, "elementName", {
            get: function () {
                return this._templateProperty.elementName;
            },
            enumerable: true,
            configurable: true
        });
        TemplateParser.prototype.parseStartElement = function (prefix, namespace, elementName, attributes) {
            if (this._state === 0) {
                this._state = 1;
            }
            else if (this._state === 2) {
                throw new Error("Template must have exactly one root element but multiple elements were found.");
            }
            this._nestingLevel++;
        };
        TemplateParser.prototype.parseEndElement = function (prefix, elementName) {
            if (this._state === 0) {
                throw new Error("Template must have exactly one root element but none was found.");
            }
            else if (this._state === 2) {
                throw new Error("No more closing elements expected for this template.");
            }
            this._nestingLevel--;
            if (this._nestingLevel === 0) {
                this._state = 2;
                this.build();
            }
        };
        TemplateParser.prototype.build = function () {
            var _this = this;
            if (this._templateProperty.name in this._templateProperty.parent.component) {
                var context = this._context;
                var errorFormat = this._templateProperty.errorFormat;
                var sourceTracker = this._templateProperty.sourceTracker;
                var template = function () {
                    var start;
                    var ui;
                    (start = new xml2ui.XmlArgsReplay(_this._recordedXmlStream, errorFormat))
                        .pipe(new XmlStateParser(ui = new ComponentParser(context, errorFormat, sourceTracker)));
                    start.replay();
                    return ui.rootComponentModule.component;
                };
                this._templateProperty.parent.component[this._templateProperty.name] = template;
            }
        };
        return TemplateParser;
    }());
    xml2ui.TemplateParser = TemplateParser;
    var ComponentParser = (function () {
        function ComponentParser(context, errorFormat, sourceTracker) {
            this.parents = new Array();
            this.complexProperties = new Array();
            this.context = context;
            this.error = errorFormat;
            this.sourceTracker = sourceTracker;
        }
        ComponentParser.prototype.parse = function (args) {
            var parent = this.parents[this.parents.length - 1];
            var complexProperty = this.complexProperties[this.complexProperties.length - 1];
            if (args.eventType === xml.ParserEventType.StartElement) {
                if (ComponentParser.isComplexProperty(args.elementName)) {
                    var name = ComponentParser.getComplexPropertyName(args.elementName);
                    this.complexProperties.push({
                        parent: parent,
                        name: name,
                        items: [],
                    });
                    if (ComponentParser.isKnownTemplate(name, parent.exports)) {
                        return new TemplateParser(this, {
                            context: (parent ? getExports(parent.component) : null) || this.context,
                            parent: parent,
                            name: name,
                            elementName: args.elementName,
                            templateItems: [],
                            errorFormat: this.error,
                            sourceTracker: this.sourceTracker
                        });
                    }
                }
                else {
                    var componentModule;
                    if (args.prefix && args.namespace) {
                        componentModule = loadCustomComponent(args.namespace, args.elementName, args.attributes, this.context, this.currentPage);
                    }
                    else {
                        var namespace = args.namespace;
                        if (defaultNameSpaceMatcher.test(namespace || '')) {
                            namespace = undefined;
                        }
                        componentModule = component_builder_1.getComponentModule(args.elementName, namespace, args.attributes, this.context);
                    }
                    if (componentModule) {
                        this.sourceTracker(componentModule.component, args.position);
                        if (parent) {
                            if (complexProperty) {
                                ComponentParser.addToComplexProperty(parent, complexProperty, componentModule);
                            }
                            else if (parent.component._addChildFromBuilder) {
                                parent.component._addChildFromBuilder(args.elementName, componentModule.component);
                            }
                        }
                        else if (this.parents.length === 0) {
                            this.rootComponentModule = componentModule;
                            if (this.rootComponentModule && this.rootComponentModule.component instanceof page_1.Page) {
                                this.currentPage = this.rootComponentModule.component;
                                if (this.currentPage.exports) {
                                    this.context = this.currentPage.exports;
                                }
                            }
                        }
                        this.parents.push(componentModule);
                    }
                }
            }
            else if (args.eventType === xml.ParserEventType.EndElement) {
                if (ComponentParser.isComplexProperty(args.elementName)) {
                    if (complexProperty) {
                        if (parent && parent.component._addArrayFromBuilder) {
                            parent.component._addArrayFromBuilder(complexProperty.name, complexProperty.items);
                            complexProperty.items = [];
                        }
                    }
                    this.complexProperties.pop();
                }
                else {
                    this.parents.pop();
                }
            }
            return this;
        };
        ComponentParser.isComplexProperty = function (name) {
            return types_1.isString(name) && name.indexOf(".") !== -1;
        };
        ComponentParser.getComplexPropertyName = function (fullName) {
            var name;
            if (types_1.isString(fullName)) {
                var names = fullName.split(".");
                name = names[names.length - 1];
            }
            return name;
        };
        ComponentParser.isKnownTemplate = function (name, exports) {
            return ComponentParser.KNOWNTEMPLATES in exports && exports[ComponentParser.KNOWNTEMPLATES] && name in exports[ComponentParser.KNOWNTEMPLATES];
        };
        ComponentParser.addToComplexProperty = function (parent, complexProperty, elementModule) {
            var parentComponent = parent.component;
            if (ComponentParser.isKnownCollection(complexProperty.name, parent.exports)) {
                complexProperty.items.push(elementModule.component);
            }
            else if (parentComponent._addChildFromBuilder) {
                parentComponent._addChildFromBuilder(complexProperty.name, elementModule.component);
            }
            else {
                parentComponent[complexProperty.name] = elementModule.component;
            }
        };
        ComponentParser.isKnownCollection = function (name, context) {
            return ComponentParser.KNOWNCOLLECTIONS in context && context[ComponentParser.KNOWNCOLLECTIONS] && name in context[ComponentParser.KNOWNCOLLECTIONS];
        };
        ComponentParser.KNOWNCOLLECTIONS = "knownCollections";
        ComponentParser.KNOWNTEMPLATES = "knownTemplates";
        return ComponentParser;
    }());
    xml2ui.ComponentParser = ComponentParser;
})(xml2ui || (xml2ui = {}));
