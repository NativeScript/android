var types_1 = require("utils/types");
var page_1 = require("ui/page");
var view_1 = require("ui/core/view");
var file_system_1 = require("file-system");
var binding_builder_1 = require("./binding-builder");
var utils_1 = require("utils/utils");
require("ui/layouts/dock-layout");
require("ui/layouts/grid-layout");
require("ui/layouts/absolute-layout");
var special_properties_1 = require("ui/builder/special-properties");
var UI_PATH = "ui/";
var MODULES = {
    "TabViewItem": "ui/tab-view",
    "FormattedString": "text/formatted-string",
    "Span": "text/span",
    "ActionItem": "ui/action-bar",
    "NavigationButton": "ui/action-bar",
    "SegmentedBarItem": "ui/segmented-bar",
};
var CODEFILE = "codeFile";
var CSSFILE = "cssFile";
var platform;
function ensurePlatform() {
    if (!platform) {
        platform = require("platform");
    }
}
function getComponentModule(elementName, namespace, attributes, exports) {
    var instance;
    var instanceModule;
    var componentModule;
    elementName = elementName.split("-").map(function (s) { return s[0].toUpperCase() + s.substring(1); }).join("");
    var moduleId = MODULES[elementName] || UI_PATH +
        (elementName.toLowerCase().indexOf("layout") !== -1 ? "layouts/" : "") +
        elementName.split(/(?=[A-Z])/).join("-").toLowerCase();
    try {
        if (types_1.isString(namespace)) {
            if (global.moduleExists(namespace)) {
                moduleId = namespace;
            }
            else {
                var pathInsideTNSModules = file_system_1.path.join(file_system_1.knownFolders.currentApp().path, "tns_modules", namespace);
                try {
                    instanceModule = global.require(pathInsideTNSModules);
                    moduleId = pathInsideTNSModules;
                }
                catch (e) {
                    moduleId = file_system_1.path.join(file_system_1.knownFolders.currentApp().path, namespace);
                }
            }
        }
        if (!instanceModule) {
            instanceModule = global.loadModule(moduleId);
        }
        var instanceType = instanceModule[elementName] || Object;
        instance = new instanceType();
    }
    catch (ex) {
        var debug = require("utils/debug");
        throw new debug.ScopeError(ex, "Module '" + moduleId + "' not found for element '" + (namespace ? namespace + ":" : "") + elementName + "'.");
    }
    if (attributes) {
        if (attributes[CODEFILE]) {
            if (instance instanceof page_1.Page) {
                var codeFilePath = attributes[CODEFILE].trim();
                if (codeFilePath.indexOf("~/") === 0) {
                    codeFilePath = file_system_1.path.join(file_system_1.knownFolders.currentApp().path, codeFilePath.replace("~/", ""));
                }
                var codeFilePathWithExt = codeFilePath.indexOf(".js") !== -1 ? codeFilePath : codeFilePath + ".js";
                if (file_system_1.File.exists(codeFilePathWithExt)) {
                    exports = global.loadModule(codeFilePath);
                    instance.exports = exports;
                }
                else {
                    throw new Error("Code file with path \"" + codeFilePathWithExt + "\" cannot be found!");
                }
            }
            else {
                throw new Error("Code file atribute is valid only for pages!");
            }
        }
        if (attributes[CSSFILE]) {
            if (instance instanceof page_1.Page) {
                var cssFilePath = attributes[CSSFILE].trim();
                if (cssFilePath.indexOf("~/") === 0) {
                    cssFilePath = file_system_1.path.join(file_system_1.knownFolders.currentApp().path, cssFilePath.replace("~/", ""));
                }
                if (file_system_1.File.exists(cssFilePath)) {
                    instance.addCssFile(cssFilePath);
                    instance[CSSFILE] = true;
                }
                else {
                    throw new Error("Css file with path \"" + cssFilePath + "\" cannot be found!");
                }
            }
            else {
                throw new Error("Css file atribute is valid only for pages!");
            }
        }
    }
    if (instance && instanceModule) {
        for (var attr in attributes) {
            var attrValue = attributes[attr];
            if (attr.indexOf(":") !== -1) {
                var platformName = attr.split(":")[0].trim();
                ensurePlatform();
                if (platformName.toLowerCase() === platform.device.os.toLowerCase()) {
                    attr = attr.split(":")[1].trim();
                }
                else {
                    continue;
                }
            }
            if (attr.indexOf(".") !== -1) {
                var subObj = instance;
                var properties = attr.split(".");
                var subPropName = properties[properties.length - 1];
                var i;
                for (i = 0; i < properties.length - 1; i++) {
                    if (types_1.isDefined(subObj)) {
                        subObj = subObj[properties[i]];
                    }
                }
                if (types_1.isDefined(subObj)) {
                    setPropertyValue(subObj, instanceModule, exports, subPropName, attrValue);
                }
            }
            else {
                setPropertyValue(instance, instanceModule, exports, attr, attrValue);
            }
        }
        componentModule = { component: instance, exports: instanceModule };
    }
    return componentModule;
}
exports.getComponentModule = getComponentModule;
function setPropertyValue(instance, instanceModule, exports, propertyName, propertyValue) {
    if (isBinding(propertyValue) && instance.bind) {
        var bindOptions = binding_builder_1.getBindingOptions(propertyName, getBindingExpressionFromAttribute(propertyValue));
        instance.bind({
            sourceProperty: bindOptions[binding_builder_1.bindingConstants.sourceProperty],
            targetProperty: bindOptions[binding_builder_1.bindingConstants.targetProperty],
            expression: bindOptions[binding_builder_1.bindingConstants.expression],
            twoWay: bindOptions[binding_builder_1.bindingConstants.twoWay]
        }, bindOptions[binding_builder_1.bindingConstants.source]);
    }
    else if (view_1.isEventOrGesture(propertyName, instance)) {
        var handler = exports && exports[propertyValue];
        if (types_1.isFunction(handler)) {
            instance.on(propertyName, handler);
        }
    }
    else {
        var attrHandled = false;
        var specialSetter = special_properties_1.getSpecialPropertySetter(propertyName);
        if (!attrHandled && specialSetter) {
            specialSetter(instance, propertyValue);
            attrHandled = true;
        }
        if (!attrHandled && instance._applyXmlAttribute) {
            attrHandled = instance._applyXmlAttribute(propertyName, propertyValue);
        }
        if (!attrHandled) {
            instance[propertyName] = utils_1.convertString(propertyValue);
        }
    }
}
exports.setPropertyValue = setPropertyValue;
function getBindingExpressionFromAttribute(value) {
    return value.replace("{{", "").replace("}}", "").trim();
}
function isBinding(value) {
    var isBinding;
    if (types_1.isString(value)) {
        var str = value.trim();
        isBinding = str.indexOf("{{") === 0 && str.lastIndexOf("}}") === str.length - 2;
    }
    return isBinding;
}
