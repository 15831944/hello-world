#include "stdafx.h"
#include "OtherShapeHelper.h"
#include "XmlHandlePlus.h"
#include <map>
//#include "../../../include/Encryption/base64/base64.h"
#include "EncryptionInterface.h"

#include "YHInfoTools.h"
#include <algorithm>
#include "HGTools.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
//#include "EncryptionInterface.h"
#include "Misc.h"
#include "../Work/WorkDef.h"
#include "ProgramMisc.h"

#include "../../../include/AbnormalShapeManager/DataManager/Figure/Complex.h"
#include "../../../include/AbnormalShapeManager/DataManager/Point/ShapePoint.h"
#include "../../../include/AbnormalShapeManager/DataManager/Figure/Figure.h"
#include "../../../include/AbnormalShapeManager/DataManager/CommonData.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/

InfoLine::InfoLine()
{
	begin.x = 0.0;
	begin.y = 0.0;
	begin.r = 0.0;
	begin.sign  = 0;
	begin.side = 0;
	begin.cut = 0;
	begin.dir = 0;
	begin.group = 1;
	begin.type = 0;

	OtherShapeHelper::clonePointInfo(end,begin);
	r = 0.0;
	dir = 0;
	groupID = 0;
	side = 3;
	visable = false;
}

InfoLine::InfoLine(IN const InfoLine& _line)
{
	OtherShapeHelper::clonePointInfo(begin,_line.begin);
	OtherShapeHelper::clonePointInfo(end,_line.end);
	r = _line.r;
	dir = _line.dir;
	groupID = _line.groupID;
	side = _line.side;
	visable = _line.visable;
}

InfoLine::InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end)
{
	OtherShapeHelper::clonePointInfo(begin,_begin);
	OtherShapeHelper::clonePointInfo(end,_end);
	r = 0.0;
	dir = 0;
	groupID = 0;
	side = 3;
	visable = false;
}

InfoLine::InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end, float _r, int _dir,int _groupID,int _side,bool _visable)
{
	OtherShapeHelper::clonePointInfo(begin,_begin);
	OtherShapeHelper::clonePointInfo(end,_end);
	r = _r;
	dir = _dir;
	groupID = _groupID;
	side = _side;
	visable = _visable;
}

InfoLine InfoLine::operator=(IN const InfoLine& _line) const
{
	return InfoLine(_line);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

bool OtherShapeHelper::isLoadPanelOutlineList = false;
TinyXml::TiXmlDocument OtherShapeHelper::docPanelOutlineList;

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
		strTmp.Format(_T("kdata\\PanelOutlineList%s"), g_szEncyptSubfix);
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
	//throw std::logic_error("shape id is empty");    ///////////////////////////////////////////////////////////////////////
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

// ������й������β�������
// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
OutlineParamShapeDict OtherShapeHelper::GetOtherShapePanelOutLineList()
{
	OutlineParamShapeDict outlineResult;

	if (!isLoadPanelOutlineList)
	{
		isLoadPanelOutlineList = true;

		//�����ļ�·��
		CString strTmp;
		strTmp.Format(_T("kdata\\PanelOutlineList%s"), g_szEncyptSubfix);
		CString hgePath = HGTools::getRelativePath(strTmp);
		CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

		//����
		//HGTools::decryptFile(hgmPath,xmlPath);
		decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

		docPanelOutlineList.LoadFile(xmlPath);

		//�ر��ļ�
		HGTools::deleteFile(xmlPath);
	}

	//���ظ���
	//TinyXml::XmlHandlePlus docHandler(&docPanelOutlineList);
	//std::vector<TinyXml::TiXmlNode*> PanelOutlineArr = docHandler.findAll("/PanelOutlineList/PanelOutline");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = docPanelOutlineList.RootElement();
	vector<TiXmlElement*> vPanelOutline = xmlHandler.GetChildElms(elmRoot);

	//for (int j = 0; j < PanelOutlineArr.size() ; j++)
	for (int j = 0; j < vPanelOutline.size() ; j++)
	{
		//�㼯��
		OutlineParamShape result;
		//��������vector�ڵ�
		//TinyXml::XmlHandlePlus PanelOutlineHandler(PanelOutlineArr[j]);
		TiXmlElement* elmPanelOutline = vPanelOutline[j];

		//std::string otherShapeID = PanelOutlineHandler.getAttr("StyleId","-1");
		CString strTmp = "-1";
		xmlHandler.GetXmlAttribute(elmPanelOutline, "StyleId", strTmp);
		std::string otherShapeID = strTmp;
		
		int n_othershapeID = atoi(otherShapeID.c_str());


		//std::vector<TinyXml::TiXmlNode*> VectorArr = PanelOutlineHandler.findAll("Vector");
		vector<TiXmlElement*> vVector = xmlHandler.GetChildElms(elmPanelOutline);
		//for (int i = 0;i<VectorArr.size();i++)
		for (int i = 0;i<vVector.size();i++)
		{
			OutlineParamPoint strPointInfo;
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
			strTmp = "0";
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
			strPointInfo["Y"] = "(" + str_y + ")";//////////////////////////////////////////////////////////////////////
			strPointInfo["R"] = str_r;
			strPointInfo["Sign"] = str_sign;
			strPointInfo["Dir"] = str_dir;
			strPointInfo["Cut"] = str_cut;
			strPointInfo["type"] = str_type;
			strPointInfo["Group"] = str_group;
			strPointInfo["Side"] = str_side;

			result.push_back(strPointInfo);
		}

		outlineResult[n_othershapeID] = result;

	}

	return outlineResult;
}

// ����string ת����
// ID:2016;gapWidth:10;gapHeight:20;   =>   { [20,50,0,0,0] , [10,20,0,0,0] , [56,72,0,0,0] , [88,6,0,0,0] }
std::vector<PointInfo> OtherShapeHelper::GetPointInfoFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight )
{
	std::vector<param_point> pa_p = GetParamPointsFromOtherShapeString(otherShapeString,panelWidth,panelHeight);
	std::string m_stypeid = GetOtherShapeId(pa_p);
	return GetPointInfoFromPanelOutLineAndParamPoint(GetOtherShapePanelOutLineListFromOtherShapeID(m_stypeid), pa_p);
}

