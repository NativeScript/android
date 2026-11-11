// The inspector reads Symbol.toStringTag while it formats a console argument,
// so this getter runs inside the outer console.log.
var arg = {};
Object.defineProperty(arg, Symbol.toStringTag, {
	get: function () {
		console.log("nested");
		return "Nested";
	}
});
console.log(arg);
postMessage("alive");
