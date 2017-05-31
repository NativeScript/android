package org.nativescript.staticbindinggenerator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

public class Generator {
    private static final String JAVA_EXT = ".java";

    private static final String CLASS_EXT = ".class";

    private static final String DEFAULT_PACKAGE_NAME = "com.tns.gen";

    private final String outputDir;
    private final String[] libs;
    private final Map<String, JavaClass> classes;

    public Generator(String outputDir, String[] libs) throws IOException {
        this(outputDir, libs, false);
    }

    public Generator(String outputDir, String[] libs, boolean throwOnError) throws IOException {
        this.outputDir = outputDir;
        this.libs = libs;
        this.classes = readClasses(libs, throwOnError);
    }


    public void writeBindings(String filename) throws IOException, ClassNotFoundException {
        Binding[] bindings = generateBindings(filename);
        Set<File> writtenFiles = new HashSet<File>();
        for (Binding b : bindings) {
            if (writtenFiles.add(b.getFile())) {
                try (PrintStream ps = new PrintStream(b.getFile())) {
                    ps.append(b.getContent());
                }
            } else {
                throw new IOException("File already exists. This may lead to undesired behavior.\nPlease change the name of one of the extended classes.\n" + b.getFile());
            }
        }
    }

    public Binding[] generateBindings(String filename) throws IOException, ClassNotFoundException {
        List<DataRow> rows = getRows(filename);

        Binding[] generatedFiles = processRows(rows);

        return generatedFiles;
    }

    public Binding generateBinding(DataRow dataRow, HashSet interfaceNames) throws ClassNotFoundException {
        JavaClass clazz = getClass(dataRow.getBaseClassname());

        boolean hasSpecifiedName = !dataRow.getFilename().isEmpty();
        String packageName = hasSpecifiedName ? getBaseDir(dataRow.getFilename()) : (DEFAULT_PACKAGE_NAME + "." + clazz.getPackageName());
        String baseDirPath = packageName.replace('.', '/');

        File baseDir = new File(outputDir, baseDirPath);
        if (!baseDir.exists()) {
            boolean success = baseDir.mkdirs();
        }

        String name;
        Boolean isInterface = clazz.isInterface();

        if (hasSpecifiedName) {
            name = getSimpleClassname(dataRow.getFilename());
        } else {
            name = getSimpleClassname(clazz.getClassName());

            if (!isInterface) {
                name += dataRow.getSuffix();
            }
        }

        if (isInterface && interfaceNames.contains(name)) {
            return null;
        } else if (isInterface) {
            interfaceNames.add(name);
        }

        String normalizedName = getNormalizedName(name);

        Writer w = new Writer();

        writeBinding(w, dataRow, clazz, packageName, name);

        String classname = dataRow.getFilename();

        return new Binding(new File(baseDir, normalizedName + JAVA_EXT), w.toString(), classname);
    }

    public Binding generateBinding(DataRow dataRow) throws ClassNotFoundException {
        return generateBinding(dataRow, new HashSet());
    }

