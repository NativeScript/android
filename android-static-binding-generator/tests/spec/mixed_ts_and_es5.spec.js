require("process").chdir("project/ast-parser");

var exec = require("child_process").exec,
	path = require("path"),
	fs = require("fs"),
	prefix = path.resolve(__dirname, "../alltests/");

describe("mixed ts with pure es5", function(){
	it("multiple classes parsed from es5 and typescript should parse without error",function(done){

		var testFolder = "mixed_multiple_classes_in_file";
			inputDir = prefix + "/"+testFolder+"/input",
			actualFile = prefix + "/"+testFolder+"/actualOutput/parsed.txt",
			expectedFile = prefix + "/"+testFolder+"/expectedOutput/parsed.txt";

		exec("node transpiled_ts_parser.js " + inputDir + " " + actualFile, function (err) {
			
			var expectedContent = fs.readFileSync(expectedFile, "utf-8");
			var actualContent = fs.readFileSync(actualFile, "utf-8");

			expect(err).toBe(null)
			expect(expectedContent).toBe(actualContent);

			done();
		})
	})
});