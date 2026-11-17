onmessage = function () {
	var stringNull = java.lang.String.null;
	postMessage({
		valueOf: java.lang.String.valueOf(7),
		nullValueOf: stringNull.valueOf(),
		distinct: stringNull !== java.lang.String,
		overload: new com.tns.tests.DummyClass().methodWithOverloadsWithOneArgument(java.io.File.null),
		isNull: java.util.Objects.isNull(stringNull)
	});
};
