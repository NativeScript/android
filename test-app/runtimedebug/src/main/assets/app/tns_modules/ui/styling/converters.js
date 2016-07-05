var enums = require("ui/enums");
var color = require("color");
var types = require("utils/types");
function colorConverter(value) {
    return new color.Color(value);
}
exports.colorConverter = colorConverter;
function floatConverter(value) {
    var result = parseFloat(value);
    return result;
}
exports.floatConverter = floatConverter;
function fontSizeConverter(value) {
    return floatConverter(value);
}
exports.fontSizeConverter = fontSizeConverter;
function textAlignConverter(value) {
    switch (value) {
        case enums.TextAlignment.left:
        case enums.TextAlignment.center:
        case enums.TextAlignment.right:
            return value;
        default:
            throw new Error("CSS text-align \"" + value + "\" is not supported.");
    }
}
exports.textAlignConverter = textAlignConverter;
function textDecorationConverter(value) {
    var values = (value + "").split(" ");
    if (values.indexOf(enums.TextDecoration.none) !== -1 || values.indexOf(enums.TextDecoration.underline) !== -1 || values.indexOf(enums.TextDecoration.lineThrough) !== -1) {
        return value;
    }
    else {
        throw new Error("CSS text-decoration \"" + value + "\" is not supported.");
    }
}
exports.textDecorationConverter = textDecorationConverter;
function whiteSpaceConverter(value) {
    switch (value) {
        case enums.WhiteSpace.normal:
        case enums.WhiteSpace.nowrap:
            return value;
        default:
            throw new Error("CSS white-space \"" + value + "\" is not supported.");
    }
}
exports.whiteSpaceConverter = whiteSpaceConverter;
function textTransformConverter(value) {
    switch (value) {
        case enums.TextTransform.none:
        case enums.TextTransform.uppercase:
        case enums.TextTransform.lowercase:
        case enums.TextTransform.capitalize:
            return value;
        default:
            throw new Error("CSS text-transform \"" + value + "\" is not supported.");
    }
}
exports.textTransformConverter = textTransformConverter;
exports.numberConverter = parseFloat;
function visibilityConverter(value) {
    if (value.toLowerCase() === enums.Visibility.collapsed) {
        return enums.Visibility.collapsed;
    }
    else if (value.toLowerCase() === enums.Visibility.collapse) {
        return enums.Visibility.collapse;
    }
    return enums.Visibility.visible;
}
exports.visibilityConverter = visibilityConverter;
function opacityConverter(value) {
    var result = parseFloat(value);
    result = Math.max(0.0, result);
    result = Math.min(1.0, result);
    return result;
}
exports.opacityConverter = opacityConverter;
function timeConverter(value) {
    var result = parseFloat(value);
    if (value.indexOf("ms") === -1) {
        result = result * 1000;
    }
    result = Math.max(0.0, result);
    return result;
}
exports.timeConverter = timeConverter;
function bezieArgumentConverter(value) {
    var result = parseFloat(value);
    result = Math.max(0.0, result);
    result = Math.min(1.0, result);
    return result;
}
exports.bezieArgumentConverter = bezieArgumentConverter;
function animationTimingFunctionConverter(value) {
    var result = enums.AnimationCurve.ease;
    switch (value) {
        case "ease":
            result = enums.AnimationCurve.ease;
            break;
        case "linear":
            result = enums.AnimationCurve.linear;
            break;
        case "ease-in":
            result = enums.AnimationCurve.easeIn;
            break;
        case "ease-out":
            result = enums.AnimationCurve.easeOut;
            break;
        case "ease-in-out":
            result = enums.AnimationCurve.easeInOut;
            break;
        case "spring":
            result = enums.AnimationCurve.spring;
            break;
        default:
            if (value.indexOf("cubic-bezier(") === 0) {
                var bezierArr = value.substring(13).split(/[,]+/);
                if (bezierArr.length !== 4) {
                    throw new Error("Invalid value for animation: " + value);
                }
                result = enums.AnimationCurve.cubicBezier(bezieArgumentConverter(bezierArr[0]), bezieArgumentConverter(bezierArr[1]), bezieArgumentConverter(bezierArr[2]), bezieArgumentConverter(bezierArr[3]));
            }
            else {
                throw new Error("Invalid value for animation: " + value);
            }
            break;
    }
    return result;
}
exports.animationTimingFunctionConverter = animationTimingFunctionConverter;
function transformConverter(value) {
    if (value === "none") {
        var operations = {};
        operations[value] = value;
        return operations;
    }
    else if (types.isString(value)) {
        var operations = {};
        var operator = "";
        var pos = 0;
        while (pos < value.length) {
            if (value[pos] === " " || value[pos] === ",") {
                pos++;
            }
            else if (value[pos] === "(") {
                var start = pos + 1;
                while (pos < value.length && value[pos] !== ")") {
                    pos++;
                }
                var operand = value.substring(start, pos);
                operations[operator] = operand.trim();
                operator = "";
                pos++;
            }
            else {
                operator += value[pos++];
            }
        }
        return operations;
    }
    else {
        return undefined;
    }
}
exports.transformConverter = transformConverter;
