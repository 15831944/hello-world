/*--------------------------------------------------------------------------------------------------------------------*
///	CSolution.cpp   --  ���������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.8
//	��ע��
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CSolution.h"

#include <time.h>   
#include <algorithm>
#include "xfunctional"

#include "../Panel/Panel.h"
#include "../BaseInfo/BaseInfo.h"
#include "../Component/Component.h"
#include "../RemainderManager/RemainderManager.h"
#include "../RemainderGroup/RemainderGroup.h"
#include "../RemainderItem/RemainderItem.h"
#include "../../Algorithm/Algorithm.h"





// ����
CSolution::CSolution()
{
	m_SlnID		= 0;					
	//m_fPanelLength	= 0.0f;							
	//m_fPanelWidth	= 0.0f;							
	m_fThickness	= 0.0f;								
	m_strMaterial	= "";				
}

// ����
CSolution::~CSolution()
{
	// ��մ�������ͷŴ���ڴ�
	for (int j = 0; j < m_PanelList.size(); j++)
	{
		Node* pPanel =  m_PanelList.at(j);
		pPanel->DeleteTree(&pPanel);
	}

	m_PanelList.clear();

}


// ����
void QuickSortComponent(vector<Component*>& List, int l, int r)  
{  
	if (l< r)  
	{        
		int i = l, j = r;
		Component* x = List[l];  
		while (i < j)  
		{  
			while(i < j && List[j]>= x) // ���������ҵ�һ��С��x����  
				j--;   
			if(i < j)  
				List[i++] = List[j];  
			while(i < j && List[i]< x) // ���������ҵ�һ�����ڵ���x����  
				i++;   
			if(i < j)  
				List[j--] = List[i];  
		}  
		List[i] = x;  
		QuickSortComponent(List, l, i - 1); // �ݹ����  
		QuickSortComponent(List, i + 1, r);  
	}  
}  

// ��С��������
bool ComponentCompareLess(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) > (*p2);	
}

// ��С��������
bool ComponentCompareGreater(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) < (*p2);	
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��С���������򣬳������ȣ������֮
//	
//	param:
//		void
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
// int CSolution::SortRemainderList_LengthFirst(void)
// {
// 	//  ����Ӵ�С����
// 	sort(m_RemainderList.begin(), m_RemainderList.end(), ComponentCompareGreater);
// 
// 	return 0;
// }


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		������򣬳������ȣ������֮
//	
//	param:
//		void
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int CSolution::SortComponentList_LengthFirst(void)
{
	//  ����Ӵ�С����
	sort(m_ComponentList.begin(), m_ComponentList.end(), ComponentCompareLess);

	return 0;
}






// ������򣬳������ȣ������֮
int CSolution::SortComponentList_Random(void)
{
	// �������;
	vector<Component*> new_list;

	srand((unsigned)time(0)); 

	// ��ԭʼ��������ѡȡС�壬���뵽�������У�
	while(m_ComponentList.size() > 0)
	{
		int nCount = m_ComponentList.size();
		int sel_id = rand() % nCount;

		Component* pCpn = m_ComponentList.at(sel_id);
		new_list.push_back(pCpn);
		m_ComponentList.erase(m_ComponentList.begin() + sel_id);
	}

	// ���¸�ֵ
	m_ComponentList = new_list;
	return 0;
}




// �������С�Ƿ񳬳�
void CSolution::CheckComponentList(void)
{
	vector<Component*>::iterator it;
	CString strMsg;

	for (it = m_ComponentList.begin(); it != m_ComponentList.end();)
	{
		Component* pCpn = *it;

		bool bOverSize = false;
		float fOrgXLen, fOrgYLen;
		if(pCpn->m_nRotatedAngle == 0)
		{
			fOrgXLen = pCpn->m_RealLength;
			fOrgYLen = pCpn->m_RealWidth;
		}
		else
		{
			fOrgXLen = pCpn->m_RealWidth;
			fOrgYLen = pCpn->m_RealLength;
		}

		if (pCpn->IsRotatable())
		{
			if(fOrgXLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// ��ת���ٴ��ж�
				if (fOrgXLen >  m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth 
					|| fOrgYLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth
					|| fOrgXLen <= 0
					|| fOrgYLen <= 0)
				{
					// ���ǳ�����ɾ��
					bOverSize = true;
				}
			}
		}
		else if(pCpn->m_Texture == 1)
		{
			if(fOrgXLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}
		else
		{
			if(fOrgXLen >  m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// ����
			strMsg += "ɾ��������Χ���������ţ�" + pCpn->m_BarCode + "\n";

			// ɾ��
			it = m_ComponentList.erase(it);
			delete pCpn;
			pCpn = NULL;
		}
		else
		{
			it++;
		}
	}


	// ����Ϣ
	if (strMsg.IsEmpty() != true)
	{
		AfxMessageBox(strMsg);
	}


}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		ƥ����ʵ�С��������ߡ�
//		̰���㷨��˼�룺ÿ�ε������������ʵ�һ��������
//		С��Ӵ�С������������ߴ�խ��������
//		˳��ƥ�䣬ÿ��ƥ��ɹ����ض������İ�����խ�������ߣ�ʹ��ÿ���µ���������խ
//	
//		��һ��ƥ�䣬�鿴�ܷ����
//		�ڶ���ƥ�䣺��ת�ٿ��Ƿ��ܰ���
//		������ƥ�䣺����������Ѿ���Ч����պ�����������������
//		
//	
//	param:
//		int &nCpnID			--	С��ID
//		int &nOutlineID		--	������ID
//
//	return:
//		true -- ��ƥ�䵽
//		false-- �޷�ƥ��
/*--------------------------------------------------------------------------------------*/
bool CSolution::MatchSuitableComponentNOutline(int &nCpnID, int &nOutlineID)
{

match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = m_ComponentList.size();
	int i_outline, nOutlineSize = m_LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = m_ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = m_LowestOutlineList.at(i_outline);

			if (line.Containable(pCpn) == true)
			{
				bMatchFlag = true;
				nCpnID = i_cpn;
				nOutlineID = i_outline;
				break;
			}
		}

		// ���ҵ�
		if (bMatchFlag == true)
		{
			break;
		}

	}

	// ������һ�λ�δ�ҵ�ƥ���С��������ߣ�������ת����תһ�£��ٱ���һ��
	if(bMatchFlag == false)
	{
		for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
		{
			Component* pCpn = m_ComponentList.at(i_cpn);

			// ��ת
			if(pCpn->IsRotatable() == true)
			{
				pCpn->ClockwiseRotate90();
			}

			// �ٴ�ƥ��
			for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
			{
				Outline& line = m_LowestOutlineList.at(i_outline);

				if (line.Containable(pCpn) == true)
				{
					bMatchFlag = true;
					nCpnID = i_cpn;
					nOutlineID = i_outline;
					break;
				}
			}

			// ���ҵ�
			if (bMatchFlag == true)
			{


				break;
			}
			else
			{
				// ������Ҫ����ת��İ����ԭ����δ����
				if(pCpn->IsRotatable() == true)
				{
					pCpn->CouterClockwiseRotate90();
				}
			}
		}
	}

	// ��ʱ��δ�ҵ�ƥ�����������ߣ�˵������������Ѿ��޷�ʹ�ã������������������գ�����һ��������
	if (bMatchFlag == false)
	{
		// ����������������
		m_LowestOutlineList.clear();

		// ���滹�������ߣ��������ң�û����ֱ�ӷ���false
		if (m_OutlineList.size() > 0)
		{
			// ѡ�����������
			int i, erase_id = 0;
			Outline first_line = m_OutlineList.at(0);
			int nCount = m_OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = m_OutlineList.at(i);

				if (ln.m_start_y == first_line.m_start_y)
				{
					erase_id = i;
				}
				else
				{
					break;
				}
			}

			// �����������Ԫ�ز���������������������������������Ƴ�
			for (i = 0; i <= erase_id; i++)
			{
				Outline ln = m_OutlineList.at(0);
				m_LowestOutlineList.push_back(ln);
				m_OutlineList.erase(m_OutlineList.begin());
			}

			// ���²���
			goto match_loop;
		}
	}



	return bMatchFlag;
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		ƥ����ʵ�С���������2����1����һ��
//	
//		��һ��ƥ�䣬�鿴�ܷ���������ܰ�������ת�ٿ��Ƿ��ܰ���
//		�ڶ���ƥ�䣺������������Ѿ���Ч����պ�����������������
//		
//	
//	param:
//		int &nCpnID			--	С��ID
//		int &nOutlineID		--	������ID
//
//	return:
//		true -- ��ƥ�䵽
//		false-- �޷�ƥ��
/*--------------------------------------------------------------------------------------*/
bool CSolution::MatchSuitableComponentNOutline2(int &nCpnID, int &nOutlineID)
{
match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = m_ComponentList.size();
	int i_outline, nOutlineSize = m_LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = m_ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = m_LowestOutlineList.at(i_outline);

			if (line.Containable(pCpn) == true)
			{
				bMatchFlag = true;
				nCpnID = i_cpn;
				nOutlineID = i_outline;
				break;
			}
			else
			{
				if (pCpn->IsRotatable() == true)
				{
					pCpn->ClockwiseRotate90();
					if (line.Containable(pCpn) == true)
					{
						bMatchFlag = true;
						nCpnID = i_cpn;
						nOutlineID = i_outline;
						break;
					}
					else
					{
						pCpn->CouterClockwiseRotate90();
					}
				}
			}
		}

		// ���ҵ�
		if (bMatchFlag == true)
		{
			break;
		}

	}

	
	// ��ʱ��δ�ҵ�ƥ�����������ߣ�˵������������Ѿ��޷�ʹ�ã������������������գ�����һ��������
	if (bMatchFlag == false)
	{
		// ����������������
		m_LowestOutlineList.clear();

		// ���滹�������ߣ��������ң�û����ֱ�ӷ���false
		if (m_OutlineList.size() > 0)
		{
			// ѡ�����������
			int i, erase_id = 0;
			Outline first_line = m_OutlineList.at(0);
			int nCount = m_OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = m_OutlineList.at(i);

				if (ln.m_start_y == first_line.m_start_y)
				{
					erase_id = i;
				}
				else
				{
					break;
				}
			}

			// �����������Ԫ�ز���������������������������������Ƴ�
			for (i = 0; i <= erase_id; i++)
			{
				Outline ln = m_OutlineList.at(0);
				m_LowestOutlineList.push_back(ln);
				m_OutlineList.erase(m_OutlineList.begin());
			}

			// ���²���
			goto match_loop;
		}
	}



	return bMatchFlag;
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		ƥ����ʵ�С��������ߡ�
//		̰���㷨��˼�룺ÿ�ε����ϳ��Ȼ��߿�ȱ������õ�����˷���С��ѡ��һ����ȫƥ���
//						���Ȼ��߿�Ⱦ��˳�������ƥ����س��ȷ����и���ƥ����ؿ�ȷ���
//						�и�
//		
//		
//	
//	param:
//		int &nCpnID			--	С��ID
//		int &nOutlineID		--	������ID
//
//	return:
//		true -- ��ƥ�䵽
//		false-- �޷�ƥ��
/*--------------------------------------------------------------------------------------*/

