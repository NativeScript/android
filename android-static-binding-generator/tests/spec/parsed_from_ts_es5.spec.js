var exec = require("child_process").exec,
	path = require("path"),
	fs = require("fs"),
	prefix = path.resolve(__dirname, "../alltests/");

describe("ES5 generated from parsing typescript", function(){
	describe("right calls", function () {

		it("when normal typescript extend with no custom java proxy should be parsed correctly",function(done){

			var testFolder = "ts_normal_extend";
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
		
		it("when typescript generated es5 class decorator, as well as overridden methods and extended class should be traversed correctly",function(done){

			var testFolder = "ts_custom_extend";
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

		it("when class is not extended, should be parsed without errors but have no output", function(done){

			var testFolder = "ts_no_extended_class";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect("").toBe(actualContent.trim())

				done();
			})
		})

		it("when there are no overridden methods, should be parsed with no errors", function(done){

			var testFolder = "ts_no_overridden_methods";
				inputDir = prefix + "/"+testFolder+"/input",
				actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
				expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt"

			exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {

				var expectedContent = fs.readFileSync(expectedFile, "utf-8");
				var actualContent = fs.readFileSync(actualFile, "utf-8");

				expect(err).toBe(null)
				expect(expectedContent).toBe(actualContent)

				done();
			})
		})

		it("when file has multiple extended classes should be parsed without errors",function(done){

			var testFolder = "ts_multiple_extends_in_a_file";
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
		it("when java proxy name doesn't follow the pattern an error should be thrown", function(done){

			var testFolder = "ts_wrong_java_proxy_name";
				inputDir = prefix + "/"+testFolder+"/input"

			var ex = exec("node transpiled_ts_parser.js " + inputDir, function (err, stdout, stderr) {
				
				expect(stderr.indexOf("decorator is not following the right pattern which is: '[namespace.]ClassName'")).not.toBe(-1);
				expect(err).not.toBe(null)
				expect(err.code).toBe(4);

				done();
			})
		})
	})
});