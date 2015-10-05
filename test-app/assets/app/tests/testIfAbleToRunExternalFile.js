describe("Tests running external files", function () {
	
	it("When_file_outside_the_project_folder_is_required_it_should_fail", function () {

		__log("When_file_outside_the_project_folder_is_required_it_should_throw_IllegalAccessException"); 

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
});