#if 0
bool CSolution::MatchSuitableComponentNRemaider(Panel* pPanel, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder)
{
	int i_remainder, nRemainderNum = m_RemainderList.size();
	int i_cpn, nLeftCpnSize = m_ComponentList.size();

	bool bMatchFlag = false;
	bool bRotateFlag = false;
	float min_remain_length, min_remain_width, min_dist;

	for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
	{
		Component* pRmd = m_RemainderList.at(i_remainder);

		for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
		{
			Component* pCpn = m_ComponentList.at(i_cpn);

			if (pRmd->Containable(pCpn) == true)
			{
				if(bMatchFlag == false)
				{
					//��һ��ѡ��
					min_remain_length = pRmd->m_RealLength - pCpn->m_RealLength;
					min_remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

					min_dist = min(min_remain_length, min_remain_width);

					pPerfectMatchComponent = pCpn;
					pPerfectMatchRemainder = pRmd;
					bRotateFlag = false;
					rotate_flag = bRotateFlag;
					bMatchFlag = true;


					// ��ȫƥ�䣬����
					if (min_remain_length == 0.0)
					{
						cut_dir = CutDir_Horizon;

						return true;
					}
					else if (min_remain_width == 0.0)
					{
						cut_dir = CutDir_Vertical;

						return true;
					}
					else
					{
						if (min_remain_width < min_remain_length)
						{
							cut_dir = CutDir_Vertical;
						}
						else
						{
							cut_dir = CutDir_Horizon;
						}
					}

				}
				else
				{
					float remaim_length = pRmd->m_RealLength - pCpn->m_RealLength;
					float remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

					// ������ƥ���
					if (remaim_length < min_dist || remain_width < min_dist)
					{
						min_remain_length = remaim_length;
						min_remain_width = remain_width;

						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = false;
						rotate_flag = bRotateFlag;

						// ��ȫƥ�䣬����
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}
					}
				}
			}

			if (pCpn->IsRotatable() == true)	// ƥ��ʱ��Ϊ���ټ�����������ʧ�棬��������ת��ֻ�Ƚ���ֵ������ʱ��������ת
			{
				Component tmp_cpn;
				tmp_cpn.m_RealLength = pCpn->m_RealWidth;
				tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

				if (pRmd->Containable(&tmp_cpn) == true)
				{
					if(bMatchFlag == false)
					{
						//��һ��ѡ��
						min_remain_length	= pRmd->m_RealLength - pCpn->m_RealWidth;
						min_remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;


						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = true;
						bMatchFlag = true;
						rotate_flag = bRotateFlag;


						// ��ȫƥ�䣬����
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}
					}
					else
					{
						float remaim_length = pRmd->m_RealLength - pCpn->m_RealWidth;
						float remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;

						// ������ƥ���
						if (remaim_length < min_dist || remain_width < min_dist)
						{
							min_remain_length = remaim_length;
							min_remain_width = remain_width;

							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = true;
							rotate_flag = bRotateFlag;

							// ��ȫƥ�䣬����
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
					}
				}
			}
		}
	}


	return bMatchFlag;

}

