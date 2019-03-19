package org.nativescript.staticbindinggenerator.generating.writing;

public interface JavaCodeWriter {
    char SPACE_LITERAL = ' ';
    char TABULATION_LITERAL = '\t';
    char COMMA_LITERAL = ',';
    char EQUALS_LITERAL = '=';
    char END_OF_STATEMENT_LITERAL = ';';
    char OPENING_CURLY_BRACKET_LITERAL = '{';
    char CLOSING_CURLY_BRACKET_LITERAL = '}';
    char OPENING_ROUND_BRACKET_LITERAL = '(';
    char CLOSING_ROUND_BRACKET_LITERAL = ')';
    char OPENING_SQUARE_BRACKET_LITERAL = '[';
    char CLOSING_SQUARE_BRACKET_LITERAL = ']';

    String PRIVATE_MODIFIER = "private";
    String PUBLIC_MODIFIER = "public";
    String PROTECTED_MODIFIER = "protected";
    String STATIC_MODIFIER = "static";
}
