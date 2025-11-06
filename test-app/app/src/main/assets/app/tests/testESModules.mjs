async function runESModuleTests() {
  let passed = 0;
  let failed = 0;
  const failureDetails = [];

  const recordPass = (message, ...args) => {
    console.log(`✅ PASS: ${message}`, ...args);
    passed++;
  };

  const recordFailure = (message, options = {}) => {
    const { error, details = [] } = options;
    const fullMessage = error?.message
      ? `${message}: ${error.message}`
      : message;
    console.log(`❌ FAIL: ${fullMessage}`);
    details.forEach((detail) => console.log(detail));
    if (error?.stack) {
      console.log("Stack trace:", error.stack);
    }
    failed++;
    failureDetails.push(fullMessage);
  };

  const logFinalSummary = () => {
    console.log("\n=== ES MODULE TEST RESULTS ===");
    console.log("Tests passed:", passed);
    console.log("Tests failed:", failed);
    console.log("Total tests:", passed + failed);

    if (failed === 0) {
      console.log("ALL ES MODULE TESTS PASSED!");
    } else {
      console.log("SOME ES MODULE TESTS FAILED!");
      console.log("FAILURE DETECTED: Starting failure logging");
      failureDetails.forEach((detail) => {
        console.log(`  ❌ ${detail}`);
      });
    }
  };

  try {
    // Test 1: Load .mjs files as ES modules
    console.log("\n--- Test 1: Loading .mjs files as ES modules ---");
    try {
      const moduleExports = await import("~/testSimpleESModule.mjs");
      if (moduleExports) {
        recordPass("Module exports:", JSON.stringify(moduleExports));
      } else {
        recordFailure("ES Module loaded but exports are null");
      }

      if (moduleExports?.moduleType === "ES Module") {
        recordPass("moduleType check passed");
      } else {
        recordFailure("moduleType check failed");
      }
    } catch (e) {
      recordFailure("Error loading ES module", { error: e });
    }

    // Test 2: Test import.meta functionality
    console.log("\n--- Test 2: Testing import.meta functionality ---");
    try {
      const importMetaModule = await import("~/testImportMeta.mjs");
      if (
        importMetaModule &&
        importMetaModule.default &&
        typeof importMetaModule.default === "function"
      ) {
        const metaResults = importMetaModule.default();
        console.log(
          "import.meta test results:",
          JSON.stringify(metaResults, null, 2)
        );

        if (
          metaResults &&
          metaResults.hasImportMeta &&
          metaResults.hasUrl &&
          metaResults.hasDirname
        ) {
          recordPass("import.meta properties present");
          console.log("   - import.meta.url:", metaResults.url);
          console.log("   - import.meta.dirname:", metaResults.dirname);
        } else {
          recordFailure("import.meta properties missing", {
            details: [
              `   - hasImportMeta: ${metaResults?.hasImportMeta}`,
              `   - hasUrl: ${metaResults?.hasUrl}`,
              `   - hasDirname: ${metaResults?.hasDirname}`,
            ],
          });
        }
      } else {
        recordFailure("import.meta module has no default export function");
      }
    } catch (e) {
      recordFailure("Error testing import.meta", { error: e });
    }

    // Test 3: Test Worker enhancements
    console.log("\n--- Test 3: Testing Worker enhancements ---");
    try {
      const workerModule = await import("~/testWorkerFeatures.mjs");
      if (
        workerModule &&
        workerModule.testWorkerFeatures &&
        typeof workerModule.testWorkerFeatures === "function"
      ) {
        const workerResults = workerModule.testWorkerFeatures();
        console.log(
          "Worker features test results:",
          JSON.stringify(workerResults, null, 2)
        );

        if (
          workerResults &&
          workerResults.stringPathSupported &&
          workerResults.urlObjectSupported &&
          workerResults.tildePathSupported
        ) {
          recordPass("Worker enhancement features present");
          console.log(
            "   - String path support:",
            workerResults.stringPathSupported
          );
          console.log(
            "   - URL object support:",
            workerResults.urlObjectSupported
          );
          console.log(
            "   - Tilde path support:",
            workerResults.tildePathSupported
          );
        } else {
          recordFailure("Worker enhancement features missing", {
            details: [
              `   - stringPathSupported: ${workerResults?.stringPathSupported}`,
              `   - urlObjectSupported: ${workerResults?.urlObjectSupported}`,
              `   - tildePathSupported: ${workerResults?.tildePathSupported}`,
            ],
          });
        }
      } else {
        recordFailure(
          "Worker features module has no testWorkerFeatures function"
        );
      }
    } catch (e) {
      recordFailure("Error testing Worker features", { error: e });
    }
  } catch (unexpectedError) {
    recordFailure("Unexpected ES module test harness failure", {
      error: unexpectedError,
    });
  } finally {
    logFinalSummary();
  }

  return { passed, failed };
}

// Run the tests immediately (avoid top-level await for broader runtime support)
runESModuleTests().catch((e) => {
  console.error("ES Module top-level failure:", e?.message ?? e);
});
