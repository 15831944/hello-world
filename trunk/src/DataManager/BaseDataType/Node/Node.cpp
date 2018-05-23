/*--------------------------------------------------------------------------------------------------------------------*/
//	Node.cpp     -- �����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Node.h"



Node::Node(void)
{
	m_pParent = NULL;
}


Node::~Node(void)
{

}


// ����ӽڵ�
void Node::AddChild(Node* pNode)
{
	if (pNode != NULL)
	{
		m_ChildrenList.push_back(pNode);
		pNode->m_pParent = this;
	}
}

//��ȡ�����͵Ľڵ�
void Node::GetAllLeafNodes(vector<Node*>& list, int type)
{
	int i, nCount = m_ChildrenList.size();

	// �ȸ����� ��Ҷ�Ӽӵ㿪ʼ����
	if (m_type == type)
	{
		list.push_back(this);
	}


	for(i = 0; i < nCount; i++)
	{
		Node* pNode = m_ChildrenList.at(i);
		pNode->GetAllLeafNodes(list, type);

	}

	return;
}


// �����ڵ�
Node* Node::CopyNode(void)
{
	Node* pNode = new Node;

	return pNode;
}


// ����������ͷ���
void Node::DeleteTree(Node** ppRoot)
{
	if (ppRoot != NULL)
	{
		Node* pRoot = *ppRoot;
		if (pRoot != NULL)
		{
			int i, nCount = pRoot->m_ChildrenList.size();

			// �ݹ��ͷ��ӽڵ�
			for(i = 0; i < nCount; i++)
			{
				Node* pNode = pRoot->m_ChildrenList.at(i);
				DeleteTree(&pNode);
			}

			delete pRoot;
			*ppRoot = NULL;
		}
	}
}

// �ͷ����нڵ㣬���������ڵ�
void Node::DeleteAllChildren(Node* pRoot)
{
	int i, nCount;

	nCount = pRoot->m_ChildrenList.size();
	for (i = 0; i < nCount; i++)
	{
		Node* pNode = pRoot->m_ChildrenList.at(i);

		DeleteTree(&pNode);
	}

	pRoot->m_ChildrenList.clear();
}

