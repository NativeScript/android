const exec = require("child_process").exec,
    path = require("path"),
    fs = require("fs"),
    prefix = path.resolve(__dirname, "../cases/"),
    sbgBindingOutoutFile = path.resolve(__dirname, "../../../sbg-bindings.txt"),
    sbgProjectDir = path.resolve(__dirname, "../../../static-binding-generator"),
    gradleExecutable = path.resolve(__dirname, "../../../../../gradlew");

function execGradle(inputPath, generatedJavaClassesRoot, callback) {
    const command = `${gradleExecutable} -p ${sbgProjectDir} -PappRoot=${inputPath} -PgeneratedJavaClassesRoot=${generatedJavaClassesRoot}`;
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
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 100 * 1000; // give enough time to start the gradle daemon
    });

    describe("Traversal tests", function () {
        it("Analyse files only in the correct folder structure", function (done) {

            let input = path.normalize(path.join(prefix, "mini_app", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "mini_app", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return done.fail(error);
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
                    return done.fail(error);
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
                    return done.fail(error);
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
                    return done.fail(error);
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
                    return done.fail(error);
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

        it("Generate bindings for interfaces listed through local aliases, and none for classes it cannot fully describe", function (done) {
            let input = path.normalize(path.join(prefix, "extends_with_interface_aliases", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "extends_with_interface_aliases", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return done.fail(error);
                }

                logExecResult(stdout, stderr);

                let lines = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');
                // <base>*<file>*<line>*<column>*<class name>*<methods>*<java name>*<path>*<interfaces>
                let interfacesOf = function (predicate) {
                    let line = lines.find(l => predicate(l.split('*')));
                    return line === undefined ? undefined : line.split('*')[8].split(',');
                };

                expect(interfacesOf(parts => parts[4] === "AliasedObject")).toEqual(["java.util.Observer", "java.util.Formattable"]);
                expect(interfacesOf(parts => parts[5].split(',').indexOf("run") > -1)).toEqual(["java.lang.Runnable"]);
                expect(interfacesOf(parts => parts[4] === "ImportedInterfaceObject")).toBeUndefined();
                expect(stdout + stderr).toContain("cannot determine the Java interface(s) _imported__WEBPACK_IMPORTED_MODULE_0__.Observer of class ImportedInterfaceObject");
                expect(lines.filter(l => l.indexOf("org.nativescript.aliases.") > -1)).toEqual([]);

                // Classes with members the parser cannot name are left to the runtime.
                let methodsOf = l => (l.split('*')[5] || "").split(',');
                expect(lines.filter(l => methodsOf(l).indexOf("toString") > -1)).toEqual([]);
                expect(lines.filter(l => methodsOf(l).indexOf("finalize") > -1)).toEqual([]);
                expect(interfacesOf(parts => parts[4] === "ComputedMember")).toBeUndefined();

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
                    return done.fail(error);
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

        it("Generated metadata for a transpiled typescript class without a constructor should contain the super call location", function (done) {
            let input = path.normalize(path.join(prefix, "extends_ts_no_constructor", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "extends_ts_no_constructor", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return done.fail(error);
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                // JavaClass*file*line*column*NewClass*...: the runtime names the class from
                // the location of the `_super.apply` call (the column of `apply`, 1-based).
                let locations = bindingsContent.map(line => line.split("*").slice(1, 5).join("*")).sort();
                expect(locations).toEqual([
                    "no_constructor*11*87*Minified",
                    "no_constructor*4*42*NoConstructor"
                ]);

                done();
            });
        });

        it("Generated metadata for bindings should return proper JavaClass name when there is a static property and double assignment", function (done) {
            let input = path.normalize(path.join(prefix, "decorated_double_assignment_ts", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "decorated_double_assignment_ts", "src", "main", "java"));

            const newClassNames = [
                "org.nativescript.MyCustomActivity",
            ];

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return done.fail(error);
                }

                logExecResult(stdout, stderr)

                let bindingsContent = fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n');

                expect(bindingsContent.length).toBe(1);


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

        it("Generated metadata for bindings lists the own data properties the runtime sees", function (done) {
            let input = path.normalize(path.join(prefix, "content_members", "app")),
                generatedJavaClassesRoot = path.normalize(path.join(prefix, "content_members", "src", "main", "java"));

            clearOutput();

            execGradle(input, generatedJavaClassesRoot, function (error, stdout, stderr) {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return done.fail(error);
                }
                logExecResult(stdout, stderr)

                // JavaClass*file*line*column*NewClass*methods*JavaProxyName*jsFile*interfaces
                let rows = {};
                for (let line of fs.readFileSync(sbgBindingOutoutFile, "utf-8").toString().trim().split('\n')) {
                    let parts = line.trim().split("*");
                    rows[parts[4]] = parts;
                }

                expect(rows["LiteralMembers"][5]).toBe("identifierMember,quotedMember,shorthandMember,dataMember,~getterMember,~setterMember,run");
                expect(rows["LiteralMembers"][8]).toBe("java.lang.Runnable");
                // The runtime finds this class by call site, so a list missing members would be wrong.
                expect(rows["UnnamedMembers"]).toBeUndefined();

                // Accessors are marked with `~`; a redefined name keeps its first
                // position and the kind of its last definition.
                expect(rows["AccessorMembers"][5]).toBe("~getOnly,~setOnly,~pair,~quotedAccessor,accessorThenData,~dataThenAccessor,~toString");
                expect(rows["AccessorInterface"][0]).toBe("java.lang.Runnable");
                expect(rows["AccessorInterface"][5]).toBe("~run");

                expect(rows["PrototypeMembers"][5]).toBe("toString,quotedPrototypeMember,chainedA,chainedB,sequenceA,sequenceB");
                expect(rows["PrototypeMembers"][8]).toBe("java.lang.Runnable");

                let source = fs.readFileSync(path.join(input, "members.js"), "utf-8").split("\n");
                let applyLine = source.findIndex(line => line.indexOf("_super.apply(this, arguments) || this;") > 0);
                expect(rows["ApplyInDeclaration"][5]).toBe("toString");
                expect(rows["ApplyInDeclaration"][2]).toBe(String(applyLine + 1));
                expect(rows["ApplyInDeclaration"][3]).toBe(String(source[applyLine].indexOf("apply(") + 1));

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
                    return done.fail(error);
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
