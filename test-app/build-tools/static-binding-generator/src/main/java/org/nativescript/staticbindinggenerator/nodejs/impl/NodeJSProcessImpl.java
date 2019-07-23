package org.nativescript.staticbindinggenerator.nodejs.impl;

import org.nativescript.staticbindinggenerator.nodejs.NodeJSProcess;
import org.nativescript.staticbindinggenerator.system.environment.EnvironmentVariablesReader;
import org.nativescript.staticbindinggenerator.system.process.ProcessExecutor;

import java.util.ArrayList;
import java.util.List;

public class NodeJSProcessImpl implements NodeJSProcess {

    private static final String NODE_PROCESS_NAME = "node";
    private static final String NODE_OPTIONS_VARIABLE = "NODE_OPTIONS";
    private static final String MAX_OLD_SPACE_SIZE_PROPERTY_NAME = "max-old-space-size";
    private static final String MAX_OLD_SPACE_SIZE_PROPERTY_NAME_UNDERSCORED = "max_old_space_size";
    private static final String MAX_OLD_SPACE_SIZE_DEFAULT_PROPERTY = "--" + MAX_OLD_SPACE_SIZE_PROPERTY_NAME + "=8192";
    private static final String MAX_OLD_SPACE_SIZE_DEFAULT_PROPERTY_UNDERSCORED = "--" + MAX_OLD_SPACE_SIZE_PROPERTY_NAME_UNDERSCORED + "=8192";

    private final ProcessExecutor processExecutor;
    private final EnvironmentVariablesReader environmentVariablesReader;

    public NodeJSProcessImpl(ProcessExecutor processExecutor, EnvironmentVariablesReader environmentVariablesReader) {
        this.processExecutor = processExecutor;
        this.environmentVariablesReader = environmentVariablesReader;
    }

    @Override
    public int runScript(String scriptPath) {
        List<String> parameters = createNodeJSParameters(scriptPath);
        return processExecutor.executeProcess(NODE_PROCESS_NAME, parameters);
    }

    private List<String> createNodeJSParameters(String scriptPath) {
        List<String> parameters = new ArrayList<>();

        if (!hasUserProvidedV8HeapSizeProperty()) {
            parameters.add(MAX_OLD_SPACE_SIZE_DEFAULT_PROPERTY);
            parameters.add(MAX_OLD_SPACE_SIZE_DEFAULT_PROPERTY_UNDERSCORED);
        }

        parameters.add(scriptPath);

        return parameters;
    }

    private boolean hasUserProvidedV8HeapSizeProperty() {
        return environmentVariablesReader.readEnvironmentVariables()
                .entrySet()
                .stream()
                .anyMatch(x -> x.getKey().equals(NODE_OPTIONS_VARIABLE)
                        && (x.getValue().contains(MAX_OLD_SPACE_SIZE_PROPERTY_NAME)
                            || x.getValue().contains(MAX_OLD_SPACE_SIZE_PROPERTY_NAME_UNDERSCORED)));
    }


}