//////////////////////////////////////////////////////////////////////////
// ����string ������

//���β����б�PW PDɾ��
void OtherShapeHelper::removeParamPoint_PanelWidthPanelHeight( std::vector<param_point>& pa_p )
{
	pa_p.erase(
		std::remove_if(pa_p.begin(),pa_p.end(),[&](param_point pa){
			return (pa._name.compare("PW") == 0 || pa._name.compare("PD") == 0);
	}),
		pa_p.end()
		);
}

// �����б� ת ����string 
// { ID : 2016 , gapWidth : 10 , gapHeight : 20 }   =>   ID:2016;gapWidth:10;gapHeight:20;
std::string OtherShapeHelper::GetOtherShapeStringFromParamPoints( std::vector<param_point> pa_p )
{
	CString result;
	for (int i = 0;i < pa_p.size();i++)
	{
		CString temp;
		temp.Format("%s:%s;",pa_p[i]._name.c_str(),pa_p[i]._value.c_str());
		result += temp;
	}
	return result.GetBuffer();
}

/************************************************************************/
/* ���� �߶�                                                            */
/************************************************************************/
//ͨ�������߶�����
std::vector<InfoLine> OtherShapeHelper::GetInfoLineFromPointInfo(const std::vector<PointInfo>& pnts )
{
	std::vector<InfoLine> lines;
	InfoLine tempLine;
	int groupId = -1;
	PointInfo lineBeginPoint;	////����߶����
	for (int i = 0 ; i<pnts.size() ; i++)
	{
		if (pnts[i].group == 1)	//����߶����
		{
			groupId++;	//��ʼһ�����߶�

			clonePointInfo(tempLine.begin,pnts[i]);
			tempLine.end;	//��д��
			tempLine.r = pnts[i].r;			//�ȴ��ţ�ʵ����Բ����Ҫ�ú�һ�������
			tempLine.dir = pnts[i].dir;		//�ȴ��ţ�ʵ����Բ����Ҫ�ú�һ�������
			tempLine.groupID = groupId;
			tempLine.side = pnts[i].side;
			tempLine.visable = true;

			//�������һ�㵽��ʼ��һ����
			clonePointInfo(lineBeginPoint,pnts[i]);
		}
		else if (pnts[i].group == 2)	//����߶��յ�
		{
			if (tempLine.dir != 0)
			{
				tempLine.r = pnts[i].r;
				tempLine.dir = pnts[i].dir;
			}
			clonePointInfo(tempLine.end,pnts[i]);
			lines.push_back(tempLine);


			//�������һ�㵽��ʼ��һ��
			clonePointInfo(tempLine.begin,pnts[i]);
			if (pnts[i].sign == 2)
			{
				tempLine.r = 0;
			}
			else
			{
				tempLine.r = pnts[i].r;
			}
			tempLine.dir = pnts[i].dir;
			tempLine.groupID = groupId;
			tempLine.side = pnts[i].side;
			tempLine.visable = true;
			clonePointInfo(tempLine.end,lineBeginPoint);
			lines.push_back(tempLine);
		}
		else //pointInfos[i].group == 0	//����߶��м�
		{
			if (tempLine.dir != 0)
			{
				tempLine.r = pnts[i].r;
				tempLine.dir = pnts[i].dir;
			}
			clonePointInfo(tempLine.end,pnts[i]);
			lines.push_back(tempLine);

			clonePointInfo(tempLine.begin,pnts[i]);
			tempLine.end;	//��д��
			if (pnts[i].sign == 2)	//Բ������
			{
				tempLine.r = 0;
				tempLine.dir = 0;
				tempLine.groupID = groupId;
				tempLine.side = pnts[i].side;
				tempLine.visable = true;
			}
			else
			{
				tempLine.r = pnts[i].r;
				tempLine.dir = pnts[i].dir;
				tempLine.groupID = groupId;
				tempLine.side = pnts[i].side;
				tempLine.visable = true;
			}
		}
	}

	return lines;
}

