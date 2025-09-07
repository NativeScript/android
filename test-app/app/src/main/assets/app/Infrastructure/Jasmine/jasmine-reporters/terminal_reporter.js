(function(global) {
    var UNDEFINED,
        exportObject;

    if (typeof module !== "undefined" && module.exports) {
        exportObject = exports;
    } else {
        exportObject = global.jasmineReporters = global.jasmineReporters || {};
    }

    function elapsed(start, end) { return (end - start)/1000; }
    function isFailed(obj) { return obj.status === "failed"; }
    function isSkipped(obj) { return obj.status === "pending"; }
    function extend(dupe, obj) { // performs a shallow copy of all props of `obj` onto `dupe`
        for (var prop in obj) {
            if (obj.hasOwnProperty(prop)) {
                dupe[prop] = obj[prop];
            }
        }
        return dupe;
    }
    function log(str) {
        // Use console.log so our test checker can detect the output
        console.log(str);
        
        // Also keep the Android log for debugging
        android.util.Log.d("{N} Runtime Tests", str);
    }


    /**
     * Basic reporter that outputs spec results to the terminal.
     * Use this reporter in your build pipeline.
     *
     * Usage:
     *
     * jasmine.getEnv().addReporter(new jasmineReporters.TerminalReporter(options);
     *
     * @param {object} [options]
     * @param {number} [options.verbosity] meaningful values are 0 through 3; anything
     *   greater than 3 is treated as 3 (default: 2)
     * @param {boolean} [options.color] print in color or not (default: true)
     */
    var DEFAULT_VERBOSITY = 2,
        ATTRIBUTES_TO_ANSI = {
            "off": 0,
            "bold": 1,
            "red": 31,
            "green": 32
        };

    exportObject.TerminalReporter = function(options) {
        var self = this;
        self.started = false;
        self.finished = false;

        // sanitize arguments
        options = options || {};
        self.verbosity = typeof options.verbosity === "number" ? options.verbosity : DEFAULT_VERBOSITY;
        self.color = options.color;

        var indent_string = '  ',
            startTime,
            suites = [],
            currentSuite = null,
            totalSpecsExecuted = 0,
            totalSpecsSkipped = 0,
            totalSpecsFailed = 0,
            totalSpecsDefined;

        var __suites = {}, __specs = {};
        function getSuite(suite) {
            __suites[suite.id] = extend(__suites[suite.id] || {}, suite);
            return __suites[suite.id];
        }
        function getSpec(spec) {
            __specs[spec.id] = extend(__specs[spec.id] || {}, spec);
            return __specs[spec.id];
        }

        self.jasmineStarted = function(summary) {
            totalSpecsDefined = summary && summary.totalSpecsDefined || NaN;
            startTime = exportObject.startTime = new Date();
            self.started = true;
        };
        self.suiteStarted = function(suite) {
            suite = getSuite(suite);
            suite._specs = 0;
            suite._nestedSpecs = 0;
            suite._failures = 0;
            suite._nestedFailures = 0;
            suite._skipped = 0;
            suite._nestedSkipped = 0;
            suite._depth = currentSuite ? currentSuite._depth+1 : 1;
            suite._parent = currentSuite;
            currentSuite = suite;
            if (self.verbosity > 2) {
                log(indentWithLevel(suite._depth, inColor(suite.description, "bold")));
            }
        };
        self.specStarted = function(spec) {
            spec = getSpec(spec);
            spec._suite = currentSuite;
            spec._depth = currentSuite._depth+1;
            currentSuite._specs++;
            if (self.verbosity > 2) {
                log(indentWithLevel(spec._depth, spec.description + ' ...'));
            }
        };
        self.specDone = function(spec) {
            spec = getSpec(spec);
            var failed = false,
                skipped = false,
                color = 'green',
                resultText = '';
            if (isSkipped(spec)) {
                skipped = true;
                color = '';
                spec._suite._skipped++;
                totalSpecsSkipped++;
            }
            if (isFailed(spec)) {
                failed = true;
                color = 'red';
                spec._suite._failures++;
                totalSpecsFailed++;
            }
            totalSpecsExecuted++;

            if (self.verbosity === 2) {
                resultText = failed ? 'F' : skipped ? 'S' : '';
            } else if (self.verbosity > 2) {
                resultText = ' ' + (failed ? 'Failed' : skipped ? 'Skipped' : 'Passed');
            }
            
            // Only log the single character result for non-failures to reduce noise
            if (!failed) {
                log(inColor(resultText, color));
            }

            if (failed) {
                // Force a simple debug message first - this should definitely appear
                console.log('FAILURE DETECTED: Starting failure logging');
                
                // Always log detailed failure information regardless of verbosity
                log('');
                log('F'); // Show the failure marker
                log(inColor('FAILED TEST: ' + spec.fullName, 'red+bold'));
                log(inColor('Suite: ' + (spec._suite ? spec._suite.description : 'Unknown'), 'red'));
                
                // Also force output directly to console.log to ensure it's captured
                console.log('JASMINE FAILURE: ' + spec.fullName);
                console.log('JASMINE SUITE: ' + (spec._suite ? spec._suite.description : 'Unknown'));
                
                // Try to extract file information from the stack trace
                var fileInfo = 'Unknown file';
                if (spec.failedExpectations && spec.failedExpectations.length > 0 && spec.failedExpectations[0].stack) {
                    var stackLines = spec.failedExpectations[0].stack.split('\n');
                    for (var j = 0; j < stackLines.length; j++) {
                        if (stackLines[j].includes('.js:') && stackLines[j].includes('app/')) {
                            var match = stackLines[j].match(/app\/([^:]+\.js)/);
                            if (match) {
                                fileInfo = match[1];
                                break;
                            }
                        }
                    }
                }
                log(inColor('File: ' + fileInfo, 'red'));
                console.log('JASMINE FILE: ' + fileInfo);
                
                for (var i = 0, failure; i < spec.failedExpectations.length; i++) {
                    log(inColor('  Error: ' + spec.failedExpectations[i].message, color));
                    console.log('JASMINE ERROR: ' + spec.failedExpectations[i].message);
                    
                    if (spec.failedExpectations[i].stack) {
                        // Only show first few lines of stack trace to avoid clutter
                        var stackLines = spec.failedExpectations[i].stack.split('\n').slice(0, 3);
                        stackLines.forEach(function(line) {
                            if (line.trim()) {
                                log(inColor('    ' + line.trim(), 'yellow'));
                                console.log('JASMINE STACK: ' + line.trim());
                            }
                        });
                    }
                }
                log('');
            }
        };
        self.suiteDone = function(suite) {
            suite = getSuite(suite);
            // disabled suite (xdescribe) -- suiteStarted was never called
            if (suite._parent === UNDEFINED) {
                self.suiteStarted(suite);
                suite._disabled = true;
            }
            if (suite._parent) {
                suite._parent._specs += suite._specs + suite._nestedSpecs;
                suite._parent._failures += suite._failures + suite._nestedFailures;
                suite._parent._skipped += suite._skipped + suite._nestedSkipped;
            }
            currentSuite = suite._parent;
            if (self.verbosity < 3) {
                return;
            }

            var total = suite._specs + suite._nestedSpecs,
                failed = suite._failures + suite._nestedFailures,
                skipped = suite._skipped + suite._nestedSkipped,
                passed = total - failed - skipped,
                color = failed ? 'red+bold' : 'green+bold',
                str = passed + ' of ' + total + ' passed (' + skipped + ' skipped)';
            log(indentWithLevel(suite._depth, inColor(str+'.', color)));
        };
        self.jasmineDone = function() {
            var now = new Date(),
                dur = elapsed(startTime, now),
                total = totalSpecsDefined || totalSpecsExecuted,
                disabled = total - totalSpecsExecuted,
                skipped = totalSpecsSkipped,
                spec_str = total + (total === 1 ? " spec, " : " specs, "),
                fail_str = totalSpecsFailed + (totalSpecsFailed === 1 ? " failure, " : " failures, "),
                skip_str = skipped + " skipped, ",
                disabled_str = disabled + " disabled in ",
                summary_str = spec_str + fail_str + skip_str + disabled_str + dur + "s.",
                result_str = (totalSpecsFailed && "FAILURE: " || "SUCCESS: ") + summary_str,
                result_color = totalSpecsFailed && "red+bold" || "green+bold";

            if (self.verbosity === 2) {
                log('');
            }

            if (self.verbosity > 0) {
                log(inColor(result_str, result_color));
            }
            //log("Specs skipped but not reported (entire suite skipped)", totalSpecsDefined - totalSpecsExecuted);

            self.finished = true;
            // this is so phantomjs-testrunner.js can tell if we're done executing
            exportObject.endTime = now;
        };
        function indentWithLevel(level, string) {
            return new Array(level).join(indent_string) + string;
        }
        function inColor(string, color) {
            var color_attributes = color && color.split("+"),
                ansi_string = "",
                i, attr;

            if (!self.color || !color_attributes) {
                return string;
            }

            for(i = 0; i < color_attributes.length; i++) {
                ansi_string += "\033[" + ATTRIBUTES_TO_ANSI[color_attributes[i]] + "m";
            }
            ansi_string += string + "\033[" + ATTRIBUTES_TO_ANSI["off"] + "m";

            return ansi_string;
        }
    };
})(this);
