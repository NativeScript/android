package com.tns.system.classloaders;

import java.util.Collection;

public interface ClassLoadersCollection {
    void addClassLoader(ClassLoader classLoader);
    Collection<ClassLoader> getClassLoadersCollection();
}
