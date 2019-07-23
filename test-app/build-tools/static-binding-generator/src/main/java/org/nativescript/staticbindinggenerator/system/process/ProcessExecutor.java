package org.nativescript.staticbindinggenerator.system.process;

import java.util.List;

public interface ProcessExecutor {

    /**
     * Executes a process by a given name or path with specified arguments
     *
     * @param processNameOrPath process name or path
     * @param arguments         arguments to pass to the process
     * @return status code of the executed process
     * @throws ProcessExecutionException if no process name/path given or InterruptedException/IOException occurs
     */
    int executeProcess(String processNameOrPath, List<String> arguments);
}