#endif


// ��ȡ�������
int CSolution::GetPanelNum(void)
{
	return m_PanelList.size();
}

// ��ȡ���

Panel* CSolution::GetPanel(int panel_num)
{
	int nCount = GetPanelNum();

	if (panel_num > nCount-1)
		return NULL;
	
	return m_PanelList[panel_num];
}







// ��С��������
bool CompareOutlineGreater(const Outline& first, const Outline& second) 
{              
	return first < second;	
}

// ��С��������
bool CompareOutlineLess(const Outline& first, const Outline& second) 
{              
	return first > second;	
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		�����Ż�	����ʣ����С������������������
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int CSolution::LayoutOptimization_MinWaste(int CutStyle, int Org)
{
	bool bUseRmd = false;
	Panel* pPanel;

	srand((unsigned)time(0)); 

	// ���С���Ƿ񳬳���Χ
	CheckComponentList();

	// ���� �������� �����֮
	SortComponentList_LengthFirst();

	if (m_ComponentList.size() > 0)
	{
		// �½����
		pPanel = new Panel;

		// �ж��������Ͽ����ã��еĻ�ֱ�ӵ��ã�û�����½����
		CSingleon* pSingelton = CSingleon::GetSingleton();
		RemainderManager& RmdManager = pSingelton->m_RemainderManager;
		RemainderItem* pItem = RmdManager.FindRemainder(m_strMaterial, m_fThickness);

		if (pItem != NULL)	// �����Ͽ���
		{
			// Panel����Ϣ
			pPanel->m_PanelType		= PanelType_Remainder;			// ���ϴ��
			pPanel->m_OrgLen		= pItem->m_Length;				// ���ô��ԭʼ��
			pPanel->m_OrgWidth		= pItem->m_Width;				// ���ô��ԭʼ��

			bUseRmd = true;

			RmdManager.SubRemainderItem(pItem);
		}
		else
		{
			// Panel����Ϣ
			pPanel->m_PanelType		= PanelType_RawMaterial;		// ԭ�ϴ��
			pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;		// ���ô��ԭʼ��
			pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;		// ���ô��ԭʼ��

			bUseRmd = false;
		}

		// Panel����Ϣ
		pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;								// ����ԭ��x ���ԭʼ������½ǵ�����
		pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;								// ����ԭ��y ���ԭʼ������½ǵ�����

		// Component����Ϣ
		pPanel->m_x				= m_BaseInfo.m_DeburringWidth;								// ����ԭ��x ���ԭʼ������½ǵ�����
		pPanel->m_y				= m_BaseInfo.m_DeburringWidth;								// ����ԭ��y ���ԭʼ������½ǵ�����
		pPanel->m_RealLength	= pPanel->m_OrgLen		- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
		pPanel->m_RealWidth		= pPanel->m_OrgWidth	- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
		pPanel->m_Material		= m_strMaterial;											// ���ô�����
		pPanel->m_Thickness		= m_fThickness;												// ���ô����
		pPanel->m_type			= NodeType_Remainder;										// ���ô��ڵ�����

		m_PanelList.push_back(pPanel);
	}

	// ��ʼ���� ��С����Ҫ�ţ�
	while (m_ComponentList.size() > 0)
	{
		// �Ű��Ż�
		ALGORITHM_API::LayoutOnePanel_Greedy(pPanel, m_BaseInfo, m_ComponentList, CutStyle, Org);

#if 1
		// �԰�����
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent_Sorted(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}

		// ��������ȴ�Ų����壬ɾ�������
		if (bUseRmd == true && nCpnCount == 0)
		{
			Node* pnode = pPanel;
			pPanel->DeleteTree(&pnode);
			pPanel = NULL;

			m_PanelList.pop_back();
		}

#else

		// �԰�����
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}

#endif

		


	


		if (m_ComponentList.size() > 0)
		{
			// �½����
			pPanel = new Panel;

			// �ж��������Ͽ����ã��еĻ�ֱ�ӵ��ã�û�����½����
			CSingleon* pSingelton = CSingleon::GetSingleton();
			RemainderManager& RmdManager = pSingelton->m_RemainderManager;
			RemainderItem* pItem = RmdManager.FindRemainder(m_strMaterial, m_fThickness);

			if (pItem != NULL)	// �����Ͽ���
			{
				// Panel����Ϣ
				pPanel->m_PanelType		= PanelType_Remainder;			// ���ϴ��
				pPanel->m_OrgLen		= pItem->m_Length;				// ���ô��ԭʼ��
				pPanel->m_OrgWidth		= pItem->m_Width;				// ���ô��ԭʼ��

				bUseRmd = true;
				RmdManager.SubRemainderItem(pItem);

			}
			else
			{
				// Panel����Ϣ
				pPanel->m_PanelType		= PanelType_RawMaterial;		// ԭ�ϴ��
				pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;		// ���ô��ԭʼ��
				pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;		// ���ô��ԭʼ��

				bUseRmd = false;
			}

			// Panel����Ϣ
			pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;								// ����ԭ��x ���ԭʼ������½ǵ�����
			pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;								// ����ԭ��y ���ԭʼ������½ǵ�����

			// Component����Ϣ
			pPanel->m_x				= m_BaseInfo.m_DeburringWidth;								// ����ԭ��x ���ԭʼ������½ǵ�����
			pPanel->m_y				= m_BaseInfo.m_DeburringWidth;								// ����ԭ��y ���ԭʼ������½ǵ�����
			pPanel->m_RealLength	= pPanel->m_OrgLen		- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
			pPanel->m_RealWidth		= pPanel->m_OrgWidth	- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
			pPanel->m_Material		= m_strMaterial;											// ���ô�����
			pPanel->m_Thickness		= m_fThickness;												// ���ô����
			pPanel->m_type			= NodeType_Remainder;										// ���ô��ڵ�����

			m_PanelList.push_back(pPanel);
		}
	}

	return 0;
}








