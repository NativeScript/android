var trace = require("trace");
var cssSelector = require("ui/styling/css-selector");
var cssParser = require("css");
var application = require("application");
var keyframeAnimation = require("ui/animation/keyframe-animation");
var cssAnimationParser = require("./css-animation-parser");
var observable = require("ui/core/dependency-observable");
var types;
function ensureTypes() {
    if (!types) {
        types = require("utils/types");
    }
}
var utils;
function ensureUtils() {
    if (!utils) {
        utils = require("utils/utils");
    }
}
var fs;
function ensureFS() {
    if (!fs) {
        fs = require("file-system");
    }
}
var vs;
function ensureVisualState() {
    if (!vs) {
        vs = require("./visual-state");
    }
}
var pattern = /('|")(.*?)\1/;
var StyleScope = (function () {
    function StyleScope() {
        this._statesByKey = {};
        this._viewIdToKey = {};
        this._localCssSelectors = [];
        this._localCssSelectorVersion = 0;
        this._localCssSelectorsAppliedVersion = 0;
        this._applicationCssSelectorsAppliedVersion = 0;
        this._keyframes = {};
    }
    Object.defineProperty(StyleScope.prototype, "css", {
        get: function () {
            return this._css;
        },
        set: function (value) {
            this._cssFileName = undefined;
            this.setCss(value);
        },
        enumerable: true,
        configurable: true
    });
    StyleScope.prototype.addCss = function (cssString, cssFileName) {
        this.setCss(cssString, cssFileName, true);
    };
    StyleScope.prototype.setCss = function (cssString, cssFileName, append) {
        if (append === void 0) { append = false; }
        this._css = this._css && append ? this._css + cssString : cssString;
        if (cssFileName) {
            this._cssFileName = cssFileName;
        }
        this._reset();
        var parsedSelectors = StyleScope.createSelectorsFromCss(this._css, cssFileName, this._keyframes);
        if (append) {
            this._localCssSelectors.push.apply(this._localCssSelectors, parsedSelectors);
        }
        else {
            this._localCssSelectors = parsedSelectors;
        }
        this._localCssSelectorVersion++;
        this.ensureSelectors();
    };
    StyleScope.prototype.removeSelectors = function (selectorExpression) {
        for (var i = this._mergedCssSelectors.length - 1; i >= 0; i--) {
            var selector = this._mergedCssSelectors[i];
            if (selector.expression === selectorExpression) {
                this._mergedCssSelectors.splice(i, 1);
            }
        }
    };
    StyleScope.prototype.getKeyframeAnimationWithName = function (animationName) {
        var keyframes = this._keyframes[animationName];
        if (keyframes !== undefined) {
            var animation = new keyframeAnimation.KeyframeAnimationInfo();
            animation.keyframes = cssAnimationParser.CssAnimationParser.keyframesArrayFromCSS(keyframes);
            return animation;
        }
        return undefined;
    };
    StyleScope.createSelectorsFromCss = function (css, cssFileName, keyframes) {
        try {
            var pageCssSyntaxTree = css ? cssParser.parse(css, { source: cssFileName }) : null;
            var pageCssSelectors = new Array();
            if (pageCssSyntaxTree) {
                pageCssSelectors = StyleScope._joinCssSelectorsArrays([pageCssSelectors, StyleScope.createSelectorsFromImports(pageCssSyntaxTree, keyframes)]);
                pageCssSelectors = StyleScope._joinCssSelectorsArrays([pageCssSelectors, StyleScope.createSelectorsFromSyntaxTree(pageCssSyntaxTree, keyframes)]);
            }
            return pageCssSelectors;
        }
        catch (e) {
            trace.write("Css styling failed: " + e, trace.categories.Error, trace.messageType.error);
        }
    };
    StyleScope.createSelectorsFromImports = function (tree, keyframes) {
        var selectors = new Array();
        ensureTypes();
        if (!types.isNullOrUndefined(tree)) {
            var imports = tree["stylesheet"]["rules"].filter(function (r) { return r.type === "import"; });
            for (var i = 0; i < imports.length; i++) {
                var importItem = imports[i]["import"];
                var match = importItem && importItem.match(pattern);
                var url = match && match[2];
                if (!types.isNullOrUndefined(url)) {
                    ensureUtils();
                    if (utils.isFileOrResourcePath(url)) {
                        ensureFS();
                        var fileName = types.isString(url) ? url.trim() : "";
                        if (fileName.indexOf("~/") === 0) {
                            fileName = fs.path.join(fs.knownFolders.currentApp().path, fileName.replace("~/", ""));
                        }
                        if (fs.File.exists(fileName)) {
                            var file = fs.File.fromPath(fileName);
                            var text = file.readTextSync();
                            if (text) {
                                selectors = StyleScope._joinCssSelectorsArrays([selectors, StyleScope.createSelectorsFromCss(text, fileName, keyframes)]);
                            }
                        }
                    }
                }
            }
        }
        return selectors;
    };
    StyleScope.prototype.ensureSelectors = function () {
        var toMerge = [];
        if ((this._applicationCssSelectorsAppliedVersion !== application.cssSelectorVersion) ||
            (this._localCssSelectorVersion !== this._localCssSelectorsAppliedVersion) ||
            (!this._mergedCssSelectors)) {
            toMerge.push(application.cssSelectors);
            this._applicationCssSelectorsAppliedVersion = application.cssSelectorVersion;
            toMerge.push(this._localCssSelectors);
            this._localCssSelectorsAppliedVersion = this._localCssSelectorVersion;
            for (var keyframe in application.keyframes) {
                this._keyframes[keyframe] = application.keyframes[keyframe];
            }
        }
        if (toMerge.length > 0) {
            this._mergedCssSelectors = StyleScope._joinCssSelectorsArrays(toMerge);
            this._applyKeyframesOnSelectors();
            return true;
        }
        else {
            return false;
        }
    };
    StyleScope._joinCssSelectorsArrays = function (arrays) {
        var mergedResult = [];
        var i;
        for (i = 0; i < arrays.length; i++) {
            if (arrays[i]) {
                mergedResult.push.apply(mergedResult, arrays[i]);
            }
        }
        ensureUtils();
        mergedResult = utils.mergeSort(mergedResult, function (a, b) { return a.specificity - b.specificity; });
        return mergedResult;
    };
    StyleScope.prototype.applySelectors = function (view) {
        this.ensureSelectors();
        view.style._beginUpdate();
        var i;
        var selector;
        var matchedStateSelectors = new Array();
        for (i = 0; i < this._mergedCssSelectors.length; i++) {
            selector = this._mergedCssSelectors[i];
            if (selector.matches(view)) {
                if (selector instanceof cssSelector.CssVisualStateSelector) {
                    matchedStateSelectors.push(selector);
                }
                else {
                    selector.apply(view, observable.ValueSource.Css);
                }
            }
        }
        if (matchedStateSelectors.length > 0) {
            var key_1 = "";
            matchedStateSelectors.forEach(function (s) { return key_1 += s.key + "|"; });
            this._viewIdToKey[view._domId] = key_1;
            if (!this._statesByKey[key_1]) {
                this._createVisualsStatesForSelectors(key_1, matchedStateSelectors);
            }
        }
        view.style._endUpdate();
    };
    StyleScope.prototype.getVisualStates = function (view) {
        var key = this._viewIdToKey[view._domId];
        if (key === undefined) {
            return undefined;
        }
        return this._statesByKey[key];
    };
    StyleScope.prototype._createVisualsStatesForSelectors = function (key, matchedStateSelectors) {
        var i, allStates = {}, stateSelector;
        this._statesByKey[key] = allStates;
        ensureVisualState();
        var _loop_1 = function() {
            stateSelector = matchedStateSelectors[i];
            var visualState = allStates[stateSelector.state];
            if (!visualState) {
                visualState = new vs.VisualState();
                allStates[stateSelector.state] = visualState;
            }
            if (stateSelector.animations && stateSelector.animations.length > 0) {
                visualState.animatedSelectors.push(stateSelector);
            }
            else {
                stateSelector.eachSetter(function (property, value) {
                    visualState.setters[property.name] = value;
                });
            }
        };
        for (i = 0; i < matchedStateSelectors.length; i++) {
            _loop_1();
        }
    };
    StyleScope.createSelectorsFromSyntaxTree = function (ast, keyframes) {
        var result = [];
        var rules = ast.stylesheet.rules;
        var rule;
        var i;
        var j;
        for (i = 0; i < rules.length; i++) {
            rule = rules[i];
            if (rule.type === "rule") {
                var filteredDeclarations = [];
                if (rule.declarations) {
                    for (j = 0; j < rule.declarations.length; j++) {
                        var declaration = rule.declarations[j];
                        if (declaration.type === "declaration") {
                            filteredDeclarations.push({
                                property: declaration.property.toLowerCase(),
                                value: declaration.value
                            });
                        }
                    }
                }
                for (j = 0; j < rule.selectors.length; j++) {
                    result.push(cssSelector.createSelector(rule.selectors[j], filteredDeclarations));
                }
            }
            else if (rule.type === "keyframes") {
                keyframes[rule.name] = rule;
            }
        }
        return result;
    };
    StyleScope.prototype._reset = function () {
        this._statesByKey = {};
        this._viewIdToKey = {};
    };
    StyleScope.prototype._applyKeyframesOnSelectors = function () {
        for (var i = this._mergedCssSelectors.length - 1; i >= 0; i--) {
            var selector = this._mergedCssSelectors[i];
            if (selector.animations !== undefined) {
                for (var _i = 0, _a = selector.animations; _i < _a.length; _i++) {
                    var animation = _a[_i];
                    var keyframe = this._keyframes[animation.name];
                    if (keyframe !== undefined) {
                        animation.keyframes = cssAnimationParser.CssAnimationParser.keyframesArrayFromCSS(keyframe);
                    }
                }
            }
        }
    };
    return StyleScope;
}());
exports.StyleScope = StyleScope;
function applyInlineSyle(view, style) {
    try {
        var syntaxTree = cssParser.parse("local { " + style + " }", undefined);
        var filteredDeclarations = syntaxTree.stylesheet.rules[0].declarations.filter(function (val, i, arr) { return val.type === "declaration"; });
        cssSelector.applyInlineSyle(view, filteredDeclarations);
    }
    catch (ex) {
        trace.write("Applying local style failed: " + ex, trace.categories.Error, trace.messageType.error);
    }
}
exports.applyInlineSyle = applyInlineSyle;
