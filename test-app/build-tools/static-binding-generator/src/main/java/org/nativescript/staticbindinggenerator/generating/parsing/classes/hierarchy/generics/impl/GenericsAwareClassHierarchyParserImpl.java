package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import org.apache.bcel.classfile.JavaClass;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.InheritanceChain;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericsAwareClassHierarchyParser;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class GenericsAwareClassHierarchyParserImpl implements GenericsAwareClassHierarchyParser {

    private static final Pattern GENERICS_ERASURE_PATTERN = Pattern.compile("<[^\\.]*>");

    private static final Map<JavaClass, GenericHierarchyView> HIERARCHIES_CACHE = new HashMap<>();

    private static final Map<String, Pattern> NESTED_GENERICS_REGEX_CACHE = new HashMap<>();

    private final GenericSignatureReader genericSignatureReader;
    private final InheritanceChain inheritanceChain;
    private final Map<String, JavaClass> classesCache;

    public GenericsAwareClassHierarchyParserImpl(GenericSignatureReader genericSignatureReader, InheritanceChain inheritanceChain, Map<String, JavaClass> classesCache) {
        this.genericSignatureReader = genericSignatureReader;
        this.inheritanceChain = inheritanceChain;
        this.classesCache = classesCache;
    }

    @Override
    public GenericHierarchyView getClassHierarchy(JavaClass javaClass) {
        GenericHierarchyView cachedHierarchy = HIERARCHIES_CACHE.get(javaClass);
        if (cachedHierarchy != null) {
            return cachedHierarchy;
        }

        ArrayDeque<GenericSignatureView> collectedGenericSignatureViews = new ArrayDeque<>();
        HashMap<String, GenericParameters> collectedParentClasses = new HashMap<>();
        HashMap<String, GenericParameters> collectedParentInterfaces = new HashMap<>();
        GenericParameters initialClassGenericParameters = null;
        HashSet<JavaClass> visitedInterfaces = new HashSet<>();

        GenericSignatureView genericSignatureView = genericSignatureReader.readGenericSignature(javaClass);

        if (hasGenericSignature(genericSignatureView)) {
            if (genericSignatureView.hasFormalTypeParameters()) {
                if (inheritanceChain == null) {
                    GenericParameters erasedFormalParameters = getGenericParametersForInitialClass(genericSignatureView);
                    GenericSignatureView reifiedSignatureView = reifyGenericSignatureView(genericSignatureView, erasedFormalParameters);
                    collectedGenericSignatureViews.addLast(reifiedSignatureView);
                } else {
                    GenericParameters genericParameters = parseProvidedInheritanceChain(inheritanceChain.getExtendedClass(), genericSignatureView.getFormalTypeParameterNames());
                    initialClassGenericParameters = genericParameters;
                    GenericSignatureView reifiedSignatureView = reifyGenericSignatureView(genericSignatureView, genericParameters);
                    collectedGenericSignatureViews.addLast(reifiedSignatureView);
                }
            } else {
                collectedGenericSignatureViews.addLast(genericSignatureView);
            }
        }

        getClassHierarchyRecursively(javaClass, collectedParentClasses, collectedParentInterfaces, visitedInterfaces, collectedGenericSignatureViews);

        return new GenericHierarchyView(collectedParentInterfaces, collectedParentClasses, javaClass.getClassName(), initialClassGenericParameters);
    }

    private static GenericParameters parseProvidedInheritanceChain(String extendedClassName, List<String> formalTypeParameters){
        String signature = extendedClassName.substring(extendedClassName.indexOf('<') + 1, extendedClassName.length() - 1);
        List<String> args = getReifiedArgumentsFromReifiedDeclaration((signature));
        Map<String, String> res = new HashMap<>();

        for(int i=0; i<formalTypeParameters.size(); i+=1){
            res.put(formalTypeParameters.get(i), args.get(i));
        }

        return new GenericParameters(res);
    }

    //////////////////////////////////////////////////////////////////////
    private static List<String> getReifiedArgumentsFromReifiedDeclaration(String reifiedDeclaration) {
        List<String> res = new ArrayList<>();
        StringBuilder reifiedArgumentBuffer = new StringBuilder();
        int openingAngleBracketsCounter = 0;
        int closingAngleBracketsCounter = 0;

        for (int i = 0; i < reifiedDeclaration.length(); i += 1) {
            char c = reifiedDeclaration.charAt(i);
            if (c == '<') {
                openingAngleBracketsCounter += 1;
                reifiedArgumentBuffer.append(c);
            } else if (c == '>') {
                closingAngleBracketsCounter += 1;
                reifiedArgumentBuffer.append(c);
            } else if (c == ',') {
                if (openingAngleBracketsCounter == closingAngleBracketsCounter) {
                    res.add(reifiedArgumentBuffer.toString());
                    reifiedArgumentBuffer = new StringBuilder();
                    openingAngleBracketsCounter = 0;
                    closingAngleBracketsCounter = 0;
                } else {
                    reifiedArgumentBuffer.append(c);
                }
            } else {
                reifiedArgumentBuffer.append(c);
            }
        }
        if (reifiedArgumentBuffer.length() > 0) {
            res.add(reifiedArgumentBuffer.toString());
        }

        return res;
    }
    //////////////////////////////////////////////////////////////////////


    private GenericParameters getGenericParametersForInitialClass(GenericSignatureView genericSignatureView) {
        Map<String, String> resolvedParameters = new HashMap<>();
        List<String> formalTypeParameters = genericSignatureView.getFormalTypeParameterNames();
        Map<String, String> formalTypeParametersBounds = genericSignatureView.getFormalTypeParameterBounds();
        List<Integer> parametersToLookupLater = new ArrayList<>();

        for (int i = 0; i < formalTypeParameters.size(); i += 1) {
            String formalTypeParameter = formalTypeParameters.get(i);
            String bound = formalTypeParametersBounds.get(formalTypeParameter);
            if (bound != null && !bound.equals("")) {
                boolean hasTypeParamInBound = false;
                for (String typeParam : formalTypeParameters) {
                    if (hasFormalTypeParameterInsideBound(bound, typeParam)) {
                        hasTypeParamInBound = true;
                        break;
                    }
                }

                if (hasTypeParamInBound) {
                    parametersToLookupLater.add(i);
                } else {
                    String erasure = getGenericsErasure(bound);
                    resolvedParameters.put(formalTypeParameter, erasure);
                }
            } else {
                resolvedParameters.put(formalTypeParameter, "java.lang.Object");
            }

        }

        while (parametersToLookupLater.size() > 0) {
            ListIterator<Integer> iter = parametersToLookupLater.listIterator();
            while (iter.hasNext()) {
                int index = iter.next();
                String typeParameter = formalTypeParameters.get(index);
                String bound = formalTypeParametersBounds.get(typeParameter);
                boolean successfullyResolved = false;

                for (Map.Entry<String, String> resolvedParameter : resolvedParameters.entrySet()) {
                    if (hasFormalTypeParameterInsideBound(bound, resolvedParameter.getKey())) {
                        String erasure = getReifiedNestedGenericArgument(bound, resolvedParameter.getKey(), resolvedParameter.getValue());
                        resolvedParameters.put(typeParameter, erasure);
                        successfullyResolved = true;
                        break;
                    }
                }

                if (successfullyResolved) {
                    iter.remove();
                }
            }
        }

        return new GenericParameters(resolvedParameters);
    }

    private String getGenericsErasure(String genericArgument) {
        return GENERICS_ERASURE_PATTERN.matcher(genericArgument).replaceAll("");
    }

    private void getClassHierarchyRecursively(JavaClass forClass, HashMap<String, GenericParameters> collectedParentClasses, HashMap<String, GenericParameters> collectedParentInterfaces, HashSet<JavaClass> visitedInterfaces, ArrayDeque<GenericSignatureView> genericSignatureViewsStack) {
        if (forClass.getClassName().equals("java.lang.Object")) {
            return;
        }

        String parentClassName = forClass.getSuperclassName();

        if (!parentClassName.equals("java.lang.Object")) {
            JavaClass parentClass = getJavaClassFromCache(parentClassName);
            GenericSignatureView parentClassGenericSignatureView = genericSignatureReader.readGenericSignature(parentClass);

            if (hasGenericSignature(parentClassGenericSignatureView)) {
                GenericParameters reifiedClass = reifyGenericClass(parentClass, parentClassGenericSignatureView, genericSignatureViewsStack);
                collectedParentClasses.put(parentClassName, reifiedClass);
                GenericSignatureView reifiedSignatureView = reifyGenericSignatureView(parentClassGenericSignatureView, reifiedClass);
                genericSignatureViewsStack.addLast(reifiedSignatureView);
            }

            getClassHierarchyRecursively(parentClass, collectedParentClasses, collectedParentInterfaces, visitedInterfaces, genericSignatureViewsStack);
        }


        for (String parentInterfaceName : forClass.getInterfaceNames()) {
            JavaClass interfaze = getJavaClassFromCache(parentInterfaceName);

            if (!visitedInterfaces.contains(interfaze)) {
                GenericSignatureView parentInterfaceGenericSignatureView = genericSignatureReader.readGenericSignature(interfaze);

                if (hasGenericSignature(parentInterfaceGenericSignatureView)) {
                    GenericParameters reifiedClass = reifyGenericClass(interfaze, parentInterfaceGenericSignatureView, genericSignatureViewsStack);
                    collectedParentInterfaces.put(parentInterfaceName, reifiedClass);
                    GenericSignatureView reifiedSignatureView = reifyGenericSignatureView(parentInterfaceGenericSignatureView, reifiedClass);
                    genericSignatureViewsStack.add(reifiedSignatureView);
                }

                visitedInterfaces.add(interfaze);
                getClassHierarchyRecursively(interfaze, collectedParentClasses, collectedParentInterfaces, visitedInterfaces, genericSignatureViewsStack);
            }
        }

        if (genericSignatureViewsStack.size() > 0) {
            genericSignatureViewsStack.removeLast();
        }
    }

    private GenericParameters reifyGenericClass(JavaClass javaClass, GenericSignatureView genericSignatureView, ArrayDeque<GenericSignatureView> genericSignatureViewsStack) {
        if (genericSignatureViewsStack.size() != 0) {
            GenericSignatureView previousSignature = genericSignatureViewsStack.peekLast();
            ClassGenericDefinition genericDefinitionFromPreviousSignature = getGenericDefinitionFromView(javaClass, previousSignature);

            List<String> providedGenericArguments = genericDefinitionFromPreviousSignature.getGenericArguments();
            List<String> currentClassFormalTypeParameters = genericSignatureView.getFormalTypeParameterNames();

            Map<String, String> genericFormalParameters = new HashMap<>();

            for (int i = 0; i < currentClassFormalTypeParameters.size(); i += 1) {
                genericFormalParameters.put(currentClassFormalTypeParameters.get(i), providedGenericArguments.get(i));
            }

            return new GenericParameters(genericFormalParameters);
        }

        return new GenericParameters();
    }

    private List<String> mapValuesToList(Map<String, String> map) {
        List<String> values = new ArrayList<>();
        for (Map.Entry<String, String> entry : map.entrySet()) {
            values.add(entry.getValue());
        }
        return values;
    }

    private GenericSignatureView reifyGenericSignatureView(GenericSignatureView genericSignatureView, GenericParameters reifiedClass) {
        Map<String, String> formalTypeParameters = reifiedClass.getGenericParameters();
        List<String> formalTypeParameterValues = mapValuesToList(formalTypeParameters);

        if (formalTypeParameters.size() > 0) {
            List<ClassGenericDefinition> currentParentInterfacesDefinitions = genericSignatureView.getParentInterfaces();
            List<ClassGenericDefinition> reifiedParentInterfaces = new ArrayList<>();
            ClassGenericDefinition currentParentClassDefinition = genericSignatureView.getParentClass();
            ClassGenericDefinition reifiedParentClass = null;

            if (currentParentClassDefinition != null) {
                reifiedParentClass = getReifiedParentDefinition(currentParentClassDefinition, formalTypeParameters);
            }

            if (currentParentInterfacesDefinitions != null && currentParentInterfacesDefinitions.size() > 0) {
                for (ClassGenericDefinition currentParentInterfaceDefinition : currentParentInterfacesDefinitions) {
                    ClassGenericDefinition reifiedGenericInterface = getReifiedParentDefinition(currentParentInterfaceDefinition, formalTypeParameters);

                    reifiedParentInterfaces.add(reifiedGenericInterface);
                }
            }


            return new GenericSignatureView(genericSignatureView.getParsedSignature(), formalTypeParameterValues, genericSignatureView.getFormalTypeParameterBounds(), reifiedParentClass, reifiedParentInterfaces);
        }

        return genericSignatureView;
    }

    private ClassGenericDefinition getReifiedParentDefinition(ClassGenericDefinition currentParentDefinition, Map<String, String> resolvedFormalTypeParameters) {
        ClassGenericDefinition reifiedGenericInterface = new ClassGenericDefinition(currentParentDefinition.getClassName());

        outer:
        for (String genericArgument : currentParentDefinition.getGenericArguments()) {
            String currentReifiedGeneric = null;

            for (Map.Entry<String, String> formalTypeParameter : resolvedFormalTypeParameters.entrySet()) {

                if (formalTypeParameter.getKey().equals(genericArgument)) { // handle cases like Gen<T> extends Gen2<T>
                    reifiedGenericInterface.addGenericArgument(formalTypeParameter.getValue());
                    continue outer;
                } else { // try lookup for nested usage of generic arguments; e.g. Gen<T> extends Gen2<List<T>>
                    String possibleResolvedNestedGenericArgument;
                    if (currentReifiedGeneric == null) { // first reification attempt, let's use genericArgument
                        possibleResolvedNestedGenericArgument = getReifiedNestedGenericArgument(genericArgument, formalTypeParameter.getKey(), formalTypeParameter.getValue());
                    } else { // already got a reification result, let's use currentReifiedGeneric
                        possibleResolvedNestedGenericArgument = getReifiedNestedGenericArgument(currentReifiedGeneric, formalTypeParameter.getKey(), formalTypeParameter.getValue());
                    }

                    if (possibleResolvedNestedGenericArgument != null) { // had success with reificating nested generic argument, store it
                        currentReifiedGeneric = possibleResolvedNestedGenericArgument;
                    }
                }
            }

            if (currentReifiedGeneric != null) {
                reifiedGenericInterface.addGenericArgument(currentReifiedGeneric);
            } else {
                reifiedGenericInterface.addGenericArgument(genericArgument);
            }

        }

        return reifiedGenericInterface;
    }

    private String getReifiedNestedGenericArgument(String genericArgument, String providedFormalTypeParameterKey, String providedFormalTypeParameterValue) {
        Pattern p = getNestedGenericArgumentRegexPattern(providedFormalTypeParameterKey);

        Matcher m = p.matcher(genericArgument);
        if (m.find()) {
            return m.replaceAll("$1" + providedFormalTypeParameterValue + "$2");
        }

        return null;
    }

    private boolean hasFormalTypeParameterInsideBound(String bound, String formalTypeParameter) {
        Pattern p = getNestedGenericArgumentRegexPattern(formalTypeParameter);
        return p.matcher(bound).matches();
    }

    private Pattern getNestedGenericArgumentRegexPattern(String providedFormalTypeParameterKey) {
        Pattern cachedPattern = NESTED_GENERICS_REGEX_CACHE.get(providedFormalTypeParameterKey);

        if (cachedPattern == null) {
            Pattern pattern = Pattern.compile("([^\\w\\[]*)" + providedFormalTypeParameterKey + "([^\\w\\[]*)");
            NESTED_GENERICS_REGEX_CACHE.put(providedFormalTypeParameterKey, pattern);
            return pattern;
        }

        return cachedPattern;
    }

    private ClassGenericDefinition getGenericDefinitionFromView(JavaClass javaClass, GenericSignatureView genericSignatureView) {
        if (javaClass.isInterface()) {
            for (ClassGenericDefinition genericDefinition : genericSignatureView.getParentInterfaces()) {
                if (genericDefinition.getClassName().equals(javaClass.getClassName())) {
                    return genericDefinition;
                }
            }

            return null;
        } else {
            return genericSignatureView.getParentClass();
        }
    }

    private boolean hasGenericSignature(GenericSignatureView possibleGenericSignature) {
        return possibleGenericSignature != null;
    }

    private JavaClass getJavaClassFromCache(String className) {
        JavaClass javaClass = classesCache.get(BcelNamingUtil.resolveClassName(className));

        if (javaClass == null) {
            throw new RuntimeException("Class not found!");
        }

        return javaClass;
    }

    Map<String, JavaClass> getClassesCache() {
        return classesCache;
    }


}
