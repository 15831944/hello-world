#include "stdafx.h"
#include "Algorithm.h"
#include "../BaseDataType/CommonData/CommonData.h"
#include "../BaseDataType/Outline/Outline.h"
#include <algorithm>



// ������򣬳������ȣ������֮
int ALGORITHM_API::SortComponentList_Random(vector<Component*>& ComponentList)
{
	// �������;
	vector<Component*> new_list;

	srand((unsigned)time(0)); 

	// ��ԭʼ��������ѡȡС�壬���뵽�������У�
	while(ComponentList.size() > 0)
	{
		int nCount = ComponentList.size();
		int sel_id = rand() % nCount;

		Component* pCpn = ComponentList.at(sel_id);
		new_list.push_back(pCpn);
		ComponentList.erase(ComponentList.begin() + sel_id);
	}

	// ���¸�ֵ
	ComponentList = new_list;
	return 0;
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
bool ALGORITHM_API::MatchSuitableComponentNOutline2(vector<Component*>& ComponentList,vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList, int &nCpnID, int &nOutlineID)
{
match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = ComponentList.size();
	int i_outline, nOutlineSize = LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = LowestOutlineList.at(i_outline);

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
		LowestOutlineList.clear();

		// ���滹�������ߣ��������ң�û����ֱ�ӷ���false
		if (OutlineList.size() > 0)
		{
			// ѡ�����������
			int i, erase_id = 0;
			Outline first_line = OutlineList.at(0);
			int nCount = OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = OutlineList.at(i);

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
				Outline ln = OutlineList.at(0);
				LowestOutlineList.push_back(ln);
				OutlineList.erase(OutlineList.begin());
			}

			// ���²���
			goto match_loop;
		}
	}



	return bMatchFlag;
}

// ��С��������
bool ALGORITHM_API::CompareOutlineGreater(const Outline& first, const Outline& second) 
{              
	return first < second;	
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
int ALGORITHM_API::BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList)
{
	// �������������
	OutlineList.clear();
	LowestOutlineList.clear();

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
		OutlineList.push_back(line);
	}

	// ��С��������
	sort(OutlineList.begin(), OutlineList.end(), CompareOutlineGreater); 

	// ѡ�����������
	int erase_id = 0;
	Outline first_line = OutlineList.at(0);
	nCount = OutlineList.size();

	for (i = 1; i < nCount; i++)
	{
		Outline ln = OutlineList.at(i);

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
		Outline ln = OutlineList.at(0);
		LowestOutlineList.push_back(ln);
		OutlineList.erase(OutlineList.begin());
	}


	return 0;
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
int ALGORITHM_API::LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Outline> OutlineList, LowestOutlineList;
	
	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// ���� �������� �����֮
	SortComponentList_Random(SrcComponentList);

	BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);



	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;
		int nCpnID, nOutlineID;

		bMatchable = MatchSuitableComponentNOutline2(SrcComponentList, OutlineList, LowestOutlineList, nCpnID, nOutlineID);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = SrcComponentList.at(nCpnID);				
			Outline lowest_line = LowestOutlineList.at(nOutlineID);
			Component* pParentNode = lowest_line.m_pParent;

			// ���ID
// 			pPlaceCpn->m_CpnID = nCpnID;
// 			nCpnID++;

			int cut_dir;

			// ѡ�������иʽ
			switch(CutStyle)
			{
			case CutDir_Horizon:
				cut_dir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				cut_dir = CutDir_Vertical;
				break;
			case CutDir_Random:
				cut_dir = rand()%2;
				break;
			default:
				break;
			}

			// ����
			ALGORITHM_API::KnifeOneRemainder(pParentNode, pPlaceCpn, cut_dir, Info.m_SawKerfWidth, Org);

			// �ؽ����������
			BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);

			// ɾ���������İ�������������
			vector<Component*>::iterator it_cpn;
			it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPlaceCpn);
			SrcComponentList.erase(it_cpn);
		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������

	return 0;
}



