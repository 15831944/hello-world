/*--------------------------------------------------------------------------------------------------------------------*/
//	Node.h      -- ����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Node_H
#define		_Node_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Node
{
public:
	Node(void);
	virtual ~Node(void);



public:
	// �ӿ�
	virtual void AddChild(Node* pNode);									// ����ӽڵ�
	virtual void GetAllLeafNodes(vector<Node*>& list, int type);		// ��ȡ�����͵Ľڵ�
	virtual Node* CopyNode(void);										// �����ڵ�

	virtual void DeleteTree(Node** ppRoot);								// �ͷ���
	virtual void DeleteAllChildren(Node* pRoot);						// �ͷ����нڵ㣬���������ڵ�

public:
	int m_type;							// 0�����ϣ��ӽڵ㣩  1��Ҫ����С�壨�ӽڵ㣩   2����ϰ壨���ڵ㣩
	Node* m_pParent;					// ���ڵ�ָ��
	vector<Node*> m_ChildrenList;		// �ӽڵ�ָ������
	
};


#endif
