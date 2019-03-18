package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.signature.SignatureVisitor;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;


public class ClassSignatureVisitor extends SignatureVisitor {
    private static final Pattern GENERICS_ERASURE_PATTERN = Pattern.compile("<[^\\.]*>");

    private static final String COMMA_SEPARATOR = ", ";
    private static final String EXTENDS_SEPARATOR = " extends ";
    private static final String IMPLEMENTS_SEPARATOR = " implements ";
    private static final String OUTER_GENERIC_ARGUMENT_SEPARATOR = "===";

    private final List<String> formalTypeParameterDeclarations;
    private final Map<String, String> formalTypeParameterBounds;
    private StringBuilder formalTypeParameterBoundsBuffer;

    private ClassGenericDefinition parentClassDefinition;
    private List<ClassGenericDefinition> parentInterfacesDefinitions;
    private ClassGenericDefinition currentParentInterfaceDefinition;

    private final String parsedClassName;
    private final boolean isInterface;
    private final StringBuilder dataBuffer;
    private final StringBuilder formalTypeParametersTempBuffer;
    private StringBuilder parentClassGenericArgumentsBuffer;
    private StringBuilder parentInterfaceGenericArgumentsBuffer;

    private boolean formalTypeParameterVisited;
    private boolean interfaceBoundVisited;
    private boolean classBoundVisited;
    private boolean parameterTypeVisited;
    private boolean interfaceVisited;

    private boolean isVisitingFormalTypeParameters;
    private boolean isVisitingParentClass;
    private boolean isVisitingParentClassGenericArguments;
    private boolean isVisitingParentInterface;
    private boolean isVisitingParentInterfaceGenericArguments;

    /**
     * The stack used to keep track of class types that have arguments. Each element of this stack is
     * a boolean encoded in one bit. The top of the stack is the least significant bit. Pushing false
     * = *2, pushing true = *2+1, popping = /2.
     */
    private int argumentStack;

    /**
     * The stack used to keep track of array class types. Each element of this stack is a boolean
     * encoded in one bit. The top of the stack is the lowest order bit. Pushing false = *2, pushing
     * true = *2+1, popping = /2.
     */
    private int arrayStack;

    /**
     * The separator to append before the next visited class or inner class type.
     */
    private String separator = "";

    public ClassSignatureVisitor(String parsedClassName, int accessFlags) {
        super(Opcodes.ASM7);
        this.parsedClassName = parsedClassName;
        this.isInterface = (accessFlags & Opcodes.ACC_INTERFACE) != 0;
        this.dataBuffer = new StringBuilder();
        dataBuffer.append(parsedClassName);
        this.formalTypeParameterDeclarations = new ArrayList<>();
        formalTypeParameterBounds = new HashMap<>();
        formalTypeParametersTempBuffer = new StringBuilder();
        parentClassGenericArgumentsBuffer = new StringBuilder();
        parentInterfaceGenericArgumentsBuffer = new StringBuilder();
        parentInterfacesDefinitions = new ArrayList<>();
        formalTypeParameterBoundsBuffer = new StringBuilder();
    }

    GenericSignatureView getGenericSignatureView() {
        complete();
        return new GenericSignatureView(dataBuffer.toString(), formalTypeParameterDeclarations, formalTypeParameterBounds, parentClassDefinition, parentInterfacesDefinitions);
    }

    private void complete() {
        if (currentParentInterfaceDefinition != null) {
            parentInterfacesDefinitions.add(currentParentInterfaceDefinition);
        }
    }

    @Override
    public void visitFormalTypeParameter(final String name) {
        resetVisitFlags();
        isVisitingFormalTypeParameters = true;


        flushFormalTypeBoundsBuffer();

        formalTypeParameterDeclarations.add(name);

        writeToBuffer(formalTypeParameterVisited ? COMMA_SEPARATOR : "<");
        writeToBuffer(name);
        formalTypeParameterVisited = true;
        interfaceBoundVisited = false;
        classBoundVisited = false;
    }

    @Override
    public SignatureVisitor visitClassBound() {
        separator = EXTENDS_SEPARATOR;
        classBoundVisited = true;
        startType();
        return this;
    }

    @Override
    public SignatureVisitor visitInterfaceBound() {
        separator = interfaceBoundVisited ? COMMA_SEPARATOR : EXTENDS_SEPARATOR;
        interfaceBoundVisited = true;
        startType();
        return this;
    }