/*---------------------------------------------------------*/
//	����˵����
//		�ڴ�����Ż�����
//
//	������
//		Panel * pSrcPanel					--	Ҫ�����Ĵ��
//		BaseInfo & base_info				--	��������Ļ�����Ϣ
//		vector<Component*> & SrcCpnList		--	С���б�
//		int cut_style						--	�иʽ
//
//	����ֵ:
//		0		--	���������,û��С��
//		1		--	���������,����С��
//		����	--	����
/*---------------------------------------------------------*/
int ALGORITHM_API::LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Component*> RemainderList;
	
	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// ������������
	BuildRemainderList(pSrcPanel, RemainderList);

	// ������÷�����Ϣ��������
	if (Info.m_bDownerFaceFirst == true)
	{
		vector<Component*> DownerInfoCpnList;
		vector<Component*>::iterator it;
		for (it = SrcComponentList.begin(); it != SrcComponentList.end();)
		{
			Component* pCpn = *it;

			if (pCpn->HaveDownerFaceHole() == true || pCpn->HaveDownerFaceSlot() == true)
			{
				// ��ԭ����ɾ�������뷴����Ϣ����
				it = SrcComponentList.erase(it);
				DownerInfoCpnList.push_back(pCpn);
			}
			else
			{
				it++;
			}
		}

		// ����ͷ��
		SrcComponentList.insert(SrcComponentList.begin(), DownerInfoCpnList.begin(), DownerInfoCpnList.end());
	}


	// ����С����Ҫ����
	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;					// ƥ��ɹ���־
		bool bRotateFlag = false;					// ��ת��־
		int nRecommendCutDir = CutDir_Horizon;		// �����иʽ
		Component* pPerfectMatchComponent = NULL, *pPerfectMatchRemainder = NULL;	// ���ƥ������Ϻ�С��

		bMatchable = MatchSuitableComponentNRemaider(RemainderList, SrcComponentList, pPerfectMatchComponent, bRotateFlag, nRecommendCutDir, pPerfectMatchRemainder, Info);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = pPerfectMatchComponent;		
			Component* pParentNode = pPerfectMatchRemainder;

			// �Ƿ���ת
			if (bRotateFlag == true)
			{
				pPlaceCpn->ClockwiseRotate90();
			}

			// ѡ�������иʽ
			switch(CutStyle)
			{
			case CutDir_Horizon:
				nRecommendCutDir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				nRecommendCutDir = CutDir_Vertical;
				break;
			case CutDir_Random:
				nRecommendCutDir = rand()%2;
				break;
			default:
				break;
			}

			// ����
			ALGORITHM_API::KnifeOneRemainder(pPerfectMatchRemainder, pPerfectMatchComponent, nRecommendCutDir, Info.m_SawKerfWidth, Org);

			// ɾ���������İ��������
			vector<Component*>::iterator it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPerfectMatchComponent);
			SrcComponentList.erase(it_cpn);

			vector<Component*>::iterator it_rmd = find(RemainderList.begin(), RemainderList.end(), pPerfectMatchRemainder);
			RemainderList.erase(it_rmd);

			// ���½�����������
			BuildRemainderList(pSrcPanel, RemainderList);

			// ��������
			SortRemainderList_LengthFirst(RemainderList);

		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������
}


