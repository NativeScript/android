package com.telerik.metadata.parsing;

import com.telerik.metadata.ClassRepo;

import java.util.HashSet;
import java.util.Set;

public final class ClassParser {

    private final NativeClassDescriptor clazz;

    private ClassParser(NativeClassDescriptor clazz) {
        this.clazz = clazz;
    }

    public static ClassParser forClassDescriptor(NativeClassDescriptor clazz) {
        return new ClassParser(clazz);
    }

    public Set<NativeMethodDescriptor> getAllDefaultMethodsFromImplementedInterfaces() {
        return getAllDefaultMethodsFromImplementedInterfacesRecursively(clazz, new HashSet<NativeMethodDescriptor>());
    }

    private HashSet<NativeMethodDescriptor> getAllDefaultMethodsFromImplementedInterfacesRecursively(NativeClassDescriptor clazz, HashSet<NativeMethodDescriptor> collectedDefaultMethods) {
        String[] implementedInterfacesNames = clazz.getInterfaceNames();

        for (String implementedInterfaceName : implementedInterfacesNames) {
            NativeClassDescriptor interfaceClass = ClassRepo.findClass(implementedInterfaceName);

            if (interfaceClass == null) {
                System.out.println(String.format("WARNING: Skipping interface %s implemented in %s as it cannot be resolved", implementedInterfaceName, clazz.getClassName()));
                continue;
            }

            for (NativeMethodDescriptor md : interfaceClass.getMethods()) {
                if (!md.isStatic() && !md.isAbstract()) {
                    collectedDefaultMethods.add(md);
                }
            }

            collectedDefaultMethods.addAll(getAllDefaultMethodsFromImplementedInterfacesRecursively(interfaceClass, new HashSet<NativeMethodDescriptor>()));
        }

        return collectedDefaultMethods;
    }
}
