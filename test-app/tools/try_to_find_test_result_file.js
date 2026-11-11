const { exec } = require("child_process");
const fs = require("fs");
const path = require("path");

const appId = "com.tns.testapplication";
const runOnDeviceOrEmulator = process.argv.slice(2).join(" ") || "-e";
const adbPrefix = `adb ${runOnDeviceOrEmulator}`;
const resultsFileName = "android_unit_test_results.xml";

const processTimeoutMs = 20 * 60 * 1000; // 20 minutes timeout (empirical constant)
const pollIntervalMs = 10 * 1000;
// adb can hang outright on a wedged or overloaded device; a hung call would also stall the
// overall timeout check, which only runs between calls.
const adbCallTimeoutMs = 30 * 1000;
// exec kills the child past maxBuffer (1 MiB by default); the results XML and a full logcat
// dump can both exceed that.
const adbExecOptions = { timeout: adbCallTimeoutMs, maxBuffer: 64 * 1024 * 1024 };
const scriptStartTime = new Date();
const cutoffMarginMs = 60 * 1000;
// Logcat stamps are device-local wall-clock time, so main() replaces this with
// the device's clock when it can read it; the host clock is only a fallback.
let logcatCutoffTime = new Date(scriptStartTime.getTime() - cutoffMarginMs);
// Uploaded by CI as an artifact when the run fails.
const diagnosticsDir = path.join(__dirname, "..", "dist", "crash-diagnostics");

let timedOut = false;
// TODO: check why gradle doesn't show error logs
console.error = console.log; // Redirect stderr to stdout for easier logcat capture

