package org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.methods.generics.MethodDeclarationTransformation;

import java.util.regex.Pattern;

public class WildcardBoundsTransformation implements MethodDeclarationTransformation<String> {

    private static final Pattern UNBOUNDED_WILDCARD_PATTERN = Pattern.compile("\\?(?! [es])"); // matches declarations like <?,?> but not <? extends SomeType> ot <? super SomeType>

    @Override
    public String transform(String input) {
        return replaceAllUnboundedWildcardsIfAny(input);
    }

    private String replaceAllUnboundedWildcardsIfAny(String declaration) {
        return UNBOUNDED_WILDCARD_PATTERN.matcher(declaration).replaceAll("? extends java.lang.Object");
    }
}
