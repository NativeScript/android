package org.nativescript.staticbindinggenerator.test;

import org.junit.Assert;
import org.junit.Test;
import org.nativescript.staticbindinggenerator.DataRow;

import java.io.IOException;
import java.util.List;

public class DataRowTest {
    @Test
    public void testParseInterface() throws IOException {
        List<String> lines = Utils.getDataRowsFromResource("datarow-interface.txt");

        DataRow row = new DataRow(lines.get(0));

        Assert.assertEquals(row.getBaseClassname(), "android.view.View.OnClickListener");
        Assert.assertEquals(row.getFile(), "MyActivity");
        Assert.assertEquals(row.getLine(), "54");
        Assert.assertEquals(row.getColumn(), "32");
        Assert.assertEquals(row.getNewClassName(), "AppClickListener");
        Assert.assertEquals(row.getMethods()[0], "onClick");
        Assert.assertEquals(row.getFilename(), "");
        Assert.assertEquals(row.getJsFilename(), "");
        Assert.assertEquals(row.getInterfaces()[0], "");
    }

    @Test
    public void testParseNamedExtend() throws IOException {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");

        DataRow row = new DataRow(lines.get(0));

        Assert.assertEquals(row.getBaseClassname(), "java.lang.Object");
        Assert.assertEquals(row.getFile(), "");
        Assert.assertEquals(row.getLine(), "");
        Assert.assertEquals(row.getColumn(), "");
        Assert.assertEquals(row.getNewClassName(), "");
        Assert.assertEquals(row.getMethods()[0], "toString");
        Assert.assertEquals(row.getFilename(), "com.tns.NativeScriptActivity");
        Assert.assertEquals(row.getJsFilename(), "MyActivity.js");
        Assert.assertEquals(row.getInterfaces()[0], "");
    }

    @Test
    public void testParseClassWithMultipleInterfaces() throws IOException {
        List<String> lines = Utils.getDataRowsFromResource("datarow-class-extends-interfaces.txt");

        DataRow row = new DataRow(lines.get(0));

        Assert.assertEquals(row.getBaseClassname(), "java.lang.Object");
        Assert.assertEquals(row.getFilename(), "com.tns.ComplexClass");
        Assert.assertEquals(row.getJsFilename(), "app.js");

        String[] methods = row.getMethods();
        Assert.assertEquals(9, methods.length);

        String[] interfaces = row.getInterfaces();
        Assert.assertEquals(4, interfaces.length);

        Assert.assertEquals(interfaces[0], "java.util.jar.Pack200.Unpacker");
        Assert.assertEquals(interfaces[2], "java.util.Observer");
    }
}
