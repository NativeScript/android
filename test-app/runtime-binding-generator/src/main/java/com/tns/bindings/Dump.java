package com.tns.bindings;

import java.util.*;

import org.ow2.asmdex.*;
import org.ow2.asmdex.structureCommon.*;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.Descriptor;
import com.tns.bindings.desc.MethodDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;
import com.tns.bindings.desc.reflection.MethodInfo;

public class Dump {
    public static final char CLASS_NAME_LOCATION_SEPARATOR = '_';

    private static final String callJsMethodSignatureCtor = "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Z[Ljava/lang/Object;";
    private static final String callJsMethodSignatureMethod = "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/Class;[Ljava/lang/Object;";
    private static final String LCOM_TNS = "Lcom/tns/gen/";
    private static final String LCOM_TNS_RUNTIME = "Lcom/tns/Runtime;";
    static final String objectClass = "Ljava/lang/Object;";

    static final String runtimeClass = LCOM_TNS_RUNTIME;
    static final String callJSMethodName = "callJSMethod";
    static final String initInstanceMethodName = "initInstance";

    static final StringBuffer methodDescriptorBuilder = new StringBuffer();


    /**
     * Returns the dex descriptor corresponding to the given method.
     *
     * @param m
     *            a {@link Method Method} object.
     * @return the descriptor of the given method.
     */
    public String getDexMethodDescriptor(final MethodDescriptor method) {
        ClassDescriptor[] parameters = method.getParameterTypes();
        methodDescriptorBuilder.setLength(0);
        getDexDescriptor(methodDescriptorBuilder, method.getReturnType());
        for (int i = 0; i < parameters.length; ++i) {
            getDexDescriptor(methodDescriptorBuilder, parameters[i]);
        }
        return methodDescriptorBuilder.toString();
    }

    /**
     * Returns the dex descriptor corresponding to the given method.
     *
     * @param m
     *            a {@link Method Method} object.
     * @return the descriptor of the given method.
     */
    public String getMethodOverloadDescriptor(final MethodDescriptor method) {
        ClassDescriptor[] parameters = method.getParameterTypes();
        methodDescriptorBuilder.setLength(0);
        for (int i = 0; i < parameters.length; ++i) {
            getDexDescriptor(methodDescriptorBuilder, parameters[i]);
        }
        return methodDescriptorBuilder.toString();
    }

    /**
     * Returns the dex descriptor corresponding to the given method.
     *
     * @param m
     *            a {@link Method Method} object.
     * @return the descriptor of the given method.
     */
    public String getDexConstructorDescriptor(final MethodDescriptor constructor) {
        ClassDescriptor[] parameters = constructor.getParameterTypes();
        methodDescriptorBuilder.setLength(0);
        getDexDescriptor(methodDescriptorBuilder, ClassDescriptor.VOID);
        for (int i = 0; i < parameters.length; ++i) {
            getDexDescriptor(methodDescriptorBuilder, parameters[i]);
        }
        return methodDescriptorBuilder.toString();
    }

    private void getDexDescriptor(final StringBuffer buf, final ClassDescriptor c) {
        ClassDescriptor d = c;
        while (true) {
            if (d.isPrimitive()) {
                String sig = d.getSignature();
                buf.append(sig);
                return;
            } else if (d.isArray()) {
                buf.append('[');
                d = d.getComponentType();
            } else {
                buf.append('L');
                String name = d.getName().replace('.', '/');
                buf.append(name);
                buf.append(';');
                return;
            }
        }
    }

    /**
     * Returns the descriptor corresponding to the given Java type.
     * From ASM sources org.objectweb.asm.Type
     * @param c
     *            an object class, a primitive class or an array class.
     * @return the descriptor corresponding to the given class.
     */
    public static String getAsmDescriptor(final ClassDescriptor c) {
        StringBuffer buf = new StringBuffer();
        getAsmDescriptor(buf, c);
        return buf.toString();
    }

