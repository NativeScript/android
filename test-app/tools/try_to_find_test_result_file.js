const { exec } = require("child_process");

const appId = "com.tns.testapplication";
const runOnDeviceOrEmulator = process.argv[2] || "";
const adbPrefix = `adb ${runOnDeviceOrEmulator} -e`;
const resultsPath = `/data/data/${appId}/android_unit_test_results.xml`;

const processTimeoutMs = 20 * 60 * 1000; // 20 minutes timeout (empirical constant)
const pollIntervalMs = 10 * 1000;
const scriptStartTime = new Date();
const logcatCutoffTime = new Date(scriptStartTime.getTime() - 60 * 1000); // 1 minute before start

let timedOut = false;
// TODO: check why gradle doesn't show error logs
console.error = console.log; // Redirect stderr to stdout for easier logcat capture

function execAndStream(command, streamOutput = false) {
  return new Promise((resolve) => {
    const child = exec(command, (error, stdout, stderr) => {
      resolve({ error, stdout, stderr });
    });

    if (!streamOutput) {
      return;
    }
    if (child.stdout) {
      child.stdout.pipe(process.stdout, { end: false });
    }
    if (child.stderr) {
      child.stderr.pipe(process.stderr, { end: false });
    }
  });
}

function parseLogcatTimestamp(line) {
  // Logcat format: "MM-DD HH:mm:ss.mmm"
  const timeMatch = line.match(/(\d{2})-(\d{2})\s+(\d{2}):(\d{2}):(\d{2})\.(\d{3})/);
  if (!timeMatch) return null;

  const month = parseInt(timeMatch[1], 10) - 1; // JavaScript months are 0-indexed
  const day = parseInt(timeMatch[2], 10);
  const hour = parseInt(timeMatch[3], 10);
  const minute = parseInt(timeMatch[4], 10);
  const second = parseInt(timeMatch[5], 10);
  const ms = parseInt(timeMatch[6], 10);

  // Create a date object for this log entry (use current year)
  const logDate = new Date();
  logDate.setMonth(month);
  logDate.setDate(day);
  logDate.setHours(hour, minute, second, ms);

  return logDate;
}

function filterLogcatFromCutoff(logcatOutput) {
  const lines = logcatOutput.split("\n");

  // Find the first line that is at or after the cutoff time
  const startIndex = lines.findIndex((line) => {
    const timestamp = parseLogcatTimestamp(line);
    return timestamp && timestamp >= logcatCutoffTime;
  });

  // If no matching line found, return empty; otherwise return from that line onward
  if (startIndex === -1) return "";
  return lines.slice(startIndex).join("\n");
}

async function readAndFilterLogcat() {
  return new Promise((resolve) => {
    exec(`${adbPrefix} logcat -d | grep ${appId}`, (error, stdout) => {
      resolve(filterLogcatFromCutoff(stdout || ""));
    });
  });
}

async function exitWithLogcatDump() {
  console.log("Dumping logcat for debugging:");
  try {
    const logcat = await readAndFilterLogcat();
    console.log(logcat);
  } finally {
    process.exit(1);
  }
}

async function ensureProcessAlive() {
  const { stdout } = await execAndStream(
    `${adbPrefix} shell "ps | grep ${appId}"`
  );

  if (!stdout) {
    console.error(`${appId} process died or never started!`);
    await exitWithLogcatDump();
  }

  console.log(`${appId} process is running`);
}

async function checkForErrorActivity() {
  const { error } = await execAndStream(
    `${adbPrefix} shell "dumpsys activity activities | grep ${appId} | grep ${appId}.ErrorReportActivity"`
  );

  if (!error) {
    console.error("App has crashed - ErrorReportActivity is displaying!");
    await exitWithLogcatDump();
  }
}

async function tryPullResultsFile() {
  const { error } = await execAndStream(`${adbPrefix} pull ${resultsPath}`);

  if (!error) {
    console.log("Tests results file found!");
    process.exit(0);
  }
}

async function pollForResults() {
  await ensureProcessAlive();
  await checkForErrorActivity();
  await tryPullResultsFile();

  if (timedOut) {
    console.error("Tests results file not found within timeout window.");
    await exitWithLogcatDump();
  }

  setTimeout(pollForResults, pollIntervalMs);
}

function main() {
  setTimeout(() => {
    timedOut = true;
  }, processTimeoutMs);

  console.log(
    `Waiting for test results file on ${
      runOnDeviceOrEmulator || "default device"
    }...`
  );
  pollForResults().catch((err) => {
    console.error("Unexpected failure while waiting for results", err);
    process.exit(1);
  });
}

main();
