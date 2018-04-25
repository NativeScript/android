const exec = require("child_process").exec,
    path = require("path"),
    fs = require("fs"),
    prefix = path.resolve(__dirname, "../cases/"),
    sbgBindingOutoutFile = path.resolve(__dirname, "../../../sbg-bindings.txt"),
    testsGradleFile = path.resolve(__dirname, "../../../static-binding-generator/runtests.gradle"),
    gradleExecutable = path.resolve(__dirname, "../../../../../gradlew");

function execGradle(inputPath, generatedJavaClassesRoot, callback) {
    const command = `${gradleExecutable} -b ${testsGradleFile} -PappRoot=${inputPath} -PgeneratedJavaClassesRoot=${generatedJavaClassesRoot}`;
    const options = {
        cwd: path.dirname(gradleExecutable)
    };
    exec(command, options , callback);
}

function logExecResult(stdout, stderr) {
    // console.log(`stdout: ${stdout}`);
    if (stderr) {
        console.log(`stderr: ${stderr}`);
    }
}

function clearOutput() {
    try {
        fs.unlinkSync(sbgBindingOutoutFile);
    } catch (e) {
    }
}

describe("parser/js_parser tests", function () {
    beforeAll(() => {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 20 * 1000; // give enough time to start the gradle daemon
    });

    describe("Traversal tests", function () {
        it("Analyse files only in the correct folder structure", function (done) {

            let input = path.normalize(path.join(prefix, "mini_app", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "mini_app", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr);

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim();

                let expectedExtends = [
                    "a.B.C.ButtonThatWillAlwaysBeExtended",
                    "a.B.C.ButtonThatIsExtendedComponent",
                    "a.B.C.ButtonThatIsExtendedComponent.Subcomponent",
                    "a.B.C.ButtonThatIsExtendedComponent1",
                    "a.B.C.ButtonThatIsExtendedComponent2",
                    "a.B.C.ButtonThatIsExtendedComponent2.Subcomponent21"
                ];
                let notExpectedExtends = [
                    "a.B.C.ButtonThatShouldNotBeExtended",
                    "a.B.C.ButtonThatShouldNotBeExtended.Submodule",
                    "a.B.C.ButtonThatShouldNotBeExtendedComponent.Subcomponent"
                ];

                for (let entry in expectedExtends) {
                    let expectedValue = expectedExtends[entry] + "*";
                    expect(bindingsContent).toContain(expectedValue);
                }

                for (let entry in notExpectedExtends) {
                    let notExpectedValue = notExpectedExtends[entry] + "*";
                    expect(bindingsContent).not.toContain(notExpectedExtends);
                }

                done();
            });
        });
    });

    describe("es5-visitors tests", function () {
        it("Generated metadata for bindings should match the pattern JavaClass*extendPrefixLoc*methods*NewClass*Location", function (done) {
            const pattern = /^(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*\*([\_A-z0-9]*)\*(\d*)\*(\d*)\*([\_A-z][A-z0-9])*\*([\_A-z][A-z0-9]*)*(\,[\_A-z][A-z0-9]*)*\*(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*)*\*((?:[a-zA-Z]\:){0,1}(?:[\/]*[\w.]+){1,})*\*(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*)*)$/mi;

            let input = path.normalize(path.join(prefix, "extends", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "extends", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr);

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                let allLines = [];

                for (let line in bindingsContent) {
                    var res = pattern.test(bindingsContent[line].trim());
                    allLines.push(res);
                }

                allLines = allLines.filter(function (val) {
                    if (val) {
                        return false;
                    } else {
                        return true;
                    }
                });

                expect(allLines.length).toBe(0);
                done();
            });
        });

        it("Files with dots in their names won't reflect in resulting java class names", function (done) {
            let input = path.normalize(path.join(prefix, "file_names_with_dots", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "file_names_with_dots", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                let expectedPartialContent = "file_with_dots";
                let foundExpected = bindingsContent[0].indexOf(expectedPartialContent)

                expect(foundExpected).toBeGreaterThan(0);
                done();
            });
        });

        it("Files and folders with dashes in their names won't reflect in resulting java class names", function (done) {
            let input = path.normalize(path.join(prefix, "directory_with_dashes", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "directory_with_dashes", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                let expectedPartialContent = "dir_with_dashes_file_with_dashes";
                let foundExpected = bindingsContent[0].indexOf(expectedPartialContent)

                expect(foundExpected).toBeGreaterThan(0);
                done();
            });
        });

        it("Generate valid metadata for bindings from a transpiled typescript file where multiple interfaces are implemented using a decorator", function (done) {
            let input = path.normalize(path.join(prefix, "extends_with_interfaces_ts", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "extends_with_interfaces_ts", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                let bindings = bindingsContent[0].split('*');
                let implInterfacesStr = bindings[bindings.length - 1];

                expect(implInterfacesStr).toBeDefined();
                expect(implInterfacesStr.length).toBeGreaterThan(0);

                let implInterfaces = implInterfacesStr.split(',');

                let expectedInterfaces = [
                    "java.util.jar.Pack200.Unpacker",
                    "java.util.Formattable",
                    "java.util.Observer",
                    "java.util.jar.Pack200.Packer"
                ];

                for (let i in expectedInterfaces) {
                    expect(implInterfaces.indexOf(expectedInterfaces[i])).toBeGreaterThan(-1);
                }

                done();
            });
        });

        it("Generated metadata for bindings should return proper JavaClass name despite emitted TS ES5 code", function (done) {
            let input = path.normalize(path.join(prefix, "decorated_extends_ts", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "decorated_extends_ts", "src", "main", "java"));

            const newClassNames = [
                "org.nativescript.a.MyCustomActivity",
                "org.nativescript.b.MyCustomActivity"
            ];

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                expect(bindingsContent.length).toBe(2);


                for (let line of bindingsContent) {
                    var lineParts = line.split("*");
                    var tsExtendsPart = lineParts[1];
                    expect(tsExtendsPart).toBeFalsy();

                    var newClassNamePart = lineParts[6];
                    expect(newClassNames).toContain(newClassNamePart);
                }

                done();
            });
        });

        it("Generate valid metadata for bindings where multiple interfaces are implemented using array", function (done) {
            let input = path.normalize(path.join(prefix, "extends_with_interfaces", "app"));
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "extends_with_interfaces", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }
                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                let bindings = bindingsContent[0].split('*');
                let implInterfacesStr = bindings[bindings.length - 1];

                expect(implInterfacesStr).toBeDefined();
                expect(implInterfacesStr.length).toBeGreaterThan(0);

                var implInterfaces = implInterfacesStr.split(',');

                var expectedInterfaces = [
                    "java.util.jar.Pack200.Unpacker",
                    "java.util.Formattable",
                    "java.util.Observer",
                    "java.util.jar.Pack200.Packer"
                ];

                for (var i in expectedInterfaces) {
                    expect(implInterfaces.indexOf(expectedInterfaces[i])).toBeGreaterThan(-1);
                }

                done();
            });
        });

    });
});