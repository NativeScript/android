// Boots jasmine-core for the test app. The test-runner-deps plugin
// (test-app/test-runner-deps) stages jasmine-core from the repo's node_modules
// into ./vendor at build time.
var jasmineCore = require("./vendor/jasmine-core/jasmine");
var JUnitXmlReporter = require("./jasmine-reporters/junit_reporter").JUnitXmlReporter;
var TerminalReporter = require("./jasmine-reporters/terminal_reporter").TerminalReporter;

jasmineCore.installGlobals(global);

var env = jasmine.getEnv();

env.configure({
    // Suites carry state from one spec to the next (the shared Workers suite
    // counts live runtimes across its specs), so they run in declaration order.
    random: false
});

// jasmine-core's pretty-printer falls back to value.toString() for tagged objects
// it does not recognize, which throws for null-prototype ones such as module
// namespaces. toBe() builds its message even when it passes, so without this an
// expect(namespace).toBe(sameNamespace) throws.
beforeEach(function () {
    jasmine.addCustomObjectFormatter(function (value) {
        if (value !== null && typeof value === "object" && typeof value.toString !== "function") {
            var tag = Object.prototype.toString.call(value);
            if (tag !== "[object Object]") {
                return tag;
            }
        }
        return undefined;
    });
});

env.addReporter(new TerminalReporter({
    verbosity: 2
}));
env.addReporter(new JUnitXmlReporter());

// Without a results file the runtests harness only gives up after its full
// timeout, so a run that cannot start still writes one failing testcase.
function saveRunFailure(message) {
    var xml = '<?xml version="1.0" encoding="UTF-8" ?>\n<testsuites>' +
        '\n <testsuite name="Jasmine run" hostname="localhost" time="0" errors="0" tests="1" skipped="0" failures="1">' +
        '\n  <testcase classname="Jasmine run" name="(run could not start)" time="0">' +
        '\n   <failure type="exception" message="Jasmine could not run the suite">' +
        '<![CDATA[' + message.replace(/\]\]>/g, ']]]]><![CDATA[>') + ']]>' +
        '\n   </failure>\n  </testcase>\n </testsuite>\n</testsuites>';
    global.__JUnitSaveResults(xml);
}

exports.execute = function () {
    env.execute().catch(function (e) {
        var message = String((e && e.stack) || e);
        console.log("Jasmine could not run the suite: " + message);
        saveRunFailure(message);
    });
};
