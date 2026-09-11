// Entry for testWorkerResourceLimits: allocates and never releases, so the
// isolate walks into the maxOldGenerationSizeMb cap the parent set.
var keep = [];
for (;;) {
    keep.push(new Array(100000).fill(1));
}