    /**
     * Appends the descriptor of the given class to the given string buffer.
     * From ASM sources org.objectweb.asm.Type
     * @param buf
     *            the string buffer to which the descriptor must be appended.
     * @param c
     *            the class whose descriptor must be computed.
     */
    private static void getAsmDescriptor(final StringBuffer buf, final ClassDescriptor c) {
        ClassDescriptor d = c;
        while (true) {
            if (d.isPrimitive()) {
                String sig = d.getSignature();
                buf.append(sig);
                return;
            } else if (d.isArray()) {
                buf.append('[');
                d = d.getComponentType();
            } else {
                buf.append('L');
                String name = d.getName().replace('.', '/');
                buf.append(name);
                buf.append(';');
                return;
            }
        }
    }

    /**
     * Returns the Class  signature of the Type normalized if it is a primitive
     */
    private static String getClassSignatureOfType(final ClassDescriptor c) {
        String name;

        final StringBuffer buf = new StringBuffer();
        ClassDescriptor result;

        if (c.isPrimitive()) {
            name = ClassDescriptor.PrimitiveTypeInfo.getBoxedTypeName(c);
        } else if (c.isArray()) {
            name = "java.lang.Object";
        } else {
            name = c.getName();
        }

        buf.append('L');
        buf.append(name.replace('.', '/'));
        buf.append(';');

        return buf.toString();
    }

    public void generateProxy(ApplicationWriter aw, String proxyName, ClassDescriptor classTo, HashSet<String> methodOverrides, HashSet<ClassDescriptor> implementedInterfaces, AnnotationDescriptor[] annotations) {
        String classSignature = getAsmDescriptor(classTo);

        String tnsClassSignature;
        if (proxyName.contains(".")) {
            tnsClassSignature = "L" + proxyName.replace('.', '/') + ";";
        } else {
            tnsClassSignature = LCOM_TNS +
                                classSignature.substring(1, classSignature.length() - 1).replace("$", "_");

            if (!classTo.isInterface()) {
                tnsClassSignature += CLASS_NAME_LOCATION_SEPARATOR + proxyName;
            }

            tnsClassSignature += ";";
        }

        ClassVisitor cv = generateClass(aw, classTo, classSignature, tnsClassSignature, implementedInterfaces, annotations);
        MethodDescriptor[] methods = getSupportedMethods(classTo, methodOverrides, implementedInterfaces);

        methods = groupMethodsByNameAndSignature(methods);

        generateFields(cv);


        MethodDescriptor[] ctors = classTo.getConstructors();
        boolean hasOverridenCtor = ((methodOverrides != null) && methodOverrides.contains("init"));
        generateCtors(cv, classTo, ctors, classSignature, tnsClassSignature, hasOverridenCtor);

        generateMethods(cv, classTo, methods, classSignature, tnsClassSignature);

        cv.visitEnd();
    }

    private MethodDescriptor[] groupMethodsByNameAndSignature(MethodDescriptor[] methods) {
        HashMap<String, MethodDescriptor> result = new HashMap<String, MethodDescriptor>();
        for (int i = 0; i < methods.length; i++) {
            MethodDescriptor method = methods[i];
            String methodName = method.getName();
            String methodOverLoadDescriptor = getMethodOverloadDescriptor(method);
            methodName +=  "_" + methodOverLoadDescriptor;
            if (!result.containsKey(methodName)) {
                result.put(methodName, method);
            }
        }

        return result.values().toArray(new MethodDescriptor[result.size()]);
    }

    private String getMethodSignature(MethodDescriptor m) {
        String sig = m.toGenericString();

        int nameIdx = sig.indexOf("(");
        int endSigIdx = sig.indexOf(")") + 1;

        return m.getName() + sig.substring(nameIdx, endSigIdx);
    }