/*---------------------------------------*/
//	����˵����
//		��һ�������и��һ����Ҫ��С�壬�����ʣ�࣬
//		��������0~2������
//		
//		ע����ڵ�Ϊ����С����������С������Ͻڵ㣬
//			�ҽڵ�Ϊ���Ͻڵ㣬�����귽���޹�
//				
//
//	������
//		Component* pParentNode	--	���и�İ��ָ�� 
//		Component* pPlaceCpn	--	��Ҫ��С��ָ��
//		int CutDir				--	�и��	0:����	1:����
//		float SawKerfWidth		--	��Ƭ���	
//		
//	����ֵ:
//
//
/*---------------------------------------*/
bool ALGORITHM_API::KnifeOneRemainder(Component* pParentNode, Component* pPlaceCpn, int CutDir, float SawKerfWidth, int Org)
{
	// �ж�Ҫ�и��С���Ƿ�Ϸ�
	if (pPlaceCpn->IsLegal() == false)
	{
		return false;
	}


	if (CutDir == CutDir_Vertical)
	{

		// ��һ��������һ�������ڵ�һ��Ϊ��
		Component* pLeft = new Component;
		Component* pRight = new Component;

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		default:						// Ĭ�����½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;
			pLeft->m_y				= pParentNode->m_y;
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
				
		}
		



		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}



		// �ڶ��� ����һ������ڵ���һ��Ϊ������Ϊ��Ҫ��С�������
		Component* pSecondRight = new Component;
		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// ���½�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ��y + С���� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
			case LayoutOrg_LeftTop:			// ���Ͻ�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;						
			case LayoutOrg_RightBottom:		// ���½�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;	// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y;	// ���ڵ��y
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y  + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ��y + С��Ŀ�� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;




				break;
			case LayoutOrg_RightTop:		// ���Ͻ�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;


				break;
			default:						// Ĭ�����½�

				// ����С��,��С��������ɾ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;			
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ����½� + С���� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
		}


		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;

		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}



	}
	else
	{
		// ��һ��������һ�������ڵ�һ��Ϊ��

		Component* pLeft = new Component;	
		Component* pRight = new Component;

		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// ���½�

				// ��ڵ�													
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			case LayoutOrg_LeftTop:			// ���Ͻ�

				// ��ڵ�																
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ�x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;				// ���ڵ�y + ���ڵ��� - С����
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ�x
				pRight->m_y				= pParentNode->m_y ;													// ���ڵ�y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightBottom:		// ���½�

				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightTop:		// ���Ͻ�
				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;														// ���ڵ�x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ�y + ���ڵ��� - С����
				pLeft->m_RealLength		= pParentNode->m_RealLength;											// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;												// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ�x
				pRight->m_y				= pParentNode->m_y ;													// ���ڵ�y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			default:						// Ĭ�����½�

				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;
		}


		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}
		// ����һ�����ҽڵ���һ��Ϊ������Ϊ��Ҫ��С�������

		Component* pSecondRight = new Component;
		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			
			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ�x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;	// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;													// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;		// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		default:						// Ĭ�����½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		}


		

		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;
		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}
	}

	return true;
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
int ALGORITHM_API::BuildRemainderList(Panel* pParent, vector<Component*>& RemainderList)
{
	// �������������
	RemainderList.clear();

	// ��ȡ���ϣ�����������
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	int i, nCount;
	nCount = list.size();

	// Node*תΪComponent*
	for(i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(list[i]);
		RemainderList.push_back(pNode);
	}

	return 0;
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

bool ALGORITHM_API::MatchSuitableComponentNRemaider(vector<Component*>& RemainderList, vector<Component*>& ComponentList, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder, BaseInfo& base_info)
{
	int i_remainder, nRemainderNum = RemainderList.size();
	int i_cpn, nLeftCpnSize = ComponentList.size();

	bool bMatchFlag = false;
	bool bRotateFlag = false;
	float min_remain_length, min_remain_width, min_dist;

	if (base_info.m_bDownerFaceFirst == true)
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

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

						// ������ƥ��� ���Ҳ�������з�����Ϣ�ҵ�ǰ����޷�����Ϣ
						if (	(remaim_length < min_dist || remain_width < min_dist) 
							&& !(pPerfectMatchComponent->HaveDownerFaceInfo() ==true && pCpn->HaveDownerFaceInfo() == false))
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

							// ������ƥ��� ���Ҳ�������з�����Ϣ�ҵ�ǰ����޷�����Ϣ
							if (	(remaim_length < min_dist || remain_width < min_dist) 
								&& !(pPerfectMatchComponent->HaveDownerFaceInfo() ==true && pCpn->HaveDownerFaceInfo() == false))
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
	}
	else
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

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
	}


	return bMatchFlag;

}


// �ж�˭�Ƚ�С
bool ALGORITHM_API::ComponentCompareSmaller(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) < (*p2);	
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		�Ӵ�С������򣬳������ȣ������֮
//	
//	param:
//		void
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::SortRemainderList_LengthFirst(vector<Component*>& RemainderList)
{
	//  ����Ӵ�С����
	sort(RemainderList.begin(), RemainderList.end(), ComponentCompareSmaller);

	return 0;
}







/*******************************************************************************************/

// ����С������
void ALGORITHM_API::CopyComponentList(vector<Component*>& SrcList, vector<Component*>& DstList)
{
	int i_cpn, nComponentCount;

	DstList.clear();
	nComponentCount = SrcList.size();
	for(i_cpn = 0; i_cpn < nComponentCount; i_cpn++)
	{
		Component* pCpn = SrcList.at(i_cpn);
		Component* pCopy = static_cast<Component*>(pCpn->CopyNode());


		DstList.push_back(pCopy);
	}
}

// �ж�˭�Ƚ�С
bool ComponentCompareArea(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return p1->IsAreaLargeThan(*p2);	
}


