#ifndef METADATAENTRY_H_
#define METADATAENTRY_H_

#include <string>
#include "jni.h"
#include "MetadataTreeNode.h"
#include "MetadataMethodInfo.h"
#include "MetadataFieldInfo.h"

namespace tns {
    enum class NodeType {
        Package,
        Class,
        Interface,
        Method,
        Field,
        StaticField
    };

    enum class MethodReturnType {
        Unknown,
        Void,
        Byte,
        Short,
        Int,
        Long,
        Float,
        Double,
        Char,
        Boolean,
        String,
        Object
    };

    class MetadataEntry {
    public:

        MetadataEntry(MetadataTreeNode* m_treeNode, NodeType nodeType);

        std::string& getName();
        std::string& getSig();
        std::string& getReturnType();
        MethodReturnType getRetType();
        std::string& getDeclaringType();
        int getParamCount();
        bool getIsFinal();
        bool getIsResolved();

        MetadataTreeNode* treeNode;
        NodeType type;
        bool isExtensionFunction;
        bool isStatic;
        bool isTypeMember;
        void* memberId;
        jclass clazz;
        std::vector<std::string> parsedSig;

        MethodInfo* mi;
        FieldInfo* fi;
        StaticFieldInfo* sfi;

        std::string name;
        std::string sig;
        std::string returnType;
        MethodReturnType retType;
        std::string declaringType;
        int paramCount;
        bool isFinal;
        bool isResolved;

    private:

        bool retTypeParsed;
        bool isFinalSet;
        bool isResolvedSet;

    };
}

#endif /* METADATAENTRY_H_ */
