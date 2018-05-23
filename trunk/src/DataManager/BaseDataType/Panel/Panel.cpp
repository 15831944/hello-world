/*--------------------------------------------------------------------------------------------------------------------*/
//	Panel.cpp      -- �����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Panel.h"
#include <algorithm>


#include "../CommonData/CommonData.h"
#include "../../Algorithm/Algorithm.h"
#include "../CSingleton/CSingleon.h"

using namespace std;

// ����
Panel::Panel(void)
{
	m_PanelType = PanelType_RawMaterial;
	m_OrgLen = 0;  
	m_OrgWidth = 0;  

	m_OriginX = 0;  
	m_OriginY = 0;   
	m_type = NodeType_Remainder;		// ��ʼ��Ϊ����
}

// ����
Panel::Panel(Panel& panel)
{
	// panel
	m_PanelType = panel.m_PanelType ;
	m_OrgLen	= panel.m_OrgLen	;
	m_OrgWidth	= panel.m_OrgWidth	;
	m_OriginX	= panel.m_OriginX	;
	m_OriginY	= panel.m_OriginY	;


	// component
	m_type		= NodeType_Remainder;
	m_RealLength = panel.m_RealLength;
	m_RealWidth = panel.m_RealWidth;
	m_Thickness = panel.m_Thickness;
	m_Material = panel.m_Material;
	
	m_PanelID =  panel.m_PanelID;
	m_x =  panel.m_x;
	m_y =  panel.m_y;


}


// ����
Panel::~Panel(void)
{

}

// ��ȡ���������
float Panel::GetUtilization(void)
{
	float utilization;
	float panel_area = 0, cpn_area = 0, remain_area = 0;
	vector<Node*> RemainderList;

	panel_area = GetArea();

	//GetAllLeafNodes(RemainderList, NodeType_Remainder);

// 	for (vector<Node*>::iterator it = RemainderList.begin(); it != RemainderList.end(); it++)
// 	{
// 		Component* pRemainder = static_cast<Component*>(*it);
// 		remain_area += pRemainder->GetArea();
// 	}
// 
// 	utilization = 1.0f - remain_area/panel_area;


	GetAllLeafNodes(RemainderList, NodeType_NeededComponent);

	for (vector<Node*>::iterator it = RemainderList.begin(); it != RemainderList.end(); it++)
	{
		Component* pCpn = static_cast<Component*>(*it);
		cpn_area += pCpn->GetAreaContainKerf();
	}

	// �����ޱ�
	CSingleon* pSingleton = CSingleon::GetSingleton();
	float DeburringWidth = pSingleton->m_BaseInfo.m_DeburringWidth;
	float DeburringArea = m_OrgLen*DeburringWidth + (m_OrgWidth-DeburringWidth)*DeburringWidth;


	utilization = (cpn_area + DeburringArea) / panel_area;


	return utilization;
}

// ��ȡС������
int Panel::GetComponentNum(void)
{
	vector<Node*> ComponentList;
	GetAllLeafNodes(ComponentList, NodeType_NeededComponent);

	return ComponentList.size();
}

/*---------------------------------------*/
//	����˵����
//		��ȡС��
//
//
//	������
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
Component* Panel::GetComponent(int NumInPanel)
{
	int nCount;
	vector<Node*> node_list;
	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	nCount = node_list.size();
	for(int i = 0; i < nCount; i++)
	{
		Component* pCpn = static_cast<Component*>(node_list.at(i));

		if (pCpn->m_NumberInPanel == NumInPanel)
		{
			return pCpn;
		}
	}

	return NULL;
}



// �������ĵ��Զ��������
bool  ComponentCompareCenterFarer(const Component* pfirst, const Component* psecond) 
{                 
// 	Component* p1 = const_cast<Component*>(pfirst);
// 	Component* p2 = const_cast<Component*>(psecond);
// 
// 
// 	return p1->m_fDist2PanelCenter < p2->m_fDist2PanelCenter;	
	return true;
}


//  ��ȡ���е�С��
void Panel::GetAllNeededComponent_Sorted(vector<Component*>& cpn_list)
{
	// ��ʼ��������Ϣ
	CSingleon* pSingleton = CSingleon::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;

	vector<Node*> node_list;
	vector<Component*> small_cpn_list;
	BOOL meet_large_cpn = FALSE;

	// �ҵ�����С�壬����С��ĳ���
	int min_len, min_width;
	Component* pLargestCpn = GetLargestNeedComponent();

	// û��С��
	if (pLargestCpn == NULL)
	{
		return; 
	}

	min_len = pLargestCpn->m_RealLength < base_info.m_MinLength ?    pLargestCpn->m_RealLength :  base_info.m_MinLength;
	min_width = pLargestCpn->m_RealWidth < base_info.m_MinWidth ?    pLargestCpn->m_RealWidth :  base_info.m_MinWidth;




	// �����ռ��ӽڵ�
	GetAllLeafNodes(node_list, NodeType_NeededComponent);


	// ��ǰ������С�壬��С������ŵ������
	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));

		if (meet_large_cpn == FALSE &&
			(pNode->m_RealLength < min_len || pNode->m_RealWidth < min_width))
		{
			small_cpn_list.push_back(pNode);
		}
		else
		{
			meet_large_cpn = TRUE;
			cpn_list.push_back(pNode);
		}
	}

	
	cpn_list.insert(cpn_list.end(), small_cpn_list.rbegin(), small_cpn_list.rend());


	// ��С������������ĵ�Զ����������

