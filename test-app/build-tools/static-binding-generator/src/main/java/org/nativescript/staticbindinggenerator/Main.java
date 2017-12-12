package org.nativescript.staticbindinggenerator;

import org.apache.commons.io.FileUtils;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static final String SBG_INPUT_OUTPUT_DIRS = "sbg-input-output-dirs.txt";
    public static final String SBG_BINDINGS_NAME = "sbg-bindings.txt";
    public static final String SBG_JS_PARCED_FILES = "sbg-js-parced-files.txt";
    public static final String SBG_INTERFACE_NAMES = "sbg-interfaces-names.txt";
    private static String jsCodeAbsolutePath;
    private static List<String> inputJsFiles = new ArrayList<>();
    private static File outputDir;
    private static File inputDir;
    private static String dependenciesFile;

    public static void main(String[] args) throws IOException, ClassNotFoundException {

        validateInput();

        //webpack specific excluded files
        getWorkerExcludeFile();

        List<DataRow> rows = Generator.getRows(dependenciesFile);

        //generate interfaceNames.txt needed for js parser
        GetInterfaceNames.generateInterfaceFile(rows);

        //run static js analysis
        runJsParser(inputDir);

        // generate java bindings
        String inputBindingFilename = Paths.get(System.getProperty("user.dir"), SBG_BINDINGS_NAME).toString();
        new Generator(outputDir, rows).writeBindings(inputBindingFilename);
    }

    private static void validateInput() throws IOException {
        dependenciesFile = "sbg-java-dependencies.txt";
        if (!(new File(dependenciesFile).exists())) {
            throw new IllegalArgumentException(String.format("Couldn't find input dependenciesFile file. Make sure the file %s is present.", dependenciesFile));
        }

        List<DataRow> inputOutput = Generator.getRows(SBG_INPUT_OUTPUT_DIRS);
        inputDir = new File(inputOutput.get(0).getRow());
        if (!inputDir.exists() || !inputDir.isDirectory()) {
            throw new IllegalArgumentException(String.format("Couldn't find the output dir %s or it wasn't a directory", inputDir.getAbsolutePath()));
        }
        jsCodeAbsolutePath = inputDir.getAbsolutePath();

        //asd
        outputDir = new File(inputOutput.get(1).getRow());
        if (!outputDir.exists() || !outputDir.isDirectory()) {
            System.out.println(String.format("Couldn't find the output dir %s or it wasn't a directory so it will be created!", outputDir.getAbsolutePath()));
            outputDir.mkdirs();
        }
    }

    private static void runJsParser(File inputDir) throws IOException {
        String parserPath = Paths.get(System.getProperty("user.dir"),"jsparser", "js_parser.js").toString();
        String inputPath = inputDir.getAbsolutePath();
        String bindingsFilePath = Paths.get(System.getProperty("user.dir"), SBG_BINDINGS_NAME).toString();
        String interfaceNamesFilePath = Paths.get(System.getProperty("user.dir"), SBG_INTERFACE_NAMES).toString();
        try {
            traverseDirectory(inputDir, false/*traverse explicitly*/);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        String pathToJsFileParams = SBG_JS_PARCED_FILES;
        PrintWriter pw = GetInterfaceNames.ensureOutputFile(pathToJsFileParams);
        pathToJsFileParams = Paths.get(System.getProperty("user.dir"), pathToJsFileParams).toString();
        for (String f : inputJsFiles) {
            pw.write(f);
            pw.write("\n");
        }
        pw.flush();
        pw.close();

        List<String> l = new ArrayList<String>();
        l.add("node");
        l.add(parserPath);
        l.add(inputPath);
        l.add(bindingsFilePath);
        l.add(interfaceNamesFilePath);
        l.add(pathToJsFileParams);

        ProcessBuilder pb = new ProcessBuilder(l);
        pb.redirectOutput(ProcessBuilder.Redirect.INHERIT);
        pb.redirectError(ProcessBuilder.Redirect.INHERIT);
        Process p = pb.start();
        try {
            p.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
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
                    JSONObject pjson = null;
                    try {
                        pjson = new JSONObject(jsonContent);
                    } catch (JSONException e) {
                        e.printStackTrace();
                        return;
                    }

                    if (!pjson.has("nativescript")) {
                        return;
                    } else {
                        JSONObject nsValue = (JSONObject) pjson.get("nativescript");
                        if (nsValue.has("recursive-static-bindings")) {
//                            System.out.println(String.format("Task: traverseDirectory: Folder will be traversed completely: %s", currentDir));
                            traverseExplicitly = true;
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

    private static String webpackWorkersExcludePath = System.getProperty("user.dir") + "/app/src/main/assets/app/__worker-chunks.json";
    private static List<String> webpackWorkersExcludesList;

    private static void getWorkerExcludeFile() {
        webpackWorkersExcludesList = new ArrayList<String>();

        File workersExcludeFile = new File(webpackWorkersExcludePath);
        if (workersExcludeFile.exists()) {
            try {
                String workersExcludeFileContent = FileUtils.readFileToString(workersExcludeFile, Charset.defaultCharset());
                webpackWorkersExcludesList = (List<String>) new JSONObject(workersExcludeFileContent);
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
        return fileName.substring(fileName.length() - 3, fileName.length()).equals(".js");
    }
}
