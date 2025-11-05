/*
 *	Code takes care of static analysis and generates "out_parsed_typescript.txt"
 *	The output file consists of information about custom and common bindings that should be generated.
 */

///////////////// CONFIGURATION /////////////////

const enableLogger =
  (process.env.JS_PARSER_ENABLE_LOGGING &&
    process.env.JS_PARSER_ENABLE_LOGGING.trim() === "true") ||
  (process.argv && process.argv.includes("enableVerboseLogging"));

const showErrorsAndWarnings =
  (process.env.JS_PARSER_ERROR_LOGGING &&
    process.env.JS_PARSER_ERROR_LOGGING.trim() === "true") ||
  (!enableLogger &&
    process.argv &&
    process.argv.includes("enableErrorLogging"));

const loggingSettings = {
  logPath: require("path").join(__dirname, "logs", "i.txt"),
  strategy: "console",
  APP_NAME: "js_parser",
  showErrorsAndWarnings: showErrorsAndWarnings,
  disable: !enableLogger,
};

const fs = require("fs");
const babelParser = require("@babel/parser");
const traverse = require("@babel/traverse");
const split = require("split");
const logger = require("./helpers/logger")(loggingSettings);
const path = require("path");
const es5_visitors = require("./visitors/es5-visitors");
const eol = require("os").EOL;

let BUILD_TOOLS_DIR = `${__dirname}/../`;
let extendDecoratorName = "JavaProxy";
let interfacesDecoratorName = "Interfaces";
let outFile = "out/out_parsed_typescript.txt"; // default out file
let inputDir = "input_parsed_typescript"; // default input folder
let SBG_INTERFACE_NAMES = "sbg-interface-names.txt";
let interfacesNamesFilePath = getRelativeToBuildTools(SBG_INTERFACE_NAMES); //default interace_names file path
let SBG_INPUT_FILE = "sbg-input-file.txt";
let SBG_BINDINGS_NAME = "sbg-bindings.txt";
let SBG_JS_PARSED_FILES = "sbg-js-parsed-files.txt";
let interfaceNames = [];
let inputFiles = [];

//env variables
if (process.env.JS_PARSER_OUT_FILE) {
  outFile = process.env.JS_PARSER_OUT_FILE.trim();
}
if (process.env.JS_PARSER_INPUT_DIR) {
  inputDir = process.env.JS_PARSER_INPUT_DIR.trim();
}
if (process.env.JS_PARSER_INTERFACE_FILE_PATH) {
  interfacesNamesFilePath = process.env.JS_PARSER_INTERFACE_FILE_PATH.trim();
}

inputDir = fs
  .readFileSync(getRelativeToBuildTools(SBG_INPUT_FILE), "UTF-8")
  .trim();

try {
  fs.unlinkSync(getRelativeToBuildTools(SBG_BINDINGS_NAME)); //delete before each run
} catch (e) {}

outFile = getRelativeToBuildTools(SBG_BINDINGS_NAME);
interfacesNamesFilePath = getRelativeToBuildTools(SBG_INTERFACE_NAMES);
let inputFilesPath = getRelativeToBuildTools(SBG_JS_PARSED_FILES);

function getRelativeToBuildTools(relativePath) {
  return path.resolve(`${BUILD_TOOLS_DIR}/${relativePath}`);
}

/////////////// PREPARATION ////////////////
// fileHelpers.createFile(outFile)

/////////////// EXECUTE ////////////////
const tsHelpersFilePath = path.join(
  inputDir,
  "..",
  "internal",
  "ts_helpers.js"
);

/*
 *	If there is an exception anywhere down the line it's caught here
 *	If the error is criticalthe process is exited.
 */
const exceptionHandler = function (path, reason) {
  if (reason.errCode && reason.errCode === 1) {
    logger.error(`Error processing '${path}': ${reason.message}`);
    logger.error("PROCESS EXITING...");
    process.stderr.write(reason.message);
    process.exit(4);
  } else {
    logger.error(`Error processing '${path}:' ` + reason);
  }
};

// ENTRY POINT!
readLinesFromFile(inputFilesPath, inputFiles, tsHelpersFilePath)
  .then(getFileAst)
  .then(readInterfaceNames) //config
  .then(traverseAndAnalyseFilesDir) //start
  .catch(exceptionHandler.bind(null, inputFilesPath));

/*
 *	Get's the javascript files that need traversing
 */
function readLinesFromFile(filePath, outArr, resolveParameter) {
  return new Promise(function (resolve, reject) {
    fs.createReadStream(filePath)
      .pipe(split())
      .on("data", function (line) {
        // skip empty lines
        if (/\S/.test(line)) {
          outArr.push(line.toString().trim());
        }
      })
      .on("error", function (err) {
        return reject(err);
      })
      .on("close", function (e) {
        return resolve(resolveParameter);
      });
  });
}

/*
 *	Get line and column of the __extends function from ts_helpers file
 */
function getFileAst(tsHelpersFilePath) {
  return new Promise(function (resolve, reject) {
    fs.readFile(tsHelpersFilePath, "utf8", function (err, fileContent) {
      if (err) {
        logger.warn("+DIDN'T parse ast from file " + tsHelpersFilePath);
        return reject(err);
      }

      logger.info("+parsing ast from " + tsHelpersFilePath);

      const ast = babelParser.parse(fileContent, {
        minify: false,
        plugins: [
          [
            "@babel/plugin-proposal-decorators",
            { decoratorsBeforeExport: true },
          ],
        ],
      });

      return resolve(ast);
    });
  });
}

