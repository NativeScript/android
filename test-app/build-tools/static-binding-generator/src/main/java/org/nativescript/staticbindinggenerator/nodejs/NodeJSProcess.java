package org.nativescript.staticbindinggenerator.nodejs;

public interface NodeJSProcess {
    /**
     * Starts a Node process to execute a JS file
     *
     * @param scriptPath path to the JS file
     * @return returns the Node process' exit code
     */
    int runScript(String scriptPath);
}