    @Override
    public SignatureVisitor visitSuperclass() {
        endFormals();
        flushFormalTypeBoundsBuffer();
        resetVisitFlags();
        startVisitingParentClass();
        separator = EXTENDS_SEPARATOR;
        startType();
        return this;
    }

    @Override
    public SignatureVisitor visitInterface() {
        resetVisitFlags();
        flushFormalTypeBoundsBuffer();
        startVisitingParentInterface();

        if (interfaceVisited) {
            storeAndResetInterfaceBuffer();
            separator = COMMA_SEPARATOR;
        } else {
            separator = isInterface ? EXTENDS_SEPARATOR : IMPLEMENTS_SEPARATOR;
            interfaceVisited = true;
        }
        startType();
        return this;
    }

    @Override
    public SignatureVisitor visitParameterType() {
        throw new RuntimeException("Not implemented!");
    }

    @Override
    public SignatureVisitor visitReturnType() {
        throw new RuntimeException("Not implemented!");
    }

    @Override
    public SignatureVisitor visitExceptionType() {
        throw new RuntimeException("Not implemented!");
    }

    @Override
    public void visitBaseType(final char descriptor) {
        throw new RuntimeException("Not implemented!");
    }

    @Override
    public void visitTypeVariable(final String name) {
        writeToBuffer(separator);
        writeToBuffer(name);
        separator = "";
        endType();
    }

    @Override
    public SignatureVisitor visitArrayType() {
        startType();
        arrayStack |= 1;
        return this;
    }

    @Override
    public void visitClassType(final String name) {
        if ("java/lang/Object".equals(name)) {
            // 'Map<java.lang.Object,java.util.List>' or 'abstract public V get(Object key);' should have
            // Object 'but java.lang.String extends java.lang.Object' is unnecessary.
            boolean needObjectClass = argumentStack % 2 != 0 || parameterTypeVisited;
            if (needObjectClass) {
                writeToBuffer(separator);
                writeToBuffer(name.replace('/', '.'));
            }
        } else {
            writeToBuffer(separator);
            writeToBuffer(name.replace('/', '.'));
        }
        separator = "";
        argumentStack *= 2;
    }

    @Override
    public void visitInnerClassType(final String name) {
        if (argumentStack % 2 != 0) {
            writeToBuffer('>');
        }
        argumentStack /= 2;
        writeToBuffer('.');
        writeToBuffer(separator);
        writeToBuffer(name.replace('/', '.'));
        separator = "";
        argumentStack *= 2;
    }

    @Override
    public void visitTypeArgument() {
        if (argumentStack % 2 == 0) {
            ++argumentStack;
            writeToBuffer('<');
        } else {
            writeToBuffer(COMMA_SEPARATOR);
        }
        writeToBuffer('?');
    }

    @Override
    public SignatureVisitor visitTypeArgument(final char tag) {
        if (argumentStack % 2 == 0) {
            ++argumentStack;
            writeToBuffer('<');
            startVisitingGenericArguments();
        } else if (argumentStack == 1) {
            writeToBuffer("===");
            storeCollectedGenericArguments();
        } else {
            writeToBuffer(COMMA_SEPARATOR);
        }

        if (tag == EXTENDS) {
            writeToBuffer("? extends ");
        } else if (tag == SUPER) {
            writeToBuffer("? super ");
        }

        startType();
        return this;
    }

    @Override
    public void visitEnd() {
        if (argumentStack % 2 != 0) {
            if (argumentStack == 1) {
                storeCollectedGenericArguments();
            }
            writeToBuffer('>');
        }
        argumentStack /= 2;
        endType();
    }

    private void resetVisitFlags() {
        isVisitingFormalTypeParameters = false;
        classBoundVisited = false;
        interfaceBoundVisited = false;
        isVisitingParentClass = false;
        isVisitingParentClassGenericArguments = false;
        isVisitingParentInterface = false;
        isVisitingParentInterfaceGenericArguments = false;
    }