/*
 *	Get's pregenerated interface names from "interfacesNamesFilePath"
 *	After reading interface names runs the visiting api
 */
function readInterfaceNames(data, err) {
  return new Promise(function (resolve, reject) {
    fs.createReadStream(interfacesNamesFilePath)
      .pipe(split())
      .on("data", function (line) {
        // skip empty lines
        if (/\S/.test(line)) {
          interfaceNames.push(line.toString().trim());
        }
      })
      .on("error", function (e) {
        return reject(false);
      })
      .on("close", function (e) {
        inputDir = path.normalize(inputDir);
        return resolve(inputDir);
      });
  });
}

/*
 *	Traverses a given input directory and attempts to visit every ".js" and ".mjs" file.
 *	It passes each found file down the line.
 */
function traverseAndAnalyseFilesDir(inputDir, err) {
  if (!fs.existsSync(inputDir)) {
    throw "The input dir: " + inputDir + " does not exist!";
  }

  traverseFiles(inputFiles);
}

function traverseFiles(filesToTraverse) {
  for (let i = 0; i < filesToTraverse.length; i += 1) {
    const fp = filesToTraverse[i];
    logger.info("Visiting JavaScript/ES Module file: " + fp);

    readFile(fp)
      .then(astFromFileContent.bind(null, fp))
      .then(visitAst.bind(null, fp))
      .then(writeToFile)
      .catch(exceptionHandler.bind(null, fp));
  }
}

/*
 *	Gets the file content as text and passes it down the line.
 */
const readFile = function (filePath, err) {
  return new Promise(function (resolve, reject) {
    fs.readFile(filePath, function (err, data) {
      if (err) {
        logger.warn(`+DIDN'T get content of file: ${filePath}!`);
        return reject(err);
      }

      const fileInfo = {
        filePath: filePath,
        data: data.toString(),
      };
      return resolve(fileInfo);
    });
  });
};

/*
 *	Get's the AST (https://en.wikipedia.org/wiki/Abstract_syntax_tree) from the file content and passes it down the line.
 *	Supports both CommonJS (.js) and ES modules (.mjs) files.
 */
const astFromFileContent = function (path, data, err) {
  return new Promise(function (resolve, reject) {
    if (err) {
      logger.warn(`+DIDN'T parse ast from file: ${path}!`);
      return reject(err);
    }

    // Determine if this is an ES module based on file extension
    const isESModule = path.endsWith('.mjs');
    
    // Configure Babel parser based on file type
    const parserOptions = {
      minify: false,
      plugins: [
        ["@babel/plugin-proposal-decorators", { decoratorsBeforeExport: true }],
        "objectRestSpread",
      ],
    };
    
    // For ES modules, set sourceType to 'module'
    if (isESModule) {
      parserOptions.sourceType = 'module';
      logger.info(`Parsing ES module: ${path}`);
    } else {
      // For regular JS files, keep existing behavior (default sourceType is 'script')
      logger.info(`Parsing CommonJS file: ${path}`);
    }

    const ast = babelParser.parse(data.data, parserOptions);
    data.ast = ast;
    return resolve(data);
  });
};

//only unique filter
function onlyUnique(value, index, self) {
  return self.indexOf(value) === index;
}

/*
 *	Visist's the passed AST with a given visitor and extracts nativescript speciffic data.
 *	Passes the extracted bindings data down the line.
 */
const visitAst = function (path, data, err) {
  return new Promise(function (resolve, reject) {
    if (err) {
      logger.warn(`+DIDN'T visit ast for file: ${path}!`);
      return reject(err);
    }

    traverse.default(data.ast, {
      enter: function (path) {
        // Determine file extension length to properly strip it from the path
        const fileExtension = data.filePath.endsWith('.mjs') ? '.mjs' : '.js';
        const extensionLength = fileExtension.length;
        
        const decoratorConfig = {
          logger: logger,
          extendDecoratorName: extendDecoratorName,
          interfacesDecoratorName: interfacesDecoratorName,
          filePath:
            data.filePath.substring(
              inputDir.length + 1,
              data.filePath.length - extensionLength
            ) || "",
          fullPathName: data.filePath
            .substring(inputDir.length + 1)
            .replace(/[\\]/g, "/"),
          interfaceNames: interfaceNames,
        };
        es5_visitors.es5Visitor(path, decoratorConfig);
      },
    });

    const customExtendsArr = es5_visitors.es5Visitor.getProxyExtendInfo();
    const normalExtendsArr = es5_visitors.es5Visitor.getCommonExtendInfo();
    const interfacesArr = es5_visitors.es5Visitor.getInterfaceInfo();

    const res = customExtendsArr
      .concat(normalExtendsArr)
      .concat(interfacesArr)
      .filter(onlyUnique)
      .join(eol);
    return resolve(res);
  });
};

const writeToFile = function (data, err) {
  return new Promise(function (resolve, reject) {
    if (data.trim() != "") {
      // fs.appendFile(outFile, stringify(data), function (writeFileError) {
      fs.appendFile(outFile, data + eol, function (writeFileError) {
        if (err) {
          logger.warn("Error from writeToFile: " + err);
          return reject(err);
        }
        if (writeFileError) {
          logger.warn("Error writing file: " + writeFileError);
          return reject(writeFileError);
        }

        logger.info("+appended '" + data + "' to file: " + outFile);
        return resolve(data);
      });
    } else {
      logger.info("No need to generate anything. (UP-TO-DATE)");
    }
  });
};
