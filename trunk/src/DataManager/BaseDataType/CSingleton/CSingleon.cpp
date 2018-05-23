/*--------------------------------------------------------------------------------------------------------------------*
///	CSingleon.cpp    --  ������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.8
//	��ע��
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CSingleon.h"

#include <algorithm>

#include "../CommonData/CommonData.h"
#include "../CSolution/CSolution.h"
#include "../Panel/Panel.h"


#include "../../../../include/KnifeDll/KnifeClass.h"
#include "../../Misc/ProgramMisc.h"


CSingleon* CSingleon::m_pSingleton = NULL;


// ����
CSingleon::CSingleon()
{
	// �Ż�����
	m_CurrentSolutionList.clear();
	m_BackupSolutionList.clear();	
	m_vComponentGroup.clear();

	m_pKnifeClass = new KnifeClass;

	m_uComponentInputItemSerialID = 0;
}

// ����
CSingleon::~CSingleon()
{

}


CSingleon* CSingleon::GetSingleton()
{
	if (m_pSingleton == NULL)
	{
		m_pSingleton = new CSingleon;
	}

	return m_pSingleton;
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
int CSingleon::Layout(int Method, int CutStyle, int Org)
{
	int nGroupSize = m_vComponentGroup.size();

	for(int i = 0; i < nGroupSize; i++)
	{
		ComponentList& theComponentList = m_vComponentGroup[i];
		if(theComponentList.size() == 0)
			continue;

		// ���������������ʼ������
		CSolution* pSolution = new CSolution;

		pSolution->m_BaseInfo = m_BaseInfo;							// ������Ϣ
 		//pSolution->m_fPanelLength = m_BaseInfo.m_PanelLength;		// ��
 		//pSolution->m_fPanelWidth = m_BaseInfo.m_PanelWidth;			// ��

		pSolution->m_fThickness = theComponentList[0]->m_Thickness;
		pSolution->m_strMaterial = theComponentList[0]->m_Material;

		// �ȵ�����������
		for (int j = 0; j < theComponentList.size();j++)
		{
			Component* pCpn = theComponentList[j];

			if (pCpn->m_Texture == TextureType_V_TEXTURE)
			{
				pCpn->ClockwiseRotate90();
			}
		}
		
		pSolution->m_ComponentList = theComponentList;				// С��

		if (Method == 0)
		{
			pSolution->LayoutOptimization_RandomSortCut(CutStyle, Org);
		}
		else
		{
			pSolution->LayoutOptimization_MinWaste(CutStyle, Org);					// �Ż�
		}

		m_CurrentSolutionList.push_back(pSolution);
	}



	return 0;
}

// �������ʵ͵İ����´�ɢ����
void CSingleon::ReOptimizeSln(CSolution* pSln, int Org)
{
	if (pSln == NULL)
		return ;
			
	// �����з����Ż��ʲ���ģ����´�ɢ�Ż�
	int ReOpPanelNum = 0, NewOpPanelNum = 0;
	int ReOpLastPanelCpnNum = 0, NewOpLastPanelCpnNum = 0;
	int i, nCount = pSln->GetPanelNum();
	vector<Panel*> ReserverList, ReOpPanelList;
	vector<Component*> ReOpCpnList;

	// �����Ż��ʲ���Ĵ��
	for(i = 0; i < nCount; i++)
	{
		Panel* pPanel = pSln->GetPanel(i);
		// �ﵽ90%�����ʣ����Բ���
		if (pPanel->GetUtilization() > m_BaseInfo.m_ThirdSectionOAccptableUtilization)
		{
			ReserverList.push_back(pPanel);
		}
		else
		{
			vector<Node*> CpnList;

			pPanel->GetAllLeafNodes(CpnList, NodeType_NeededComponent);
			for (vector<Node*>::iterator it = CpnList.begin(); it != CpnList.end(); it++)
			{
				Node* pNode = *it;

				ReOpCpnList.push_back(static_cast<Component*>(pNode->CopyNode()));
			}

			ReOpPanelList.push_back(pPanel);
		}
	}

	if ( ReOpCpnList.size() > 0)
	{

		// ��������
		for (i = 0; i < ReOpCpnList.size(); i++)
		{
			Component* pCpn = ReOpCpnList[i];

			if (pCpn->m_RealWidth > pCpn->m_RealLength && pCpn->IsRotatable() == true)
			{
				pCpn->ClockwiseRotate90();
			}
		}


		// ��ʼ�����Ż�
		Panel* pLastPanel;
		Component* pLargestRemainderOfReOpLastPanel, *pLargestRemainderOfNewOpLastPanel;

		ReOpPanelNum = ReOpPanelList.size();
		pLastPanel = ReOpPanelList.back();
		ReOpLastPanelCpnNum = pLastPanel->GetComponentNum();
		pLargestRemainderOfReOpLastPanel = pLastPanel->GetLargestRemainder();


		CSolution sln;

		sln.m_BaseInfo = pSln->m_BaseInfo;
		//sln.m_fPanelLength = pSln->m_fPanelLength;
		//sln.m_fPanelWidth = pSln->m_fPanelWidth;


		sln.m_fThickness = pSln->m_fThickness;
		sln.m_strMaterial = pSln->m_strMaterial;

		// �ȵ�����������
		vector<Component*> TmpCpnList;
		for (vector<Component*>::iterator it = ReOpCpnList.begin(); it != ReOpCpnList.end(); it++)
		{
			Component* pNode = static_cast<Component*>((*it)->CopyNode());

			pNode->m_pParent = NULL;

			TmpCpnList.push_back(pNode);
		}


		sln.m_ComponentList = TmpCpnList;		// С��
		sln.m_BaseInfo = pSln->m_BaseInfo;		// ������Ϣ

		int cut_layout_method = rand()%2;

		if (cut_layout_method == 0)
		{
			sln.LayoutOptimization_RandomSortCut(CutDir_Random, Org);				// �Ż�
		}
		else
		{
			sln.LayoutOptimization_MinWaste(CutDir_Random, Org);
		}

		NewOpPanelNum = sln.GetPanelNum();
		if (NewOpPanelNum  < ReOpPanelNum)	
		{
			// ������
			ReOpPanelNum = NewOpPanelNum;

			for (int j = 0; j < ReOpPanelList.size(); j++)
			{
				Node* pPanel =  ReOpPanelList.at(j);
				pPanel->DeleteTree(&pPanel);
			}

			// �滻
			ReOpPanelList.clear();
			ReOpPanelList = sln.m_PanelList;
			sln.m_PanelList.clear();
		}
		else if(NewOpPanelNum  == ReOpPanelNum)	
		{
			pLastPanel = sln.m_PanelList.back();
			NewOpLastPanelCpnNum = pLastPanel->GetComponentNum();
			pLargestRemainderOfNewOpLastPanel = pLastPanel->GetLargestRemainder();

			if (NewOpLastPanelCpnNum < ReOpLastPanelCpnNum)
			{
				ReOpLastPanelCpnNum = NewOpLastPanelCpnNum;

				for (int j = 0; j < ReOpPanelList.size(); j++)
				{
					Node* pPanel =  ReOpPanelList.at(j);
					pPanel->DeleteTree(&pPanel);
				}

				// �滻
				ReOpPanelList.clear();
				ReOpPanelList = sln.m_PanelList;
				sln.m_PanelList.clear();
			}
			else if (ReOpLastPanelCpnNum == NewOpLastPanelCpnNum)
			{
				if (pLargestRemainderOfNewOpLastPanel->IsAreaLargeThan(*pLargestRemainderOfReOpLastPanel) == true)
				{
					for (int j = 0; j < ReOpPanelList.size(); j++)
					{
						Node* pPanel =  ReOpPanelList.at(j);
						pPanel->DeleteTree(&pPanel);
					}

					// �滻
					ReOpPanelList.clear();
					ReOpPanelList = sln.m_PanelList;
					sln.m_PanelList.clear();
				}

			}
		}

		// ���������Ż�����
		pSln->m_PanelList.clear();
		pSln->m_PanelList = ReserverList;
		pSln->m_PanelList.insert(pSln->m_PanelList.end(), ReOpPanelList.begin(), ReOpPanelList.end());
	}
}



// ��ȡ��ǰ�Ż������������
int CSingleon::GetCurSolutionPanelNum(void)	
{
	int i, panel_num = 0, nCount = m_CurrentSolutionList.size();

	for (i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_CurrentSolutionList.at(i);

		panel_num += pSln->GetPanelNum();
	}

	return panel_num;
}

// ��ȡ��ǰ�Ż������������
int CSingleon::GetBackupSolutionPanelNum(void)	
{
	int i, panel_num = 0, nCount = m_BackupSolutionList.size();

	for (i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		panel_num += pSln->GetPanelNum();
	}

	return panel_num;
}

// ��ȡ��ǰ�Ż���������
int CSingleon::GetCurSolutionNum(void)
{
	return m_CurrentSolutionList.size();
}

// ��ȡ��ǰ�Ż���������
int CSingleon::GetBackupSolutionNum(void)
{
	return m_BackupSolutionList.size();
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		�������Ž������,������Ż��������Ⱦ��Ż���������������٣����ȡ������ٵ�
//		2017-4-14  yuanzb  ����ʱ�����ִ����ܰ�����ͬ����ÿ������������ͬ�����
//		Ӧ��һ��һ���������Ƚϣ�����Ӧ������������Ƚ�	
//
//	param:
//		void
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void CSingleon::BackupBestSolution(void)
{
	// ��û�����ŷ�����ֱ�ӱ���
	if(m_BackupSolutionList.size() == 0)
	{

		SetBackupSolution(m_CurrentSolutionList);
	}
	else
	{

		// ����ʱ�����ִ����ܰ�����ͬ����ÿ������������ͬ�������Ӧ��һ��һ���������Ƚϣ�����Ӧ������������Ƚ�
		// ��Ϊ������Ź���ģ����������˳��Ҳ��ͬ������˳��Ƚϵ�ǰ�ͱ��ݵĽ������

		int nSlnCount = m_BackupSolutionList.size();
		int nCurSlnCount = m_CurrentSolutionList.size();

		// �ж�
		if (nSlnCount != nCurSlnCount)
		{
			AfxMessageBox("�������������ƥ�䣡");
			return;
		}
		
		// ѭ���Ƚ�ÿ��������������߲���
		for (int  i_sln = 0; i_sln < nSlnCount; i_sln++)
		{
			CSolution* pCurSln = m_CurrentSolutionList.at(i_sln);
			CSolution* pBackupSln = m_BackupSolutionList.at(i_sln);

			// �Աȴ������
			int CurSlnPanelNum = pCurSln->GetPanelNum();
			int BackupSlnPanelNum = pBackupSln->GetPanelNum();

			// ������ȡ�� ��Ա�滻
			if (CurSlnPanelNum < BackupSlnPanelNum)									// �����
			{
				m_BackupSolutionList[i_sln] = pCurSln;
				m_CurrentSolutionList[i_sln] = pBackupSln;
			}
			else if (CurSlnPanelNum == BackupSlnPanelNum )							// �������ͬ��С������
			{
				CSolution* pLastSln = NULL;
				Panel* pLastPanel = NULL;
				int CurSlnCpnNumOfLastPanel;
				int BackupSlnCpnNumOfLastPanel;
				Component* pCurSlnLargestRemainderOfLastPanel, *pBackupSlnLargestRemainderOfLastPanel;

				pLastSln = pCurSln;
				pLastPanel = pLastSln->m_PanelList.back();
				pCurSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
				CurSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

				pLastSln = pBackupSln;
				pLastPanel = pLastSln->m_PanelList.back();
				pBackupSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
				BackupSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

				// �ð��ٵĽ��ţ��滻
				if (CurSlnCpnNumOfLastPanel < BackupSlnCpnNumOfLastPanel)
				{
					m_BackupSolutionList[i_sln] = pCurSln;
					m_CurrentSolutionList[i_sln] = pBackupSln;
				}
				else if (CurSlnCpnNumOfLastPanel == BackupSlnCpnNumOfLastPanel)		// �������С��������ͬ��������������
				{
					// ָ�뱣��
					if (pCurSlnLargestRemainderOfLastPanel != NULL && pBackupSlnLargestRemainderOfLastPanel != NULL)
					{
						if (pCurSlnLargestRemainderOfLastPanel->IsAreaLargeThan(*pBackupSlnLargestRemainderOfLastPanel) == true)
						{
							m_BackupSolutionList[i_sln] = pCurSln;
							m_CurrentSolutionList[i_sln] = pBackupSln;
						}
					}
				}
			}
		}

		// �����ǰ����
		ClearCurrentSolution();

#if 0

		// ��Ҫ�Ƚ�
		int CurSlnPanelNum = GetCurSolutionPanelNum();
		int BackupSlnPanelNum = GetBackupSolutionPanelNum();

		// ����
		if (CurSlnPanelNum < BackupSlnPanelNum)									// �����
		{
			ClearBackupSolution();
			SetBackupSolution(m_CurrentSolutionList);
			ClearCurrentSolution();
		}
		else if (CurSlnPanelNum == BackupSlnPanelNum )							// �������ͬ��С������
		{
			CSolution* pLastSln = NULL;
			Panel* pLastPanel = NULL;
			int CurSlnCpnNumOfLastPanel;
			int BackupSlnCpnNumOfLastPanel;
			Component* pCurSlnLargestRemainderOfLastPanel, *pBackupSlnLargestRemainderOfLastPanel;

			pLastSln = m_CurrentSolutionList.back();
			pLastPanel = pLastSln->m_PanelList.back();
			pCurSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
			CurSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

			pLastSln = m_BackupSolutionList.back();
			pLastPanel = pLastSln->m_PanelList.back();
			pBackupSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
			BackupSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

			if (CurSlnCpnNumOfLastPanel < BackupSlnCpnNumOfLastPanel)
			{
				ClearBackupSolution();
				SetBackupSolution(m_CurrentSolutionList);
				ClearCurrentSolution();
			}
			else if (CurSlnCpnNumOfLastPanel == BackupSlnCpnNumOfLastPanel)		// �������С��������ͬ��������������
			{
				if (pCurSlnLargestRemainderOfLastPanel->IsAreaLargeThan(*pBackupSlnLargestRemainderOfLastPanel) == true)
				{
					ClearBackupSolution();
					SetBackupSolution(m_CurrentSolutionList);
					ClearCurrentSolution();
				}
			}
		}
		else
		{
			ClearCurrentSolution();
		}

#endif

	}
}

// �����������
void CSingleon::ClearCurrentSolution(void)
{
	for(int i = 0; i < m_CurrentSolutionList.size(); i++)
	{
		CSolution* pSln = m_CurrentSolutionList[i];

		if (pSln != NULL)
			delete pSln;
	}

	m_CurrentSolutionList.clear();
}

/*---------------------------------------*/
//	����˵����
//		�����������
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
void CSingleon::ClearBackupSolution(void)
{
	for(int i = 0; i < m_BackupSolutionList.size(); i++)
	{
		CSolution* pSln = m_BackupSolutionList[i];

		if (pSln != NULL)
			delete pSln;
	}

	m_BackupSolutionList.clear();
}

