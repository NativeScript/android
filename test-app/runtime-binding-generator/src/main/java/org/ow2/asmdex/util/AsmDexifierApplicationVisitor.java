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

import java.io.PrintWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.ArrayList;

import org.ow2.asmdex.ApplicationReader;
import org.ow2.asmdex.ApplicationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.Opcodes;

/**
 * An {@link ApplicationVisitor} that prints the AsmDex code that generates the Application
 * it visits. This Application visitor can be used to quickly write ASM code to generate
 * some given bytecode: <ul> <li>write the Java source code equivalent to the
 * bytecode you want to generate;</li> <li>compile it with <tt>javac</tt>;</li>
 * <li>make a {@link AsmDexifierApplicationVisitor} visit this compiled class (see the
 * {@link #main main} method);</li> <li>edit the generated source code, if
 * necessary.</li> </ul>
 *
 * NOTE:
 * <ul>
 * <li> Having all the dumps in one dump() method provides a code that is quickly superior to the 64k
 *   limit per method. So each class has its own dumpClassName() method, which is called by
 *   the dump() method.</li>
 * <li> The Opcodes values are obtained through reflection. If refactoring, check out if
 *   OPCODES_FULLY_QUALIFIED_NAME is still valid.</li>
 * </ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class AsmDexifierApplicationVisitor extends  ApplicationVisitor {

    final private AsmDexPrinter pr;

    /**
     * Fully qualified name of the Opcodes class.
     */
    public static final String OPCODES_FULLY_QUALIFIED_NAME = "org.ow2.asmdex.Opcodes";

    /**
     * How every Opcode names start.
     */
    private static final String OPCODES_START = "INSN_";

    /**
     * Possible package name.
     */
    private String packageName = null;

    /**
     * Possible path and filename of the file where to save the output.
     */
    private String filenameOutput = null;

    /**
     * How many Opcodes there could be.
     */
    private static final int NB_OPCODES = 256;

    /**
     * The print writer to be used to print the Application.
     */
    protected final PrintWriter pw;

    /**
     * The Dex filename, without the path and extension.
     */
    private static String dexFileName;

    /**
     * Classes to visit, or Null if all must be visited.
     */
    private static String[] classesToVisit;

    /**
     * Array where all the opcode names are written.
     */
    private static String[] opcodeNames;

    /**
     * List containing the name of the dumpMethods.
     */
    private ArrayList<String> dumpMethods;

    /**
     * Used to store the text of the methods to call. We have to store it because we only
     * know all the method names when everything has been visited.
     */
    private TextComposite methodToCall;

    /**
     * Constructs a new {@link AsmDexifierApplicationVisitor} object.
     *
     * @param api API level
     * @param pw the print writer to be used to print the class.
     */
    public AsmDexifierApplicationVisitor(int api, final PrintWriter pw) {
        super(api);
        this.pw = pw;
        pr = new AsmDexPrinter();
        dumpMethods = new ArrayList<String>();
        methodToCall = new TextComposite();
    }

    /**
     * Constructs a new {@link AsmDexifierApplicationVisitor} object.
     *
     * @param api API level
     * @param pw the print writer to be used to print the class.
     * @param packageName name of the package, or Null if none should be written.
     * @param filenameOutput possible path and filename of the file where to save the output.
     *        If not Null, a Main method will be created.
     */
    public AsmDexifierApplicationVisitor(int api, final PrintWriter pw, final String packageName,
                                         final String filenameOutput) {
        this(api, pw);
        this.packageName = packageName;
        this.filenameOutput = filenameOutput;
    }


    /**
     * Prints the ASM source code to generate the given Application to the standard
     * output. Not to give classes means visiting all the classes of the Dex file.
     * <p> Usage: ASMifierClassVisitor [-debug] &lt;Dex file to open&gt;
     * [&lt;fully qualified class names&gt;]
     *
     * @param args the command line arguments.
     *
     * @throws Exception if the class cannot be found, or if an IO exception
     *         occurs.
     */
    public static void main(final String[] args) throws Exception {
        int i = 0;
        int flags = ApplicationReader.SKIP_DEBUG;

        int nbArgs = args.length;

        boolean ok = (nbArgs > 0);

        if (ok && "-debug".equals(args[0])) {
            i = 1;
            flags = 0;
            if (nbArgs < 2) {
                ok = false;
            }
        }
        if ((!ok) || (!args[i].endsWith(".dex"))) {
            System.err.println("Prints the AsmDex code to generate the given class.");
            System.err.println("Usage: ASMifierApplicationVisitor [-debug] "
                               + "<Dex file to open> "
                               + "[<fully qualified class names>]");
            return;
        }

        // Visits all the Classes or one/several ?
        classesToVisit = null;
        if (nbArgs > (i + 1)) {
            int nbClasses = nbArgs - i - 1;
            classesToVisit = new String[nbClasses];
            int classIndex = 0;
            for (int j = i + 1; j < nbArgs; j++) {
                classesToVisit[classIndex++] = args[j];
            }
        }

        dexFileName = trimDexFileName(args[i]);

        // Sets up the Opcodes array by using Reflection.
        opcodeNames = new String[NB_OPCODES];

        Field[] fields = Class.forName(OPCODES_FULLY_QUALIFIED_NAME).getFields();
        for (Field field : fields) {
            String fieldName = field.getName();
            int goodFlag = Modifier.FINAL | Modifier.STATIC | Modifier.PUBLIC;
            if (fieldName.startsWith(OPCODES_START)) {
                if (field.getModifiers() == goodFlag) {
                    int opcode = field.getInt(field);
                    if (opcode < NB_OPCODES) {
                        opcodeNames[opcode] = fieldName;
                    }
                }
            }
        }

        ApplicationReader ar = new ApplicationReader(Opcodes.ASM4, args[i]);
        ar.accept(new AsmDexifierApplicationVisitor(Opcodes.ASM4, new PrintWriter(System.out)),
                  classesToVisit, flags);
    }

    /**
     * Trims the given Dex filename of its path and extension.
     * @param fullFileName name that can contain the path and the extension.
     * @return name of the dex file, without the path and the extension.
     */
    public static String trimDexFileName(String fullFileName) {
        String trimmedFileName = fullFileName;
        int n = trimmedFileName.lastIndexOf(".dex");
        if (n != -1) {
            trimmedFileName = trimmedFileName.substring(0, n);
        }
        n = trimmedFileName.lastIndexOf('/');
        if (n != -1) {
            trimmedFileName = trimmedFileName.substring(n + 1);
        }

        return trimmedFileName;
    }

    /**
     * Sets the dex filename, which mustn't contain path and extension. It is useful to name
     * the class generated (prefixed with "Dump").
     * @param name the dex filename.
     */
    public static void setDexFileName(String name) {
        dexFileName = name;
    }


    // ------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------

    @Override
    public void visit() {
        // Puts the first letter of the filename in upper case.
        char firstLetter = Character.toUpperCase(dexFileName.charAt(0));
        String otherLetters = dexFileName.length() > 1 ? dexFileName.substring(1) : "";
        String camelName = firstLetter + otherLetters;

        if (packageName != null) {
            pr.addText("package " + packageName + ";\n");
            pr.addEOL();
        }

        pr.addText("import java.util.*;\n");
        if (filenameOutput != null) {
            pr.addText("import java.io.*;\n");
        }
        pr.addText("import org.ow2.asmdex.*;\n");
        pr.addText("import org.ow2.asmdex.structureCommon.*;\n\n");

        pr.addText("public class " + camelName + "Dump implements Opcodes {\n\n");

        if (filenameOutput != null) {
            pr.addText("public static void main(String[] args) throws Exception {\n");
            pr.addText("	byte[] b = dump();\n");
            pr.addText("	File outputFile;\n");
            pr.addText("	if (args.length == 1) {\n");
            pr.addText("		outputFile = new File(args[0]);\n");
            pr.addText("		FileOutputStream outputStream = new FileOutputStream(outputFile);\n");
            pr.addText("		outputStream.write(b);\n");
            pr.addText("		outputStream.close();\n");
            pr.addText("	}\n");
            pr.addText("}\n\n");
        }

        pr.addText("public static byte[] dump() throws Exception {\n\n");

        pr.addText("\tApplicationWriter aw = new ApplicationWriter();\n");
        pr.addText("\taw.visit();\n");
        pr.addEOL();
        pr.closeText();

        // Prepare the calls to the various dump Methods.
        pr.addTextToList(methodToCall);
        pr.addEOL();

        pr.addText("\taw.visitEnd();\n\n");
        pr.addText("\treturn aw.toByteArray();\n");
        pr.addText("}\n\n");
        pr.closeText();
    }

    @Override
    public ClassVisitor visitClass(int access, String name, String[] signature,
                                   String superName, String[] interfaces) {

        String className = getAcceptableMethodName(name);

        pr.addText("public static void ");
        pr.addText(className);
        pr.addText("(ApplicationWriter aw) {\n");

        pr.addText("\tClassVisitor cv;\n");
        pr.addText("\tFieldVisitor fv;\n");
        pr.addText("\tMethodVisitor mv;\n");
        pr.addText("\tAnnotationVisitor av0;\n\n");

        pr.addText("\tcv = aw.visitClass(");
        pr.addAccessFlags(access, true);
        pr.addConstant(name, true);
        pr.addConstant(signature, true);
        pr.addConstant(superName, true);
        pr.addConstant(interfaces, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierClassVisitor cv = new AsmDexifierClassVisitor(api, 1);
        pr.addTextToList(cv.getTextComponent());

        pr.addText("}\n\n");
        pr.closeText();
        return cv;
    }

    @Override
    public void visitEnd() {
        pr.addText("}\n\n");
        pr.closeText();

        for (String methodName : dumpMethods) {
            StringBuffer sb = new StringBuffer("\t" + methodName);
            sb.append("(aw);\n");
            TextLeaf leaf = new TextLeaf(sb);
            methodToCall.addComponent(leaf);
        }

        // Prints the String Tree.
        pr.getTextComponent().print(pw);
        pw.flush();
    }


    // ------------------------------------------------
    // Static utility Methods.
    // ------------------------------------------------

    /**
     * Returns the opcode name from the given opcode, or Null if the opcode isn't found.
     * @param opcode the opcode.
     * @return the opcode name from the given opcode, or Null if the opcode isn't found.
     */
    public static String getOpcodeName(int opcode) {
        String result = null;
        if ((opcodeNames != null) && (opcode < opcodeNames.length)) {
            result = opcodeNames[opcode];
        }
        return result;
    }

    /**
     * Returns a correct Method name from the given Class name, which may probably be
     * fully qualified. If also must not already be in the dumpMethods list, in which
     * case a number is added to the name.
     * @param className
     * @return a correct Class name.
     */
    public String getAcceptableMethodName(String className) {
        String result = "Method"; // Fail-safe name.

        if ((className != null) && (!className.equals(""))) {

            int i = className.lastIndexOf('/');
            if (i >= 0) {
                result = className.substring(i + 1);
            } else {
                result = className;
            }

            i = result.lastIndexOf(';');
            if (i > 0) {
                result = result.substring(0, i);
            }
        }

        result = "dump" + result;

        // Checks if this name isn't already in the list.
        if (dumpMethods.contains(result)) {
            boolean stillFound = true;
            int i = 2;
            String newResult = null;
            while (stillFound) {
                newResult = result + i;
                stillFound = dumpMethods.contains(newResult);
                i++;
            }
            result = newResult;
        }

        dumpMethods.add(result);

        return result;
    }
}
