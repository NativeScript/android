package com.telerik.metadata.parsing;

import com.telerik.metadata.security.classes.SecuredClassRepository;
import com.telerik.metadata.security.classes.SecuredNativeClassDescriptor;

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
            SecuredNativeClassDescriptor securedNativeClassDescriptor = SecuredClassRepository.INSTANCE.findClass(implementedInterfaceName);

            if (!securedNativeClassDescriptor.isUsageAllowed()) {
                System.out.println(String.format("WARNING: Skipping interface %s implemented in %s as it cannot be resolved", implementedInterfaceName, clazz.getClassName()));
                continue;
            }

            NativeClassDescriptor interfaceClass = securedNativeClassDescriptor.getNativeDescriptor();

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
