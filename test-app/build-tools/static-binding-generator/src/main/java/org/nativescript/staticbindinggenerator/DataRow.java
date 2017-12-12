package org.nativescript.staticbindinggenerator;

public class DataRow {
    private final String DELIMITER = "\\*";
    private final int ELEMENT_NUMBER = 6;
    private final String row;
    private String baseClassname;
    private String suffix;
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
        suffix = data[1];
        methods = data[2].split(",");
        filename = data[3];
        jsFilename = data[4];
        interfaces = data[5].split(",");
    }
}
