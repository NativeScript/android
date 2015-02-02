#include "MetadataReader.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include <sstream>
#include <assert.h>


using namespace std;
using namespace tns;

MetadataReader::MetadataReader()
	: m_nodesLength(0), m_nodeData(nullptr), m_nameData(nullptr), m_valueData(nullptr), m_root(nullptr), m_getTypeMetadataCallback(nullptr)
{
}

MetadataReader::MetadataReader(uint32_t nodesLength, uint8_t *nodeData, uint8_t *nameData, uint8_t *valueData, GetTypeMetadataCallback getTypeMetadataCallback)
	: m_nodesLength(nodesLength), m_nodeData(nodeData), m_nameData(nameData), m_valueData(valueData), m_getTypeMetadataCallback(getTypeMetadataCallback)
{
	m_root = BuildTree();
}

MetadataTreeNode* MetadataReader::BuildTree()
{
	MetadataTreeNodeRawData *rootNodeData = reinterpret_cast<MetadataTreeNodeRawData*>(m_nodeData);

	MetadataTreeNodeRawData *curNodeData = rootNodeData;

	int len = m_nodesLength / sizeof(MetadataTreeNodeRawData);

	m_v.resize(len);
	MetadataTreeNode *emptyNode = nullptr;
	fill(m_v.begin(), m_v.end(), emptyNode);

	for (int i = 0; i < len; i++)
	{
		MetadataTreeNode *node = m_v[i];
		if (nullptr == node)
		{
			node = new MetadataTreeNode;
			node->name = ReadName(m_nameData, curNodeData->offsetName);
			node->offsetValue = curNodeData->offsetValue;
			m_v[i] = node;
		}

		uint16_t curNodeDataId = curNodeData - rootNodeData;

		if (curNodeDataId != curNodeData->firstChildId)
		{
			node->children = new vector<MetadataTreeNode*>;
			MetadataTreeNodeRawData *childNodeData = rootNodeData + curNodeData->firstChildId;
			while (true)
			{
				MetadataTreeNode *childNode = new MetadataTreeNode;
				childNode->parent = node;
				childNode->name = ReadName(m_nameData, childNodeData->offsetName);
				childNode->offsetValue = childNodeData->offsetValue;

				node->children->push_back(childNode);

				uint16_t childNodeDataId = childNodeData - rootNodeData;

				m_v[childNodeDataId] = childNode;

				if (childNodeDataId == childNodeData->nextSiblingId)
					break;

				childNodeData = rootNodeData + childNodeData->nextSiblingId;
			}
		}

		curNodeData++;
	}

	return m_v[0];
}

MetadataEntry MetadataReader::ReadInstanceFieldEntry(uint8_t*& data)
{
	MetadataEntry entry;
	uint8_t *curPtr = data;

	//read name
	uint32_t nameOffset = *reinterpret_cast<uint32_t*>(curPtr);
	string name = ReadName(m_nameData, nameOffset);
	curPtr += sizeof(uint32_t);

	uint16_t *nodeIdPtr = reinterpret_cast<uint16_t*>(curPtr);

	//	read node pointer id
	uint16_t nodeId = *nodeIdPtr++;
	string fieldTypeName = ReadTypeName(nodeId);

	// read modifier
	uint8_t *nodeFinalVarPtr = reinterpret_cast<uint8_t*>(nodeIdPtr);
	uint8_t nodeFinalVar = *nodeFinalVarPtr;
	entry.isFinal = nodeFinalVar == MetadataTreeNode::FINAL;
	nodeFinalVarPtr += sizeof(uint8_t);

	entry.name = name;
	entry.type = NodeType::Field;
	entry.sig = fieldTypeName;
	entry.isMember = true;

	data = nodeFinalVarPtr;

	return entry;
}

MetadataEntry MetadataReader::ReadStaticFieldEntry(uint8_t*& data)
{
	uint8_t *curPtr = data;
	MetadataEntry entry = ReadInstanceFieldEntry(data);

	uint16_t *nodeIdPtr = reinterpret_cast<uint16_t*>(data);
	uint16_t nodeId = *nodeIdPtr++;
	string declTypeName = ReadTypeName(nodeId);

	entry.type = NodeType::StaticField;
	entry.declaringType = declTypeName;
	entry.isStatic = true;

	data = reinterpret_cast<uint8_t*>(nodeIdPtr);

	return entry;
}

