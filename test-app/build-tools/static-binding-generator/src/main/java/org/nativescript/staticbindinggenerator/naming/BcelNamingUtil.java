package org.nativescript.staticbindinggenerator.naming;

import org.apache.bcel.generic.Type;

public final class BcelNamingUtil {

    public static String resolveClassName(String className){
        return className.replace('$', '.');
    }

    public static String resolveBcelTypeName(Type type){
        return resolveBcelTypeName(type.toString());
    }

    public static String resolveBcelTypeName(String typeSignature){
        return typeSignature.replace('$', '.');
    }

    private BcelNamingUtil(){}
}