#include "stdafx.h"
#include "OtherShapeHelper.h"
#include "../Misc/YHInfoTools.h"
#include <algorithm>
#include "HGTools.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "EncryptionInterface.h"
#include "Misc.h"
#include "ProgramMisc.h"


bool OtherShapeHelper::isLoadPanelOutlineList = false;
TinyXml::TiXmlDocument OtherShapeHelper::docPanelOutlineList;


struct OutlineLine
{
	void FrontToBack();
	float m_fStartX;
	float m_fStartY;
	float m_fEndX;
	float m_fEndY;

	int m_nLineType;
	float m_fR;
	int m_nDir;
};

void OutlineLine::FrontToBack()
{
	float fTmpX = m_fStartX;
	float fTmpY = m_fStartY;
	m_fStartX = m_fEndX;
	m_fStartY = m_fEndY;
	m_fEndX = fTmpX;
	m_fEndY = fTmpY;
	if(m_nDir == ANTI_CLOCKWISE)
		m_nDir = CLOCKWISE;
	else if(m_nDir == CLOCKWISE)
		m_nDir = ANTI_CLOCKWISE;
}


struct OutlineLine_Machining
{
	OutlineLine m_theLine;
	int m_nSide;
	int m_nType;
	int m_nCut;
};

OtherShapeHelper::OtherShapeHelper()
{

}


// �������PointInfo 
// x:50.0,y:30.0;  => {[ x:50.0 , y:30.0 ]}
string OtherShapeHelper::GetPointInfoStringFromPointInfo( vector<PointInfo> PointInfos )
{
	CString result;
	for (int i = 0; i < PointInfos.size() ; i++)
	{
		CString formater;
		formater.Format("X:%f,Y:%f,R:%f,Sign:%d,Dir:%d,Cut:%d,Group:%d,Side:%d,type:%d;",
			PointInfos[i].x,
			PointInfos[i].y,
			PointInfos[i].r,
			PointInfos[i].sign,
			PointInfos[i].dir,
			PointInfos[i].cut,
			PointInfos[i].group,
			PointInfos[i].side,
			PointInfos[i].type
			);
		result = result + formater;
	}
	return result.GetBuffer();
}

//ͨ������������
std::vector<PointInfo> OtherShapeHelper::getPointInfoRectangle(const std::string rotate, std::string& panelWidth,std::string& panelHeight )
{
	std::vector<PointInfo> result;
	result.push_back(GenerateOnePointInfo("0",panelHeight,"0","0","0","0","1","1"));
	result.push_back(GenerateOnePointInfo("0","0","0","0","0","0","0","1"));
	result.push_back(GenerateOnePointInfo(panelWidth,"0","0","0","0","0","0","1"));
	result.push_back(GenerateOnePointInfo(panelWidth,panelHeight,"0","0","0","0","0","1"));
	result.push_back(GenerateOnePointInfo("0",panelHeight,"0","0","0","0","2","1"));

	return result;
}

/************************************************************************/
/* �����                                                               */
/************************************************************************/

PointInfo OtherShapeHelper::GenerateOnePointInfo( std::string str_x,std::string str_y,std::string str_r,std::string str_sign,std::string str_dir,std::string str_cut,std::string str_group,std::string str_side )
{
	PointInfo info;
	info.x = atof(str_x.c_str());
	info.y = atof(str_y.c_str());
	info.r = atof(str_r.c_str());
	info.sign = atoi(str_sign.c_str());
	info.dir = atoi(str_dir.c_str());
	info.cut = atoi(str_cut.c_str());
	info.group = atoi(str_group.c_str());
	info.side = atoi(str_side.c_str());
	return info;
}

