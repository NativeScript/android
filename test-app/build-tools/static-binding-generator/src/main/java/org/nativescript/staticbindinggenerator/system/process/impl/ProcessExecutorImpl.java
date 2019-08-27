package org.nativescript.staticbindinggenerator.system.process.impl;

import com.google.common.base.Strings;

import org.nativescript.staticbindinggenerator.system.process.ProcessExecutionException;
import org.nativescript.staticbindinggenerator.system.process.ProcessExecutor;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ProcessExecutorImpl implements ProcessExecutor {

    @Override
    public int executeProcess(String processNameOrPath, List<String> arguments) {
        if (Strings.isNullOrEmpty(processNameOrPath)) {
            throw new ProcessExecutionException("Missing process name!");
        }

        List<String> processBuilderArguments = new ArrayList<>();
        processBuilderArguments.add(processNameOrPath);
        processBuilderArguments.addAll(arguments);

        ProcessBuilder processBuilder = new ProcessBuilder(processBuilderArguments);
        processBuilder.redirectOutput(ProcessBuilder.Redirect.INHERIT);
        processBuilder.redirectError(ProcessBuilder.Redirect.INHERIT);

        try {
            Process p = processBuilder.start();
            p.waitFor();
            return p.exitValue();
        } catch (InterruptedException | IOException e) {
            throw new ProcessExecutionException(e);
        }
    }
}
