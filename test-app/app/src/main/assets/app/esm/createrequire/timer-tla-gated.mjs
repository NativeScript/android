// Own fixture file, never shared with the pumpRunLoop spec: a settled module
// would satisfy any later require from the registry without pumping at all,
// so the default-options spec must park a graph of its own.
export const value = await new Promise(function (resolve) {
    __ns__setTimeout(function () { resolve("timer-gated"); }, 10);
});