    private void collectAbstractMethods(final ClassDescriptor clazz, List<MethodDescriptor> result) {
        if (!clazz.isAbstract()) {
            return;
        }
        Set<String> alreadyAddedMethods = new HashSet<String>();
        for (MethodDescriptor md: result) {
            String sig = getMethodSignature(md);
            alreadyAddedMethods.add(sig);
        }

        Set<String> concreteMethods = new HashSet<String>();
        // TODO refactor this
        boolean isInterfaceClass = clazz.isInterface();
        ClassDescriptor startingConcreteClassDesc = isInterfaceClass ? new ClassInfo(Object.class) : clazz;
        for (MethodDescriptor objMethod: startingConcreteClassDesc.getDeclaredMethods()) {
            if (!objMethod.isStatic()) {
                String sig = getMethodSignature(objMethod);
                concreteMethods.add(sig);
            }
        }

        Deque<ClassDescriptor> typesToProcess = new ArrayDeque<ClassDescriptor>();
        typesToProcess.add(startingConcreteClassDesc);
        if (clazz.isInterface()) {
            typesToProcess.add(clazz);
        }

        while (!typesToProcess.isEmpty()) {
            ClassDescriptor currentType = typesToProcess.pollFirst();
            MethodDescriptor[] methods = currentType.getDeclaredMethods();
            for (MethodDescriptor m: methods) {
                if (m.isStatic()) {
                    continue;
                }
                String sig = getMethodSignature(m);
                if (m.isAbstract()) {
                    if (!concreteMethods.contains(sig) && !alreadyAddedMethods.contains(sig)) {
                        if (isInterfaceClass) {
                            m.setAsInterfaceMethod();
                        }
                        result.add(m);
                        alreadyAddedMethods.add(sig);
                    }
                } else if (!concreteMethods.contains(sig)) {
                    concreteMethods.add(sig);
                }
            }

            if (!currentType.isInterface() && !currentType.getName().equals("java.lang.Object")) {
                typesToProcess.addFirst(currentType.getSuperclass());
            }

            for (ClassDescriptor iface: currentType.getInterfaces()) {
                typesToProcess.add(iface);
            }
        }
    }

    private MethodDescriptor[] getSupportedMethods(ClassDescriptor clazz, HashSet<String> methodOverrides, HashSet<ClassDescriptor> interfacesToImplement) {
        ArrayList<MethodDescriptor> result = new ArrayList<MethodDescriptor>();

        collectAbstractMethods(clazz, result);

        for (ClassDescriptor iface : interfacesToImplement) {
            collectAbstractMethods(iface, result);
        }

        boolean isApplicationClass = isApplicationClass(clazz);

        if (!clazz.isInterface()) {
            HashMap<String, MethodDescriptor> finalMethods = new HashMap<String, MethodDescriptor>();
            while (clazz != null) {
                MethodDescriptor[] methods = clazz.getDeclaredMethods();

                ArrayList<MethodDescriptor> methodz = new ArrayList<MethodDescriptor>();

                for (int i = 0; i < methods.length; i++) {
                    MethodDescriptor candidateMethod = methods[i];
                    if (isApplicationClass && candidateMethod.getName().equals("attachBaseContext")) {
                        // this is the only Application method called before onCreate (where we initialize the runtime, so we skip this method)
                        continue;
                    }
                    if (methodOverrides != null && !methodOverrides.contains(candidateMethod.getName())) {
                        continue;
                    }

                    methodz.add(candidateMethod);
                }

                for (int i = 0; i < methodz.size(); i++) {
                    MethodDescriptor method = methodz.get(i);

                    if (isMethodSupported(method, finalMethods)) {
                        result.add(method);
                    }
                }

                clazz = clazz.getSuperclass();
            }
        }

        return result.toArray(new MethodDescriptor[result.size()]);

        //For debugging
        //return new Method[] { result.get(0) };
    }

    private static boolean isMethodSupported(MethodDescriptor method, HashMap<String, MethodDescriptor> finalMethods) {
        if (method.isSynthetic() || method.isStatic() || method.isPrivate()) {
            return false;
        }

        if (method.isFinal()) {
            finalMethods.put(method.getName(), method);
            return false;
        }

        boolean isPackagePrivate = !method.isPrivate() && !method.isPublic() && !method.isProtected();
        if (isPackagePrivate) {
            return false;
        }

        if (isMethodMarkedAsFinalInClassHierarchy(method, finalMethods)) {
            return false;
        }

//		if (finalMethods.size() != 0)
//		{
//			Method finalMethod = finalMethods.get(method.getName());
//			if (finalMethod != null)
//			{
//				boolean isSameFinalMethod = areMethodSignaturesEqual(finalMethod, method);
//				if (isSameFinalMethod)
//				{
//					return false;
//				}
//			}
//		}

        return true;
    }

