package org.nativescript.staticbindinggenerator.nodejs.impl;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.nativescript.staticbindinggenerator.nodejs.NodeJSProcess;
import org.nativescript.staticbindinggenerator.system.environment.EnvironmentVariablesReader;
import org.nativescript.staticbindinggenerator.system.process.ProcessExecutor;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

public class NodeJSProcessImplTest {

    private static final String NODE_PROCESS_NAME = "node";
    private static final String TEST_SCRIPT_FILE_PATH = "testFile.js";
    private static final String TEST_USER_PROVIDED_NODE_OPTIONS_UNDERSCORED = "--max_old_space_size=1234";
    private static final String TEST_USER_PROVIDED_NODE_OPTIONS = "--max-old-space-size=1234";
    private static final String DEFAULT_HEAP_SIZE_ARGUMENT_UNDERSCORED = "--max_old_space_size=8192";
    private static final String DEFAULT_HEAP_SIZE_ARGUMENT = "--max-old-space-size=8192";
    private static final int TEST_EXIT_CODE = 666;

    @Mock
    private ProcessExecutor processExecutor;

    @Mock
    private EnvironmentVariablesReader environmentVariablesReader;

    private NodeJSProcess nodeJSProcess;

    @Before
    public void setUp() {
        initMocks(this);
        nodeJSProcess = new NodeJSProcessImpl(processExecutor, environmentVariablesReader);
    }

    @Test
    public void testProcessStartedWithoutDefaultHeapSizeIfUserProvidedNodeOptions() {
        when(environmentVariablesReader.readEnvironmentVariables()).thenReturn(createTestNodeOptionsEnvironmentVariable(TEST_USER_PROVIDED_NODE_OPTIONS));
        List<String> expectedProcessArguments = new ArrayList<>();
        expectedProcessArguments.add(TEST_SCRIPT_FILE_PATH);

        nodeJSProcess.runScript(TEST_SCRIPT_FILE_PATH);

        verify(environmentVariablesReader).readEnvironmentVariables();
        verify(processExecutor).executeProcess(NODE_PROCESS_NAME, expectedProcessArguments);
    }

    @Test
    public void testProcessStartedWithoutDefaultHeapSizeIfUserProvidedUnderscoredNodeOptions() {
        when(environmentVariablesReader.readEnvironmentVariables()).thenReturn(createTestNodeOptionsEnvironmentVariable(TEST_USER_PROVIDED_NODE_OPTIONS_UNDERSCORED));
        List<String> expectedProcessArguments = new ArrayList<>();
        expectedProcessArguments.add(TEST_SCRIPT_FILE_PATH);

        nodeJSProcess.runScript(TEST_SCRIPT_FILE_PATH);

        verify(environmentVariablesReader).readEnvironmentVariables();
        verify(processExecutor).executeProcess(NODE_PROCESS_NAME, expectedProcessArguments);
    }

    private Map<String, String> createTestNodeOptionsEnvironmentVariable(String nodeOptionsValue) {
        Map<String, String> mockedEnvironment = new HashMap<>();
        mockedEnvironment.put("NODE_OPTIONS", nodeOptionsValue);
        return mockedEnvironment;
    }

    @Test
    public void testProcessStartedWithDefaultHeapSizeIfUserDidNotProvideNodeOptions() {
        when(environmentVariablesReader.readEnvironmentVariables()).thenReturn(new HashMap<>());
        List<String> expectedProcessArguments = new ArrayList<>();
        expectedProcessArguments.add(DEFAULT_HEAP_SIZE_ARGUMENT);
        expectedProcessArguments.add(DEFAULT_HEAP_SIZE_ARGUMENT_UNDERSCORED);
        expectedProcessArguments.add(TEST_SCRIPT_FILE_PATH);

        nodeJSProcess.runScript(TEST_SCRIPT_FILE_PATH);

        verify(environmentVariablesReader).readEnvironmentVariables();
        verify(processExecutor).executeProcess(NODE_PROCESS_NAME, expectedProcessArguments);
    }

    @Test
    public void testProcessExitsWithCodeFromProcessExecutor() {
        when(processExecutor.executeProcess(any(), any())).thenReturn(TEST_EXIT_CODE);

        int actualExitCode = nodeJSProcess.runScript(TEST_SCRIPT_FILE_PATH);

        assertEquals("Unexpected status code returned", TEST_EXIT_CODE, actualExitCode);
    }


}