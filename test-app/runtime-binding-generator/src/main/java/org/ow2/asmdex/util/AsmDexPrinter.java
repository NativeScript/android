/* Software Name : AsmDex
 * Version : 1.0
 *
 * Copyright © 2012 France Télécom
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.ow2.asmdex.util;

import java.util.List;

/**
 * An abstract visitor for the AsmDexifier.
 *
 * @author Julien Névo, based on the ASM framework.
 */
public class AsmDexPrinter {

    /**
     * Strings for access flags. ACC_BRIDGE and ACC_VARARGS are omitted because they have the
     * same value as ACC_VOLATILE and ACC_TRANSIENT respectively. Using one instead of another
     * shouldn't be a problem...
     */
    private static String[] accessStrings = new String[] { "ACC_PUBLIC", "ACC_PRIVATE", "ACC_PROTECTED",
            "ACC_STATIC", "ACC_FINAL", "ACC_SYNCHRONIZED", "ACC_VOLATILE", /*"ACC_BRIDGE", */"ACC_TRANSIENT",
            /*"ACC_VARARGS", */"ACC_NATIVE", "ACC_INTERFACE", "ACC_ABSTRACT", "ACC_STRICT", "ACC_SYNTHETIC",
            "ACC_ANNOTATION", "ACC_ENUM", "", "ACC_CONSTRUCTOR", "ACC_DECLARED_SYNCHRONIZED"
                                                         };

    /**
     * The tabulation of the current element.
     */
    protected int currentTabulation = 0;

    /**
     * A buffer that can be used to create strings.
     */
    protected StringBuffer text = new StringBuffer();

    /**
     * The text to be printed. Since the code of methods is not necessarily
     * visited in sequential order, one method after the other, but can be
     * interlaced (some instructions from method one, then some instructions
     * from method two, then some instructions from method one again...), it is
     * not possible to print the visited instructions directly to a sequential
     * stream. A class is therefore printed in a two steps process: a string
     * tree is constructed during the visit, and printed to a sequential stream
     * at the end of the visit. This tree is stored in this field, as a
     * Composite that can contain other Composite, which can themselves
     * contain other Composite, and so on.
     */
    private TextComposite textComposite = new TextComposite();

    /**
     * Returns the Text Component this element holds.
     * @return the Text Component this element holds.
     */
    protected TextComponent getTextComponent() {
        return textComposite;
    }

    /**
     * Adds the current text to the list of texts, and resets the text.
     * Must be done for each text (or part of it) that is over.
     */
    public void closeText() {
        TextLeaf component = new TextLeaf(text);
        textComposite.addComponent(component);
        text = new StringBuffer();
    }

    /**
     * Adds the given Text Component to the list of this element.
     * @param tc the Text Component to add.
     */
    public void addTextToList(TextComponent tc) {
        textComposite.addComponent(tc);
    }

    /**
     * Appends a comma and a space to the current text.
     */
    public void addComma() {
        text.append(", ");
    }

    /**
     * Adds an End Of Line tag at the end of the current text.
     */
    public void addEOL() {
        text.append("\n");
    }

    /**
     * Adds a boolean string to the current text. This last one needs to be closed (with the
     * closeText() method) in order to be actually used.
     * @param bool the boolean value to encode.
     * @param addComma true to add a comma at the end of the text.
     */
    public void addBoolean(boolean bool, boolean addComma) {
        addText(bool ? "true" : "false", addComma);
    }

    /**
     * Adds a given text to the current text. This last one needs to be closed (with the
     * closeText() method) in order to be actually used.
     * @param textToAdd the text to add.
     */
    public void addText(String textToAdd) {
        text.append(textToAdd);
    }

    /**
     * Adds a given text to the current text. This last one needs to be closed (with the
     * closeText() method) in order to be actually used.
     * @param textToAdd the text to add.
     * @param addComma true to add a comma at the end of the text.
     */
    public void addText(String textToAdd, boolean addComma) {
        addText(textToAdd);
        if (addComma) {
            addComma();
        }
    }