/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��ÿһ�������Ż�,
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
// int CSolution::LayoutOptimization(void)
// {
// 	Panel* pPanel;
// 
// 	srand((unsigned)time(0)); 
// 
// 
// 	// ���С���Ƿ񳬳���Χ
// 	CheckComponentList();
// 
// 	// ���� �������� �����֮
// 	SortComponentList_LengthFirst();
// 
// 	if (m_ComponentList.size() > 0)
// 	{
// 		// �½����
// 		pPanel = new Panel;
// 		pPanel->m_RealLength = m_fPanelLength;		// ���ô�峤
// 		pPanel->m_RealWidth = m_fPanelWidth;		// ���ô���
// 		pPanel->m_Material = m_strMaterial;			// ���ô�����
// 		pPanel->m_Thickness = m_fThickness;			// ���ô����
// 		pPanel->m_type = NodeType_Remainder;		// ���ô��ڵ�����
// 
// 		m_PanelList.push_back(pPanel);
// 
// 		// ��������������������
// 		BuildOutlineList(pPanel);
// 	}
// 
// 	// ��ʼ���� ��С����Ҫ�ţ�
// 	while (m_ComponentList.size() > 0)
// 	{
// 		// ѡ������ʵ�С���ŷ�
// 		bool bMatchable = false;
// 		int nCpnID, nOutlineID;
// 
// 		bMatchable = MatchSuitableComponentNOutline2(nCpnID, nOutlineID);
// 		if (bMatchable == true)
// 		{
// 			Component* pPlaceCpn = m_ComponentList.at(nCpnID);				// 
// 			Outline lowest_line = m_LowestOutlineList.at(nOutlineID);
// 			Component* pParentNode = lowest_line.m_pParent;
// 			
// 			int cut_dir = rand()%2;
// 
// 			if (cut_dir == CutDir_Vertical)
// 			{
// 				// ���ڵ�����һ�������ڵ�һ��Ϊ��
// 
// 				// ��ڵ�
// 				Component* pLeft	= new Component;
// 				pLeft->m_x			= pParentNode->m_x;
// 				pLeft->m_y			= pParentNode->m_y;
// 				pLeft->m_RealLength	= pPlaceCpn->m_RealLength;
// 				pLeft->m_RealWidth	= pParentNode->m_RealWidth;
// 				pLeft->m_Thickness	= pParentNode->m_Thickness;
// 				pLeft->m_Texture	= pParentNode->m_Texture;
// 				pLeft->m_type		= NodeType_Remainder;
// 
// 				// �ҽڵ�
// 				Component* pRight	= new Component;
// 				pRight->m_x			= pParentNode->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
// 				pRight->m_y			= pParentNode->m_y;
// 				pRight->m_RealLength= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
// 				pRight->m_RealWidth = pParentNode->m_RealWidth;
// 				pRight->m_Thickness = pParentNode->m_Thickness;
// 				pRight->m_Texture	= pParentNode->m_Texture;
// 				pRight->m_type		= NodeType_Remainder;
// 
// 				// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// ����һ������ڵ���һ��Ϊ������Ϊ��Ҫ��С�������
// 
// 				// ����С��,��С��������ɾ����С��
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// ɾ���������İ�������������
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// ��һ������
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x;			
// 				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С���� + ���
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;	// ���ڵ��� - С���� - ���
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// ���������ߣ������ҽڵ㣬��������������
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line1.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line1);
// 
// 				if (line2.IsLegal() == true)
// 					m_OutlineList.push_back(line2);
// 			}
// 			else
// 			{
// 				// ���ڵ����һ�������ڵ�һ��Ϊ��
// 
// 				// ��ڵ�
// 				Component* pRight		= new Component;
// 				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
// 				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;				// ���ڵ����½� + С����
// 				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
// 				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;		// ���ڵ��� - С���� - ���
// 				pRight->m_Thickness		= pParentNode->m_Thickness;
// 				pRight->m_Texture		= pParentNode->m_Texture;
// 				pRight->m_type			= NodeType_Remainder;
// 
// 				// �ҽڵ�
// 				Component* pLeft		= new Component;																
// 				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½�  + С�峤�� + ���
// 				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
// 				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
// 				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
// 				pLeft->m_Thickness		= pParentNode->m_Thickness;
// 				pLeft->m_Texture		= pParentNode->m_Texture;
// 				pLeft->m_type			= NodeType_Remainder;
// 
// 				// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// ����һ�����ҽڵ���һ��Ϊ������Ϊ��Ҫ��С�������
// 
// 				// ����С��,��С��������ɾ����С��
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// ɾ���������İ�������������
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// ��һ������
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С���� + ���
// 				pSecondRight->m_y			= pLeft->m_y;															// ���ڵ����½� 
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;	// ���ڵ��� - С���� - ���
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// ���������ߣ������ҽڵ㣬��������������
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line2.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line2);
// 
// 				if (line1.IsLegal() == true)
// 					m_OutlineList.push_back(line1);
// 			}
// 
// 
// 			// ����
// 			sort(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), CompareOutlineGreater);
// 			sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater);
// 
// 			int a = 0;
// 			//BuildOutlineList(pPanel);
// 		}
// 		else
// 		{
// 			// �½����
// 			pPanel = new Panel;
// 			pPanel->m_RealLength = m_fPanelLength;		// ���ô�峤
// 			pPanel->m_RealWidth = m_fPanelWidth;		// ���ô���
// 			pPanel->m_Material = m_strMaterial;			// ���ô�����
// 			pPanel->m_Thickness = m_fThickness;			// ���ô����
// 			pPanel->m_type = NodeType_Remainder;		// ���ô��ڵ�����
// 
// 
// 			m_PanelList.push_back(pPanel);
// 
// 			// ��������������������
// 			BuildOutlineList(pPanel);
// 		}
// 
// 	}
// 
// 
// 	// �����������������
// 
// 	// �������
// 
// 	return 0;
// }


	// �����Ż�
int CSolution::LayoutOptimization_RandomSortCut(int CutStyle, int Org)
{
	Panel* pPanel;

	srand((unsigned)time(0)); 


	// ���С���Ƿ񳬳���Χ
	CheckComponentList();

	// ���� �������� �����֮
	SortComponentList_Random();

	if (m_ComponentList.size() > 0)
	{
		// �½����
		pPanel = new Panel;

		// Panel����Ϣ
		pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;										// ���ô��ԭʼ��
		pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;										// ���ô��ԭʼ��
		pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;							// ����ԭ��x ���ԭʼ������½ǵ�����
		pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;							// ����ԭ��y ���ԭʼ������½ǵ�����

		// Component����Ϣ
		pPanel->m_x				= m_BaseInfo.m_DeburringWidth;							// ����ԭ��x ���ԭʼ������½ǵ�����
		pPanel->m_y				= m_BaseInfo.m_DeburringWidth;							// ����ԭ��y ���ԭʼ������½ǵ�����
		pPanel->m_RealLength	= m_BaseInfo.m_PanelLength	- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
		pPanel->m_RealWidth		=  m_BaseInfo.m_PanelWidth		- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
		pPanel->m_Material		= m_strMaterial;										// ���ô�����
		pPanel->m_Thickness		= m_fThickness;											// ���ô����
		pPanel->m_type			= NodeType_Remainder;									// ���ô��ڵ�����

		m_PanelList.push_back(pPanel);

		// ��������������������
		//BuildOutlineList(pPanel);
	}


	// ��ʼ���� ��С����Ҫ�ţ�
	while (m_ComponentList.size() > 0)
	{
		ALGORITHM_API::LayoutOnePanel_LowestOutline(pPanel, m_BaseInfo, m_ComponentList, CutStyle, Org);

		// �԰�����
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}


		if (m_ComponentList.size() > 0)
		{
			// �½����
			pPanel = new Panel;

			// Panel����Ϣ
			pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;										// ���ô��ԭʼ��
			pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;										// ���ô��ԭʼ��
			pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;							// ����ԭ��x ���ԭʼ������½ǵ�����
			pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;							// ����ԭ��y ���ԭʼ������½ǵ�����

			// Component����Ϣ
			pPanel->m_x				= m_BaseInfo.m_DeburringWidth;							// ����ԭ��x ���ԭʼ������½ǵ�����
			pPanel->m_y				= m_BaseInfo.m_DeburringWidth;							// ����ԭ��y ���ԭʼ������½ǵ�����
			pPanel->m_RealLength	= m_BaseInfo.m_PanelLength	- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
			pPanel->m_RealWidth		=  m_BaseInfo.m_PanelWidth		- 2*m_BaseInfo.m_DeburringWidth;	// ���ô����ʵ�� ���ȥ�����ޱ�
			pPanel->m_Material		= m_strMaterial;										// ���ô�����
			pPanel->m_Thickness		= m_fThickness;											// ���ô����
			pPanel->m_type			= NodeType_Remainder;									// ���ô��ڵ�����

			m_PanelList.push_back(pPanel);
		}
	}







	// ��ʼ���� ��С����Ҫ�ţ�
