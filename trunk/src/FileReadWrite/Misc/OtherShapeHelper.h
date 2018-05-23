#ifndef		_OtherShapeHelper_H_
#define		_OtherShapeHelper_H_

#include "../../../include/KnifeDll/KnifeClass.h"
//#include "../../../include/TinyXml/tinyxml.h"
#include "../Misc/XmlHandlePlus.h"

#include <string>
#include <vector>

using namespace std;

#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	std::string _name;
	std::string _value;
};
#endif

class AFX_EXT_CLASS OtherShapeHelper
{
public:

	OtherShapeHelper();

public:

	// �������PointInfo 
	// x:50.0,y:30.0;  => {[ x:50.0 , y:30.0 ]}
	static string GetPointInfoStringFromPointInfo(vector<PointInfo> PointInfos);

	// �������PointInfo 
	//  {[ x:50.0 , y:30.0 ]} => x:50.0,y:30.0;
	static std::vector<PointInfo> GetPointInfoFromPointInfoString(std::string PointInfo);

	//ͨ������������
	static std::vector<PointInfo> getPointInfoRectangle(const std::string rotate, std::string& panelWidth,std::string& panelHeight );

	//////////////////////////////////////////////////////////////////////////
	// �����
	static PointInfo GenerateOnePointInfo( std::string str_x,std::string str_y,std::string str_r,std::string str_sign,std::string str_dir,std::string str_cut,std::string str_group,std::string str_side );

	//��ת�������ı�������
	static std::vector<PointInfo> getPointInfoRotate(const std::vector<PointInfo> pointInfos,const std::string rotate , std::string panelWidth,std::string panelHeight );

	//���ڶ�����ת90��
	static std::vector<PointInfo> RotatePointInfoBackDegree90( std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);
	//����center��תangle��
	static std::vector<PointInfo> RotatePointInfoDegree90(std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);
	static std::vector<PointInfo> RotatePointInfoDegree180(std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);

	// ����string �����б�
	// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , gapWidth : 10 , gapHeight : 20 }
	static std::vector<param_point> GetParamPointsFromOtherShapeString( std::string otherShapeString );

	// ����string �����б�
	// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , PW : 50 , PD : 50 , gapWidth : 10 , gapHeight : 20 }
	static std::vector<param_point> GetParamPointsFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight );

	//���ݳ������������������
	static bool sortFunctionByStrLen(const param_point & m1, const param_point & m2) ;
	//���ݳ������������������, �������ճ��ȣ�������
	static void sortParam_pointByStrLen(std::vector<param_point> & pa_p) ;

	//���β����б�PW�ж�
	static bool isParamPointHasPanelWidth( const std::vector<param_point>& pa_p );
	//���β����б�PD�ж�
	static bool isParamPointHasPanelHeight( const std::vector<param_point>& pa_p );

	//���β����б� ��������
	static void PushParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value );
	//���β����б� ��������
	static void SetParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value );
	//���β����б� ��ȡ����
	static std::string GetParamPoint( std::vector<param_point>& pa_p, std::string name );

	//��������ר�� �����б���ת
	static void RotateOtherShapeParamPoint(std::vector<param_point>& pa_p,std::string rotate);
	//��������ר�� ������ת
	static std::vector<PointInfo> RotateOtherShapePointInfos(std::vector<PointInfo> m_pointInfos,std::string changeType ,std::string m_panelwidth, std::string m_panelheight);

	// ������string�������滻�������β������󣬼������������ͨ����
	// ID:2016;PW:50;PD:60;gapWidth:10;gapHeight:20; + {[x:"PD-gapWidth",y:"PW-gapHeight"]} => {[ x:50.0 , y:30.0 ]}
	static std::vector<PointInfo> GetPointInfoFromPanelOutLineAndParamPoint(std::vector<std::map<std::string,std::string>> panelOutline, std::vector<param_point> pa_p);

	// ��ù������β�������
	// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
	static std::vector<std::map<std::string,std::string>> GetOtherShapePanelOutLineListFromOtherShapeID(std::string otherShapeID);

	//����string�л�ȡ ID����
	static std::string GetOtherShapeId( const std::string& otherShapeID );
	//���β����б��л�ȡ ID����
	static std::string GetOtherShapeId( const std::vector<param_point>& pa_p );

	//�ѵ�ӵ��������Ƶ���һ����
	static void GetPointsToYPlus(std::vector<PointInfo>& pointInfos, const std::string rotate, std::string m_panelwidth, std::string m_panelheight);

	static std::vector<PointInfo> GetPointInfoReverse( const std::vector<PointInfo> pointInfos );

	//��÷�ת�����������ҷ�ת��
	static std::vector<PointInfo> getPointInfoBack(const std::vector<PointInfo> pointInfos,const std::string& panelWidth,const std::string& panelHeight);
private:
	static TinyXml::TiXmlDocument docPanelOutlineList;
	static bool isLoadPanelOutlineList;
};

#endif