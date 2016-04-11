if (!global.__didThrow) {
    global.__didThrow = true;
    throw new Error('did throw');
} else {
    delete global.__didThrow;
    TNSLog("no throw");
}
