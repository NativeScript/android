package com.tns;

import com.tns.bindings.AnnotationDescriptor;
import com.tns.bindings.Dump;
import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.MethodDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;
import com.tns.bindings.desc.reflection.MethodInfo;

import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.ow2.asmdex.ApplicationWriter;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;


public class DumpMethodTests {

    private Dump dump;
    private static HashMap<String, MethodDescriptor> nameToMethodDescriptor;
    private static ClassDescriptor testClassDescriptor;
    private static ApplicationWriter aw = new ApplicationWriter();
    private static HashSet<String> methodOverrides;
    private static HashSet<ClassDescriptor> overridenInterfaces;

    @Before
    public void instanceSetUp() {
        this.dump = new Dump();
    }

    @BeforeClass
    public static void setUp() {
        nameToMethodDescriptor = new HashMap<>();

        Method[] testClassMethods = TestClass.class.getDeclaredMethods();
        for (int i = 0; i < testClassMethods.length; i++) {
            MethodDescriptor md = new MethodInfo(testClassMethods[i]);
            nameToMethodDescriptor.put(md.getName(), md);
        }
        try {
            MethodDescriptor ctor = new MethodInfo(TestClass.class.getDeclaredConstructor(new Class[0]));
            nameToMethodDescriptor.put("constructor", ctor);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        testClassDescriptor = new ClassInfo(TestClass.class);

        aw.visit();

        methodOverrides = new HashSet<String>();
        methodOverrides.add("TestMethod");
        methodOverrides.add("TestMethod1");

        overridenInterfaces = new HashSet<ClassDescriptor>();
        overridenInterfaces.add(new ClassInfo(Interface1.class));
    }

    @Test
    public void getDexMethodDescriptor_method_returns_correct_result() throws NoSuchMethodException {
        MethodDescriptor mi = nameToMethodDescriptor.get("TestMethod");

        String result = this.dump.getDexMethodDescriptor(mi);
        String expected = "VLjava/lang/String;Ljava/lang/Integer;Ljava/lang/Object;";

        Assert.assertEquals(expected, result);
    }

    @Test
    public void getMethodOverloadDescriptor_method_returns_correct_result() throws NoSuchMethodException {
        MethodDescriptor mi = nameToMethodDescriptor.get("TestMethod");

        String result = this.dump.getMethodOverloadDescriptor(mi);
        String expected = "Ljava/lang/String;Ljava/lang/Integer;Ljava/lang/Object;";

        Assert.assertEquals(expected, result);
    }


    @Test
    public void getDexConstructorDescriptor_method_returns_correct_result() throws NoSuchMethodException {
        MethodDescriptor mi = nameToMethodDescriptor.get("constructor");

        String result = this.dump.getDexConstructorDescriptor(mi);
        String expected = "V";

        Assert.assertEquals(expected, result);
    }

    @Test
    public void getAsmDescriptor_method_returns_correct_result() throws NoSuchMethodException {
        String result = this.dump.getAsmDescriptor(testClassDescriptor);
        String expected = "Lcom/tns/TestClass;";

        Assert.assertEquals(expected, result);
    }




    @Test
    public void getClassSignatureOfType_method_returns_correct_result() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        final Object[] params = {testClassDescriptor };

        Method m = Dump.class.getDeclaredMethod("getClassSignatureOfType", ClassDescriptor.class);
        m.setAccessible(true);

        String result = (String)m.invoke(this.dump.getClass(), params);
        String expected = "Lcom/tns/TestClass;";

        Assert.assertEquals(expected, result);
    }

    @Test
    public void getMethodSignature_method_returns_correct_result() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Method m = Dump.class.getDeclaredMethod("getMethodSignature", MethodDescriptor.class);
        m.setAccessible(true);

        MethodDescriptor md = nameToMethodDescriptor.get("TestMethodNoParams");
        final Object[] params = {md};
        String result = (String)m.invoke(this.dump, params);
        String expected = "TestMethodNoParams()";

        MethodDescriptor md1 = nameToMethodDescriptor.get("TestMethod1");
        final Object[] params1 = {md1};
        String result1 = (String)m.invoke(this.dump, params1);
        String expected1 = "TestMethod1(java.lang.String)";

        Assert.assertEquals(expected1, result1);
        Assert.assertEquals(expected, result);
    }

    @Test
    public void collectAbstractMethods_method_returns_correct_result() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Method m = Dump.class.getDeclaredMethod("collectAbstractMethods", ClassDescriptor.class, List.class);
        m.setAccessible(true);

        MethodDescriptor md = nameToMethodDescriptor.get("TestMethodNoParams");
        Object[] params= new Object[2];
        params[0] = new ClassInfo(Interface1.class);
        ArrayList<MethodDescriptor> mds = new ArrayList<>();
        params[1] = mds;

        m.invoke(this.dump, params);
        Assert.assertTrue(mds.size() > 0);

        String expected = mds.get(0).getName();
        String result = "methodFromInterface";

        Assert.assertEquals(expected, result);
    }

    @Test
    public void getSupportedMethods_method_returns_correct_result() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {

        Method m = Dump.class.getDeclaredMethod("getSupportedMethods", ClassDescriptor.class, HashSet.class, HashSet.class);
        m.setAccessible(true);

        Object[] params= new Object[3];
        params[0] = testClassDescriptor;
        HashSet<String> methodOverrides = new HashSet<>();
        methodOverrides.add("TestMethod");
        methodOverrides.add("NonExistentMethod"); //will be ignored because it's not part of TestClass.class
        params[1] = methodOverrides;
        HashSet<ClassDescriptor> interfacesToImplement = new HashSet<>();
        interfacesToImplement.add(new ClassInfo(Interface1.class));
        params[2] = interfacesToImplement;

        MethodDescriptor[] methodDescriptors = (MethodDescriptor[])m.invoke(this.dump, params);
        Assert.assertTrue(methodDescriptors.length == 2);

        String interfaceMethodName = methodDescriptors[0].getName();
        String TestMethodName= methodDescriptors[1].getName();

        String expectedInterfaceName = "methodFromInterface";
        String expectedTestMethodName = "TestMethod";

        Assert.assertEquals(expectedInterfaceName, interfaceMethodName);
        Assert.assertEquals(expectedTestMethodName, TestMethodName);
    }

    @Test
    public void isMethodSupported_method_returns_correct_result() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {

        Method m = Dump.class.getDeclaredMethod("isMethodSupported", MethodDescriptor.class, HashMap.class);
        m.setAccessible(true);

        Object[] params= new Object[2];
        params[0] = nameToMethodDescriptor.get("TestMethodNoParams");
        HashMap<String, MethodDescriptor> finalMethods = new HashMap<>();
        params[1] = finalMethods;

        boolean isSupportedMethod = (boolean)m.invoke(Dump.class, params);
        Assert.assertTrue(isSupportedMethod);

        MethodInfo mi = null;
        for (Method m1: AbstractClass.class.getDeclaredMethods()) {
            if (m1.getName().equals("PrivateMethod")) {
                mi = new MethodInfo(m1);
                break;
            }
        }
        params[0] = mi;
        isSupportedMethod = (boolean)m.invoke(Dump.class, params);
        Assert.assertFalse(isSupportedMethod);
    }
}
