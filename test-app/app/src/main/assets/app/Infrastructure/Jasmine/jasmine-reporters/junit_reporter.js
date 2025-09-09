/* global java, __phantom_writeFile */
(function(global) {
    var UNDEFINED,
        exportObject;

    if (typeof module !== "undefined" && module.exports) {
        exportObject = exports;
    } else {
        exportObject = global.jasmineReporters = global.jasmineReporters || {};
    }

    function trim(str) { return str.replace(/^\s+/, "" ).replace(/\s+$/, "" ); }
    function elapsed(start, end) { return (end - start)/1000; }
    function isFailed(obj) { return obj.status === "failed"; }
    function isSkipped(obj) { return obj.status === "pending"; }
    function pad(n) { return n < 10 ? '0'+n : n; }
    function extend(dupe, obj) { // performs a shallow copy of all props of `obj` onto `dupe`
        for (var prop in obj) {
            if (obj.hasOwnProperty(prop)) {
                dupe[prop] = obj[prop];
            }
        }
        return dupe;
    }
    function ISODateString(d) {
        return d.getFullYear() + '-' +
            pad(d.getMonth()+1) + '-' +
            pad(d.getDate()) + 'T' +
            pad(d.getHours()) + ':' +
            pad(d.getMinutes()) + ':' +
            pad(d.getSeconds());
    }
    function escapeInvalidXmlChars(str) {
        return str.replace(/</g, "&lt;")
            .replace(/\>/g, "&gt;")
            .replace(/\"/g, "&quot;")
            .replace(/\'/g, "&apos;")
            .replace(/\&/g, "&amp;");
    }
    function getQualifiedFilename(path, filename, separator) {
        if (path && path.substr(-1) !== separator && filename.substr(0) !== separator) {
            path += separator;
        }
        return path + filename;
    }
    function log(str) {
    	__log(str);
        android.util.Log.d("{N} Runtime Tests", str);
//        var con = global.console || console;
//        if (con && con.log) {
//            con.log(str);
//        }
    }


    /**
     * Generates JUnit XML for the given spec run. There are various options
     * to control where the results are written, and the default values are
     * set to create as few .xml files as possible. It is possible to save a
     * single XML file, or an XML file for each top-level `describe`, or an
     * XML file for each `describe` regardless of nesting.
     *
     * Usage:
     *
     * jasmine.getEnv().addReporter(new jasmineReporters.JUnitXmlReporter(options);
     *
     * @param {object} [options]
     * @param {string} [savePath] directory to save the files (default: '')
     * @param {boolean} [consolidateAll] whether to save all test results in a
     *   single file (default: true)
     *   NOTE: if true, {filePrefix} is treated as the full filename (excluding
     *     extension)
     * @param {boolean} [consolidate] whether to save nested describes within the
     *   same file as their parent (default: true)
     *   NOTE: true does nothing if consolidateAll is also true.
     *   NOTE: false also sets consolidateAll to false.
     * @param {boolean} [useDotNotation] whether to separate suite names with
     *   dots instead of spaces, ie "Class.init" not "Class init" (default: true)
     * @param {string} [filePrefix] is the string value that is prepended to the
     *   xml output file (default: junitresults-)
     *   NOTE: if consolidateAll is true, the default is simply "junitresults" and
     *     this becomes the actual filename, ie "junitresults.xml"
     */
    exportObject.JUnitXmlReporter = function(options) {
        var self = this;
        self.started = false;
        self.finished = false;
        // sanitize arguments
        options = options || {};
        self.savePath = options.savePath || '';
        self.consolidate = options.consolidate === UNDEFINED ? true : options.consolidate;
        self.consolidateAll = self.consolidate !== false && (options.consolidateAll === UNDEFINED ? true : options.consolidateAll);
        self.useDotNotation = options.useDotNotation === UNDEFINED ? true : options.useDotNotation;
        self.filePrefix = options.filePrefix || (self.consolidateAll ? 'junitresults' : 'junitresults-');

        var suites = [],
            currentSuite = null,
            totalSpecsExecuted = 0,
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
            exportObject.startTime = new Date();
            self.started = true;
        };
        self.suiteStarted = function(suite) {
            suite = getSuite(suite);
            suite._startTime = new Date();
            suite._specs = [];
            suite._suites = [];
            suite._failures = 0;
            suite._skipped = 0;
            suite._parent = currentSuite;
            if (!currentSuite) {
                suites.push(suite);
            } else {
                currentSuite._suites.push(suite);
            }
            currentSuite = suite;
        };
        self.specStarted = function(spec) {
            spec = getSpec(spec);
            spec._startTime = new Date();
            spec._suite = currentSuite;
            currentSuite._specs.push(spec);
        };
        self.specDone = function(spec) {
            spec = getSpec(spec);
            spec._endTime = new Date();
            if (isSkipped(spec)) { spec._suite._skipped++; }
            if (isFailed(spec)) { spec._suite._failures++; }
            totalSpecsExecuted++;
        };
        self.suiteDone = function(suite) {
            suite = getSuite(suite);
            // disabled suite (xdescribe) -- suiteStarted was never called
            if (suite._parent === UNDEFINED) {
                self.suiteStarted(suite);
                suite._disabled = true;
            }
            suite._endTime = new Date();
            currentSuite = suite._parent;
        };
        self.jasmineDone = function() {
            var output = '';
            for (var i = 0; i < suites.length; i++) {
                output += self.getOrWriteNestedOutput(suites[i]);
            }
            // if we have anything to write here, write out the consolidated file
            if (output) {
                wrapOutputAndWriteFile(self.filePrefix, output);
            }
            //log("Specs skipped but not reported (entire suite skipped)", totalSpecsDefined - totalSpecsExecuted);

            self.finished = true;
            // this is so phantomjs-testrunner.js can tell if we're done executing
            exportObject.endTime = new Date();
        };

        self.getOrWriteNestedOutput = function(suite) {
            var output = suiteAsXml(suite);
            
            for (var i = 0; i < suite._suites.length; i++) {
                output += self.getOrWriteNestedOutput(suite._suites[i]);
            }
            if (self.consolidateAll || self.consolidate && suite._parent) {
                return output;
            } else {
                // if we aren't supposed to consolidate output, just write it now
                wrapOutputAndWriteFile(generateFilename(suite), output);
                return '';
            }
        };

        self.writeFile = function(filename, text) {
            var errors = [];
            var path = self.savePath;

            function phantomWrite(path, filename, text) {
                // turn filename into a qualified path
                filename = getQualifiedFilename(path, filename, window.fs_path_separator);
                // write via a method injected by phantomjs-testrunner.js
                __phantom_writeFile(filename, text);
            }

            function nodeWrite(path, filename, text) {
                var fs = require("fs");
                var nodejs_path = require("path");
                require("mkdirp").sync(path); // make sure the path exists
                var filepath = nodejs_path.join(path, filename);
                var xmlfile = fs.openSync(filepath, "w");
                fs.writeSync(xmlfile, text, 0);
                fs.closeSync(xmlfile);
                return;
            }

            // Attempt writing with each possible environment.
            // Track errors in case no write succeeds
            try {
                phantomWrite(path, filename, text);
                return;
            } catch (e) { errors.push('  PhantomJs attempt: ' + e.message); }
            try {
                nodeWrite(path, filename, text);
                return;
            } catch (f) { errors.push('  NodeJS attempt: ' + f.message); }
            try {
                // Instead of writing XML files, output test summary to console
                // Parse the XML text to extract test summary
                var testMatch = text.match(/tests="(\d+)"/g);
                var failureMatch = text.match(/failures="(\d+)"/g);
                var errorMatch = text.match(/errors="(\d+)"/g);
                var skippedMatch = text.match(/skipped="(\d+)"/g);
                
                var totalTests = 0;
                var totalFailures = 0;
                var totalErrors = 0;
                var totalSkipped = 0;
                
                // Sum up all test suite results
                if (testMatch) {
                    for (var i = 0; i < testMatch.length; i++) {
                        var match = testMatch[i].match(/tests="(\d+)"/);
                        if (match) totalTests += parseInt(match[1]);
                    }
                }
                
                if (failureMatch) {
                    for (var i = 0; i < failureMatch.length; i++) {
                        var match = failureMatch[i].match(/failures="(\d+)"/);
                        if (match) totalFailures += parseInt(match[1]);
                    }
                }
                
                if (errorMatch) {
                    for (var i = 0; i < errorMatch.length; i++) {
                        var match = errorMatch[i].match(/errors="(\d+)"/);
                        if (match) totalErrors += parseInt(match[1]);
                    }
                }
                
                if (skippedMatch) {
                    for (var i = 0; i < skippedMatch.length; i++) {
                        var match = skippedMatch[i].match(/skipped="(\d+)"/);
                        if (match) totalSkipped += parseInt(match[1]);
                    }
                }
                
                // Output in a format our test checker can detect
                var resultPrefix = (totalFailures > 0 || totalErrors > 0) ? "FAILURE:" : "SUCCESS:";
                console.log(resultPrefix + " " + totalTests + " specs, " + (totalFailures + totalErrors) + " failures, " + totalSkipped + " skipped");
                
                return;
            } catch (f) { 
                errors.push('  tns-android console output attempt: ' + f.message);
            }

            // If made it here, no write succeeded.  Let user know.
            log("Warning: writing junit report failed for '" + path + "', '" +
                filename + "'. Reasons:\n" +
                errors.join("\n")
            );
        };

        /******** Helper functions with closure access for simplicity ********/
        function generateFilename(suite) {
            return self.filePrefix + getFullyQualifiedSuiteName(suite, true) + '.xml';
        }

        function getFullyQualifiedSuiteName(suite, isFilename) {
            var fullName;
            if (self.useDotNotation || isFilename) {
                fullName = suite.description;
                for (var parent = suite._parent; parent; parent = parent._parent) {
                    fullName = parent.description + '.' + fullName;
                }
            } else {
                fullName = suite.fullName;
            }

            // Either remove or escape invalid XML characters
            if (isFilename) {
                var fileName = "",
                    rFileChars = /[\w\.]/,
                    chr;
                while (fullName.length) {
                    chr = fullName[0];
                    fullName = fullName.substr(1);
                    if (rFileChars.test(chr)) {
                        fileName += chr;
                    }
                }
                return fileName;
            } else {
            	var finalName = escapeInvalidXmlChars(fullName);
                return finalName;
            }
        }

        function suiteAsXml(suite) {
        	
        	var fullyQualifiedSuiteName = getFullyQualifiedSuiteName(suite);
            var xml = '\n <testsuite name="' + getFullyQualifiedSuiteName(suite) + '"';
            xml += ' timestamp="' + ISODateString(suite._startTime) + '"';
            xml += ' hostname="localhost"'; // many CI systems like Jenkins don't care about this, but junit spec says it is required
            xml += ' time="' + elapsed(suite._startTime, suite._endTime) + '"';
            xml += ' errors="0"';
            xml += ' tests="' + suite._specs.length + '"';
            xml += ' skipped="' + suite._skipped + '"';
            // Because of JUnit's flat structure, only include directly failed tests (not failures for nested suites)
            xml += ' failures="' + suite._failures + '"';
            xml += '>';

            for (var i = 0; i < suite._specs.length; i++) {
                xml += specAsXml(suite._specs[i]);
            }
            
            xml += '\n </testsuite>';
            return xml;
        }
        function specAsXml(spec) {
            var xml = '\n  <testcase classname="' + getFullyQualifiedSuiteName(spec._suite) + '"';
            xml += ' name="' + escapeInvalidXmlChars(spec.description) + '"';
            xml += ' time="' + elapsed(spec._startTime, spec._endTime) + '"';
            xml += '>';

            if (isSkipped(spec)) {
                xml += '<skipped />';
            } else if (isFailed(spec)) {
                for (var i = 0, failure; i < spec.failedExpectations.length; i++) {
                    failure = spec.failedExpectations[i];
                    xml += '\n   <failure type="' + (failure.matcherName || "exception") + '"';
                    xml += ' message="' + trim(escapeInvalidXmlChars(failure.message))+ '"';
                    xml += '>';
                    xml += '<![CDATA[' + trim(failure.stack || failure.message) + ']]>';
                    xml += '\n   </failure>';
                }
            }
            xml += '\n  </testcase>';
            return xml;
        }

        // To remove complexity and be more DRY about the silly preamble and <testsuites> element
        var prefix = '<?xml version="1.0" encoding="UTF-8" ?>';
        prefix += '\n<testsuites>';
        var suffix = '\n</testsuites>';
        function wrapOutputAndWriteFile(filename, text) {
            if (filename.substr(-4) !== '.xml') { filename += '.xml'; }
            self.writeFile(filename, (prefix + text + suffix));
        }
    };
})(this);
