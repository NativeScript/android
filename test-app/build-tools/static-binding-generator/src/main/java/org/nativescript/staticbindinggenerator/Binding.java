package org.nativescript.staticbindinggenerator;

import java.io.File;

public class Binding {
    private final File file;
    private final String content;
    private final String classname;
    private final String source;
    private final boolean involvesAppClasses;

    public Binding(File file, String content, String classname) {
        this(file, content, classname, "");
    }

    public Binding(File file, String content, String classname, String source) {
        this(file, content, classname, source, false);
    }

    public Binding(File file, String content, String classname, String source, boolean involvesAppClasses) {
        this.file = file;
        this.content = content;
        this.classname = classname;
        this.source = source;
        this.involvesAppClasses = involvesAppClasses;
    }

    /** Whether the binding's class, interfaces or their supertypes include a class of the app module. */
    public boolean involvesAppClasses() {
        return involvesAppClasses;
    }

    public File getFile() {
        return file;
    }

    public String getContent() {
        return content;
    }

    public String getClassname() {
        return classname;
    }

    /** The extend() call site the binding was generated for, for messages. */
    public String getSource() {
        return source;
    }

}