//��ת�������ı�������
std::vector<PointInfo> OtherShapeHelper::getPointInfoRotate( const std::vector<PointInfo> pointInfos,const std::string rotate , std::string m_panelwidth,std::string m_panelheight )
{
	std::vector<PointInfo> m_pointInfos = pointInfos;
	float fOrgXLen, fOrgYLen;
	switch (atoi(rotate.c_str()))
	{
	case 2:	//��ʱ��90
		fOrgXLen = atof(m_panelheight.c_str());
		fOrgYLen = atof(m_panelwidth.c_str());
		m_pointInfos = OtherShapeHelper::RotatePointInfoBackDegree90(m_pointInfos, fOrgXLen, fOrgYLen);
		break;
	case 3:	//˳ʱ��180
		fOrgXLen = atof(m_panelwidth.c_str());
		fOrgYLen = atof(m_panelheight.c_str());
		m_pointInfos = OtherShapeHelper::RotatePointInfoDegree180(m_pointInfos, fOrgXLen, fOrgYLen);
		break;
	case 1:	//˳ʱ��90 
		fOrgXLen = atof(m_panelheight.c_str());
		fOrgYLen = atof(m_panelwidth.c_str());
		m_pointInfos = OtherShapeHelper::RotatePointInfoDegree90(m_pointInfos, fOrgXLen, fOrgYLen);
		break;
	case 0:

		break;//��ת
	default:
		break;
	}
	return m_pointInfos;
}

//���ڶ�����ת90��
std::vector<PointInfo> OtherShapeHelper::RotatePointInfoBackDegree90( std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen)
{
	for (int i = 0;i<pointInfos.size();i++)
	{
		float tempx = fOrgYLen - pointInfos[i].y;
		float tempy = pointInfos[i].x;
		pointInfos[i].x = tempx;
		pointInfos[i].y = tempy;
	}
	return pointInfos;
}

//���ڶ�����ת90��
std::vector<PointInfo> OtherShapeHelper::RotatePointInfoDegree90( std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen)
{
	for (int i = 0;i<pointInfos.size();i++)
	{
		float tempx = pointInfos[i].y;
		float tempy = fOrgXLen - pointInfos[i].x;
		pointInfos[i].x = tempx;
		pointInfos[i].y = tempy;
	}
	return pointInfos;
}

std::vector<PointInfo> OtherShapeHelper::RotatePointInfoDegree180( std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen)
{
	for (int i = 0;i<pointInfos.size();i++)
	{
		float tempx = fOrgXLen - pointInfos[i].x;
		float tempy = fOrgYLen - pointInfos[i].y;
		pointInfos[i].x = tempx;
		pointInfos[i].y = tempy;
	}
	return pointInfos;
}

// ����string �����б�
// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , gapWidth : 10 , gapHeight : 20 }
std::vector<param_point> OtherShapeHelper::GetParamPointsFromOtherShapeString( std::string otherShapeString )
{
	vector<param_point> pa_p;
	std::string str_info = otherShapeString;
	std::string str = "";
	std::string str_name;
	std::string str_value;
	for (int i = 0;i < str_info.size();i++)
	{
		char ch = str_info.at(i);
		if(ch == ':')
		{
			str_name = str;
			str = "";
		}
		else if (ch == ';')
		{
			str_value = str;
			str = "";
			param_point p;

			if (str_name == "" || str_value == "")
			{
				//TODO: �쳣����
				ASSERT(0);
				throw std::logic_error("unknown empty strname strvalue.");
				return vector<param_point>();
			}
			p._name = str_name;
			p._value = str_value;
			pa_p.push_back(p);			//�������б�
		}
		else
		{
			str = str + ch;
		}
	}
	sortParam_pointByStrLen(pa_p);
	return pa_p;
}

// ����string �����б�
// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , PW : 50 , PD : 50 , gapWidth : 10 , gapHeight : 20 }
std::vector<param_point> OtherShapeHelper::GetParamPointsFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight )
{
	std::vector<param_point> pa_p = GetParamPointsFromOtherShapeString(otherShapeString);
	if (!isParamPointHasPanelWidth(pa_p))
	{
		PushParamPoint(pa_p,"PW",panelWidth);
	}
	else
	{
		SetParamPoint(pa_p,"PW",panelWidth);
	}
	if (!isParamPointHasPanelHeight(pa_p))
	{
		PushParamPoint(pa_p,"PD",panelHeight);
	}
	else
	{
		SetParamPoint(pa_p,"PD",panelHeight);
	}
	sortParam_pointByStrLen(pa_p);
	return pa_p;
}

