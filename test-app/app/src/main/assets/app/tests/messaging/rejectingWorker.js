onerror = function () {
    throw new Error("thrown by scope onerror");
};
Promise.reject(new Error("original rejection"));
