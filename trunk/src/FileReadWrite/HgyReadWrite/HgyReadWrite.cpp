#include "stdafx.h"

#include "HgyReadWrite.h"
#include "../Misc/HGTools.h"
#include "../Misc/Misc.h"

//#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"


#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../Misc/EncryptionInterface.h"


/*---------------------------------------*/
//	����˵����
//		����Ϊhgy,�ض���CSharp���õ���ʱ����
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
bool HgyReadWrite::WriteHgy_CSharp(CString szFileDir)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();


	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 


	// ������
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	//TiXmlElement* pSingletonElement = new TiXmlElement("������");
	TiXmlElement* pSingletonElement = new TiXmlElement("Root");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pSingletonElement);


	// �������
	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
	int nSlnNum = SlnList.size();

	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
	{
		CSolution* pCurSln = SlnList.at(i_sln);

		//TiXmlElement* pSolutionElement = new TiXmlElement("Solution");
		//TiXmlElement* pSolutionElement = new TiXmlElement("�������");
		TiXmlElement* pSolutionElement = new TiXmlElement("Panel");

		pSolutionElement->SetAttribute("Material",		pCurSln->m_strMaterial.GetBuffer());
		pSolutionElement->SetAttribute("Thickness",		pCurSln->m_fThickness);
		pSolutionElement->SetAttribute("Count",			pCurSln->GetPanelNum());

		pSingletonElement->LinkEndChild(pSolutionElement);
	}

	m_xmlDoc->SaveFile(szFileDir);
	delete m_xmlDoc;

	return true;
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
bool  HgyReadWrite::WriteHgy(CString szFileDir)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();


	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 


	// ������
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	TiXmlElement* pSingletonElement = new TiXmlElement("������");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pSingletonElement);



	// ����С��
	vector<ComponentInputItem>& InputItem = pSingleton->m_vBackupComponentInputItem;

	//TiXmlElement* pInputItemElement = new TiXmlElement("InputItem");
	TiXmlElement* pInputItemElement = new TiXmlElement("����С��");

	pInputItemElement->SetAttribute("Line",	InputItem.size());
	pSingletonElement->LinkEndChild(pInputItemElement);


	for (vector<ComponentInputItem>::iterator i_item = InputItem.begin(); i_item != InputItem.end(); i_item++)
	{
		ComponentInputItem& item = *i_item;

		TiXmlElement* pItemElement = new TiXmlElement("ԭʼС��");

		pItemElement->SetAttribute("OrderID",				item.m_strOrderID);							// ������
		pItemElement->SetAttribute("CabinetID",				item.m_strCabinetID);						// ���
		pItemElement->SetAttribute("Barcode",				item.m_strBarcode);							// ����
		pItemElement->SetAttribute("CabinetName",			item.m_strCabinetName);						// ������
		pItemElement->SetAttribute("PanelName",				item.m_strPanelName);						// �����
		pItemElement->SetAttribute("Material",				item.m_strMaterial);						// ����
		pItemElement->SetAttribute("Length",				GetFloatString(item.m_fLength,1));			// ���ϳ�
		pItemElement->SetAttribute("Width",					GetFloatString(item.m_fWidth,1));				// ���Ͽ�
		pItemElement->SetAttribute("Thickness",				GetFloatString(item.m_fThickness,1));			// ���
		pItemElement->SetAttribute("Count",					item.m_nCount);								// ��������
		pItemElement->SetAttribute("Texture",				item.m_strTexture);							// ����

		//���		
		pItemElement->SetAttribute("Banding0",				GetFloatString(item.m_afBanding[0],1));	
		pItemElement->SetAttribute("Banding1",				GetFloatString(item.m_afBanding[1],1));	
		pItemElement->SetAttribute("Banding2",				GetFloatString(item.m_afBanding[2],1));	
		pItemElement->SetAttribute("Banding3",				GetFloatString(item.m_afBanding[3],1));	

		pItemElement->SetAttribute("OtherShapeIDAndParam",	item.m_strOtherShapeIDAndParam);			// ���β���
		pItemElement->SetAttribute("RotateType",			item.m_eRotateType);						// ��ת
		pItemElement->SetAttribute("FrontSlotInfo",			item.m_strUpperFaceSlotInfo);				// �������Ϣ
		pItemElement->SetAttribute("SlotFlipped",			item.m_bSlotFlipped);						// �۷�ת
		pItemElement->SetAttribute("BackSlotInfo",			item.m_strDownerFaceSlotInfo);				// �������Ϣ
		pItemElement->SetAttribute("FrontHoleInfo",			item.m_strUpperFaceHoleInfo);				// �������Ϣ
		pItemElement->SetAttribute("VHoleFlipped",			item.m_bVHoleFlipped);						// �׷�ת
		pItemElement->SetAttribute("BackHoleInfo",			item.m_strDownerFaceHoleInfo);				// �������Ϣ
		pItemElement->SetAttribute("CustomerInfo",			item.m_strCustomerInfo);					// �ͻ���Ϣ
		pItemElement->SetAttribute("JoinedStore",			item.m_strJoinedStore);						// ���˵�
		pItemElement->SetAttribute("SlottingFlag",			item.m_strSlottingFlag);					// ���۱�ʶ
		pItemElement->SetAttribute("Reminder",				item.m_strReminder);						// ���Ǻ�
		pItemElement->SetAttribute("Drilling",				item.m_strDrilling);						// ���
		pItemElement->SetAttribute("OrderType",				item.m_strOrderType);						// ��������
		pItemElement->SetAttribute("ReverseSideBarcode",	item.m_strReverseSideBarcode);				// ��������
		pItemElement->SetAttribute("ProductLength",			GetFloatString(item.m_fProductLength,1));						// ��Ʒ����
		pItemElement->SetAttribute("ProductWidth",			GetFloatString(item.m_fProductWidth,1));						// ��Ʒ���
		pItemElement->SetAttribute("ProductThickness",		GetFloatString(item.m_fProductThickness,1));					// ��Ʒ���
		pItemElement->SetAttribute("OtherShapeID",			item.m_strOtherShapeID);					// ����ID
		pItemElement->SetAttribute("CustomerAddress",		item.m_strCustomerAddress);					// �ͻ���ַ
		pItemElement->SetAttribute("HoleSlotFlag",			item.m_strHoleSlotFlag);					// ��۱�ʶ
		pItemElement->SetAttribute("AbnormalInfo",			item.m_strAbnormalInfo);					// ���ε���Ϣ



		pInputItemElement->LinkEndChild(pItemElement);


	}





	


	// �������
	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
	int nSlnNum = SlnList.size();

	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
	{
		CSolution* pCurSln = SlnList.at(i_sln);

		//TiXmlElement* pSolutionElement = new TiXmlElement("Solution");
		TiXmlElement* pSolutionElement = new TiXmlElement("�������");

		pSolutionElement->SetAttribute("SlnId",			pCurSln->m_SlnID);
		pSolutionElement->SetAttribute("PanelLength",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_PanelLength, 1));
		pSolutionElement->SetAttribute("PanelWidth",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_PanelWidth, 1));
		pSolutionElement->SetAttribute("SawKerfWidth",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_SawKerfWidth, 1));
		pSolutionElement->SetAttribute("DeburringWidth",GetFloatStringTrimming(pCurSln->m_BaseInfo.m_DeburringWidth, 1));
		pSolutionElement->SetAttribute("Material",		pCurSln->m_strMaterial.GetBuffer());
		pSolutionElement->SetAttribute("Thickness",		GetFloatStringTrimming(pCurSln->m_fThickness, 1));
		pSingletonElement->LinkEndChild(pSolutionElement);

		int nPanelNum = pCurSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pCurPanel = pCurSln->GetPanel(i_panel);

			//TiXmlElement* pPanelElement = new TiXmlElement("Panel");
			TiXmlElement* pPanelElement = new TiXmlElement("���");

			// panel����Ϣ
			pPanelElement->SetAttribute("PanelId",		pCurPanel->m_PanelID);
			pPanelElement->SetAttribute("OrgLen",		GetFloatString(pCurPanel->m_OrgLen, 1));
			pPanelElement->SetAttribute("OrgWidth",		GetFloatString(pCurPanel->m_OrgWidth, 1));
			pPanelElement->SetAttribute("OriginX",		GetFloatString(pCurPanel->m_OriginX, 1));
			pPanelElement->SetAttribute("OriginY",		GetFloatString(pCurPanel->m_OriginY, 1));

			// component����Ϣ
			pPanelElement->SetAttribute("x",			GetFloatString(pCurPanel->m_x, 1));
			pPanelElement->SetAttribute("y",			GetFloatString(pCurPanel->m_y, 1));
			pPanelElement->SetAttribute("RealLength",	GetFloatString(pCurPanel->m_RealLength, 1));
			pPanelElement->SetAttribute("RealWidth",	GetFloatString(pCurPanel->m_RealWidth, 1));
			pPanelElement->SetAttribute("Thickness",	GetFloatString(pCurPanel->m_Thickness, 1));
			pPanelElement->SetAttribute("Material",		pCurPanel->m_Material);
			pPanelElement->SetAttribute("type",			pCurPanel->m_type);


			pSolutionElement->LinkEndChild(pPanelElement);

			// �������Խṹ
			SaveOneTree2Xml(pPanelElement, pCurPanel);

		}
	}


	CString xml_path = szFileDir.Left(szFileDir.GetLength() - 4) + ".xml";

	m_xmlDoc->SaveFile(xml_path);
	delete m_xmlDoc;

