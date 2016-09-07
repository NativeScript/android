var exec = require("child_process").exec,
	path = require("path"),
	fs = require("fs"),
	prefix = path.resolve(__dirname, "../alltests/");

describe("pure es5", function(){
	describe("right calls", function () {
		it("when normal custom extended class with overridden methods, should be parsed without errors",function(done){

			var testFolder = "es5_normal_input_and_output_custom";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})

		it("when normal common extended class with overridden methods, should be parsed without errors",function(done){

			var testFolder = "es5_normal_input_and_output_common";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})

		it("when extend is called no overridden methods, should be able to parse with no errors", function(done){

			var testFolder = "es5_no_overridden_methods";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})

		it("when extend is called with custom class name, should be able to parse with no errors", function(done){

			var testFolder = "es5_extend_called_with_custom_class_name";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})

		it("when only one object is passed to extend, should be able to parse with no errors", function(done){

			var testFolder = "es5_called_with_only_one_parameter_object";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})

		it("when file has multiple extended classes should be parsed without errors", function(done){

			var testFolder = "es5_multiple_extends_in_a_file";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent);

				done();
			})
		})
	})


	describe("wrong calls", function () {
		it("when extend is not called an error should be thrown", function(done){

			var testFolder = "es5_extend_not_called";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {
				
				expect(stderr.indexOf("You need to call the extend")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})

		it("when extend is not called with parameters an error should be thrown", function(done){

			var testFolder = "es5_extend_not_called_with_parameters";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {

				expect(stderr.indexOf("You need to call the extend with parameters")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})

		it("when extend is called with one wrong parameter and error should be thrown", function(done){

			var testFolder = "es5_extend_not_called_with_correct_parameter";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {

				expect(stderr.indexOf("Not enough or too many arguments passed")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})

		it("when an extend name is not following the naming convention, an error should be thrown", function(done){

			var testFolder = "es5_called_with_wrong_parameter_first";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {

				expect(stderr.indexOf("The 'extend' you are trying to make has an invalid name")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})


		it("when a custom extend name is not following the naming convention, an error should be thrown", function(done){

			var testFolder = "es5_called_with_wrong_custom_parameter_first";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {

				expect(stderr.indexOf("The 'extend' you are trying to make has an invalid name")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})
	})
});