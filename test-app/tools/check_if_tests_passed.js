const
	fs = require("fs"),
	xml2js = require("xml2js"),
	parser = new xml2js.Parser();

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

fs.readFile(testResultsFile, function(err, data) {
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
		for (var i = 0; i < testSuites.length; i++) {
			const testSuite = testSuites[i];
			const testCases = testSuite.testcase || [];
			for (var j = 0; j < testCases.length; j++) {
				const testCase = testCases[j];
				if (testCase.failure) {
					const failures = testCase.failure || [];
					for (var k = 0; k < failures.length; k++) {
						const failure = failures[k];
						failedTests.push(`${testCase.$.name}\n${failure._}`);
					}
				}
			}
		}

		if (failedTests.length > 0) {
			console.error(`\nFailed test cases:\n${failedTests.join("\n")}\n`);
			process.exit(1);
		}
	});
});