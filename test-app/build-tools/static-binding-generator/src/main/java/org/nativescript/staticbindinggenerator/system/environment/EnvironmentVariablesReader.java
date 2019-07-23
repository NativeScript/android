package org.nativescript.staticbindinggenerator.system.environment;

import java.util.Map;

public interface EnvironmentVariablesReader {
    /**
     * Reads the current process' environment variables
     *
     * @return the environment variables as name=value collection
     */
    Map<String, String> readEnvironmentVariables();
}
