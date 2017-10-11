package com.tns.bindings.desc;

public interface ClassDescriptor extends Descriptor {
    class PrimitiveTypeInfo implements ClassDescriptor {
        private final String sig;

        PrimitiveTypeInfo(String sig) {
            this.sig = sig;
        }

        @Override
        public boolean isPrimitive() {
            return true;
        }

        @Override
        public boolean isArray() {
            return false;
        }

        @Override
        public boolean isInterface() {
            return false;
        }

        @Override
        public String getSignature() {
            return sig;
        }

        @Override
        public String getName() {
            return sig;
        }

        @Override
        public MethodDescriptor[] getConstructors() {
            return new MethodDescriptor[0];
        }

        @Override
        public ClassDescriptor getComponentType() {
            return null;
        }

        @Override
        public MethodDescriptor[] getDeclaredMethods() {
            return new MethodDescriptor[0];
        }

        @Override
        public ClassDescriptor[] getInterfaces() {
            return new ClassDescriptor[0];
        }

        @Override
        public ClassDescriptor getSuperclass() {
            return null;
        }

        @Override
        public boolean isSynthetic() {
            return false;
        }

        @Override
        public boolean isStatic() {
            return false;
        }

        @Override
        public boolean isPrivate() {
            return false;
        }

        @Override
        public boolean isPublic() {
            return false;
        }

        @Override
        public boolean isProtected() {
            return false;
        }

        @Override
        public boolean isFinal() {
            return false;
        }

        @Override
        public boolean isAbstract() {
            return false;
        }

        public static String getBoxedTypeName(ClassDescriptor c) {
            String name = null;
            if (c.isPrimitive()) {
                char ch = c.getSignature().charAt(0);
                switch (ch) {
                case 'V':
                    name = Void.class.getName();
                    break;
                case 'Z':
                    name = Boolean.class.getName();
                    break;
                case 'C':
                    name = Character.class.getName();
                    break;
                case 'B':
                    name = Byte.class.getName();
                    break;
                case 'S':
                    name = Short.class.getName();
                    break;
                case 'I':
                    name = Integer.class.getName();
                    break;
                case 'J':
                    name = Long.class.getName();
                    break;
                case 'F':
                    name = Float.class.getName();
                    break;
                case 'D':
                    name = Double.class.getName();
                    break;
                }
            }
            return name;
        }
    }
    boolean isPrimitive();

    boolean isArray();

    boolean isInterface();

    String getSignature();

    String getName();

    MethodDescriptor[] getConstructors();

    ClassDescriptor getComponentType();

    MethodDescriptor[] getDeclaredMethods();

    ClassDescriptor[] getInterfaces();

    ClassDescriptor getSuperclass();

    static ClassDescriptor VOID = new PrimitiveTypeInfo("V");
    static ClassDescriptor BOOLEAN = new PrimitiveTypeInfo("Z");
    static ClassDescriptor CHAR = new PrimitiveTypeInfo("C");
    static ClassDescriptor BYTE = new PrimitiveTypeInfo("B");
    static ClassDescriptor SHORT = new PrimitiveTypeInfo("S");
    static ClassDescriptor INT = new PrimitiveTypeInfo("I");
    static ClassDescriptor LONG = new PrimitiveTypeInfo("J");
    static ClassDescriptor FLOAT = new PrimitiveTypeInfo("F");
    static ClassDescriptor DOUBLE = new PrimitiveTypeInfo("D");
}
