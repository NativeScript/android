__disableVerboseLogging();
__log("starting tests");

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

//require("./shared");
//
//require("./tests/testMetadata");
//require("./tests/testAsserts");
//require("./tests/testWeakRef");
//require("./tests/tests");
//require("./tests/testMethodResolution");
//require("./tests/testMethodResolutionWithNulls");
//require("./tests/testArrays");
//require("./tests/testsForRuntimeBindingGenerator");
//require("./tests/testPrimitiveTypeConversion");
//require("./tests/numericConversionTests");
//require("./tests/inheritanceChainResolutionTest");
//require("./tests/exceptionHandlingTests");
//require("./tests/dispatchCallbacksOnUiThreadTests");
//require("./tests/stringConversionTests");
//require("./tests/testsForTypescript");
//require("./tests/testGC");
//require("./tests/testsMemoryManagement");
//require("./tests/testFieldGetSet");
//require("./tests/extendedClassesTests");
//require("./tests/extendClassNameTests");
//require("./tests/testJniReferenceLeak");
//require("./tests/testNativeModules");
//require("./tests/requireExceptionTests");
//require("./tests/java-array-test");
//require("./tests/field-access-test");
//require("./tests/byte-buffer-test");
//require("./tests/dex-interface-implementation");
//require("./tests/testInterfaceImplementation");
