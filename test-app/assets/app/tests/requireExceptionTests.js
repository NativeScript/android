describe("Tests require exceptions ", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("when file is required by absolute path and is outside the project folder an exception should be thrown", function () {
	
		var illegalAccesExceptionCaught = false;
		var fileSeparator = "/";
		var nonExistingFileName = "nonExistingFile";
		var nonExistingFileExtension = ".js";
		
		//create a file in external storage
		var pathToExternalStorage = android.os.Environment.getExternalStorageDirectory().toString();
		var appDirectory = new java.io.File(pathToExternalStorage + fileSeparator + nonExistingFileName + nonExistingFileExtension);
		appDirectory.mkdirs();
		
		try
		{
			//try to require it with absolute path (requireing files with absolute path should not be possible)
			require(pathToExternalStorage + fileSeparator + nonExistingFileName);
		}
		catch(e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("when requiring a relative extenal path an exception should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Module ../../../a.js is an external path. You can only load modules inside the application!";
		var thrownException;
		try
		{
			require("../../../a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
	
	it("when requiring a relative (../) non existing module and error should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Failed to find module: \"../a.js\", relative to: /app/";
		var thrownException;
		try
		{
			require("../a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);			
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
	
	it("when requiring a relative (~/) non existing module and error should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Failed to find module: \"~/a.js\", relative to: /app/";
		var thrownException;
		try
		{
			require("~/a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);			
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
	
	it("when requiring a relative (./) non existing module and error should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Failed to find module: \"./a.js\", relative to: /app/";
		var thrownException;
		try
		{
			require("./a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);			
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
	
	it("when requiring a relative non existing tns_module an error should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Failed to find module: \"a.js\", relative to: /app/tns_modules";
		var thrownException;
		try
		{
			require("a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);			
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
	
	it("when requiring a relative non existing tns_module from folder an error should be thrown", function () {
		
		var exceptionCaught = false;
		var partialMessage = "Error: com.tns.NativeScriptException: Failed to find module: \"a/a.js\", relative to: /app/tns_modules";
		var thrownException;
		try
		{
			require("a/a.js");
		}
		catch(e)
		{
			thrownException = e.toString().substr(0, partialMessage.length);			
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
		expect(partialMessage).toBe(thrownException);
	});
});