MetadataEntry MetadataReader::ReadInstanceMethodEntry(uint8_t*& data)
{
	MetadataEntry entry;
	uint8_t *curPtr = data;

	uint32_t nameOffset = *reinterpret_cast<uint32_t*>(curPtr);
	string name = ReadName(m_nameData, nameOffset);
	curPtr += sizeof(uint32_t);

	uint16_t sigLen = *reinterpret_cast<uint16_t*>(curPtr);
	assert(sigLen > 0);
	curPtr += sizeof(uint16_t);
	uint16_t *nodeIdPtr = reinterpret_cast<uint16_t*>(curPtr);
	string sig = "(";
	string ret;
	for (int i=0; i<sigLen; i++)
	{
		uint16_t nodeId = *nodeIdPtr++;
		string curArgTypeName = ReadTypeName(nodeId);
		MetadataTreeNode *node = m_v[nodeId];
		uint8_t nodeType = GetNodeType(node);
		bool isRefType = IsNodeTypeClass(nodeType) || IsNodeTypeInterface(nodeType);
		if (i == 0)
		{
			if ((curArgTypeName[0] != '[') && isRefType)
			{
				ret.append("L");
			}
			ret.append(curArgTypeName);
			if ((curArgTypeName[0] != '[') && isRefType)
			{
				ret.append(";");
			}
		}
		else
		{
			if ((curArgTypeName[0] != '[') && isRefType)
			{
				sig.append("L");
			}
			sig.append(curArgTypeName);
			if ((curArgTypeName[0] != '[') && isRefType)
			{
				sig.append(";");
			}
		}
	}
	if (ret.empty())
	{
		ret = "V";
	}
	sig += ")" + ret;

	entry.name = name;
	entry.paramCount = sigLen - 1;
	entry.sig = sig;
	entry.type = NodeType::Method;
	entry.isMember = true;

	data = reinterpret_cast<uint8_t*>(nodeIdPtr);

	return entry;
}

MetadataEntry MetadataReader::ReadStaticMethodEntry(uint8_t*& data)
{
	uint8_t *curPtr = data;

	MetadataEntry entry = ReadInstanceMethodEntry(data);

	uint16_t *nodeIdPtr = reinterpret_cast<uint16_t*>(data);

	uint16_t nodeId = *nodeIdPtr++;
	string declTypeName = ReadTypeName(nodeId);

	// TODO: static method?
	entry.type = NodeType::Method;
	entry.declaringType = declTypeName;
	entry.isStatic = true;

	data = reinterpret_cast<uint8_t*>(nodeIdPtr);

	return entry;
}

string MetadataReader::ReadInterfaceImplementationTypeName(MetadataTreeNode *treeNode, bool& isPrefix)
{
	uint8_t *data = m_valueData + treeNode->offsetValue + sizeof(uint8_t) + sizeof(uint16_t);

	isPrefix = *data == 1;

	uint32_t pos = *reinterpret_cast<uint32_t*>(data + sizeof(uint8_t));

	uint16_t len = *reinterpret_cast<uint16_t*>(m_nameData + pos);

	char *ptr = reinterpret_cast<char*>(m_nameData + pos + sizeof(uint16_t));

	string name(ptr, len);

	assert(name.length() == len);

	return name;
}

string MetadataReader::ReadName(uint8_t *names, uint32_t offset)
{
	uint16_t length = *reinterpret_cast<short*>(names + offset);

	string name(reinterpret_cast<char*>(names + offset + sizeof(uint16_t)), length);

	return name;
}

string MetadataReader::ReadTypeName(uint16_t nodeId)
{
	MetadataTreeNode *treeNode = m_v[nodeId];

	return ReadTypeName(treeNode);
}

string MetadataReader::ReadTypeName(MetadataTreeNode *treeNode)
{
	string name;

	auto itFound = m_typeNameCache.find(treeNode);

	if (itFound != m_typeNameCache.end())
	{
		name = itFound->second;
	}
	else
	{
		name = ReadTypeNameInternal(treeNode);

		m_typeNameCache.insert(make_pair(treeNode, name));
	}

	return name;
}