/*---------------------------------------*/
//	����˵����
//		������Ϲ�����
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
void CSingleon::ClearRemainderManager(void)
{

	for(int i = 0; i < m_RemainderManager.m_RemainderGroupList.size(); i++)
	{
		RemainderGroup* pGroup = m_RemainderManager.m_RemainderGroupList[i];

		if (pGroup != NULL)
			delete pGroup;
	}

	m_RemainderManager.m_RemainderGroupList.clear();
}



// ���ñ����Ż�����
void CSingleon::SetBackupSolution(vector<CSolution*> &sln_list)
{
	ClearBackupSolution();
	
	m_BackupSolutionList = sln_list;
	sln_list.clear();
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		����������ݣ���ʱ�������ڴ�й©���⣬�Ժ��ٴ���
//	
//	param:
//		void
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
// 
void CSingleon::ClearAllData(void)
{
	// �ͷŽ������
	ClearCurrentSolution();

	// �ͷŽ������
	ClearBackupSolution();

	// �ͷ����Ϲ�����
	ClearRemainderManager();

	// �ͷ�С�����
	m_vComponentGroup.clear();
}


// ���½��������
void CSingleon::UpdateSlnNum(void)
{
	int i, nCount;

	nCount = GetBackupSolutionNum();
	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		pSln->m_SlnID = i;

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			pPanel->m_PanelID = i_panel;
		}
	}

}

