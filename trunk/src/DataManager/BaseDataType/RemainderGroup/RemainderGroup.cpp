/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderGroup.cpp      -- ��������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.9.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "RemainderGroup.h"



RemainderGroup::RemainderGroup(void)
{
	m_Thickness = 0;			
}


RemainderGroup::~RemainderGroup(void)
{
	// ��մ�������ͷŴ���ڴ�
	for (int j = 0; j < m_RemainderItemList.size(); j++)
	{
		RemainderItem* pItem =  m_RemainderItemList.at(j);
		delete pItem;
	}

	m_RemainderItemList.clear();
}


/*---------------------------------------*/
//	����˵����
//		�������
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
void RemainderGroup::AddRemainder(RemainderItem* pAddItem)
{
	m_RemainderItemList.push_back(pAddItem);

	UpdateRemainderItemID();

}

/*---------------------------------------*/
//	����˵����
//		������Ϣ
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
void RemainderGroup::UpdateRemainderItemID()
{
	int nCount = m_RemainderItemList.size();

	for (int i = 0; i < nCount; i++)
	{
		RemainderItem* pItem = m_RemainderItemList.at(i);

		pItem->m_GroupID = m_GroupID;
		pItem->m_ItemID = i;
	}
}

/*---------------------------------------*/
//	����˵����
//		ɾ��һ��������
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
void RemainderGroup::DelRemainder(int id)
{
	RemainderItem* pitem = m_RemainderItemList.at(id);
	delete pitem;

	m_RemainderItemList.erase(m_RemainderItemList.begin() + id);

	UpdateRemainderItemID();

}
