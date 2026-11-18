package org.nativescript.staticbindinggenerator;

public class InputParameters {

    // if the flag is passed the deprecation warnings will not be suppressed
    private static final String SHOW_DEPRECATION_WARNINGS = "-show-deprecation-warnings";
    // if the flag is passed the generation will exit on error
    private static final String THROW_ON_ERROR = "-throw-on-error";
    // name classes of anonymous and named class extends after their content, not their call site
    private static final String CONTENT_KEYED_BINDINGS = "-content-keyed-bindings";

    private static InputParameters current = new InputParameters();

    private boolean showDeprecationWarnings;
    private boolean throwOnError;
    private boolean contentKeyedBindings;

    public InputParameters() {
        this.showDeprecationWarnings = false;
        this.throwOnError = false;
        this.contentKeyedBindings = false;
    }

    public void setContentKeyedBindings(boolean value) {
        this.contentKeyedBindings = value;
    }

    public boolean getContentKeyedBindings() {
        return contentKeyedBindings;
    }

    public void setShowDeprecationWarnings(boolean value) {
        this.showDeprecationWarnings = value;
    }

    public boolean getSuppressDeprecationWarnings() {
        return !showDeprecationWarnings;
    }

    public void setThrowOnError(boolean value) {
        this.throwOnError = value;
    }

    public boolean getThrowOnError() {
        return throwOnError;
    }

    public static void parseCommand(String[] args) {
        InputParameters inputParameters = new InputParameters();

        if (args != null) {
            for (int i = 0; i < args.length; i++) {
                String commandArg = args[i];

                if (commandArg.equals(SHOW_DEPRECATION_WARNINGS)) {
                    inputParameters.setShowDeprecationWarnings(true);
                }

                if (commandArg.equals(THROW_ON_ERROR)) {
                    inputParameters.setThrowOnError(true);
                }

                if (commandArg.equals(CONTENT_KEYED_BINDINGS)) {
                    inputParameters.setContentKeyedBindings(true);
                }
            }
        }

        InputParameters.current = inputParameters;
    }

    public static InputParameters getCurrent() {
        return InputParameters.current;
    }
}