//���Ƶ���Ϣ
void OtherShapeHelper::clonePointInfo( PointInfo& newPointInfo,const PointInfo& oldPointInfo )
{
	newPointInfo.x = oldPointInfo.x;	// x���꣬��������������ΰ�ԭ������
	newPointInfo.y = oldPointInfo.y;	// y���꣬��������������ΰ�ԭ������
	newPointInfo.r = oldPointInfo.r;	// �뾶��ֱ�ߵ�Ϊ 0��Բ����Ϊ�뾶ֵ
	newPointInfo.sign = oldPointInfo.sign;	// 0��ֱ�ߵ�  1��Բ����� 2��Բ���յ�
	newPointInfo.dir = oldPointInfo.dir;	// 0��ֱ�� 1����ʱ�� 2��˳ʱ��
	newPointInfo.cut = oldPointInfo.cut;	// 0:������ 1���� 2�������䵶
	newPointInfo.group = oldPointInfo.group;	//  1����� 2���յ�
	newPointInfo.side = oldPointInfo.side;	// �ӹ����� 1�����У��������� 2�����У��������� 3�����ĵ�
	newPointInfo.type = oldPointInfo.type;	// ϳ������ 0����CNC�ӹ��� 1��CNC�ӹ���
}


//�����߶���PDC�ϻ�ͼ
void OtherShapeHelper::DrawLinesInPDC( CDC* pDC, const std::vector<InfoLine>& lines,CPoint offsetPoint, float _scale )
{
	float diff = 0.00001;
	for (int i = 0; i < lines.size() ; i++)
	{
		if (OtherShapeHelper::isPointFloatEquals(lines[i].begin.x,lines[i].begin.y,lines[i].end.x,lines[i].end.y))
		{
			//����Ϊ0ʲô����Ҫ��
		}
		else if ((0.0 - diff) < lines[i].r && lines[i].r < (0.0 + diff))//������ֱ��
		{
			pDC->MoveTo( lines[i].begin.x *_scale + offsetPoint.x ,
				lines[i].begin.y * _scale + offsetPoint.y );
			pDC->LineTo( lines[i].end.x *_scale + offsetPoint.x ,
				lines[i].end.y * _scale + offsetPoint.y);
		}
		else if (lines[i].r > 0.0 + diff)	//������Բ��(С��180��)
		{
			//����Բ��
			OtherPointFloat begin(lines[i].begin.x *_scale + offsetPoint.x ,
				lines[i].begin.y * _scale + offsetPoint.y);
			OtherPointFloat end(lines[i].end.x *_scale + offsetPoint.x ,
				lines[i].end.y * _scale + offsetPoint.y);

			RectF rect;
			double beginAngle;
			double endAngle;
			OtherShapeHelper::getArc(
				OtherPointFloat(lines[i].begin.x,lines[i].begin.y),
				OtherPointFloat(lines[i].end.x,lines[i].end.y),
				lines[i].r,lines[i].dir,
				rect,beginAngle,endAngle);

			rect.X = rect.X * _scale + offsetPoint.x;
			rect.Y = rect.Y * _scale + offsetPoint.y;
			rect.Width = rect.Width*_scale;
			rect.Height = rect.Height*_scale;

			POINT bpoint;
			bpoint.x = begin.x;
			bpoint.y = begin.y;

			POINT epoint;
			epoint.x = end.x;
			epoint.y = end.y;

			//��Բ��
			if (lines[i].dir == 1)
			{
				pDC->MoveTo( lines[i].begin.x *_scale + offsetPoint.x ,
					lines[i].begin.y * _scale + offsetPoint.y );
				pDC->ArcTo(OtherShapeHelper::buildCRect(rect),bpoint,epoint);
			}
			else if (lines[i].dir == 2)
			{
				pDC->MoveTo( lines[i].end.x *_scale + offsetPoint.x ,
					lines[i].end.y * _scale + offsetPoint.y );
				pDC->ArcTo(OtherShapeHelper::buildCRect(rect),epoint,bpoint);
			}
		}
		else if (lines[i].r < 0.0 - diff )	//������Բ��������180�ȣ�
		{
			//����Բ��
			OtherPointFloat begin(lines[i].begin.x *_scale + offsetPoint.x ,
				lines[i].begin.y * _scale + offsetPoint.y);
			OtherPointFloat end(lines[i].end.x *_scale + offsetPoint.x ,
				lines[i].end.y * _scale + offsetPoint.y);

			int dir = lines[i].dir == 1 ? 2 : 1;//Ӱ��Բ�ļ��㷽ʽ�����ı�ԭ��dir

			RectF rect;
			double beginAngle;
			double endAngle;
			OtherShapeHelper::getArc(
				OtherPointFloat(lines[i].begin.x,lines[i].begin.y),
				OtherPointFloat(lines[i].end.x,lines[i].end.y),
				-1* lines[i].r,dir,
				rect,beginAngle,endAngle);

			rect.X = rect.X * _scale + offsetPoint.x;
			rect.Y = rect.Y * _scale + offsetPoint.y;
			rect.Width = rect.Width*_scale;
			rect.Height = rect.Height*_scale;

			POINT bpoint;
			bpoint.x = begin.x;
			bpoint.y = begin.y;

			POINT epoint;
			epoint.x = end.x;
			epoint.y = end.y;

			//��Բ��
			if (lines[i].dir == 1)
			{
				pDC->MoveTo( lines[i].begin.x *_scale + offsetPoint.x ,
					lines[i].begin.y * _scale + offsetPoint.y );
				pDC->ArcTo(OtherShapeHelper::buildCRect(rect),bpoint,epoint);
			}
			else if (lines[i].dir == 2)
			{
				pDC->MoveTo( lines[i].end.x *_scale + offsetPoint.x ,
					lines[i].end.y * _scale + offsetPoint.y );
				pDC->ArcTo(OtherShapeHelper::buildCRect(rect),epoint,bpoint);
			}
		}
	}
}

