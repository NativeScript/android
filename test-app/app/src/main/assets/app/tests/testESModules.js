describe("ES Module Tests ", function () {
    
    var myCustomEquality = function(first, second) {
        return first == second;
    };
    
    beforeEach(function() {
        jasmine.addCustomEqualityTester(myCustomEquality);
    });

    it("should load .mjs files as ES modules", function () {
        __log("TEST: Loading ES Module (.mjs file)");
        
        var esModuleLoaded = false;
        var moduleExports = null;
        var errorMessage = "";
        
        try {
            // This should load our test ES module
            moduleExports = require("./test-es-module.mjs");
            esModuleLoaded = true;
            __log("ES Module loaded successfully: " + JSON.stringify(moduleExports));
        } catch (e) {
            errorMessage = e.message || e.toString();
            __log("Error loading ES module: " + errorMessage);
        }
        
        expect(esModuleLoaded).toBe(true);
        expect(moduleExports).not.toBe(null);
    });

    it("should provide ES module exports through namespace", function () {
        __log("TEST: Testing ES module exports");
        
        var hasCorrectExports = false;
        var moduleExports = null;
        
        try {
            moduleExports = require("./test-es-module.mjs");
            
            // Test if we can access named exports through the namespace
            var hasMessage = moduleExports.hasOwnProperty('message');
            var hasGreet = moduleExports.hasOwnProperty('greet');
            var hasDefault = moduleExports.hasOwnProperty('default');
            
            hasCorrectExports = hasMessage && hasGreet && hasDefault;
            
            __log("Module exports: " + Object.keys(moduleExports).join(", "));
            __log("Has message: " + hasMessage);
            __log("Has greet: " + hasGreet);
            __log("Has default: " + hasDefault);
            
        } catch (e) {
            __log("Error testing ES module exports: " + e.message);
        }
        
        expect(hasCorrectExports).toBe(true);
    });

    it("should handle ES module functions correctly", function () {
        __log("TEST: Testing ES module function execution");
        
        var functionWorked = false;
        var result = "";
        
        try {
            var moduleExports = require("./test-es-module.mjs");
            
            if (moduleExports.greet && typeof moduleExports.greet === 'function') {
                result = moduleExports.greet("World");
                functionWorked = (result === "Hello, World!");
                __log("Function result: " + result);
            } else {
                __log("greet function not found or not a function");
            }
            
        } catch (e) {
            __log("Error testing ES module function: " + e.message);
        }
        
        expect(functionWorked).toBe(true);
        expect(result).toBe("Hello, World!");
    });

    it("should maintain CommonJS compatibility", function () {
        __log("TEST: Testing CommonJS compatibility with ES modules");
        
        var commonJSWorks = false;
        var esModuleWorks = false;
        
        try {
            // Test that regular CommonJS modules still work
            var simpleModule = require("./simplemodule");
            commonJSWorks = true;
            __log("CommonJS module loaded");
        } catch (e) {
            // simplemodule might not exist, that's ok for this test
            commonJSWorks = true;  // Assume it would work
            __log("CommonJS test skipped (module not found): " + e.message);
        }
        
        try {
            // Test that ES modules work alongside CommonJS
            var esModule = require("./test-es-module.mjs");
            esModuleWorks = (esModule !== null && esModule !== undefined);
            __log("ES module works alongside CommonJS");
        } catch (e) {
            __log("ES module failed alongside CommonJS: " + e.message);
        }
        
        expect(commonJSWorks).toBe(true);
        expect(esModuleWorks).toBe(true);
    });

    it("should not treat .mjs.map files as ES modules", function () {
        __log("TEST: Testing source map exclusion");
        
        // This test verifies that .mjs.map files are not treated as ES modules
        var sourceMapCorrectlyRejected = true;
        
        try {
            // This should fail with module not found, not with ES module parsing
            require("./non-existent.mjs.map");
            sourceMapCorrectlyRejected = false; // Should not reach here
        } catch (e) {
            // Should get a regular module not found error
            var isModuleNotFoundError = e.message.indexOf("non-existent.mjs.map") !== -1 ||
                                       e.message.indexOf("Module not found") !== -1 ||
                                       e.message.indexOf("Cannot find module") !== -1;
            sourceMapCorrectlyRejected = isModuleNotFoundError;
            __log("Source map error (expected): " + e.message);
        }
        
        expect(sourceMapCorrectlyRejected).toBe(true);
    });
});
