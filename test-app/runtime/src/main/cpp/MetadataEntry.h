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

        MetadataEntry(MetadataTreeNode *m_treeNode, NodeType nodeType);

        MetadataEntry(const MetadataEntry &other) = default;

        MetadataEntry &operator=(const MetadataEntry &other) {
            if (this != &other) {
                treeNode = other.treeNode;
                type = other.type;
                isExtensionFunction = other.isExtensionFunction;
                isStatic = other.isStatic;
                isTypeMember = other.isTypeMember;
                memberId = other.memberId;
                clazz = other.clazz;
                parsedSig = other.parsedSig;
                mi = other.mi;
                field = other.field;
                name = other.name;
                sig = other.sig;
                returnType = other.returnType;
                retType = other.retType;
                declaringType = other.declaringType;
                paramCount = other.paramCount;
                isFinal = other.isFinal;
                isResolved = other.isResolved;
                isResolvedSet = other.isResolvedSet;
                isFinalSet = other.isFinalSet;
            }
            return *this;
        }

        std::string &getName() {
            return !name.empty() ? name : readName();
        }

        std::string &getSig() {
            return !sig.empty() ? sig : readSig();
        }

        std::string &getReturnType() {
            return !returnType.empty() ? returnType : readReturnType();
        }

        MethodReturnType getRetType() {
            return retTypeParsed ? retType : readRetType();
        }

        std::string &getDeclaringType() {
            return !declaringType.empty() ? declaringType : readDeclaringType();
        }

        int getParamCount() {
            return paramCount != -1 ? paramCount : readParamCount();
        }

        bool getIsFinal() {
            return isFinalSet ? isFinal : readIsFinal();
        }

        bool getIsResolved() {
            return isResolvedSet ? isResolved : readIsResolved();
        }

        MetadataTreeNode *treeNode;
        NodeType type;
        bool isExtensionFunction;
        bool isStatic;
        bool isTypeMember;
        void *memberId;
        jclass clazz;
        std::vector<std::string> parsedSig;

        MethodInfo mi;
        // A field's record in the value stream.
        const uint8_t *field;

        std::string name;
        std::string sig;
        std::string returnType;
        MethodReturnType retType;
        std::string declaringType;
        int paramCount;
        bool isFinal;
        bool isResolved;

    private:

        std::string &readName();

        std::string &readSig();

        std::string &readReturnType();

        MethodReturnType readRetType();

        std::string &readDeclaringType();

        int readParamCount();

        bool readIsFinal();

        bool readIsResolved();

        bool retTypeParsed;
        bool isFinalSet;
        bool isResolvedSet;

    };
}

#endif /* METADATAENTRY_H_ */
