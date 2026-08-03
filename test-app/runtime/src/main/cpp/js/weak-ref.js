// `clear` outlives init, so the prototype it flags is captured now rather than
// reached through the global at call time.
const WeakRefPrototype = global.WeakRef.prototype;

WeakRefPrototype.get = WeakRefPrototype.deref;
WeakRefPrototype.__hasWarnedAboutClear = false;
WeakRefPrototype.clear = () => {
    if (WeakRefPrototype.__hasWarnedAboutClear) {
        return;
    }
    WeakRefPrototype.__hasWarnedAboutClear = true;
    console.warn('WeakRef.clear() is non-standard and has been deprecated. It does nothing and the call can be safely removed.');
}
