#ifndef METADATAENTRY_H_
#define METADATAENTRY_H_

#include <string>
#include "jni.h"
#include "MetadataTreeNode.h"

namespace tns
{
	enum class NodeType
	{
		Package,
		Class,
		Interface,
		Method,
		Field,
		StaticField
	};

	enum class MethodReturnType
	{
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

	struct MetadataEntry
	{
		MetadataEntry()
			: isTypeMember(false), name(std::string()), treeNode(nullptr), sig(std::string()), returnType(std::string()), retType(MethodReturnType::Unknown), paramCount(0),
			  isStatic(false), isFinal(false), declaringType(std::string()),
			  isResolved(false), memberId(nullptr), clazz(nullptr)
		{
		}
		MetadataTreeNode *treeNode;
		NodeType type;
		std::string name;
		std::string sig;
		std::string returnType;
		MethodReturnType retType;
		std::string declaringType;
		int paramCount;
		bool isStatic;
		bool isFinal;
		bool isTypeMember;
		bool isResolved;
		void *memberId;
		jclass clazz;
		std::vector<std::string> parsedSig;
	};
}

#endif /* METADATAENTRY_H_ */
