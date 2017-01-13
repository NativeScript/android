package org.nativescript.staticbindinggenerator;

import java.io.File;

public class Binding {
    private final File file;
    private final String content;
    private final String classname;

    public Binding(File file, String content, String classname) {
        this.file = file;
        this.content = content;
        this.classname = classname;
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

}
