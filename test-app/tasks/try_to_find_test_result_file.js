var 
	searchForFile = require('child_process').exec,
	execFindFile = require('child_process').exec,
	fs = require('fs'),
	
	isTimeToExit = false,
	
	processTimeout = 11 * 60 * 1000, // 11 minutes timeout (empirical constant :)) 
	searchInterval = 10000;
	
searchForFile("", getFile);

function getFile(error, stdout, stderr) {
	closeProcessAfter(processTimeout);
	setInterval(tryToGetFile, searchInterval);
};

function closeProcessAfter(timeout) {
	//this will force process closed even if the the file is not found
	setTimeout(function () {isTimeToExit = true;}, timeout); 
}

function tryToGetFile() {
	execFindFile("adb pull /sdcard/android_unit_test_results.xml", checkIfFileExists);
}

function checkIfFileExists(err,stout,stderr) { 

	//if you find file in sdcard exit process
	if(!err) {
		console.log('Tests results file found file!');
		isTimeToExit = true;
	}
	else {
		//if the time to get the file is out exit process
    if(isTimeToExit) {
      console.log(err);
      console.log('Tests results file not found!');
      process.exit();
    }
	}
}