    /**
     * Appends a string representation of the given constant to the given
     * buffer, with quotes if necessary.
     *
     * @param cst a single or array of ints, {@link Integer}, {@link Float}, {@link Long},
     *        {@link Double} or {@link String} object. May be <tt>null</tt>.
     * @param addComma true to add a comma at the end.
     */
    public void addConstant(final Object cst, boolean addComma) {

        // Some primitive types must be boxed because it is often the only way for AsmDex to know their
        // type (ex : annotationVisitor.visit requires an Object. Floats, doubles and longs have prefixes
        // and int is used by default. But shorts, bytes and characters must be boxed.

        // Primitives inside Arrays are written directly without boxing, with the prefixes if needed.
        if (cst == null) {
            text.append("null");
        } else if (cst instanceof String) {
            appendString((String)cst);
        } else if (cst instanceof Integer) {
            text.append(cst);
        } else if (cst instanceof Boolean) {
            text.append(cst);
        } else if (cst instanceof Byte) {
            text.append("Byte.valueOf((byte)").append(cst).append(')');
        } else if (cst instanceof Character) {
            int c = ((Character)cst).charValue();
            text.append("Character.valueOf((char)").append(c).append(')');
        } else if (cst instanceof Short) {
            text.append("Short.valueOf((short)").append(cst).append(')');
        } else if (cst instanceof Double) {
            addNumber((Double)cst, false);
        } else if (cst instanceof Float) {
            addNumber((Float)cst, false);
        } else if (cst instanceof Long) {
            addNumber((Long)cst, false);

            // Tests arrays.
        } else if (cst instanceof int[]) {
            text.append("new int[] { ");
            int[] ints = (int[])cst;
            for (int i = 0, size = ints.length; i < size; i++) {
                addNumber(ints[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof boolean[]) {
            text.append("new boolean[] { ");
            boolean[] array = (boolean[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addBoolean(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof byte[]) {
            text.append("new byte[] { ");
            byte[] array = (byte[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof short[]) {
            text.append("new short[] { ");
            short[] array = (short[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof char[]) {
            text.append("new char[] { ");
            char[] array = (char[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof float[]) {
            text.append("new float[] { ");
            float[] array = (float[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof double[]) {
            text.append("new double[] { ");
            double[] array = (double[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof long[]) {
            text.append("new long[] { ");
            long[] array = (long[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof String[]) {
            text.append("new String[] { ");
            String[] strings = (String[])cst;
            for (int i = 0, size = strings.length; i < size; i++) {
                addConstant(strings[i], (i != (size - 1)));
            }
            text.append(" }");
        }


        // Manages Object[] arrays. This is only useful for FillArrayData instructions.

        else if (cst instanceof Byte[]) {
            text.append("new Byte[] { ");
            Byte[] array = (Byte[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Integer[]) {
            text.append("new Integer[] { ");
            Integer[] ints = (Integer[])cst;
            for (int i = 0, size = ints.length; i < size; i++) {
                addNumber(ints[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Boolean[]) {
            text.append("new Boolean[] { ");
            Boolean[] array = (Boolean[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addBoolean(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Short[]) {
            text.append("new Short[] { ");
            Short[] array = (Short[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Character[]) {
            text.append("new Character[] { ");
            Character[] array = (Character[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Float[]) {
            text.append("new Float[] { ");
            Float[] array = (Float[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Double[]) {
            text.append("new Double[] { ");
            Double[] array = (Double[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        } else if (cst instanceof Long[]) {
            text.append("new Long[] { ");
            Long[] array = (Long[])cst;
            for (int i = 0, size = array.length; i < size; i++) {
                addNumber(array[i], (i != (size - 1)));
            }
            text.append(" }");
        }



        else {
            try {
                throw new Exception ("Unhandled constant type.");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        if (addComma) {
            addComma();
        }

    }

    /**
     * Adds the declaration of an ArrayList of Strings to the current text.
     * @param list the List of Strings to add.
     * @param addComma true to add a comma at the end.
     */
    public void addStringArrayList(final List<String> list, boolean addComma) {
        if (list == null) {
            text.append("null");
        } else {
            text.append("new ArrayList<Label>(Arrays.asList(");
            int i = 0;
            for (String string : list) {
                if (i != 0) {
                    text.append(", ");
                }
                text.append(string);
                i++;
            }
            text.append("))");
        }

        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds the declaration of an Array of Labels to the current text, from their names.
     * @param labelNames the names of the Labels.
     * @param addComma true to add a comma at the end.
     */
    public void addLabelArray(final String[] labelNames, boolean addComma) {
        if (labelNames == null) {
            addText("null", addComma);
        } else {
            addText("new Label[] { ");

            boolean isFirst = true;
            for (String string : labelNames) {
                if (!isFirst) {
                    addComma();
                }
                addText(string);
                isFirst = false;
            }

            addText(" }", addComma);
        }


    }

    /**
     * Appends the Access flags to the given buffer.
     * @param accessFlags the access flags to encode.
     * @param addComma true to add a comma at the end.
     */
    public void addAccessFlags(int accessFlags, boolean addComma) {
        if (accessFlags == 0) {
            text.append('0');
        } else {
            boolean isFirst = true;
            int accessIndex = 0;
            while (accessFlags != 0) {
                if ((accessFlags & 1) != 0) {
                    if (!isFirst) {
                        text.append(" + ");
                    }
                    text.append(accessStrings[accessIndex]);
                    isFirst = false;
                }
                accessFlags >>>= 1;
                accessIndex++;
            }
        }

        if (addComma) {
            addComma();
        }
    }

    /**
     * Appends a quoted string to a given buffer.
     *
     *
     * @param s the string to be added.
     */
    public void appendString(final String s) {
        text.append('\"');
        for (int i = 0; i < s.length(); ++i) {
            char c = s.charAt(i);
            if (c == '\n') {
                text.append("\\n");
            } else if (c == '\r') {
                text.append("\\r");
            } else if (c == '\\') {
                text.append("\\\\");
            } else if (c == '"') {
                text.append("\\\"");
            } else if (c < 0x20 || c > 0x7f) {
                text.append("\\u");
                if (c < 0x10) {
                    text.append("000");
                } else if (c < 0x100) {
                    text.append("00");
                } else if (c < 0x1000) {
                    text.append('0');
                }
                text.append(Integer.toString(c, 16));
            } else {
                text.append(c);
            }
        }
        text.append('\"');
    }

    /**
     * Adds a number at the end of the current text.
     * @param nb the number to add.
     * @param addComma true to add a comma at the end.
     */
    public void addNumber(int nb, boolean addComma) {
        text.append(nb);
        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds a Float number at the end of the current text.
     * @param nb the number to add.
     * @param addComma true to add a comma at the end.
     */
    public void addNumber(float nb, boolean addComma) {
        text.append(nb);
        text.append('f');
        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds a long number at the end of the current text.
     * @param nb the number to add.
     * @param addComma true to add a comma at the end.
     */
    public void addNumber(long nb, boolean addComma) {
        text.append(nb);
        text.append('L');
        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds a long number at the end of the current text.
     * @param nb the number to add.
     * @param addComma true to add a comma at the end.
     */
    public void addNumber(double nb, boolean addComma) {
        text.append(nb);
        text.append('d');
        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds to the current text the name of the given opcode, or the number in hexadecimal
     * if the opcode isn't known.
     * @param opcode the opcode.
     * @param addComma true to add a comma after the opcode name.
     */
    public void addOpcode(int opcode, boolean addComma) {
        String string = AsmDexifierApplicationVisitor.getOpcodeName(opcode);
        if (string == null) {
            string = "0x" + Integer.toHexString(opcode);
        }
        text.append(string);
        if (addComma) {
            addComma();
        }
    }

    /**
     * Adds tabulation according to the tabulation count stored in the current element.
     */
    public void addTabulation() {
        for (int i = 0; i < currentTabulation; i++) {
            text.append("\t");
        }
    }

}
