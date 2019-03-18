package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy;

import java.util.Set;

public class HierarchyView {
    private final Set<String> allImplementedInterfacesNames;
    private final Set<String> allParentClassesNames;

    public HierarchyView(Set<String> allExtendedClassesNames, Set<String> allImplementedInterfacesNames) {
        this.allParentClassesNames = allExtendedClassesNames;
        this.allImplementedInterfacesNames = allImplementedInterfacesNames;
    }

    public Set<String> getAllImplementedInterfacesNames() {
        return allImplementedInterfacesNames;
    }

    public Set<String> getAllParentClassesNames() {
        return allParentClassesNames;
    }

    @Override
    public String toString() {
        return "HierarchyView{" +
                "allImplementedInterfacesNames=" + stringSetToString(allImplementedInterfacesNames) +
                ", allParentClassesNames=" + stringSetToString(allParentClassesNames) +
                '}';
    }

    private String stringSetToString(Set<String> set){
        StringBuilder sb = new StringBuilder();

        for(String str: set){
            sb.append(str);
            sb.append(System.lineSeparator());
        }

        return sb.toString();
    }
}