    private void flushFormalTypeBoundsBuffer() {
        int formalTypeParameterBoundsBufferSize = formalTypeParameterBoundsBuffer.length();
        if (formalTypeParameterBoundsBufferSize != 0 && formalTypeParameterBoundsBuffer.charAt(0) != '>') {
            int formalTypeParameterDeclarationsSize = formalTypeParameterDeclarations.size();
            if (formalTypeParameterDeclarationsSize > 0) {
                String lastParameter = formalTypeParameterDeclarations.get(formalTypeParameterDeclarationsSize - 1);

                if (formalTypeParameterBoundsBuffer.charAt(formalTypeParameterBoundsBufferSize - 1) == '>') {
                    formalTypeParameterBoundsBuffer.deleteCharAt(formalTypeParameterBoundsBufferSize - 1);
                }

                String test = formalTypeParameterBoundsBuffer.toString();
                String collectedBounds = formalTypeParameterBoundsBuffer.substring(" extends ".length());
                formalTypeParameterBounds.put(lastParameter, collectedBounds);
                formalTypeParameterBoundsBuffer = new StringBuilder();
            }
        }
    }

    private void startVisitingGenericArguments() {
        if (isVisitingParentClass) {
            startVisitingParentClassGenericArguments();
        } else if (isVisitingParentInterface) {
            startVisitingParentInterfaceGenericArguments();
        }
    }

    private void storeCollectedGenericArguments() {
        if (isVisitingParentClassGenericArguments) {
            parentClassDefinition.addGenericArgument(parentClassGenericArgumentsBuffer.toString());
            parentClassGenericArgumentsBuffer = new StringBuilder();
        } else if (isVisitingParentInterfaceGenericArguments) {
            currentParentInterfaceDefinition.addGenericArgument(parentInterfaceGenericArgumentsBuffer.toString());
            parentInterfaceGenericArgumentsBuffer = new StringBuilder();
        }
    }

    private void startVisitingParentClass() {
        isVisitingParentClass = true;

        isVisitingParentInterface = false;
        isVisitingParentInterfaceGenericArguments = false;
    }

    private void startVisitingParentClassGenericArguments() {
        isVisitingParentClassGenericArguments = true;
    }

    private void startVisitingParentInterface() {
        isVisitingParentInterface = true;

        isVisitingParentClass = false;
        isVisitingParentClassGenericArguments = false;
    }

    private void startVisitingParentInterfaceGenericArguments() {
        isVisitingParentInterfaceGenericArguments = true;
    }

    private void storeAndResetInterfaceBuffer() {
        parentInterfacesDefinitions.add(currentParentInterfaceDefinition);
    }

    private void writeToBuffer(char ch) {
        writeToBuffer(String.valueOf(ch));
    }

    private void writeToBuffer(String data) {
        dataBuffer.append(data);

        if (isVisitingFormalTypeParameters) {
            writeFormalTypeInformation(data);
        } else if (isVisitingParentClass && !EXTENDS_SEPARATOR.equals(data)) {
            writeParentClassInformation(data);
        } else if (isVisitingParentInterface && !IMPLEMENTS_SEPARATOR.equals(data)) {
            writeParentInterfaceInformation(data);
        }
    }

    private void writeFormalTypeInformation(String data) {
        formalTypeParametersTempBuffer.append(data);
        if (classBoundVisited || interfaceBoundVisited) {
            formalTypeParameterBoundsBuffer.append(data);
        }
    }

    private void writeParentClassInformation(String data) {
        if (isVisitingParentClassGenericArguments && !OUTER_GENERIC_ARGUMENT_SEPARATOR.equals(data)) {
            parentClassGenericArgumentsBuffer.append(data);
        } else if (!"<".equals(data) && !OUTER_GENERIC_ARGUMENT_SEPARATOR.equals(data)) {
            parentClassDefinition = new ClassGenericDefinition(data);
        }
    }

    private void writeParentInterfaceInformation(String data) {
        if (parentInterfaceGenericArgumentsBuffer.length() != 0 || !">".equals(data)) {

            if (isVisitingParentInterfaceGenericArguments && !OUTER_GENERIC_ARGUMENT_SEPARATOR.equals(data)) {
                parentInterfaceGenericArgumentsBuffer.append(data);
            } else if (!"<".equals(data) && !OUTER_GENERIC_ARGUMENT_SEPARATOR.equals(data)) {
                currentParentInterfaceDefinition = new ClassGenericDefinition(data);
            }
        }
    }

    public String getDeclaration() {
        return dataBuffer.toString();
    }

    private void endFormals() {
        if (formalTypeParameterVisited) {
            writeToBuffer('>');
            formalTypeParameterVisited = false;
        }
    }

    private void startType() {
        arrayStack *= 2;
    }

    private void endType() {
        if (arrayStack % 2 == 0) {
            arrayStack /= 2;
        } else {
            while (arrayStack % 2 != 0) {
                arrayStack /= 2;
                writeToBuffer("[]");
            }
        }
    }

}