//���ݳ������������������, �������ճ��ȣ�������
void OtherShapeHelper::sortParam_pointByStrLen(std::vector<param_point> & pa_p) 
{
	std::sort(pa_p.begin(),pa_p.end(),sortFunctionByStrLen);
}

//���ݳ������������������
bool OtherShapeHelper::sortFunctionByStrLen(const param_point & m1, const param_point & m2) 
{
	return m1._name.length() > m2._name.length();
}

//���β����б�PW�ж�
bool OtherShapeHelper::isParamPointHasPanelWidth( const std::vector<param_point>& pa_p )
{
	for (int i = 0; i < pa_p.size() ; i++)
	{
		if (pa_p[i]._name.compare("PW") == 0)
		{
			return true;
		}
	}
	return false;
}

//���β����б�PD�ж�
bool OtherShapeHelper::isParamPointHasPanelHeight( const std::vector<param_point>& pa_p )
{
	for (int i = 0; i < pa_p.size() ; i++)
	{
		if (pa_p[i]._name.compare("PD") == 0)
		{
			return true;
		}
	}
	return false;
}

//���β����б� ��������
void OtherShapeHelper::PushParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value )
{
	param_point pa;
	pa._name = name;
	pa._value = value;
	pa_p.push_back(pa);

}

//���β����б� ��������
void OtherShapeHelper::SetParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value )
{
	for (int i = 0;i < pa_p.size();i++)
	{
		if(pa_p[i]._name.compare(name) == 0)
		{
			pa_p[i]._value = value;
			break;
		}
	}
}

//���β����б� ��ȡ����
std::string OtherShapeHelper::GetParamPoint( std::vector<param_point>& pa_p, std::string name )
{
	for (int i = 0;i < pa_p.size();i++)
	{
		if(pa_p[i]._name.compare(name) == 0)
		{
			return pa_p[i]._value;
		}
	}
	return "";
}

//��������ר�� �����б���ת
void OtherShapeHelper::RotateOtherShapeParamPoint(std::vector<param_point>& pa_p,std::string rotate)
{
	std::string panelwidth = OtherShapeHelper::GetParamPoint(pa_p,"PW");
	std::string panelheight = OtherShapeHelper::GetParamPoint(pa_p,"PD");
	int m_changetype = atoi(rotate.c_str());
	switch (m_changetype)
	{
	case 2:	//��ʱ��90
		OtherShapeHelper::SetParamPoint(pa_p,"PW",panelheight);
		OtherShapeHelper::SetParamPoint(pa_p,"PD",panelwidth);
		//std::swap(m_panelwidth,m_panelheight);
		break;
	case 3:	//˳ʱ��180
		//��ת
		break;
	case 1:	//˳ʱ��90
		OtherShapeHelper::SetParamPoint(pa_p,"PW",panelheight);
		OtherShapeHelper::SetParamPoint(pa_p,"PD",panelwidth);
		//std::swap(m_panelwidth,m_panelheight);
		break;
	case 0:
		break;//��ת
	default:
		break;
	}
}