// 	while (m_ComponentList.size() > 0)
// 	{
// 		// ѡ������ʵ�С���ŷ�
// 		bool bMatchable = false;
// 		int nCpnID, nOutlineID;
// 
// 		bMatchable = MatchSuitableComponentNOutline2(nCpnID, nOutlineID);
// 		if (bMatchable == true)
// 		{
// 			Component* pPlaceCpn = m_ComponentList.at(nCpnID);				// 
// 			Outline lowest_line = m_LowestOutlineList.at(nOutlineID);
// 			Component* pParentNode = lowest_line.m_pParent;
// 
// 			int cut_dir = rand()%2;
// 
// 			if (cut_dir == 1)
// 			{
// 				// ���ڵ�����һ�������ڵ�һ��Ϊ��
// 
// 				// ��ڵ�
// 				Component* pLeft	= new Component;
// 				pLeft->m_x			= pParentNode->m_x;
// 				pLeft->m_y			= pParentNode->m_y;
// 				pLeft->m_RealLength	= pPlaceCpn->m_RealLength;
// 				pLeft->m_RealWidth	= pParentNode->m_RealWidth;
// 				pLeft->m_Thickness	= pParentNode->m_Thickness;
// 				pLeft->m_Texture	= pParentNode->m_Texture;
// 				pLeft->m_type		= NodeType_Remainder;
// 
// 				// �ҽڵ�
// 				Component* pRight	= new Component;
// 				pRight->m_x			= pParentNode->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
// 				pRight->m_y			= pParentNode->m_y;
// 				pRight->m_RealLength= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
// 				pRight->m_RealWidth = pParentNode->m_RealWidth;
// 				pRight->m_Thickness = pParentNode->m_Thickness;
// 				pRight->m_Texture	= pParentNode->m_Texture;
// 				pRight->m_type		= NodeType_Remainder;
// 
// 				// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// ����һ������ڵ���һ��Ϊ������Ϊ��Ҫ��С�������
// 
// 				// ����С��,��С��������ɾ����С��
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// ɾ���������İ�������������
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// ��һ������
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x;			
// 				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С���� + ���
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;	// ���ڵ��� - С���� - ���
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// ���������ߣ������ҽڵ㣬��������������
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line1.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line1);
// 
// 				if (line2.IsLegal() == true)
// 					m_OutlineList.push_back(line2);
// 			}
// 			else
// 			{
// 				// ���ڵ����һ�������ڵ�һ��Ϊ��
// 
// 				// ��ڵ�
// 				Component* pRight		= new Component;
// 				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
// 				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;				// ���ڵ����½� + С����
// 				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
// 				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;		// ���ڵ��� - С���� - ���
// 				pRight->m_Thickness		= pParentNode->m_Thickness;
// 				pRight->m_Texture		= pParentNode->m_Texture;
// 				pRight->m_type			= NodeType_Remainder;
// 
// 				// �ҽڵ�
// 				Component* pLeft		= new Component;																
// 				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½�  + С�峤�� + ���
// 				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
// 				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
// 				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
// 				pLeft->m_Thickness		= pParentNode->m_Thickness;
// 				pLeft->m_Texture		= pParentNode->m_Texture;
// 				pLeft->m_type			= NodeType_Remainder;
// 
// 				// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// ����һ�����ҽڵ���һ��Ϊ������Ϊ��Ҫ��С�������
// 
// 				// ����С��,��С��������ɾ����С��
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// ɾ���������İ�������������
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// ��һ������
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// ���ڵ����½� + С���� + ���
// 				pSecondRight->m_y			= pLeft->m_y;															// ���ڵ����½� 
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;	// ���ڵ��� - С���� - ���
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// ���������ߣ������ҽڵ㣬��������������
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line2.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line2);
// 
// 				if (line1.IsLegal() == true)
// 					m_OutlineList.push_back(line1);
// 			}
// 
// 
// 			// ����
// 			sort(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), CompareOutlineGreater);
// 			sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater);
// 
// 			int a = 0;
// 			//BuildOutlineList(pPanel);
// 		}
// 		else
// 		{
// 			// �½����
// 			pPanel = new Panel;
// 			pPanel->m_RealLength = m_fPanelLength;		// ���ô�峤
// 			pPanel->m_RealWidth = m_fPanelWidth;		// ���ô���
// 			pPanel->m_Material = m_strMaterial;			// ���ô�����
// 			pPanel->m_Thickness = m_fThickness;			// ���ô����
// 			pPanel->m_type = NodeType_Remainder;		// ���ô��ڵ�����
// 
// 
// 			m_PanelList.push_back(pPanel);
// 
// 			// ��������������������
// 			BuildOutlineList(pPanel);
// 		}
// 
// 	}


	// �����������������

	// �������

	return 0;
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��ʼ������������
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int CSolution::BuildOutlineList(Panel* pParent)
{
	// �������������
	m_OutlineList.clear();
 	m_LowestOutlineList.clear();

	// ��ȡ���ϣ�����������
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	// ����������
	int i, nCount;
	nCount = list.size();

	// ������
	if(nCount == 0)
		return 0;

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		Outline line;
		float start_x = pCpn->m_x;
		float end_x = pCpn->m_x + pCpn->m_RealLength;
		float start_y = pCpn->m_y;
		float end_y = pCpn->m_y + pCpn->m_RealWidth;
		
		// ����������
		line.SetOutline(start_x, end_x, start_y, end_y, pCpn);
		m_OutlineList.push_back(line);
	}

	// ��С��������
	sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater); 

	// ѡ�����������
	int erase_id = 0;
	Outline first_line = m_OutlineList.at(0);
	nCount = m_OutlineList.size();

	for (i = 1; i < nCount; i++)
	{
		Outline ln = m_OutlineList.at(i);

		if (ln.m_start_y == first_line.m_start_y)
		{
			erase_id = i;
		}
		else
		{
			break;
		}
	}

	// �����������Ԫ�ز���������������������������������Ƴ�
	for (i = 0; i <= erase_id; i++)
	{
		Outline ln = m_OutlineList.at(0);
		m_LowestOutlineList.push_back(ln);
		m_OutlineList.erase(m_OutlineList.begin());
	}


	return 0;
}


