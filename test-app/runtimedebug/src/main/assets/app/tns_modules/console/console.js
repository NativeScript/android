var trace = require("trace");
var platform = require("platform");
var Console = (function () {
    function Console() {
        this.TAG = "JS";
        this.dir = this.dump;
        this._timers = {};
        this._stripFirstTwoLinesRegEx = /^([^\n]*?\n){2}((.|\n)*)$/gmi;
    }
    Console.prototype.sprintf = function (message) {
        var regex = /%%|%(\d+\$)?([-+\'#0 ]*)(\*\d+\$|\*|\d+)?(\.(\*\d+\$|\*|\d+))?([scboxXuideEfFgGj])/g;
        var a = arguments;
        var i = 0;
        var format = a[i++];
        var pad = function (str, len, chr, leftJustify) {
            if (!chr) {
                chr = ' ';
            }
            var padding = (str.length >= len) ? '' : new Array(1 + len - str.length >>> 0)
                .join(chr);
            return leftJustify ? str + padding : padding + str;
        };
        var justify = function (value, prefix, leftJustify, minWidth, zeroPad, customPadChar) {
            var diff = minWidth - value.length;
            if (diff > 0) {
                if (leftJustify || !zeroPad) {
                    value = pad(value, minWidth, customPadChar, leftJustify);
                }
                else {
                    value = value.slice(0, prefix.length) + pad('', diff, '0', true) + value.slice(prefix.length);
                }
            }
            return value;
        };
        var formatBaseX = function (value, base, prefix, leftJustify, minWidth, precision, zeroPad) {
            var number = value >>> 0;
            prefix = prefix && number && {
                '2': '0b',
                '8': '0',
                '16': '0x'
            }[base] || '';
            value = prefix + pad(number.toString(base), precision || 0, '0', false);
            return justify(value, prefix, leftJustify, minWidth, zeroPad);
        };
        var formatString = function (value, leftJustify, minWidth, precision, zeroPad, customPadChar) {
            if (precision != null) {
                value = value.slice(0, precision);
            }
            return justify(value, '', leftJustify, minWidth, zeroPad, customPadChar);
        };
        var that = this;
        var doFormat = function (substring, valueIndex, flags, minWidth, _, precision, type) {
            var number, prefix, method, textTransform, value;
            if (substring === '%%') {
                return '%';
            }
            var leftJustify = false;
            var positivePrefix = '';
            var zeroPad = false;
            var prefixBaseX = false;
            var customPadChar = ' ';
            var flagsl = flags.length;
            for (var j = 0; flags && j < flagsl; j++) {
                switch (flags.charAt(j)) {
                    case ' ':
                        positivePrefix = ' ';
                        break;
                    case '+':
                        positivePrefix = '+';
                        break;
                    case '-':
                        leftJustify = true;
                        break;
                    case "'":
                        customPadChar = flags.charAt(j + 1);
                        break;
                    case '0':
                        zeroPad = true;
                        customPadChar = '0';
                        break;
                    case '#':
                        prefixBaseX = true;
                        break;
                }
            }
            if (!minWidth) {
                minWidth = 0;
            }
            else if (minWidth === '*') {
                minWidth = +a[i++];
            }
            else if (minWidth.charAt(0) === '*') {
                minWidth = +a[minWidth.slice(1, -1)];
            }
            else {
                minWidth = +minWidth;
            }
            if (minWidth < 0) {
                minWidth = -minWidth;
                leftJustify = true;
            }
            if (!isFinite(minWidth)) {
                throw new Error('sprintf: (minimum-)width must be finite');
            }
            if (!precision) {
                precision = 'fFeE'.indexOf(type) > -1 ? 6 : (type === 'd') ? 0 : undefined;
            }
            else if (precision === '*') {
                precision = +a[i++];
            }
            else if (precision.charAt(0) === '*') {
                precision = +a[precision.slice(1, -1)];
            }
            else {
                precision = +precision;
            }
            value = valueIndex ? a[valueIndex.slice(0, -1)] : a[i++];
            switch (type) {
                case 's':
                    return formatString(String(value), leftJustify, minWidth, precision, zeroPad, customPadChar);
                case 'c':
                    return formatString(String.fromCharCode(+value), leftJustify, minWidth, precision, zeroPad);
                case 'b':
                    return formatBaseX(value, 2, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
                case 'o':
                    return formatBaseX(value, 8, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
                case 'x':
                    return formatBaseX(value, 16, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
                case 'X':
                    return formatBaseX(value, 16, prefixBaseX, leftJustify, minWidth, precision, zeroPad)
                        .toUpperCase();
                case 'u':
                    return formatBaseX(value, 10, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
                case 'i':
                case 'd':
                    number = +value || 0;
                    number = Math.round(number - number % 1);
                    prefix = number < 0 ? '-' : positivePrefix;
                    value = prefix + pad(String(Math.abs(number)), precision, '0', false);
                    return justify(value, prefix, leftJustify, minWidth, zeroPad);
                case 'e':
                case 'E':
                case 'f':
                case 'F':
                case 'g':
                case 'G':
                    number = +value;
                    prefix = number < 0 ? '-' : positivePrefix;
                    method = ['toExponential', 'toFixed', 'toPrecision']['efg'.indexOf(type.toLowerCase())];
                    textTransform = ['toString', 'toUpperCase']['eEfFgG'.indexOf(type) % 2];
                    value = prefix + Math.abs(number)[method](precision);
                    return justify(value, prefix, leftJustify, minWidth, zeroPad)[textTransform]();
                case 'j':
                    return that.createDump(value);
                default:
                    return substring;
            }
        };
        return format.replace(regex, doFormat);
    };
    Console.prototype.formatParams = function (message) {
        if (arguments.length <= 1) {
            return "" + message;
        }
        var res = this.sprintf.apply(this, arguments);
        if (res === message) {
            var args = Array.prototype.slice.call(arguments);
            return args.join(' ');
        }
        return res;
    };
    Console.prototype.timeMillis = function () {
        return java.lang.System.nanoTime() / 1000000;
    };
    Console.prototype.time = function (reportName) {
        var name = reportName ? '__' + reportName : '__internal_console_time__';
        if (('undefined' === typeof (this._timers[name])) || (this._timers.hasOwnProperty(name))) {
            this._timers[name] = this.timeMillis();
        }
        else {
            this.warn('invalid name for timer console.time(' + reportName + ')');
        }
    };
    Console.prototype.timeEnd = function (reportName) {
        var name = reportName ? '__' + reportName : '__internal_console_time__';
        if (this._timers.hasOwnProperty(name)) {
            var val = this._timers[name];
            if (val) {
                var time = this.timeMillis();
                this.info('console.time(' + reportName + '): %.6f ms', (time - val));
                this._timers[name] = undefined;
            }
            else {
                this.warn('undefined console.time(' + reportName + ')');
            }
        }
    };
    Console.prototype.assert = function (test, message) {
        var formatParams = [];
        for (var _i = 2; _i < arguments.length; _i++) {
            formatParams[_i - 2] = arguments[_i];
        }
        if (!test) {
            Array.prototype.shift.apply(arguments);
            var formatedMessage = this.formatParams.apply(this, arguments);
            this.error(formatedMessage, trace.messageType.error);
            if (global.__consoleMessage) {
                global.__consoleMessage(formatedMessage, "error");
            }
        }
    };
    Console.prototype.info = function (message) {
        var formatParams = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            formatParams[_i - 1] = arguments[_i];
        }
        this.logMessage(this.formatParams.apply(this, arguments), trace.messageType.info);
    };
    Console.prototype.warn = function (message) {
        var formatParams = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            formatParams[_i - 1] = arguments[_i];
        }
        var formatedMessage = this.formatParams.apply(this, arguments);
        this.logMessage(formatedMessage, trace.messageType.warn);
        if (global.__consoleMessage) {
            global.__consoleMessage(formatedMessage, "warning");
        }
    };
    Console.prototype.error = function (message) {
        var formatParams = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            formatParams[_i - 1] = arguments[_i];
        }
        var formatedMessage = this.formatParams.apply(this, arguments);
        this.logMessage(formatedMessage, trace.messageType.error);
        if (global.__consoleMessage) {
            global.__consoleMessage(formatedMessage, "error");
        }
    };
    Console.prototype.log = function (message) {
        var formatParams = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            formatParams[_i - 1] = arguments[_i];
        }
        var formatedMessage = this.formatParams.apply(this, arguments);
        this.logMessage(formatedMessage, trace.messageType.log);
        if (global.__consoleMessage) {
            global.__consoleMessage(formatedMessage, "log");
        }
    };
    Console.prototype.logMessage = function (message, messageType) {
        if (!global.android) {
            return;
        }
        var arrayToLog = [];
        if (message.length > 4000) {
            var i;
            for (i = 0; i * 4000 < message.length; i++) {
                arrayToLog.push(message.substr((i * 4000), 4000));
            }
        }
        else {
            arrayToLog.push(message);
        }
        for (i = 0; i < arrayToLog.length; i++) {
            switch (messageType) {
                case trace.messageType.log: {
                    android.util.Log.v(this.TAG, arrayToLog[i]);
                    break;
                }
                case trace.messageType.warn: {
                    android.util.Log.w(this.TAG, arrayToLog[i]);
                    break;
                }
                case trace.messageType.error: {
                    android.util.Log.e(this.TAG, arrayToLog[i]);
                    break;
                }
                case trace.messageType.info: {
                    android.util.Log.i(this.TAG, arrayToLog[i]);
                    break;
                }
            }
        }
    };
    Console.prototype.trace = function () {
        var stack;
        stack = (new Error()).stack.toString();
        stack = stack.replace(this._stripFirstTwoLinesRegEx, "$2");
        stack = "Stack Trace:\n" + stack;
        this.log(stack);
    };
    Console.prototype.createDump = function (obj) {
        var result = [];
        if (null == obj) {
            result.push("=== dump(): object is 'null' ===");
            return result.join('');
        }
        if ("undefined" === typeof obj) {
            result.push("=== dump(): object is 'undefined' ===");
            return result.join('');
        }
        result.push('=== dump(): dumping members ===\n');
        var stringifyValueCache = [];
        var stringifyKeyCache = [];
        result.push(JSON.stringify(obj, function (k, v) {
            stringifyKeyCache.push(k);
            if (typeof v === 'object' && v !== null) {
                if (stringifyValueCache.indexOf(v) !== -1) {
                    return "#CR:" + (v.toString ? v.toString() : v);
                }
                stringifyValueCache.push(v);
            }
            if (typeof v === 'function') {
                return k + "()" + v;
            }
            return v;
        }, 4));
        result.push('\n=== dump(): dumping function and properties names ===\n');
        for (var id in obj) {
            try {
                if (typeof (obj[id]) === 'function') {
                    result.push(id + '()\n');
                }
                else {
                    if (typeof (obj[id]) !== 'object' && stringifyKeyCache.indexOf(id) === -1) {
                        result.push(id + ": " + (obj[id] + '\n'));
                    }
                }
            }
            catch (err) {
                result.push(id + ': inaccessible');
            }
        }
        stringifyValueCache = null;
        stringifyKeyCache = null;
        result.push('=== dump(): finished ===');
        return result.join('');
    };
    Console.prototype.dump = function (obj) {
        var dump = this.createDump(obj);
        if (platform.device.os === platform.platformNames.android) {
            this.log(dump);
        }
        else if (platform.device.os === platform.platformNames.ios) {
            console.log(dump);
        }
    };
    return Console;
}());
exports.Console = Console;