/*---------------------------------------*/
//	����˵����
//		��������ȱ������νṹ
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
void ALGORITHM_API::TravelNeedComponent_AreaLargeFirst(Component* pCpn, vector<Component*>& cpn_list)
{
	int i, nCount = pCpn->m_ChildrenList.size();

	// �ȸ����� ��Ҷ�Ӽӵ㿪ʼ����
	if (pCpn->m_type == NodeType_NeededComponent)
	{
		cpn_list.push_back(pCpn);
	}

	// �ռ��ӽڵ�
	vector<Component*> tmp_list;

	for(i = 0; i < nCount; i++)
	{
		Component* pTmpCpn = static_cast<Component*>(pCpn->m_ChildrenList.at(i));
		tmp_list.push_back(pTmpCpn);
	}

	// �����������
	sort(tmp_list.begin(), tmp_list.end(), ComponentCompareArea);


	for(i = 0; i < nCount; i++)
	{
		Component* pNode = tmp_list.at(i);
		TravelNeedComponent_AreaLargeFirst(pNode, cpn_list);
	}

	

	return ;
}

/*---------------------------------------------------*/
/*				��ѧ��ؼ���ӿ�					 */
/*---------------------------------------------------*/

/*---------------------------------------*/
//	����˵����
//		��ȡ��PointInfo��֮�����
//
//
//	������
//
//
//
//
//	����ֵ:
//		����
//
/*---------------------------------------*/
float ALGORITHM_API::GetPointsDistance(PointInfo p1, PointInfo p2)
{
	float dx, dy, dis_point;
	dx = p1.x - p2.x;
	dy = p1.y - p2.y;

	// ���������
	dis_point = sqrt(dx*dx + dy*dy);

	return dis_point;
}


#define  COINCIDENT_DIST		(0.2)		// �غϾ����ж�

// �����غϵĵ�
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints(vector<PointInfo>& oldpoint)
{
	// �����غϵ�
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// ѡ��
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// ȡֵ
		float cur_x, cur_y, next_x, next_y, dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// ���������
		float dis_point = sqrt(dx*dx + dy*dy);

		// ��ǰ������һ�����غϵ�
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  ���һ�����һ���غϣ������һ�㲻��Բ���յ㣬ɾ��
			if (i == old_point_size-1)
			{
				if (cur_point.sign == 2 || cur_point.sign == 3)	// Բ���յ㣬���
				{
					filtrationpoint.push_back(cur_point);
				}

				continue;
			}

			// ��Ҫ�ϲ���
			PointInfo new_one;
			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// Բ��β����Բ����㣬��Ϊ�м��
			{
				new_one = cur_point;
				new_one.sign = 3;	// Բ�����ӵ�
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// Բ����β���غϣ�ɾ�����㣬����ֱ�ߵ�
			{
				new_one = cur_point;
				new_one.sign = 0;
				new_one.r = 0;
			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// Բ��β���ֱ�ߣ�ɾ��ֱ�ߵ�
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// ֱ�߽�Բ�����
			{
				new_one = next_point;
			}
			else													// ֱ�߽�ֱ�ߣ�ֱ��ɾ�˵ڶ�����
			{
				new_one = cur_point;
			}

			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(new_one);
					i++;
				}

			}
			else
			{
				// ���غϣ����
				filtrationpoint.push_back(new_one);
				i++;
			}
		}
		else
		{
			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && cur_point.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(cur_point);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	// �����һ�����һ�����Ƿ�Ϊֱ�������һ�����غ�,���ε�����һ�����ڵ���3������ض��������ݴ���
	int nCount;
	float dist;
	PointInfo first_pnt, last_pnt;

	nCount = filtrationpoint.size();
	first_pnt = filtrationpoint.at(0);
	last_pnt = filtrationpoint.at(nCount-1);

	dist = ALGORITHM_API::GetPointsDistance(first_pnt, last_pnt);
	if (dist < COINCIDENT_DIST)
	{
		if (last_pnt.sign == 0)
		{
			filtrationpoint.pop_back();
		}
	}

	return filtrationpoint;
}