string MetadataReader::ReadTypeNameInternal(MetadataTreeNode *treeNode)
{
	string name;

	uint8_t prevNodeType;

	while (treeNode->parent != nullptr)
	{
		int curNodeType = GetNodeType(treeNode);

		bool isArrayElement = treeNode->offsetValue > ARRAY_OFFSET;

		if (isArrayElement)
		{
			uint16_t forwardNodeId = treeNode->offsetValue - ARRAY_OFFSET;
			MetadataTreeNode *forwardNode = m_v[forwardNodeId];
			name = ReadTypeName(forwardNode);
			uint8_t forwardNodeType = GetNodeType(forwardNode);
			if (IsNodeTypeInterface(forwardNodeType) || IsNodeTypeClass(forwardNodeType))
				name = "L" + name + ";";
		}
		else
		{
			if (!name.empty())
			{
				if (!IsNodeTypeArray(curNodeType))
				{
					if ((IsNodeTypeClass(prevNodeType) || IsNodeTypeInterface(prevNodeType))
						&& (IsNodeTypeClass(curNodeType) || IsNodeTypeInterface(curNodeType)))
					{
						name = "$" + name;
					}
					else
					{
						name = "/" + name;
					}
				}
			}

			name = treeNode->name + name;

			prevNodeType = curNodeType;
		}

		treeNode = treeNode->parent;
	}

	return name;
}

bool MetadataReader::IsNodeTypeArray(uint8_t type)
{
	bool isArray = (((type & MetadataTreeNode::PRIMITIVE) == 0)
					&& ((type & MetadataTreeNode::ARRAY) == MetadataTreeNode::ARRAY));

	return isArray;
}

bool MetadataReader::IsNodeTypeStatic(uint8_t type)
{
	bool isStatic = (type & MetadataTreeNode::STATIC) == MetadataTreeNode::STATIC;

	return isStatic;
}

bool MetadataReader::IsNodeTypeClass(uint8_t type)
{
	bool isClass = (((type & MetadataTreeNode::PRIMITIVE) == 0)
					&& ((type & MetadataTreeNode::CLASS) == MetadataTreeNode::CLASS));

	return isClass;
}

bool MetadataReader::IsNodeTypeInterface(uint8_t type)
{
	bool isInterface = (((type & MetadataTreeNode::PRIMITIVE) == 0)
						&& ((type & MetadataTreeNode::INTERFACE) == MetadataTreeNode::INTERFACE));

	return isInterface;
}

bool MetadataReader::IsNodeTypePackage(uint8_t type)
{
	bool isPackage = type == MetadataTreeNode::PACKAGE;

	return isPackage;
}

bool MetadataReader::IsPrimitive(uint8_t type)
{
	bool isPrimitive = (type & MetadataTreeNode::PRIMITIVE) == MetadataTreeNode::PRIMITIVE;

	return isPrimitive;
}

uint8_t* MetadataReader::GetValueData() const
{
	return m_valueData;
}

uint16_t MetadataReader::GetNodeId(MetadataTreeNode *treeNode)
{
	auto itFound = find(m_v.begin(), m_v.end(), treeNode);
	assert(itFound != m_v.end());
	uint16_t nodeId = itFound - m_v.begin();

	return nodeId;
}

MetadataTreeNode* MetadataReader::GetRoot() const
{
	return m_root;
}

uint8_t MetadataReader::GetNodeType(MetadataTreeNode *treeNode)
{
	if (treeNode->type == MetadataTreeNode::INVALID_TYPE)
	{
		uint8_t nodeType;

		if (treeNode->metadata == nullptr)
		{
			uint32_t offsetValue = treeNode->offsetValue;

			if (offsetValue == 0)
			{
				nodeType = MetadataTreeNode::PACKAGE;
			}
			else if ((0 < offsetValue) && (offsetValue < ARRAY_OFFSET))
			{
				nodeType = *(m_valueData + offsetValue);
			}
			else if (offsetValue == ARRAY_OFFSET)
			{
				nodeType = MetadataTreeNode::ARRAY;
			}
			else
			{
				uint16_t nodeId = offsetValue - ARRAY_OFFSET;
				MetadataTreeNode *arrElemNode = m_v[nodeId];
				nodeType = *(m_valueData + arrElemNode->offsetValue);
			}
		}
		else
		{
			const auto& metadata = *treeNode->metadata;
			if (metadata == "P")
			{
				nodeType = MetadataTreeNode::PACKAGE;
			}
			else
			{
				string line;
				stringstream s(metadata);
				getline(s, line);

				string typePart;
				string instancePart;
				stringstream tmp(line);
				tmp >> typePart >> instancePart;

				bool isInterface = typePart == "I";
				bool isStatic = instancePart == "S";

				nodeType = isInterface ? MetadataTreeNode::INTERFACE : MetadataTreeNode::CLASS;

				if (isStatic)
					nodeType |= MetadataTreeNode::STATIC;
			}
		}

		treeNode->type = nodeType;
	}

	return treeNode->type;
}

