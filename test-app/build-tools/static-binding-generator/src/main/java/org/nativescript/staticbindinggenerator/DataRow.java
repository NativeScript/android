package org.nativescript.staticbindinggenerator;

import java.util.Arrays;

public class DataRow {
    private final String DELIMITER = "\\*";
    private final int ELEMENT_NUMBER = 9;
    private final String row;
    private String baseClassname;
    private String suffix;
    private String file;
    private String line;
    private String column;
    private String newClassName;
    private String[] methods;
    private String filename;
    private String jsFilename;
    private String[] interfaces;

    public DataRow(String row) {
        this.row = row;
        parse(this.row);
    }

    public String getRow() {
        return row;
    }

    public String getBaseClassname() {
        return baseClassname;
    }


    public String getSuffix() {
        return suffix;
    }

    public String getFile() {
        return file;
    }

    public String getLine() {
        return line;
    }

    public String getColumn() {
        return column;
    }

    public String getNewClassName() {
        return newClassName;
    }

    public String[] getMethods() {
        return methods;
    }

    public String getFilename() {
        return filename;
    }

    public String getJsFilename() {
        return jsFilename;
    }

    public String[] getInterfaces() {
        return interfaces;
    }

    private void parse(String row) {
        String[] data = row.split(DELIMITER, -1);
        if (data.length != ELEMENT_NUMBER) {
//            throw new IllegalArgumentException("Wrong number of elements " + data.length + " (Expected " + ELEMENT_NUMBER +")\n" + row);
            return;
        }

        baseClassname = data[0];
        file = data[1];
        line = data[2];
        column = data[3];

        int newClassNameIndex = 4;

        newClassName = data[newClassNameIndex];

        methods = data[newClassNameIndex + 1].split(",");
        filename = data[newClassNameIndex + 2];
        jsFilename = data[newClassNameIndex + 3];
        interfaces = data[newClassNameIndex + 4].split(",");
    }

    @Override
    public String toString() {
        return "DataRow{" +
                "DELIMITER='" + DELIMITER + '\'' +
                ", ELEMENT_NUMBER=" + ELEMENT_NUMBER +
                ", row='" + row + '\'' +
                ", baseClassname='" + baseClassname + '\'' +
                ", suffix='" + suffix + '\'' +
                ", file='" + file + '\'' +
                ", line='" + line + '\'' +
                ", column='" + column + '\'' +
                ", newClassName='" + newClassName + '\'' +
                ", methods=" + Arrays.toString(methods) +
                ", filename='" + filename + '\'' +
                ", jsFilename='" + jsFilename + '\'' +
                ", interfaces=" + Arrays.toString(interfaces) +
                '}';
    }
}
