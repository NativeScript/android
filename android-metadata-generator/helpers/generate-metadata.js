#!/usr/bin/env node

var path = require("path");
var fs = require("fs");
var nodeFs = require("node-fs");

var userArgs = process.argv.slice(2);
if (userArgs.length !== 2)
{
  console.error("No arguments supplied. Usage: generator [jars directory] [destination directory]");
  process.exit(-1);
}


var sourceDir = userArgs[0];
var destinationDir = userArgs[1];
var generatorPath = path.normalize(process.argv[1]);

sourceDir = path.normalize(sourceDir);
destinationDir = path.normalize(destinationDir);

console.log("Argument sourceDir: " + sourceDir);
console.log("Argument destinationDir: " + destinationDir);
console.log("Argument generatorPath: " + generatorPath);
console.log("Current working directory: " + process.cwd());

var sourceDirAbsolute;
try
{
    sourceDirAbsolute = fs.realpathSync(sourceDir);
    console.log("Using absolute jars directory: " + sourceDirAbsolute);

    stats = fs.lstatSync(sourceDirAbsolute);
    if (stats.isDirectory())
    {
        var jarFiles = fs.readdirSync(sourceDirAbsolute);
        console.log("\nSource jar files:\n" + jarFiles.join("\n") + "\n\n");
    }
    else
    {
      console.error("jars path should be a directory");
      process.exit(-1);
    }
}
catch (e)
{
   console.error("jars path not found. Pass an existing directory with nativescript.jar file in it.");
   process.exit(-1);
}

//Validate jars dir contains nativescript.jar
try
{
    stats = fs.lstatSync(path.normalize(sourceDirAbsolute + "/nativescript.jar"));
    if (stats.isFile())
    {
      console.log("nativescript.jar found in source directory.");
    }
}
catch (e)
{
  console.error("nativescript.jar not found in source directory.");
  process.exit(-1);
}

try
{
    //Query the entry
    stats = fs.lstatSync(destinationDir);
    // Is it a directory?
    if (!stats.isDirectory())
    {
      console.error("Destination path should be a directory");
      process.exit(-1);
    }
}
catch (e)
{
  console.log("creating destination directory " + destinationDir);
  nodeFs.mkdirSync(destinationDir, 0777, true);
}

var destinationDirAbsolute = fs.realpathSync(destinationDir);
console.log("Using absolute destination directory: " + destinationDirAbsolute);

var generatorPathAbsolute = fs.realpathSync(generatorPath);

var exited = false;


function execSyncFallback(command)
{
  var child_process = require('child_process');
  // Run the command in a subshell
  child_process.exec(command + ' 2>&1 1>output && echo done! > done');
   
  // Block the event loop until the command has executed.
  while (!fs.existsSync('done')) {
  // Do nothing
  }
   
  // Read the output
  var output = fs.readFileSync('output', {encoding: "utf8"});
   
  // Delete temporary files.
  fs.unlinkSync('output');
  fs.unlinkSync('done');
   
  return output;
}

var execSync = fs.execSync || execSyncFallback;
var classesAbsoluteDir = path.normalize(generatorPathAbsolute  + "/../../classes/");
console.log("calling java -cp " + classesAbsoluteDir + " com.telerik.metadata.Generator " + sourceDirAbsolute + " " + destinationDirAbsolute);
var output = execSync("java -cp " + classesAbsoluteDir + " com.telerik.metadata.Generator " + sourceDirAbsolute + " " + destinationDirAbsolute);
console.log("\n\nGenerator output:\n\n" + output);
