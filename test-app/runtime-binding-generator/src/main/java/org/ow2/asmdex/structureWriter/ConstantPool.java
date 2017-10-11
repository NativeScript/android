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

package org.ow2.asmdex.structureWriter;

import org.ow2.asmdex.Opcodes;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * Constant Pool of the Application. It has a link to every element the Application holds.
 *
 * Dalvik requires some structures to be sorted. It is faster to sort them once the Application is parsed
 * than adding elements one after one, so we manage first simple collections, and then the
 * prepareGeneration method builds the sorted ones.
 *
 * Also, we use Symbolic elements from Strings, Fields, Types and Methods so that we can generate the bytecode
 * right after each method is parsed. This is useful to be able to destroy the elements related the Methods
 * when their parsing is over. The indexes are then remapped correctly after the Application is fully parsed
 * (see {@link CodeItem#mapResolvedIndexes()}).
 *
 * NOTES :
 * - Empty Strings can be added to the Constant Pool, but only if useful. Empty parameters aren't encoded
 *   in the Constant Pool (yet they are declared in the Debug Item). This is tested by the Debug Item
 *   encoder though, not the Constant Pool itself.
 *
 * - a "this" String is added by dx, I don't know exactly where it comes from. I add it when a
 *   non-static/non-constructor Method is found, it seems logical.
 *
 * @author Julien Névo
 */
public class ConstantPool {

    /**
     * Indicates whether the searches must be performed on Symbolic elements (where available) or not.
     * It matters only for Strings, Fields, Types and Methods.
     */
    private boolean useSymbolicElements = true;

    /* Here follows the symbolic maps. They are useful to instructions, because the opcode of methods
     * is generated at the end of each of them, and the list of instructions are erased in order to
     * reduce memory consumption. But indexes to Strings, Fields, Types and Methods, which are used by
     * instructions, are only definitely known when the whole application has been parsed, as they must
     * be ordered within the Dex file.
     * The solution consists in using symbolic Strings/Fields/Types/Methods which refers an index
     * that will later be mapped into the "real" index, once the full application is parsed.
     */
    /**
     * Map linking a symbolic String to its index.
     */
    private HashMap<String, Integer> symbolicStringsToIndexes = new HashMap<String, Integer>();

    /**
     * Map linking a symbolic Types to its index.
     */
    private HashMap<String, Integer> symbolicTypesToIndexes = new HashMap<String, Integer>();

    /**
     * Map linking a symbolic Methods to its index.
     */
    private HashMap<Method, Integer> symbolicMethodsToIndexes = new HashMap<Method, Integer>();

    /**
     * Map linking a symbolic Fields to its index.
     */
    private HashMap<Field, Integer> symbolicFieldsToIndexes = new HashMap<Field, Integer>();



    /**
     * Array linking a symbolic String index to a resolved String index.
     */
    private int[] symbolicStringsIndexToResolvedStringsIndex;

    /**
     * Array linking a symbolic Field index to a resolved Field index.
     */
    private int[] symbolicFieldsIndexToResolvedFieldsIndex;

    /**
     * Array linking a symbolic Field index to a resolved Field index.
     */
    private int[] symbolicMethodsIndexToResolvedMethodsIndex;

    /**
     * Array linking a symbolic Field index to a resolved Field index.
     */
    private int[] symbolicTypesIndexToResolvedTypesIndex;

    /*
     * The following maps links the structures to an index. It is only filled once the application has been
     * fully parsed.
     */
    private HashMap<String, Integer> resolvedStringsToIndexes = new HashMap<String, Integer>();
    private HashMap<String, Integer> resolvedTypesToIndexes = new HashMap<String, Integer>();
    private HashMap<Method, Integer> resolvedMethodsToIndexes = new HashMap<Method, Integer>();
    private HashMap<Field, Integer> resolvedFieldsToIndexes = new HashMap<Field, Integer>();

    private HashMap<Prototype, Integer> prototypesToIndexes = new HashMap<Prototype, Integer>();
    //private HashMap<ClassDefinitionItem, Integer> classesToIndexes = new HashMap<ClassDefinitionItem, Integer>();

    /**
     * Map linking a TypeList structure to its offset from the beginning of the file.
     */
    private HashMap<TypeList, Integer> typeListsToOffsets = new HashMap<TypeList, Integer>();

    /**
     * Ordered Set in which the newly discovered Strings are added. This is useful to order the
     * Strings when assigning them indexes, as requested by the Dex format.
     */
    private TreeSet<String> strings = new TreeSet<String>();

    /**
     * Unordered Set in which the newly discovered prototypes are added. The Prototypes will be sorted
     * only when the Application has been fully parsed. The ordered collection is just below.
     */
    private HashMap<Prototype,Prototype> prototypes = new HashMap<Prototype,Prototype>();

    /**
     * List of Ordered Prototypes. It is only completed when everything has been parsed.
     */
    private TreeSet<Prototype> orderedPrototypes;

    /**
     * Ordered Set in which the newly discovered Types are added. A Type added here must also
     * be added in the strings structure above.
     */
    private TreeSet<String> types = new TreeSet<String>();

    /**
     * Ordered Set in which the newly discovered Fields are added. The Fields are sorted by the
     * class owning name, their own name, and their Type.
     */
    private HashMap<Field,Field> fields = new HashMap<Field,Field>();

    /**
     * Unordered Set containing Methods. They will be sorted when all the Application is parsed.
     * This is done to speed up the addition of new Methods. The ordered collection is just below.
     */
    private HashMap<Method,Method> methods = new HashMap<Method,Method>();

    /**
     * List of Ordered Fields. It is only completed when everything has been parsed.
     */
    private TreeSet<Field> orderedFields;

    /**
     * List of Ordered Methods. It is only completed when everything has been parsed.
     */
    private TreeSet<Method> orderedMethods;

    /**
     * List of Ordered Classes. It is only completed when everything has been parsed.
     */
    private ArrayList<ClassDefinitionItem> orderedClasses;

    /**
     * Unordered Set containing TypeList, from to the type_list structure.
     * The ordered list is below, constructed after all has been parsed.
     */
    private HashSet<TypeList> typeList = new HashSet<TypeList>();

    /**
     * Set of Ordered TypeLists. It is only completed when everything has been parsed.
     */
    private TreeSet<TypeList> orderedTypeList;

    /**
     * Map linking a String from the Type_ids list (as encoded in the future Dex file) to the
     * String index in the string_ids. This map is created only when the header is being built.
     */
    //private HashMap<String, Integer> stringsToTypeIdIndexes = new HashMap<String, Integer>();

    /**
     * Map linking a ClassDefinitionItem to its static_value_encoded_array_item offset. Only Classes
     * with an actual array are encoded. If a Class doesn't found its array, it means it has none.
     * This map is created when the value_encoded_array_items are built.
     */
    private HashMap<ClassDefinitionItem, Integer> staticValuesEncodedArrayItemOffsets = new HashMap<ClassDefinitionItem, Integer>();


    /**
     * Set of annotation_items. They are ordered, and unique.
     */
    //private HashSet<AnnotationItem> annotationItems = new HashSet<AnnotationItem>();
    private TreeSet<AnnotationItem> annotationItems = new TreeSet<AnnotationItem>();

    /**
     * Map of annotation_items linking an annotation_item to an Offset.
     */
    private HashMap<AnnotationItem, Integer> annotationItemsToOffsets = new HashMap<AnnotationItem, Integer>();

    /**
     * Set of annotation_set_items. They are ordered, and unique.
     */
    //private HashSet<AnnotationSetItem> annotationSetItems = new HashSet<AnnotationSetItem>();
    private TreeSet<AnnotationSetItem> annotationSetItems = new TreeSet<AnnotationSetItem>();

    /**
     * Map of annotation_set_items linking an annotation_set_item to an Offset.
     */
    private HashMap<AnnotationSetItem, Integer> annotationSetItemsToOffsets = new HashMap<AnnotationSetItem, Integer>();

    /**
     * Set of annotation_set_ref_list. They are not ordered, but we need them to be unique.
     */
    private HashSet<AnnotationSetRefList> annotationSetRefLists = new HashSet<AnnotationSetRefList>();

    /**
     * Map of annotation_set_ref_lists linking an annotation_set_ref_list to an Offset.
     */
    private HashMap<AnnotationSetRefList, Integer> annotationSetRefListsToOffsets = new HashMap<AnnotationSetRefList, Integer>();

    /**
     * Map linking a Class to the offset of its annotation_directory_item, if any.
     */
    //private HashMap<ClassDefinitionItem, Integer> classesToAnnotationDirectoryItemOffsets = new HashMap<ClassDefinitionItem, Integer>();
    private HashMap<AnnotationDirectoryItem, Integer> classesToAnnotationDirectoryItemOffsets = new HashMap<AnnotationDirectoryItem, Integer>();

    /**
     * Map linking a ClassName (fully qualified) to its Class Definition Item. It is useful to sort the
     * Class at the end of the parsing.
     */
    // We use a sorted map instead of a regular map so that names of classes are roughly ordered although some
    // items may be introduced to respect the type hierarchy (required constraint imposed by sortClasses().
    // This rough ordering is the one used by dx. [PC - 7/1/13]
    private Map<String, ClassDefinitionItem> classNameToClassDefinitionItem = new TreeMap<String, ClassDefinitionItem>();

    /**
     * HashSet of annotation_directory_items. They are unique, but we don't care about their order.
     */
    private HashSet<AnnotationDirectoryItem> annotationDirectoryItems = new HashSet<AnnotationDirectoryItem>();




    // ---------------------------------------
    // Getters and Setters.
    // ---------------------------------------

    /**
     * Returns the number of Strings currently parsed.
     * @return the number of Strings currently parsed.
     */
    public int getStringCount() {
        return strings.size();
    }

    /**
     * Returns the number of Types currently parsed.
     * @return the number of Types currently parsed.
     */
    public int getTypeCount() {
        return types.size();
    }

    /**
     * Returns the number of Prototypes currently parsed.
     * @return the number of Prototypes currently parsed.
     */
    public int getPrototypeCount() {
        return prototypes.size();
    }

    /**
     * Returns the number of Fields currently parsed.
     * @return the number of Fields currently parsed.
     */
    public int getFieldCount() {
        return fields.size();
    }

    /**
     * Returns the number of Methods currently parsed.
     * @return the number of Methods currently parsed.
     */
    public int getMethodCount() {
        return methods.size();
    }

    /**
     * Returns the number of Classes currently parsed.
     * @return the number of Classes currently parsed.
     */
    public int getClassDefinitionCount() {
        return classNameToClassDefinitionItem.size();
    }

    /**
     * Returns the number of TypeList currently parsed.
     * @return the number of TypeList currently parsed.
     */
    public int getTypeListCount() {
        return typeList.size();
    }

    /**
     * Returns the ordered Strings of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered Strings of the Application.
     */
    public TreeSet<String> getStrings() {
        return strings;
    }

    /**
     * Returns the ordered Types of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered Types of the Application.
     */
    public TreeSet<String> getTypes() {
        return types;
    }

    /**
     * Returns the ordered Fields of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered Fields of the Application.
     */
    public TreeSet<Field> getFields() {
        return orderedFields;
    }

    /**
     * Returns the ordered Prototypes of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered Prototypes of the Application.
     */
    public TreeSet<Prototype> getPrototypes() {
        return orderedPrototypes;
    }

    /**
     * Returns the ordered Methods of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered Methods of the Application.
     */
    public TreeSet<Method> getMethods() {
        return orderedMethods;
    }

    /**
     * Returns the ordered TypeLists of the Application. It is only set when the Application has been fully parsed.
     * @return the ordered TypeLists of the Application.
     */
    public TreeSet<TypeList> getTypeList() {
        return orderedTypeList;
    }

    /**
     * Returns the index of a String. The Symbolic index is used as long as the application hasn't been
     * fully parsed.
     * @param string the String we want the index of.
     * @return the index of the String.
     */
    public int getStringIndex(String string) {
        if (useSymbolicElements) {
            return symbolicStringsToIndexes.get(string);
        } else {
            return resolvedStringsToIndexes.get(string);
        }
    }

    /**
     * Returns the Offset of a TypeList. <i>The Dex file must have been fully parsed before.</i>
     * @param typeList the TypeList to find.
     * @return the offset of the TypeList, from the beginning of the file.
     */
    public int getTypeListOffset(TypeList typeList) {
        return typeListsToOffsets.get(typeList);
    }

    /**
     * Sets an Offset to a TypeList.
     * @param typeList the TypeList to give the Offset.
     * @param offset the Offset to set.
     */
    public void setTypeListOffset(TypeList typeList, int offset) {
        typeListsToOffsets.put(typeList, offset);
    }

    /**
     * Returns the Index of a Type. The Symbolic index is used as long as the application hasn't been
     * fully parsed.
     * @param type name of the Type.
     * @return the index of the Type.
     */
    public int getTypeIndex(String type) {
        if (useSymbolicElements) {
            return symbolicTypesToIndexes.get(type);
        } else {
            return resolvedTypesToIndexes.get(type);
        }
    }

    /**
     * Returns the Index of a Prototype. <i>The Dex file must have been fully parsed before.</i>
     * @param prototype the Prototype.
     * @return the index of the Prototype.
     */
    public int getPrototypeIndex(Prototype prototype) {
        return prototypesToIndexes.get(prototype);
    }

    /**
     * Returns the Index of a Field. The Symbolic index is used as long as the application hasn't been
     * fully parsed.
     * @param field the Field.
     * @return the index of the Field.
     */
    public int getFieldIndex(Field field) {
        if (useSymbolicElements) {
            return symbolicFieldsToIndexes.get(field);
        } else {
            return resolvedFieldsToIndexes.get(field);
        }
    }

    /**
     * Returns the Index of a Method. The Symbolic index is used as long as the application hasn't been
     * fully parsed.
     * @param method the Method.
     * @return the index of the Method.
     */
    public int getMethodIndex(Method method) {
        if (useSymbolicElements) {
            return symbolicMethodsToIndexes.get(method);
        } else {
            return resolvedMethodsToIndexes.get(method);
        }
    }

    /**
     * Returns the offset of an annotation_item.
     * @param annotationItem the offset of the annotation_item.
     * @return the offset of the annotation_item.
     */
    public int getAnnotationItemOffset(AnnotationItem annotationItem) {
        return annotationItemsToOffsets.get(annotationItem);
    }

    /**
     * Sets the offset of an annotation_item.
     * @param annotationItem the annotation_item.
     * @param offset the Offset of the annotation_item.
     */
    public void setAnnotationItemOffset(AnnotationItem annotationItem, int offset) {
        annotationItemsToOffsets.put(annotationItem, offset);
    }

    /**
     * Returns the offset of an annotation_set_item.
     * @param annotationSetItem the annotation_set_item.
     * @return the offset of the annotation_set_item.
     */
    public int getAnnotationSetItemOffset(AnnotationSetItem annotationSetItem) {
        return annotationSetItemsToOffsets.get(annotationSetItem);
    }

    /**
     * Sets the offset of an annotation_set_item.
     * @param annotationSetItem the annotation_set_item.
     * @param offset the offset of the annotation_set_item.
     */
    public void setAnnotationSetItemOffset(AnnotationSetItem annotationSetItem, int offset) {
        annotationSetItemsToOffsets.put(annotationSetItem, offset);
    }

    /**
     * Returns the offset of an annotation_set_ref_list.
     * @param annotationSetRefList the annotation_set_ref_list.
     * @return offset the offset of the annotation_set_ref_list.
     */
    public int getAnnotationSetRefListOffset(AnnotationSetRefList annotationSetRefList) {
        return annotationSetRefListsToOffsets.get(annotationSetRefList);
    }

    /**
     * Sets the offset of an annotation_set_ref_list.
     * @param annotationSetRefList the annotation_set_ref_list.
     * @param offset the offset of the annotation_set_ref_list.
     */
    public void setAnnotationSetRefListOffset(AnnotationSetRefList annotationSetRefList, int offset) {
        annotationSetRefListsToOffsets.put(annotationSetRefList, offset);
    }

    /**
     * Returns the offset of an annotation_directory_item, or 0 if the Class doesn't have an
     * annotation_directory_item.
     * @param annotationDirectoryItem the annotation_directory_item.
     * @return the offset of the annotation_directory_item, or 0.
     */
    public int getAnnotationDirectoryItemOffset(AnnotationDirectoryItem annotationDirectoryItem) {
        if (classesToAnnotationDirectoryItemOffsets.containsKey(annotationDirectoryItem)) {
            return classesToAnnotationDirectoryItemOffsets.get(annotationDirectoryItem);
        } else {
            return 0;
        }
    }

    /**
     * Sets the offset of an annotation_directory_item.
     * @param annotationDirectoryItem the annotation_directory_item belongs to.
     * @param offset the offset of the annotation_directory_item.
     */
    public void setAnnotationDirectoryItemOffset(AnnotationDirectoryItem annotationDirectoryItem, int offset) {
        classesToAnnotationDirectoryItemOffsets.put(annotationDirectoryItem, offset);
    }

    /**
     * Returns the Set of annotation_items.
     * @return the Set of annotation_items.
     */
    public TreeSet<AnnotationItem> getAnnotationItems() {
        return annotationItems;
    }

    /**
     * Returns the set of annotation_set_item.
     * @return the set of annotation_set_item.
     */
    public TreeSet<AnnotationSetItem> getAnnotationSetItems() {
        return annotationSetItems;
    }

    /**
     * Returns the set of annotation_set_ref_list.
     * @return the set of annotation_set_ref_list.
     */
    public HashSet<AnnotationSetRefList> getAnnotationSetRefLists() {
        return annotationSetRefLists;
    }

    /**
     * Returns the count of annotation_items.
     * @return the count of annotation_items.
     */
    public int getAnnotationItemCount() {
        return annotationItems.size();
    }

    /**
     * Returns the count of annotation_set_items.
     * @return the count of annotation_set_items.
     */
    public int getAnnotationSetItemCount() {
        return annotationSetItems.size();
    }

    /**
     * Returns the count of annotation_set_ref_lists.
     * @return the count of annotation_set_ref_lists.
     */
    public int getAnnotationSetRefListsCount() {
        return annotationSetRefLists.size();
    }

    /**
     * Returns the count of the annotation_directory_item.
     * @return the count of the annotation_directory_item.
     */
    public int getAnnotationDirectoryItemCount() {
        return classesToAnnotationDirectoryItemOffsets.size();
    }

    /**
     * Returns the offset of the encoded_array_item of a static_values_offset for one Class if it exists.
     * @param cdi the Class.
     * @return the offset of the encoded_array_item of a static_values_offset for one Class, or 0 if no
     * 		   offset has been found.
     */
    public int getOffsetOfStaticValuesEncodedArrayItemOfClass(ClassDefinitionItem cdi) {
        int result = 0;
        if (staticValuesEncodedArrayItemOffsets.containsKey(cdi)) {
            result = staticValuesEncodedArrayItemOffsets.get(cdi);
        }
        return result;
    }

    /**
     * Returns an ordered list of the classes of the Application. It is only completed when the
     * Application has been fully parsed.
     * @return an ordered list of the classes of the Application.
     */
    public ArrayList<ClassDefinitionItem> getClasses() {
        return orderedClasses;
    }

    /**
     * Returns the annotation_directory_items, unique but non-ordered.
     * @return the annotation_directory_items.
     */
    public HashSet<AnnotationDirectoryItem> getAnnotationDirectoryItems() {
        return annotationDirectoryItems;
    }

    /**
     * Returns the resolved method index from the symbolic index. The application must have been
     * fully parsed for the conversion to be possible.
     * @param methodIndex the symbolic method index.
     * @return the resolved method index.
     */
    public int getResolvedMethodIndexFromSymbolicMethodIndex(int methodIndex) {
        return symbolicMethodsIndexToResolvedMethodsIndex[methodIndex];
    }

    /**
     * Returns the resolved Field index from the symbolic index. The application must have been
     * fully parsed for the conversion to be possible.
     * @param fieldIndex the symbolic Field index.
     * @return the resolved Field index.
     */
    public int getResolvedFieldIndexFromSymbolicFieldIndex(int fieldIndex) {
        return symbolicFieldsIndexToResolvedFieldsIndex[fieldIndex];
    }

    /**
     * Returns the resolved Type index from the symbolic index. The application must have been
     * fully parsed for the conversion to be possible.
     * @param typeIndex the symbolic Type index.
     * @return the resolved Type index.
     */
    public int getResolvedTypeIndexFromSymbolicTypeIndex(int typeIndex) {
        return symbolicTypesIndexToResolvedTypesIndex[typeIndex];
    }

    /**
     * Returns the resolved String index from the symbolic index. The application must have been
     * fully parsed for the conversion to be possible.
     * @param stringIndex the symbolic String index.
     * @return the resolved String index.
     */
    public int getResolvedStringIndexFromSymbolicStringIndex(int stringIndex) {
        return symbolicStringsIndexToResolvedStringsIndex[stringIndex];
    }


    // ---------------------------------------
    // Public methods.
    // ---------------------------------------

    /**
     * Adds a String to the Constant Pool. Nothing happens if the String already exists or is null.
     * Empty strings are authorized.
     * @param string the String to add.
     */
    public void addStringToConstantPool(String string) {
        if (string != null) {
            if (useSymbolicElements) {
                if (!symbolicStringsToIndexes.containsKey(string)) {
                    symbolicStringsToIndexes.put(string, symbolicStringsToIndexes.size());
                }
            }
            strings.add(string);
        }
    }

    /**
     * Adds Strings to the Constant pool. Nothing happens if the String already exists or is null.
     * Empty strings are authorized.
     * @param strings the Strings to add.
     */
    public void addStringsToConstantPool(String[] strings) {
        if (strings != null) {
            for (String string : strings) {
                addStringToConstantPool(string);
            }
        }
    }

    /**
     * Adds a Type to the Constant Pool. Nothing happens if the Type already exists, is null
     * or empty. The Type is added to both the Types and Strings sets.
     * @param type the Type to add.
     */
    public void addTypeToConstantPool(String type) {
        if ((type != null) && (!"".equals(type))) {
            addStringToConstantPool(type);
            if (useSymbolicElements) {
                if (!symbolicTypesToIndexes.containsKey(type)) {
                    symbolicTypesToIndexes.put(type, symbolicTypesToIndexes.size());
                }
            }
            types.add(type);
        }
    }

    /**
     * Adds Types to the Constant Pool. Nothing happens if the Types already exists, are null
     * or empty. The Type is added to both the Types and Strings sets.
     * @param types the Types to add.
     */
    public void addTypesToConstantPool(String[] types) {
        if (types != null) {
            for (String type : types) {
                addTypeToConstantPool(type);
            }
        }
    }

    /**
     * Adds a Prototype to the Constant Pool. Adds the Strings inside, but also the Prototype to the
     * Prototype Pool.
     * @param prototype Prototype to add.
     */
    public void addPrototypeToConstantPool(Prototype prototype) {
        Prototype existing = prototypes.get(prototype);
        if (existing == null) {
            prototype.initialize();
            addStringToConstantPool(prototype.getShortyDescriptor());
            addTypeToConstantPool(prototype.getReturnType());
            addTypeListToConstantPool(prototype.getParameterTypes());
            prototypes.put(prototype,prototype);
        }
    }

    /**
     * Creates a Prototype from the descriptor and adds it to the Constant Pool, as well as the
     * Strings inside. Returns the Prototype created.
     * @param descriptor full descriptor in the TypeDescriptor format.
     * @return the Prototype created.
     */
    public Prototype addPrototypeToConstantPool(String descriptor) {
        Prototype prototype = new Prototype(descriptor);
        Prototype existing = prototypes.get(prototype);
        if (existing == null) {
            addPrototypeToConstantPool(prototype);
        } else {
            prototype = existing;
        }
        return prototype;
    }


    /**
     * Adds a TypeList to the Constant Pool, as well as the Strings inside. If the TypeList is empty, it is
     * not added.
     * @param typeList the TypeList to add to the Constant Pool.
     */
    public void addTypeListToConstantPool(TypeList typeList) {
        if (typeList.size() > 0) {
            for (String type : typeList.getTypeList()) {
                addTypeToConstantPool(type);
            }
            this.typeList.add(typeList);
        }
    }

    /**
     * Adds a Method to the Constant Pool being built. Does nothing if the Constant Pool already
     * contains a similar item <i>unless the stored Method was tagged Unknown</i>, in which case it means
     * that it was just referred by an instruction, but not actually visited, and so lacks information.
     * This Method can be called when declaring a Method without knowing what is inside, for example
     * when a reference to this Method is found. In that case, the information are added.
     * The method name, descriptor, and class name are added to the constant pool if necessary.
     * In return is received the Method related to the given parameters.
     *
     * @param methodName the method name.
     * @param className the class name of the method.
     * @param methodDescriptor the method descriptor.
     * @param access the access flags of the method.
     * @param signature the signature of the method. May be Null.
     * @param exceptionNames the exceptions of the method. May be null.
     * @return the Method related to the given parameters.
     */
    public Method addMethodToConstantPool(String methodName, String className,
                                          String methodDescriptor, int access, String[] signature, String[] exceptionNames) {

        // int prototypeHashCode = Prototype.calculateHashCode(methodDescriptor);
        // int methodHashCode = Method.calculateHashCode(methodName, className, prototypeHashCode);
        Prototype prototype = addPrototypeToConstantPool(methodDescriptor);
        Method method = new Method(methodName, className, prototype);
        Method foundMethod = methods.get(method);
        if (foundMethod == null) {
            method.init(access, signature, exceptionNames, this);
            methods.put(method,method);
            symbolicMethodsToIndexes.put(method, symbolicMethodsToIndexes.size());

            // Add the name, descriptor, className and exceptions to the Constant Pool.
            addStringToConstantPool(methodName);
            addTypeToConstantPool(className);
            addStringsToConstantPool(signature);
            addTypesToConstantPool(exceptionNames);
            return method;
        } else {
            // The Method to add has already been added before, probably when it has been declared.
            // Maybe the current instance is the declaration. One of the two has incomplete information.
            // We must give them these information without replacing the object, because Instructions are
            // referring the incomplete Method.

            // If the current method is unknown, it is useless to get information from it.
            if (((access & Opcodes.ACC_UNKNOWN) == 0) && foundMethod.isUnknown()) {
                foundMethod.completeInformation(access, exceptionNames, signature, this);
            }
            return foundMethod; // In the end, we return the previously added Method.
        }


    }

    /**
     * Adds a Field in the Constant Pool. Does nothing if the constant pool already
     * contains a similar item. The field name, type, and class owning name are added to the constant
     * pool. In return is received the Field related to the given parameters.
     * This method can be called when declaring a Field without knowing what is inside, for example
     * when a reference to this Field is found. In that case, the Field access should be ACC_UNKNOWN.
     * @param fieldName name of the Field
     * @param desc type of the field in TypeDescriptor format.
     * @param classOwningName name of the class owning the Field.
     * @param access the access flags of the Field.
     * @param signature the Signature of the field. May be Null.
     * @param value the value of the Field. Null for non-final static Fields.
     * @return the Field related to the given parameters.
     */
    public Field addFieldToConstantPool(String fieldName, String desc, String classOwningName,
                                        int access, String[] signature, Object value) {

        // int fieldHashCode = Field.calculateHashCode(fieldName, desc, classOwningName);
        Field field = new Field(fieldName, desc, classOwningName);
        Field foundField = fields.get(field);
        if (foundField == null) {
            field.init(access, signature, value, this);
            fields.put(field,field);

            symbolicFieldsToIndexes.put(field, symbolicFieldsToIndexes.size());

            // Adds the name, type and class owning name to the constant pool.
            addStringToConstantPool(fieldName);
            addTypeToConstantPool(desc);
            addTypeToConstantPool(classOwningName);
            addStringsToConstantPool(signature);
            return field;
        } else {
            // The Field to add has already been added before, probably when it has been declared.
            // Or maybe the current instance is the declaration. One of the two has incomplete information.
            // We must give them these information without replacing the object, because Instructions are
            // referring the incomplete Field.

            // If the current Field is unknown, it is useless to get information from it.
            if (((access & Opcodes.ACC_UNKNOWN) == 0) && foundField.isUnknown()) {
                foundField.completeInformation(access, signature, value, this);
            }

            return foundField;  // In the end, we use the already added Field.
        }
    }

    /**
     * Adds a class to the constant pool. Nothing happens if the Class already exists.
     * The class name, superClass name, signature and interfaces name are added to the constant pool.
     * In return is received the ClassDefinitionItem related to the class.
     * @param className the name of the class.
     * @param superName the name of the super class.
     * @param access the access flags of the class.
     * @param interfaceNames the name of the interfaces of the class.
     * @param signature the signature of the class. Used only for generics. May be null.
     * @return the ClassDefinitionItem related to the class.
     */
    public ClassDefinitionItem addClassToConstantPool(String className, String superName,
            int access, String[] interfaceNames, String[] signature) {

        // We build a Class and add it to the Class constant pool.
        if (!classNameToClassDefinitionItem.containsKey(className)) {
            TypeList interfaceTypeList = new TypeList(interfaceNames);
            ClassDefinitionItem cdi = new ClassDefinitionItem(className, superName,
                    access, interfaceTypeList, signature, this);

            classNameToClassDefinitionItem.put(className, cdi);

            // Adds the name, superName, signature and interfaces to the constant pool.
            addTypeToConstantPool(className);
            addTypeToConstantPool(superName);
            addStringsToConstantPool(signature);

            addTypeListToConstantPool(interfaceTypeList);
            return cdi;
        } else {
            return classNameToClassDefinitionItem.get(className);
        }

    }

    /**
     * Adds an annotation_item to the Constant Pool. It must have its full content known.
     * @param annotationItem the annotation_item to add.
     */
    public void addAnnotationItemToConstantPool(AnnotationItem annotationItem) {
        annotationItems.add(annotationItem);
    }

    /**
     * Adds an annotation_set_item to the Constant Pool. It must have its full content known.
     * Doesn't add the annotation_set_item if it doesn't contain any annotation_item.
     * @param annotationSetItem the annotation_set_item to add.
     */
    public void addAnnotationSetItemToConstantPool(AnnotationSetItem annotationSetItem) {
        if (annotationSetItem.getNbAnnotationItems() > 0) {
            annotationSetItems.add(annotationSetItem);
        }
    }

    /**
     * Adds an annotation_set_item to the Constant Pool. It must have its full content known.
     * Even annotation_set_item with a size of 0 are added. This is especially useful for the
     * Annotated Parameter, as annotation_set_ref_list will probably contain empty annotation_set_items.
     * @param annotationSetItem the annotation_set_item to add.
     */
    public void addAnnotationSetItemNoSizeCheck(AnnotationSetItem annotationSetItem) {
        annotationSetItems.add(annotationSetItem);
    }

    /**
     * Adds an annotation_set_ref_list to the Constant Pool. It also registers all the annotation_set_items
     * it contains to the Constant Pool. It must have its full content known.
     * @param annotationSetRefList the annotation_set_ref_list to add.
     */
    public void addAnnotationSetRefListToConstantPool(AnnotationSetRefList annotationSetRefList) {
        if (annotationSetRefList.getNbAnnotationSetItemsUsed() > 0) {
            annotationSetRefLists.add(annotationSetRefList);

            // Registers all the annotation_set_items it contains.
            for (AnnotationSetItem annotationSetItem : annotationSetRefList.getAnnotationSetItems()) {
                addAnnotationSetItemNoSizeCheck(annotationSetItem);
            }
        }
    }

    /**
     * Adds an offset for the encoded_array_item of a static_values_offset for one Class.
     * @param cdi the class.
     * @param offset the offset to add.
     */
    public void addOffsetForStaticValuesEncodedArrayItemOfClass(ClassDefinitionItem cdi, int offset) {
        staticValuesEncodedArrayItemOffsets.put(cdi, offset);
    }

    /**
     * Adds an annotation_directory_item to the Constant Pool.
     * @param annotationDirectoryItem the annotation_directory_item to add.
     */
    public void addAnnotationDirectoryItem(AnnotationDirectoryItem annotationDirectoryItem) {
        annotationDirectoryItems.add(annotationDirectoryItem);
    }

    /**
     * Prepares the various elements to be generated. This must be done only once, after the whole
     * Application has been parsed. It consists in generating the sorted collections, and building the
     * Index Maps.
     */
    public void prepareGeneration() {
        generateSortedCollections();
        buildIndexMaps();
    }

    /**
     * Generates the sorted collections from the unsorted ones (it is faster to sort them once the
     * Application is parsed than adding elements one after one).
     */
    private void generateSortedCollections() {
        orderedTypeList = new TreeSet<TypeList>(typeList);
        orderedFields = new TreeSet<Field>(fields.values());
        orderedPrototypes = new TreeSet<Prototype>(prototypes.values());
        orderedMethods = new TreeSet<Method>(methods.values());
        sortClasses();
    }

    /**
     * Builds the Index Maps, allowing to link Strings, Types, Fields... to an Index, sorted in the order
     * requested by the Dex format.
     * Also creates the link between the symbolic elements and the resolved elements.
     * The application must have been fully parsed, and the sorted collections must have been generated
     * (generateSortedCollections method).
     */
    private void buildIndexMaps() {
        useSymbolicElements = false;
        int symbolicIndex;

        // Builds the Strings index map, and links the symbolic Strings indexes to the resolved Strings indexes.
        resolvedStringsToIndexes.clear();
        symbolicStringsIndexToResolvedStringsIndex = new int[strings.size()];
        int index = 0;
        for (String string : strings) {
            resolvedStringsToIndexes.put(string, index);

            symbolicIndex = symbolicStringsToIndexes.get(string);
            symbolicStringsIndexToResolvedStringsIndex[symbolicIndex] = index;

            index++;
        }

        // Builds the Types index map, and links the symbolic Types indexes to the resolved Types indexes.
        resolvedTypesToIndexes.clear();
        symbolicTypesIndexToResolvedTypesIndex = new int[types.size()];
        index = 0;
        for (String type : types) {
            resolvedTypesToIndexes.put(type, index);

            symbolicIndex = symbolicTypesToIndexes.get(type);
            symbolicTypesIndexToResolvedTypesIndex[symbolicIndex] = index;

            index++;
        }

        // Builds the Prototypes index map. The collection must be sorted first.
        prototypesToIndexes.clear();
        index = 0;
        for (Prototype prototype : orderedPrototypes) {
            prototypesToIndexes.put(prototype, index);
            index++;
        }

        // Builds the Fields index map, and links the symbolic Fields indexes to the resolved Types indexes.
        resolvedFieldsToIndexes.clear();
        symbolicFieldsIndexToResolvedFieldsIndex = new int[fields.size()];
        index = 0;
        for (Field field : orderedFields) {
            resolvedFieldsToIndexes.put(field, index);

            symbolicIndex = symbolicFieldsToIndexes.get(field);
            symbolicFieldsIndexToResolvedFieldsIndex[symbolicIndex] = index;

            index++;
        }

        // Builds the Methods index map, and links the symbolic Methods indexes to the resolved Types indexes.
        // The collection must be sorted first.
        resolvedMethodsToIndexes.clear();
        symbolicMethodsIndexToResolvedMethodsIndex = new int[orderedMethods.size()];
        index = 0;
        for (Method method : orderedMethods) {
            resolvedMethodsToIndexes.put(method, index);

            symbolicIndex = symbolicMethodsToIndexes.get(method);
            symbolicMethodsIndexToResolvedMethodsIndex[symbolicIndex] = index;

            index++;
        }
    }


    /**
     * Sorts the Classes into a new List. Each Class must be written after its possible Parent.
     */
    private void sortClasses() {
        orderedClasses = new ArrayList<ClassDefinitionItem>();

        Collection<ClassDefinitionItem> classes = classNameToClassDefinitionItem.values();
        for (ClassDefinitionItem cdi : classes) {
            sortClassRec(cdi);
        }

        if (classNameToClassDefinitionItem.size() != orderedClasses.size()) {
            try {
                throw new Exception("Sorted Classes list doesn't match the original list !");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void sortClassRec(ClassDefinitionItem cdi) {
        // We make sure the SuperClass hasn't already been encoded.
        if (!orderedClasses.contains(cdi)) {
            String superClass = cdi.getSuperClassName();
            // Checks the super class, if any.
            if (superClass != null) {
                sortClassRecString(superClass);
            }
            // Checks the interfaces, if any.
            TypeList tl = cdi.getInterfaces();
            for (String className : tl.getTypeList()) {
                sortClassRecString(className);
            }

            orderedClasses.add(cdi);
        }
    }

    private void sortClassRecString(String className) {
        ClassDefinitionItem ncdi = classNameToClassDefinitionItem.get(className);
        // The SuperClass may not be found if it's a Java SuperClass like Object, Enum etc.
        // We also make sure the SuperClass hasn't already been encoded.
        if ((ncdi != null) && (!orderedClasses.contains(ncdi))) {
            sortClassRec(ncdi);
        }
    }




}
