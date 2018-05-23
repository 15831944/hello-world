#include "stdafx.h"
#include "Algorithm.h"
#include "Markup.h"


#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../include/FileReadWrite/HgmReadWrite/HgmReadWrite.h"
#include "../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"

// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/FileReadWrited.lib")
#pragma comment(lib, "../../lib/tinyxmld.lib")

#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/FileReadWrite.lib")
#pragma comment(lib, "../../lib/tinyxml.lib")

#pragma  message(".......................................��ǰΪrelease�汾")

#endif





void SetBaseInfo(BaseInfo* pInfo)
{
	CString strTmp;

	// ���������
	pInfo->m_SawKerfWidth = 6;
	pInfo->m_PanelLength = 2440;
	pInfo->m_PanelWidth = 1220;

	// ��һ�׶β���
	pInfo->m_FirstSectionOPTimes = 1;
	pInfo->m_FirstSectionOPMethod = 1;	// ̰���㷨

	// �ڶ��׶β���
	pInfo->m_SecondSectionOPTimes = 0;

	// �����׶β���0
	pInfo->m_ThirdSectionOPTimes = 0;
	pInfo->m_ThirdSectionOAccptableUtilization = 0.9;

	// ����ԭ��
	pInfo->m_LayoutOrg = 1;

}




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