#if (NEW_ENCRYPT_BASE64 == 1)


	//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
	CString hgo_path = szFileDir;

	encrypt_base64(xml_path.GetBuffer(), hgo_path.GetBuffer());
	DeleteFile(xml_path);


#endif



	return true;
}







// ���������νṹ��XML�ļ�
bool HgyReadWrite::SaveOneTree2Xml(TiXmlElement* pCurNode, Component* pCurCpn)
{
	if (pCurCpn == NULL)
	{
		return false;
	}

	int i, nCount;
	
	nCount = pCurCpn->m_ChildrenList.size();
	for (i = 0; i < nCount; i++)
	{
		TiXmlElement* pCpnElem = NULL;
		Component* pChildCpn = static_cast<Component*>(pCurCpn->m_ChildrenList.at(i));
		int cur_cpn_type = pChildCpn->m_type;


		// �жϰ������
		switch(cur_cpn_type)
		{
		case NodeType_Remainder:
			//pElem = new TiXmlElement("Remainder");
			pCpnElem = new TiXmlElement("����");
			break;
		case NodeType_NeededComponent:
			//pElem = new TiXmlElement("Component");
			pCpnElem = new TiXmlElement("С��");
			break;
		case NodeType_CombinePanel:
			//pElem = new TiXmlElement("CombinePanel");
			pCpnElem = new TiXmlElement("��ϰ�");
			break;
		default:
			pCpnElem = new TiXmlElement("Error");
			break;
		}

		pCpnElem->SetAttribute("type",				pChildCpn->m_type);						// 0�����ϣ��ӽڵ㣩  1��Ҫ����С�壨�ӽڵ㣩   2����ϰ壨���ڵ㣩
		pCpnElem->SetAttribute("NumberInPanel",		pChildCpn->m_NumberInPanel);			// ������� �ڼ����ŵ�С��
		pCpnElem->SetAttribute("Barcode",			pChildCpn->m_BarCode);					// ����

		pCpnElem->SetAttribute("CabinetName",		pChildCpn->m_strCabinetName);			// ��������
		pCpnElem->SetAttribute("ComponentName",		pChildCpn->m_strComponentName);			// �������
		pCpnElem->SetAttribute("Material",			pChildCpn->m_Material);					// ����
		pCpnElem->SetAttribute("Texture",			pChildCpn->m_Texture);					// ����	0��������  1������ 2������

		pCpnElem->SetAttribute("Thickness",			GetFloatString(pChildCpn->m_Thickness, 1));				// ���
		pCpnElem->SetAttribute("RealLength",		GetFloatString(pChildCpn->m_RealLength, 1));				// ��ʵ��
		pCpnElem->SetAttribute("RealWidth",			GetFloatString(pChildCpn->m_RealWidth, 1));				// ��ʵ��
		pCpnElem->SetAttribute("x",					GetFloatString(pChildCpn->m_x, 1));						// x����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
		pCpnElem->SetAttribute("y",					GetFloatString(pChildCpn->m_y, 1));						// y����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
		pCpnElem->SetAttribute("RotatedAngle",		pChildCpn->m_nRotatedAngle);			// ��ת��ʶ	
		pCpnElem->SetAttribute("OtherShapeID",		pChildCpn->m_nOtherShapeID);			// ����ID
		pCpnElem->SetAttribute("KnifeDownPos",		pChildCpn->m_nKnifeDownPos);			// �µ���

		pCpnElem->SetAttribute("OrderID",			pChildCpn->m_strOrderID);				// ������
		pCpnElem->SetAttribute("CabinetID",			pChildCpn->m_strCabinetID);				// ���

		pCpnElem->SetAttribute("Banding0",			GetFloatString(pChildCpn->m_afBanding[0], 1));				// ���  
		pCpnElem->SetAttribute("Banding1",			GetFloatString(pChildCpn->m_afBanding[1], 1));
		pCpnElem->SetAttribute("Banding2",			GetFloatString(pChildCpn->m_afBanding[2], 1));
		pCpnElem->SetAttribute("Banding3",			GetFloatString(pChildCpn->m_afBanding[3], 1));

		pCpnElem->SetAttribute("SlotFlipped",		pChildCpn->m_bSlotFlipped);				// �۷�ת
		pCpnElem->SetAttribute("VHoleFlipped",		pChildCpn->m_bVHoleFlipped);			// �׷�ת
		pCpnElem->SetAttribute("CustomerInfo",		pChildCpn->m_strCustomerInfo);			// �ͻ���Ϣ
		pCpnElem->SetAttribute("JoinedStore",		pChildCpn->m_strJoinedStore);			// ���˵�
		pCpnElem->SetAttribute("SlottingFlag",		pChildCpn->m_strSlottingFlag);			// ���۱�ʶ
		pCpnElem->SetAttribute("Reminder",			pChildCpn->m_strReminder);				// ���Ǻ�
		pCpnElem->SetAttribute("Drilling",			pChildCpn->m_strDrilling);				// ���
		pCpnElem->SetAttribute("OrderType",			pChildCpn->m_strOrderType);				// ��������
		pCpnElem->SetAttribute("ReverseSideBarcode",pChildCpn->m_strReverseSideBarcode);	// ��������
		pCpnElem->SetAttribute("ProductLength",		GetFloatString(pChildCpn->m_fProductLength, 1));			// ��Ʒ����
		pCpnElem->SetAttribute("ProductWidth",		GetFloatString(pChildCpn->m_fProductWidth, 1));			// ��Ʒ���
		pCpnElem->SetAttribute("ProductThickness",	GetFloatString(pChildCpn->m_fProductThickness, 1));		// ��Ʒ���
		pCpnElem->SetAttribute("CustomerAddress",	pChildCpn->m_strCustomerAddress);		// �ͻ���ַ
		pCpnElem->SetAttribute("HoleSlotFlag",		pChildCpn->m_strHoleSlotFlag);			// ��۱�ʶ


		if (cur_cpn_type == NodeType_NeededComponent)
		{
			// ����������
			SaveOutline(pCpnElem, pChildCpn);

			// ���������
			SaveUpperFaceHole(pCpnElem, pChildCpn);

			// ���������
			SaveUpperFaceSlot(pCpnElem, pChildCpn);

			// ���淴���
			SaveDownerFaceHole(pCpnElem, pChildCpn);

			// ���淴���
			SaveDownerFaceSlot(pCpnElem, pChildCpn);
		}


		// ����ڵ���븸����ڵ�
		pCurNode->LinkEndChild(pCpnElem);

		// �ݹ����
		if (pChildCpn->m_ChildrenList.size() > 0)
		{
			SaveOneTree2Xml(pCpnElem, pChildCpn);
		}
	}





	return true;
}

