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

package org.ow2.asmdex;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.ow2.asmdex.lowLevelUtils.DexFileReader;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.specificAnnotationParser.DefaultAnnotationSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.EnclosingClassSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.EnclosingMethodSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.ExceptionSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.ISpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.InnerClassSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.MemberClassesSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationParser.SignatureSpecificAnnotationParser;
import org.ow2.asmdex.specificAnnotationVisitors.DefaultAnnotationInformation;
import org.ow2.asmdex.specificAnnotationVisitors.DefaultAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.EnclosingClassAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.EnclosingMethodAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.ExceptionAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.InnerClassAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.MemberClassesAnnotationVisitor;
import org.ow2.asmdex.specificAnnotationVisitors.SignatureAnnotationVisitor;
import org.ow2.asmdex.structureReader.ClassDefinitionItem;
import org.ow2.asmdex.structureReader.FieldIdItem;
import org.ow2.asmdex.structureReader.MethodIdItem;

/**
 * An Android Application parser for dex files to make an {@link ApplicationVisitor}
 * visit an existing application.
 * This class parses a byte array conforming to the APK/dex class file format and
 * calls the appropriate visit methods of a given class visitor for each class, field,
 * method and bytecode instruction encountered.<br />
 * <br />
 * About Inner/Outer Classes :
 *
 * <ul><li> Inner Class of an Outer Class can be visited when visiting this Outer Class, but the
 *   Outer Name isn't encoded, as well as the access flags (this one can be retrieved
 *   elsewhere, though).</li>
 * <li> Method Inner Class aren't encoded at all in the Outer Class or the Outer Method.</li>
 * <li> Visiting an Inner Class calls visitInnerClass, which gives the internal
 *   name of the inner class (BUILT !), the class that contains it, the accessflags.</li>
 * <li> Visiting a Method Inner Class calls visitOuterClass AND visitInnerClass (ASM does that).</li>
 *
 * <li> Visiting an Inner Class from a normal class having an inner class doesn't provide the outerName.</li>
 *
 * <li> The outerName of an inner class when visiting an inner class
 *   from a normal class is not given.</li>
 *
 * <li> When calling visitInnerClass, the internalName is reconstructed from the outer
 *   name and inner name.</li>
 *
 * <li> Also done for the visitOuterClass.</li>
 *
 * CHOICES :
 * <ul><li> Line number don't always have a Label, because it's not always given by the
 *    Debug Info. As ASM requires it in visitLineNumber, we systematically create a
 *    label whenever a Line Number is found.</li></ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class ApplicationReader {

    /**
     * The bytecode to be parsed. <i>The content of this array must not be modified.</i>
     */
    public final byte[] byteCode;

    /**
     * A reader to the DexFile to parse.
     */
    private DexFileReader dexFile;

    /*
     * Maps linking fields, methods and parameters indexes to their annotation offset,
     * for the class being currently parsed. This corresponds to what is encoded in
     * the field_annotation structure.
     */
    private HashMap<Integer, Integer> fieldAnnotationOffsetsOfClass;
    private HashMap<Integer, Integer> methodAnnotationOffsetsOfClass;
    private HashMap<Integer, Integer> parameterAnnotationOffsetsOfClass;

    /**
     * Map to link a Type_Id index to an actual Class Index. This is useful because
     * some structures (like dalvik.annotation.MemberClasses) refers to classes
     * thanks to the type_id of their internal name.
     */
    private HashMap<Integer, Integer> typeIdToClassIndexMap;

    /**
     * Map linking a Class Name to an actual Class Index.
     */
    private HashMap<String, Integer> classNameToIndex;

    /**
     * List of Default Annotations for the current Class. They are stored because
     * even though they are read in the Class, they must be visited for each Method
     * that has one, when we encounter them.
     * In order to have a fast look-up, we link the Method name its possible data about
     * the Default Annotation.
     */
    private HashMap<String, DefaultAnnotationInformation> defaultAnnotations = new HashMap<String, DefaultAnnotationInformation>();

    /**
     * If true, displays the structures of the Decoded Application.
     */
    public static final boolean DEBUG_DISPLAY_STRUCTURES = false;

    /**
     * True to enable annotations support.
     */
    private static final boolean ANNOTATIONS = true;

    /**
     * True to enable bytecode writing support. This consists in copying the Constant Pool and the bytecode
     * of the methods directly to the output if the Reader is linked to the Writer. That is, if the methods
     * aren't modified by an Adapter.
     */
    private static final boolean WRITER = true;

    /**
     * Flag to skip method code. If this class is set <code>CODE</code>
     * attribute won't be visited. This can be used, for example, to retrieve
     * annotations for methods and method parameters.
     */
    public static final int SKIP_CODE = 1;

    /**
     * Flag to skip the debug information in the class. If this flag is set the
     * debug information of the class is not visited, i.e. the
     * {@link MethodVisitor#visitLocalVariable visitLocalVariable} and
     * {@link MethodVisitor#visitLineNumber visitLineNumber} methods will not be
     * called. However, note that in AsmDex, debug_info_items may still be
     * created as it contains the prologue and end_sequence. In order to completely
     * prevent the debug_info_items to be created, use the
     * {@link ApplicationWriter#SKIP_DEBUG_INFO_ITEMS} flag.
     */
    public static final int SKIP_DEBUG = 2;

    /**
     * Little enumeration to help identify a Visitor.
     *
     * @author Julien Névo
     */
    private static enum VisitorType { classVisitor, methodVisitor, fieldVisitor };

    /**
     * API level
     */
    protected int api;

    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------

    /**
     * Constructs a new {@link ApplicationReader} object.
     * @param api the ASMDEX api level
     * @param byteCode the bytecode of the application to be read.
     */
    public ApplicationReader(final int api, final byte[] byteCode) {
        this(api, byteCode, 0, byteCode.length);
    }

    /**
     * Constructs a new {@link ApplicationReader} object.
     * @param api the ASMDEX api level
     * @param byteCode the bytecode of the application to be read.
     * @param startOffset the start offset of the application data.
     * @param length the length of the application data.
     */
    public ApplicationReader(final int api, final byte[] byteCode, final int startOffset, final int length) {

        // Uses the byteCode array as-is for parsing the header, unless an
        // offset and different length were requested, in which case we
        // work on a copy of the array.
        if ((startOffset != 0) || (length != byteCode.length)) {
            this.byteCode = Arrays.copyOfRange(byteCode, startOffset, length + startOffset);
        } else {
            this.byteCode = byteCode;
        }

        // Opens the Dex file to check the validity of its header and parses it.
        dexFile = new DexFileReader();
        try {
            dexFile.parse(this.byteCode);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Constructs a new {@link ApplicationReader} object.
     * @param api the ASMDEX api level
     * @param inputStream an input stream from which to read the application.
     * @throws IOException if a problem occurs during reading.
     */
    public ApplicationReader(final int api, final InputStream inputStream ) throws IOException {
        this(api, readApplication(inputStream));
    }

    /**
     * Constructs a new {@link ApplicationReader} object.
     * @param api the ASMDEX api level
     * @param fileName name and path of the application (DEX) to be read.
     * @throws IOException if an exception occurs during reading.
     */
    public ApplicationReader(int api, final String fileName) throws IOException {
        // Opens as an InputStream the file whose name is given.
        this(api, readApplication(new FileInputStream(new File(fileName))));
    }

    /**
     * Constructs a new {@link ApplicationReader} object.
     * @param api the ASMDEX api level
     * @param file the dex file to be read.
     * @throws IOException if an exception occurs during reading.
     */
    public ApplicationReader(int api, final File file) throws IOException {
        this(api, readApplication(new FileInputStream(file)));
    }


    /**
     * Returns the Dex file.
     * @return the Dex file.
     */
    public IDalvikValueReader getDexFile() {
        return dexFile;
    }

    /**
     * Reads and stores the bytecode of an application.
     * @param inputStream an input stream from which to read the application.
     * @return the bytecode read from the given input stream.
     * @throws IOException if a problem occurs during reading.
     */
    private static byte[] readApplication(final InputStream inputStream) throws IOException {
        byte[] result = null;

        if (inputStream == null) {
            throw new IOException("Application not found");
        } else {
            ByteArrayOutputStream buffer = null;

            try {
                buffer = new ByteArrayOutputStream();

                byte[] dataRead = new byte[16384];
                int nbBytesRead;

                while ((nbBytesRead = inputStream.read(dataRead)) != -1) {
                    buffer.write(dataRead, 0, nbBytesRead);
                }

                buffer.flush();
                result = buffer.toByteArray();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (buffer != null) {
                    buffer.close();
                }
                inputStream.close();
            }
        }
        if (result == null) {
            throw new IOException("Application bytecode not initialized");
        }
        return result;
    }


    // ------------------------------------------------------------------------
    // Public methods
    // ------------------------------------------------------------------------

    /**
     * Makes the given visitor visit the Dex Application of this {@link ApplicationReader}.
     * All the classes, or only several, can be visited.
     * This application is the one specified in the constructor (see
     * {@link #ApplicationReader(int, byte[]) ApplicationReader}).
     *
     * @param applicationVisitor the visitor that must visit this class.
     * @param classesToVisit the names of the classes to visit, or Null to visit
     * 		  all the classes of the application.
     * @param flags option flags that can be used to modify the default behavior
     *        of this class. See {@link #SKIP_DEBUG}, {@link #SKIP_CODE}.
     */
    public void accept(final ApplicationVisitor applicationVisitor, final String[] classesToVisit,
                       final int flags) {
        // Attributes are not supported.
        accept(applicationVisitor, classesToVisit, null, flags);
    }

    /**
     * Makes the given visitor visit the Dex Application of this {@link ApplicationReader}.
     * All the classes are visited.
     * This application is the one specified in the constructor (see
     * {@link #ApplicationReader(int, byte[]) ApplicationReader}).
     *
     * @param applicationVisitor the visitor that must visit this class.
     * @param flags option flags that can be used to modify the default behavior
     *        of this class. See {@link #SKIP_DEBUG}, {@link #SKIP_CODE}.
     */
    public void accept(final ApplicationVisitor applicationVisitor, final int flags) {
        // Attributes are not supported.
        accept(applicationVisitor, null, null, flags);
    }

    /**
     * Makes the given visitor visit the Java class of this {@link ClassVisitor}.
     * This class is the one specified in the constructor
     *
     * @param applicationVisitor the visitor that must visit this Application.
     * @param classesToVisit the names of the classes to visit, or null to visit
     * 		  all the classes of the application.
     * @param attrs Attributes are <i>NOT</i> supported by AsmDex, ignored.
     * 		  Use annotations instead.
     * @param flags option flags that can be used to modify the default behavior
     *        of this class. See {@link #SKIP_DEBUG}, {@link #SKIP_CODE}.
     */
    public void accept(
        final ApplicationVisitor applicationVisitor,
        final String[] classesToVisit,
        final Object attrs,
        final int flags) {
        // The header has been parsed before, there's no need to parse it again.
        applicationVisitor.visit();

        // Parses the Classes (class_defs structure).
        int classDefinitionsSize = dexFile.getClassDefinitionsSize();

        // Just a little test to display all the classes of the dex file.
        if (DEBUG_DISPLAY_STRUCTURES) {
            System.out.println("----------------------- NB classes = " + classDefinitionsSize);
            for (int testI = 0; testI < classDefinitionsSize; testI++) {
                ClassDefinitionItem cdi = dexFile.getClassDefinitionItem(testI);
                System.out.println(dexFile.getStringItemFromTypeIndex(cdi.getClassIndex()));
            }
            System.out.println("-----------------------\n");

            // Just a little test to display all the methods of the dex file.
            System.out.println("----------------------- NB methods = " + dexFile.getMethodIdsSize());
            for (int testI = 0; testI < dexFile.getMethodIdsSize(); testI++) {
                System.out.println(
                    dexFile.getStringItemFromStringIndex(dexFile.getMethodIdItem(testI).getNameIndex()) + ", " +
                    dexFile.getStringItemFromTypeIndex(dexFile.getMethodIdItem(testI).getClassIndex()) + ", " +
                    dexFile.getShortyStringFromProtoIndex(dexFile.getMethodIdItem(testI).getPrototypeIndex()));
            }
            System.out.println("-----------------------\n");

            // Just a little test to display all the strings of the dex file.
            System.out.println("----------------------- NB strings = " + dexFile.getStringIdsSize());
            for (int testI = 0; testI < dexFile.getStringIdsSize(); testI++) {
                System.out.println(dexFile.getStringItemFromStringIndex(testI));
            }
            System.out.println("-----------------------\n");

            // Just a little test to display all the types of the dex file.
            System.out.println("----------------------- NB types = " + dexFile.getTypeIdsSize());
            for (int testI = 0; testI < dexFile.getTypeIdsSize(); testI++) {
                System.out.println(dexFile.getStringItemFromTypeIndex(testI));
            }
            System.out.println("-----------------------\n");

            // Just a little test to display all the prototypes of the dex file.
            System.out.println("----------------------- NB proto = " + dexFile.getProtoIdsSize());
            for (int testI = 0; testI < dexFile.getProtoIdsSize(); testI++) {
                //System.out.println(dexFile.getShortyStringFromProtoIndex(testI));
                System.out.println(dexFile.getDescriptorFromPrototypeIndex(testI));
            }
            System.out.println("-----------------------\n");

            // Just a little test to display all the fields of the dex file.
            System.out.println("----------------------- NB fields = " + dexFile.getFieldIdsSize());
            for (int testI = 0; testI < dexFile.getFieldIdsSize(); testI++) {
                System.out.println(dexFile.getNameFromFieldIndex(testI));
            }
            System.out.println("-----------------------\n");
        }

        // Builds the typeIdToClassIndexMap, in order to associate the Type_Ids to their
        // Class Index. Builds as well the classNameToIndex in order to reach a Class quickly according to
        // its name.
        typeIdToClassIndexMap = new HashMap<Integer, Integer>(classDefinitionsSize);
        classNameToIndex = new HashMap<String, Integer>(classDefinitionsSize);
        for (int index = 0; index < classDefinitionsSize; index++) {
            dexFile.seek(dexFile.getClassDefinitionOffset(index));
            int classIndexRead = dexFile.uint();
            typeIdToClassIndexMap.put(classIndexRead, index);

            String className = dexFile.getStringItemFromTypeIndex(classIndexRead);
            classNameToIndex.put(className, index);
        }

        // Visit all the Classes, or some ?
        if (classesToVisit == null) {
            // Visit all the Classes.
            // We couldn't do it inside the loop before, because we wouldn't have filled the
            // typeIdToClassIndexMap before visiting the Classes.
            // We also take car of reading the Classes in the order they were encoded.
            for (int index = 0; index < classDefinitionsSize; index++) {
                dexFile.seek(dexFile.getClassDefinitionOffset(index));
                int classIndexRead = dexFile.uint();
                String className = dexFile.getStringItemFromTypeIndex(classIndexRead);
                visitClass(applicationVisitor, className, flags);
            }
        } else {
            // Visits only some of the Classes ?
            for (String className : classesToVisit) {
                visitClass(applicationVisitor, className, flags);
            }
        }

        // Once everything is parsed, call the VisitEnd of the Application.
        applicationVisitor.visitEnd();
    }


    // ------------------------------------------------------------------------
    // Private methods
    // ------------------------------------------------------------------------

    /**
     * Visits a Class whose name is given.
     * @param applicationVisitor the Application visitor.
     * @param className the name of the Class.
     * @param flags option flags that can be used to modify the default behavior
     *        of this class. See {@link #SKIP_DEBUG}, {@link #EXPAND_FRAMES},
     *        {@link #SKIP_FRAMES}, {@link #SKIP_CODE}.
     */
    private void visitClass(final ApplicationVisitor applicationVisitor,
                            final String className, final int flags) {

        boolean skipCode = (flags & SKIP_CODE) != 0;
        boolean skipDebug = (flags & SKIP_DEBUG) != 0;

        // Finds the Class Index from its name.
        int classIndex = classNameToIndex.get(className);

        // Parsing the class itself (class_def_item structure).
        ClassDefinitionItem classDefinitionItem = dexFile.getClassDefinitionItem(classIndex);
        int accessFlags = classDefinitionItem.getAccessFlags();
        int superclassIndex = classDefinitionItem.getSuperclassIndex();
        int interfacesOffset = classDefinitionItem.getInterfacesOffset();
        int sourceFileIndex = classDefinitionItem.getSourceFileIndex();
        int annotationsOffset = classDefinitionItem.getAnnotationsOffset();
        int classDataOffset = classDefinitionItem.getClassDataOffset();
        int staticValuesOffset = classDefinitionItem.getStaticValuesOffset();
        defaultAnnotations.clear(); // Must be reset for each class class.

        // Finds the Interfaces, if any.
        String[] interfaces = null;
        if (interfacesOffset != 0) {
            dexFile.seek(interfacesOffset);
            int interfacesListSize = dexFile.uint();
            interfaces = new String[interfacesListSize];
            for (int noInterface = 0; noInterface < interfacesListSize; noInterface++) {
                int typeIndex = dexFile.ushort();
                // Gets the right descriptor in the Type_Ids, and the string related.
                String interfaceName = dexFile.getStringItemFromTypeIndex(typeIndex);
                interfaces[noInterface] = interfaceName;
            }
        }

        // Finds the Super Class.
        String superName;
        if (superclassIndex == Opcodes.NO_INDEX_SIGNED) {
            // If no Index, it is considered a Root class.
            superName = Constants.OBJECT_STRING;
        } else {
            superName = dexFile.getStringItemFromTypeIndex(superclassIndex);
        }

        String[] signature = null;
        // BEFORE visiting the Class, we have to know if there's a Signature in the Class Annotation.
        if (ANNOTATIONS && (annotationsOffset != 0)) {
            dexFile.seek(annotationsOffset); // Gets to the annotations_directory_item.
            int classAnnotationsOffset = dexFile.uint();
            if (classAnnotationsOffset != 0) {
                // Visits the Signature Annotation, if any.
                dexFile.seek(classAnnotationsOffset); // Gets to the annotation_set_item.
                signature = readSignatureAnnotation(); // Visits the Signature annotations.
            }
        }

        // We can now visit the Class.
        ClassVisitor classVisitor = applicationVisitor.visitClass(accessFlags, className, signature,
                                    superName, interfaces);

        // Gets the values from the static fields. Only Final Static fields are
        // encoded here. The non-static fields are initialized in the
        // constructor.
        Object[] staticValues = null;
        if (staticValuesOffset != 0) {
            dexFile.seek(staticValuesOffset);
            staticValues = decodedEncodedArrayAsObjects();
        }

        // Visits the Class.
        if (classVisitor != null) {
            classVisitor.visit(0, accessFlags, className, signature, superName, interfaces);

            // Calls the visitSource method.
            if (sourceFileIndex != Opcodes.NO_INDEX_SIGNED) {
                String sourceFile = dexFile.getStringItemFromStringIndex(sourceFileIndex);
                classVisitor.visitSource(sourceFile, null);
            }


            // Reads the annotations. The Class annotations are parsed and visited directly.
            // The field, method and parameter annotations are stored into three maps, to
            // to speed up the search when parsing these elements later.
            int classAnnotationsOffset = 0;

            if (ANNOTATIONS && (annotationsOffset != 0)) {
                dexFile.seek(annotationsOffset); // Get to the annotations_directory_item.
                classAnnotationsOffset = dexFile.uint();
                if (classAnnotationsOffset != 0) {
                    // Visit the outer class.
                    dexFile.seek(classAnnotationsOffset); // Get to the annotation_set_item.
                    readOuterClassAnnotations(classVisitor); // Visit the Outer Class annotations.

                    // Visit annotations of the Class only.
                    dexFile.seek(classAnnotationsOffset); // Get to the annotation_set_item.
                    readAndVisitAnnotations(classVisitor, 0, VisitorType.classVisitor); // Visit the Class annotations.

                    // Visit Default Annotation. They are declared in the Class but ASM
                    // wants them in the Methods they are linked to, so we have to store them and
                    // use them later.
                    dexFile.seek(classAnnotationsOffset); // Get to the annotation_set_item.
                    readDefaultAnnotations();

                    dexFile.seek(annotationsOffset + 4); // Get to the annotations_directory_item, fields_size field.
                }

                // Now pointing on the annotations_directory_item, fields_size field.
                // We build the annotations maps for use later.
                int fieldsSize = dexFile.uint();
                int annotatedMethodsSize = dexFile.uint();
                int annotatedParametersSize = dexFile.uint();
                // These instructions must be consecutive, because the fields are
                // encoded sequentially.
                fieldAnnotationOffsetsOfClass = dexFile.fillOffsetHashMap(fieldsSize);
                methodAnnotationOffsetsOfClass = dexFile.fillOffsetHashMap(annotatedMethodsSize);
                parameterAnnotationOffsetsOfClass = dexFile.fillOffsetHashMap(annotatedParametersSize);

                // Visits the inner class, if any.
                if (classAnnotationsOffset != 0) {
                    dexFile.seek(classAnnotationsOffset); // Get to the annotation_set_item.
                    readInnerClassAnnotations(className, classVisitor);

                    dexFile.seek(classAnnotationsOffset);
                    readMemberClassesAnnotations(classVisitor);
                }
            }

            // Gets the four fields from the class_data_item.
            // The class_data_offset can be 0 in case of a "marker interface", in which case
            // the class has no field and method.
            if (classDataOffset != 0) {
                dexFile.seek(classDataOffset);
                int nbStaticFields = dexFile.uleb128();
                int nbInstanceFields = dexFile.uleb128();
                int nbDirectMethods = dexFile.uleb128();
                int nbVirtualMethods = dexFile.uleb128();

                // Visits the fields.
                // First, searches for the static fields, then the instance fields.
                // They are directly after the static fields, so the reader must
                // not move between the two calls.
                visitFields(classVisitor, nbStaticFields, staticValues, annotationsOffset);
                visitFields(classVisitor, nbInstanceFields, null, annotationsOffset);

                // Visits the methods. First, the direct methods, then the virtual ones.
                // Same remark as above.
                visitMethods(classVisitor, nbDirectMethods, annotationsOffset, skipDebug, skipCode);
                visitMethods(classVisitor, nbVirtualMethods, annotationsOffset, skipDebug, skipCode);
            }

            classVisitor.visitEnd();
        }
    }

    /**
     * Method that visits the fields, whether they are static or instance fields.
     * Should only be called by the accept method. The dex file reader must point
     * on the right structure (5th or 6th structure item of class_data_item). It will point
     * at the end of the structure on return.
     * @param classVisitor visitor to call whenever a field is found.
     * @param nbFields number of fields there are.
     * @param staticValues array of the values of the Final Static values. Must be
     * 		  Null if the fields to be encountered are non-Final Static.
     * @param annotationsOffset offset to the annotations_directory_item. Used to parse
     * 		  the annotations of the fields. May be 0 if no annotation is present.
     */
    private void visitFields(ClassVisitor classVisitor,
                             int nbFields, Object[] staticValues, int annotationsOffset) {
        int fieldIndex = 0;

        int nbStaticValues = (staticValues != null ? staticValues.length : 0);
        for (int i = 0; i < nbFields; i++) {
            int readFieldIndex = dexFile.uleb128();
            // The field index read is a diff, unless it's the first read.
            fieldIndex = (i == 0) ? readFieldIndex : fieldIndex + readFieldIndex;
            int fieldAccessFlags = dexFile.uleb128();
            int saveDexFilePosition = dexFile.getPos(); // Save the position to come back there later.

            dexFile.seek(dexFile.getOffsetFieldIdItem(fieldIndex));
            // Gets the fields from the field_it_item structure of this field.
            dexFile.skipShort(); // Skip ClassIndex, not useful.
            int typeIndexInFieldId = dexFile.ushort();
            int nameIndexInFieldId = dexFile.uint();

            // Gets Name and Type.
            String fieldName = dexFile.getStringItemFromStringIndex(nameIndexInFieldId);
            String fieldType = dexFile.getStringItemFromTypeIndex(typeIndexInFieldId);

            // Gets the initialization value, previously parsed.
            // Only for Final Static fields.
            Object fieldValue = null;
            if (i < nbStaticValues) {
                fieldValue = staticValues[i];
            }

            // BEFORE visiting the Field, we have to know if a Signature Annotation is linked to it,
            // because it is needed by the visitField method.
            String[] signature = null;
            if (ANNOTATIONS && (annotationsOffset != 0)) {
                if (fieldAnnotationOffsetsOfClass.containsKey(fieldIndex)) {
                    dexFile.seek(fieldAnnotationOffsetsOfClass.get(fieldIndex)); // Now pointing on anotation_set_item.
                    signature = readSignatureAnnotation(); // Visits the Signature annotations.
                }
            }

            FieldVisitor fieldVisitor = classVisitor.visitField(fieldAccessFlags, fieldName, fieldType, signature, fieldValue);

            // Visits the Field.
            if (fieldVisitor != null) {
                // Finds if our current field is linked to an annotation.
                if (ANNOTATIONS && (annotationsOffset != 0)) {
                    if (fieldAnnotationOffsetsOfClass.containsKey(fieldIndex)) {
                        dexFile.seek(fieldAnnotationOffsetsOfClass.get(fieldIndex)); // Now pointing on anotation_set_item.
                        readAndVisitAnnotations(fieldVisitor, 0, VisitorType.fieldVisitor);
                    }
                }
                fieldVisitor.visitEnd(); // Each Field visited must be "ended".
            }
            dexFile.seek(saveDexFilePosition); // Recovers position in the fields list.
        }
    }


    /**
     * Method that visits the methods, whether they are direct or virtual.
     * Should only be called by the accept method.
     * The dex file reader must point on the right structure (7th or 8th item
     * of class_data_item). It will point at the end of the structure on return.
     * @param classVisitor Visitor to call whenever a method is found.
     * @param nbMethods Number of methods there are.
     * @param annotationsOffset offset to the annotations_directory_item. Used to parse
     * the annotations of the fields. May be 0 if no annotation is present.
     * @param skipDebug indicates if the debug information must be skipped.
     * @param skipCode indicates if the code information must be skipped.
     */
    private void visitMethods(ClassVisitor classVisitor, int nbMethods, int annotationsOffset,
                              boolean skipDebug, boolean skipCode) {
        int methodIndex = 0;
        for (int i = 0; i < nbMethods; i++) {
            int readMethodIndex = dexFile.uleb128();
            // The method index read may be a diff, unless it's the first read.
            methodIndex = (i == 0) ? readMethodIndex : methodIndex + readMethodIndex;
            int methodAccessFlags = dexFile.uleb128();
            int codeOffset = dexFile.uleb128();
            int saveDexFilePosition = dexFile.getPos(); // Saves the position to come back there later.

            dexFile.seek(dexFile.getOffsetMethodIdItem(methodIndex));
            // Gets the fields from the method_it_item structure of this method.
            dexFile.skipShort(); // Skips ClassIndex, not useful.
            int protoIndexInFieldId = dexFile.ushort();
            int nameIndexInFieldId = dexFile.uint();

            // Gets Name
            String methodName = dexFile.getStringItemFromStringIndex(nameIndexInFieldId);

            // Gets Descriptor
            String methodDescriptor = dexFile.getDescriptorFromPrototypeIndex(protoIndexInFieldId);

            // BEFORE visiting the method and its annotations, we want to display the
            // exceptions and signature it may produce. We have to make a first pass of the annotations.
            String[] exceptions = null;
            String[] signature = null;
            if (ANNOTATIONS && (annotationsOffset != 0)) {
                // Finds if our current method is linked to an exception annotation.
                if (methodAnnotationOffsetsOfClass.containsKey(methodIndex)) {
                    dexFile.seek(methodAnnotationOffsetsOfClass.get(methodIndex)); // Now pointing on annotation_set_item.
                    List<String> exceptionList = readExceptionAnnotations();
                    if (exceptionList != null) {
                        exceptions = exceptionList.toArray(new String[exceptionList.size()]);
                    }

                    dexFile.seek(methodAnnotationOffsetsOfClass.get(methodIndex)); // Now pointing on annotation_set_item.
                    signature = readSignatureAnnotation(); // Visits the Signature annotations.
                }
            }

            MethodVisitor methodVisitor = classVisitor.visitMethod(methodAccessFlags, methodName, methodDescriptor, signature, exceptions);
            // Visits the Method.
            if (methodVisitor != null) {
                // Visits the annotations and parameter annotations.
                if (ANNOTATIONS && (annotationsOffset != 0)) {
                    // First of all, check if there's an Annotation Default.
                    // They've been already parsed in the Class, and stored, so that they
                    // can be read later in the right Method.
                    if (defaultAnnotations.containsKey(methodName)) {
                        AnnotationVisitor annotationVisitor = methodVisitor.visitAnnotationDefault();
                        // As stated in the ASM visitDefaultAnnotation(), it consists only in one
                        // visit(), and one visitEnd().
                        if (annotationVisitor != null) {
                            Object value = defaultAnnotations.get(methodName).getValue();
                            visitDefaultAnnotationValue(annotationVisitor,methodName,value);
                            annotationVisitor.visitEnd();
                        }
                    }

                    // Finds if our current Method is linked to an annotation.
                    if (methodAnnotationOffsetsOfClass.containsKey(methodIndex)) {
                        dexFile.seek(methodAnnotationOffsetsOfClass.get(methodIndex)); // Now pointing on annotation_set_item.
                        readAndVisitAnnotations(methodVisitor, -1, VisitorType.methodVisitor);
                    }

                    // Visits the parameter annotations.
                    if (parameterAnnotationOffsetsOfClass.containsKey(methodIndex)) {
                        dexFile.seek(parameterAnnotationOffsetsOfClass.get(methodIndex)); // Now pointing on anotation_set_ref_list.
                        int nbAnnotations = dexFile.uint();

                        for (int annotationIndex = 0; annotationIndex < nbAnnotations; annotationIndex++) {
                            int annotationSetItemOffset = dexFile.uint();
                            int saveReaderPosition = dexFile.getPos();
                            dexFile.seek(annotationSetItemOffset);
                            readAndVisitAnnotations(methodVisitor, annotationIndex, VisitorType.methodVisitor);
                            dexFile.seek(saveReaderPosition);
                        }
                    }
                }

                boolean isOptimizationUsed = false;

                // Visits the code of this method, or not if optimization possible.
                if (!skipCode) {
                    /*
                     * If the returned MethodVisitor is in fact a MethodWriter, it
                     * means there is no method adapter between the reader and the
                     * writer. If, in addition, the writer's constant pool was
                     * copied from this reader (mw.cw.aw.ar == this), and the signature
                     * and exceptions of the method have not been changed, then it
                     * is possible to skip all visit events and just copy the
                     * original code of the method to the writer (the access, name
                     * and descriptor can have been changed, this is not important
                     * since they are not copied as is from the reader).
                     */
                    if (WRITER && (methodVisitor instanceof MethodWriter)) {
                        MethodWriter methodWriter = (MethodWriter)methodVisitor;
                        if (methodWriter.getClassWriter().getApplicationWriter().getApplicationReader() == this) {
                            // Same signature ?
                            String[] otherSignature = methodWriter.getMethod().getSignature();
                            if (Arrays.equals(signature, otherSignature)) {
                                // Same exceptions ?
                                String[] otherExceptions = methodWriter.getMethod().getExceptionNames();
                                if (Arrays.equals(exceptions, otherExceptions)) {
                                    // The bytecode can be copied. However, we don't do it now. We simply
                                    // store the offset the bytecode (code_item actually), and it will be copied
                                    // when the Dex file is written. Is can only be copied at this moment
                                    // because we might add some more Fields, Methods etc. which will modify
                                    // the indexes and will make the copied code use wrong indexes.
                                    isOptimizationUsed = true;
                                    methodWriter.setStartBytecodeToCopy(codeOffset);
                                }
                            }
                        }
                    }

                    // Else, we manage the visit of the code in a normal way, sending all the events
                    // through the visit.
                    if (!isOptimizationUsed) {
                        MethodCodeReader methodCodeReader = new MethodCodeReader(
                            dexFile, methodVisitor, codeOffset, skipDebug);
                        methodCodeReader.visitMethodCode();
                    }
                }

                // Visits the end of the method, even if the code was skipped or optimization was used.
                // Even abstract methods have a visitEnd().
                methodVisitor.visitEnd();
            }

            dexFile.seek(saveDexFilePosition); // Recovers position in the methods list.
        }
    }

    private void visitDefaultAnnotationValue(AnnotationVisitor annotationVisitor, String name, Object value) {
        if (value instanceof DefaultAnnotationVisitor) {
            DefaultAnnotationVisitor dav = (DefaultAnnotationVisitor) value;
            String desc = dav.getDesc();
            if (desc == null) {
                //  Here we have an array
                AnnotationVisitor av = annotationVisitor.visitArray(name);
                if (av != null) {
                    for (DefaultAnnotationInformation info : dav.getDefaultAnnotationInformationList()) {
                        visitDefaultAnnotationValue(av, info.getName(), info.getValue());
                    }
                    av.visitEnd();
                }
            } else {
                // Here we have an annotation with its description type
                AnnotationVisitor av = annotationVisitor.visitAnnotation(name,desc);
                if (av != null) {
                    for (DefaultAnnotationInformation info : dav.getDefaultAnnotationInformationList()) {
                        visitDefaultAnnotationValue(av, info.getName(), info.getValue());
                    }
                    av.visitEnd();
                }
            }
        } else if (value instanceof DefaultAnnotationInformation.EnumInfo) {
            DefaultAnnotationInformation.EnumInfo enumInfo = (DefaultAnnotationInformation.EnumInfo) value;
            annotationVisitor.visitEnum(name,enumInfo.enumDesc, enumInfo.enumValue);
        } else if (value instanceof DefaultAnnotationInformation.ClassInfo) {
            DefaultAnnotationInformation.ClassInfo classInfo = (DefaultAnnotationInformation.ClassInfo) value;
            annotationVisitor.visitClass(name,classInfo.className);
        } else {
            // Primitive type case.
            annotationVisitor.visit(name, value);
        }
    }

    /**
     * Reads an encoded value, as described in the encoded_value format. The
     * parsing may be recursive.
     * The dex file reader must point on the annotation element to visit. On
     * return, the reader points on the structure next to it.
     * @param annotationVisitor Visitor to visit the annotation element. May be null.
     * @param valueName the name of the value.
     */
    private void readEncodedValue(
        AnnotationVisitor annotationVisitor, String valueName) {

        // Decodes the format.
        int value = dexFile.ubyte();
        int valueType = value & 0x1f;
        int valueArg = (value >> 5) & 0x7;

        switch (valueType) {
        case Opcodes.VALUE_ENUM: {
            int enumIndex = (int)dexFile.sizedLong(valueArg);
            String enumValue = dexFile.getNameFromFieldIndex(enumIndex);
            String enumDesc = dexFile.getTypeNameFromFieldIndex(enumIndex);
            if (annotationVisitor != null) {
                annotationVisitor.visitEnum(valueName, enumDesc, enumValue);
            }
            break;
        }

        case Opcodes.VALUE_ARRAY: {
            readEncodedArray(annotationVisitor, valueName);
            break;
        }
        case Opcodes.VALUE_ANNOTATION: {
            readEncodedAnnotation(annotationVisitor, valueName);
            break;
        }
        case Opcodes.VALUE_TYPE: {
            int typeIndex = (int)dexFile.sizedLong(valueArg);
            String type = dexFile.getStringItemFromTypeIndex(typeIndex);
            if (annotationVisitor != null) {
                annotationVisitor.visitClass(valueName, type);
            }
            break;
        }

        default: {
            Object val = interpretEncodedValue(valueType, valueArg);
            if (annotationVisitor != null) {
                annotationVisitor.visit(valueName, val);
            }
            break;
        }
        }
    }



    /**
     * Interpret the given valueType/valueArg and returns the corresponding object.
     * The dex file reader must point after the byte containing the valueType/valueArg.
     * On return, the reader points after the last byte of the structure.
     * @param valueType the low order five bits of the first byte of encoded_value encoding.
     * @param valueArg the high order three bits of the first byte of encoded_value encoding.
     * @return Object of the right type (Byte, Short...).
     */
    private Object interpretEncodedValue(int valueType, int valueArg) {
        Object result = null;

        // Remember that the valueArg, if used as a Size, is encoded as Size-1,
        // like described in the dex documentation.
        // VALUE_ENUM, VALUE_ARRAY, VALUE_TYPE and VALUE_ANNOTATION are treated before.
        switch (valueType) {
        case Opcodes.VALUE_BOOLEAN:
            result = Boolean.valueOf(valueArg == 0 ? false : true);
            break;
        case Opcodes.VALUE_BYTE:
            result = Byte.valueOf((byte)dexFile.completeSignSizedLong(dexFile.sizedLong(valueArg), valueArg));
            break;
        case Opcodes.VALUE_SHORT:
            result = Short.valueOf((short)dexFile.completeSignSizedLong(dexFile.sizedLong(valueArg), valueArg));
            break;
        case Opcodes.VALUE_CHAR:
            result = Character.valueOf((char)dexFile.sizedLong(valueArg));
            break;
        case Opcodes.VALUE_INT:
            result = Integer.valueOf((int)dexFile.completeSignSizedLong(dexFile.sizedLong(valueArg), valueArg));
            break;
        case Opcodes.VALUE_LONG:
            result = Long.valueOf(dexFile.completeSignSizedLong(dexFile.sizedLong(valueArg), valueArg));
            break;
        case Opcodes.VALUE_FLOAT:
            result = Float.valueOf(Float.intBitsToFloat((int)dexFile.sizedLong(valueArg) << (8 * (3 - valueArg))));
            break;
        case Opcodes.VALUE_DOUBLE: {
            long temp = dexFile.sizedLong(valueArg) << (8 * (7 - valueArg));
            result = Double.valueOf(Double.longBitsToDouble(temp));
        }
        break;
        case Opcodes.VALUE_FIELD:
            result = Integer.valueOf((int)dexFile.sizedLong(valueArg));
            break;
        case Opcodes.VALUE_METHOD:
            result = Integer.valueOf((int)dexFile.sizedLong(valueArg));
            break;
        case Opcodes.VALUE_STRING:
            int stringIndex = (int)dexFile.sizedLong(valueArg);
            result = dexFile.getStringItemFromStringIndex(stringIndex);
            break;
        case Opcodes.VALUE_NULL:
            break;
        default:
            try {
                throw new Exception("Unknown value format : 0x" + Integer.toHexString(valueType));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return result;
    }

    /**
     * Decodes an encoded_array structure and returns an array of Objects. This method is useful when
     * the elements of the array must be then parsed and sent separately. If the array must be used as
     * a whole and unique Object, later to be parsed, better use the decodeEncodedArray method.
     * The dex file reader must point on the encoded_array. On return, the reader points
     * after the last byte of the structure.
     * @return an array of Objects.
     */
    private Object[] decodedEncodedArrayAsObjects() {
        int arraySize = dexFile.uleb128();

        Object[] result = new Object[arraySize];
        for (int i = 0; i < arraySize; i++) {
            int value = dexFile.ubyte();
            int valueType = value & 0x1f;
            int valueArg = (value >> 5) & 0x7;
            result[i] = interpretEncodedValue(valueType, valueArg);
        }

        return result;
    }

    /**
     * Decodes an encoded_array structure and returns an Object that contains an array of elements. This
     * method must be used over decodedEncodedArrayAsObjects if we need to send one single Object to
     * be parsed later.
     * The dex file reader must point on the encoded_array. On return, the reader points
     * after the last byte of the structure.
     * @return an Object containing an array of elements.
     */
    private Object decodeEncodedArray() {

        // Decodes the encoded_array format.
        int arraySize = dexFile.uleb128();

        // Empty arrays can be of any type, it doesn't matter as nothing will be put inside.
        if (arraySize == 0) {
            return new int[0];
        }

        int value = dexFile.ubyte();
        int valueType = value & 0x1f;
        int valueArg = (value >> 5) & 0x7;

        // We have to create a typed array according to the type.
        switch (valueType) {
        case Opcodes.VALUE_INT: {
            int[] array = new int[arraySize];
            array[0] = (Integer)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Integer)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_BOOLEAN: {
            boolean[] array = new boolean[arraySize];
            array[0] = (Boolean)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Boolean)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_BYTE: {
            byte[] array = new byte[arraySize];
            array[0] = (Byte)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Byte)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_SHORT: {
            short[] array = new short[arraySize];
            array[0] = (Short)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Short)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_CHAR: {
            char[] array = new char[arraySize];
            array[0] = (Character)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Character)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_LONG: {
            long[] array = new long[arraySize];
            array[0] = (Long)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Long)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_FLOAT: {
            float[] array = new float[arraySize];
            array[0] = (Float)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Float)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_DOUBLE: {
            double[] array = new double[arraySize];
            array[0] = (Double)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (Double)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_STRING: {
            String[] array = new String[arraySize];
            array[0] = (String)interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = (String)interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        case Opcodes.VALUE_TYPE:
        case Opcodes.VALUE_FIELD:
        case Opcodes.VALUE_METHOD:
        case Opcodes.VALUE_NULL: {
            Object[] array = new Object[arraySize];
            array[0] = interpretEncodedValue(valueType, valueArg);
            for (int i = 1; i < arraySize; i++) {
                value = dexFile.ubyte();
                array[i] = interpretEncodedValue(value & 0x1f, (value >> 5) & 0x7);
            }
            return array;
        }
        default:
            try {
                throw new Exception("Unhandled value format : " + Integer.toHexString(valueType));
            } catch (Exception e) {
                e.printStackTrace();
            }
            return null;
        }
    }

    /**
     * Reads an encoded array, as described in the encoded_array format and
     * visits it with the given visitor. We make the distinction between primitive
     * and non-primitive Arrays. On the first case, we only call the visit method on
     * an array composed on the primitive values. On the second case, we call the
     * visitArray method, and visit each elements.
     * The dex file reader must point on the encoded array to visit. On return, the reader
     * points on the structure next to it.
     * @param annotationVisitor Visitor to visit the encoded elements. May be null.
     * @param valueName the name of the value.
     */
    private void readEncodedArray(
        AnnotationVisitor annotationVisitor, String valueName) {

        // If primitive array, must build the array, and visit it (visit method,
        // NOT visitArray, specialized for non primitive types).
        if (isEncodedArrayPrimitive()) {
            Object primitiveArray = decodeEncodedArray();
            if (annotationVisitor != null) {
                annotationVisitor.visit(valueName, primitiveArray);
            }
        } else {
            // Non primitive array. We visit each elements, as well as calling the
            // visitArray method.
            int nbElements = dexFile.uleb128();
            AnnotationVisitor arrayElementsAnnotationVisitor =
                (annotationVisitor == null) ? null : annotationVisitor.visitArray(valueName);
            for (int elementIndex = 0; elementIndex < nbElements; elementIndex++) {
                readEncodedValue(arrayElementsAnnotationVisitor, null);
            }
            if (arrayElementsAnnotationVisitor != null) {
                arrayElementsAnnotationVisitor.visitEnd();
            }
        }
    }

    /**
     * The dex file reader must point on the annotation element to test. On return, the
     * reader position <i>is</i> saved.
     * @return true if the next Encoded Value is a primitive type.
     */
    private boolean isEncodedValuePrimitive() {
        boolean isPrimitive = true;

        int saveReaderPosition = dexFile.getPos();

        int value = dexFile.ubyte();
        int valueType = value & 0x1f;

        switch (valueType) {
        case Opcodes.VALUE_ENUM:
        case Opcodes.VALUE_ANNOTATION:
        case Opcodes.VALUE_ARRAY:
        case Opcodes.VALUE_STRING:
        case Opcodes.VALUE_TYPE:
            isPrimitive = false;
            break;
        }

        dexFile.seek(saveReaderPosition);
        return isPrimitive;
    }

    /**
     * Indicates if an encoded_array structure whether composed of Primitive type.
     * Only the first element is tested, though.
     * The dex file reader must point on the encoded_array element to test. On return,
     * the reader position <i>is</i> saved.
     * @return true if the encoded_array structure is composed of Primitive types.
     */
    private boolean isEncodedArrayPrimitive() {
        boolean isPrimitive = true;
        int saveReaderPosition = dexFile.getPos();

        int nbElements = dexFile.uleb128(); // Just a security, probably useless.
        if (nbElements > 0) {
            isPrimitive = isEncodedValuePrimitive();
        }

        dexFile.seek(saveReaderPosition);
        return isPrimitive;
    }


    // ----------------------------------------
    // Annotations management
    // ----------------------------------------
    /**
     * Reads the Annotations given by the reader, and calls the given visitor to
     * visit it. Several annotations can be encoded. Annotations about inner/outer class, Throws,
     * Default, Signature are skipped, as Signature are handled when visiting the Classes, Methods
     * and Fields (before, actually).
     * The dex file reader must point on annotation_set_item.
     * @param visitor visitor that must visit the values. Can be either a ClassVisitor,
     * FieldVisitor or MethodVisitor.
     * @param parameterNumber only used for MethodVisitor to call visitParameterAnnotation instead
     * of visitAnnotation, but only if this number is >=0. MethodVisitor wanting to call
     * visitAnnotation must use -1 for example.
     * @param visitorType indicates the type of the visitor (class, field or method visitor).
     */
    private void readAndVisitAnnotations(Object visitor, int parameterNumber, VisitorType visitorType) {

        int[] annotationOffsets = dexFile.getAnnotationItemOffsetsFromAnnotationSetItem();
        for (int annotationIndex = 0, nbAnnotations = annotationOffsets.length;
                annotationIndex < nbAnnotations; annotationIndex++) {

            dexFile.seek(annotationOffsets[annotationIndex]);
            // Treats the Annotation Item.
            short classVisibility = dexFile.ubyte();
            boolean isAnnotationVisible = (classVisibility == Opcodes.VISIBILITY_RUNTIME);
            // Now pointing on encoded_annotation format.
            String annotationType = dexFile.getStringItemFromTypeIndex(dexFile.uleb128());

            // Skips the Throws, Inner/Outer class, Default, Signature annotations.
            if (((((annotationType.equals(Constants.EXCEPTION_ANNOTATION_INTERNAL_NAME)) ||
                    (annotationType.equals(Constants.ENCLOSING_CLASS_ANNOTATION_INTERNAL_NAME))) ||
                    ((annotationType.equals(Constants.INNER_CLASS_ANNOTATION_INTERNAL_NAME))) ||
                    (annotationType.equals(Constants.ENCLOSING_METHOD_ANNOTATION_INTERNAL_NAME))) ||
                    (annotationType.equals(Constants.MEMBER_CLASSES_ANNOTATION_INTERNAL_NAME))) ||
                    ((annotationType.equals(Constants.ANNOTATION_DEFAULT_INTERNAL_NAME)))   ||
                    (annotationType.equals(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME))) {
                continue;
            }

            AnnotationVisitor annotationVisitor = null;
            if (visitorType == VisitorType.classVisitor) {
                annotationVisitor = ((ClassVisitor)visitor).visitAnnotation(annotationType, isAnnotationVisible);
            } else if (visitorType == VisitorType.fieldVisitor) {
                annotationVisitor = ((FieldVisitor)visitor).visitAnnotation(annotationType, isAnnotationVisible);
            } else if (visitorType == VisitorType.methodVisitor) {
                if (parameterNumber < 0) {
                    annotationVisitor = ((MethodVisitor)visitor).visitAnnotation(annotationType, isAnnotationVisible);
                } else {
                    annotationVisitor = ((MethodVisitor)visitor).visitParameterAnnotation(parameterNumber, annotationType, isAnnotationVisible);
                }
            }

            // Visits annotation and its elements.
            if (annotationVisitor != null) {
                int nbAnnotationElements = dexFile.uleb128();
                for (int i = 0; i < nbAnnotationElements; i++) {
                    readAnnotationElement(annotationVisitor);
                }
                annotationVisitor.visitEnd();
            }
        }
    }

    /**
     * Reads the Annotations, check only the Exception annotations, and returns
     * their internal names.
     * The dex file reader must point on an annotation_set_item.
     * @return the internal name of each exception, or Null if none were found.
     */
    private List<String> readExceptionAnnotations() {
        // Creates the Exception Annotation Parser and makes the search for this
        // specific annotation in the annotations.
        ISpecificAnnotationParser specificAnnotationParser =
            new ExceptionSpecificAnnotationParser(Constants.EXCEPTION_ANNOTATION_INTERNAL_NAME);

        boolean foundAnnotation = parseSpecificAnnotations(new ExceptionAnnotationVisitor(api), specificAnnotationParser);

        return foundAnnotation ?
               ((ExceptionSpecificAnnotationParser)specificAnnotationParser).getExceptions()
               : null;
    }

    /**
     * Reads the Default Annotations, if any, and sets the DefaultAnnotations field.
     * The dex file reader must point on an annotation_set_item.
     */
    private void readDefaultAnnotations() {
        // Creates the Default Annotation Parser and makes the search for this
        // specific annotation in the annotations.
        ISpecificAnnotationParser specificAnnotationParser =
            new DefaultAnnotationSpecificAnnotationParser(Constants.ANNOTATION_DEFAULT_INTERNAL_NAME);

        boolean foundAnnotation = parseSpecificAnnotations(new DefaultAnnotationVisitor(api, Constants.ANNOTATION_DEFAULT_INTERNAL_NAME), specificAnnotationParser);
        if (foundAnnotation) {
            // We found a Default Annotation. However, this method was called as we were
            // visiting a Class. But ASM calls the visitDefaultAnnotation for each Method
            // which has a Default Annotation linked to it. So we store our result and will
            // use it for each Method having a Default Annotation.
            DefaultAnnotationVisitor dav = (DefaultAnnotationVisitor)((DefaultAnnotationSpecificAnnotationParser)specificAnnotationParser).getAnnotationVisitor();
            List <DefaultAnnotationInformation> entries = dav.getDefaultAnnotationInformationList();
            if (entries.size() == 1) {
                dav = (DefaultAnnotationVisitor) entries.get(0).getValue();
                for (DefaultAnnotationInformation info : dav.getDefaultAnnotationInformationList()) {
                    String methodName = info.getName();
                    defaultAnnotations.put(methodName, info);
                }
            }
        }
    }

    /**
     * Reads the Signature Annotation, check only the Signature annotations. This is
     * useful to find Signature linked to Classes, Methods and Fields.
     * The dex file reader must point on an annotation_set_item.
     * @return an Array of Strings describing the Signature, or Null if none were found.
     */
    private String[] readSignatureAnnotation() {
        String[] result = null;

        // Creates the Signature Annotation Parser and makes the search for this
        // specific annotation in the annotations.
        ISpecificAnnotationParser specificAnnotationParser =
            new SignatureSpecificAnnotationParser(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME);

        boolean foundAnnotation = parseSpecificAnnotations(new SignatureAnnotationVisitor(api), specificAnnotationParser);
        if (foundAnnotation) {
            result = ((SignatureSpecificAnnotationParser)specificAnnotationParser).getSignature();
        }

        return result;
    }

    /**
     * Reads the Annotations, check only the MemberClasses annotations. This is
     * useful to find Inner Classes.
     * The dex file reader must point on an annotation_set_item.
     */
    private void readMemberClassesAnnotations(ClassVisitor classVisitor) {
        // Creates the Member Classes Annotation Parser and makes the search for this
        // specific annotation in the annotations.
        ISpecificAnnotationParser specificAnnotationParser =
            new MemberClassesSpecificAnnotationParser(Constants.MEMBER_CLASSES_ANNOTATION_INTERNAL_NAME);

        boolean foundAnnotation = parseSpecificAnnotations(new MemberClassesAnnotationVisitor(api), specificAnnotationParser);
        if (foundAnnotation) {
            MemberClassesSpecificAnnotationParser parser = (MemberClassesSpecificAnnotationParser)specificAnnotationParser;

            // Get the informations from the parser.
            List<String> classes = parser.getInnerClasses();

            for (String name : classes) {

                int i = name.lastIndexOf('$');
                // Reconstruction of the innerName and outerName.
                String innerName = name.substring(i + 1, name.length() - 1); // Removes also the ";" at the end.
                // Obfuscators may remove mention of the inner name. We must cope with it.
                String outerName =
                    (i<0) ? null : name.substring(0, i) + ";"; // Adds the ";" at the end.
                // classVisitor.visitInnerClass(name, outerName, innerName, accessFlags);
                classVisitor.visitMemberClass(name, outerName, innerName);
            }
        }
    }

    /**
     * Reads the Annotations, check only the ones related to inner classes
     * (EnclosingClass, InnerClasses), and call the visitInnerClass accordingly.
     * The dex file reader must point on an annotation_set_item.
     * @param className
     * @param classVisitor the visitor to visit the inner class.
     */
    private void readInnerClassAnnotations(String className, ClassVisitor classVisitor) {
        // Creates the InnerClass and EnclosingClass Parsers and makes the search for these
        // specific annotations in the annotations.

        // First, the parser for the Inner Class annotation.
        ISpecificAnnotationParser innerClassSpecificAnnotationParser =
            new InnerClassSpecificAnnotationParser(Constants.INNER_CLASS_ANNOTATION_INTERNAL_NAME);

        // Then, the parser for the Enclosing Class annotation.
        ISpecificAnnotationParser enclosingClassSpecificAnnotationParser =
            new EnclosingClassSpecificAnnotationParser(Constants.ENCLOSING_CLASS_ANNOTATION_INTERNAL_NAME);

        boolean foundFirstAnnotation = parseSpecificAnnotations(new InnerClassAnnotationVisitor(api), innerClassSpecificAnnotationParser);
        boolean foundSecondAnnotation = parseSpecificAnnotations(new EnclosingClassAnnotationVisitor(api), enclosingClassSpecificAnnotationParser);

        if (foundFirstAnnotation && foundSecondAnnotation) {
            InnerClassSpecificAnnotationParser innerParser = (InnerClassSpecificAnnotationParser)innerClassSpecificAnnotationParser;
            EnclosingClassSpecificAnnotationParser enclosingParser = (EnclosingClassSpecificAnnotationParser)enclosingClassSpecificAnnotationParser;

            // Gets the information.
            String outerClassName = enclosingParser.getClassName();
            String simpleNameInnerClass = innerParser.getSimpleNameInnerClass();
            classVisitor.visitInnerClass(className, outerClassName, simpleNameInnerClass, innerParser.getAccessFlagsInnerClass());
        }
    }

    /**
     * Reads the Annotations, checks only the ones related to outer classes
     * (EnclosingClass), and calls the visitOuterClass accordingly.
     * The InnerClass annotation is also needed to follow the ASM behavior (outer, then inner
     * visitor called).
     * The dex file reader must point on an annotation_set_item.
     * @param classVisitor the visitor to visit the inner class.
     */
    private void readOuterClassAnnotations(ClassVisitor classVisitor) {
        // Create the Enclosing Method and InnerClass Annotation Parsers and makes the search for these
        // specific annotations.
        ISpecificAnnotationParser enclosingMethodSpecificAnnotationParser =
            new EnclosingMethodSpecificAnnotationParser(Constants.ENCLOSING_METHOD_ANNOTATION_INTERNAL_NAME);

        // Then, the parser for the Inner Class annotation. Even though
        // we're looking for Outer class, the Inner Class annotation is encoded, and
        // gives information about the class that's actually embedded. ASM always calls
        // visitInnerClass after visitOuterClass.
        ISpecificAnnotationParser innerClassSpecificAnnotationParser =
            new InnerClassSpecificAnnotationParser(Constants.INNER_CLASS_ANNOTATION_INTERNAL_NAME);

        boolean foundFirstAnnotation = parseSpecificAnnotations(new EnclosingMethodAnnotationVisitor(api), enclosingMethodSpecificAnnotationParser);
        boolean foundSecondAnnotation = parseSpecificAnnotations(new InnerClassAnnotationVisitor(api), innerClassSpecificAnnotationParser);

        if (foundFirstAnnotation && foundSecondAnnotation) {
            int methodId = ((EnclosingMethodSpecificAnnotationParser)enclosingMethodSpecificAnnotationParser).getClassId();

            // Reaches the outer method.
            MethodIdItem methodIdItem = dexFile.getMethodIdItem(methodId);
            String nameEnclosingClass = dexFile.getStringItemFromTypeIndex(methodIdItem.getClassIndex()); // Get name from class_idx.
            String methodDescriptor = dexFile.getDescriptorFromPrototypeIndex(methodIdItem.getPrototypeIndex());
            String methodName = dexFile.getStringItemFromStringIndex(methodIdItem.getNameIndex());

            // visitOuterClass must only be called if an enclosing class exists.
            if (nameEnclosingClass != null) {
                classVisitor.visitOuterClass(nameEnclosingClass, methodName, methodDescriptor);
            }

            // Visits the InnerClass.
            InnerClassSpecificAnnotationParser innerParser = (InnerClassSpecificAnnotationParser)innerClassSpecificAnnotationParser;
            String simpleNameInnerClass = innerParser.getSimpleNameInnerClass();
            String outerClassName = dexFile.getNameFromMethodIndex(methodId);
            String nameInnerClass = null;
            // Reconstruction the name of the inner class.
            /*
            if ((nameEnclosingClass != null) && (simpleNameInnerClass != null) && (outerClassName != null)) {
            	nameInnerClass = nameEnclosingClass.replace(';', '$') + '1' + simpleNameInnerClass;
            }
            classVisitor.visitInnerClass(nameInnerClass, outerClassName, simpleNameInnerClass, innerParser.getAccessFlagsInnerClass());
            */
            if ((nameEnclosingClass != null) && (simpleNameInnerClass != null) && (outerClassName != null)) {
                nameInnerClass = nameEnclosingClass.replace(';', '$') + '1' + simpleNameInnerClass + ";";
            }
            classVisitor.visitInnerClass(nameInnerClass, nameEnclosingClass, simpleNameInnerClass, innerParser.getAccessFlagsInnerClass());
        }
    }

    /**
     * Parses a specific annotation and make an annotation visitor visit it.
     * This method is made to allow different visitors to visit
     * the same annotations for different purposes.
     * The dex file reader must point on an annotation_set_item. Its position is saved.
     * @param av the annotation visitor.
     * @param specificAnnotationParser annotation parser to perform an operation when
     * 		  finding the desired annotation.
     * @return true if the annotation has been found.
     */
    private boolean parseSpecificAnnotations(AnnotationVisitor av,
            ISpecificAnnotationParser specificAnnotationParser) {

        int savePositionreader = dexFile.getPos();
        boolean foundAnnotation = false;

        int[] annotationOffsets = dexFile.getAnnotationItemOffsetsFromAnnotationSetItem();
        for (int annotationIndex = 0, nbAnnotations = annotationOffsets.length;
                annotationIndex < nbAnnotations; annotationIndex++) {
            dexFile.seek(annotationOffsets[annotationIndex]);
            // Treats the Annotation Item.
            dexFile.skipByte(); // Skip visibility.
            // Now pointing on encoded_annotation format.
            String annotationType = dexFile.getStringItemFromTypeIndex(dexFile.uleb128());
            // Search if the annotation is what we are looking for.
            if (specificAnnotationParser.getAnnotationName().equals(annotationType)) {
                foundAnnotation = true;

                // Visits the annotations.
                // There should be only one, but we use a loop, just in case.
                int nbAnnotationElements = dexFile.uleb128();
                for (int i = 0; i < nbAnnotationElements; i++) {
                    readAnnotationElement(av);
                }
                specificAnnotationParser.treat(dexFile, this, av);
            }
        }
        dexFile.seek(savePositionreader);

        return foundAnnotation;
    }

    /**
     * Reads an annotation element, as described by the annotation_element format,
     * and visit it with the given visitor. On return, the reader points on the structure next
     * to it. This is useful because the annotation_element items are ordered one after the other
     * in the encoded_annotation format.
     * The dex file reader must point on the annotation element to visit.
     * @param annotationVisitor Visitor to visit the annotation element. May be null.
     */
    private void readAnnotationElement(AnnotationVisitor annotationVisitor) {

        String annotationName = dexFile.getStringItemFromStringIndex(dexFile.uleb128());
        readEncodedValue(annotationVisitor, annotationName);
    }

    /**
     * Reads an Encoded Annotation structure, as described by the encoded_annotation
     * format, and visit it with the given visitor. On return, the reader points
     * on the structure next to it.
     * The dex file reader must point on the annotation element to visit.
     * @param annotationVisitor Visitor to visit the annotation element.
     * @param valueName name of the value.
     */
    private void readEncodedAnnotation(AnnotationVisitor annotationVisitor,
                                       String valueName) {

        String annotationType = dexFile.getStringItemFromTypeIndex(dexFile.uleb128());
        AnnotationVisitor nestedAnnotationVisitor =
            annotationVisitor.visitAnnotation(valueName, annotationType);

        // It is important to read the elements even if the given visitor is null.
        // Because the data is stored linearly, we must continue to parse it to reach the
        // data "behind" the ones we want to skip. We simply don't have to visit the elements
        // if the visitor is null.
        int nbElements = dexFile.uleb128();
        for (int elementIndex = 0; elementIndex < nbElements; elementIndex++) {
            readAnnotationElement(nestedAnnotationVisitor);
        }

        if (nestedAnnotationVisitor != null) {
            nestedAnnotationVisitor.visitEnd();
        }
    }


    // --------------------------------------------------
    // "Constant Pool optimization" methods
    // --------------------------------------------------

    /**
     * Copies the constant pool data into the given {@link ApplicationWriter}. Should
     * be called before the {@link #accept(ApplicationVisitor,int)} method.
     *
     * However, contrary to ASM, only the Strings, Types, Fields and Methods indexes are actually copied,
     * because that's the only things the methods are referring to. Also note that the elements given to
     * the ApplicationWriter are stored as symbolic indexes. Indeed, more Fields, Strings, Types or
     * Methods could be added, thus making the references already used wrong.
     *
     * @param applicationWriter the {@link ApplicationWriter} to copy constant pool into.
     */
    public void copyPool(ApplicationWriter applicationWriter) {

        // Copy the Strings.
        for (int i = 0, size = dexFile.getStringIdsSize(); i < size; i++) {
            String string = dexFile.getStringItemFromStringIndex(i);
            applicationWriter.addStringFromApplicationReader(string);
        }

        // Copy the Types.
        for (int i = 0, size = dexFile.getTypeIdsSize(); i < size; i++) {
            String type = dexFile.getStringItemFromTypeIndex(i);
            applicationWriter.addTypeNameFromApplicationReader(type);
        }

        // Copy the Fields.
        for (int i = 0, size = dexFile.getFieldIdsSize(); i < size; i++) {
            FieldIdItem fii = dexFile.getFieldIdItem(i);
            String className = dexFile.getStringItemFromTypeIndex(fii.getClassIndex());
            String type = dexFile.getStringItemFromTypeIndex(fii.getTypeIndex());
            String fieldName = dexFile.getStringItemFromStringIndex(fii.getNameIndex());
            applicationWriter.addFieldFromApplicationReader(className, type, fieldName);
        }

        // Copy the Methods.
        for (int i = 0, size = dexFile.getMethodIdsSize(); i < size; i++) {
            MethodIdItem mii = dexFile.getMethodIdItem(i);
            String className = dexFile.getStringItemFromTypeIndex(mii.getClassIndex());
            String methodName = dexFile.getStringItemFromStringIndex(mii.getNameIndex());
            String prototype = dexFile.getDescriptorFromPrototypeIndex(mii.getPrototypeIndex());
            applicationWriter.addMethodFromApplicationReader(className, prototype, methodName);
        }
    }

}