// ������string�������滻�������β������󣬼������������ͨ����
// ID:2016;PW:50;PD:60;gapWidth:10;gapHeight:20; + {[x:"PD-gapWidth",y:"PW-gapHeight"]} => {[ x:50.0 , y:30.0 ]}
std::vector<PointInfo> OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint( std::vector<std::map<std::string,std::string>> panelOutline, std::vector<param_point> pa_p )
{
	OtherShapeHelper::sortParam_pointByStrLen(pa_p);
	//����outline
	std::vector<PointInfo> m_pointInfos;
	for (int i = 0;i<panelOutline.size();i++)
	{
		//���滻��������
		for (int k = 0;k < pa_p.size();k++)
		{
			std::string p_name = pa_p.at(k)._name;
			std::string p_value = pa_p.at(k)._value;

			panelOutline[i]["X"] = YHInfoTools::replace_allstr(panelOutline[i]["X"],p_name,p_value);
			panelOutline[i]["Y"] = YHInfoTools::replace_allstr(panelOutline[i]["Y"],p_name,p_value);
			panelOutline[i]["R"] = YHInfoTools::replace_allstr(panelOutline[i]["R"],p_name,p_value);
			panelOutline[i]["Sign"] = YHInfoTools::replace_allstr(panelOutline[i]["Sign"],p_name,p_value);
			panelOutline[i]["Dir"] = YHInfoTools::replace_allstr(panelOutline[i]["Dir"],p_name,p_value);
			panelOutline[i]["Cut"] = YHInfoTools::replace_allstr(panelOutline[i]["Cut"],p_name,p_value);
			panelOutline[i]["type"] = YHInfoTools::replace_allstr(panelOutline[i]["type"],p_name,p_value);
			panelOutline[i]["Group"] = YHInfoTools::replace_allstr(panelOutline[i]["Group"],p_name,p_value);
			panelOutline[i]["Side"] = YHInfoTools::replace_allstr(panelOutline[i]["Side"],p_name,p_value);
		}

		//hg3d::Construction cst;

		//���������ݽ�����������
		if (panelOutline[i]["Sign"].compare("-1") != 0)
		{
			PointInfo pointInfo;
			pointInfo.x = KnifeClass::calcFormula(panelOutline[i]["X"]);
			pointInfo.y = KnifeClass::calcFormula(panelOutline[i]["Y"]);
			pointInfo.r = KnifeClass::calcFormula(panelOutline[i]["R"]);
			pointInfo.sign = atoi(panelOutline[i]["Sign"].c_str());
			pointInfo.dir = atoi(panelOutline[i]["Dir"].c_str());
			pointInfo.cut = atoi(panelOutline[i]["Cut"].c_str());
			pointInfo.group = atoi(panelOutline[i]["Group"].c_str());
			pointInfo.side = atoi(panelOutline[i]["Side"].c_str());
			m_pointInfos.push_back(pointInfo);
		}
	}
	return m_pointInfos;
}

// ��ù������β�������
// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
std::vector<std::map<std::string,std::string>> OtherShapeHelper::GetOtherShapePanelOutLineListFromOtherShapeID( std::string otherShapeID )
{
	//�㼯��
	std::vector<std::map<std::string,std::string>> result;

	if (!isLoadPanelOutlineList)
	{
		isLoadPanelOutlineList = true;



		// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

		// hge�ļ�·����xml�ļ�·��
		CString strTmp;
		strTmp.Format(_T("kdata\\PanelOutlineList%s"), g_szSuffix);
		CString hgePath = HGTools::getRelativePath(strTmp);
		CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

		// �����ļ�
		decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

		//�����ļ�·��
		CString hgmPath = HGTools::getRelativePath("kdata\\PanelOutlineList.hgm");
		CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

		//����
		//HGTools::decryptFile(hgmPath,xmlPath);

#endif





		docPanelOutlineList.LoadFile(xmlPath);

		//�ر��ļ�
		HGTools::deleteFile(xmlPath);
	}

	//���ظ���
	//TinyXml::XmlHandlePlus docHandler(&docPanelOutlineList);
	//std::vector<TinyXml::TiXmlNode*> PanelOutlineArr = docHandler.findAll("/PanelOutlineList/PanelOutline",TinyXml::XmlAttrValueCompare("StyleId",otherShapeID));
	TiXmlElement* elmRoot = docPanelOutlineList.RootElement();
	XmlHandler xmlHandler;
	TiXmlElement* elmThePanelOutline = xmlHandler.GetIndicatedElement(elmRoot, "PanelOutline", "StyleId", otherShapeID.c_str());
	//if (PanelOutlineArr.size() != 1)
	if(elmThePanelOutline  == NULL)
	{
		return result;
	}


	//��������vector�ڵ�
	//TinyXml::XmlHandlePlus PanelOutlineHandler(PanelOutlineArr[0]);
	//std::vector<TinyXml::TiXmlNode*> VectorArr = PanelOutlineHandler.findAll("Vector");
	vector<TiXmlElement*> vVector = xmlHandler.GetChildElms(elmThePanelOutline, "Vector");
	//for (int i = 0;i<VectorArr.size();i++)
	for (int i = 0;i<vVector.size();i++)
	{
		std::map<std::string,std::string> strPointInfo;
		//TinyXml::XmlHandlePlus vectorHandler(VectorArr[i]);
		TiXmlElement* elmVector = vVector[i];

		//std::string str_x = vectorHandler.getAttr("X","0");
		//std::string str_y = vectorHandler.getAttr("Y","0");
		//std::string str_r = vectorHandler.getAttr("R","0");
		//std::string str_sign = vectorHandler.getAttr("Sign","0");
		//std::string str_dir = vectorHandler.getAttr("Dir","0");
		//std::string str_cut = vectorHandler.getAttr("Cut","0");
		//std::string str_type = vectorHandler.getAttr("type","0");
		//std::string str_group = vectorHandler.getAttr("Group","0");
		//std::string str_side = vectorHandler.getAttr("Side","0");
		CString strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "X", strTmp);
		std::string str_x = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Y", strTmp);
		std::string str_y = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "R", strTmp);
		std::string str_r = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Sign", strTmp);
		std::string str_sign = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Dir", strTmp);
		std::string str_dir = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Cut", strTmp);
		std::string str_cut = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "type", strTmp);
		std::string str_type = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Group", strTmp);
		std::string str_group = strTmp;
		strTmp = "0";
		xmlHandler.GetXmlAttribute(elmVector, "Side", strTmp);
		std::string str_side = strTmp;

		strPointInfo["X"] = str_x;
		strPointInfo["Y"] = "(" + str_y + ")";////////////////////////////////////////////////////////////////
		strPointInfo["R"] = str_r;
		strPointInfo["Sign"] = str_sign;
		strPointInfo["Dir"] = str_dir;
		strPointInfo["Cut"] = str_cut;
		strPointInfo["type"] = str_type;
		strPointInfo["Group"] = str_group;
		strPointInfo["Side"] = str_side;

		result.push_back(strPointInfo);
	}


	return result;
}