// 	m_fCenterX = m_OrgLen/2.0;
// 	m_fCenterY = m_OrgWidth/2.0;
// 
// 	nCount = small_cpn_list.size();
// 	for (i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = small_cpn_list.at(i);
// 
// 		pNode->m_fCenterX = pNode->m_x + pNode->m_RealLength/2.0;
// 		pNode->m_fCenterY = pNode->m_y + pNode->m_RealWidth/2.0;
// 
// 		float dx = pNode->m_fCenterX - m_fCenterX;
// 		float dy = pNode->m_fCenterY - m_fCenterY;
// 
// 		pNode->m_fDist2PanelCenter = sqrt(dx*dx + dy*dy);
// 	}
// 
// 	sort(small_cpn_list.begin(), small_cpn_list.end(), ComponentCompareCenterFarer);


//	cpn_list.insert(cpn_list.end(), small_cpn_list.begin(), small_cpn_list.end());

}





/*---------------------------------------*/
//	����˵����
//		���������  ��ȡ���е�С��
//	
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void Panel::GetAllNeedComponent_AreaLargeFirst(vector<Component*>& cpn_list)
{

	vector<Component*> small_cpn_list;
	vector<Component*> node_list;

	ALGORITHM_API::TravelNeedComponent_AreaLargeFirst(this, node_list);

	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));

		cpn_list.push_back(pNode);
	}




	return;
}


// ��ȡ���е�����
// void Panel::GetAllRemainder(vector<Component*>& cpn_list)
// {
// 	vector<Node*> node_list;
// 
// 	GetAllLeafNodes(node_list, NodeType_Remainder);
// 
// 	int i, nCount = node_list.size();
// 	for (i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = static_cast<Component*>(node_list.at(i));
// 
// 		cpn_list.push_back(pNode);
// 	}
// }


// ����廹ԭΪ
void Panel::Recover(vector<Component*>& cpn_list)
{
	vector<Node*> node_list;

	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	// �ռ�С�壬�ж��������ϵ
	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));
		Node* pParent = pNode->m_pParent;
		
		pNode->m_pParent = NULL;
		cpn_list.push_back(pNode);

		//vector<Node*>::iterator it = find(pParent->m_ChildrenList.begin(), pParent->m_ChildrenList.end(), pNode);
		vector<Node*>::iterator it = pParent->m_ChildrenList.begin();
		pParent->m_ChildrenList.erase(it);

	}

	//�ͷ��ڴ�
	DeleteAllChildren(this);

	// ��ʼ��
	m_type = NodeType_Remainder;
	m_ChildrenList.clear();

}


/*---------------------------------------*/
//	����˵����
//		��С�廹ԭ
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void Panel::RecoverOneComponent(Component* pCpn)
{
	Node* pParent = pCpn->m_pParent;

	pCpn->m_pParent = NULL;

	//vector<Node*>::iterator it = find(pParent->m_ChildrenList.begin(), pParent->m_ChildrenList.end(), pCpn);
	vector<Node*>::iterator it = pParent->m_ChildrenList.begin();
	pParent->m_ChildrenList.erase(it);
}

/*---------------------------------------*/
//	����˵����
//		�Ƿ����һ�����
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool Panel::IsBetterThan(Panel* pDstPanel)
{
	if(pDstPanel != NULL)
	{
		Component* pCurLargestRemainder = this->GetLargestRemainder();
		Component* pDstLargestRemainder = pDstPanel->GetLargestRemainder();

		return pCurLargestRemainder->IsAreaLargeThan(*pDstLargestRemainder);
	}

	return false;
}


/*---------------------------------------*/
//	����˵����
//		�Ƿ��з�����Ϣ
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool  Panel::HaveDownerFaceInfo(void)
{
	vector<Node*> node_list;

	// ��ȡ����С��, �����ռ��ӽڵ�
	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	// �����ж��Ƿ��з�����Ϣ
	int nCount = node_list.size();

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = node_list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		if (pCpn->HaveDownerFaceInfo() == true)
		{
			return true;
		}
	}

	return false;
}