void CSingleon::UpdateComponentMachiningInfo(void)
{
	int i, nCount;
	nCount = GetBackupSolutionNum();

	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vector<Component*> vAllComponent;
			FindAllComponentInPanel(*pPanel, vAllComponent);

			for(int j = 0; j < vAllComponent.size(); j++)
			{
				Component& theComponent = *(vAllComponent[j]);
				theComponent.m_nRotatedAngle = theComponent.m_nRotatedAngle % 180;//����Ϊ����ת�ˡ�����û����ת������
				if(theComponent.m_nRotatedAngle != 0)//����Ϊ0ʱ�������Ű�ʱ�Ѱ����ת��
				{
					theComponent.m_nRotatedAngle = -90;//���Ű�ʱ��ת�˰����ͳһ��Ϊ˳ʱ����ת��90��
					OutlinePointsClockwiseRotate90(theComponent.m_vOutlinePoint, theComponent.m_RealWidth, theComponent.m_RealLength);
					HoleClockwiseRotate90(theComponent.m_vUpperFaceHole, theComponent.m_RealWidth, theComponent.m_RealLength);
					SlotClockwiseRotate90(theComponent.m_vUpperFaceSlot, theComponent.m_RealWidth, theComponent.m_RealLength);
					HoleClockwiseRotate90(theComponent.m_vDownerFaceHole, theComponent.m_RealWidth, theComponent.m_RealLength);
					SlotClockwiseRotate90(theComponent.m_vDownerFaceSlot, theComponent.m_RealWidth, theComponent.m_RealLength);
					BandingClockwiseRotate90(theComponent.m_afBanding);
					LabelPosClockwiseRotate90(theComponent.m_fXLabelCenter, theComponent.m_fYLabelCenter, theComponent.m_RealWidth, theComponent.m_RealLength);

					float fOrgProductXLen = theComponent.m_fProductLength;
					float fOrgProductYLen = theComponent.m_fProductWidth;
					theComponent.m_fProductLength = fOrgProductYLen;
					theComponent.m_fProductWidth = fOrgProductXLen;
					if (theComponent.m_Texture == 1)
						theComponent.m_Texture = 2;
					else if(theComponent.m_Texture == 2)
						theComponent.m_Texture = 1;
				}
			}
		}
	}
}