/*---------------------------------------*/
//	����˵����
//		��ȡ������Ρ��ײ�����
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadOutlineHoleSlotInfo(TiXmlElement* pCurCpnNode, Component* pNewCpn)
{

	for (TiXmlElement * pCurInfoNode = pCurCpnNode->FirstChildElement(); pCurInfoNode != NULL; pCurInfoNode = (TiXmlElement*)(pCurInfoNode->NextSibling()))
	{
		string node_name = pCurInfoNode->Value();

		if(node_name == "������")
		{
			// ��ȡ���ε���Ϣ
			ReadOutline(pCurInfoNode, pNewCpn);
		}
		else if (node_name == "�����")
		{
			// ��ȡ����� 
			ReadUpperFaceHole(pCurInfoNode, pNewCpn);

		}
		else if (node_name == "�����")
		{
			// ��ȡ�����
			ReadUpperFaceSlot(pCurInfoNode, pNewCpn);

		}
		else if (node_name == "�����")
		{
			// ��ȡ�����
			ReadDownerFaceHole(pCurInfoNode, pNewCpn);
		}
		else if (node_name == "�����")
		{
			// ��ȡ�����
			ReadDownerFaceSlot(pCurInfoNode, pNewCpn);
		}
		else
		{
			AfxMessageBox("���ݴ���");
		}
	}

}


