__disableVerboseLogging();

// methods that common tests need to run
var testContent = "";
__collect = gc;
TNSClearOutput = function () {
	testContent = "";
}
TNSLog = function (text) {
	testContent += text;
}
TNSGetOutput = function () {
	return testContent;
}
__approot = __dirname.substr(0, __dirname.length - 4);
var shared = require("./shared");
shared.runRequireTests();
shared.runWeakRefTests();
shared.runRuntimeTests();
shared.runWorkerTests();
require("./tests/testWebAssembly");
require("./tests/testMultithreadedJavascript");
require("./tests/testInterfaceDefaultMethods");
require("./tests/testInterfaceStaticMethods");
require("./tests/testMetadata");
require("./tests/testAsserts");
require("./tests/testWeakRef");
require("./tests/tests");
require("./tests/testMethodResolution");
require("./tests/testMethodResolutionWithNulls");
require("./tests/testArrays");
require("./tests/testsForRuntimeBindingGenerator");
require("./tests/testPrimitiveTypeConversion");
require("./tests/numericConversionTests");
require("./tests/inheritanceChainResolutionTest");
require("./tests/exceptionHandlingTests");
require("./tests/discardedExceptionsTest");
require("./tests/dispatchCallbacksOnUiThreadTests");
require("./tests/stringConversionTests");
require("./tests/testsForTypescript");
require("./tests/testGC");
require("./tests/testsMemoryManagement");
require("./tests/testFieldGetSet");
require("./tests/extendedClassesTests");
//require("./tests/extendClassNameTests"); // as tests now run with SBG, this test fails the whole build process
require("./tests/testJniReferenceLeak");
require("./tests/testNativeModules");
require("./tests/requireExceptionTests");
require("./tests/java-array-test");
require("./tests/field-access-test");
require("./tests/byte-buffer-test");
require("./tests/dex-interface-implementation");
require("./tests/testInterfaceImplementation");
require("./tests/testRuntimeImplementedAPIs");
require("./tests/testsInstanceOfOperator");
require("./tests/testReleaseNativeCounterpart");
require("./tests/testJSONObjects");
require("./tests/kotlin/companions/testCompanionObjectsSupport");
require("./tests/kotlin/properties/testPropertiesSupport");
require("./tests/kotlin/delegation/testDelegationSupport");
require("./tests/kotlin/objects/testObjectsSupport");
require("./tests/kotlin/functions/testTopLevelFunctionsSupport");
require("./tests/kotlin/extensions/testExtensionFunctionsSupport");
require("./tests/kotlin/enums/testEnumsSupport");
require("./tests/kotlin/access/testInternalLanguageFeaturesSupport");
require("./tests/testPackagePrivate");
require("./tests/kotlin/properties/testPropertiesSupport.js");
require('./tests/testNativeTimers');
require("./tests/testPostFrameCallback");
require("./tests/console/logTests.js");
require('./tests/testURLImpl.js');
require('./tests/testURLSearchParamsImpl.js');
require('./tests/testPerformanceNow');

// ES MODULE TESTS
__log("=== Running ES Modules Tests ===");
require("./tests/testESModules");
