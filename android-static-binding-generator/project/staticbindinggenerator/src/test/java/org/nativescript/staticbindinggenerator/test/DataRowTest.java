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

        Assert.assertEquals(row.getBaseClassname(), "java.lang.Runnable");
        Assert.assertEquals(row.getSuffix(), "_fapp_tns_modules_timer_timer_l14_c20__");
        Assert.assertEquals(row.getFilename(), "");
        Assert.assertEquals(row.getJsFilename(), "");
        String[] methods = row.getMethods();
        Assert.assertEquals(methods.length, 1);
        Assert.assertEquals(methods[0], "run");
    }

    @Test
    public void testParseNamedExtend() throws IOException {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");

        DataRow row = new DataRow(lines.get(0));

        Assert.assertEquals(row.getBaseClassname(), "java.lang.Object");
        Assert.assertEquals(row.getSuffix(), "_frnal_prepareExtend_l62_c37__HelloWorldModel");
        Assert.assertEquals(row.getFilename(), "a.b.c.MyObject");
        Assert.assertEquals(row.getJsFilename(), "some/full/path/myobj.js");
        String[] methods = row.getMethods();
        Assert.assertEquals(methods.length, 1);
        Assert.assertEquals(methods[0], "hashCode");
    }

    @Test
    public void testParseClassWithMultipleInterfaces() throws IOException {
        List<String> lines = Utils.getDataRowsFromResource("datarow-class-extends-interfaces.txt");

        DataRow row = new DataRow(lines.get(0));

        Assert.assertEquals(row.getBaseClassname(), "java.lang.Object");
        Assert.assertEquals(row.getSuffix(), "");
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
