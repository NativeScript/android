// Entry for testWorkerTerminateDuringRequireMint. Every call names a directory
// no earlier call used, so each one runs the native mint instead of being
// served from the per-directory require cache.
var createRequire = require("ns:module").createRequire;

postMessage("spinning");

// Bounded so a worker that terminate() failed to stop does not outlive the spec.
var deadline = Date.now() + 2000;
for (var i = 0; Date.now() < deadline;) {
    for (var batch = 0; batch < 200; batch++, i++) {
        createRequire("/worker-require-mint-spin/" + i + "/");
    }
}
