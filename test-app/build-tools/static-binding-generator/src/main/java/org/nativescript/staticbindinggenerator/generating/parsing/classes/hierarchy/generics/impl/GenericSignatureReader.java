package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import org.apache.bcel.classfile.Attribute;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Signature;
import org.nativescript.staticbindinggenerator.generating.writing.impl.TraceSignatureVisitor;
import org.objectweb.asm.signature.SignatureReader;
import org.objectweb.asm.signature.SignatureVisitor;

public class GenericSignatureReader {

    GenericSignatureView readGenericSignature(JavaClass javaClass) {
        String classSignature = tryGetGenericSignature(javaClass);

        if (isGenericClass(classSignature)) {
            final SignatureReader reader = new SignatureReader(classSignature);
            ClassSignatureVisitor parser = new ClassSignatureVisitor(javaClass.getClassName(), javaClass.getAccessFlags());
            reader.accept(parser);
            return parser.getGenericSignatureView();
        }

        return null;
    }

    private String tryGetGenericSignature(JavaClass javaClass) {
        Attribute[] attributes = javaClass.getAttributes();
        for (Attribute attribute : attributes) {
            if (attribute instanceof Signature) {
                Signature signature = (Signature) attribute;
                return signature.getSignature();
            }
        }

        return null;
    }

    private boolean isGenericClass(String signature) {
        return signature != null && !signature.equals("");
    }
}