    private static boolean isMethodMarkedAsFinalInClassHierarchy(MethodDescriptor method, HashMap<String, MethodDescriptor> finalMethods) {
        if (finalMethods.size() != 0) {
            MethodDescriptor finalMethod = finalMethods.get(method.getName());
            if (finalMethod != null) {
                boolean isSameFinalMethod = areMethodSignaturesEqual(finalMethod, method);
                if (isSameFinalMethod) {
                    return true;
                }
            }
        }

        return false;
    }

    private static boolean areMethodSignaturesEqual(MethodDescriptor x, MethodDescriptor y) {
        if (x.equals(y)) {
            return true;
        }

        if (!x.getName().equals(y.getName())) {
            return false;
        }

        ClassDescriptor[] xParams = x.getParameterTypes();
        ClassDescriptor[] yParams = y.getParameterTypes();

        if (xParams.length != yParams.length) {
            return false;
        }

        boolean result = true;

        for (int i = 0; i < xParams.length; i++) {
            if (!xParams[i].equals(yParams[i])) {
                result = false;
                break;
            }
        }

        return result;
    }

    private void generateCtors(ClassVisitor cv, ClassDescriptor classTo, MethodDescriptor[] ctors, String classSignature, String tnsClassSignature, boolean hasOverridenCtor) {
        if (classTo.isInterface()) {
            try {
                // TODO refactor this
                MethodDescriptor defaultCtor = new MethodInfo(Object.class.getConstructor());
                generateCtor(cv, classTo, defaultCtor, classSignature, tnsClassSignature, false);
            } catch (Exception e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        } else {
            for (MethodDescriptor ctor : ctors) {
                generateCtor(cv, classTo, ctor, classSignature, tnsClassSignature, hasOverridenCtor);
            }
        }
    }


    private void generateCtor(ClassVisitor cv, ClassDescriptor classTo, MethodDescriptor ctor, String classSignature, String tnsClassSignature, boolean hasOverridenCtor) {
        //TODO: handle generic and vararg ctors if needed
        String ctorSignature = getDexConstructorDescriptor(ctor);
        //org.objectweb.asm.Type.getConstructorDescriptor(ctor);

        MethodVisitor mv;
        int ctorModifiers = getDexModifiers(ctor);
        //int locaVarsCount = 2;
        //int thisRegister = locaVarsCount + 1;

        mv = cv.visitMethod(ctorModifiers + org.ow2.asmdex.Opcodes.ACC_CONSTRUCTOR, "<init>", ctorSignature, null, null);
        mv.visitCode();

        //mv.visitMaxs(4, 0); //TODO: max stack size should be equal to localVarCount + 1
        int thisRegister = generateMaxStackSize(mv, ctor);

        int argCount = ctor.getParameterTypes().length;
        int[] args = generateArgsArray(thisRegister, argCount, ctor);

        if (!classTo.isInterface()) {
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT_RANGE, classSignature, "<init>", ctorSignature, args);
        } else {
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT_RANGE, objectClass, "<init>", ctorSignature, args);
        }

        if (!isApplicationClass(classTo)) {
            generateInitializedBlock(mv, thisRegister, classSignature, tnsClassSignature);
        }

        if (hasOverridenCtor) {
            generateCtorOverridenBlock(mv, thisRegister, ctor, classSignature, tnsClassSignature);
        }

