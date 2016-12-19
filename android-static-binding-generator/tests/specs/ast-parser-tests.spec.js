'use strict';

var exec = require("child_process").exec,
    path = require("path"),
    fs = require("fs"),
    prefix = path.resolve(__dirname, "../cases/"),
    interfaceNames = path.resolve(__dirname, "../interfaces-names.txt"),
    gradleTraverse = path.resolve(__dirname, "../../project"),
    gradleTaskName = "traverseJsFilesArgs",
    parser = path.resolve(__dirname, "../../project/parser/js_parser.js");

function execGradle(inputPath, bindingsOutput, jsFilesInput, callback) {
    exec("gradle -p " + gradleTraverse + " " + gradleTaskName + " -Ptest -PjsCodeDir=\"" + inputPath + "\" -PbindingsFilePath=\"" + bindingsOutput + "\" -PinterfaceNamesFilePath=\"" + interfaceNames + "\" -PjsParserPath=\"" + parser + "\" -PjsFilesParameter=\"" + jsFilesInput + "\"", callback);
}

function logExecResult(stdout, stderr) {
    // console.log(`stdout: ${stdout}`);
    if (stderr) {
        console.log(`stderr: ${stderr}`);
    }
}

function clearOutput(bindingsOutput, jsFilesInput) {
    try {
        fs.unlinkSync(bindingsOutput);
    } catch (e) {

    }

    try {
        fs.unlinkSync(jsFilesInput);
    } catch (e) {

    }
}

describe("parser/js_parser tests", function () {

    describe("js_parser tests", function () {

        it("Analyse files only in the correct folder structure", function (done) {

            let input = path.normalize(prefix + "/mini_app/app"),
                jsFilesParameter = path.normalize(prefix + "/mini_app/jsfiles.txt"),
                output = path.normalize(prefix + "/mini_app/bindings.txt");

            clearOutput(output, jsFilesParameter);

            execGradle(input, output, jsFilesParameter, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr);

                let bindingsContent = fs.readFileSync(output, "utf-8").toString().trim();

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
            const pattern = /^(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*\*([\_A-z0-9]*)\*([\_A-z][A-z0-9]*)*(\,[\_A-z][A-z0-9]*)*\*(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*)*\*((?:[a-zA-Z]\:){0,1}(?:[\\/]*[\w.]+){1,})*\*(([a-zA-Z_$][a-zA-Z\d_$]*\.)*[a-zA-Z_$][a-zA-Z\d_$]*)*)$/mi;

            let input = path.normalize(prefix + "/extends/app"),
                jsFilesParameter = path.normalize(prefix + "/extends/jsFiles.txt"),
                output = prefix + "/extends/bindings.txt";

            clearOutput(output, jsFilesParameter);

            execGradle(input, output, jsFilesParameter, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(output, "utf-8").toString().trim().split('\n');

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

        it("Generate valid metadata for bindings from a transpiled typescript file where multiple interfaces are implemented using a decorator", function (done) {
            let input = path.normalize(prefix + "/extends_with_interfaces_ts/app"),
                jsFilesParameter = path.normalize(prefix + "/extends_with_interfaces_ts/jsFiles.txt"),
                output = prefix + "/extends_with_interfaces_ts/bindings.txt";

            clearOutput(output, jsFilesParameter);

            execGradle(input, output, jsFilesParameter, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(output, "utf-8").toString().trim().split('\n');

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


        it("Generated metadata for bindings should return proper JavaClass name despite emitted TS ES5 code",
            function (done) {
                let input = path.normalize(prefix + "/decorated_extends_ts/app"),
                    jsFilesParameter = path.normalize(prefix + "/decorated_extends_ts/jsFiles.txt"),
                    output = prefix + "/decorated_extends_ts/bindings.txt";

                const newClassName = "org.nativescript.a.MyCustomActivity";

                clearOutput(output, jsFilesParameter);

                execGradle(input, output, jsFilesParameter, function (error, stdout, stderr) {
                    if (error) {
                        console.error(`exec error: ${error}`);
                        return;
                    }

                    logExecResult(stdout, stderr)

                    let bindingsContent = fs.readFileSync(output, "utf-8").toString().trim().split('\n');

                    expect(bindingsContent.length).toBe(2);

                    for (let line of bindingsContent) {
                        var lineParts = line.split("*");
                        var tsExtendsPart = lineParts[1];
                        expect(tsExtendsPart).toBeFalsy();

                        var newClassNamePart = lineParts[3];
                        expect(newClassNamePart).toBe(newClassName);
                    }

                    done();
                });
            });

        it("Generate valid metadata for bindings where multiple interfaces are implemented using array", function (done) {
            let input = path.normalize(prefix + "/extends_with_interfaces/app"),
                jsFilesParameter = path.normalize(prefix + "/extends_with_interfaces/jsFiles.txt"),
                output = prefix + "/extends_with_interfaces/bindings.txt";

            clearOutput(output, jsFilesParameter);

            execGradle(input, output, jsFilesParameter, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(output, "utf-8").toString().trim().split('\n');

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