//����string�л�ȡ ID����
std::string OtherShapeHelper::GetOtherShapeId( const std::string& otherShapeID )
{
	std::string m_stypeid;
	std::vector<param_point> pa_p = GetParamPointsFromOtherShapeString(otherShapeID);
	for (int i = 0;i < pa_p.size();i++)
	{
		std::string p_id = "ID";
		if (pa_p[i]._name.compare(p_id) == 0)
		{
			m_stypeid = pa_p[i]._value;
			return m_stypeid;
		}
	}
	throw std::logic_error("shape id is empty");
	return "";
}

//���β����б��л�ȡ ID����
std::string OtherShapeHelper::GetOtherShapeId( const std::vector<param_point>& pa_p )
{
	std::string m_stypeid;
	for (int i = 0;i < pa_p.size();i++)
	{
		std::string p_id = "ID";
		if (pa_p[i]._name.compare(p_id) == 0)
		{
			m_stypeid = pa_p[i]._value;
			return m_stypeid;
		}
	}
	throw std::logic_error("shape id is empty");
	return "";
}

//��������ר�� ������ת
std::vector<PointInfo> OtherShapeHelper::RotateOtherShapePointInfos(std::vector<PointInfo> m_pointInfos,std::string changeType ,std::string m_panelwidth, std::string m_panelheight)
{
	int m_changetype = atoi(changeType.c_str());

	float fOrgXLen = atof(m_panelwidth.c_str());
	float fOrgYLen = atof(m_panelheight.c_str());
	switch (m_changetype)
	{
	case 1:	//˳ʱ��90
	case 2:	//��ʱ��90
		fOrgXLen = atof(m_panelheight.c_str());
		fOrgYLen = atof(m_panelwidth.c_str());
		break;

	}

	switch (m_changetype)
	{
	case 2:	//��ʱ��90
		m_pointInfos = OtherShapeHelper::RotatePointInfoBackDegree90(m_pointInfos, fOrgXLen, fOrgYLen);
		std::swap(m_panelwidth,m_panelheight);
		break;

	case 3:	//˳ʱ��180
		m_pointInfos = OtherShapeHelper::RotatePointInfoDegree180(m_pointInfos, fOrgXLen, fOrgYLen);
		break;

	case 1:	//˳ʱ��90
		m_pointInfos = OtherShapeHelper::RotatePointInfoDegree90(m_pointInfos, fOrgXLen, fOrgYLen);
		std::swap(m_panelwidth,m_panelheight);
		break;

	case 0:
		break;//��ת
	default:
		break;
	}
	return m_pointInfos;
}