//�ȶԵ��Ƿ��غ�
bool OtherShapeHelper::isPointFloatEquals(float p1_x ,float p1_y, float p2_x , float p2_y)
{
	float mindiff = 0.0001;	//��Χ

	if ( -mindiff < (p1_x - p2_x) && (p1_x - p2_x) < mindiff )	//X�غ�
	{
		if ( ( -mindiff < (p1_y - p2_y) && (p1_y - p2_y) < mindiff ) )	//Y�غ�
		{
			return true;
		}
	}
	return false;
}

CRect OtherShapeHelper::buildCRect(RectF rect)
{
	return CRect(rect.X,rect.Y,rect.X + rect.Width, rect.Y + rect.Height);
}


/************************************************************************/
/* �������߼���                                                         */
/************************************************************************/

#include <math.h>  
#define MY_PI 3.14159265358979323846  

const int OtherShapeHelper::ArcSignLeft = 1;
const int OtherShapeHelper::ArcSignRight = 2;

//ͨ�������Լ�Բ���ͷ��򣬵õ�Բ������ɵĲ����� rect,beginAngle,endAngle ���������
void OtherShapeHelper::getArc(OtherPointFloat first, OtherPointFloat last,float radius,int sign,RectF& rect,double& beginAngle,double& sweepAngle)
{
	ASSERT(!first.Equals(last));	//��ʼ�ͽ�����ͬһ�㣬��������Բ�����Բ��ܼ���
	ASSERT(radius != 0.0);			//�뾶Ϊ0
	ASSERT(sign == ArcSignLeft || sign == ArcSignRight);	//sign	ȡֵ

	//ȡ��Բ�ĺ������
	OtherPointFloat center = getCircleCenter(first,last,radius,sign);
	rect = RectF(center.x - radius,center.y - radius,radius+radius,radius+radius);

	getAngularCoordinate_DegreeCatchShort(center , first, last, radius, beginAngle, sweepAngle);
	//getDrawArcAngularCoordinate(beginAngle,sweepAngle);
}