function execAndStream(command, streamOutput = false) {
  return new Promise((resolve) => {
    const child = exec(command, adbExecOptions, (error, stdout, stderr) => {
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
  const timeMatch = line.match(
    /(\d{2})-(\d{2})\s+(\d{2}):(\d{2}):(\d{2})\.(\d{3})/
  );
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

function adbOutput(args) {
  return new Promise((resolve) => {
    exec(`${adbPrefix} ${args}`, adbExecOptions, (error, stdout, stderr) => {
      resolve({ error, stdout: stdout || "", stderr: stderr || "" });
    });
  });
}

function writeDiagnostic(name, contents) {
  try {
    fs.mkdirSync(diagnosticsDir, { recursive: true });
    fs.writeFileSync(path.join(diagnosticsDir, name), contents);
  } catch (err) {
    console.log(`Could not write ${name}: ${err.message}`);
  }
}

function lastLines(text, count) {
  const lines = text.split("\n");
  return lines.slice(Math.max(0, lines.length - count)).join("\n");
}

// The newest ApplicationExitInfo record: its reason (native crash, crash, ANR,
// low memory, signaled...) is the one thing that tells a crash from a kill.
function latestExitRecord(exitInfo) {
  const start = exitInfo.indexOf("ApplicationExitInfo #0:");
  if (start === -1) return "";
  const next = exitInfo.indexOf("ApplicationExitInfo #1:", start);
  return exitInfo.slice(start, next === -1 ? undefined : next).trim();
}

async function currentAppPid() {
  const { stdout } = await adbOutput(
    `shell "pidof ${appId} 2>/dev/null || ps | grep ${appId}"`
  );
  return parseAppPid(stdout);
}

async function collectDiagnostics() {
  const sections = [];

  // Unfiltered by pid on purpose: crash_dump's backtrace (tag DEBUG), the
  // ActivityManager death/ANR lines and lmkd kills are logged by other pids.
  const fullLogcat = filterLogcatFromCutoff(
    (await adbOutput("logcat -d -v threadtime -b main,system,crash")).stdout
  );
  writeDiagnostic("logcat.txt", fullLogcat);

  if (appPID !== -1) {
    const appLogcat = filterLogcatFromCutoff(
      (await adbOutput(`logcat -d -v threadtime --pid=${appPID}`)).stdout
    );
    writeDiagnostic("logcat-app.txt", appLogcat);
    sections.push(["Last app logcat lines", lastLines(appLogcat, 150)]);
  }

  const crashBuffer = filterLogcatFromCutoff(
    (await adbOutput("logcat -d -v threadtime -b crash")).stdout
  );
  writeDiagnostic("logcat-crash.txt", crashBuffer);
  sections.push(["Crash buffer", lastLines(crashBuffer, 200)]);

  const exitInfo = (await adbOutput(`shell dumpsys activity exit-info ${appId}`)).stdout;
  writeDiagnostic("exit-info.txt", exitInfo);
  sections.push(["Latest process exit", latestExitRecord(exitInfo)]);

  // Tombstones and other processes' stacks need root, which emulator images
  // without Google Play grant. Restarting adbd breaks every other adb client
  // of the device, so only CI, which owns its emulator, asks for it.
  if (process.env.CI) {
    await adbOutput("root");
    await adbOutput("wait-for-device");
  }

  const livePid = await currentAppPid();
  if (Number.isFinite(livePid)) {
    const stacks = await adbOutput(`shell debuggerd -b ${livePid}`);
    writeDiagnostic("stacks.txt", stacks.stdout + stacks.stderr);
    sections.push(["App is still alive; thread stacks written to stacks.txt", ""]);
  }

  // stat's mtime is epoch seconds, independent of the device's time zone.
  const cutoffSeconds = Math.floor((scriptStartTime.getTime() - cutoffMarginMs) / 1000);
  const { stdout: tombstoneList } = await adbOutput(
    `shell "stat -c '%Y %n' /data/tombstones/tombstone_* 2>/dev/null"`
  );
  const tombstones = tombstoneList
    .split("\n")
    .map((line) => line.trim().split(" "))
    .filter(([mtime, file]) => file && !file.endsWith(".pb") && parseInt(mtime, 10) >= cutoffSeconds)
    .map(([, file]) => file);
  for (const file of tombstones) {
    const { stdout } = await adbOutput(`exec-out cat ${file}`);
    writeDiagnostic(path.basename(file) + ".txt", stdout);
  }
  sections.push([
    "Tombstones from this run",
    tombstones.length ? tombstones.join("\n") : "(none, or not readable without root)",
  ]);

  for (const [title, body] of sections) {
    console.log(`----- ${title} -----`);
    if (body) console.log(body);
  }
  console.log(`Full diagnostics written to ${diagnosticsDir}`);
}

async function exitWithDiagnostics() {
  try {
    await collectDiagnostics();
  } catch (err) {
    console.log(`Collecting diagnostics failed: ${err && err.message}`);
  } finally {
    process.exit(1);
  }
}

// A single slow or failed adb call on a loaded host is not a crash; only give up after
// several consecutive misses.
const maxConsecutiveLivenessMisses = 3;
let consecutiveLivenessMisses = 0;
let appPID = -1;

// pidof prints "<pid>". API 21-23 images have no pidof, so fall back to their ps, whose rows
// are "<user> <pid> ...".
function parseAppPid(output) {
  const text = (output || "").trim();
  const match = /^(\d+)/.exec(text) || /^\S+\s+(\d+)/.exec(text);
  return match ? parseInt(match[1], 10) : NaN;
}

async function ensureProcessAlive() {
  const { stdout } = await execAndStream(
    `${adbPrefix} shell "pidof ${appId} 2>/dev/null || ps | grep ${appId}"`
  );
  const pid = parseAppPid(stdout);

  if (!Number.isFinite(pid)) {
    consecutiveLivenessMisses++;
    if (consecutiveLivenessMisses < maxConsecutiveLivenessMisses) {
      console.log(
        `${appId} process not found (${consecutiveLivenessMisses}/${maxConsecutiveLivenessMisses}), checking again`
      );
      return;
    }
    console.error(`${appId} process died or never started!`);
    await exitWithDiagnostics();
  }

  consecutiveLivenessMisses = 0;
  if (appPID === -1) {
    appPID = pid;
  }
  console.log(`${appId} process is running`);
}

async function checkForErrorActivity() {
  const { error } = await execAndStream(
    `${adbPrefix} shell "dumpsys activity activities | grep ${appId} | grep ${appId}.ErrorReportActivity"`
  );

  if (!error) {
    console.error("App has crashed - ErrorReportActivity is displaying!");
    await exitWithDiagnostics();
  }
}

function isCompleteJunitXml(text) {
  if (!text || typeof text !== "string") {
    return false;
  }
  const trimmed = text.trim();
  return /<testsuites[\s>]/.test(trimmed) && trimmed.includes("</testsuites>");
}

// The app writes into its private data dir. `run-as` reads it without root, as
// long as the installed build is debuggable.
async function tryPullResultsFile() {
  const { error, stdout } = await execAndStream(
    `${adbPrefix} exec-out run-as ${appId} cat ${resultsFileName}`
  );
  if (!error && isCompleteJunitXml(stdout)) {
    require("fs").writeFileSync(resultsFileName, stdout);
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
    await exitWithDiagnostics();
  }

  setTimeout(pollForResults, pollIntervalMs);
}

function delay(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function useDeviceClockForLogcatCutoff() {
  const { stdout } = await adbOutput(`shell "date '+%m-%d %H:%M:%S.000'"`);
  const deviceNow = parseLogcatTimestamp(stdout);
  if (deviceNow) {
    logcatCutoffTime = new Date(deviceNow.getTime() - cutoffMarginMs);
  }
}

async function main() {
  await useDeviceClockForLogcatCutoff();
  setTimeout(() => {
    timedOut = true;
  }, processTimeoutMs);

  console.log(
    `Waiting for test results file on ${
      runOnDeviceOrEmulator || "default device"
    }...`
  );
  await delay(10000); // Initial delay to allow app startup
  pollForResults().catch((err) => {
    console.error("Unexpected failure while waiting for results", err);
    process.exit(1);
  });
}

main();
