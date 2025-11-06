function runESModuleTests() {
    var passed = 0;
    var failed = 0;
    
    // Test 1: Load .mjs files as ES modules
    console.log("\n--- Test 1: Loading .mjs files as ES modules ---");
    try {
        var moduleExports = require("~/testSimpleESModule.mjs");
        if (moduleExports && moduleExports !== null) {
            console.log("Module exports:", JSON.stringify(moduleExports));
            passed++;
        } else {
            console.log("❌ FAIL: ES Module loaded but exports are null");
            failed++;
        }
        if (moduleExports.moduleType === "ES Module") {
            console.log("   - moduleType check passed");
            passed++;
        } else {
            console.log("❌ FAIL: moduleType check failed");
            failed++;
        }
    } catch (e) {
        console.log("❌ FAIL: Error loading ES module:", e.message);
        failed++;
    }
    
    // Test 2: Test import.meta functionality
    console.log("\n--- Test 2: Testing import.meta functionality ---");
    try {
        var importMetaModule = require("~/testImportMeta.mjs");
        if (importMetaModule && importMetaModule.default && typeof importMetaModule.default === 'function') {
            var metaResults = importMetaModule.default();
            console.log("import.meta test results:", JSON.stringify(metaResults, null, 2));
            
            if (metaResults && metaResults.hasImportMeta && metaResults.hasUrl && metaResults.hasDirname) {
                // console.log("   - import.meta.url:", metaResults.url);
                // console.log("   - import.meta.dirname:", metaResults.dirname);
                passed++;
            } else {
                console.log("❌ FAIL: import.meta properties missing");
                console.log("   - hasImportMeta:", metaResults?.hasImportMeta);
                console.log("   - hasUrl:", metaResults?.hasUrl);
                console.log("   - hasDirname:", metaResults?.hasDirname);
                failed++;
            }
        } else {
            console.log("❌ FAIL: import.meta module has no default export function");
            failed++;
        }
    } catch (e) {
        console.log("❌ FAIL: Error testing import.meta:", e.message);
        console.log("Stack trace:", e.stack);
        failed++;
    }
    
    // Test 3: Test Worker enhancements
    console.log("\n--- Test 3: Testing Worker enhancements ---");
    try {
        var workerModule = require("~/testWorkerFeatures.mjs");
        if (workerModule && workerModule.testWorkerFeatures && typeof workerModule.testWorkerFeatures === 'function') {
            var workerResults = workerModule.testWorkerFeatures();
            console.log("Worker features test results:", JSON.stringify(workerResults, null, 2));
            
            if (workerResults && workerResults.stringPathSupported && workerResults.urlObjectSupported && workerResults.tildePathSupported) {
                console.log("   - String path support:", workerResults.stringPathSupported);
                console.log("   - URL object support:", workerResults.urlObjectSupported);
                console.log("   - Tilde path support:", workerResults.tildePathSupported);
                passed++;
            } else {
                console.log("❌ FAIL: Worker enhancement features missing");
                console.log("   - stringPathSupported:", workerResults?.stringPathSupported);
                console.log("   - urlObjectSupported:", workerResults?.urlObjectSupported);
                console.log("   - tildePathSupported:", workerResults?.tildePathSupported);
                failed++;
            }
        } else {
            console.log("❌ FAIL: Worker features module has no testWorkerFeatures function");
            failed++;
        }
    } catch (e) {
        console.log("❌ FAIL: Error testing Worker features:", e.message);
        console.log("Stack trace:", e.stack);
        failed++;
    }
    
    // Final results
    console.log("\n=== ES MODULE TEST RESULTS ===");
    console.log("Tests passed:", passed);
    console.log("Tests failed:", failed);
    console.log("Total tests:", passed + failed);
    
    if (failed === 0) {
        console.log("ALL ES MODULE TESTS PASSED!");
    } else {
        console.log("SOME ES MODULE TESTS FAILED!");
    }
    
    return { passed: passed, failed: failed };
}

// Run the tests immediately
runESModuleTests();