/*---------------------------------------*/
//	����˵����
//		��ȡ�����������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadOutline(TiXmlElement* pCpnElem, Component* pCpn)
{
	// �����ԭ�е�
	pCpn->m_vOutlinePoint.clear();

	// ѭ����ȡ��������Ϣ
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		PointInfo pnt;

		pnt.x		= _ttof(pInfoNode->Attribute("X"	));	
		pnt.y		= _ttof(pInfoNode->Attribute("Y"	));	
		pnt.r		= _ttof(pInfoNode->Attribute("R"	));	
		pnt.sign	= _ttoi(pInfoNode->Attribute("Sign"	));
		pnt.dir		= _ttoi(pInfoNode->Attribute("Dir"	));	
		pnt.cut		= _ttoi(pInfoNode->Attribute("Cut"	));	
		pnt.group	= _ttoi(pInfoNode->Attribute("Group"));
		pnt.side	= _ttoi(pInfoNode->Attribute("Side"	));
		pnt.type	= _ttoi(pInfoNode->Attribute("type"	));

		pCpn->m_vOutlinePoint.push_back(pnt);
	}
}

/*---------------------------------------*/
//	����˵����
//		��ȡ������������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	// �����ԭ�е�
	pCpn->m_vUpperFaceHole.clear();

	// ѭ����ȡ��������Ϣ
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllVec hole;

		hole._x			=	_ttof(pInfoNode->Attribute("X"		));	
		hole._y			=	_ttof(pInfoNode->Attribute("Y"		));	
		hole._r			=	_ttof(pInfoNode->Attribute("R"		));	
		hole._depth		=	_ttof(pInfoNode->Attribute("Depth"	));
		hole.panel_info	=	pInfoNode->Attribute("Info"	);

		pCpn->m_vUpperFaceHole.push_back(hole);
	}
}

/*---------------------------------------*/
//	����˵����
//		��ȡ������������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	// �����ԭ�е�
	pCpn->m_vUpperFaceSlot.clear();

	// ѭ����ȡ��������Ϣ
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllSlot slot;

		slot._x			= _ttof(pInfoNode->Attribute("X"		));	
		slot._y			= _ttof(pInfoNode->Attribute("Y"		));	
		slot._width		= _ttof(pInfoNode->Attribute("Width"	));	
		slot._height	= _ttof(pInfoNode->Attribute("Height"	));	
		slot._depth		= _ttof(pInfoNode->Attribute("Depth"	));
		slot._dir		= _ttoi(pInfoNode->Attribute("Dir"		));
		slot.panel_info	= pInfoNode->Attribute("Info"		);	

		pCpn->m_vUpperFaceSlot.push_back(slot);
	}
}


/*---------------------------------------*/
//	����˵����
//		��ȡ������������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	// �����ԭ�е�
	pCpn->m_vDownerFaceHole.clear();

	// ѭ����ȡ��������Ϣ
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllVec hole;

		hole._x			=	_ttof(pInfoNode->Attribute("X"		));	
		hole._y			=	_ttof(pInfoNode->Attribute("Y"		));	
		hole._r			=	_ttof(pInfoNode->Attribute("R"		));	
		hole._depth		=	_ttof(pInfoNode->Attribute("Depth"	));
		hole.panel_info	=	pInfoNode->Attribute("Info"	);

		pCpn->m_vDownerFaceHole.push_back(hole);
	}
}


