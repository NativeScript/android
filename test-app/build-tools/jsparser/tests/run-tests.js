(function () {
    let { spawnSync, execSync } = require('child_process');

    console.log("Installing JavaScript parser test dependencies.");

    let npmInstallResult = execSync('npm install');

    console.log(npmInstallResult.toString());

    console.log("Executing JavaScript parser tests using Jasmine.");

    const subprocess = spawnSync('node', ['./node_modules/jasmine-xml-reporter/bin/jasmine.js', '--junitreport', '--output=test-results', 'JASMINE_CONFIG_PATH=jasmine.json'], { cwd: __dirname });

    let stdout = subprocess.stdout.toString();
    let stderr = subprocess.stderr.toString();
    let code = subprocess.status;
    let err = subprocess.error;

    if (code !== 0) {
        console.log("Tests runner exited with a non-zero code.");

        let errorString = stdout;

        if (err) {
            errorString = err;
        }

        if (stderr) {
            errorString = stderr;
        }

        throw new Error(errorString);
    }

    console.log(stdout);
}());
