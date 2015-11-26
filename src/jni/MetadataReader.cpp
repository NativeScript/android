#include "MetadataReader.h"
#include "MetadataMethodInfo.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include <sstream>
#include <assert.h>



using namespace std;
using namespace tns;

MetadataReader::MetadataReader()
	: m_nodesLength(0), m_nodeData(nullptr), m_nameLength(0), m_nameData(nullptr), m_valueLength(0), m_valueData(nullptr), m_root(nullptr), m_getTypeMetadataCallback(nullptr)
{
}

MetadataReader::MetadataReader(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength, uint8_t *nameData, uint32_t valueLength, uint8_t *valueData, GetTypeMetadataCallback getTypeMetadataCallback)
	: m_nodesLength(nodesLength), m_nodeData(nodeData), m_nameLength(nameLength), m_nameData(nameData), m_valueLength(valueLength), m_valueData(valueData), m_getTypeMetadataCallback(getTypeMetadataCallback)
{
	m_root = BuildTree();
}

MetadataTreeNode* MetadataReader::BuildTree()
{
	MetadataTreeNodeRawData *rootNodeData = reinterpret_cast<MetadataTreeNodeRawData*>(m_nodeData);

	MetadataTreeNodeRawData *curNodeData = rootNodeData;

	int len = m_nodesLength / sizeof(MetadataTreeNodeRawData);

	m_v.resize(len + 1000);
	MetadataTreeNode *emptyNode = nullptr;
	fill(m_v.begin(), m_v.end(), emptyNode);

	for (int i = 0; i < len; i++)
	{
		MetadataTreeNode *node = GetNodeById(i);
		if (nullptr == node)
		{
			node = new MetadataTreeNode;
			node->name = ReadName(curNodeData->offsetName);
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
				childNode->name = ReadName(childNodeData->offsetName);
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

	return GetNodeById(0);
}

void MetadataReader::FillEntryWithFiedldInfo(FieldInfo *fi, MetadataEntry& entry)
{
	entry.isTypeMember = true;
	entry.name = ReadName(fi->nameOffset);
	entry.sig = ReadTypeName(fi->nodeId);
	entry.isFinal = fi->finalModifier == MetadataTreeNode::FINAL;
}

void MetadataReader::FillEntryWithMethodInfo(MethodInfo& mi, MetadataEntry& entry)
{
	entry.type = NodeType::Method;
	entry.isTypeMember = true;
	entry.name = mi.GetName();
	entry.isResolved = mi.CheckIsResolved() == 1;
	uint16_t sigLength = mi.GetSignatureLength();
	assert(sigLength > 0);
	entry.paramCount = sigLength - 1;
	entry.sig = mi.GetSignature();
	FillReturnType(entry);
}

string MetadataReader::ParseReturnType(const string& signature)
{
	int idx = signature.find(')');
	auto returnType = signature.substr(idx + 1);
	return returnType;
}

MethodReturnType MetadataReader::GetReturnType(const string& returnType)
{
	MethodReturnType retType;
	char retTypePrefix = returnType[0];
	switch (retTypePrefix)
	{
		case 'V':
			retType = MethodReturnType::Void;
			break;
		case 'B':
			retType = MethodReturnType::Byte;
			break;
		case 'S':
			retType = MethodReturnType::Short;
			break;
		case 'I':
			retType = MethodReturnType::Int;
			break;
		case 'J':
			retType = MethodReturnType::Long;
			break;
		case 'F':
			retType = MethodReturnType::Float;
			break;
		case 'D':
			retType = MethodReturnType::Double;
			break;
		case 'C':
			retType = MethodReturnType::Char;
			break;
		case 'Z':
			retType = MethodReturnType::Boolean;
			break;
		case '[':
		case 'L':
			retType = (returnType == "Ljava/lang/String;")
						? MethodReturnType::String
						: MethodReturnType::Object;
			break;
		default:
			assert(false);
			break;
	}
	return retType;
}

void MetadataReader::FillReturnType(MetadataEntry& entry)
{
	entry.returnType = ParseReturnType(entry.sig);
	entry.retType = GetReturnType(entry.returnType);
}

MetadataEntry MetadataReader::ReadInstanceFieldEntry(uint8_t **data)
{
	FieldInfo *fi = *reinterpret_cast<FieldInfo**>(data);

	MetadataEntry entry;
	FillEntryWithFiedldInfo(fi, entry);
	entry.isStatic = false;
	entry.type = NodeType::Field;

	*data += sizeof(FieldInfo);

	return entry;
}

MetadataEntry MetadataReader::ReadStaticFieldEntry(uint8_t **data)
{
	StaticFieldInfo *sfi = *reinterpret_cast<StaticFieldInfo**>(data);

	MetadataEntry entry;
	FillEntryWithFiedldInfo(sfi, entry);
	entry.isStatic = true;
	entry.type = NodeType::StaticField;
	entry.declaringType = ReadTypeName(sfi->declaringType);

	*data += sizeof(StaticFieldInfo);

	return entry;
}

MetadataEntry MetadataReader::ReadInstanceMethodEntry(uint8_t **data)
{
	MetadataEntry entry;
	MethodInfo mip(*data, this); //method info pointer+

	FillEntryWithMethodInfo(mip, entry);

	*data += mip.GetSizeOfReadMethodInfo();

	return entry;
}

MetadataTreeNode* MetadataReader::GetNodeById(uint16_t nodeId)
{
	return m_v[nodeId];
}

MetadataEntry MetadataReader::ReadStaticMethodEntry(uint8_t** data)
{
	MetadataEntry entry;
	MethodInfo smip(*data, this); //static method info pointer

	FillEntryWithMethodInfo(smip, entry);
	entry.isStatic = true;
	entry.declaringType = smip.GetDeclaringType();

	*data += smip.GetSizeOfReadMethodInfo();

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

string MetadataReader::ReadName(uint32_t offset)
{
	uint16_t length = *reinterpret_cast<short*>(m_nameData + offset);

	string name(reinterpret_cast<char*>(m_nameData + offset + sizeof(uint16_t)), length);

	return name;
}

string MetadataReader::ReadTypeName(uint16_t nodeId)
{
	MetadataTreeNode *treeNode = GetNodeById(nodeId);

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
			MetadataTreeNode *forwardNode = GetNodeById(forwardNodeId);
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
			MetadataTreeNode *arrElemNode = GetNodeById(nodeId);
			nodeType = *(m_valueData + arrElemNode->offsetValue);
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
			uint32_t childNodeId = (childNode->offsetValue >= ARRAY_OFFSET)
									? (childNode->offsetValue - ARRAY_OFFSET)
									: GetNodeId(childNode);

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

				string line;
				string kind;
				string name;
				stringstream s(part);

				getline(s, line);
				stringstream typeLine(line);
				typeLine >> kind >> name;
				auto cKind = kind[0];

				// package, class, interface
				assert((cKind == 'P') || (cKind == 'C') || (cKind == 'I'));

				if ((cKind == 'C') || (cKind == 'I'))
				{
					child->metadata = new string(part);
					child->type = (cKind == 'C') ? MetadataTreeNode::CLASS : MetadataTreeNode::INTERFACE;
					if (name == "S")
					{
						child->type |= MetadataTreeNode::STATIC;
					}

					getline(s, line);
					stringstream baseClassLine(line);
					baseClassLine >> kind >> name;
					cKind = kind[0];

					assert(cKind == 'B');
					auto baseClassTreeNode = GetOrCreateTreeNodeByName(name);
					auto baseClassNodeId = GetNodeId(baseClassTreeNode);

					child->offsetValue = m_valueLength;
					m_valueData[m_valueLength++] = child->type;
					*reinterpret_cast<uint16_t*>(m_valueData + m_valueLength) = baseClassNodeId;
					m_valueLength += sizeof(uint16_t);
				}
				else
				{
					child->type = MetadataTreeNode::PACKAGE;
				}

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

	if (treeNode != nullptr)
	{
		uint16_t baseClassNodeId = *reinterpret_cast<uint16_t*>(m_valueData + treeNode->offsetValue + 1);

		size_t nodeCount = m_v.size();

		assert(baseClassNodeId < nodeCount);

		baseClassNode = GetNodeById(baseClassNodeId);
	}

	return baseClassNode;
}