//����������׷�����⣬Բ��Բ�������㣬ĳһ���ؽ�����������׷����һ�����Ҫ�ƶ��ĽǶ���
void OtherShapeHelper::getAngularCoordinate_DegreeCatchShort( OtherPointFloat center, OtherPointFloat one, OtherPointFloat other, float radius, double &beginAngle, double &sweepAngle )
{
	double beginAngle1 = 0.0;
	double sweepAngle1 = 0.0;
	getAngularCoordinate_DegreeCatch(center,one,other,radius,beginAngle1,sweepAngle1);

	double beginAngle2 = 0.0;
	double sweepAngle2 = 0.0;
	getAngularCoordinate_DegreeCatch(center,other,one,radius,beginAngle2,sweepAngle2);

	if (sweepAngle1 < sweepAngle2)
	{
		beginAngle = beginAngle1;
		sweepAngle = sweepAngle1;
	}
	else
	{
		beginAngle = beginAngle2;
		sweepAngle = sweepAngle2;
	}
}

//����������׷�����⣬Բ��Բ�������㣬һ���ؽ�����������׷���ڶ�����Ҫ�ƶ��ĽǶ���
void OtherShapeHelper::getAngularCoordinate_DegreeCatch( OtherPointFloat center, OtherPointFloat first, OtherPointFloat last, float radius, double &beginAngle, double &sweepAngle )
{
	//������������
	int sign = 1;

	//���㻡�ߵĽǶ�
	double beginrad = getRadFrom2Point(center,first);
	double beginAngleDegree = getAngleFromRad(beginrad);
	double endrad = getRadFrom2Point(center,last);
	double endAngleDegree = getAngleFromRad(endrad);


	if (endAngleDegree < beginAngleDegree)	//������360��Ҳ������0�㣬�����˳������ʱ���ʱ����Ҫ�Ǵ�������˳ʱ��ʱ��ҪС��
	{
		endAngleDegree += 360.0;
	}

	beginAngle = beginAngleDegree;
	sweepAngle = endAngleDegree - beginAngleDegree;
}

//������Ƕ�  
double OtherShapeHelper::getRadFrom2Point(OtherPointFloat pointCenter,OtherPointFloat pointDis)  
{  
	OtherPointFloat point1 = OtherPointFloat(pointCenter);  
	OtherPointFloat point2 = OtherPointFloat(pointDis);  
	OtherPointFloat deltaPoint = OtherPointFloat(point2.x-point1.x,point2.y-point1.y);  

	//a = 3; b = -3;  
	//�������Ϊ(a,b)
	//mod(atan2(b,a),2*pi)
	//ֻ��
	double y = deltaPoint.y;
	double x = deltaPoint.x;
	double modnum = 2*MY_PI;
	double dAngle = ( x != 0.0 ) ? fabs(fmod(atan2(y,x),modnum)) :  MY_PI*0.5;//����arctan�������⣬��X������0ʱ��������
	//�°�
	if (pointDis.y > pointCenter.y)
	{
		dAngle = 2*MY_PI - dAngle;
	}
	return dAngle;  
}

//����ת�Ƕ�  
double OtherShapeHelper::getAngleFromRad(double rad)  
{  
	return (rad / MY_PI * 180);  
} 

void OtherShapeHelper::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
	if (radius != 0.0 && len_beginToend != 0.0)
	{
		float dis_x = (beginx - endx)/len_beginToend * radius;
		float dis_y = (beginy - endy)/len_beginToend * radius;

		float mid_x = endx + dis_x;
		float mid_y = endy + dis_y;

		float mid_x_move = mid_x - endx;
		float mid_y_move = mid_y - endy;

		float arc = (MY_PI-2*asin((0.5*len_beginToend)/radius))/2;

		center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
		center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);

		center_x = center_x + endx;
		center_y = center_y + endy;
	}
	else
	{
		center_x = 0.0;
		center_y = 0.0;
	}
}