// ��ʼ������������
// int CSolution::BuildRemainderList(Panel* pParent)
// {
// 	// �������������
// 	m_RemainderList.clear();
// 
// 	// ��ȡ���ϣ�����������
// 	vector<Node*> list;
// 	pParent->GetAllLeafNodes(list, NodeType_Remainder);	
// 
// 	int i, nCount;
// 	nCount = list.size();
// 
// 	// Node*תΪComponent*
// 	for(i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = static_cast<Component*>(list[i]);
// 		m_RemainderList.push_back(pNode);
// 	}
// 
// 	return 0;
// }


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��������ڲ������Ż�,˼·��
//			1���Ա��ݵ��Ż������е�ÿһ���������Ų����õ��������
//			2���Ӻ���Ĵ���в����Ƿ����㹻С�����ϣ������ŵ���ǰ�����
//			3���Դ����ƣ������һ���
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/

//#define  TEST_FLAG		2

void CSolution::ReOptimizeEveryPanel(int Org)
{
	vector<Panel*>::iterator it_panel_begin, it_panel_end;

	it_panel_begin = m_PanelList.begin();
	it_panel_end = m_PanelList.end();


	for (vector<Panel*>::iterator  it_panel = it_panel_begin; it_panel != it_panel_end; it_panel++)
	{
		bool bBetterFlag = false;
		Panel* pCurpanel = *it_panel;
		vector<Component*> CurCpnList, NewCpnList;

		// ����һ�ݴ�� һ��С�壬
		Panel* pNewPanel = new Panel(*pCurpanel);

		pCurpanel->GetAllNeededComponent(CurCpnList);
		ALGORITHM_API::CopyComponentList(CurCpnList, NewCpnList);

		/************************************************************************/
		/*                          ��ʼ����                                    */
		/************************************************************************/
	

//#if	(TEST_FLAG == CutDir_Horizon)

		// �µ�С������ -- ̰���㷨-ȫ������
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Horizon, Org) == 0)
		{
			// �µĴ��Ⱦɴ�����ϴ��ں����Ĵ������С�壬�嵽��ǰ����
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// ����
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}

		// �´��ָ���ԭʼ״̬
		pNewPanel->Recover(NewCpnList);

