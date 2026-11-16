package org.nativescript.staticbindinggenerator;

import java.io.File;

public class Binding {
    private final File file;
    private final String content;
    private final String classname;
    private final String source;

    public Binding(File file, String content, String classname) {
        this(file, content, classname, "");
    }

    public Binding(File file, String content, String classname, String source) {
        this.file = file;
        this.content = content;
        this.classname = classname;
        this.source = source;
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
