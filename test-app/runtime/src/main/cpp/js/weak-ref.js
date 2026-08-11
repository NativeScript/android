global.WeakRef.prototype.get = global.WeakRef.prototype.deref;
global.WeakRef.prototype.__hasWarnedAboutClear = false;
global.WeakRef.prototype.clear = () => {
    if(global.WeakRef.prototype.__hasWarnedAboutClear) {
        return;
    }
    global.WeakRef.prototype.__hasWarnedAboutClear = true;
    console.warn('WeakRef.clear() is non-standard and has been deprecated. It does nothing and the call can be safely removed.');
}