//#elif (TEST_FLAG == CutDir_Vertical)



		// �µ�С������ -- ̰���㷨-ȫ������

		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Vertical, Org) == 0)
		{
			// �µĴ��Ⱦɴ�����ϴ��ں����Ĵ������С�壬�嵽��ǰ����
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// ����
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}
		
		// �´��ָ���ԭʼ״̬
		pNewPanel->Recover(NewCpnList);

//#elif  (TEST_FLAG == CutDir_Default)



		// �µ�С������ -- ̰���㷨-ȫ��Ĭ����
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Default, Org) == 0)
		{
			// �µĴ��Ⱦɴ�����ϴ��ں����Ĵ������С�壬�嵽��ǰ����
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// ����
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}
		
		// �´��ָ���ԭʼ״̬
		pNewPanel->Recover(NewCpnList);

//#else

		vector<Component*> new_list, cur_list;

		// �µ�С������ -- ̰���㷨-ȫ�������
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Random, Org) == 0)
		{
			// �µĴ��Ⱦɴ�����ϴ��ں����Ĵ������С�壬�嵽��ǰ����
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// ����
				
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}

//#endif
		

		pCurpanel->GetAllNeededComponent(cur_list);
		pNewPanel->GetAllNeededComponent(new_list);

		// �ͷ����Ŵ��
		Node* pTmp = pNewPanel;
		pNewPanel->DeleteTree(&pTmp);
		pNewPanel = NULL;



		/************************************************************************/
		/*              �鿴����İ���û�п�����������                          */
		/************************************************************************/
		
		if (bBetterFlag == true)
		{
			vector<Panel*>::iterator it_left_panel;
			Component* pRmd = pCurpanel->GetLargestRemainder();

			for (it_left_panel = it_panel + 1; it_left_panel != it_panel_end; it_left_panel++)
			{
				Panel* pLeftPanel = *it_left_panel;

				vector<Component*> left_cpn_list;
				pLeftPanel->GetAllNeededComponent(left_cpn_list);

				for (int i_cpn = 0; i_cpn < left_cpn_list.size(); i_cpn++)
				{
					Component* pCpn = left_cpn_list.at(i_cpn);

					// ����������
					if (pRmd->Containable(pCpn) == true)
					{
						Panel::RecoverOneComponent(pCpn);
						ALGORITHM_API::KnifeOneRemainder(pRmd, pCpn, 0, m_BaseInfo.m_SawKerfWidth, Org);

						return;
					}
					else if (pCpn->IsRotatable() == true)
					{
						Component tmp_cpn;
						tmp_cpn.m_RealLength = pCpn->m_RealWidth;
						tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

						if (pRmd->Containable(&tmp_cpn) == true)
						{
							// ��ת������
							pCpn->ClockwiseRotate90();

							Panel::RecoverOneComponent(pCpn);
							ALGORITHM_API::KnifeOneRemainder(pRmd, pCpn, 0, m_BaseInfo.m_SawKerfWidth, Org);
							return;
						}

					}
				}
			}
		}
	}
}


// ����Ӵ�С����
bool DescendingUtilization( Panel* pfirst,  Panel* psecond) 
{                 
	float u1 = pfirst->GetUtilization();
	float u2 = psecond->GetUtilization();

	if (u1 > u2)
	{
		return true;
	}
	else
	{
		return false;
	}

}


/*---------------------------------------*/
//	����˵����
//		�Դ���б����Ż��ʽ�������
//		���ϰ����ȣ�ԭ�ϰ����ں���
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
void CSolution::SortPanelList(void)
{
	// ������б�һ��Ϊ��

	vector<Panel*> RemainderList, RawMaterialList;

	for (int i_panel = 0; i_panel < m_PanelList.size(); i_panel++)
	{
		Panel* pPanel = m_PanelList.at(i_panel);

		if (pPanel->m_PanelType == PanelType_RawMaterial)
		{
			RawMaterialList.push_back(pPanel);
		}
		else
		{
			RemainderList.push_back(pPanel);
		}

	}

	// ���ԭ�б�
	m_PanelList.clear();

	// ���ϱ�����
	sort(RemainderList.begin(), RemainderList.end(), DescendingUtilization);

	// ԭ�ϱ�����
	sort(RawMaterialList.begin(), RawMaterialList.end(), DescendingUtilization);

	// ����ԭ����
	m_PanelList.insert(m_PanelList.end(), RemainderList.begin(), RemainderList.end());
	m_PanelList.insert(m_PanelList.end(), RawMaterialList.begin(), RawMaterialList.end());

	

}