bool IsFloatEqual(float f1, float f2)
{
	if(fabs(f1 - f2) <= 0.01)
		return TRUE;
	return FALSE;
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






// ���Դ�ӡ
int Test(char* pszXml)
{
// 	CMarkup xml;
// 
// 	xml.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");
// 	xml.AddElem((MCD_CSTR)"UserInfo"); //�ڵ�ǰ��λ��Ԫ�ػ�����ֵ�λ��֮������һ��Ԫ��
// 	xml.IntoElem(); //���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ�á�
// 	xml.AddElem((MCD_CSTR)"UserID",(MCD_CSTR)"Jason");  
// 	xml.AddElem((MCD_CSTR)"UserID",(MCD_CSTR)"evil");
// 	xml.OutOfElem(); //ʹ��ǰ��λ�ñ�ɵ�ǰλ�á�
// 	xml.Save((MCD_CSTR)"E:\\�µ�̻�\\trunk\\bin\\UserInfo.xml");     //��ָ��Ŀ¼   ��XML����д���ļ���
	
	CMarkup xml;
	xml.SetDoc((const char*)pszXml);
	BOOL bFind = true;
	xml.ResetMainPos();//����ǰ��λ�ø�λΪ��һ���ֵ�λ��֮ǰ

	while (xml.FindChildElem((MCD_CSTR)"ComPanel"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
	{
		xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��

		CString strTagName = _T("");  
		CString strData = _T("");  
		CString strData3 = _T("");  

		strTagName = xml.GetTagName();		//�õ���λ��Ԫ�أ������ڽ��е�ָ��ģ���ǩ����
		strData = xml.GetAttrib("Barcode"); // �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ
		strData3 = xml.GetAttrib("Name"); // �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ

		AfxMessageBox(strData + ":" + strData3);

		while (xml.FindChildElem((MCD_CSTR)"ComInfomation"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
		{
			xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��

			while (xml.FindChildElem((MCD_CSTR)"Info"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
			{
				xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��

				CString strTagName = _T("");  
				CString strData2 = _T("");  
				CString strData3 = _T("");  

				strTagName = xml.GetTagName();		//�õ���λ��Ԫ�أ������ڽ��е�ָ��ģ���ǩ����
				strData2 = xml.GetAttrib("Name");	// �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ
				strData3 = xml.GetAttrib("Value");	// �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ

				AfxMessageBox(strData2 + ":" + strData3);

				xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ�� 
			}

			xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ��  
		}


		xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ��  
	}



	return 0;
}


int TestRead(void)
{
// 	CMarkup xml;
// 
// 	xml.Load((MCD_CSTR)"E:\\�µ�̻�\\trunk\\bin\\UserInfo.xml");
// 	BOOL bFind = true;
// 	xml.ResetMainPos();//����ǰ��λ�ø�λΪ��һ���ֵ�λ��֮ǰ
// 	while (xml.FindChildElem((MCD_CSTR)"UserID"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
// 	{
// 		xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��  
// 		CString strTagName = _T("");  
// 		CString strData = _T("");  
// 		strTagName = xml.GetTagName();  //�õ���λ��Ԫ�أ������ڽ��е�ָ��ģ���ǩ����
// 		strData = xml.GetData(); // �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ

		//����Ϊ�˿���������CString ת char*����
// 		int len1 =WideCharToMultiByte(CP_ACP,0,strTagName,-1,NULL,0,NULL,NULL);  
// 		char ptagName[128]; 
// 		WideCharToMultiByte(CP_ACP,0,strTagName,-1,ptagName,len1,NULL,NULL );  
// 
// 		int len2 =WideCharToMultiByte(CP_ACP,0,strData,-1,NULL,0,NULL,NULL);  
// 		char pData[128];  
// 		WideCharToMultiByte(CP_ACP,0,strData,-1,pData,len2,NULL,NULL ); 
// 
// 		printf("\n-----tagName:%s,Data:%s\n",ptagName,pData);
// 		xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ��  
// 	}
// 
// 	system("pause");
 	return 0;





}



/*---------------------------------------*/
//	����˵����
//		����xml�ַ�������Ȼ������
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
char* LayoutOptimizeByStream(char* pszXml)
{
	vector<ComponentInputItem> m_vComponentInputItem;

	// ��յ����������
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	// ���ļ�����ȡ��Ϣ
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString strCurPath = ExeFile;
	strCurPath = strCurPath.Left(strCurPath.ReverseFind('\\'));


	//CString SrcFilePath = strCurPath + "\\unseodata.hgm";
	CString DstFilePath = strCurPath + "\\TEMP\\seodata.xml";
	CString TempFilePath = strCurPath + "\\TEMP";

	// һ��
	if (!PathIsDirectory(TempFilePath))  
	{  
		::CreateDirectory(TempFilePath, NULL);//����Ŀ¼,���еĻ���Ӱ��  

		if (!PathIsDirectory(TempFilePath))
		{
			//AfxMessageBox(_T(TempFilePath + "Ŀ¼����ʧ��"));
			return NULL;
		}
	} 

	SourceFilePreProccesParam param;

	if(HgmReadWrite::ReadPanelInfoByStream(pszXml, m_vComponentInputItem, param ) == true)
	{
		// ����С��
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;                                                            
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}
	}
	else
	{
		//AfxMessageBox("hgm�ļ���ȡʧ�ܣ�");
		return FALSE;
	}


	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// ׼����������ʼ�Ż�
	SetBaseInfo(&(pSingleton->m_BaseInfo));

	// ����ԭ��
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;

	// �Ż����

	ComponentList componentList;

	// ��һ���Ż�
	for(int i_first_op_times = 0; i_first_op_times < pSingleton->m_BaseInfo.m_FirstSectionOPTimes; i_first_op_times++)
	{
		// �����������
		pSingleton->ClearAllData();

		// ���¸�ֵ
		ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
		SplitComponentList(componentList, pSingleton->m_vComponentGroup);

		// �Ż�
		if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// ���������
		{
			pSingleton->Layout(0, CutDir_Random, Org);
		}
		else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// ̰��
		{
			pSingleton->Layout(1, CutDir_Random, Org);
		}
		else
		{
			// ��� ̰��+���������
			int flag = /*m_valFirstSectionOPTimes*/pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

			if (i_first_op_times > flag) // ���
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else
			{
				pSingleton->Layout(1, CutDir_Random, Org);
			}
		}

		pSingleton->BackupBestSolution();
	}

	// ���½������ID�Ͱ��ID
	pSingleton->UpdateSlnNum();
	//���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�
	pSingleton->UpdateComponentMachiningInfo();


	CMarkup xml;

	//xml.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");/

	xml.AddElem((MCD_CSTR)"Root"); //�ڵ�ǰ��λ��Ԫ�ػ�����ֵ�λ��֮������һ��Ԫ��

	//xml.SetAttrib((MCD_CSTR)"Material", (MCD_CSTR)"2");
	
	// �������
 	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
 	int nSlnNum = SlnList.size();
 
 	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
 	{
 		CSolution* pCurSln = SlnList.at(i_sln);
 
 		xml.AddChildElem((MCD_CSTR)"Panel");  
 		xml.IntoElem();
 
 		CString strMaterial;
 		CString strThickness;
 		CString strCount;
 
 		strMaterial = pCurSln->m_strMaterial.GetBuffer();
 		strThickness.Format("%0.1f", pCurSln->m_fThickness);
 		strCount.Format("%d", pCurSln->GetPanelNum());
 
		xml.SetAttrib((MCD_CSTR)"Material", (MCD_CSTR)(strMaterial.GetBuffer()));
		xml.SetAttrib((MCD_CSTR)"Thickness", (MCD_CSTR)(strThickness.GetBuffer()));
		xml.SetAttrib((MCD_CSTR)"Count", (MCD_CSTR)(strCount.GetBuffer()));
 		xml.OutOfElem();
 
 	}

//	xml.OutOfElem(); //ʹ��ǰ��λ�ñ�ɵ�ǰλ�á�

// 	xml.AddElem( "ORDER" );
// 
// 
// 	xml.AddChildElem( "ITEM" );
// 
// 	xml.IntoElem();
// 
// 	xml.AddChildElem( "SN");
// 
// 	xml.SetChildAttrib((MCD_CSTR)"111", (MCD_CSTR)"222");
// 
// 	xml.OutOfElem();

	static CString strResult = xml.GetDoc();

	xml.Save((MCD_CSTR)"E:\\�µ�̻�\\trunk\\bin\\UserInfo.xml");     //��ָ��Ŀ¼   ��XML����д���ļ���
	


	// �����ļ�
// 	if (HgyReadWrite::WriteHgy_CSharp(DstFilePath) == true)
// 	{
// 		//AfxMessageBox(DstFilePath + "�ļ������ɹ�!");
// 	}
// 	else
// 	{
// 		//AfxMessageBox("�ļ�����ʧ�ܣ�");
// 		return FALSE;
// 	}




	return strResult.GetBuffer();


}



int LayoutOptimize()
{

	vector<ComponentInputItem> m_vComponentInputItem;

	// ��յ����������
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	// ���ļ�����ȡ��Ϣ
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString strCurPath = ExeFile;
	strCurPath = strCurPath.Left(strCurPath.ReverseFind('\\'));
	

	CString SrcFilePath = strCurPath + "\\unseodata.hgm";
	CString DstFilePath = strCurPath + "\\TEMP\\seodata.xml";
	CString TempFilePath = strCurPath + "\\TEMP";

	// һ��
	if (!PathIsDirectory(TempFilePath))  
	{  
		::CreateDirectory(TempFilePath, NULL);//����Ŀ¼,���еĻ���Ӱ��  

		if (!PathIsDirectory(TempFilePath))
		{
			//AfxMessageBox(_T(TempFilePath + "Ŀ¼����ʧ��"));
			return FALSE;
		}
	} 

	SourceFilePreProccesParam param;

	if(HgmReadWrite::ReadPanelInfo(SrcFilePath, m_vComponentInputItem, param ) == true)
	{
		// ����С��
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;                                                            
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}
	}
	else
	{
		//AfxMessageBox("hgm�ļ���ȡʧ�ܣ�");
		return FALSE;
	}

	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// ׼����������ʼ�Ż�
	SetBaseInfo(&(pSingleton->m_BaseInfo));

	// ����ԭ��
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;

	// �Ż����

	ComponentList componentList;

	// ��һ���Ż�
	for(int i_first_op_times = 0; i_first_op_times < pSingleton->m_BaseInfo.m_FirstSectionOPTimes; i_first_op_times++)
	{
		// �����������
		pSingleton->ClearAllData();

		// ���¸�ֵ
		ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
		SplitComponentList(componentList, pSingleton->m_vComponentGroup);

		// �Ż�
		if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// ���������
		{
			pSingleton->Layout(0, CutDir_Random, Org);
		}
		else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// ̰��
		{
			pSingleton->Layout(1, CutDir_Random, Org);
		}
		else
		{
			// ��� ̰��+���������
			int flag = /*m_valFirstSectionOPTimes*/pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

			if (i_first_op_times > flag) // ���
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else
			{
				pSingleton->Layout(1, CutDir_Random, Org);
			}
		}

		pSingleton->BackupBestSolution();
	}

	// ���½������ID�Ͱ��ID
	pSingleton->UpdateSlnNum();
	//���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�
	pSingleton->UpdateComponentMachiningInfo();

	// �����ļ�
	if (HgyReadWrite::WriteHgy_CSharp(DstFilePath) == true)
	{
		//AfxMessageBox(DstFilePath + "�ļ������ɹ�!");
	}
	else
	{
		//AfxMessageBox("�ļ�����ʧ�ܣ�");
		return FALSE;
	}


	return TRUE;
}


