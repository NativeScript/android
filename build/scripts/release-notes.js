// run this using "node release-notes.js", this will prepend to the start of the "../../CHANGELOG.MD"

var https = require('https');
var fs = require('fs');

var readline = require('readline');

var milestoneRequest = {
    hostname: 'api.github.com',
    path: '/repos/NativeScript/android-runtime/milestones',
    method: 'GET',
    headers: {
        "User-Agent": "AutoRN",
        "Authorization": undefined
    }
};

var issuesRequest = {
    hostname: 'api.github.com',
    path: undefined,
    method: 'GET',
    headers: {
        "User-Agent": "AutoRN",
        "Authorization": undefined
    }
};

var username, password;
var milestones = {};

getCredentials();

function getCredentials() {

    var rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    rl.on('SIGINT', function() {
        // Ctrl-C
        console.log("Canceled.");
        process.exit(1);
    });

    rl.question("Username: ", function(usr) {
        rl.close();
        username = usr;
        getPassword(function(pass) {
            password = pass;
            listMilestones();
        });
    })
};

function listMilestones() {

    console.log("Request open milestones.");
    milestoneRequest.headers.Authorization = "Basic " + new Buffer(username + ":" + password).toString("base64");

    var req = https.request(milestoneRequest, gitHubResponse(function (data) {

        if (!(data instanceof Array)) {
            console.log("Error. Expected to get array with the issues for the milestone!\n");
            process.exit(1);
        } else {
            console.log("Open Milestones:");
            data.forEach(function (milestone) {
                console.log(" - '" + milestone.title + "'");
                milestones[milestone.title] = milestone;
            });
            console.log();
        }

        selectMilestone();

    }));
    req.end();

    req.on('error', function (e) {
        console.error(e);
    });
}

function selectMilestone() {
    var rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    rl.on('SIGINT', function() {
        // Ctrl-C
        console.log("Canceled.");
        process.exit(1);
    });

    rl.question("Milestone: ", function(milestone) {
        rl.close();
        var selectedMilestone = milestones[milestone];
        if (selectedMilestone) {
            createReleaseNotes(selectedMilestone);
        } else {
            console.log("Milestone error!");
        }
    });
}

function createReleaseNotes(milestone) {
        
    console.log("Request closed issues in the milestone.");
    issuesRequest.headers.Authorization = "Basic " + new Buffer(username + ":" + password).toString("base64");
    issuesRequest.path = '/repos/NativeScript/android-runtime/issues?milestone=' + milestone.number + '&state=closed';
    
    var req = https.request(issuesRequest, gitHubResponse(function (data) {

        if (!(data instanceof Array)) {
            console.log("Error. Expected to get array with the milestones!\n");
            process.exit(1);
        } else {
            var issues = data;

            console.log("Received " + issues.length + " entries.");

            var bugs = issues.filter(function (i) {
                return i.labels.filter(function (l) {
                    return l.name == "T:Bug";
                }).length > 0;
            });
            var features = issues.filter(function (i) {
                return i.labels.filter(function (l) {
                    return l.name == "T:Feature";
                }).length > 0;
            });
            
             var performance = issues.filter(function (i) {
                return i.labels.filter(function (l) {
                    return l.name == "T:Performance";
                }).length > 0;
            });

            console.log(" - " + bugs.length + " fixed (e.g. T:Bug)");
            console.log(" - " + features.length + " new (e.g. T:Feature");
            console.log(" - " + performance.length + " performance (e.g. T:Performance");

            var md = "";

            md += milestone.title + "\r\n==\r\n\r\n";

            printSection("What's New", features);
            printSection("Bug Fixes", bugs);
            printSection("Performance", performance);

            function printSection(title, issues) {
                if (issues.length > 0) {
                    md += "## " + title + "\r\n\r\n";
                    issues.forEach(printIssue);
                    md += "\r\n";
                }
            }

            function printIssue(i) {
                md += " - [" + i.title + " (#" + i.number + ")](" + i.html_url + ")\r\n";
            }

            var currentLog = fs.readFileSync("../../CHANGELOG.md");
            var newLog = md + currentLog;
            fs.writeFileSync("../../CHANGELOG.md", newLog);
        }

    }));
    req.end();

    req.on('error', function (e) {
        console.error(e);
    });
}

function gitHubResponse(f) {
    var msg = "";

    return function(res) {

        console.log();

        var nlTerminate = false;
        if (res.headers['x-ratelimit-limit']) {
            console.log("Rate limit: " + res.headers['x-ratelimit-limit']);
            nlTerminate = true;
        }
        if (res.headers['x-ratelimit-remaining']) {
            console.log("Rate remaining: " + res.headers['x-ratelimit-remaining']);
            nlTerminate = true;
        }
        if (res.headers['x-ratelimit-reset']) {
            console.log("Rate will reset: " + new Date(res.headers['x-ratelimit-reset'] * 1000));
            nlTerminate = true;
        }

        if (nlTerminate) {
            console.log();
        }

        res.on('data', function (d) {
            msg += d;
        });
        res.on('end', function (d) {

            var data = JSON.parse(msg);
            if (data.message) {
                console.log("Message:\n" + data.message + "\n");
            }
            if (data.documentation_url) {
                console.log("Documentation:\n" + data.documentation_url + "\n");
            }

            f(data);
        });
    }
}

function getPassword(callback) {
    process.stdout.write("Password: ");

    var stdin = process.stdin;
    stdin.resume();
    stdin.setRawMode(true);
    stdin.resume();
    stdin.setEncoding('utf8');

    var password = '';
    stdin.on('data', onStdinData);

    function onStdinData(ch) {
        ch = ch + "";

        switch (ch) {
        case "\n":
        case "\r":
        case "\u0004":
            // They've finished typing their password
            process.stdout.write('\n');
            stdin.setRawMode(false);
            stdin.pause();
            callback(password);
            stdin.removeListener('data', onStdinData);
            break;
        case "\u0003":
            // Ctrl-C
            console.log("Canceled.");
            process.exit(1);
            break;
        case "\u007F":
            // Backspace
            if (password.length > 0) {
                password = password.substring(0, password.length - 1);
                process.stdout.write('*\033[<1>D');
            } else {
                process.stdout.write(' \033[<1>D');
            }
            break;
        default:
            // More passsword characters
            process.stdout.write('*\033[<1>D');
            password += ch;
            break;
        }
    }
}