// �������PointInfo 
//  {[ x:50.0 , y:30.0 ]} => x:50.0,y:30.0;
std::vector<PointInfo> OtherShapeHelper::GetPointInfoFromPointInfoString( std::string strPointInfo )
{
	std::vector<PointInfo> pointInfos;
	//X:100,Y:200;X:500,Y:700;
	std::vector<std::string> stringArr;
	TinyXml::XmlHandlePlus::split(strPointInfo,";",stringArr);
	for (int i = 0; i < stringArr.size() ; i++)
	{
		std::map<string,string> paramitem_map;

		if (stringArr[i].compare("") == 0)
		{
			continue;
		}

		//X:100,Y:200; //X:500,Y:700;
		std::vector<std::string> paramlist;
		TinyXml::XmlHandlePlus::split(stringArr[i],",",paramlist);
		for (int j = 0; j < paramlist.size() ; j++)
		{
			//X:100 //Y:200
			std::vector<std::string> parmitem;
			TinyXml::XmlHandlePlus::split(paramlist[j],":",parmitem);
			for (int k = 0; k < parmitem.size() ; k++)
			{
				//X //100
				paramitem_map[parmitem[0]] = parmitem[1];
			}
		}

		PointInfo info;
		info.x = atof(paramitem_map["X"].c_str());
		info.y = atof(paramitem_map["Y"].c_str());
		info.r = atof(paramitem_map["R"].c_str());
		info.sign = atoi(paramitem_map["Sign"].c_str());
		info.dir = atoi(paramitem_map["Dir"].c_str());
		info.cut = atoi(paramitem_map["Cut"].c_str());
		info.group = atoi(paramitem_map["Group"].c_str());
		info.side = atoi(paramitem_map["Side"].c_str());
		info.type = atoi(paramitem_map["type"].c_str());
		pointInfos.push_back(info);
	}
	return pointInfos;
}

/************************************************************************/
/* ���ε㴦��                                                           */
/************************************************************************/
//�ѵ�ӵ��������Ƶ���һ����
void OtherShapeHelper::GetPointsToYPlus(std::vector<PointInfo>& pointInfos, const std::string rotate, std::string m_panelwidth, std::string m_panelheight)
{
	float fOrgYLen = atof(m_panelheight.c_str());;

	int nRotateType = atoi(rotate.c_str());
	switch (nRotateType)
	{
	case 2:	//��ʱ��90
		fOrgYLen = atof(m_panelwidth.c_str());
		break;
	case 3:	//˳ʱ��180
		break;
	case 1:	//˳ʱ��90
		fOrgYLen = atof(m_panelwidth.c_str());
		break;
	case 0:
		break;//��ת
	default:
		break;
	}

	for (int i = 0;i<pointInfos.size();i++)
	{
		pointInfos[i].y = pointInfos[i].y + fOrgYLen;
	}
}

//��÷�ת�����������ҷ�ת��
std::vector<PointInfo> OtherShapeHelper::getPointInfoBack( const std::vector<PointInfo> pointInfos,const std::string& panelWidth,const std::string& panelHeight )
{
	//////////////////////////////////////////////////////////////////////////
	std::vector<PointInfo> backInfo = pointInfos;
	//backInfo = OtherShapeHelper::MirrorPointInfoWithX(backInfo);
	//backInfo = OtherShapeHelper::MovePointInfo(backInfo,OtherPointFloat(atof(panelWidth.c_str()),0));
	//backInfo = OtherShapeHelper::GetPointInfoReverse(backInfo);
	for (int i = 0;i<backInfo.size();i++)
	{
		backInfo[i].x = atof(panelWidth.c_str()) - backInfo[i].x;
		if (backInfo[i].dir == 1)
		{
			backInfo[i].dir = 2;
		}
		else if (backInfo[i].dir == 2)
		{
			backInfo[i].dir = 1;
		}
	}

	backInfo = OtherShapeHelper::GetPointInfoReverse(backInfo);

	return backInfo;
}

