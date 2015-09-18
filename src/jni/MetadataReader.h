#ifndef METADATAREADER_H_
#define METADATAREADER_H_

#include "MetadataEntry.h"
#include "MetadataFieldInfo.h"
#include <map>
#include <string>

namespace tns
{
	typedef std::vector<std::string> (*GetTypeMetadataCallback)(const std::string& classname, int index);

	class MethodInfo;

	class MetadataReader
	{
	public:
		MetadataReader();

		MetadataReader(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength, uint8_t *nameData, uint32_t valueLength, uint8_t *valueData, GetTypeMetadataCallback getTypeMetadataCallack);

		MetadataEntry ReadInstanceMethodEntry(uint8_t **data);

		MetadataEntry ReadStaticMethodEntry(uint8_t **data);

		MetadataEntry ReadInstanceFieldEntry(uint8_t **data);

		MetadataEntry ReadStaticFieldEntry(uint8_t **data);

		std::string ReadTypeName(uint16_t nodeId);

		std::string ReadTypeName(MetadataTreeNode *treeNode);

		std::string ReadName(uint32_t offset);

		std::string ReadInterfaceImplementationTypeName(MetadataTreeNode *treeNode, bool& isPrefix);

		uint8_t* GetValueData() const;

		uint8_t GetNodeType(MetadataTreeNode *treeNode);

		uint16_t GetNodeId(MetadataTreeNode *treeNode);

		MetadataTreeNode* GetRoot() const;

		MetadataTreeNode* GetOrCreateTreeNodeByName(const std::string& className);

		MetadataTreeNode* GetBaseClassNode(MetadataTreeNode *treeNode);

		MetadataTreeNode* GetNodeById(uint16_t nodeId);

		bool IsPrimitive(uint8_t type);

		bool IsNodeTypeArray(uint8_t type);

		bool IsNodeTypeStatic(uint8_t type);

		bool IsNodeTypeClass(uint8_t type);

		bool IsNodeTypeInterface(uint8_t type);

		bool IsNodeTypePackage(uint8_t type);

		static void FillReturnType(MetadataEntry& entry);

		static std::string ParseReturnType(const std::string& signature);

		static MethodReturnType GetReturnType(const std::string& returnType);

	private:

		static const uint32_t ARRAY_OFFSET = 1000000000;

		MetadataTreeNode* BuildTree();

		std::string ReadTypeNameInternal(MetadataTreeNode *treeNode);

		void FillEntryWithFiedldInfo(FieldInfo *fi, MetadataEntry& entry);

		void FillEntryWithMethodInfo(MethodInfo& mi, MetadataEntry& entry);

		MetadataTreeNode *m_root;
		uint32_t m_nodesLength;
		uint32_t m_nameLength;
		uint32_t m_valueLength;
		uint8_t *m_nodeData;
		uint8_t *m_nameData;
		uint8_t *m_valueData;
		std::vector<MetadataTreeNode*> m_v;
		GetTypeMetadataCallback m_getTypeMetadataCallback;

		std::map<MetadataTreeNode*, std::string> m_typeNameCache;
	};
}


#endif /* METADATAREADER_H_ */