//����Բ�ģ�sign=0Բ�����߶η�������ֱߣ�sign=1Բ�����߶η�������ֱ�
// ...................
// .......last........
// ......./|\.........
// .sign=1.|..........
// .@......|...@......
// ........|...sign=2.
// ........|..........
// ........|..........
// ......first........
// ...................
OtherPointFloat OtherShapeHelper::getCircleCenter(OtherPointFloat first, OtherPointFloat last , float radius , int sign)
{
	OtherPointFloat center;
	if (sign == ArcSignLeft)
	{
		OtherShapeHelper::getCircleCenter(first.x,last.x,first.y,last.y,radius,sign,center.x,center.y);
	}
	else if (sign == ArcSignRight)
	{
		OtherShapeHelper::getCircleCenter(last.x,first.x,last.y,first.y,radius,sign,center.x,center.y);
	}
	return center;
}

//���sign����-1�ĵ�,����غϵĵ�
std::vector<PointInfo> OtherShapeHelper::ClearUnnessaryPointInfo( const std::vector<PointInfo> pointInfos)
{
	std::vector<PointInfo> points = ClearPointInfoSignNegative(pointInfos);
	//std::vector<InfoLine> lines = GetInfoLineFromPointInfo(points);
	//std::vector<InfoLine> linesAfterRemoveNoLen = RemoveLengthEmptyLine(lines);
	//return GetPointInfoFromInfoLine(linesAfterRemoveNoLen);
	return points;
}

// �������PointInfo���sign -1�� 
std::vector<PointInfo> OtherShapeHelper::ClearPointInfoSignNegative( std::vector<PointInfo> PointInfos )
{
	std::vector<PointInfo> result;
	for (int i = 0 ; i< PointInfos.size() ; i++)
	{
		if (PointInfos[i].sign != -1)
		{
			result.push_back(PointInfos[i]);
		}
	}
	return result;
}

std::vector<PointInfo> OtherShapeHelper::MoveOutlinePoints(std::vector<PointInfo> vOutlinePoint, float fXOffset, float fYOffset)
{
	std::vector<PointInfo> result;
	for(int i = 0; i < vOutlinePoint.size(); i++)
	{
		PointInfo onePoint = vOutlinePoint[i];
		onePoint.x += fXOffset;
		onePoint.y += fYOffset;
		result.push_back(onePoint);
	}
	return result;
}

//��÷���
std::vector<std::vector<InfoLine>> OtherShapeHelper::GetInfoLineGroup( const std::vector<InfoLine> lines )
{
	//���߶η���
	int goupID = 0;
	std::vector<std::vector<InfoLine>> tempLineGroup;
	std::vector<InfoLine> tempLine;
	if (lines.size() > 0 )
	{
		for (int i = 0; i < lines.size() ; i++)
		{
			if (i == 0)
			{
				goupID = lines[i].groupID;
			}
			//���GroupID�仯
			if (goupID != lines[i].groupID)
			{
				tempLineGroup.push_back(tempLine);
				tempLine.clear();
				goupID = lines[i].groupID;
			}
			tempLine.push_back(lines[i]);
		}
		tempLineGroup.push_back(tempLine);
	}
	return tempLineGroup;
}


vector<vector<PointInfo>> SplitAllVertexToGroup(vector<PointInfo>& vAllOutlineVertex)
{
	vector<vector<PointInfo>> vRet;

	vector<PointInfo> vOneGroupVertex;
	for(int i = 0; i < vAllOutlineVertex.size(); i++)
	{
		vOneGroupVertex.push_back(vAllOutlineVertex[i]);
		if(vAllOutlineVertex[i].group == 2)
		{
			vRet.push_back(vOneGroupVertex);
			vOneGroupVertex.clear();
		}
	}

	return vRet;
}

//LineType GetLineType(int nLineSign)
//{
//	LineType eLineType = UNKNOWN;
//	if(nLineSign == 0 || nLineSign == 2)
//		eLineType = STRAIGHT_LINE;
//	else if(nLineSign == 1 || nLineSign == 3)
//		eLineType = CIRCULAR_ARC;
//	return eLineType;
//}

OutlineLine GenLineByVertex(PointInfo& vertex, PointInfo& vertexNext)
{
	OutlineLine line;
	line.m_fStartX = vertex.x;
	line.m_fStartY = vertex.y;
	line.m_nLineType = GetLineType(vertex.sign);
	line.m_fR = vertexNext.r;
	line.m_nDir = vertexNext.dir;
	line.m_fEndX = vertexNext.x;
	line.m_fEndY = vertexNext.y;

	return line;
}

//int GetLineType(int nSign)
//{
//	LineType eLineType = UNKNOWN;
//	if(nSign == 0 || nSign == 2)
//		eLineType = STRAIGHT_LINE;
//	else if(nSign == 1 || nSign == 3)
//		eLineType = CIRCULAR_ARC;
//	return eLineType;
//}

