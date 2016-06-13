describe("TNS require", function () {

    beforeEach(TNSClearOutput);
    afterEach(TNSClearOutput);

    it("path with leading slash is treated as absolute from the root of the device", function () {
        require("./AbsolutePath");
        expect(TNSGetOutput()).toBe("main started and AbsolutePath/dependency started and main executed");
    });

    it("can change require object", function () {
        require("./ChangingRequireObject");
        expect(TNSGetOutput()).toBe("main started message and main executed");
    });

    it("can circularly require module index", function () {
        require("./CircularIndexRequire");
        var expected = 'main started and dependency1 started and dependency2 started'
            + ' visible undefined visible undefined and dependency2 ended'
            + ' and dependency1 ended and main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it('can circularly require modules', function () {
        require("./CircularRequire");
        var expected = 'main started and dependency1 started'
            + ' and dependency2 started and dependency3 started'
            + ' and dependency3 ended and dependency2 ended'
            + ' and dependency1 ended and main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it('can circularly require and export', function () {
        require("./CircularRequireWithExports");
        var expected = 'main started and dependency1 started and dependency2 started'
            + ' and dependency3 started visible from dependency3 undefined'
            + ' and dependency3 ended and dependency2 ended and dependency1 ended'
            + ' and main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it('can bubble exports from one module to another in non-circular require', function () {
        require("./ExportsBubbling");
        expect(TNSGetOutput()).toBe('main started level5 exports and main ended');
    });

    it('module has id with the name of the file', function () {
        expect(module.id).toBe(__filename);
        expect(module.id).toMatch(/\/.*\/app\/shared\/Require\/index.js$/);
    });

    it('modules can require further modules', function () {
        require("./FiveLevelsOfRequire");
        var expected = 'main started level5 level4 level3 level2 level1 and main executed';
        expect(TNSGetOutput()).toBe(expected);
    });

    if (!global.NSObject) {
        it('deletes module cache on error', function () {
            require("./ModuleErrorCache");
            expect(TNSGetOutput()).toBe('did throw1no throw');
        });
    }

    it('can export var-s', function () {
        require("./ModuleVariable");
        expect(TNSGetOutput()).toBe('main started just title and main ended');
    });

    it('can require a module multiple times, sharing the required module instance', function () {
        require("./MultipleRequireOfAFile");
        expect(TNSGetOutput()).toBe('main started module main middle main end');
    });

    it('can set variables in the global object', function () {
        require("./NestedGlobalObject");
        var expected = 'main started(app -> dependency1)(dependency1 -> dependency2)'
            + '(dependency2 -> dependency3)(dependency3 -> dependency4)'
            + '(dependency4 -> dependency5)(dependency5 -> end)(end -> dependency5)'
            + '(dependency5 -> dependency4)(dependency4 -> dependency3)'
            + '(dependency3 -> dependency2)(dependency2 -> dependency1)'
            + '(dependency1 -> app) and main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it('throws for missing modules', function () {
        require("./NotExistingFileRequire");
        expect(TNSGetOutput()).toBe('main started Error main ended');
    });

    it("would load package.json if available, and use its 'main' property for js file name", function () {
        require("./PackageJsonApp");
        expect(TNSGetOutput()).toBe('73DB08A5-8FB2-4648-BD6C-9ECB844A4D8E');
    });

    it("would load package.json if available, and use its 'main' property for js file name without extension", function () {
        require("./PackageJsonAppWithoutExtension");
        expect(TNSGetOutput()).toBe('813de57a-232c-49cc-b00f-a70e0f566c52');
    });

    it("would load package.json if available, but if there is no 'main' property, would fallback to index.js", function () {
        require("./PackageJsonAppNoMain");
        expect(TNSGetOutput()).toBe("4262F53F-2320-419E-8B3C-626FFB88EC92");
    });

    it('would throw if a package.json main file can not be located', function () {
        require("./PackageJsonSyntaxError");
        expect(TNSGetOutput()).toBe("ModuleError");
    });

    it("require extensions", function () {
        require("./RequireExtensions");
        expect(TNSGetOutput()).toBe("dependency1 loaded");
    });

    it("package.json main property in tns_modules points to a js file in tns_modules", function () {
        require("./PackageJsonTns");
        expect(TNSGetOutput()).toBe("CD11903C-B38B-4D68-BEB6-D72C3FAD906F");
    });

    it('can require from tns_modules', function () {
        require("./RequireModuleFolder");
        expect(TNSGetOutput()).toBe('main started from module folder main ended');
    });

    it('searches tns_modules before app', function () {
        require("./RequireModuleFolderConflict");
        expect(TNSGetOutput()).toBe('main started from module folder main ended');
    });

    it("when require a tns_module that is a directory name it should load the index js inside it", function () {
        require("shared/Require/RequirePriority/dependency3");
        var expected = ' from module folder in index file';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("when require a tns_module that is a directory name it should load the package json inside it", function () {
        require("shared/Require/RequirePriority/dependency5");
        var expected = ' from module folder in dependency5 dir'
        expect(TNSGetOutput()).toBe(expected);
    });

    it('has a priority to load from tns_module first then from app and relative folder', function () {
        require("./RequirePriority");
        var expected = 'main started from module folder from module folder in'
            + ' index file from root folder in index file from root folder'
            + ' from root folder in index file main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("can set properties in the global object", function () {
        require("./SimpleGlobalObject");
        var expected = 'main started appVisible1 appVisible1 appVisible2'
            + ' undefined depVisible1 depVisible1 depVisible2 undefined'
            + ' changed and main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("can simply require", function () {
        require("./SimpleRequire");
        expect(TNSGetOutput()).toBe("main started and dependency executed and main executed");
    });

    it("can require modules and modules can export", function () {
        require("./SimpleRequireWithExports");
        var expected = 'main started and dependency started and dependency executed'
            + ' public string 42 public function and main executed';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("required module can require a second module", function () {
        require("./TwoLevelsOfRequire");
        var expected = 'main started and dependency1 started and dependency2 started and dependency2'
            + ' executed and dependency1 executed and main executed';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("can require a module that can require second module and both can export", function () {
        require("./TwoLevelsOfRequireWithExports");
        var expected = 'main started and dependency1 started and dependency2 started'
            + ' and dependency2 executed dependency2 string 2 dependency2'
            + ' function and dependency1 executed dependency1 string 1'
            + ' dependency1 function and main executed';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("can can catch a syntax error in module", function () {
        require("./SyntaxErrorInModule");
        var expected = 'main started SyntaxError main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("can can catch a runtime error in module", function () {
        require("./RuntimeErrorInModule");
        var expected = 'main started ReferenceError main ended';
        expect(TNSGetOutput()).toBe(expected);
    });

    it("require relative to home", function () {
       var fileName = __filename.substring(__approot.length + "/app".length);
       expect(require("~" + fileName)).toBe(global.require(__filename));
    });

    it("require file when there is directory with the same name", function () {
        var result = require("./FileAndDirectoryWithSameName");
        var expected = 'file module.js title';

        expect(result.title).toBe(expected);
    });

    it("file ending with commented line", function () {
       expect(function () { require("./FileEndingWithCommentedLine"); }).not.toThrow();
    });

    it("should require JSON files", function () {
        require('./RequireJsonFile');
        expect(TNSGetOutput()).toBe('test-value');
    });

    it("should handle JSON file errors", function () {
        require('./RequireJsonCorruptFile');
        expect(TNSGetOutput()).toMatch(/JSON Parse error: Unterminated string$|Unexpected token/)
    });

    it("shouldn't load invalid JSON file", function () {
        require("./RequireJsonCorruptFile1");
        expect(TNSGetOutput()).toMatch(/JSON Parse error: Unable to parse JSON string$|Unexpected token/)
    });

    it("when using global in a module global should be defined", function () {
        var module = require("./GlobalIsDefined/module");
        module.accessGlobalObject();
        expect(TNSGetOutput()).not.toBe(undefined);
    });

    it("when require a module via app root syntax it should be loaded", function () {
        var mymodule = require("~/shared/Require/RequireWithTildeSyntax/module.js");
        var value = mymodule.echo(12345)
        expect(value).toBe(12345);
    });

    it("should load module with null char in it", function () {
        var text = require("./ModuleWithNullChar/index").text;
        var s = "Hello world";
        expect(text.length).toBe(s.length);
    });

    it("should have module.require exported function", function () {
        expect(typeof module.require).toBe("function");
    });

    it("should load module through module.require exported function", function () {
        var module1 = require("./ModuleRequireFunction/module1");
        expect(module1.msg).toBe("module1");

        var module2 = module1.module.require("./module2");
        expect(module2.msg).toBe("module2");
    });

    it("should load module through global.require", function () {
        expect(typeof global.require).toBe("function");

        var module3 = global.require("./shared/Require/GlobalRequire/index");
        expect(module3.msg).toBe("module3");
    });

    it('Case Sensitive', function () {
        require("./CaseSensitive");
        // WARNING: The following test is platform specific
        var expected = global.android ? 'filefolder' : 'file';
        expect(TNSGetOutput()).toBe(expected);
    });

    it('File with dots', function () {
        require("./FileWithDots");
        expect(TNSGetOutput()).toBe('file.name');
    });
});