//���ĵ��˳��
std::vector<PointInfo> OtherShapeHelper::GetPointInfoReverse( const std::vector<PointInfo> pointInfos )
{
	std::vector<PointInfo> result;

	if(pointInfos.size() <= 2)
		return result;

	//������ȡ������Ϣ
	std::vector<std::vector<PointInfo>> divideByGroup;
	std::vector<PointInfo> tempDivideByGroup;
	for (int i = 0 ; i < pointInfos.size() ; i++)
	{
		if(pointInfos[i].group == 1)
		{
			if(tempDivideByGroup.empty() == false)//��һ���߶�û�н���
			{
				throw std::logic_error("��һ���߶�û�н���");
			}
			tempDivideByGroup.push_back(pointInfos[i]);
		}
		else if(pointInfos[i].group == 2)
		{
			//����һ����
			tempDivideByGroup.push_back(pointInfos[i]);

			divideByGroup.push_back(tempDivideByGroup);
			tempDivideByGroup.clear();
		}
		else
		{
			tempDivideByGroup.push_back(pointInfos[i]);
		}
	}
	if (tempDivideByGroup.size() > 0)	//������ƣ����û�н������������
	{
		divideByGroup.push_back(tempDivideByGroup);
		tempDivideByGroup.clear();
	}

	vector<vector<OutlineLine_Machining>> vOutlineLineGroup;
	for (int i = 0 ; i < divideByGroup.size() ; i++)
	{
		vector<PointInfo>& thePointGroup = divideByGroup[i];
		vector<OutlineLine_Machining> vOneGroupOutline;
		int nPointCount = thePointGroup.size();
		for(int j = 0; j < nPointCount-1; j++)
		{
			PointInfo& theStartPoint = thePointGroup[j];
			PointInfo& theEndPoint = thePointGroup[j+1];
			OutlineLine_Machining line;
			line.m_theLine.m_fStartX = theStartPoint.x;
			line.m_theLine.m_fStartY = theStartPoint.y;
			line.m_theLine.m_fEndX = theEndPoint.x;
			line.m_theLine.m_fEndY = theEndPoint.y;
			line.m_theLine.m_fR = theEndPoint.r;
			line.m_theLine.m_nDir = theEndPoint.dir;
			line.m_theLine.m_nLineType = GetLineType(theStartPoint.sign);
			//line.m_nCut = theStartPoint.cut;
			line.m_nSide = theStartPoint.side;
			line.m_nType = theStartPoint.type;
			vOneGroupOutline.push_back(line);
		}
		vOutlineLineGroup.push_back(vOneGroupOutline);
	}

	for(int i = 0; i < vOutlineLineGroup.size(); i++)
	{
		vector<OutlineLine_Machining>& vOneGroupOutline = vOutlineLineGroup[i];
		int nLineCount = vOneGroupOutline.size();
		for(int j = nLineCount-1; j >= 0; j--)
		{
			OutlineLine_Machining theLine = vOneGroupOutline[j];
			theLine.m_theLine.FrontToBack();
			if(j == nLineCount-1)
			{
				PointInfo pointStart;
				pointStart.x = theLine.m_theLine.m_fStartX;
				pointStart.y = theLine.m_theLine.m_fStartY;
				pointStart.sign = theLine.m_theLine.m_nLineType == LineType::CIRCULAR_ARC ? 1 : 0;
				pointStart.group = OutlineGroupType::OUTLINE_GROUP_TYPE_START;
				pointStart.cut = (i == 0) ? 0 : 2;
				pointStart.side = theLine.m_nSide;
				pointStart.type = theLine.m_nType;
				result.push_back(pointStart);
			}

			PointInfo thePoint;
			thePoint.x = theLine.m_theLine.m_fEndX;
			thePoint.y = theLine.m_theLine.m_fEndY;
			thePoint.r = theLine.m_theLine.m_fR;
			thePoint.dir = theLine.m_theLine.m_nDir;
			thePoint.group = (j == 0) ? OutlineGroupType::OUTLINE_GROUP_TYPE_END : OutlineGroupType::OUTLINE_GROUP_TYPE_LINK;
			thePoint.cut = ((j == 0) && (i+1 != vOutlineLineGroup.size())) ? 1 : 0;
			if(j > 0)
			{
				OutlineLine_Machining theLineNext = vOneGroupOutline[j-1];
				thePoint.sign = GetLineLinkingTypeByLineType((LineType)(theLine.m_theLine.m_nLineType), (LineType)(theLineNext.m_theLine.m_nLineType));
				thePoint.side = theLineNext.m_nSide;
				thePoint.type = theLineNext.m_nType;
			}
			else
			{
				thePoint.sign = GetLineLinkingTypeByLineType((LineType)(theLine.m_theLine.m_nLineType), LineType::STRAIGHT_LINE);
				//thePoint.side = theLineNext.m_nSide;
				//thePoint.type = theLineNext.m_nType;
			}

			result.push_back(thePoint);
		}
	}

	return result;


	//std::vector<PointInfo> result;

	////������ȡ������Ϣ
	//std::vector<std::vector<PointInfo>> divideByGroup;
	//std::vector<PointInfo> tempDivideByGroup;
	//for (int i = 0 ; i < pointInfos.size() ; i++)
	//{
	//	if(pointInfos[i].group == 1)
	//	{
	//		if(tempDivideByGroup.empty() == false)//��һ���߶�û�н���
	//		{
	//			throw std::logic_error("��һ���߶�û�н���");
	//		}
	//		tempDivideByGroup.push_back(pointInfos[i]);
	//	}
	//	else if(pointInfos[i].group == 2)
	//	{
	//		//����һ����
	//		tempDivideByGroup.push_back(pointInfos[i]);

	//		divideByGroup.push_back(tempDivideByGroup);
	//		tempDivideByGroup.clear();
	//	}
	//	else
	//	{
	//		tempDivideByGroup.push_back(pointInfos[i]);
	//	}
	//}
	//if (tempDivideByGroup.size() > 0)	//������ƣ����û�н������������
	//{
	//	divideByGroup.push_back(tempDivideByGroup);
	//	tempDivideByGroup.clear();
	//}

	////���鷭ת������Ϣ
	//std::vector<PointInfo> backpoints;
	//for (int i = 0; i < divideByGroup.size() ; i++)
	//{
	//	for (auto it = divideByGroup[i].rbegin();it != divideByGroup[i].rend();it++)
	//	{
	//		backpoints.push_back(*it);
	//	}
	//}

	////��ת������´���
	//result = backpoints;
	//for (int i = 0 ; i < result.size() ; i++)
	//{
	//	//����ת����ԭ����һ�鿪ʼ�ĵ㣬ת����Ϊһ������ĵ�
	//	if (result[i].group == 1)
	//	{
	//		result[i].group = 2;
	//	}
	//	else if (result[i].group == 2)
	//	{
	//		result[i].group = 1;
	//	}

	//	//Բ������ԭ�����⣬��ת��Բ���򷴷���
	//	if (result[i].dir == 1)
	//	{
	//		result[i].dir = 2;
	//	}
	//	else if (result[i].dir == 2)
	//	{
	//		result[i].dir = 1;
	//	}

	//	//Բ����ʼ����յ㣬�յ����ʼ�㡣Բ���Ĵ�С�ɽ�������ƣ����Զ���������ʱ���м��ȡ��ǰ��һ�����ֵ
	//	if (result[i].sign == 1)
	//	{
	//		result[i].sign = 2;
	//	}
	//	else if (result[i].sign == 2)
	//	{
	//		result[i].sign = 1;
	//	}
	//	else if (result[i].sign == 3)
	//	{
	//		//ȡ��һ������
	//		result[i].dir = result[i-1].dir;
	//		result[i].r = backpoints[i-1].r;
	//	}

	//	//���η������䵶����ȫ����գ������һ��������һ����������ĵ㣬����һ������Ҫ�𵶣��������Ҫ�µ�
	//	result[i].cut = 0;
	//	if (i > 0)
	//	{
	//		if (result[i-1].group == 2)
	//		{
	//			result[i-1].cut = 1;
	//			result[i].cut = 2;
	//		}
	//	}

	//}
	//return result;
}