MetadataTreeNode* MetadataReader::GetOrCreateTreeNodeByName(const string& className)
{
	MetadataTreeNode *treeNode = GetRoot();

	int arrayIdx = -1;
	string arrayName = "[";

	while (className[++arrayIdx] == '[')
	{
		MetadataTreeNode *child = treeNode->GetChild(arrayName);

		if (child == nullptr)
		{
			vector<MetadataTreeNode*> *children = treeNode->children;
			if (children == nullptr)
				children = treeNode->children = new vector<MetadataTreeNode*>;

			child = new MetadataTreeNode;
			child->name = "[";
			child->parent = treeNode;
			child->offsetValue = ARRAY_OFFSET;

			children->push_back(child);
			m_v.push_back(child);
		}

		treeNode = child;
	}

	string cn = className.substr(arrayIdx);

	if (arrayIdx > 0)
	{
		char last = *cn.rbegin();
		if (last == ';')
			cn = cn.substr(1, cn.length() - 2);
	}

	vector<string> names;
	Util::SplitString(cn, "/$", names);

	if (arrayIdx > 0)
	{
		bool found = false;
		MetadataTreeNode *forwardedNode = GetOrCreateTreeNodeByName(cn);

		uint16_t forwardedNodeId = GetNodeId(forwardedNode);
		vector<MetadataTreeNode*>& children = *treeNode->children;
		for (auto childNode: children)
		{
			assert(childNode->offsetValue >= ARRAY_OFFSET);
			uint32_t childNodeId = childNode->offsetValue - ARRAY_OFFSET;

			if (childNodeId == forwardedNodeId)
			{
				treeNode = childNode;
				found = true;
				break;
			}
		}

		if (!found)
		{
			MetadataTreeNode *forwardNode = new MetadataTreeNode;
			forwardNode->offsetValue = forwardedNodeId + ARRAY_OFFSET;
			forwardNode->parent = treeNode;

			m_v.push_back(forwardNode);
			children.push_back(forwardNode);

			treeNode = forwardNode;
		}

		return treeNode;
	}

	MetadataTreeNode *newType = nullptr;

	int curIdx = 0;
	for (auto it = names.begin(); it != names.end(); ++it)
	{
		MetadataTreeNode *child = treeNode->GetChild(*it);

		if (child == nullptr)
		{
			vector<string> api = m_getTypeMetadataCallback(cn, curIdx);

			for (auto part: api)
			{
				vector<MetadataTreeNode*> *children = treeNode->children;
				if (children == nullptr)
					children = treeNode->children = new vector<MetadataTreeNode*>;

				child = new MetadataTreeNode;
				child->name = *it++;
				child->parent = treeNode;
				child->metadata = new string(part);

				m_v.push_back(child);
				children->push_back(child);

				newType = treeNode = child;
			}

			return treeNode;
		}
		else
		{
			treeNode = child;
		}
		++curIdx;
	}

	return treeNode;
}

MetadataTreeNode* MetadataReader::GetBaseClassNode(MetadataTreeNode *treeNode)
{
	MetadataTreeNode *baseClassNode = nullptr;

	uint16_t baseClassNodeId = *reinterpret_cast<uint16_t*>(m_valueData + treeNode->offsetValue + 1);

	size_t nodeCount = m_v.size();

	assert(baseClassNodeId < nodeCount);

	baseClassNode = m_v[baseClassNodeId];

	return baseClassNode;
}