        generateReturnVoid(mv);
    }

    private void generateReturnVoid(MethodVisitor mv) {
        mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
        mv.visitEnd();
    }

    private void generateCtorOverridenBlock(MethodVisitor mv, int thisRegister, MethodDescriptor ctor, String classSignature, String tnsClassSignature) {
        int argCount = generateArrayForCallJsArguments(mv, ctor.getParameterTypes(), thisRegister, classSignature, tnsClassSignature);
        mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "init"); //put "init" in register 1
        mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 2, 1); //put true to register 2 == isConstructor argument
        mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, LCOM_TNS_RUNTIME, "callJSMethod", callJsMethodSignatureCtor, new int[]
                           { 3, 1, 2, 0 }); //invoke callJSMethod(this, "init", true, params)
    }

    private void generateInitializedBlock(MethodVisitor mv, int thisRegister, String classSignature, String tnsClassSignature) {
        mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, tnsClassSignature, "__initialized", "Z", thisRegister - 2, thisRegister); //put __initialized in local var 1
        Label label = new Label();
        mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, label, thisRegister - 2, 0); //compare local var 1 with false
        mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, thisRegister - 1, 1); 		//put true in local var 1
        mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, tnsClassSignature, "__initialized", "Z", thisRegister - 1 , thisRegister); //set field to the value of 2
        mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, LCOM_TNS_RUNTIME, "initInstance", "VLjava/lang/Object;", new int[] { thisRegister }); //call init instance passing this as arugment
        mv.visitLabel(label);
    }

    private void generateMethods(ClassVisitor cv, ClassDescriptor classTo, MethodDescriptor[] methods, String classSignature, String tnsClassSignature) {
        //for (Method method : methods)
        for (int i = 0; i < methods.length; i++) {
            MethodDescriptor sourceMethod = methods[i];

            generateMethod(cv, classTo, sourceMethod, i, classSignature, tnsClassSignature);
        }

        generateEqualsSuper(cv);
        generateHashCodeSuper(cv);
    }

    private void generateEqualsSuper(ClassVisitor cv) {
        MethodVisitor mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "equals__super", "ZLjava/lang/Object;", null, null);
        mv.visitCode();
        mv.visitMaxs(3, 0);
        mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "equals", "ZLjava/lang/Object;", new int[] { 1, 2 });
        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
        mv.visitEnd();
    }

    private void generateHashCodeSuper(ClassVisitor cv) {
        MethodVisitor mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "hashCode__super", "I", null, null);
        mv.visitCode();
        mv.visitMaxs(2, 0);
        mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "hashCode", "I", new int[] { 1 });
        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
        mv.visitEnd();
    }

    private void generateMethod(ClassVisitor cv, ClassDescriptor classTo, MethodDescriptor method, int methodNumber, String classSignature, String tnsClassSignature) {
        if (ProxyGenerator.IsLogEnabled) {
            System.out.println("Generator: generatingMethod " + method.getName());
        }

        //TODO: handle checked exceptions
        String methodDexSignature = getDexMethodDescriptor(method);
        String[] exceptions = new String[0];
        MethodVisitor mv;
        int methodModifiers = getDexModifiers(method);

        mv = cv.visitMethod(methodModifiers, method.getName(), methodDexSignature, null, exceptions);
        mv.visitCode();

        int thisRegister = generateMaxStackSize(mv, method);

        if (!classTo.isInterface()) {
            if (isApplicationClass(classTo) && method.getName().equals("onCreate")) {
                generateRuntimeInitializedBlock(mv, method, thisRegister, classSignature, tnsClassSignature, classTo.getName());
            } else {
                if (!method.isInterfaceMethod()) { //interface methods do not need an initialized block
                    generateInitializedBlock(mv, thisRegister, classSignature, tnsClassSignature);
                }
            }
        }

        if (!isApplicationClass(classTo) || !method.getName().equals("onCreate")) {
            generateCallOverrideBlock(mv, method, thisRegister, classSignature, tnsClassSignature);
        }
        generateReturnFromObject(mv, method.getReturnType(), thisRegister, 1);

        mv.visitEnd();
    }

    private boolean isApplicationClass(ClassDescriptor clazz) {
        boolean isApplicationClass = false;
        //TODO: plamen5kov: improve check for application class include MultidexApplication and other common scenarios
        String applicationClassName = "android.app.Application";
        ClassDescriptor currentClass = clazz;
        while ((currentClass != null) && !isApplicationClass) {
            isApplicationClass = currentClass.getName().equals(applicationClassName);
            if (!isApplicationClass) {
                currentClass = currentClass.getSuperclass();
            }
        }
        return isApplicationClass;
    }

    private void generateRuntimeInitializedBlock(MethodVisitor mv, MethodDescriptor method, int thisRegister, String classSignature, String tnsClassSignature, String superClassname) {
        String name = "L" + superClassname.replace('.', '/') + ";";

        mv.visitMethodInsn(Opcodes.INSN_INVOKE_SUPER, name, "onCreate", "V", new int[] { thisRegister });
        mv.visitMethodInsn(Opcodes.INSN_INVOKE_STATIC, "Lcom/tns/RuntimeHelper;", "initRuntime", "Lcom/tns/Runtime;Landroid/app/Application;", new int[] { thisRegister });
        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
        Label label = new Label();
        mv.visitJumpInsn(Opcodes.INSN_IF_EQZ, label, 0, 0);
        mv.visitMethodInsn(Opcodes.INSN_INVOKE_VIRTUAL, LCOM_TNS_RUNTIME, "run", "V", new int[] { 0 });
        generateCallOverrideBlock(mv, method, thisRegister, classSignature, tnsClassSignature);
        mv.visitLabel(label);
    }

    private int generateMaxStackSize(MethodVisitor mv, MethodDescriptor method) {
        //3 local vars are enough for NativeScript bindings methods. Local vars start from 0 register till register 2.
        //Then 'this' is register 3 and then all parameters according to their size
        int registersCount = 3/*local vars*/ + 1 /*this*/;
        int thisRegister = registersCount - 1;

        ClassDescriptor[] paramTypes = method.getParameterTypes();
        int paramCount = paramTypes.length;

        for (int i = 0; i < paramCount; i++) {
            ClassDescriptor paramType = paramTypes[i];

            if (paramType.equals(ClassDescriptor.LONG) || paramType.equals(ClassDescriptor.DOUBLE)) {
                registersCount += 2;
            } else {
                registersCount += 1;
            }
        }

        mv.visitMaxs(registersCount, 0);

        return thisRegister;
    }

    private void generateCallOverrideBlock(MethodVisitor mv, MethodDescriptor method, int thisRegister, String classSignature, String tnsClassSignature) {
        //call the override
        int argCount = generateArrayForCallJsArguments(mv, method.getParameterTypes() , thisRegister, classSignature, tnsClassSignature);
        mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, method.getName());

        ClassDescriptor returnType = method.getReturnType();
        if (returnType.isPrimitive()) {
            //mv.visitFieldInsn(INSN_SGET_OBJECT, "Ljava/lang/Long;", "TYPE", "Ljava/lang/Class;", 2, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_SGET_OBJECT, getClassSignatureOfType(returnType), "TYPE", "Ljava/lang/Class;", 2, 0);
        } else {
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CONST_CLASS, 2, 0, 0, getClassSignatureOfType(returnType));
        }

        mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, callJSMethodName, callJsMethodSignatureMethod, new int[] { thisRegister, 1, 2, 0 });

        //Label returnLabel = new Label();
        //mv.visitLabel(returnLabel);
    }

    private int[] generateArgsArray(int thisRegister, int argCount, MethodDescriptor ctor) {
        ClassDescriptor[] paramTypes = ctor.getParameterTypes();
        int argumentsCount = paramTypes.length;
        int[] argsForSuper = new int[1 + argumentsCount*2]; //thisRegister + argCount * 2 since it long and double take 2 registers

        int argsForSuperIndex = 0;
        argsForSuper[argsForSuperIndex] = thisRegister;
        argsForSuperIndex++;

        int arrayIndex = 0;
        while (arrayIndex < argumentsCount) {
            ClassDescriptor paramType = paramTypes[arrayIndex];
            if (paramType.equals(ClassDescriptor.LONG) || paramType.equals(ClassDescriptor.DOUBLE)) {
                argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 1;
                argsForSuperIndex++;
                argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 2;
                argsForSuperIndex++;
            } else {
                argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 1;
                argsForSuperIndex++;

            }

            arrayIndex++;
        }

        return Arrays.copyOf(argsForSuper, argsForSuperIndex);
    }

    /**
     * 	Creates new Object[] or null value (when no arguments) and puts it in register 0
     */
    private int generateArrayForCallJsArguments(MethodVisitor mv, ClassDescriptor[] paramTypes, int thisRegister, String classSignature, String tnsClassSignature) {
        int argumentsCount = paramTypes.length;

        if (argumentsCount == 0) {
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0); //set null at register 0. our params array is null
            return 0;
        }

        mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 2, argumentsCount); //put array size in register 2
        mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_ARRAY, 0, 0, 2, "[Ljava/lang/Object;"); //create array with size in register 2 and put it in register 0

        int arrayIndex = 0;
        int argNumber = 4;
        int numberOfDoubleRegisterArguments = 0;
        while (argNumber < 4 + argumentsCount + numberOfDoubleRegisterArguments) {
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, arrayIndex); //put the array access index value in register 1

            ClassDescriptor paramType = paramTypes[arrayIndex];
            if (paramType.isPrimitive()) {
                if (paramType.equals(ClassDescriptor.INT)) {
                    //box the primitive value at register i
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Integer;", "valueOf", "Ljava/lang/Integer;I", new int[] { argNumber });

                    //move the result object in register 2
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);

                    //put the object at register 2 in array at index register 1
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
                } else if (paramType.equals(ClassDescriptor.CHAR)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Character;", "valueOf", "Ljava/lang/Character;C", new int[] { argNumber });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

                } else if (paramType.equals(ClassDescriptor.BYTE)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Byte;", "valueOf", "Ljava/lang/Byte;B", new int[] { argNumber });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

                } else if (paramType.equals(ClassDescriptor.SHORT)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Short;", "valueOf", "Ljava/lang/Short;S", new int[] { argNumber });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

                } else if (paramType.equals(ClassDescriptor.BOOLEAN)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Boolean;", "valueOf", "Ljava/lang/Boolean;Z", new int[] { argNumber });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
                } else if (paramType.equals(ClassDescriptor.LONG)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Long;", "valueOf", "Ljava/lang/Long;J", new int[] { argNumber, argNumber + 1 });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
                    argNumber++;
                    numberOfDoubleRegisterArguments++;
                } else if (paramType.equals(ClassDescriptor.FLOAT)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Float;", "valueOf", "Ljava/lang/Float;F", new int[] { argNumber });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

                } else if (paramType.equals(ClassDescriptor.DOUBLE)) {
                    mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Double;", "valueOf", "Ljava/lang/Double;D", new int[] { argNumber, argNumber + 1 });
                    mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
                    mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
                    argNumber++;
                    numberOfDoubleRegisterArguments++;
                }
            } else {
                //register 1 contains the value of the arrayIndex
                //i is the register containing the value of the object reference
                //0 register contains the reference to the array
                mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, argNumber, 0, 1);
            }



