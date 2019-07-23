package org.nativescript.staticbindinggenerator;

import org.apache.commons.io.FileUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.nativescript.staticbindinggenerator.nodejs.NodeJSProcess;
import org.nativescript.staticbindinggenerator.nodejs.impl.NodeJSProcessImpl;
import org.nativescript.staticbindinggenerator.system.environment.impl.EnvironmentVariablesReaderImpl;
import org.nativescript.staticbindinggenerator.system.process.impl.ProcessExecutorImpl;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static final String SBG_INPUT_FILE = "sbg-input-file.txt";
    public static final String SBG_OUTPUT_FILE = "sbg-output-file.txt";
    public static final String SBG_BINDINGS_NAME = "sbg-bindings.txt";
    public static final String SBG_JS_PARSED_FILES = "sbg-js-parsed-files.txt";
    public static final String SBG_INTERFACE_NAMES = "sbg-interfaces-names.txt";

    private static String jsCodeAbsolutePath;
    private static List<String> inputJsFiles;
    private static File outputDir;
    private static File inputDir;
    private static String dependenciesFile;
    private static String webpackWorkersExcludePath;

    static {
        inputJsFiles = new ArrayList<>();
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException {
        InputParameters.parseCommand(args);

        validateInput();

        getWorkerExcludeFile();

        List<DataRow> rows = Generator.getRows(dependenciesFile);
        GetInterfaceNames.generateInterfaceFile(rows);
        generateJsInputFile();
        runJsParser();

        // generate java bindings
        String inputBindingFilename = Paths.get(System.getProperty("user.dir"), SBG_BINDINGS_NAME).toString();
        Generator generator = new Generator(outputDir, rows, isSuppressCallJSMethodExceptionsEnabled());
        generator.writeBindings(inputBindingFilename);
    }

    /*
     * Method should traverse all js files from input folder and put the ones that need traversing in another file
     * */
    private static void generateJsInputFile() throws IOException {
        try {
            traverseDirectory(inputDir, false/*traverse explicitly*/);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        String pathToJsFileParams = SBG_JS_PARSED_FILES;
        PrintWriter pw = GetInterfaceNames.ensureOutputFile(pathToJsFileParams);
        for (String f : inputJsFiles) {
            pw.write(f);
            pw.write("\n");
        }
        pw.flush();
        pw.close();
    }

    private static boolean isSuppressCallJSMethodExceptionsEnabled() throws IOException {
        File jsonFile = new File(inputDir, "package.json");
        if (!jsonFile.exists()) {
            return false;
        }
        String jsonContent = FileUtils.readFileToString(jsonFile, "UTF-8");
        JSONObject pjson = null;
        try {
            pjson = new JSONObject(jsonContent);
            if (pjson.has("android")) {
                Object androidJsonProperty = pjson.get("android");

                if (androidJsonProperty instanceof JSONObject) {
                    JSONObject androidSettings = (JSONObject) androidJsonProperty;
                    if (androidSettings.has("suppressCallJSMethodExceptions") && androidSettings.get("suppressCallJSMethodExceptions").toString().equals("true")) {
                        return true;
                    }
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return false;
    }

    private static void validateInput() throws IOException {
        dependenciesFile = "sbg-java-dependencies.txt";
        if (!(new File(dependenciesFile).exists())) {
            throw new IllegalArgumentException(String.format("Couldn't find input dependenciesFile file. Make sure the file %s is present.", dependenciesFile));
        }

        List<DataRow> inputFile = Generator.getRows(SBG_INPUT_FILE);
        inputDir = new File(inputFile.get(0).getRow());
        webpackWorkersExcludePath = Paths.get(inputDir.getAbsolutePath(), "__worker-chunks.json").toString();

        if (!inputDir.exists() || !inputDir.isDirectory()) {
            throw new IllegalArgumentException(String.format("Couldn't find the output dir %s or it wasn't a directory", inputDir.getAbsolutePath()));
        }
        jsCodeAbsolutePath = inputDir.getAbsolutePath();

        List<DataRow> outputFile = Generator.getRows(SBG_OUTPUT_FILE);
        outputDir = new File(outputFile.get(0).getRow());
        if (!outputDir.exists() || !outputDir.isDirectory()) {
            System.out.println(String.format("Couldn't find the output dir %s or it wasn't a directory so it will be created!", outputDir.getAbsolutePath()));
            outputDir.mkdirs();
        }
    }

    /*
     * Run the javascript static analysis [js_parser] and generate an output file.
     * This output file should contain all the information needed to generate java counterparts to the traversed js classes.
     * */
    private static void runJsParser() {
        String parserPath = Paths.get(System.getProperty("user.dir"), "jsparser", "js_parser.js").toString();
        NodeJSProcess nodeJSProcess = new NodeJSProcessImpl(new ProcessExecutorImpl(), new EnvironmentVariablesReaderImpl());
        int exitCode = nodeJSProcess.runScript(parserPath);

        if (exitCode != 0) {
            System.exit(exitCode);
        }
    }

    private static Boolean rootTraversed = false;

    private static void traverseDirectory(File currentDir, boolean traverseExplicitly) throws IOException, JSONException {
        Boolean pJsonFile = false;

        if (!traverseExplicitly) {
            if (rootTraversed || !currentDir.getAbsolutePath().equals(jsCodeAbsolutePath)) {
                for (File f : currentDir.listFiles()) {
                    if (f.getName().equals("package.json")) {
                        pJsonFile = true;
                        break;
                    }
                }

                if (pJsonFile) {
                    File jsonFile = new File(currentDir, "package.json");
                    String jsonContent = FileUtils.readFileToString(jsonFile, "UTF-8");
                    JSONObject pjson;
                    try {
                        pjson = new JSONObject(jsonContent);
                    } catch (JSONException e) {
                        e.printStackTrace();
                        return;
                    }

                    if (!pjson.has("nativescript")) {
                        return;
                    } else {
                        Object nativeScriptJsonProperty = pjson.get("nativescript");
                        if (nativeScriptJsonProperty instanceof JSONObject) {
                            JSONObject nsValue = (JSONObject) nativeScriptJsonProperty;
                            if (nsValue.has("recursive-static-bindings")) {
                                traverseExplicitly = true;
                            }
                        } else {
                            return;
                        }
                    }
                }
            } else {
                rootTraversed = true;
            }
        }
        for (File f : currentDir.listFiles()) {
            String currFile = f.getAbsolutePath();
            if (f.isFile() && isJsFile(currFile) && !isWorkerScript(currFile)) {
                inputJsFiles.add(currFile);
            }
            if (f.isDirectory()) {
                traverseDirectory(f, traverseExplicitly);
            }
        }
    }

    private static List<String> webpackWorkersExcludesList;

    /*
     * Should provide the webpack specific files that need to be excluded from the js analysis
     * */
    private static void getWorkerExcludeFile() {
        webpackWorkersExcludesList = new ArrayList<String>();

        File workersExcludeFile = new File(webpackWorkersExcludePath);
        if (workersExcludeFile.exists()) {
            try {
                String workersExcludeFileContent = FileUtils.readFileToString(workersExcludeFile, Charset.defaultCharset());
                JSONArray jsonarray = new JSONArray(workersExcludeFileContent);
                for (int i = 0; i < jsonarray.length(); i++) {
                    String excludeFile = (String) jsonarray.get(i);
                    webpackWorkersExcludesList.add(excludeFile);
                }
            } catch (Exception e) {
                e.printStackTrace();
                System.out.println("Malformed workers exclude file at ${webpackWorkersExcludePath}");
            }
        }
    }

    private static boolean isWorkerScript(String currFile) {
        for (String f : webpackWorkersExcludesList) {
            File workerExcludeFile = new File(f);
            if (workerExcludeFile.getAbsolutePath().equals(currFile)) {
                return true;
            }
        }
        return false;
    }

    private static boolean isJsFile(String fileName) {
        return fileName.substring(fileName.length() - 3).equals(".js");
    }
}