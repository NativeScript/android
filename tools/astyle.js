let process = require("process");
let child_process = require("child_process");
let argv = process.argv;

const validFiles = ['c', 'cpp', 'h', 'java'];

// let date = new Date();
// let dateString = `${date.getDay() + 1}-${date.getMonth() + 1}-${date.getFullYear()}--${date.getHours()}:${date.getMinutes()}`
// let backupDir = `formatbackup-${dateString}`;

let astyleOptions = "--style=java -H -k1 -j -C -s4 -xi -n" // "--options=tools\\.astyle"
let astyle = "astyle";

if (process.platform.indexOf("win") === 0) {
    astyle = "tools\\astyle.exe"
} else if (process.platform.indexOf("linux") === 0) {
    astyle = "./tools/astyle"
}

let against = argv[2] || "HEAD";

let stdout = child_process.execSync(`git diff-index --diff-filter=ACMR --name-only -r --cached ${against} --`);
let stdoutStr = stdout.toString("utf8").split(/\r?\n/);

let filteredFiles = stdoutStr.filter((val) => {
    var fileNameParts = val.split(".");
    if (fileNameParts.length > 1) {
        // get the extension (i.e., the last "piece" of the file name)
        var fileExtension = fileNameParts[fileNameParts.length - 1];

        // if the extension is in the array, return true, if not, return false
        return validFiles.indexOf(fileExtension) >= 0;
    }
    else {
        return false;
    }
});

for (let i = 0; i < filteredFiles.length; i++) {
    let file = filteredFiles[i];
    let executable = `${astyle} ${astyleOptions} ${file}`;

    let processStdout = child_process.execSync(executable);
    console.log(processStdout.toString());
    child_process.execSync(`git add ${file}`);
}
