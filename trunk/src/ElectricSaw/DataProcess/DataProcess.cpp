#include "stdafx.h"
#include "DataProcess.h"
#include "../Misc/Misc.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/Algorithm/Algorithm.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"





/*--------------------------------------------------------------------------------------*/
//	purpose:
//		�������EXCELС��ṹ�壨ComponentInputItem��ת��ΪС���ࣨComponent������
//	
//	param��
//		vector<ComponentInputItem>& vComponentInputItem -- �����EXCELС������
//		ComponentList& componentList -- С������
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void ConvertInputInfoToComponentList(vector<ComponentInputItem>& vComponentInputItem, ComponentList& componentList)
{
	componentList.clear();

	int nID = 0;
	for(int i = 0; i < vComponentInputItem.size(); i++)
	{
		ComponentInputItem& theInputItem = vComponentInputItem[i];
		for(int j = 0; j < theInputItem.m_nCount; j++)
		{
			Component *pCpn = new Component;
			pCpn->m_CpnID = nID;
			pCpn->m_BarCode = theInputItem.m_strBarcode;
			pCpn->m_strCabinetName = theInputItem.m_strCabinetName;
			pCpn->m_strComponentName = theInputItem.m_strPanelName;
			pCpn->m_RealLength = theInputItem.m_fLength;
			pCpn->m_RealWidth = theInputItem.m_fWidth;
			pCpn->m_Thickness = theInputItem.m_fThickness;
			pCpn->m_Material = theInputItem.m_strMaterial;

			if(theInputItem.m_strTexture.CompareNoCase("����") == 0)
				pCpn->m_Texture = TextureType_H_TEXTURE;
			else if(theInputItem.m_strTexture.CompareNoCase("����") == 0)
				pCpn->m_Texture = TextureType_V_TEXTURE;
			else if(theInputItem.m_strTexture.CompareNoCase("������") == 0)
				pCpn->m_Texture = TextureType_NO_TEXTURE;
			else
				pCpn->m_Texture = TextureType_H_TEXTURE;

			pCpn->m_type = NodeType_NeededComponent;
			pCpn->m_x = 0;
			pCpn->m_y = 0;
			componentList.push_back(pCpn);

			nID++;
		}
	}
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��С����ݲ��ʺͺ�ȷ��飬ͬ���ʡ�ͨ��ȵ�Ϊһ��
//	
//	param��
//		ComponentList& SrcComponentList -- ���е�С������
//		vector<ComponentList>& SplitComponentGroup -- �ֺ����С������
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void SplitComponentList(ComponentList& SrcComponentList, vector<ComponentList>& SplitComponentGroup)
{
	int nSrcCompSize = SrcComponentList.size();

	// ����
	for(int i = 0; i < nSrcCompSize; i++)
	{
		int nListToInsertItemID = -1;
		int nSplitCompGroupSize = SplitComponentGroup.size();
		Component* pSrcComponent = SrcComponentList[i];

		// �鿴����������Ƿ�����ͬ���ʡ���ȵķ���
		for(int j = 0; j < nSplitCompGroupSize; j++)
		{
			if(SplitComponentGroup[j].size() > 0 )		// ���ڷ���
			{
				Component* pSplitComponent = SplitComponentGroup[j][0];

				// �жϲ��ʡ�����Ƿ���ͬ
				if (pSplitComponent->m_Material.Compare(pSrcComponent->m_Material) == 0 
					&& IsFloatEqual(pSplitComponent->m_Thickness, pSrcComponent->m_Thickness) == TRUE)
				{
					nListToInsertItemID = j;
					break;
				}
			}
		}

		if(nListToInsertItemID != -1)	// �ҵ����з���
		{
			SplitComponentGroup[nListToInsertItemID].push_back(pSrcComponent);
		}
		else							// δ�ҵ����飬�½����鲢����
		{
			ComponentList componentList;
			componentList.push_back(pSrcComponent);
			SplitComponentGroup.push_back(componentList);
		}
	}
}

void ProcessOptimize(/*CSingleon& data*/)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	//pSingleton->Layout();
	//pSingleton->BackupBestSolution();

	CString str;
	
	str.Format("%d", pSingleton->GetBackupSolutionPanelNum());
	str += "����,";
	str = "Ŀǰ����:" + str;

	MessageBox(NULL, str, "", MB_OK);

}