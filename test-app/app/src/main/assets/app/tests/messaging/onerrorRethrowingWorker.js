// The scope's onerror throws an error whose message holds a NUL and an
// unpaired surrogate, and whose stack getter throws.
onerror = function () {
    var error = new TypeError("before\0after \uD800");
    Object.defineProperty(error, "stack", {
        get: function () { throw new RangeError("thrown by the stack getter"); }
    });
    throw error;
};
onmessage = function () {
    throw new Error("thrown by onmessage");
};