// ����Ԥ��ϰ������������������Ԥ��ϰ����һЩ�ֶ�
void CSingleon::UpdatePreCombinedComponent(void)
{
	int i, nCount;
	nCount = GetBackupSolutionNum();

	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vector<Component*> vAllComponent;
			FindAllComponentInPanel(*pPanel, vAllComponent);

			for(int j = 0; j < vAllComponent.size(); j++)
			{
				Component& theComponent = *(vAllComponent[j]);
				if(theComponent.m_ChildrenList.size() > 0)
				{
					theComponent.m_nRotatedAngle = theComponent.m_nRotatedAngle % 180;//����Ϊ����ת�ˡ�����û����ת������
					for(int k = 0; k < theComponent.m_ChildrenList.size(); k++)
					{
						Component* pThePreCombinedComponent = (Component*)(theComponent.m_ChildrenList[k]);
						float fComponentLen_BeforeOpt = (pThePreCombinedComponent->m_nRotatedAngle / 90) % 2 == 0 ? pThePreCombinedComponent->m_RealLength : pThePreCombinedComponent->m_RealWidth;
						float fComponentWidth_BeforeOpt = (pThePreCombinedComponent->m_nRotatedAngle / 90) % 2 == 0 ? pThePreCombinedComponent->m_RealWidth : pThePreCombinedComponent->m_RealLength;
						float fCombinedLen_BeforeOpt = (theComponent.m_nRotatedAngle != 0) ? theComponent.m_RealWidth : theComponent.m_RealLength;
						float fCombinedWidth_BeforeOpt = (theComponent.m_nRotatedAngle != 0) ? theComponent.m_RealLength : theComponent.m_RealWidth;
						float fComponentX_Org = pThePreCombinedComponent->m_x;
						float fComponentY_Org = pThePreCombinedComponent->m_y;

						//���±�Ԥ��ϰ��������
						if(theComponent.m_nRotatedAngle != 0)//����Ϊ0ʱ�������Ű�ʱ�Ѱ����ת��
						{
							pThePreCombinedComponent->m_x = theComponent.m_x + fComponentY_Org;
							pThePreCombinedComponent->m_y = theComponent.m_y + (fCombinedLen_BeforeOpt - (fComponentX_Org + fComponentLen_BeforeOpt));
						}
						else
						{
							pThePreCombinedComponent->m_x = theComponent.m_x + fComponentX_Org;
							pThePreCombinedComponent->m_y = theComponent.m_y + fComponentY_Org;
						}

						//���±�Ԥ��ϰ���ĽǶ�
						int nClockwiseRotateTimes = abs(GetRegularAngle(pThePreCombinedComponent->m_nRotatedAngle + theComponent.m_nRotatedAngle))/90;
						pThePreCombinedComponent->m_nRotatedAngle = 0;
						for(int l = 0; l < nClockwiseRotateTimes; l++)
						{
							ComponentRotateClockwise90(pThePreCombinedComponent);
						}
					}

					if(theComponent.m_nRotatedAngle != 0)//����Ϊ0ʱ�������Ű�ʱ�Ѱ����ת��
					{
						theComponent.m_nRotatedAngle = -90;//���Ű�ʱ��ת�˰����ͳһ��Ϊ˳ʱ����ת��90��
					}

					theComponent.m_type = NodeType_CombinePanel;
				}
			}

			vAllComponent.clear();
			FindAllComponentInPanel(*pPanel, vAllComponent);
			//���°���ID,�ѱ�Ԥ��ϰ��Ҳ�����
			std::sort(vAllComponent.begin(),vAllComponent.end(), Sort_ComponentByMachiningOrder);
			for(int i = 0; i < vAllComponent.size(); i++)
			{
				vAllComponent[i]->m_NumberInPanel = i + 1;
			}
		}
	}
}

// ��������С��ӿ�
void CSingleon::SetBackupComponentInputItem(vector<ComponentInputItem>& InputItem)
{
	m_vBackupComponentInputItem.clear();
	m_vBackupComponentInputItem = InputItem;
}