// �����غϵĵ�,��֤ͼ�αպ����һ������ֱ��ҲҪ��ӣ�
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints2(vector<PointInfo>& oldpoint)
{
	// �����غϵ�
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// ѡ��
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// ȡֵ
		float cur_x, cur_y, next_x, next_y, dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// ���������
		float dis_point = sqrt(dx*dx + dy*dy);

		// ��ǰ������һ�����غϵ�
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  ���һ��ֱ�����
			if (i == old_point_size-1)
			{
				// �ж��Ƿ����������ͬ����ĵ�
				int nNewCount = filtrationpoint.size();
				if (nNewCount > 0)
				{
					float d;
					PointInfo prev_new_one;

					prev_new_one = filtrationpoint.at(nNewCount-1);
					d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

					// �غ�
					if (d < COINCIDENT_DIST)
					{
						// �����, ����
						continue;
					}
					else
					{
						// ���غϣ����

						// ���һ����ΪԲ������ǣ�ֱ�Ӹ�Ϊֱ�ߵ�
						if (cur_point.sign == 1)
						{
							cur_point.sign = 0;
						}


						filtrationpoint.push_back(cur_point);
						continue;
					}
				}
				else
				{
					filtrationpoint.push_back(cur_point);
					continue;
				}
			}

			// ��Ҫ�ϲ���
			PointInfo new_one;
  			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// Բ��β����Բ����㣬��Ϊ�м��
			{
				new_one = cur_point;
				new_one.sign = 3;	// Բ�����ӵ�
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// Բ����β���غϣ�ɾ�����㣬����ֱ�ߵ�
			{
				new_one = cur_point;
				new_one.sign = 0; 
				new_one.r = 0;
			}
			else if (cur_point.sign == 3 && next_point.sign == 2)	// Բ�����ӵ����յ��غϣ�����β��
			{
				new_one = next_point;

				// ����β��뾶Ϊ0������������ӵ�İ뾶Ϊ׼
				if (cur_point.r > next_point.r)
				{
					new_one.r = cur_point.r;
				}

			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// Բ��β���ֱ�ߣ�ɾ��ֱ�ߵ�
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// ֱ�߽�Բ����� 
			{
				new_one = next_point;
			}
			else if (cur_point.sign == 1 && next_point.sign == 3)	// Բ����������ӵ��غϣ�ɾ�����ӵ�
			{
				new_one = cur_point;
			}
			else													// ֱ�߽�ֱ�ߣ�ֱ��ɾ�˵ڶ�����
			{
				new_one = cur_point;
			}



			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(new_one);
					//i++;
				}

			}
			else
			{
				// ���غϣ����
				filtrationpoint.push_back(new_one);
				//i++;
			}
		}
		else
		{
			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					// �����
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	return filtrationpoint;
}

/*---------------------------------------*/
//	����˵����
//			��ת��
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
void ALGORITHM_API::rotatepoint(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x - x0;
	float mid_y = y - y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x + x0;
	y = rotate_y + y0; 
}




/*---------------------------------------*/
//	����˵����
//		��ȡԲ��
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
void ALGORITHM_API::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
	float dis_x = (beginx - endx)/len_beginToend*radius;
	float dis_y = (beginy - endy)/len_beginToend*radius;

	float mid_x = endx + dis_x;
	float mid_y = endy + dis_y;

	float mid_x_move = mid_x - endx;
	float mid_y_move = mid_y - endy;

	float arc = (3.1415926-2*asin((0.5*len_beginToend)/radius))/2;

	center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
	center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);

	center_x = center_x + endx;
	center_y = center_y + endy;
}

/*---------------------------------------*/
//	����˵����
//		����ȡԲ��,�Ľ���getCircleCenter�Ĵ���
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
void ALGORITHM_API::GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float dx = beginx - endx;
	float dy = beginy - endy;
	float chord_len = sqrt(dx*dx + dy*dy);
	float sita = acos(0.5*chord_len/radius);	// �Ƕ�
	float new_x, new_y;

	// ˳ʱ����ת��ָ��Բ�ĵ������� ������180��ʱ��Ҫ��ʱ����ת
	if (sign == 1)
		ClockwiseRotate(dx, dy, new_x, new_y, sita);
	else
		CouterClockwiseRotate(dx, dy, new_x, new_y, sita);

	// �����Ĺ淶��
	float normal_new_x, normal_new_y;
	NormalVector(new_x, new_y, normal_new_x, normal_new_y);

	// β��ָ��Բ�ĵ�������׼ȷֵ
	float radius_x, radius_y;
	radius_x = normal_new_x * radius;
	radius_y = normal_new_y * radius;

	// β��+ָ��Բ�������� ���Բ�����ꡣ
	center_x = endx + radius_x;
	center_y = endy + radius_y;

}

/*---------------------------------------*/
//	����˵����
//		��ʱ����ת
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
void ALGORITHM_API::CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	����˵����
//		˳ʱ����ת
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
void ALGORITHM_API::ClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) + mid_y*sin(arc);
	float rotate_y = -mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	����˵����
//		�����淶��
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
void ALGORITHM_API::NormalVector(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}
