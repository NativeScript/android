package org.nativescript.staticbindinggenerator.generating.writing.impl;
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//


import org.objectweb.asm.signature.SignatureVisitor;

public final class TraceSignatureVisitor extends SignatureVisitor {
    private static final String COMMA_SEPARATOR = ", ";
    private static final String EXTENDS_SEPARATOR = " extends ";
    private static final String IMPLEMENTS_SEPARATOR = " implements ";
    private final boolean isInterface;
    private final StringBuilder declaration;
    private StringBuilder returnType;
    private StringBuilder exceptions;
    private boolean formalTypeParameterVisited;
    private boolean interfaceBoundVisited;
    private boolean parameterTypeVisited;
    private boolean interfaceVisited;
    private int argumentStack;
    private int arrayStack;
    private String separator = "";

    public TraceSignatureVisitor(int accessFlags) {
        super(458752);
        this.isInterface = (accessFlags & 512) != 0;
        this.declaration = new StringBuilder();
    }

    private TraceSignatureVisitor(StringBuilder stringBuilder) {
        super(458752);
        this.isInterface = false;
        this.declaration = stringBuilder;
    }

    public void visitFormalTypeParameter(String name) {
        this.declaration.append(this.formalTypeParameterVisited ? ", " : "<").append(name);
        this.formalTypeParameterVisited = true;
        this.interfaceBoundVisited = false;
    }

    public SignatureVisitor visitClassBound() {
        this.separator = " extends ";
        this.startType();
        return this;
    }

    public SignatureVisitor visitInterfaceBound() {
        this.separator = this.interfaceBoundVisited ? ", " : " extends ";
        this.interfaceBoundVisited = true;
        this.startType();
        return this;
    }

    public SignatureVisitor visitSuperclass() {
        this.endFormals();
        this.separator = " extends ";
        this.startType();
        return this;
    }

    public SignatureVisitor visitInterface() {
        if (this.interfaceVisited) {
            this.separator = ", ";
        } else {
            this.separator = this.isInterface ? " extends " : " implements ";
            this.interfaceVisited = true;
        }

        this.startType();
        return this;
    }

    public SignatureVisitor visitParameterType() {
        this.endFormals();
        if (this.parameterTypeVisited) {
            this.declaration.append(", ");
        } else {
            this.declaration.append('(');
            this.parameterTypeVisited = true;
        }

        this.startType();
        return this;
    }

    public SignatureVisitor visitReturnType() {
        this.endFormals();
        if (this.parameterTypeVisited) {
            this.parameterTypeVisited = false;
        } else {
            this.declaration.append('(');
        }

        this.declaration.append(')');
        this.returnType = new StringBuilder();
        return new TraceSignatureVisitor(this.returnType);
    }

    public SignatureVisitor visitExceptionType() {
        if (this.exceptions == null) {
            this.exceptions = new StringBuilder();
        } else {
            this.exceptions.append(", ");
        }

        return new TraceSignatureVisitor(this.exceptions);
    }

    public void visitBaseType(char descriptor) {
        switch(descriptor) {
            case 'B':
                this.declaration.append("byte");
                break;
            case 'C':
                this.declaration.append("char");
                break;
            case 'D':
                this.declaration.append("double");
                break;
            case 'E':
            case 'G':
            case 'H':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'T':
            case 'U':
            case 'W':
            case 'X':
            case 'Y':
            default:
                throw new IllegalArgumentException();
            case 'F':
                this.declaration.append("float");
                break;
            case 'I':
                this.declaration.append("int");
                break;
            case 'J':
                this.declaration.append("long");
                break;
            case 'S':
                this.declaration.append("short");
                break;
            case 'V':
                this.declaration.append("void");
                break;
            case 'Z':
                this.declaration.append("boolean");
        }

        this.endType();
    }

    public void visitTypeVariable(String name) {
        this.declaration.append(this.separator).append(name);
        this.separator = "";
        this.endType();
    }

    public SignatureVisitor visitArrayType() {
        this.startType();
        this.arrayStack |= 1;
        return this;
    }

    public void visitClassType(String name) {
        if ("java/lang/Object".equals(name)) {
            boolean needObjectClass = this.argumentStack % 2 != 0 || this.parameterTypeVisited;
            if (needObjectClass) {
                this.declaration.append(this.separator).append(name.replace('/', '.'));
            }
        } else {
            this.declaration.append(this.separator).append(name.replace('/', '.'));
        }

        this.separator = "";
        this.argumentStack *= 2;
    }

    public void visitInnerClassType(String name) {
        if (this.argumentStack % 2 != 0) {
            this.declaration.append('>');
        }

        this.argumentStack /= 2;
        this.declaration.append('.');
        this.declaration.append(this.separator).append(name.replace('/', '.'));
        this.separator = "";
        this.argumentStack *= 2;
    }

    public void visitTypeArgument() {
        if (this.argumentStack % 2 == 0) {
            ++this.argumentStack;
            this.declaration.append('<');
        } else {
            this.declaration.append(", ");
        }

        this.declaration.append('?');
    }

    public SignatureVisitor visitTypeArgument(char tag) {
        if (this.argumentStack % 2 == 0) {
            ++this.argumentStack;
            this.declaration.append('<');
        } else {
            this.declaration.append(", ");
        }

        if (tag == '+') {
            this.declaration.append("? extends ");
        } else if (tag == '-') {
            this.declaration.append("? super ");
        }

        this.startType();
        return this;
    }

    public void visitEnd() {
        if (this.argumentStack % 2 != 0) {
            this.declaration.append('>');
        }

        this.argumentStack /= 2;
        this.endType();
    }

    public String getDeclaration() {
        return this.declaration.toString();
    }

    public String getReturnType() {
        return this.returnType == null ? null : this.returnType.toString();
    }

    public String getExceptions() {
        return this.exceptions == null ? null : this.exceptions.toString();
    }

    private void endFormals() {
        if (this.formalTypeParameterVisited) {
            this.declaration.append('>');
            this.formalTypeParameterVisited = false;
        }

    }

    private void startType() {
        this.arrayStack *= 2;
    }

    private void endType() {
        if (this.arrayStack % 2 == 0) {
            this.arrayStack /= 2;
        } else {
            while(this.arrayStack % 2 != 0) {
                this.arrayStack /= 2;
                this.declaration.append("[]");
            }
        }

    }
}
