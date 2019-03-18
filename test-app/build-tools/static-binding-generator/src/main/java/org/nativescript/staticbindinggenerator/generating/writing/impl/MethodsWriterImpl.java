package org.nativescript.staticbindinggenerator.generating.writing.impl;

import org.apache.bcel.generic.Type;
import org.nativescript.staticbindinggenerator.DefaultValues;
import org.nativescript.staticbindinggenerator.Writer;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.MethodSignatureVisitor;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.generating.writing.MethodsWriter;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.objectweb.asm.signature.SignatureReader;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MethodsWriterImpl implements MethodsWriter {

    private static final String SUPER_KEYWORD = "super";
    private static final String SUPER_METHOD_CALL_PREFIX = "super.";
    private static final String PARAMETER_PREFIX = "param_";
    private static final String ARGS_VARIABLE_NAME = "args";
    private static final String RUNTIME_VARIABLE_NAME = "runtime";
    private static final String ON_CREATE_METHOD_NAME = "onCreate";

    private static final String GET_INSTANCE_METHOD_SIGNATURE_PATTERN = "public static %s getInstance()";
    private static final String RUNTIME_CALL_JS_METHOD_CALL_PATTERN = "com.tns.Runtime.callJSMethod(this, \"%s\", %s.class, " + ARGS_VARIABLE_NAME + ")";
    private static final String RUNTIME_CALL_JS_CONSTRUCTOR_METHOD_CALL_PATTERN = "com.tns.Runtime.callJSMethod(this, \"%s\", %s.class, true," + ARGS_VARIABLE_NAME + ")";
    private static final String ARGS_VARIABLE_PATTERN = "java.lang.Object[] " + ARGS_VARIABLE_NAME + " = new java.lang.Object[%d];";

    private static final String TRY_BLOCK_NAME_BEGINNING = "try {";
    private static final String GENERIC_CATCH_BLOCK_ARGUMENT_NAME = "throwable";
    private static final String GENERIC_CATCH_BLOCK_BEGINNING = "catch (Throwable " + GENERIC_CATCH_BLOCK_ARGUMENT_NAME + ") {";

    private static final String APPLICATION_INSTANCE_VARIABLE_NAME = "thiz";
    private static final String INSTANCE_VARIABLE_NAME = "this";
    private static final String GET_INSTANCE_METHOD_RETURN_STATEMENT = "return " + APPLICATION_INSTANCE_VARIABLE_NAME + ";";
    private static final String INTERNAL_RUNTIME_EQUALS_METHOD_RETURN_STATEMENT = "return super.equals(other);";
    private static final String INTERNAL_RUNTIME_HASHCODE_METHOD_RETURN_STATEMENT = "return super.hashCode();";
    private static final String ANDROID_LOG_METHOD_CALL_STATEMENT = "android.util.Log.w(\\\"Error\\\", " + GENERIC_CATCH_BLOCK_ARGUMENT_NAME + ");\"";
    private static final String RUNTIME_INIT_METHOD_CALL_STATEMENT = "com.tns.Runtime " + RUNTIME_VARIABLE_NAME + " = RuntimeHelper.initRuntime(this);";
    private static final String RUNTIME_RUN_METHOD_CALL_STATEMENT = RUNTIME_VARIABLE_NAME + ".run();";
    private static final String RUNTIME_INIT_INSTANCE_METHOD_CALL_STATEMENT = "com.tns.Runtime.initInstance(this);";

    private static final String RUNTIME_IS_INITIALIZED_METHOD_CALL = "Runtime.isInitialized()";

    private static final String INTERNAL_RUNTIME_EQUALS_METHOD_SIGNATURE = "public boolean equals__super(java.lang.Object other)";
    private static final String INTERNAL_RUNTIME_HASHCODE_METHOD_SIGNATURE = "public int hashCode__super()";

    private static final String RETURN_KEYWORD = "return";
    private static final String IF_STATEMENT_BEGINNING = "if";

    private static final String NOT_EQUALS_OPERATOR = "!=";
    private static final String NULL_VALUE = "null";
    private static final char NEGATE_LITERAL = '!';


    private final Writer writer;
    private final boolean shouldSuppressCallJsMethodExceptions;
    private final boolean isForApplicationClass;


    public MethodsWriterImpl(final Writer writer, boolean shouldSuppressCallJsMethodExceptions, boolean isForApplicationClass) {
        this.writer = writer;
        this.shouldSuppressCallJsMethodExceptions = shouldSuppressCallJsMethodExceptions;
        this.isForApplicationClass = isForApplicationClass;
    }

    @Override
    public void writeMethod(ReifiedJavaMethod method) {
        if(method.getName().equals("getItem")){
            System.out.println("!!!!!! VM: ");
            System.out.println(method.getReifiedReturnType());
            System.out.println(method.getSignature());
            System.out.println(method.getGenericSignature());
            System.out.println("!!!!!! VM end:");
        }
        writeMethodSignature(method);
        writer.write(OPENING_CURLY_BRACKET_LITERAL);
        writeRuntimeInitCallIfNecessary(method);
        writeRuntimeInitializationCheckIfNecessary(method);
        writeMethodBody(method);
        writeRuntimeRunCallIfNecessary(method);
        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }

    @Override
    public void writeConstructor(String className, ReifiedJavaMethod method, boolean hasUserImplementedInitMethod) {
        writer.write(getMethodVisibilityModifier(method));
        writer.write(SPACE_LITERAL);
        writer.write(className);
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writeMethodArguments(method);
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
        writer.write(OPENING_CURLY_BRACKET_LITERAL);
        writer.write(SUPER_KEYWORD);
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writeMethodArgumentsPass(method);
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
        writer.write(END_OF_STATEMENT_LITERAL);

        if (!isForApplicationClass) {
            writer.write(RUNTIME_INIT_INSTANCE_METHOD_CALL_STATEMENT);
        }

        if (hasUserImplementedInitMethod) {
            writeMethodBody(method);
        }

        if (isForApplicationClass) {
            writer.write(APPLICATION_INSTANCE_VARIABLE_NAME);
            writer.write(EQUALS_LITERAL);
            writer.write(INSTANCE_VARIABLE_NAME);
            writer.write(END_OF_STATEMENT_LITERAL);
        }

        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }

    @Override
    public void writeDefaultConstructor(String className) {
        writer.write(PUBLIC_MODIFIER);
        writer.write(SPACE_LITERAL);
        writer.write(className);
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
        writer.write(OPENING_CURLY_BRACKET_LITERAL);
        writer.write(RUNTIME_INIT_INSTANCE_METHOD_CALL_STATEMENT);
        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }

    private void writeRuntimeInitCallIfNecessary(ReifiedJavaMethod method) {
        if (method.getName().equals(ON_CREATE_METHOD_NAME) && isForApplicationClass) {
            writer.write(RUNTIME_INIT_METHOD_CALL_STATEMENT);
        }
    }

    private void writeRuntimeRunCallIfNecessary(ReifiedJavaMethod method) {
        if (method.getName().equals(ON_CREATE_METHOD_NAME) && isForApplicationClass) {
            writeIfStatementBeginning(RUNTIME_VARIABLE_NAME + NOT_EQUALS_OPERATOR + NULL_VALUE);
            writer.write(OPENING_CURLY_BRACKET_LITERAL);
            writer.writeln(RUNTIME_RUN_METHOD_CALL_STATEMENT);
            writer.writeln(CLOSING_CURLY_BRACKET_LITERAL);
        }
    }

    private void writeRuntimeInitializationCheckIfNecessary(ReifiedJavaMethod method) {

        if (isForApplicationClass && !method.isFromInterface()) {
            writeIfStatementBeginning(NEGATE_LITERAL + RUNTIME_IS_INITIALIZED_METHOD_CALL);
            writer.write(OPENING_CURLY_BRACKET_LITERAL);

            boolean isVoid = method.getReturnType().equals(Type.VOID);

            if (!isVoid) {
                writer.write(RETURN_KEYWORD);
                writer.write(SPACE_LITERAL);
            }
            writer.write(SUPER_METHOD_CALL_PREFIX);
            writer.write(getMethodName(method));
            writer.write(OPENING_ROUND_BRACKET_LITERAL);

            writeMethodArgumentsPass(method);

            writer.write(CLOSING_ROUND_BRACKET_LITERAL);
            writer.write(END_OF_STATEMENT_LITERAL);

            if (isVoid) {
                writer.write(RETURN_KEYWORD);
                writer.write(END_OF_STATEMENT_LITERAL);
            }

            writer.writeln(CLOSING_CURLY_BRACKET_LITERAL);
        }
    }

    private void writeMethodArgumentsPass(ReifiedJavaMethod method) {
        int paramCount = method.getArgumentTypes().length;
        for (int i = 0; i < paramCount; i++) {
            if (i > 0) {
                writer.write(COMMA_LITERAL);
            }
            writer.write(PARAMETER_PREFIX + i);
        }
    }

    private void writeIfStatementBeginning(String condition) {
        writer.write(IF_STATEMENT_BEGINNING);
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writer.write(condition);
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
    }

    private void writeMethodSignature(ReifiedJavaMethod method) {
        writer.write(getMethodVisibilityModifier(method));
        writer.write(SPACE_LITERAL);
//        writer.write(BcelNamingUtil.resolveBcelTypeName(method.getReturnType()));
        writer.write(BcelNamingUtil.resolveBcelTypeName(method.getReifiedReturnType()));
        writer.write(SPACE_LITERAL);
        writer.write(getMethodName(method));
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writeMethodArguments(method);
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
    }

    private void writeMethodArguments(ReifiedJavaMethod method) {

        List<String> reifiedArgumentsList = method.getReifiedArguments();

        for (int i = 0; i < reifiedArgumentsList.size(); i += 1) {
            if (i > 0) {
                writer.write(COMMA_LITERAL);
            }

            String reifiedArgument = reifiedArgumentsList.get(i);
            String bcelResolvedTypeName = BcelNamingUtil.resolveBcelTypeName(reifiedArgument);
            writer.write(bcelResolvedTypeName);
        }


//        Type[] args = method.getArgumentTypes();
//        Map<Integer, String> genericParameters = null;
//
//        if (method.hasGenericSignature()) {
//            genericParameters = getGenericParameters(method.getGenericSignature());
//        }
//
//        for (int i = 0; i < args.length; i++) {
//
//            if (i > 0) {
//                writer.write(COMMA_LITERAL);
//            }
//
//            if (genericParameters != null && genericParameters.containsKey(i)) {
//                String genericArgumentName = genericParameters.get(i);
//                GenericParameters classGenericParameters;
//                if (method.isFromInterface()) {
//                    classGenericParameters = genericHierarchyView.getAllImplementedInterfaces().get(method.getDeclaringClassName());
//                } else {
//                    classGenericParameters = genericHierarchyView.getAllParentClasses().get(method.getDeclaringClassName());
//                }
//
//                if (classGenericParameters != null) {
//                    String reifiedClassGenericParameter = classGenericParameters.getGenericParameters().get(genericArgumentName);
//                    if (reifiedClassGenericParameter != null && !reifiedClassGenericParameter.equals("")) {
//                        writer.write(reifiedClassGenericParameter);
//                    } else {
//                        writer.write(BcelNamingUtil.resolveBcelTypeName(args[i]));
//                    }
//                } else {
//                    writer.write(BcelNamingUtil.resolveBcelTypeName(args[i]));
//                }
//            } else {
//                writer.write(BcelNamingUtil.resolveBcelTypeName(args[i]));
//            }
//
//            writer.write(SPACE_LITERAL);
//            writer.write(PARAMETER_PREFIX);
//            writer.write(i);
//        }
    }

    private Map<Integer, String> getGenericParameters(String signature) {
        SignatureReader sigReader = new SignatureReader(signature);
        MethodSignatureVisitor visitor = new MethodSignatureVisitor();
        sigReader.accept(visitor);
        return visitor.getParametersMap();
    }

    private String getMethodVisibilityModifier(ReifiedJavaMethod method) {
        return method.isPublic() ? PUBLIC_MODIFIER : PROTECTED_MODIFIER;
    }

    private void writeMethodBody(ReifiedJavaMethod method) {
        Type returnType = method.getReturnType();

//        if(method.isConstructor()){
//            writeArgumentsVariableForConstructorCall(method);
//        } else {
        writeArgumentsVariableForMethodCall(method);
//        }

        writeCallJsMethodExceptionsSuppressBlockBeginningIfNecessary();

        String methodCallPatern = method.isConstructor() ? RUNTIME_CALL_JS_CONSTRUCTOR_METHOD_CALL_PATTERN : RUNTIME_CALL_JS_METHOD_CALL_PATTERN;
        String runtimeCallJsMethodCall = String.format(methodCallPatern, getMethodName(method), BcelNamingUtil.resolveBcelTypeName(returnType));

        if (!returnType.equals(Type.VOID)) {
            writeReturnStatementWithCast(returnType, runtimeCallJsMethodCall);
        } else {
            writer.write(runtimeCallJsMethodCall);
            writer.write(END_OF_STATEMENT_LITERAL);
        }

        writeCallJsMethodExceptionsSuppressBlockClosingIfNecessary(returnType, getMethodName(method));
    }

    private String getMethodName(ReifiedJavaMethod method) {
        if (method.isConstructor()) {
            return "init";
        }

        return method.getName();
    }

    private void writeArgumentsVariableForConstructorCall(ReifiedJavaMethod method) {
        Type[] argTypes = method.getArgumentTypes();

        writer.write(String.format(ARGS_VARIABLE_PATTERN, argTypes.length + 1)); // write the args variable

        for (int i = 0; i < argTypes.length; i++) {
            writer.write(ARGS_VARIABLE_NAME);
            writer.write(OPENING_SQUARE_BRACKET_LITERAL);
            writer.write(i);
            writer.write(CLOSING_SQUARE_BRACKET_LITERAL);
            writer.write(EQUALS_LITERAL);
            writer.write(PARAMETER_PREFIX);
            writer.write(i);
            writer.write(END_OF_STATEMENT_LITERAL);
        }

        writer.write(ARGS_VARIABLE_NAME);
        writer.write(OPENING_SQUARE_BRACKET_LITERAL);
        writer.write(argTypes.length);
        writer.write(CLOSING_SQUARE_BRACKET_LITERAL);
        writer.write(EQUALS_LITERAL);
        writer.write(true);
        writer.write(END_OF_STATEMENT_LITERAL);
    }

    private void writeArgumentsVariableForMethodCall(ReifiedJavaMethod method) {
        Type[] argTypes = method.getArgumentTypes();

        writer.write(String.format(ARGS_VARIABLE_PATTERN, argTypes.length)); // write the args variable

        for (int i = 0; i < argTypes.length; i++) {
            writer.write(ARGS_VARIABLE_NAME);
            writer.write(OPENING_SQUARE_BRACKET_LITERAL);
            writer.write(i);
            writer.write(CLOSING_SQUARE_BRACKET_LITERAL);
            writer.write(EQUALS_LITERAL);
            writer.write(PARAMETER_PREFIX);
            writer.write(i);
            writer.write(END_OF_STATEMENT_LITERAL);
        }
    }

    private void writeReturnStatementWithCast(Type typeForCasting, String value) {
        writer.write(RETURN_KEYWORD);
        writer.write(OPENING_ROUND_BRACKET_LITERAL);
        writer.write(BcelNamingUtil.resolveBcelTypeName(typeForCasting));
        writer.write(CLOSING_ROUND_BRACKET_LITERAL);
        writer.write(value);
        writer.write(END_OF_STATEMENT_LITERAL);
    }

    private void writeReturnStatement(String value) {
        writer.write(RETURN_KEYWORD);
        writer.write(SPACE_LITERAL);
        writer.write(value);
        writer.write(END_OF_STATEMENT_LITERAL);
    }

    private void writeCallJsMethodExceptionsSuppressBlockBeginningIfNecessary() {
        if (shouldSuppressCallJsMethodExceptions) {
            writer.write(TRY_BLOCK_NAME_BEGINNING);
            writer.write(OPENING_CURLY_BRACKET_LITERAL);
        }
    }

    private void writeCallJsMethodExceptionsSuppressBlockClosingIfNecessary(Type returnType, String methodName) {
        if (shouldSuppressCallJsMethodExceptions) {
            writer.write(CLOSING_CURLY_BRACKET_LITERAL);
            writer.write(GENERIC_CATCH_BLOCK_BEGINNING);
            //writer.write(ANDROID_LOG_METHOD_CALL_STATEMENT);
            writer.writeln("\t\t\tcom.tns.Runtime.passSuppressedExceptionToJs(t, \"" + methodName + "\");");
            writer.writeln("\t\t\tandroid.util.Log.w(\"Warning\", \"NativeScript discarding uncaught JS exception!\");");

            if (!returnType.equals(Type.VOID)) {
                writeReturnStatement(DefaultValues.defaultStringValueFor(returnType));
            }

            writer.write(CLOSING_CURLY_BRACKET_LITERAL);
        }
    }

    @Override
    public void writeGetInstanceMethod(String className) {
        writer.write(String.format(GET_INSTANCE_METHOD_SIGNATURE_PATTERN, className));
        writer.write(OPENING_CURLY_BRACKET_LITERAL);

        writer.write(GET_INSTANCE_METHOD_RETURN_STATEMENT);

        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }

    @Override
    public void writeInternalRuntimeEqualsMethod() {
        writeInternalRuntimeMethod(INTERNAL_RUNTIME_EQUALS_METHOD_SIGNATURE, INTERNAL_RUNTIME_EQUALS_METHOD_RETURN_STATEMENT);
    }

    @Override
    public void writeInternalRuntimeHashCodeMethod() {
        writeInternalRuntimeMethod(INTERNAL_RUNTIME_HASHCODE_METHOD_SIGNATURE, INTERNAL_RUNTIME_HASHCODE_METHOD_RETURN_STATEMENT);
    }

    private void writeInternalRuntimeMethod(String signature, String returnStatement) {
        writer.write(signature);
        writer.write(OPENING_CURLY_BRACKET_LITERAL);
        writer.write(returnStatement);
        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }
}
