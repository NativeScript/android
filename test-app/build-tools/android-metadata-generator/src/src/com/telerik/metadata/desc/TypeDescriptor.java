package com.telerik.metadata.desc;

public interface TypeDescriptor {
    String getSignature();

    TypeDescriptor VOID = new PrimitiveType("V");
    TypeDescriptor CHAR = new PrimitiveType("C");
    TypeDescriptor BOOLEAN = new PrimitiveType("Z");
    TypeDescriptor BYTE = new PrimitiveType("B");
    TypeDescriptor SHORT = new PrimitiveType("S");
    TypeDescriptor INT = new PrimitiveType("I");
    TypeDescriptor LONG = new PrimitiveType("J");
    TypeDescriptor FLOAT = new PrimitiveType("F");
    TypeDescriptor DOUBLE = new PrimitiveType("D");

    class PrimitiveType implements TypeDescriptor {
        private final String signature;

        private PrimitiveType(String signature) {
            this.signature = signature;
        }

        @Override
        public String getSignature() {
            return signature;
        }
    }
}
