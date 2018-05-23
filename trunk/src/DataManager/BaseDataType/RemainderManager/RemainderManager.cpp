iterator
iterator
iterator
iterator
/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderManager.cpp      -- ���Ϲ�������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.9.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "RemainderManager.h"



RemainderManager::RemainderManager(void)
{			

}


RemainderManager::~RemainderManager(void)
{

}


/*---------------------------------------*/
//	����˵����
//		����������ID
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
void RemainderManager::UpdateRemainderGroupID()
{
	int nCount = m_RemainderGroupList.size();

	for (int i = 0; i < nCount; i++)
	{
		RemainderGroup* pGroup = m_RemainderGroupList.at(i);

		pGroup->m_GroupID = i;
		pGroup->UpdateRemainderItemID();

	}
}


/*---------------------------------------*/
//	����˵����
//		���������
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
void RemainderManager::AddRemainderGroup(RemainderGroup* pAddGroup)
{
	m_RemainderGroupList.push_back(pAddGroup);
	UpdateRemainderGroupID();
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
void RemainderManager::AddRemainderItem(RemainderItem* pAddItem)
{
	// ����
	if (pAddItem == NULL)
	{
		return;
	}

	RemainderItem* pItem = FindRemainder(pAddItem->m_Material, pAddItem->m_Thickness);

	if (pItem != NULL)
	{
		// �ҵ�������
		int group_id = pItem->m_GroupID;

		RemainderGroup* pGroup = m_RemainderGroupList.at(group_id);
		pGroup->AddRemainder(pAddItem);
	}
	else
	{
		//δ�ҵ����½�

		RemainderGroup* pNewGroup = new RemainderGroup;

		pNewGroup->m_Material = pAddItem->m_Material;
		pNewGroup->m_Thickness = pAddItem->m_Thickness;

		AddRemainderGroup(pNewGroup);
		pNewGroup->AddRemainder(pAddItem);

	}

	UpdateRemainderGroupID();

}

/*---------------------------------------*/
//	����˵����
//		����һ������
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
void RemainderManager::SubRemainderItem(RemainderItem* pDelItem)
{
	if (pDelItem->m_nCount > 1)	// �ж�
	{
		pDelItem->m_nCount--;
	}
	else	// ֻʣ��һ��
	{
		int group_id = pDelItem->m_GroupID;
		int item_id = pDelItem->m_ItemID;

		RemainderGroup* pGroup = m_RemainderGroupList.at(group_id);

		pGroup->DelRemainder(item_id);

		if (pGroup->m_RemainderItemList.size() == 0)
		{
			DelRemainderGroup(group_id);
		}
	}

	UpdateRemainderGroupID();
}


/*---------------------------------------*/
//	����˵����
//
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
void RemainderManager::DelRemainderGroup(int id)
{
	RemainderGroup* pGroup = m_RemainderGroupList.at(id);
	delete pGroup;

	m_RemainderGroupList.erase(m_RemainderGroupList.begin() + id);


	UpdateRemainderGroupID();
}


/*---------------------------------------*/
//	����˵����
//		Ѱ������
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
RemainderItem* RemainderManager::FindRemainder(CString Material, float Thickness)
{
	vector<RemainderGroup*>::iterator it_group;

	for (it_group = m_RemainderGroupList.begin(); it_group != m_RemainderGroupList.end(); it_group++)
	{
		RemainderGroup* pGroup = *it_group;

		if (pGroup->m_Material == Material && pGroup->m_Thickness == Thickness)
		{


			return pGroup->m_RemainderItemList.at(0);
		}
	}

	return NULL;

}



// ��ȡ
void RemainderManager::GetAllItems(vector<RemainderItem*>& item_list)
{
	for (vector<RemainderGroup*>::iterator it_group = m_RemainderGroupList.begin(); it_group != m_RemainderGroupList.end(); it_group++)
	{
		RemainderGroup* pGroup = *it_group;

		for (vector<RemainderItem*>::iterator it_item = pGroup->m_RemainderItemList.begin(); it_item != pGroup->m_RemainderItemList.end(); it_item++)
		{
			item_list.push_back(*it_item);
		}
	}
}