//			int arrayPutOpCode = getArrayPutInstructionByType(paramTypes[arrayIndex]);

            //mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, i, 0, 1); //put params[i] in the array at index register 1
            //mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_
            argNumber++;
            arrayIndex++;
        }

        return argumentsCount;
    }

    private static final String integerTypeDescriptor = "Ljava/lang/Integer;";
    private static final String booleanTypeDescriptor = "Ljava/lang/Boolean;";
    private static final String byteTypeDescriptor = "Ljava/lang/Byte;";
    private static final String characterTypeDescriptor = "Ljava/lang/Character;";
    private static final String shortTypeDescriptor = "Ljava/lang/Short;";
    private static final String doubleTypeDescriptor = "Ljava/lang/Double;";
    private static final String floatTypeDescriptor = "Ljava/lang/Float;";
    private static final String longTypeDescriptor = "Ljava/lang/Long;";

    private void generateReturnFromObject(MethodVisitor mv, ClassDescriptor targetReturnType, int thisRegister, int valueRegister) {
        if (targetReturnType.equals(ClassDescriptor.VOID)) {
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            return;
        }

        mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, valueRegister); //get the result of from the very last method call in register 1;
        if (targetReturnType.isPrimitive()) {
            if (targetReturnType.equals(ClassDescriptor.BOOLEAN)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, booleanTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, booleanTypeDescriptor, "booleanValue", "Z", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.CHAR)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, characterTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, characterTypeDescriptor, "charValue", "C", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.BYTE)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, byteTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, byteTypeDescriptor, "byteValue", "B", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.SHORT)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, shortTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, shortTypeDescriptor, "shortValue", "S", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.INT)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, integerTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, integerTypeDescriptor, "intValue", "I", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.LONG)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, longTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, longTypeDescriptor, "longValue", "J", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_WIDE, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_WIDE, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.FLOAT)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, floatTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, floatTypeDescriptor, "floatValue", "F", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            } else if (targetReturnType.equals(ClassDescriptor.DOUBLE)) {
                mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, doubleTypeDescriptor); //throw exception if can't cast this reference to the return type
                mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, doubleTypeDescriptor, "doubleValue", "D", new int[] { valueRegister });
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_WIDE, valueRegister);
                mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_WIDE, valueRegister);
            }
        } else { //return object
            String returnTypeDescriptor = getAsmDescriptor(targetReturnType);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, returnTypeDescriptor); //throw exception if can't cast this reference to the return type
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, valueRegister);
        }
    }

    private void generateFields(ClassVisitor cv) {
        FieldVisitor fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__initialized", "Z", null, null);
        fv.visitEnd();
    }

    static final String[] classImplentedInterfaces = new String[] { "Lcom/tns/NativeScriptHashCodeProvider;" };
    static final String[] interfaceImplementedInterfaces = new String[] { "Lcom/tns/NativeScriptHashCodeProvider;", "" };

    private ClassVisitor generateClass(ApplicationWriter aw, ClassDescriptor classTo, String classSignature, String tnsClassSignature, HashSet<ClassDescriptor> implementedInterfaces, AnnotationDescriptor[] annotations) {
        ClassVisitor cv;

        int classModifiers = getDexModifiers(classTo);
        ArrayList<String> interfacesToImplement = new ArrayList(Arrays.asList(classImplentedInterfaces));

        if (classTo.isInterface()) {
            interfaceImplementedInterfaces[1] = classSignature; //new String[] { "Lcom/tns/NativeScriptHashCodeProvider;", classSignature };
            for (String interfaceToImpl : interfaceImplementedInterfaces) {
                if (!interfacesToImplement.contains(interfaceToImpl)) {
                    interfacesToImplement.add(interfaceToImpl);
                }
            }

            classSignature = objectClass;
        } else {
            if (implementedInterfaces != null) {
                for (ClassDescriptor interfaceToImpl : implementedInterfaces) {
                    interfacesToImplement.add(getAsmDescriptor(interfaceToImpl));
                }
            }
        }

        String[] interfacesToImplementArr = new String[interfacesToImplement.size()];
        interfacesToImplementArr = interfacesToImplement.toArray(interfacesToImplementArr);

        cv = aw.visitClass(classModifiers, tnsClassSignature, null, classSignature, interfacesToImplementArr);
        cv.visit(0, classModifiers, tnsClassSignature, null, classSignature, interfacesToImplementArr);
        if ((annotations !=  null) && (annotations.length > 0)) {
            for (AnnotationDescriptor ad: annotations) {
                String annotationClassname = ad.getAnnotationClassname();
                boolean isVisible = ad.isRuntimeVisible();
                AnnotationVisitor av = cv.visitAnnotation(annotationClassname, isVisible);
                setAnnotationFields(av, ad);
                av.visitEnd();
            }
        }
        cv.visitSource(classTo.getName() +  ".java", null);
        return cv;
    }

    private void setAnnotationFields(AnnotationVisitor av, AnnotationDescriptor ad) {
        AnnotationDescriptor.Parameter[] params = ad.getParams();
        if (params.length > 0) {
            for (AnnotationDescriptor.Parameter p: params) {
                av.visit(p.getName(), p.getValue());
            }
        }
    }

    private int getDexModifiers(Descriptor descriptor) {
        if (descriptor.isPublic()) {
            return org.ow2.asmdex.Opcodes.ACC_PUBLIC;
        }

        return org.ow2.asmdex.Opcodes.ACC_PROTECTED;
    }
}