/*---------------------------------------*/
//	����˵����
//		��ȡ������������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	// �����ԭ�е�
	pCpn->m_vDownerFaceSlot.clear();

	// ѭ����ȡ��������Ϣ
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllSlot slot;

		slot._x			= _ttof(pInfoNode->Attribute("X"		));	
		slot._y			= _ttof(pInfoNode->Attribute("Y"		));	
		slot._width		= _ttof(pInfoNode->Attribute("Width"	));	
		slot._height	= _ttof(pInfoNode->Attribute("Height"	));	
		slot._depth		= _ttof(pInfoNode->Attribute("Depth"	));
		slot._dir		= _ttoi(pInfoNode->Attribute("Dir"		));
		slot.panel_info	= pInfoNode->Attribute("Info"		);	

		pCpn->m_vDownerFaceSlot.push_back(slot);
	}
}

/*---------------------------------------*/
//	����˵����
//		��������������
//
//
//	������
//		TiXmlElement* pCpnElem	--	���XML�ڵ�
//		Component* pCpn			--	���ָ��
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void HgyReadWrite::SaveOutline(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nOutlineCount = pCpn->m_vOutlinePoint.size();
	if (nOutlineCount > 0)
	{
		// �½�һ��Outline�ڵ�
		//TiXmlElement* pOutlineElem = new TiXmlElement("Outline");
		TiXmlElement* pOutlineElem = new TiXmlElement("������");
		pCpnElem->LinkEndChild(pOutlineElem);

		// �½�һ������������ڵ����pOutlineElem��
		for (int i_outline = 0; i_outline < nOutlineCount; i_outline++)
		{
			PointInfo pnt = pCpn->m_vOutlinePoint.at(i_outline);
			//TiXmlElement* pVectorNode = new TiXmlElement("PointInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("����Ϣ");

			pVectorNode->SetAttribute("X",		GetFloatString(pnt.x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(pnt.y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(pnt.r,1));	
			pVectorNode->SetAttribute("Sign",	pnt.sign);
			pVectorNode->SetAttribute("Dir",	pnt.dir);	
			pVectorNode->SetAttribute("Cut",	pnt.cut);	
			pVectorNode->SetAttribute("Group",	pnt.group);
			pVectorNode->SetAttribute("Side",	pnt.side);
			pVectorNode->SetAttribute("type",	pnt.type);

			pOutlineElem->LinkEndChild(pVectorNode);
		}
	}
}




// ���������������
void HgyReadWrite::SaveUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nUpperFaceHoleCount = pCpn->m_vUpperFaceHole.size();
	if (nUpperFaceHoleCount > 0)
	{
		// �½�һ��Outline�ڵ�
		//TiXmlElement* pUpperFaceHoleElem = new TiXmlElement("UpperFaceHole");
		TiXmlElement* pUpperFaceHoleElem = new TiXmlElement("�����");
		pCpnElem->LinkEndChild(pUpperFaceHoleElem);

		// �½�һ������������ڵ����pOutlineElem��
		for (int i_hole = 0; i_hole < nUpperFaceHoleCount; i_hole++)
		{
			AllVec hole = pCpn->m_vUpperFaceHole.at(i_hole);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceHoleInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("�������Ϣ");

			pVectorNode->SetAttribute("X",		GetFloatString(hole._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(hole._y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(hole._r,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(hole._depth,1));
			pVectorNode->SetAttribute("Info",	hole.panel_info);	

			pUpperFaceHoleElem->LinkEndChild(pVectorNode);
		}
	}
}	

// ���������������
void HgyReadWrite::SaveUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nUpperFaceSlotCount = pCpn->m_vUpperFaceSlot.size();
	if (nUpperFaceSlotCount > 0)
	{
		// �½�һ��Outline�ڵ�
		//TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("UpperFaceSlot");
		TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("�����");
		pCpnElem->LinkEndChild(pUpperFaceSlotElem);

		// �½�һ������������ڵ����pOutlineElem��
		for (int i_slot = 0; i_slot < nUpperFaceSlotCount; i_slot++)
		{
			AllSlot slot = pCpn->m_vUpperFaceSlot.at(i_slot);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceSlotInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("�������Ϣ");

			pVectorNode->SetAttribute("X",		GetFloatString(slot._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(slot._y,1));	
			pVectorNode->SetAttribute("Width",	GetFloatString(slot._width,1));	
			pVectorNode->SetAttribute("Height",	GetFloatString(slot._height,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(slot._depth,1));
			pVectorNode->SetAttribute("Dir",	slot._dir);
			pVectorNode->SetAttribute("Info",	slot.panel_info);	

			pUpperFaceSlotElem->LinkEndChild(pVectorNode);
		}
	}
}		

// ���������������
void HgyReadWrite::SaveDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nDownerFaceHoleCount = pCpn->m_vDownerFaceHole.size();
	if (nDownerFaceHoleCount > 0)
	{
		// �½�һ��Outline�ڵ�
		//TiXmlElement* pDownerFaceHoleElem = new TiXmlElement("DownerFaceHole");
		TiXmlElement* pDownerFaceHoleElem = new TiXmlElement("�����");
		pCpnElem->LinkEndChild(pDownerFaceHoleElem);

		// �½�һ������������ڵ����pOutlineElem��
		for (int i_hole = 0; i_hole < nDownerFaceHoleCount; i_hole++)
		{
			AllVec hole = pCpn->m_vDownerFaceHole.at(i_hole);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceHoleInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("�������Ϣ");

			pVectorNode->SetAttribute("X",		GetFloatString(hole._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(hole._y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(hole._r,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(hole._depth,1));
			pVectorNode->SetAttribute("Info",	hole.panel_info);	

			pDownerFaceHoleElem->LinkEndChild(pVectorNode);
		}
	}
}	

// ���������������
void HgyReadWrite::SaveDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nDownerFaceSlotCount = pCpn->m_vDownerFaceSlot.size();
	if (nDownerFaceSlotCount > 0)
	{
		// �½�һ��Outline�ڵ�
		//TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("DownerFaceSlot");
		TiXmlElement* pDownerFaceSlotElem = new TiXmlElement("�����");
		pCpnElem->LinkEndChild(pDownerFaceSlotElem);

		// �½�һ������������ڵ����pOutlineElem��
		for (int i_slot = 0; i_slot < nDownerFaceSlotCount; i_slot++)
		{
			AllSlot slot = pCpn->m_vDownerFaceSlot.at(i_slot);
			//TiXmlElement* pVectorNode = new TiXmlElement("DownerFaceSlotInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("�������Ϣ");

			pVectorNode->SetAttribute("X",		GetFloatString(slot._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(slot._y,1));	
			pVectorNode->SetAttribute("Width",	GetFloatString(slot._width,1));	
			pVectorNode->SetAttribute("Height",	GetFloatString(slot._height,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(slot._depth,1));
			pVectorNode->SetAttribute("Dir",	slot._dir);
			pVectorNode->SetAttribute("Info",	slot.panel_info);	

			pDownerFaceSlotElem->LinkEndChild(pVectorNode);
		}
	}
}	







// ��XML�ļ���ȡ������ͽṹ
bool HgyReadWrite::LoadOneTreeFromXml(TiXmlElement* pCurNode, Component* pParentCpn)
{
	if (pParentCpn == NULL)
	{
		return false;
	}

	for (TiXmlElement * pCurCpnNode = pCurNode->FirstChildElement(); pCurCpnNode != NULL; pCurCpnNode = (TiXmlElement*)(pCurCpnNode->NextSibling()))
	{
		string bar_code;
		string node_name = pCurCpnNode->Value();
		Component* pNewCpn = new Component;

		pNewCpn->m_type								= _ttoi(pCurCpnNode->Attribute("type"));				// 0�����ϣ��ӽڵ㣩  1��Ҫ����С�壨�ӽڵ㣩   2����ϰ壨���ڵ㣩
		pNewCpn->m_NumberInPanel					= _ttoi(pCurCpnNode->Attribute("NumberInPanel"));		// ������� �ڼ����ŵ�С��
		pNewCpn->m_BarCode							= pCurCpnNode->Attribute("Barcode");

		pNewCpn->m_strCabinetName					= pCurCpnNode->Attribute("CabinetName");				// ��������
		pNewCpn->m_strComponentName					= pCurCpnNode->Attribute("ComponentName");				// �������
		pNewCpn->m_Material							= pCurCpnNode->Attribute("Material");					// ����
		pNewCpn->m_Texture							= _ttoi(pCurCpnNode->Attribute("Texture"));				// ����	0��������  1������ 2������

		pNewCpn->m_Thickness						= _ttof(pCurCpnNode->Attribute("Thickness"));			// ���
		pNewCpn->m_RealLength						= _ttof(pCurCpnNode->Attribute("RealLength"));			// ��ʵ��
		pNewCpn->m_RealWidth						= _ttof(pCurCpnNode->Attribute("RealWidth"));			// ��ʵ��
		pNewCpn->m_x								= _ttof(pCurCpnNode->Attribute("x"));					// x����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
		pNewCpn->m_y								= _ttof(pCurCpnNode->Attribute("y"));					// y����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
		pNewCpn->m_nRotatedAngle					= _ttoi(pCurCpnNode->Attribute("RotatedAngle"));		// ��ת��ʶ
		pNewCpn->m_nOtherShapeID					= _ttoi(pCurCpnNode->Attribute("OtherShapeID"));		// ����ID
		pNewCpn->m_nKnifeDownPos					= _ttoi(pCurCpnNode->Attribute("KnifeDownPos"));		// �µ���

		pNewCpn->m_strOrderID						= pCurCpnNode->Attribute("OrderID");					// ������
		pNewCpn->m_strCabinetID						= pCurCpnNode->Attribute("CabinetID");					// ���

		pNewCpn->m_afBanding[0]						= _ttof(pCurCpnNode->Attribute("Banding0"));			// ��� 
		pNewCpn->m_afBanding[1]						= _ttof(pCurCpnNode->Attribute("Banding1"));			// ��� 
		pNewCpn->m_afBanding[2]						= _ttof(pCurCpnNode->Attribute("Banding2"));			// ��� 
		pNewCpn->m_afBanding[3]						= _ttof(pCurCpnNode->Attribute("Banding3"));			// ��� 	 

		pNewCpn->m_bSlotFlipped						= _ttoi(pCurCpnNode->Attribute("SlotFlipped"));			// �۷�ת
		pNewCpn->m_bVHoleFlipped					= _ttoi(pCurCpnNode->Attribute("VHoleFlipped"));		// �׷�ת
		pNewCpn->m_strCustomerInfo					= pCurCpnNode->Attribute("CustomerInfo");				// �ͻ���Ϣ
		pNewCpn->m_strJoinedStore					= pCurCpnNode->Attribute("JoinedStore");				// ���˵�
		pNewCpn->m_strSlottingFlag					= pCurCpnNode->Attribute("SlottingFlag");				// ���۱�ʶ
		pNewCpn->m_strReminder						= pCurCpnNode->Attribute("Reminder");					// ���Ǻ�
		pNewCpn->m_strDrilling						= pCurCpnNode->Attribute("Drilling");					// ���
		pNewCpn->m_strOrderType						= pCurCpnNode->Attribute("OrderType");					// ��������
		pNewCpn->m_strReverseSideBarcode			= pCurCpnNode->Attribute("ReverseSideBarcode");			// ��������
		pNewCpn->m_fProductLength					= _ttof(pCurCpnNode->Attribute("ProductLength"));		// ��Ʒ����
		pNewCpn->m_fProductWidth					= _ttof(pCurCpnNode->Attribute("ProductWidth"));		// ��Ʒ���
		pNewCpn->m_fProductThickness				= _ttof(pCurCpnNode->Attribute("ProductThickness"));	// ��Ʒ���
		pNewCpn->m_strCustomerAddress				= pCurCpnNode->Attribute("CustomerAddress");			// �ͻ���ַ
		pNewCpn->m_strHoleSlotFlag					= pCurCpnNode->Attribute("HoleSlotFlag");				// ��۱�ʶ







		// ���븸�ڵ�
		pParentCpn->AddChild(pNewCpn);

		// С�岻�ݹ��ȡ
		if (node_name == "С��")
		{
			// ��ȡ�������ײ���Ϣ
			ReadOutlineHoleSlotInfo(pCurCpnNode, pNewCpn);
		}
		else
		{
			// �������͵Ľڵ�ݹ��ȡ
			LoadOneTreeFromXml(pCurCpnNode, pNewCpn);
		}
	}

	return true;
}



bool  HgyReadWrite::ReadHgy(CString strSrcPath)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	//�����ļ���ȡ��׼
	CString XmlPath = HGTools::getXmlPathFromHgxPath(strSrcPath);

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	decrypt_base64(strSrcPath.GetBuffer(), XmlPath.GetBuffer());

#else

	//HGTools::decryptFile(strSrcPath, XmlPath);

#endif




	// �����ļ�
	TiXmlDocument* m_xmlDoc = new TiXmlDocument();
	m_xmlDoc->LoadFile(XmlPath);

	TiXmlElement* pRoot = m_xmlDoc->RootElement();

	// �ж�
	if (pRoot == NULL)
	{
		return false;
	}

	string strValue =  pRoot->Value();
	if (strValue != "������")
	{
		return false;
	}

	for (TiXmlElement * pCurNode = pRoot->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
	{
		string strCurNodeName =  pCurNode->Value();
		
		if (strCurNodeName == "����С��")
		{
			vector<ComponentInputItem>  vInputCpn;

			for (TiXmlElement * pInputItem = pCurNode->FirstChildElement(); pInputItem != NULL; pInputItem = (TiXmlElement*)(pInputItem->NextSibling()))
			{
				ComponentInputItem item;

				item.m_strOrderID				=		pInputItem->Attribute("OrderID"				)		;	// ������
				item.m_strCabinetID				=		pInputItem->Attribute("CabinetID"			)		;	// ���
				item.m_strBarcode				=		pInputItem->Attribute("Barcode"				)		;	// ����
				item.m_strCabinetName			=		pInputItem->Attribute("CabinetName"			)		;	// ������
				item.m_strPanelName				=		pInputItem->Attribute("PanelName"			)		;	// �����
				item.m_strMaterial				=		pInputItem->Attribute("Material"			)		;	// ����
				item.m_fLength					= _ttof(pInputItem->Attribute("Length")				)		;	// ���ϳ�
				item.m_fWidth					= _ttof(pInputItem->Attribute("Width")				)		;	// ���Ͽ�
				item.m_fThickness				= _ttof(pInputItem->Attribute("Thickness")			)		;	// ���
				item.m_nCount					= _ttoi(pInputItem->Attribute("Count")				)		;	// ��������
				item.m_strTexture				=		pInputItem->Attribute("Texture"				)		;	// ����

				//���
				item.m_afBanding[0]				= _ttof(pInputItem->Attribute("Banding0")			)		;	
				item.m_afBanding[1]				= _ttof(pInputItem->Attribute("Banding1")			)		;	
				item.m_afBanding[2]				= _ttof(pInputItem->Attribute("Banding2")			)		;	
				item.m_afBanding[3]				= _ttof(pInputItem->Attribute("Banding3")			)		;	

				item.m_strOtherShapeIDAndParam	=		pInputItem->Attribute("OtherShapeIDAndParam")		;	// ���β���
				item.m_eRotateType				= _ttoi(pInputItem->Attribute("RotateType")			)		;	// ��ת
				item.m_strUpperFaceSlotInfo		=		pInputItem->Attribute("FrontSlotInfo"		)		;	// �������Ϣ
				item.m_bSlotFlipped				= _ttoi(pInputItem->Attribute("SlotFlipped")		)		;	// �۷�ת
				item.m_strDownerFaceSlotInfo 	=		pInputItem->Attribute("BackSlotInfo"		)		;	// �������Ϣ
				item.m_strUpperFaceHoleInfo		=		pInputItem->Attribute("FrontHoleInfo"		)		;	// �������Ϣ
				item.m_bVHoleFlipped			= _ttoi(pInputItem->Attribute("VHoleFlipped")		)		;	// �׷�ת
				item.m_strDownerFaceHoleInfo 	=		pInputItem->Attribute("BackHoleInfo"		)		;	// �������Ϣ
				item.m_strCustomerInfo		 	=		pInputItem->Attribute("CustomerInfo"		)		;	// �ͻ���Ϣ
				item.m_strJoinedStore		 	=		pInputItem->Attribute("JoinedStore"			)		;	// ���˵�
				item.m_strSlottingFlag		 	=		pInputItem->Attribute("SlottingFlag"		)		;	// ���۱�ʶ
				item.m_strReminder			 	=		pInputItem->Attribute("Reminder"			)		;	// ���Ǻ�
				item.m_strDrilling			 	=		pInputItem->Attribute("Drilling"			)		;	// ���
				item.m_strOrderType			 	=		pInputItem->Attribute("OrderType"			)		;	// ��������
				item.m_strReverseSideBarcode 	=		pInputItem->Attribute("ReverseSideBarcode"	)		;	// ��������
				item.m_fProductLength			= _ttof(pInputItem->Attribute("ProductLength"	)	)		;	// ��Ʒ����
				item.m_fProductWidth			= _ttof(pInputItem->Attribute("ProductWidth"	)	)		;	// ��Ʒ���
				item.m_fProductThickness		= _ttof(pInputItem->Attribute("ProductThickness")	)		;	// ��Ʒ���
				item.m_strOtherShapeID			=		pInputItem->Attribute("OtherShapeID"		)		;	// ����ID
				item.m_strCustomerAddress		=		pInputItem->Attribute("CustomerAddress"		)		;	// �ͻ���ַ
				item.m_strHoleSlotFlag			=		pInputItem->Attribute("HoleSlotFlag"		)		;	// ��۱�ʶ
				item.m_strAbnormalInfo			=		pInputItem->Attribute("AbnormalInfo"		)		;	// ���ε���Ϣ


				vInputCpn.push_back(item);
			}

			pSingleton->SetBackupComponentInputItem(vInputCpn);
		}
		else if (strCurNodeName == "�������")
		{
			CSolution* pNewSln = new CSolution;

 			pNewSln->m_SlnID						= _ttoi(pCurNode->Attribute("SlnId"));	
			pNewSln->m_BaseInfo.m_PanelLength		= _ttof(pCurNode->Attribute("PanelLength"));		
			pNewSln->m_BaseInfo.m_PanelWidth		= _ttof(pCurNode->Attribute("PanelWidth"));	
			pNewSln->m_BaseInfo.m_SawKerfWidth		= _ttof(pCurNode->Attribute("SawKerfWidth"));
			pNewSln->m_BaseInfo.m_DeburringWidth	= _ttof(pCurNode->Attribute("DeburringWidth"));	
			pNewSln->m_strMaterial					=		pCurNode->Attribute("Material");			
			pNewSln->m_fThickness					= _ttof(pCurNode->Attribute("Thickness"));		

			pSingleton->m_BackupSolutionList.push_back(pNewSln);

			// ��ȡ�������
			for (TiXmlElement * pCurPanelNode = pCurNode->FirstChildElement(); pCurPanelNode != NULL; pCurPanelNode = (TiXmlElement*)(pCurPanelNode->NextSibling()))
			{
				Panel* pNewPanel = new Panel;

				// panel����Ϣ
				pNewPanel->m_PanelID	= _ttoi(pCurPanelNode->Attribute("PanelId"		));
				pNewPanel->m_OrgLen		= _ttof(pCurPanelNode->Attribute("OrgLen"		));
				pNewPanel->m_OrgWidth	= _ttof(pCurPanelNode->Attribute("OrgWidth"		));
				pNewPanel->m_OriginX	= _ttof(pCurPanelNode->Attribute("OriginX"		));
				pNewPanel->m_OriginY	= _ttof(pCurPanelNode->Attribute("OriginY"		));

				// component����Ϣ
				pNewPanel->m_x			= _ttof(pCurPanelNode->Attribute("x"			));
				pNewPanel->m_y			= _ttof(pCurPanelNode->Attribute("y"			));
				pNewPanel->m_RealLength	= _ttof(pCurPanelNode->Attribute("RealLength"	));
				pNewPanel->m_RealWidth	= _ttof(pCurPanelNode->Attribute("RealWidth"	));
				pNewPanel->m_Thickness	= _ttof(pCurPanelNode->Attribute("Thickness"	));
				pNewPanel->m_type		= _ttoi(pCurPanelNode->Attribute("type"	));
				pNewPanel->m_Material	= pCurPanelNode->Attribute("Material");

				// �������Խṹ
				LoadOneTreeFromXml(pCurPanelNode, pNewPanel);

				pNewSln->m_PanelList.push_back(pNewPanel);
			}
		}
		else
		{
			AfxMessageBox("hgy�ж�ȡ��δ֪�ڵ㣬������");
		}
	}


	// ��ȡ��֮��ɾ�������ļ�
	DeleteFile(XmlPath); 


	return true;
}