#include "stdafx.h"

#include "HgmReadWrite.h"
#include "Markup.h"


#include "../Misc/HGTools.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/ProgramMisc.h"

#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/KnifeDll/KnifeClass.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../Misc/Misc.h"

using namespace TinyXml;

#define NO_OTHER_SHAPE_AUTO_CREATE 1





/*---------------------------------------*/
//	����˵����
//		��ȡxml�ַ����еİ����Ϣ
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
bool  HgmReadWrite::ReadPanelInfoByStream(char* pszXml, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{


	vector<map<string, string>> input_matrial = translatexmlByStream(pszXml, param);

	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strOrderID					= one_cpn_map["������"].c_str();
		componentInputItem.m_strCabinetID				= one_cpn_map["���"].c_str();
		componentInputItem.m_strBarcode					= one_cpn_map["������"].c_str();
		componentInputItem.m_strCabinetName				= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_strPanelName				= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_strMaterial				= one_cpn_map["��������"].c_str();
		componentInputItem.m_fLength					= _ttof(one_cpn_map["���ϳ���"].c_str());
		componentInputItem.m_fWidth						= _ttof(one_cpn_map["���Ͽ��"].c_str());
		componentInputItem.m_fThickness					= _ttof(one_cpn_map["���Ϻ��"].c_str());
		componentInputItem.m_nCount						= _ttoi(one_cpn_map["��������"].c_str());
		componentInputItem.m_strTexture					= one_cpn_map["��·����"].c_str();
		CString strBandingString						= one_cpn_map["�����"].c_str();
		SpyBandingString(strBandingString, componentInputItem.m_afBanding);
		componentInputItem.m_strOtherShapeIDAndParam	= one_cpn_map["����"].c_str();
		componentInputItem.m_eRotateType				= atoi(one_cpn_map["��ת"].c_str());
		componentInputItem.m_strUpperFaceSlotInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_bSlotFlipped				= strcmp(one_cpn_map["�۷�ת"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceSlotInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_bVHoleFlipped				= strcmp(one_cpn_map["�׷�ת"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceHoleInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strCustomerInfo			= one_cpn_map["�ͻ���Ϣ"].c_str();
		componentInputItem.m_strJoinedStore				= one_cpn_map["���˵�"].c_str();
		componentInputItem.m_strSlottingFlag			= one_cpn_map["���۱�ʶ"].c_str();
		componentInputItem.m_strReminder				= one_cpn_map["���Ǻ�"].c_str();
		componentInputItem.m_strDrilling				= one_cpn_map["���"].c_str();
		componentInputItem.m_strOrderType				= one_cpn_map["��������"].c_str();
		componentInputItem.m_strReverseSideBarcode		= one_cpn_map["��������"].c_str();
		componentInputItem.m_fProductLength				= _ttof(one_cpn_map["��Ʒ����"].c_str());
		componentInputItem.m_fProductWidth				= _ttof(one_cpn_map["��Ʒ���"].c_str());
		componentInputItem.m_fProductThickness			= _ttof(one_cpn_map["��Ʒ���"].c_str());
		componentInputItem.m_strOtherShapeID			= one_cpn_map["����ID"].c_str();
		componentInputItem.m_strCustomerAddress			= one_cpn_map["�ͻ���ַ"].c_str();
		componentInputItem.m_strHoleSlotFlag			= one_cpn_map["��۱�ʶ"].c_str();
		componentInputItem.m_strAbnormalInfo			= one_cpn_map["���ε���Ϣ"].c_str();
		componentInputItem.m_strSendingDate             = one_cpn_map["��������"].c_str();

		vComponentInputItem.push_back(componentInputItem);



	}

	return true;
}





/*---------------------------------------*/
//	����˵����
//		��ȡhgm�ļ��еİ����Ϣ
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
bool  HgmReadWrite::ReadPanelInfo(CString strSrcPath, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{

	//�����ļ���ȡ��׼
	CString XmlPath = HGTools::getXmlPathFromHgxPath(strSrcPath);



	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	decrypt_base64(strSrcPath.GetBuffer(), XmlPath.GetBuffer());

#else

	//HGTools::decryptFile(strSrcPath, XmlPath);

#endif




	vector<map<string, string>> input_matrial = translatexml(XmlPath.GetBuffer(), param);

	DeleteFile(XmlPath);


// 	string PointOverPanelSize = "";
// 	string AbnormalPointError = "";
	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strOrderID					= one_cpn_map["������"].c_str();
		componentInputItem.m_strCabinetID				= one_cpn_map["���"].c_str();
		componentInputItem.m_strBarcode					= one_cpn_map["������"].c_str();
		componentInputItem.m_strCabinetName				= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_strPanelName				= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_strMaterial				= one_cpn_map["��������"].c_str();
		componentInputItem.m_fLength					= _ttof(one_cpn_map["���ϳ���"].c_str());
		componentInputItem.m_fWidth						= _ttof(one_cpn_map["���Ͽ��"].c_str());
		componentInputItem.m_fThickness					= _ttof(one_cpn_map["���Ϻ��"].c_str());
 		componentInputItem.m_nCount						= _ttoi(one_cpn_map["��������"].c_str());
 		componentInputItem.m_strTexture					= one_cpn_map["��·����"].c_str();
		CString strBandingString						= one_cpn_map["�����"].c_str();
		SpyBandingString(strBandingString, componentInputItem.m_afBanding);
 		componentInputItem.m_strOtherShapeIDAndParam	= one_cpn_map["����"].c_str();
		componentInputItem.m_eRotateType				= atoi(one_cpn_map["��ת"].c_str());
 		componentInputItem.m_strUpperFaceSlotInfo		= one_cpn_map["�����1"].c_str();
 		componentInputItem.m_bSlotFlipped				= strcmp(one_cpn_map["�۷�ת"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceSlotInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_bVHoleFlipped				= strcmp(one_cpn_map["�׷�ת"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceHoleInfo		= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strCustomerInfo			= one_cpn_map["�ͻ���Ϣ"].c_str();
		componentInputItem.m_strJoinedStore				= one_cpn_map["���˵�"].c_str();
		componentInputItem.m_strSlottingFlag			= one_cpn_map["���۱�ʶ"].c_str();
		componentInputItem.m_strReminder				= one_cpn_map["���Ǻ�"].c_str();
		componentInputItem.m_strDrilling				= one_cpn_map["���"].c_str();
		componentInputItem.m_strOrderType				= one_cpn_map["��������"].c_str();
		componentInputItem.m_strReverseSideBarcode		= one_cpn_map["��������"].c_str();
		componentInputItem.m_fProductLength				= _ttof(one_cpn_map["��Ʒ����"].c_str());
		componentInputItem.m_fProductWidth				= _ttof(one_cpn_map["��Ʒ���"].c_str());
		componentInputItem.m_fProductThickness			= _ttof(one_cpn_map["��Ʒ���"].c_str());
		componentInputItem.m_strOtherShapeID			= one_cpn_map["����ID"].c_str();
		componentInputItem.m_strCustomerAddress			= one_cpn_map["�ͻ���ַ"].c_str();
		componentInputItem.m_strHoleSlotFlag			= one_cpn_map["��۱�ʶ"].c_str();
		componentInputItem.m_strAbnormalInfo			= one_cpn_map["���ε���Ϣ"].c_str();
		componentInputItem.m_strPanelClass              = one_cpn_map["�������"].c_str();
		componentInputItem.m_strSendingDate              = one_cpn_map["��������"].c_str();
		
		vComponentInputItem.push_back(componentInputItem);


		// �ж������Ƿ񳬳����
// 		float f_pwidth			= atof(one_cpn_map["���ϳ���"].c_str());
// 		float f_pheight			= atof(one_cpn_map["���Ͽ��"].c_str());
// 		std::string str_pinfo	= one_cpn_map["������"];
// 
// 
// 		// �ж����������Ƿ񳬳��˰���ߴ�
// 		std::vector<PointInfo> input_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(one_cpn_map["���ε���Ϣ"]);
// 
// 		// �������ε����
// 		int nCount = input_pointInfos.size();
// 		float OutlineMax_x, OutlineMax_y, OutlineMin_x, OutlineMin_y, max_r;
// 
// 		// �����㲻��
// 		if (nCount < 3)
// 		{
// 			AbnormalPointError = AbnormalPointError + "�����" + str_pinfo + " " + one_cpn_map["���ϳ���"] + "*" + one_cpn_map["���Ͽ��"] + "���ε����ݴ���\r\n";
// 		}
// 		else
// 		{
// 			PointInfo pnt = input_pointInfos[0];
// 			OutlineMax_x = OutlineMin_x = pnt.x;
// 			OutlineMax_y = OutlineMin_y = pnt.y;
// 			max_r = pnt.r;
// 
// 			// ��þ���
// 			for (int j = 1; j < nCount; j++)
// 			{
// 				pnt = input_pointInfos[j];
// 				float x = pnt.x;
// 				float y = pnt.y;
// 				float r = pnt.r;
// 
// 				if (x > OutlineMax_x)
// 					OutlineMax_x = x;
// 
// 				if (x < OutlineMin_x)
// 					OutlineMin_x = x;
// 
// 				if (y > OutlineMax_y)
// 					OutlineMax_y = y;
// 
// 				if (y < OutlineMin_y)
// 					OutlineMin_y = y;
// 
// 				if (r > max_r)
// 					max_r = r;
// 			}
// 
// 			// �ж��Ƿ񳬳������С
// 			float width = abs(OutlineMax_x - OutlineMin_x);
// 			float height = abs(OutlineMax_y - OutlineMin_y);
// 
// 			if (width > f_pwidth || height > f_pheight || max_r > f_pwidth || max_r > f_pheight)
// 			{
// 				PointOverPanelSize = PointOverPanelSize + "�����" + str_pinfo + " " + one_cpn_map["���ϳ���"] + "*" + one_cpn_map["���Ͽ��"] + "���ε���ܳ��������С������\r\n";
// 			}
// 		}

	}

// 	if (PointOverPanelSize != "")
// 	{
// 		CString str_Outedgeinfo;
// 		str_Outedgeinfo.Format("%s",PointOverPanelSize.c_str());
// 		AfxMessageBox(str_Outedgeinfo);
// 	}


	return true;
}


/*---------------------------------------*/
//	����˵����
//		��ȡXML�ļ��еİ����Ϣ
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
bool  HgmReadWrite::ReadPanelInfo_Debug(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{

	vector<map<string, string>> input_matrial = translatexml(szFileDir.GetBuffer(), param);


	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strBarcode			= one_cpn_map["������"].c_str();
		componentInputItem.m_strCabinetName		= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_strPanelName		= one_cpn_map["��Ʒ����"].c_str();
		componentInputItem.m_fLength			= _ttof(one_cpn_map["���ϳ���"].c_str());
		componentInputItem.m_fWidth				= _ttof(one_cpn_map["���Ͽ��"].c_str());
		componentInputItem.m_fThickness			= _ttof(one_cpn_map["���Ϻ��"].c_str());
		componentInputItem.m_strMaterial		= one_cpn_map["��������"].c_str();
		componentInputItem.m_strTexture			= one_cpn_map["��·����"].c_str();
		componentInputItem.m_nCount				= _ttoi(one_cpn_map["��������"].c_str());
		componentInputItem.m_strOtherShapeIDAndParam = one_cpn_map["����"].c_str();
		componentInputItem.m_strOtherShapeID    = one_cpn_map["����ID"].c_str();
		componentInputItem.m_strAbnormalInfo	= one_cpn_map["���ε���Ϣ"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo	= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strDownerFaceHoleInfo	= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strUpperFaceSlotInfo	= one_cpn_map["�����1"].c_str();
		componentInputItem.m_strDownerFaceSlotInfo	= one_cpn_map["�����1"].c_str();

		vComponentInputItem.push_back(componentInputItem);
	}

	return true;
}


/*---------------------------------------*/
//	����˵����
//		��xml�ַ���ת����map����
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
vector<map<string, string>> HgmReadWrite::translatexmlByStream(char* pszXml, SourceFilePreProccesParam& param)
{

	std::vector<std::map<std::string,std::string>> matrial_info;

	// �ĵ�
	CMarkup xml;
	xml.SetDoc((const char*)pszXml);
	BOOL bFind = true;
	xml.ResetMainPos();//����ǰ��λ�ø�λΪ��һ���ֵ�λ��֮ǰ

	// ���ڵ�
	if (xml.FindElem((MCD_CSTR)"KnifeData") == false)
	{
		AfxMessageBox("���ļ�");
		return matrial_info;
	}


	bool b_upvecImportEnable		= param.b_upvecImportEnable;//�����
	bool b_downvecImportEnable		= param.b_downvecImportEnable;//�����
	bool b_upsoltImportEnable		= param.b_upsoltImportEnable;//�����
	bool b_downsoltImportEnable		= param.b_downsoltImportEnable;//�����
	bool b_othershapeImportEnable	= param.b_othershapeImportEnable;//���ι���


	int i_comnum	= param.i_comnum;//�и�����
	int i_comchange =param.i_comchange;//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
	float f_changex = param.f_changex;//�ײ�ƫ��
	float f_changey = param.f_changey;//�ײ�ƫ��


	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	std::map<float,float> vecAdder = param.vecAdder;

	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	vector<float> _cxy = param._cxy;



	while (xml.FindChildElem((MCD_CSTR)"ComPanel"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
	{
		xml.IntoElem();  //���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ�� ComPanel

		std::map<std::string,std::string> map_one_matrial;

// 		CString strTagName = _T("");  
// 		CString strData = _T("");  
// 		CString strData3 = _T("");  
// 
// 		strTagName = xml.GetTagName();		//�õ���λ��Ԫ�أ������ڽ��е�ָ��ģ���ǩ����
// 		strData = xml.GetAttrib("Barcode"); // �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ
// 		strData3 = xml.GetAttrib("Name"); // �õ���ǰ��λ��Ԫ�ػ�ڵ���ַ���ֵ

//		AfxMessageBox(strData + ":" + strData3);

		while (xml.FindChildElem((MCD_CSTR)"ComInfomation"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
		{
			xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��

			while (xml.FindChildElem((MCD_CSTR)"Info"))   //��λ����һ����Ԫ�أ�ƥ��Ԫ������·����
			{
				xml.IntoElem();//���뵱ǰ��λ�õ���һ������ǰ��λ�ñ�Ϊ��λ��

				std::string info_name	= xml.GetAttrib("Name");
				std::string info_value	= xml.GetAttrib("Value");

				map_one_matrial[info_name] = info_value;


				xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ�� 
			}

			xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ��  
		}


		int knifenum = atoi(map_one_matrial["��������"].c_str()) * i_comnum;
		CString str_knifenum;
		str_knifenum.Format("%d",knifenum);
		map_one_matrial["��������"] = str_knifenum;

		// ������
		if (map_one_matrial.size() > 0)
		{
			matrial_info.push_back(map_one_matrial);
		}

		xml.OutOfElem();//ʹ��ǰ��λ�ñ�ɵ�ǰλ��  
	}


	return matrial_info;
}





/*---------------------------------------*/
//	����˵����
//		��hgm�ļ�ת����map����
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
vector<map<string, string>> HgmReadWrite::translatexml(std::string filepath, SourceFilePreProccesParam& param)
{
	std::vector<std::map<std::string,std::string>> matrial_info;

	// �ĵ�
	TiXmlDocument* doc = new TiXmlDocument;
	doc->LoadFile(filepath.c_str());

	// ���ڵ�
	TiXmlElement *pRootElement = doc->RootElement();
	if (pRootElement == NULL)
	{
		AfxMessageBox("���ļ�");
		return matrial_info;
	}

	bool b_upvecImportEnable		= param.b_upvecImportEnable;//�����
	bool b_downvecImportEnable		= param.b_downvecImportEnable;//�����
	bool b_upsoltImportEnable		= param.b_upsoltImportEnable;//�����
	bool b_downsoltImportEnable		= param.b_downsoltImportEnable;//�����
	bool b_othershapeImportEnable	= param.b_othershapeImportEnable;//���ι���
	bool bReserveDeepHole           = param.bReserveDeepHole;
	bool bReserveDeepSlot           = param.bReserveDeepSlot;


	int i_comnum	= param.i_comnum;//�и�����
	int i_comchange =param.i_comchange;//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
	float f_changex = param.f_changex;//�ײ�ƫ��
	float f_changey = param.f_changey;//�ײ�ƫ��


	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	std::map<float,float> vecAdder = param.vecAdder;

	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	vector<float> _cxy = param._cxy;


	for (TiXmlElement* pCurCompanel = pRootElement->FirstChildElement(); pCurCompanel != NULL; pCurCompanel = (TiXmlElement*)(pCurCompanel->NextSibling()))
	{
		std::map<std::string,std::string> map_one_matrial;
		float fComponentYLen = 0.0;
		float fComponentThickness = 0.0;

		string strBarCode = pCurCompanel->Attribute("Barcode");

		int index0_saw = 0;
		int index0_vertical = 0;

		int index5_saw = 0;
		int index5_vertical = 0;

		for (TiXmlElement * pCurComInfomation = pCurCompanel->FirstChildElement(); pCurComInfomation != NULL; pCurComInfomation = (TiXmlElement*)(pCurComInfomation->NextSibling()))
		{
			string strComInfo =  pCurComInfomation->Value();
			string strFaceIndex;
			const char* pIndex = pCurComInfomation->Attribute("Index");

			if (pIndex != NULL)
				strFaceIndex = pIndex;

			if (strComInfo == "ComInfomation")
			{
				// ���������Ϣ

#if 1

				for (TiXmlElement * pCurInfoNode = pCurComInfomation->FirstChildElement(); pCurInfoNode != NULL; pCurInfoNode = (TiXmlElement*)(pCurInfoNode->NextSibling()))
				{

					std::string info_name = pCurInfoNode->Attribute("Name");
					std::string info_value = pCurInfoNode->Attribute("Value");

					map_one_matrial[info_name] = info_value;
					int i = 0;
				}

				fComponentYLen = atof(map_one_matrial["���Ͽ��"].c_str());
				fComponentThickness = atof(map_one_matrial["���Ϻ��"].c_str());
#endif
			}
			else if(strComInfo == "PanelFace" &&  strFaceIndex == "0")	
			{
				// ������Ϣ 
#if 1

				//��ȡ�������Ϣ
				std::string saw_name = "�����1";
				std::string saw_value = "";
				//��ȡ�������Ϣ
				std::string vertical_name = "�����1";
				std::string vertical_value = "";

				for (TiXmlElement * pCurSawOrVtc = pCurComInfomation->FirstChildElement(); pCurSawOrVtc != NULL; pCurSawOrVtc = (TiXmlElement*)(pCurSawOrVtc->NextSibling()))
				{
					string NodeValue = pCurSawOrVtc->Value();


					// ������Ϣ Saw
					if (NodeValue == "SAW")		
					{
						index0_saw++;

						string str_pos_x; 
						string str_pos_y; 
						string str_angle; 
						string str_size_x;
						string str_size_y;
						string str_depth; 

						// ��ȡ����Ϣ
						for (TiXmlElement * saw_node = pCurSawOrVtc->FirstChildElement(); saw_node != NULL; saw_node = (TiXmlElement*)(saw_node->NextSibling()))
						{
							string param_name = saw_node->Attribute("Name");
							
							if (param_name == "pos_x")
							{
								str_pos_x = saw_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = saw_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = saw_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = saw_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = saw_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = saw_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						int i_angle = atoi(str_angle.c_str());
						if (i_angle%180 != 0)
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_y.c_str());
							float size_y = atof(str_size_x.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_y;
							float _height = size_x;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,0);

							if (b_upsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
						else
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_x.c_str());
							float size_y = atof(str_size_y.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_x;
							float _height = size_y;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,1);

							if (b_upsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
					}

					// ����Ϣ

					//�����Ϣ VERTICAL
					if (NodeValue == "VERTICAL")
					{
						index0_vertical++;


						std::string str_pos_x	;
						std::string str_pos_y	;
						std::string str_angle	;
						std::string str_size_x	;
						std::string str_size_y	;
						std::string str_depth	;



						// ��ȡ����Ϣ
						for (TiXmlElement * vertical_node = pCurSawOrVtc->FirstChildElement(); vertical_node != NULL; vertical_node = (TiXmlElement*)(vertical_node->NextSibling()))
						{
							string param_name = vertical_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = vertical_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						float pos_x = atof(str_pos_x.c_str());
						float pos_y = atof(str_pos_y.c_str());
						float size_x = atof(str_size_x.c_str());
						float size_y = atof(str_size_y.c_str());
						float depth = atof(str_depth.c_str());

						float _x = pos_x + f_changex;
						float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
						float _r = size_x;
						float _depth = depth;

						bool vecdelete = false;
						for (int v = 0;v < _cxy.size();v++)
						{
							if (_r - _cxy.at(v) > -0.1 && _r - _cxy.at(v) < 0.1)
							{
								vecdelete = true;
								break;
							}
						}
						if (vecdelete == false)
						{
							for (auto iter_mapf = vecAdder.begin();iter_mapf != vecAdder.end();iter_mapf++)
							{
								float f_r = iter_mapf->first;
								float f_f = iter_mapf->second;
								if (f_r - _r > -0.01 && f_r - _r < 0.01)
								{
									_r = _r + f_f;
									break;
								}
							}

							char one_vertical_value[100];
							sprintf_s(one_vertical_value,"%0.1f,%0.1f,%0.1f,%0.1f;",_x,_y,_r,_depth);
							if (b_upvecImportEnable || (bReserveDeepHole && (_depth - fComponentThickness) > -0.1))
								vertical_value = vertical_value + one_vertical_value;
						}
					}
				}

				// ���Ӳ�����
				map_one_matrial[saw_name] = saw_value;

				// ���ӿ�����
				map_one_matrial[vertical_name] = vertical_value;

#endif
			}
			else if (strComInfo == "PanelFace" &&  strFaceIndex == "5")
			{
				//��ȡ�������Ϣ

#if 1
				std::string saw_name = "�����1";
				std::string saw_value = "";

				//��ȡ�������Ϣ
				std::string vertical_name = "�����1";
				std::string vertical_value = "";


				for (TiXmlElement * pCurSawOrVtc = pCurComInfomation->FirstChildElement(); pCurSawOrVtc != NULL; pCurSawOrVtc = (TiXmlElement*)(pCurSawOrVtc->NextSibling()))
				{
					string NodeValue = pCurSawOrVtc->Value();


					// ������Ϣ Saw
					if (NodeValue == "SAW")		
					{
						index0_saw++;

						string str_pos_x; 
						string str_pos_y; 
						string str_angle; 
						string str_size_x;
						string str_size_y;
						string str_depth; 

						// ��ȡ����Ϣ
						for (TiXmlElement * saw_node = pCurSawOrVtc->FirstChildElement(); saw_node != NULL; saw_node = (TiXmlElement*)(saw_node->NextSibling()))
						{
							string param_name = saw_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = saw_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = saw_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = saw_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = saw_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = saw_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = saw_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						int i_angle = atoi(str_angle.c_str());
						if (i_angle%180 != 0)
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_y.c_str());
							float size_y = atof(str_size_x.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_y;
							float _height = size_x;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,0);

							if (b_downsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
						else
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_x.c_str());
							float size_y = atof(str_size_y.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_x;
							float _height = size_y;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,1);
							
							if (b_downsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
					}

					// ����Ϣ

					//�����Ϣ VERTICAL
					if (NodeValue == "VERTICAL")
					{
						index5_vertical++;


						std::string str_pos_x	;
						std::string str_pos_y	;
						std::string str_angle	;
						std::string str_size_x	;
						std::string str_size_y	;
						std::string str_depth	;



						// ��ȡ����Ϣ
						for (TiXmlElement * vertical_node = pCurSawOrVtc->FirstChildElement(); vertical_node != NULL; vertical_node = (TiXmlElement*)(vertical_node->NextSibling()))
						{
							string param_name = vertical_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = vertical_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						float pos_x = atof(str_pos_x.c_str());
						float pos_y = atof(str_pos_y.c_str());
						float size_x = atof(str_size_x.c_str());
						float size_y = atof(str_size_y.c_str());
						float depth = atof(str_depth.c_str());

						float _x = pos_x + f_changex;
						float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
						float _r = size_x;
						float _depth = depth;

						bool vecdelete = false;
						for (int v = 0;v < _cxy.size();v++)
						{
							if (_r - _cxy.at(v) > -0.1 && _r - _cxy.at(v) < 0.1)
							{
								vecdelete = true;
								break;
							}
						}
						if (vecdelete == false)
						{
							for (auto iter_mapf = vecAdder.begin();iter_mapf != vecAdder.end();iter_mapf++)
							{
								float f_r = iter_mapf->first;
								float f_f = iter_mapf->second;
								if (f_r - _r > -0.01 && f_r - _r < 0.01)
								{
									_r = _r + f_f;
									break;
								}
							}

							char one_vertical_value[100];
							sprintf_s(one_vertical_value,"%0.1f,%0.1f,%0.1f,%0.1f;",_x,_y,_r,_depth);

							if (b_downvecImportEnable || (bReserveDeepHole && (_depth - fComponentThickness) > -0.1))
								vertical_value = vertical_value + one_vertical_value;
						}
					}
				}

				map_one_matrial[saw_name] = saw_value;

				map_one_matrial[vertical_name] = vertical_value;

#endif

			}
			else if (strComInfo == "OutLine")
			{
				//¼������

#if 1

				vector<PointInfo> infos;

				//��outline����س���
				for (TiXmlElement * pVectorNode = pCurComInfomation->FirstChildElement(); pVectorNode != NULL; pVectorNode = (TiXmlElement*)(pVectorNode->NextSibling()))
				{

					PointInfo info;

					info.x		= atof(	pVectorNode->Attribute("X"));	
					info.y		= atof(	pVectorNode->Attribute("Y"));	
					info.r		= atof(	pVectorNode->Attribute("R"));	
					info.sign	= atoi(	pVectorNode->Attribute("Sign"));	
					info.dir	= atoi(	pVectorNode->Attribute("Dir"));	
					info.cut	= atoi(	pVectorNode->Attribute("Cut"));	
					info.group	= atoi(	pVectorNode->Attribute("Group"));	
					info.side	= atoi(	pVectorNode->Attribute("Side"));	
					info.type	= atoi(	pVectorNode->Attribute("type"));	

					if (info.group == 1 && infos.size()>0)	//һ��Ҫ�õ�պ�
					{
						infos[infos.size() - 1].group = 2;
					}

					infos.push_back(info);
				}

				if (infos.size()>0)	//һ��Ҫ�õ�պ�
				{
					infos[infos.size() - 1].group = 2;
				}

				map_one_matrial["���ε���Ϣ"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(infos);

#endif
			}
		}

		//����������������
		{
			//��outline�ģ��������Σ��������У�36
			if (map_one_matrial["���ε���Ϣ"].compare("") != 0 && b_othershapeImportEnable)
			{
				//���ݽ���ѡ�����ת��ͼ�ν�����ת
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(map_one_matrial["���ε���Ϣ"]);
				//m_pointInfos = OtherShapeHelper::SetPointInfoYNegative(m_pointInfos);
				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, map_one_matrial["��ת"], map_one_matrial["���ϳ���"], map_one_matrial["���Ͽ��"]);
				m_pointInfos = OtherShapeHelper::getPointInfoRotate(m_pointInfos,map_one_matrial["��ת"],map_one_matrial["���ϳ���"],map_one_matrial["���Ͽ��"]);
				map_one_matrial["���ε���Ϣ"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos);
			}
			//û��outline�ģ��������ַ�������outline
			else if (map_one_matrial["���ε���Ϣ"].compare("") == 0 && map_one_matrial["����"].compare("") != 0 && b_othershapeImportEnable)
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				//��ý�������в�������У��
				std::vector<param_point> pa_p = OtherShapeHelper::GetParamPointsFromOtherShapeString(map_one_matrial["����"],map_one_matrial["���ϳ���"],map_one_matrial["���Ͽ��"]);

				OutputDebugString("START:\n");
				for(int k = 0; k < pa_p.size(); k++)
				{
					CString strTmp;
					strTmp.Format("name=%s value =%s\n", pa_p[k]._name.c_str(), pa_p[k]._value.c_str());
					OutputDebugString(strTmp);
				}

				OtherShapeHelper::RotateOtherShapeParamPoint(pa_p,map_one_matrial["��ת"]);

				//���ƽ�����������ɵ�ͼ��
				std::vector<std::map<std::string,std::string>>m_outline = OtherShapeHelper::GetOtherShapePanelOutLineListFromOtherShapeID(OtherShapeHelper::GetOtherShapeId(pa_p));
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint(m_outline,pa_p);

				OutputDebugString("START:\n");
				for(int k = 0; k < m_pointInfos.size(); k++)
				{
					CString strTmp;
					strTmp.Format("x=%f y =%f\n", m_pointInfos[k].x, m_pointInfos[k].y);
					OutputDebugString(strTmp);
				}

				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, map_one_matrial["��ת"], map_one_matrial["���ϳ���"], map_one_matrial["���Ͽ��"]);

				//���ݽ���ѡ�����ת��ͼ�ν�����ת
				m_pointInfos = OtherShapeHelper::RotateOtherShapePointInfos(m_pointInfos,map_one_matrial["��ת"],map_one_matrial["���ϳ���"],map_one_matrial["���Ͽ��"]);

				map_one_matrial["���ε���Ϣ"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos);
#endif
			}
			else	//ɶ��û������outlineΪ������
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				map_one_matrial["����ID"] = "��0";
				map_one_matrial["���ε���Ϣ"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(
					OtherShapeHelper::getPointInfoRectangle(
					map_one_matrial["��ת"],
					map_one_matrial["���ϳ���"],
					map_one_matrial["���Ͽ��"])
					);
#endif
			}
		}

		////�������ײ۷�ת
		//{
		//	//��Ҫ���пײ���ת
		//	if (map_one_matrial["�ײ���ת"].compare("1") == 0 )
		//	{
		//		//map_one_matrial["�����1"] = ExchangeVerticalXY(map_one_matrial["�����1"], fComponentYLen);
		//		//map_one_matrial["�����1"] = ExchangeVerticalXY(map_one_matrial["�����1"], fComponentYLen);
		//		//map_one_matrial["�����1"] = ExchangeSlotXY(map_one_matrial["�����1"], fComponentYLen);
		//		//map_one_matrial["�����1"] = ExchangeSlotXY(map_one_matrial["�����1"], fComponentYLen);
		//		map_one_matrial["��·����"] = "������";
		//	}
		//}

		//��ת���� ����������Ϣ��ת=1��
		if (i_comchange == 1)
		{
			//��������Ϣ
			if (map_one_matrial["�����1"] != "" || map_one_matrial["�����1"] != "")
			{
				//����������Ϣ
				if (map_one_matrial["�����1"] == "" && map_one_matrial["�����1"] == "")
				{
					//��ת
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//��ת���ã�������ȷ�ת=2��
		else if (i_comchange == 2)
		{
			//��������Ϊ��
			if (map_one_matrial["�����1"] == "")
			{
				//�ҷ���ײ�Ϊ��
				if (map_one_matrial["�����1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
				//�������Ϊ�գ�����۲���
				else if (map_one_matrial["�����1"] == "" && map_one_matrial["�����1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//��ת���ã��������ȷ�ת=3��
		else if (i_comchange == 3)
		{
			//��������Ϊ��
			if (map_one_matrial["�����1"] == "")
			{
				//�ҷ���۲�Ϊ��
				if (map_one_matrial["�����1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
				//�������Ϊ�գ�����ײ���
				else if (map_one_matrial["�����1"] == "" && map_one_matrial["�����1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//������������������ʱ��ת
		else if (i_comchange == 4)
		{
			//������� < �����������ת
			if (index0_vertical < index5_vertical)
			{
				TurnOverVecSlot(map_one_matrial);
			}
		}

		int knifenum = atoi(map_one_matrial["��������"].c_str()) * i_comnum;
		CString str_knifenum;
		str_knifenum.Format("%d",knifenum);
		map_one_matrial["��������"] = str_knifenum;

		// ������
		if (map_one_matrial.size() > 0)
		{
			matrial_info.push_back(map_one_matrial);
		}
	}

	return matrial_info;
}

/*---------------------------------------*/
//	����˵����
//		���ص���������Ϣ
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
void HgmReadWrite::LoadSettings_KnifeType()
{

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hgm�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szSuffix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

	// �����ļ�
	decrypt_base64(knifeType_hge.GetBuffer(), knifeType_xml.GetBuffer());


#else

	// hgm�ļ�·����xml�ļ�·��
	CString knifeType_hgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hgm);

	// �����ļ�
	//HGTools::decryptFile(knifeType_hgm, knifeType_xml);

#endif

	

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.LoadFile(knifeType_xml);



	HGTools::deleteFile(knifeType_xml);
}

/*---------------------------------------*/
//	����˵����
//		���浶��������Ϣ
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
void HgmReadWrite::SaveSettings_KnifeType()
{

#if (NEW_ENCRYPT_BASE64 == 1)

	//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szSuffix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.SaveFile(knifeType_xml);

	encrypt_base64(knifeType_xml.GetBuffer(), knifeType_hge.GetBuffer());
	DeleteFile(knifeType_xml);
	//HGTools::encryptFile(knifeType_xml,knifeType_hge);
	//HGTools::deleteFile(knifeType_xml);


#else

	//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
	CString knifeType_hgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hgm);

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.SaveFile(knifeType_xml);


	//HGTools::encryptFile(knifeType_xml,knifeType_hgm);
	HGTools::deleteFile(knifeType_xml);

#endif



}