void SloveZeroLine(vector<OutlineLine>& vLine)
{
	for(vector<OutlineLine>::iterator itr = vLine.begin(); itr != vLine.end();)
	{
		if(IsFloatEqual(itr->m_fStartX, itr->m_fEndX) && IsFloatEqual(itr->m_fStartY, itr->m_fEndY))
			itr = vLine.erase(itr);
		else
			itr++;
	}
}

void CalcMillingTrackByOutlineLines(vector<OutlineLine>& vOneSegmentLines, float fKnifeDia, vector<OutlineLine>& vOneSegmentLines_MillingTrack)
{
	//Cad_Complex* pComplex = new Cad_Complex;

	//ShapePoint* pPtn = new ShapePoint(0, 0);
	//pPtn->m_FigureType = FigureType_Arc;
	//pComplex->AddShapePoint(pPtn);

	//ShapePoint* pPtn1 = new ShapePoint(10, 10);
	//pPtn1->m_FigureType = FigureType_Arc;
	//pComplex->AddShapePoint(pPtn1);

	//ShapePoint* pPtn2 = new ShapePoint(0, 20);
	//pPtn2->m_FigureType = FigureType_Arc;
	//pComplex->AddShapePoint(pPtn2);

	//Cad_Complex* pTmpFig = static_cast<Cad_Complex*>(pComplex->CalParallelFigure(1, fKnifeDia/2));

	CString strTmp;

	Cad_Complex* pComplex = new Cad_Complex;
	for(int j = 0; j < vOneSegmentLines.size(); j++)
	{
		OutlineLine& theLine = vOneSegmentLines[j];
		if(theLine.m_nLineType == LineType::UNKNOWN || theLine.m_nLineType == LineType::STRAIGHT_LINE)
		{
			ShapePoint* pPtn = new ShapePoint(theLine.m_fStartX, theLine.m_fStartY);
			if(j == 0)
				pPtn->m_FigureType = FigureType_PolyLine;
			else
			{
				OutlineLine& linePre = vOneSegmentLines[j-1];
				if(linePre.m_nLineType == LineType::UNKNOWN || linePre.m_nLineType == LineType::STRAIGHT_LINE)
					pPtn->m_FigureType = FigureType_PolyLine;
				else
					pPtn->m_FigureType = FigureType_Arc;
			}
			pComplex->AddShapePoint(pPtn);
			strTmp.Format(_T("X=%f Y=%f Type=%d\n"), pPtn->m_x, pPtn->m_y, pPtn->m_FigureType);
			OutputDebugString(strTmp);
		}
		else
		{
			ShapePoint* pPtnStart = new ShapePoint(theLine.m_fStartX, theLine.m_fStartY);
			if(j == 0)
				pPtnStart->m_FigureType = FigureType_Arc;
			else
			{
				OutlineLine& linePre = vOneSegmentLines[j-1];
				if(linePre.m_nLineType == LineType::UNKNOWN || linePre.m_nLineType == LineType::STRAIGHT_LINE)
					pPtnStart->m_FigureType = FigureType_PolyLine;
				else
					pPtnStart->m_FigureType = FigureType_Arc;
			}

			pComplex->AddShapePoint(pPtnStart);
			strTmp.Format(_T("X=%f Y=%f Type=%d\n"), pPtnStart->m_x, pPtnStart->m_y, pPtnStart->m_FigureType);
			OutputDebugString(strTmp);

			float fXMid, fYMid;
			GetArcMidPoint(theLine.m_fStartX, theLine.m_fStartY, theLine.m_fEndX, theLine.m_fEndY, theLine.m_fR, theLine.m_nDir, fXMid, fYMid);
			ShapePoint* pPtnMid = new ShapePoint(fXMid, fYMid);
			pPtnMid->m_FigureType = FigureType_Arc;
			pComplex->AddShapePoint(pPtnMid);
			strTmp.Format(_T("X=%f Y=%f Type=%d\n"), pPtnMid->m_x, pPtnMid->m_y, pPtnMid->m_FigureType);
			OutputDebugString(strTmp);
		}
		if(j == vOneSegmentLines.size() - 1)
		{
			ShapePoint* pPtnEnd = new ShapePoint(theLine.m_fEndX, theLine.m_fEndY);
			if(theLine.m_nLineType == LineType::UNKNOWN || theLine.m_nLineType == LineType::STRAIGHT_LINE)
				pPtnEnd->m_FigureType = FigureType_PolyLine;
			else
				pPtnEnd->m_FigureType = FigureType_Arc;
			pComplex->AddShapePoint(pPtnEnd);
			strTmp.Format(_T("X=%f Y=%f Type=%d\n"), pPtnEnd->m_x, pPtnEnd->m_y, pPtnEnd->m_FigureType);
			OutputDebugString(strTmp);
		}
	}

	Cad_Complex* pTmpFig = static_cast<Cad_Complex*>(pComplex->CalParallelFigure(1, fKnifeDia/2));
	pTmpFig->CalShapePointOutlineInfo();

	int nFigCount = pTmpFig->GetShapePointNum();

	for(int l = 0; l < nFigCount; l++)
	{
		ShapePoint* pPnt = pTmpFig->GetShapePoint(l);
		CString strTmp;
		strTmp.Format(_T("x=%.1f y==%.1f figuretype=%d\n"), pPnt->m_x, pPnt->m_y, pPnt->m_FigureType);
		OutputDebugString(strTmp);
	}

	int l = 0;
	while(l < nFigCount)
	{
		ShapePoint* pPnt = pTmpFig->GetShapePoint(l);

		bool bIsArc = false;
		if(l < nFigCount - 2)
		{
			ShapePoint* pPntNext = pTmpFig->GetShapePoint(l+1);
			ShapePoint* pPntNextNext = pTmpFig->GetShapePoint(l+2);
			if(pPntNext->m_FigureType == FigureType_Arc && pPntNextNext->m_FigureType == FigureType_Arc)
				bIsArc = true;
		}

		if(/*pPnt->m_FigureType == FigureType_PolyLine*/!bIsArc)
		{
			if(l != nFigCount - 1)
			{
				ShapePoint* pPntNext = pTmpFig->GetShapePoint(l+1);
				OutlineLine line;
				line.m_fStartX = pPnt->m_x;
				line.m_fStartY = pPnt->m_y;
				line.m_fEndX = pPntNext->m_x;
				line.m_fEndY = pPntNext->m_y;
				line.m_nLineType = LineType::STRAIGHT_LINE;
				vOneSegmentLines_MillingTrack.push_back(line);
			}
		}
		else/* if(pPnt->m_FigureType == FigureType_Arc)*/
		{
			if(l < nFigCount - 2)
			{
				ShapePoint* pPntMid = pTmpFig->GetShapePoint(l+1);
				ShapePoint* pPntEnd = pTmpFig->GetShapePoint(l+2);
				OutlineLine line;
				line.m_fStartX = pPnt->m_x;
				line.m_fStartY = pPnt->m_y;
				line.m_fEndX = pPntEnd->m_x;
				line.m_fEndY = pPntEnd->m_y;
				line.m_nLineType = LineType::CIRCULAR_ARC;

				float fXCenter, fYCenter;
				GetCircleCenterByThreePoint(line.m_fStartX, line.m_fStartY, pPntMid->m_x, pPntMid->m_y, line.m_fEndX, line.m_fEndY, fXCenter, fYCenter);
				line.m_fR = GetPointDistance(line.m_fStartX, line.m_fStartY, fXCenter, fYCenter);
				line.m_nDir = GetArcDirByPoint(line.m_fStartX, line.m_fStartY, line.m_fEndX, line.m_fEndY, fXCenter, fYCenter);

				vOneSegmentLines_MillingTrack.push_back(line);
				l++;
			}

		}
		l++;
	}

	delete pTmpFig;
	pTmpFig = NULL;

#ifndef _DEBUG
	delete pComplex;
	pComplex = NULL;
#endif
}

void OutlineGroupRotateClockwise90(vector<vector<OutlineLine>>& vMillingOutlineGroup, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vMillingOutlineGroup.size(); i++)
	{
		for(int j = 0; j < vMillingOutlineGroup[i].size(); j++)
		{
			OutlineLine& theOutlineLine = vMillingOutlineGroup[i][j];
			float fOrgStartX = theOutlineLine.m_fStartX;
			float fOrgStartY = theOutlineLine.m_fStartY;
			float fOrgEndX = theOutlineLine.m_fEndX;
			float fOrgEndY = theOutlineLine.m_fEndY;
			theOutlineLine.m_fStartX = fOrgStartY;
			theOutlineLine.m_fStartY = fOrgXLen - fOrgStartX;
			theOutlineLine.m_fEndX = fOrgEndY;
			theOutlineLine.m_fEndY = fOrgXLen - fOrgEndX;
		}
	}
}