var keyframeAnimationModule = require("ui/animation/keyframe-animation");
var converters = require("../styling/converters");
var types = require("utils/types");
var colorModule = require("color");
var styleProperty = require("ui/styling/style-property");
var animationProperties = {
    "animation-name": function (info, declaration) { return info.name = declaration.value; },
    "animation-duration": function (info, declaration) { return info.duration = converters.timeConverter(declaration.value); },
    "animation-delay": function (info, declaration) { return info.delay = converters.timeConverter(declaration.value); },
    "animation-timing-function": function (info, declaration) { return info.curve = converters.animationTimingFunctionConverter(declaration.value); },
    "animation-iteration-count": function (info, declaration) { return declaration.value === "infinite" ? info.iterations = Number.MAX_VALUE : info.iterations = converters.numberConverter(declaration.value); },
    "animation-direction": function (info, declaration) { return info.isReverse = declaration.value === "reverse"; },
    "animation-fill-mode": function (info, declaration) { return info.isForwards = declaration.value === "forwards"; }
};
var CssAnimationParser = (function () {
    function CssAnimationParser() {
    }
    CssAnimationParser.keyframeAnimationsFromCSSDeclarations = function (declarations) {
        var animations = new Array();
        var animationInfo = undefined;
        if (declarations === null || declarations === undefined) {
            return undefined;
        }
        for (var _i = 0, declarations_1 = declarations; _i < declarations_1.length; _i++) {
            var declaration = declarations_1[_i];
            if (declaration.property === "animation") {
                CssAnimationParser.keyframeAnimationsFromCSSProperty(declaration.value, animations);
            }
            else {
                var propertyHandler = animationProperties[declaration.property];
                if (propertyHandler) {
                    if (animationInfo === undefined) {
                        animationInfo = new keyframeAnimationModule.KeyframeAnimationInfo();
                        animations.push(animationInfo);
                    }
                    propertyHandler(animationInfo, declaration);
                }
            }
        }
        return animations.length === 0 ? undefined : animations;
    };
    CssAnimationParser.keyframesArrayFromCSS = function (cssKeyframes) {
        var parsedKeyframes = new Array();
        for (var _i = 0, _a = cssKeyframes.keyframes; _i < _a.length; _i++) {
            var keyframe = _a[_i];
            var declarations = CssAnimationParser.parseKeyframeDeclarations(keyframe);
            for (var _b = 0, _c = keyframe.values; _b < _c.length; _b++) {
                var time_1 = _c[_b];
                if (time_1 === "from") {
                    time_1 = 0;
                }
                else if (time_1 === "to") {
                    time_1 = 1;
                }
                else {
                    time_1 = parseFloat(time_1) / 100;
                    if (time_1 < 0) {
                        time_1 = 0;
                    }
                    if (time_1 > 100) {
                        time_1 = 100;
                    }
                }
                var current = parsedKeyframes[time_1];
                if (current === undefined) {
                    current = {};
                    current.duration = time_1;
                    parsedKeyframes[time_1] = current;
                }
                for (var _d = 0, _e = keyframe.declarations; _d < _e.length; _d++) {
                    var declaration = _e[_d];
                    if (declaration.property === "animation-timing-function") {
                        current.curve = converters.animationTimingFunctionConverter(declaration.value);
                    }
                }
                current.declarations = declarations;
            }
        }
        var array = new Array();
        for (var parsedKeyframe in parsedKeyframes) {
            array.push(parsedKeyframes[parsedKeyframe]);
        }
        array.sort(function (a, b) { return a.duration - b.duration; });
        return array;
    };
    CssAnimationParser.keyframeAnimationsFromCSSProperty = function (value, animations) {
        if (types.isString(value)) {
            var values = value.split(/[,]+/);
            for (var _i = 0, values_1 = values; _i < values_1.length; _i++) {
                var parsedValue = values_1[_i];
                var animationInfo = new keyframeAnimationModule.KeyframeAnimationInfo();
                var arr = parsedValue.trim().split(/[ ]+/);
                if (arr.length > 0) {
                    animationInfo.name = arr[0];
                }
                if (arr.length > 1) {
                    animationInfo.duration = converters.timeConverter(arr[1]);
                }
                if (arr.length > 2) {
                    animationInfo.curve = converters.animationTimingFunctionConverter(arr[2]);
                }
                if (arr.length > 3) {
                    animationInfo.delay = converters.timeConverter(arr[3]);
                }
                if (arr.length > 4) {
                    animationInfo.iterations = parseInt(arr[4]);
                }
                if (arr.length > 5) {
                    animationInfo.isReverse = arr[4] === "reverse";
                }
                if (arr.length > 6) {
                    animationInfo.isForwards = arr[5] === "forwards";
                }
                if (arr.length > 7) {
                    throw new Error("Invalid value for animation: " + value);
                }
                animations.push(animationInfo);
            }
        }
    };
    CssAnimationParser.parseKeyframeDeclarations = function (keyframe) {
        var declarations = {};
        var transforms = { scale: undefined, translate: undefined };
        for (var _i = 0, _a = keyframe.declarations; _i < _a.length; _i++) {
            var declaration = _a[_i];
            var property = styleProperty.getPropertyByCssName(declaration.property);
            if (property) {
                var val = declaration.value;
                if (property.name === "opacity") {
                    val = parseFloat(val);
                }
                else if (property.name === "backgroundColor") {
                    val = new colorModule.Color(val);
                }
                declarations[property.name] = val;
            }
            else {
                var pairs = styleProperty.getShorthandPairs(declaration.property, declaration.value);
                if (pairs) {
                    for (var j = 0; j < pairs.length; j++) {
                        var pair = pairs[j];
                        if (!this.preprocessAnimationValues(pair, transforms)) {
                            declarations[pair.property.name] = pair.value;
                        }
                    }
                }
            }
        }
        if (transforms.scale !== undefined) {
            declarations["scale"] = transforms.scale;
        }
        if (transforms.translate !== undefined) {
            declarations["translate"] = transforms.translate;
        }
        var array = new Array();
        for (var declaration in declarations) {
            var keyframeDeclaration = {};
            keyframeDeclaration.property = declaration;
            keyframeDeclaration.value = declarations[declaration];
            array.push(keyframeDeclaration);
        }
        return array;
    };
    CssAnimationParser.preprocessAnimationValues = function (pair, transforms) {
        if (pair.property.name === "scaleX") {
            if (transforms.scale === undefined) {
                transforms.scale = { x: 1, y: 1 };
            }
            transforms.scale.x = pair.value;
            return true;
        }
        if (pair.property.name === "scaleY") {
            if (transforms.scale === undefined) {
                transforms.scale = { x: 1, y: 1 };
            }
            transforms.scale.y = pair.value;
            return true;
        }
        if (pair.property.name === "translateX") {
            if (transforms.translate === undefined) {
                transforms.translate = { x: 0, y: 0 };
            }
            transforms.translate.x = pair.value;
            return true;
        }
        if (pair.property.name === "translateY") {
            if (transforms.translate === undefined) {
                transforms.translate = { x: 0, y: 0 };
            }
            transforms.translate.y = pair.value;
            return true;
        }
        return false;
    };
    return CssAnimationParser;
}());
exports.CssAnimationParser = CssAnimationParser;
