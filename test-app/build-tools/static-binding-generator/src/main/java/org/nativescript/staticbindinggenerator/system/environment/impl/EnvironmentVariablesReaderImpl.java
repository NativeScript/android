package org.nativescript.staticbindinggenerator.system.environment.impl;

import org.nativescript.staticbindinggenerator.system.environment.EnvironmentVariablesReader;

import java.util.Map;

public class EnvironmentVariablesReaderImpl implements EnvironmentVariablesReader {

    @Override
    public Map<String, String> readEnvironmentVariables() {
        return System.getenv();
    }

}
