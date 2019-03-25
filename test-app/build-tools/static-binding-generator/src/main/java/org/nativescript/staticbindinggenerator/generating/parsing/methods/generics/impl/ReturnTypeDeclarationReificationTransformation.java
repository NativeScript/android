package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodDeclarationTransformation;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ReturnTypeDeclarationReificationTransformation implements MethodDeclarationTransformation<String> {

    private final GenericParameters genericParametersToUse;

    public ReturnTypeDeclarationReificationTransformation(GenericParameters genericParametersToUse) {
        this.genericParametersToUse = genericParametersToUse;
    }

    @Override
    public String transform(String input) {
        return reifyReturnTypeIfNecessary(input);
    }

    private String reifyReturnTypeIfNecessary(String returnType) {
        String reifiedReturnType = returnType;

        for (Map.Entry<String, String> resolvedFormalTypeParameter : genericParametersToUse.getGenericParameters().entrySet()) {
            if (reifiedReturnType.equals(resolvedFormalTypeParameter.getKey())) {
                return resolvedFormalTypeParameter.getValue();
            } else {
                Pattern pattern = Pattern.compile("([^\\w\\.])" + resolvedFormalTypeParameter.getKey() + "(\\W)");
                Matcher m = pattern.matcher(reifiedReturnType);
                if (m.find()) {
                    String replacement = BcelNamingUtil.resolveClassName(resolvedFormalTypeParameter.getValue());
                    reifiedReturnType = m.replaceAll("$1" + replacement + "$2");
                }
            }
        }

        return reifiedReturnType.replaceAll("\\$", ".");
    }
}
