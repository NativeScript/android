const fs = require("fs"),
  xml2js = require("xml2js"),
  parser = new xml2js.Parser();

// TODO: check why gradle doesn't show error logs
console.error = console.log;

if (process.argv.length < 3) {
  console.error(`Usage: node ${process.argv[1]} <unit_test_results_file>`);
  process.exit(1);
}

const testResultsFile = process.argv[2];
if (!fs.existsSync(testResultsFile)) {
  console.error(`The specified file ${testResultsFile} does not exist`);
  process.exit(1);
}

console.log(`\nStart processing ${testResultsFile}\n`);

fs.readFile(testResultsFile, function (err, data) {
  if (err) {
    console.error(`An error occurred while reading ${testResultsFile}: ${err}`);
    process.exit(1);
  }

  parser.parseString(data, function (err, result) {
    if (err) {
      console.error(`Unable to parse ${testResultsFile}: ${err}`);
      process.exit(1);
    }

    if (!result.testsuites) {
      console.error(`Invalid XML: expected <testsuites> element.`);
      process.exit(1);
    }

    const testSuites = result.testsuites.testsuite || [];
    const failedTests = [];
    let totalTests = 0;
    let passedTests = 0;
    let failedTestCount = 0;

    console.log(`Found ${testSuites.length} test suite(s)\n`);

    for (var i = 0; i < testSuites.length; i++) {
      const testSuite = testSuites[i];
      const testCases = testSuite.testcase || [];
      const suiteName = testSuite.$.name || `Suite ${i + 1}`;

      console.log(`Processing test suite: ${suiteName}`);
      console.log(`  Tests in suite: ${testCases.length}`);

      for (var j = 0; j < testCases.length; j++) {
        const testCase = testCases[j];
        const testName = testCase.$.name;
        totalTests++;

        if (testCase.failure) {
          failedTestCount++;
          console.log(`  ✗ FAILED: ${testName}`);
          const failures = testCase.failure || [];
          for (var k = 0; k < failures.length; k++) {
            const failure = failures[k];
            failedTests.push({
              suite: suiteName,
              test: testName,
              message: failure.$ ? failure.$.message : "",
              details: failure._,
            });
          }
          if (failures.length === 0) {
            failedTests.push({
              suite: suiteName,
              test: testName,
              message: "Unknown failure",
              details: "No failure details provided in the test results.",
            });
          }
        } else {
          passedTests++;
          console.log(`  ✓ PASSED: ${testName}`);
        }
      }
      console.log("");
    }

    console.log(`\n${"=".repeat(60)}`);
    console.log(`Test Results Summary:`);
    console.log(`  Total Tests: ${totalTests}`);
    console.log(`  Passed: ${passedTests}`);
    console.log(`  Failed: ${failedTestCount}`);
    console.log(`${"=".repeat(60)}\n`);

    if (failedTests.length > 0) {
      console.error(`\n${"!".repeat(60)}`);
      console.error(`FAILED TEST DETAILS (${failedTests.length} failure(s)):`);
      console.error(`${"!".repeat(60)}\n`);

      // Group failures by suite
      const failuresBySuite = {};
      for (var i = 0; i < failedTests.length; i++) {
        const failure = failedTests[i];
        if (!failuresBySuite[failure.suite]) {
          failuresBySuite[failure.suite] = [];
        }
        failuresBySuite[failure.suite].push(failure);
      }

      // Display grouped failures
      let failureCount = 1;
      for (const suite in failuresBySuite) {
        console.error(`Test Suite: ${suite}`);
        const suiteFailures = failuresBySuite[suite];
        for (var i = 0; i < suiteFailures.length; i++) {
          const failure = suiteFailures[i];
          console.error(`  [${failureCount}] Test Name: ${failure.test}`);
          if (failure.message) {
            console.error(`      Error Type: ${failure.message}`);
          }
          console.error(`      Details:`);
          const details = failure.details.split("\n");
          for (var j = 0; j < details.length; j++) {
            if (details[j].trim()) {
              console.error(`        ${details[j]}`);
            }
          }
          console.error("");
          failureCount++;
        }
      }
      console.error(`${"!".repeat(60)}\n`);
      process.exit(1);
    } else {
      console.log("All tests passed! ✓\n");
    }
  });
});