    private List<DataRow> getRows(String filename) throws IOException {
        List<DataRow> rows = new ArrayList<DataRow>();
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(filename)));
            String line;
            while ((line = br.readLine()) != null) {
                DataRow row = new DataRow(line);
                rows.add(row);
            }
        } finally {
            if (br != null) {
                br.close();
            }
        }
        return rows;
    }

    private Binding[] processRows(List<DataRow> rows) throws IOException, ClassNotFoundException {
        ArrayList<Binding> bindings = new ArrayList<>();
        HashSet interfaceNames = new HashSet();

        for (DataRow dataRow : rows) {
            String classname = dataRow.getBaseClassname();
            boolean isJavaExtend = classes.containsKey(classname);
            if (isJavaExtend) {

//                System.out.println("SBG: DataRow: baseClassName: " + classname + ", suffix: " + dataRow.getSuffix() + ", interfaces: " + String.join(", ", Arrays.asList(dataRow.getInterfaces())) + ", jsFileName: " + dataRow.getJsFilename());

                Binding binding = generateBinding(dataRow, interfaceNames);

                if (binding != null) {
                    bindings.add(binding);
                }
            }
        }

        return bindings.toArray(new Binding[bindings.size()]);
    }

    private void collectImplementedInterfaces(String[] interfaces, JavaClass clazz) {
        String[] implInterfaces = interfaces;
        if (implInterfaces.length > 0 && !implInterfaces[0].isEmpty()) {
            // since JavaClass.setInterfaceNames overwrites all interfaces, we need to preserve any
            // original interfaces implemented by the class/interface
            ArrayList<String> interfacesList = new ArrayList<String>();
            String[] nativeInterfaces = clazz.getInterfaceNames();
            if (nativeInterfaces.length > 0) {
                for (String intface : nativeInterfaces) {
                    interfacesList.add(intface);
                }
            }

            for (String intface : implInterfaces) {
                interfacesList.add(intface);
            }

            String[] arr = interfacesList.toArray(new String[interfacesList.size()]);
            clazz.setInterfaceNames(arr);
        }
    }

    private String getNormalizedName(String filename) {
        StringBuilder sb = new StringBuilder(filename.length());
        for (char ch : filename.toCharArray()) {
            char c = Character.isJavaIdentifierPart(ch) ? ch : '_';
            sb.append(c);
        }

        return sb.toString();
    }

    private Map<String, List<Method>> getPublicApi(JavaClass clazz) throws ClassNotFoundException {
        Map<String, List<Method>> api = new HashMap<String, List<Method>>();
        JavaClass currentClass = clazz;
        String clazzName = clazz.getClassName();
        while (true) {
            String currentClassname = currentClass.getClassName();

            boolean shouldCollectMethods = !(!clazzName.equals(currentClassname) && currentClass.isAbstract());

            if (shouldCollectMethods || currentClass.isInterface()) {
                // Don't include abstract parent class's methods to avoid compilation issues
                // where a child class has 2 methods, of the same type, with just a
                // return type/parameter type that differs by being of a superclass of the class being extended.
                // see Test testCanCompileBindingClassExtendingAnExtendedClassWithMethodsWithTheSameSignature
                List<Method> methods = new ArrayList<Method>();
                for (Method m : currentClass.getMethods()) {
                    methods.add(m);
                }

//                System.out.println("SBG: getPublicApi:collectInterfaceMethods classname: " + currentClassname);

                collectInterfaceMethods(clazz, methods);
                for (Method m : methods) {
                    if (!m.isSynthetic() && (m.isPublic() || m.isProtected()) && !m.isStatic()) {
                        String name = m.getName();

                        List<Method> methodGroup;
                        if (api.containsKey(name)) {
                            methodGroup = api.get(name);
                        } else {
                            methodGroup = new ArrayList<Method>();
                            api.put(name, methodGroup);
                        }
                        boolean found = false;
                        String methodSig = m.getSignature();
                        for (Method m1 : methodGroup) {
                            found = methodSig.equals(m1.getSignature());
                            if (found) {
                                break;
                            }
                        }
                        if (!found) {
                            methodGroup.add(m);
                        }
                    }
                }
            }

            if (currentClassname.equals("java.lang.Object")) {
                break;
            } else {
                String superClassName = currentClass.getSuperclassName();
                currentClass = getClass(superClassName);
            }
        }
        return api;
    }

    private Map<String, JavaClass> readClasses(String[] libs, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> map = new HashMap<String, JavaClass>();
        if (libs != null) {
            for (String lib : libs) {
                File f = new File(lib);
                Map<String, JavaClass> classes = f.isFile() ? readJar(lib, throwOnError) : readDir(lib, throwOnError);
                map.putAll(classes);
            }
        }
        return map;
    }

    private Map<String, JavaClass> readJar(String path, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();
        JarInputStream jis = null;
        try {
            String name = null;
            jis = new JarInputStream(new FileInputStream(path));
            for (ZipEntry ze = jis.getNextEntry(); ze != null; ze = jis.getNextEntry()) {
                try {
                    name = ze.getName();
                    if (name.endsWith(CLASS_EXT)) {
                        name = name.substring(0, name.length() - CLASS_EXT.length()).replace('/', '.').replace('$', '.');
                        ClassParser cp = new ClassParser(jis, name);
                        JavaClass clazz = cp.parse();
                        classes.put(name, clazz);
                    }
                } catch (Exception e) {
                    if (throwOnError) {
                        throw new RuntimeException(e);
                    }
                }
            }
        } finally {
            if (jis != null) {
                jis.close();
            }
        }
        return classes;
    }

    private Map<String, JavaClass> readDir(String path, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();

        ArrayDeque<File> d = new ArrayDeque<File>();
        d.add(new File(path));

        while (!d.isEmpty()) {
            File cur = d.pollFirst();
            File[] files = cur.listFiles();
            for (File f: files) {
                if (f.isFile() && f.getName().endsWith(CLASS_EXT)) {
                    ClassParser cp = new ClassParser(f.getAbsolutePath());
                    JavaClass clazz = cp.parse();
                    String name = clazz.getClassName();
                    name = name.replace('/', '.').replace('$', '.');
                    classes.put(name, clazz);
                } else if (f.isDirectory()) {
                    d.addLast(f);
                }
            }
        }

        return classes;
    }

    private String getBaseDir(String classname) {
        int idx = classname.lastIndexOf('.');
        String baseDir = classname.substring(0, idx);
        return baseDir;
    }

    private String getSimpleClassname(String classname) {
        int idx = classname.lastIndexOf('.');
        String name = classname.substring(idx + 1, classname.length()).replace("$", "_");
        return name;
    }

    private String getFullMethodSignature(Method m) {
        String sig = m.getName() + m.getSignature();
        return sig;
    }

    private void writeBinding(Writer w, DataRow dataRow, JavaClass clazz, String packageName, String name) throws ClassNotFoundException {
        String[] implInterfaces = dataRow.getInterfaces();
        collectImplementedInterfaces(implInterfaces, clazz);

        Map<String, List<Method>> api = getPublicApi(clazz);

        w.writeln("package " + packageName + ";");
        w.writeln();

        boolean isApplicationClass = isApplicationClass(clazz, classes);
        if (isApplicationClass && !packageName.equals("com.tns")) {
            w.writeln("import com.tns.RuntimeHelper;");
            w.writeln("import com.tns.Runtime;");
            w.writeln();
        }

        boolean hasSpecifiedName = !dataRow.getFilename().isEmpty();
        if (hasSpecifiedName) {
            w.writeln("@com.tns.JavaScriptImplementation(javaScriptFile = \"./" + dataRow.getJsFilename() + "\")");
        }
        w.write("public class " + name);
        boolean isInterface = clazz.isInterface();
        String extendKeyword = isInterface ? " implements " : " extends ";
        w.write(extendKeyword);
        w.write(clazz.getClassName().replace('$', '.'));
        if (!isInterface) {
            w.write(" implements");
            w.write(" com.tns.NativeScriptHashCodeProvider");

            if (implInterfaces.length > 0 && !implInterfaces[0].isEmpty()) {
                for (String intface : implInterfaces) {
                    w.write(", " + intface);
                }
            }
        }
        w.writeln(" {");

        if (isClassApplication(clazz)) {
            //get instance method
            w.write("\t");
            w.writeln("private static " + clazz.getClassName().replace('$', '.') + " thiz;");
            w.writeln();
        }

        boolean hasInitMethod = false;
        String[] methods = dataRow.getMethods();
        for (String m : methods) {
            hasInitMethod = m.equals("init");
            if (hasInitMethod) {
                break;
            }
        }

        boolean hasInitMethod2 = !isApplicationClass && hasInitMethod;
        writeConstructors(clazz, name, hasInitMethod2, isApplicationClass, w);

        if (isInterface) {
            Set<String> objectMethods = new HashSet<String>();
            for (Method objMethod : getClass("java.lang.Object").getMethods()) {
                if (!objMethod.isStatic() && (objMethod.isPublic() || objMethod.isProtected())) {
                    String sig = getFullMethodSignature(objMethod);
                    objectMethods.add(sig);
                }
            }
            Set<Method> notImplementedObjectMethods = new HashSet<Method>();
            Method[] currentIfaceMethods = clazz.getMethods();
            ArrayList<Method> ifaceMethods = new ArrayList<Method>();
            for (Method m : currentIfaceMethods) {
                if (!m.getName().equals("<clinit>")) {
                    ifaceMethods.add(m);
                }
            }

            ArrayDeque<String> interfaceNames = new ArrayDeque<String>();
            for (String iname : clazz.getInterfaceNames()) {
                interfaceNames.add(iname);
            }
            while (!interfaceNames.isEmpty()) {
                String iname = interfaceNames.pollFirst();
                JavaClass iface = getClass(iname);
                for (String iname2 : iface.getInterfaceNames()) {
                    interfaceNames.add(iname2.replace('$', '.'));
                }
                Method[] ims = iface.getMethods();
                for (Method m : ims) {
                    ifaceMethods.add(m);
                }
            }

            Set<String> methodOverrides = new HashSet<String>();
            for (String methodName : dataRow.getMethods()) {
                methodOverrides.add(methodName);
            }
            for (Method ifaceMethod : ifaceMethods) {
                if (!ifaceMethod.isStatic()) {
                    String sig = getFullMethodSignature(ifaceMethod);
                    if (objectMethods.contains(sig) && !methodOverrides.contains(ifaceMethod.getName())) {
                        notImplementedObjectMethods.add(ifaceMethod);
                    }
                }
            }
            for (Method m : ifaceMethods) {
                if (!notImplementedObjectMethods.contains(m)) {
                    writeMethodBody(m, w, isApplicationClass, true);
                }
            }
        } else {
            List<Method> interfaceMethods = new ArrayList<Method>();
            collectInterfaceMethods(clazz, interfaceMethods);
            for (String methodName : dataRow.getMethods()) {
                if (api.containsKey(methodName)) {
                    List<Method> methodGroup = api.get(methodName);
                    for (Method m : methodGroup) {
                        boolean isInterfaceMethod = false;
                        if (interfaceMethods.contains(m)) {
                            isInterfaceMethod = true;
                        }
                        writeMethodBody(m, w, isApplicationClass, isInterfaceMethod);
                    }
                }
            }
        }
        if (!isInterface) {
            writeHashCodeProviderImplementationMethods(w);
        }

        if (isClassApplication(clazz)) {
            w.write("\t");
            w.write("public static ");
            w.write(clazz.getClassName().replace('$', '.'));
            w.writeln(" getInstance() {");
            w.write("\t\t");
            w.writeln("return thiz;");
            w.write("\t");
            w.writeln("}");
        }

        w.writeln("}");
    }

    private boolean isClassApplication(JavaClass clazz) {
        String className = clazz.getClassName();
        return className.equals("android.app.Application") ||
               className.equals("android.support.multidex.MultiDexApplication") ||
               className.equals("android.test.mock.MockApplication");
    }

    private void writeMethodBody(Method m, Writer w, boolean isApplicationClass, boolean isInterfaceMethod) {
        String visibility = m.isPublic() ? "public" : "protected";
        w.write("\t");
        w.write(visibility);
        w.write(" ");
        writeType(m.getReturnType(), w);
        w.write(" ");
        w.write(m.getName());
        writeMethodSignature(m, w);
        w.write(" ");
        writeThrowsClause(m, w);
        w.writeln(" {");
        writeMethodBody(m, false, isApplicationClass, w, isInterfaceMethod);
        w.writeln("\t}");
        w.writeln();
    }

    private void writeHashCodeProviderImplementationMethods(Writer w) {
        w.write("\t");
        w.writeln("public boolean equals__super(java.lang.Object other) {");
        w.write("\t\t");
        w.writeln("return super.equals(other);");
        w.write("\t");
        w.writeln("}");
        w.writeln();

        w.write("\t");
        w.writeln("public int hashCode__super() {");
        w.write("\t\t");
        w.writeln("return super.hashCode();");
        w.write("\t");
        w.writeln("}");
        w.writeln();
    }

    private void writeMethodSignature(Method m, Writer w) {
        w.write('(');
        Type[] args = m.getArgumentTypes();
        for (int i = 0; i < args.length; i++) {
            if (i > 0) {
                w.write(", ");
            }
            writeType(args[i], w);
            w.write(" param_");
            w.write(i);
        }
        w.write(')');
    }

    private void writeThrowsClause(Method m, Writer w) {
    }

    private void writeConstructors(JavaClass clazz, String classname, boolean hasInitMethod, boolean isApplicationClass, Writer w) {
        boolean isInterface = clazz.isInterface();
        if (isInterface) {
            w.write("\tpublic ");
            w.write(classname);
            w.writeln("() {");
            w.writeln("\t\tcom.tns.Runtime.initInstance(this);");
            w.writeln("\t}");
            w.writeln();
        } else {
            List<Method> ctors = new ArrayList<Method>();
            for (Method m : clazz.getMethods()) {
                if (m.getName().equals("<init>")) {
                    ctors.add(m);
                }
            }
            for (Method c : ctors) {
                if (c.isPublic() || c.isProtected()) {
                    String visibility = c.isPublic() ? "public" : "protected";
                    w.write("\t");
                    w.write(visibility);
                    w.write(" ");
                    w.write(classname);
                    writeMethodSignature(c, w);
                    w.writeln("{");
                    w.write("\t\tsuper(");
                    Type[] ctorArgs = c.getArgumentTypes();
                    for (int i = 0; i < ctorArgs.length; i++) {
                        if (i > 0) {
                            w.write(", ");
                        }
                        w.write("param_");
                        w.write(i);
                    }
                    w.writeln(");");
                    if (!isApplicationClass) {
                        w.writeln("\t\tcom.tns.Runtime.initInstance(this);");
                    }
                    if (hasInitMethod) {
                        writeMethodBody(c, true, false, w, false);
                    }
                    if (isClassApplication(clazz)) {
                        //get instance method
                        w.write("\t\t");
                        w.writeln("thiz = this;");
                    }
                    w.writeln("\t}");
                    w.writeln();
                }
            }
        }
    }

    private void writeMethodBody(Method m, boolean isConstructor, boolean isApplicationClass, Writer w, boolean isInterfaceMethod) {
        if (m.getName().equals("onCreate") && isApplicationClass) {
            w.writeln("\t\tcom.tns.Runtime runtime = RuntimeHelper.initRuntime(this);");
        }
        if (isApplicationClass && !isInterfaceMethod) {
            w.writeln("\t\tif (!Runtime.isInitialized()) {");
            boolean isVoid = m.getReturnType().equals(Type.VOID);
            w.write("\t\t\t");
            if (!isVoid) {
                w.write("return ");
            }
            w.write("super." + m.getName() + "(");
            int paramCount = m.getArgumentTypes().length;
            for (int i = 0; i < paramCount; i++) {
                if (i > 0) {
                    w.write(", ");
                }
                w.write("param_" + i);
            }
            w.writeln(");");
            if (isVoid) {
                w.writeln("\t\t\treturn;");
            }
            w.writeln("\t\t}");
        }
        Type[] args = m.getArgumentTypes();
        int argLen = args.length + (isConstructor ? 1 : 0);
        w.write("\t\tjava.lang.Object[] args = ");
        if (argLen == 0) {
            w.writeln("null;");
        } else {
            w.write("new java.lang.Object[");
            w.write(argLen);
            w.writeln("];");
        }
        for (int i = 0; i < args.length; i++) {
            w.write("\t\targs[");
            w.write(i);
            w.write("] = param_");
            w.write(i);
            w.writeln(";");
        }
        String name = isConstructor ? "init" : m.getName();
        if (name.equals("init")) {
            w.write("\t\targs[");
            w.write(argLen - 1);
            w.write("] = ");
            w.write(isConstructor);
            w.writeln(";");
        }
        w.write("\t\t");
        Type ret = m.getReturnType();
        if (!ret.equals(Type.VOID)) {
            w.write("return (");
            writeType(ret, w);
            w.write(')');
        }
        w.write("com.tns.Runtime.callJSMethod(this, \"");
        w.write(name);
        w.write("\", ");
        writeType(ret, w);
        w.writeln(".class, args);");
        if (m.getName().equals("onCreate") && isApplicationClass) {
            w.writeln("\t\tif (runtime != null) {");
            w.writeln("\t\t\truntime.run();");
            w.writeln("\t\t}");
        }
    }

    private void writeType(Type t, Writer w) {
        String type = t.toString().replace('$', '.');
        w.write(type);
    }

    private void collectInterfaceMethods(JavaClass clazz, List<Method> methods) throws ClassNotFoundException {
        JavaClass currentClass = clazz;

        while (true) {
            String currentClassname = currentClass.getClassName();

            Queue<String> queue = new ArrayDeque<String>();
            for (String name : clazz.getInterfaceNames()) {
                queue.add(name);
            }

            while (!queue.isEmpty()) {
                String ifaceName = queue.poll();
                JavaClass currentInterface = getClass(ifaceName);
                Method[] ifaceMethods = currentInterface.getMethods();
                for (Method m : ifaceMethods) {
                    methods.add(m);
                }
                for (String name : currentInterface.getInterfaceNames()) {
                    queue.add(name);
                }
            }

            if (currentClassname.equals("java.lang.Object")) {
                break;
            } else {
                String superClassName = currentClass.getSuperclassName();
                currentClass = getClass(superClassName);
            }
        }
    }

    private boolean isApplicationClass(JavaClass clazz, Map<String, JavaClass> classes) throws ClassNotFoundException {
        boolean isApplicationClass = false;

        String applicationClassname = "android.app.Application";

        JavaClass currentClass = clazz;
        while (true) {
            String currentClassname = currentClass.getClassName();

            isApplicationClass = currentClassname.equals(applicationClassname);
            if (isApplicationClass) {
                break;
            }

            if (currentClassname.endsWith("java.lang.Object")) {
                break;
            }

            String superClassName = currentClass.getSuperclassName();
            currentClass = getClass(superClassName);
        }

        return isApplicationClass;
    }

    private JavaClass getClass(String className) throws ClassNotFoundException {
        JavaClass clazz = classes.get(className.replace('$', '.'));

        if (clazz == null) {
            throw new ClassNotFoundException("Class: " + className);
        }

        return clazz;
    }
}
