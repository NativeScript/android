package com.tns.system.classes.generating;

import java.lang.reflect.InvocationHandler;
import java.util.List;

public interface RuntimeBindingGenerator {

    Class<?> generateBinding(String extendedClassName, List<String> implementedInterfacesNames, List<String> overriddenMethodsNames);

    boolean isRuntimeGeneratedBinding(Object object);

    void setInvocationHandlerToBinding(Object object, InvocationHandler invocationHandler);
}
