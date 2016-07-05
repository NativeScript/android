var observable_1 = require("data/observable");
var dependency_observable_1 = require("ui/core/dependency-observable");
var weakEvents = require("ui/core/weak-event-listener");
var types = require("utils/types");
var trace = require("trace");
var bindingBuilder = require("../builder/binding-builder");
var viewModule = require("ui/core/view");
var application = require("application");
var polymerExpressions = require("js-libs/polymer-expressions");
var specialProperties = require("ui/builder/special-properties");
var utils = require("utils/utils");
var bindingContextProperty = new dependency_observable_1.Property("bindingContext", "Bindable", new dependency_observable_1.PropertyMetadata(undefined, dependency_observable_1.PropertyMetadataSettings.Inheritable, onBindingContextChanged));
function onBindingContextChanged(data) {
    var bindable = data.object;
    bindable._onBindingContextChanged(data.oldValue, data.newValue);
}
var contextKey = "context";
var paramsRegex = /\[\s*(['"])*(\w*)\1\s*\]/;
var bc = bindingBuilder.bindingConstants;
var defaultBindingSource = {};
var Bindable = (function (_super) {
    __extends(Bindable, _super);
    function Bindable() {
        _super.apply(this, arguments);
        this.bindings = new Map();
    }
    Object.defineProperty(Bindable.prototype, "bindingContext", {
        get: function () {
            return this._getValue(Bindable.bindingContextProperty);
        },
        set: function (value) {
            this._setValue(Bindable.bindingContextProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Bindable.prototype.bind = function (options, source) {
        if (source === void 0) { source = defaultBindingSource; }
        var binding = this.bindings.get(options.targetProperty);
        if (binding) {
            binding.unbind();
        }
        binding = new Binding(this, options);
        this.bindings.set(options.targetProperty, binding);
        var bindingSource = source;
        if (bindingSource === defaultBindingSource) {
            bindingSource = this.bindingContext;
            binding.sourceIsBindingContext = true;
        }
        binding.bind(bindingSource);
    };
    Bindable.prototype.unbind = function (property) {
        var binding = this.bindings.get(property);
        if (binding) {
            binding.unbind();
            this.bindings.delete(property);
        }
    };
    Bindable.prototype._updateTwoWayBinding = function (propertyName, value) {
        var binding = this.bindings.get(propertyName);
        if (binding) {
            binding.updateTwoWay(value);
        }
    };
    Bindable.prototype._setCore = function (data) {
        _super.prototype._setCore.call(this, data);
        this._updateTwoWayBinding(data.propertyName, data.value);
    };
    Bindable.prototype._onPropertyChanged = function (property, oldValue, newValue) {
        if (trace.enabled) {
            trace.write(this + "._onPropertyChanged(" + property.name + ", " + oldValue + ", " + newValue + ")", trace.categories.Binding);
        }
        _super.prototype._onPropertyChanged.call(this, property, oldValue, newValue);
        if (this instanceof viewModule.View) {
            if (property.inheritable && this._isInheritedChange() === true) {
                return;
            }
        }
        var binding = this.bindings.get(property.name);
        if (binding && !binding.updating) {
            if (binding.options.twoWay) {
                if (trace.enabled) {
                    trace.write((this + "._updateTwoWayBinding(" + property.name + ", " + newValue + ");") + property.name, trace.categories.Binding);
                }
                this._updateTwoWayBinding(property.name, newValue);
            }
            else {
                if (trace.enabled) {
                    trace.write(this + ".unbind(" + property.name + ");", trace.categories.Binding);
                }
                this.unbind(property.name);
            }
        }
    };
    Bindable.prototype._onBindingContextChanged = function (oldValue, newValue) {
        this.bindings.forEach(function (binding, index, bindings) {
            if (!binding.updating && binding.sourceIsBindingContext) {
                if (trace.enabled) {
                    trace.write("Binding " + binding.target.get() + "." + binding.options.targetProperty + " to new context " + newValue, trace.categories.Binding);
                }
                if (!types.isNullOrUndefined(newValue)) {
                    binding.bind(newValue);
                }
                else {
                    binding.clearBinding();
                }
            }
        });
    };
    Bindable.bindingContextProperty = bindingContextProperty;
    return Bindable;
}(dependency_observable_1.DependencyObservable));
exports.Bindable = Bindable;
var emptyArray = [];
function getProperties(property) {
    var result = emptyArray;
    if (property) {
        var parentsMatches = property.match(bindingBuilder.parentsRegex);
        result = property.replace(bindingBuilder.parentsRegex, "parentsMatch")
            .replace(/\]/g, "")
            .split(/\.|\[/);
        var parentsMatchesCounter = 0;
        for (var i = 0, resultLength = result.length; i < resultLength; i++) {
            if (result[i] === "parentsMatch") {
                result[i] = parentsMatches[parentsMatchesCounter++];
            }
        }
    }
    return result;
}
var Binding = (function () {
    function Binding(target, options) {
        this.propertyChangeListeners = new Map();
        this.target = new WeakRef(target);
        this.options = options;
        this.sourceProperties = getProperties(options.sourceProperty);
        this.targetOptions = this.resolveOptions(target, getProperties(options.targetProperty));
    }
    Binding.prototype.loadedHandlerVisualTreeBinding = function (args) {
        var target = args.object;
        target.off(viewModule.View.loadedEvent, this.loadedHandlerVisualTreeBinding, this);
        if (!types.isNullOrUndefined(target.bindingContext)) {
            this.bind(target.bindingContext);
        }
    };
    ;
    Binding.prototype.clearSource = function () {
        var _this = this;
        this.propertyChangeListeners.forEach(function (observable, index, map) {
            weakEvents.removeWeakEventListener(observable, observable_1.Observable.propertyChangeEvent, _this.onSourcePropertyChanged, _this);
        });
        this.propertyChangeListeners.clear();
        this.sourcesAndProperties = null;
        if (this.source) {
            this.source.clear();
        }
        if (this.sourceOptions) {
            this.sourceOptions.instance.clear();
            this.sourceOptions = undefined;
        }
    };
    Binding.prototype.sourceAsObject = function (source) {
        var objectType = typeof source;
        if (objectType === "number") {
            source = new Number(source);
        }
        else if (objectType === "boolean") {
            source = new Boolean(source);
        }
        else if (objectType === "string") {
            source = new String(source);
        }
        return source;
    };
    Binding.prototype.bind = function (source) {
        this.clearSource();
        source = this.sourceAsObject(source);
        var sourceValue;
        if (!types.isNullOrUndefined(source)) {
            this.source = new WeakRef(source);
            this.sourceOptions = this.resolveOptions(source, this.sourceProperties);
            sourceValue = this.getSourcePropertyValue();
            this.updateTarget(sourceValue);
            this.addPropertyChangeListeners(this.source, this.sourceProperties);
        }
        else if (!this.sourceIsBindingContext) {
            sourceValue = this.getSourcePropertyValue();
            this.updateTarget(sourceValue ? sourceValue : source);
        }
    };
    Binding.prototype.resolveObjectsAndProperties = function (source, properties) {
        var result = [];
        var currentObject = source;
        var currentObjectChanged = false;
        for (var i = 0, propsArrayLength = properties.length; i < propsArrayLength; i++) {
            var property = properties[i];
            if (property === bc.bindingValueKey) {
                currentObjectChanged = true;
            }
            if (property === bc.parentValueKey || property.indexOf(bc.parentsValueKey) === 0) {
                var parentView = this.getParentView(this.target.get(), property).view;
                if (parentView) {
                    currentObject = parentView.bindingContext;
                }
                else {
                    var targetInstance = this.target.get();
                    targetInstance.off(viewModule.View.loadedEvent, this.loadedHandlerVisualTreeBinding, this);
                    targetInstance.on(viewModule.View.loadedEvent, this.loadedHandlerVisualTreeBinding, this);
                }
                currentObjectChanged = true;
            }
            if (currentObject) {
                result.push({ instance: currentObject, property: property });
            }
            else {
                break;
            }
            if (!currentObjectChanged && (i < propsArrayLength - 1)) {
                currentObject = currentObject ? currentObject[properties[i]] : null;
            }
            currentObjectChanged = false;
        }
        return result;
    };
    Binding.prototype.addPropertyChangeListeners = function (source, sourceProperty, parentProperies) {
        var objectsAndProperties = this.resolveObjectsAndProperties(source.get(), sourceProperty);
        var prop = parentProperies || "";
        for (var i = 0, length_1 = objectsAndProperties.length; i < length_1; i++) {
            prop += "$" + objectsAndProperties[i].property;
            var currentObject = objectsAndProperties[i].instance;
            if (!this.propertyChangeListeners.has(prop) && currentObject instanceof observable_1.Observable) {
                weakEvents.addWeakEventListener(currentObject, observable_1.Observable.propertyChangeEvent, this.onSourcePropertyChanged, this);
                this.propertyChangeListeners.set(prop, currentObject);
            }
        }
    };
    Binding.prototype.unbind = function () {
        if (!this.source) {
            return;
        }
        this.clearSource();
        if (this.targetOptions) {
            this.targetOptions = undefined;
        }
        this.sourceProperties = undefined;
    };
    Binding.prototype.prepareExpressionForUpdate = function () {
        var escapedSourceProperty = utils.escapeRegexSymbols(this.options.sourceProperty);
        var expRegex = new RegExp(escapedSourceProperty, 'g');
        var resultExp = this.options.expression.replace(expRegex, bc.newPropertyValueKey);
        return resultExp;
    };
    Binding.prototype.updateTwoWay = function (value) {
        if (this.updating || !this.options.twoWay) {
            return;
        }
        var newValue = value;
        if (this.options.expression) {
            var changedModel = {};
            changedModel[bc.bindingValueKey] = value;
            changedModel[bc.newPropertyValueKey] = value;
            var sourcePropertyName = "";
            if (this.sourceOptions) {
                sourcePropertyName = this.sourceOptions.property;
            }
            else if (typeof this.options.sourceProperty === "string" && this.options.sourceProperty.indexOf(".") === -1) {
                sourcePropertyName = this.options.sourceProperty;
            }
            if (sourcePropertyName !== "") {
                changedModel[sourcePropertyName] = value;
            }
            var updateExpression = this.prepareExpressionForUpdate();
            this.prepareContextForExpression(changedModel, updateExpression, undefined);
            var expressionValue = this._getExpressionValue(updateExpression, true, changedModel);
            if (expressionValue instanceof Error) {
                trace.write(expressionValue.message, trace.categories.Binding, trace.messageType.error);
            }
            newValue = expressionValue;
        }
        this.updateSource(newValue);
    };
    Binding.prototype._getExpressionValue = function (expression, isBackConvert, changedModel) {
        try {
            var exp_1 = polymerExpressions.PolymerExpressions.getExpression(expression);
            if (exp_1) {
                var context_1 = this.source && this.source.get && this.source.get() || global;
                var model = {};
                var addedProps = [];
                for (var prop in application.resources) {
                    if (application.resources.hasOwnProperty(prop) && !context_1.hasOwnProperty(prop)) {
                        context_1[prop] = application.resources[prop];
                        addedProps.push(prop);
                    }
                }
                this.prepareContextForExpression(context_1, expression, addedProps);
                model[contextKey] = context_1;
                var result = exp_1.getValue(model, isBackConvert, changedModel ? changedModel : model);
                var addedPropsLength = addedProps.length;
                for (var i = 0; i < addedPropsLength; i++) {
                    delete context_1[addedProps[i]];
                }
                addedProps.length = 0;
                return result;
            }
            return new Error(expression + " is not a valid expression.");
        }
        catch (e) {
            var errorMessage = "Run-time error occured in file: " + e.sourceURL + " at line: " + e.line + " and column: " + e.column;
            return new Error(errorMessage);
        }
    };
    Binding.prototype.onSourcePropertyChanged = function (data) {
        var sourceProps = this.sourceProperties;
        var sourcePropsLength = sourceProps.length;
        var changedPropertyIndex = sourceProps.indexOf(data.propertyName);
        var parentProps = "";
        if (changedPropertyIndex > -1) {
            parentProps = "$" + sourceProps.slice(0, changedPropertyIndex + 1).join("$");
            while (this.propertyChangeListeners.get(parentProps) !== data.object) {
                changedPropertyIndex += sourceProps.slice(changedPropertyIndex + 1).indexOf(data.propertyName) + 1;
                parentProps = "$" + sourceProps.slice(0, changedPropertyIndex + 1).join("$");
            }
        }
        if (this.options.expression) {
            var expressionValue = this._getExpressionValue(this.options.expression, false, undefined);
            if (expressionValue instanceof Error) {
                trace.write(expressionValue.message, trace.categories.Binding, trace.messageType.error);
            }
            else {
                this.updateTarget(expressionValue);
            }
        }
        else {
            if (changedPropertyIndex > -1) {
                var props = sourceProps.slice(changedPropertyIndex + 1);
                var propsLength = props.length;
                if (propsLength > 0) {
                    var value = data.value;
                    for (var i = 0; i < propsLength; i++) {
                        value = value[props[i]];
                    }
                    this.updateTarget(value);
                }
                else if (data.propertyName === this.sourceOptions.property) {
                    this.updateTarget(data.value);
                }
            }
        }
        if (changedPropertyIndex > -1 && changedPropertyIndex < sourcePropsLength - 1) {
            var probablyChangedObject = this.propertyChangeListeners.get(parentProps);
            if (probablyChangedObject &&
                probablyChangedObject !== data.object[sourceProps[changedPropertyIndex]]) {
                for (var i = sourcePropsLength - 1; i > changedPropertyIndex; i--) {
                    var prop = "$" + sourceProps.slice(0, i + 1).join("$");
                    if (this.propertyChangeListeners.has(prop)) {
                        weakEvents.removeWeakEventListener(this.propertyChangeListeners.get(prop), observable_1.Observable.propertyChangeEvent, this.onSourcePropertyChanged, this);
                        this.propertyChangeListeners.delete(prop);
                    }
                }
                var newProps = sourceProps.slice(changedPropertyIndex + 1);
                var newObject = data.object[sourceProps[changedPropertyIndex]];
                if (!types.isNullOrUndefined(newObject) && typeof newObject === 'object') {
                    this.addPropertyChangeListeners(new WeakRef(newObject), newProps, parentProps);
                }
            }
        }
    };
    Binding.prototype.prepareContextForExpression = function (model, expression, newProps) {
        var parentViewAndIndex;
        var parentView;
        var addedProps = newProps || [];
        if (expression.indexOf(bc.bindingValueKey) > -1) {
            model[bc.bindingValueKey] = model;
            addedProps.push(bc.bindingValueKey);
        }
        if (expression.indexOf(bc.parentValueKey) > -1) {
            parentView = this.getParentView(this.target.get(), bc.parentValueKey).view;
            if (parentView) {
                model[bc.parentValueKey] = parentView.bindingContext;
                addedProps.push(bc.parentValueKey);
            }
        }
        var parentsArray = expression.match(bindingBuilder.parentsRegex);
        if (parentsArray) {
            for (var i = 0; i < parentsArray.length; i++) {
                parentViewAndIndex = this.getParentView(this.target.get(), parentsArray[i]);
                if (parentViewAndIndex.view) {
                    model[bc.parentsValueKey] = model[bc.parentsValueKey] || {};
                    model[bc.parentsValueKey][parentViewAndIndex.index] = parentViewAndIndex.view.bindingContext;
                    addedProps.push(bc.parentsValueKey);
                }
            }
        }
    };
    Binding.prototype.getSourcePropertyValue = function () {
        if (this.options.expression) {
            var changedModel = {};
            changedModel[bc.bindingValueKey] = this.source ? this.source.get() : undefined;
            var expressionValue = this._getExpressionValue(this.options.expression, false, changedModel);
            if (expressionValue instanceof Error) {
                trace.write(expressionValue.message, trace.categories.Binding, trace.messageType.error);
            }
            else {
                return expressionValue;
            }
        }
        if (this.sourceOptions) {
            var sourceOptionsInstance = this.sourceOptions.instance.get();
            if (this.sourceOptions.property === bc.bindingValueKey) {
                return sourceOptionsInstance;
            }
            else if ((sourceOptionsInstance instanceof observable_1.Observable) && (this.sourceOptions.property && this.sourceOptions.property !== "")) {
                return sourceOptionsInstance.get(this.sourceOptions.property);
            }
            else if (sourceOptionsInstance && this.sourceOptions.property && this.sourceOptions.property !== "" &&
                this.sourceOptions.property in sourceOptionsInstance) {
                return sourceOptionsInstance[this.sourceOptions.property];
            }
            else {
                trace.write("Property: '" + this.sourceOptions.property + "' is invalid or does not exist. SourceProperty: '" + this.options.sourceProperty + "'", trace.categories.Binding, trace.messageType.error);
            }
        }
        return null;
    };
    Binding.prototype.clearBinding = function () {
        this.clearSource();
        this.updateTarget(undefined);
    };
    Binding.prototype.updateTarget = function (value) {
        if (this.updating) {
            return;
        }
        this.updateOptions(this.targetOptions, types.isNullOrUndefined(value) ? dependency_observable_1.unsetValue : value);
    };
    Binding.prototype.updateSource = function (value) {
        if (this.updating || !this.source || !this.source.get()) {
            return;
        }
        this.updateOptions(this.sourceOptions, value);
    };
    Binding.prototype.getParentView = function (target, property) {
        if (!target || !(target instanceof viewModule.View)) {
            return { view: null, index: null };
        }
        var result;
        if (property === bc.parentValueKey) {
            result = target.parent;
        }
        var index = null;
        if (property.indexOf(bc.parentsValueKey) === 0) {
            result = target.parent;
            var indexParams = paramsRegex.exec(property);
            if (indexParams && indexParams.length > 1) {
                index = indexParams[2];
            }
            if (!isNaN(index)) {
                var indexAsInt = parseInt(index);
                while (indexAsInt > 0) {
                    result = result.parent;
                    indexAsInt--;
                }
            }
            else if (types.isString(index)) {
                while (result && result.typeName !== index) {
                    result = result.parent;
                }
            }
        }
        return { view: result, index: index };
    };
    Binding.prototype.resolveOptions = function (obj, properties) {
        var objectsAndProperties = this.resolveObjectsAndProperties(obj, properties);
        if (objectsAndProperties.length > 0) {
            var resolvedObj = objectsAndProperties[objectsAndProperties.length - 1].instance;
            var prop = objectsAndProperties[objectsAndProperties.length - 1].property;
            return {
                instance: new WeakRef(this.sourceAsObject(resolvedObj)),
                property: prop
            };
        }
        return null;
    };
    Binding.prototype.updateOptions = function (options, value) {
        var optionsInstance;
        if (options && options.instance) {
            optionsInstance = options.instance.get();
        }
        if (!optionsInstance) {
            return;
        }
        this.updating = true;
        try {
            if (optionsInstance instanceof Bindable &&
                viewModule.isEventOrGesture(options.property, optionsInstance) &&
                types.isFunction(value)) {
                optionsInstance.off(options.property, null, optionsInstance.bindingContext);
                optionsInstance.on(options.property, value, optionsInstance.bindingContext);
            }
            else {
                var specialSetter = specialProperties.getSpecialPropertySetter(options.property);
                if (specialSetter) {
                    specialSetter(optionsInstance, value);
                }
                else {
                    if (optionsInstance instanceof observable_1.Observable) {
                        optionsInstance.set(options.property, value);
                    }
                    else {
                        optionsInstance[options.property] = value;
                    }
                }
            }
        }
        catch (ex) {
            trace.write("Binding error while setting property " + options.property + " of " + optionsInstance + ": " + ex, trace.categories.Binding, trace.messageType.error);
        }
        this.updating = false;
    };
    return Binding;
}());
exports.Binding = Binding;
