#ifndef		_OtherShapeHelper_H_
#define		_OtherShapeHelper_H_

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/KnifeDll/KnifeClass.h"
#include "../../../include/TinyXml/tinyxml.h"

#include "../UIData/UIData.h"

#include <string>
#include <vector>

using namespace std;

using namespace TinyXml;

typedef std::map<std::string,std::string>	OutlineParamPoint;		//���ε㣨��ʽ��ʽ��
typedef std::vector<OutlineParamPoint>		OutlineParamShape;		//���Σ���ʽ��ʽ��
typedef std::map<int,OutlineParamShape>		OutlineParamShapeDict;	//�����ֵ䣨��ʽ��ʽ��

#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	std::string _name;
	std::string _value;
};
#endif

//�����ڲ�ʹ�õ���Ϣ
#ifndef POINT_FLOAT
#define POINT_FLOAT
class OtherPointFloat
{
public:
	OtherPointFloat()
	{
		x = 0.0f;
		y = 0.0f;
	}

	OtherPointFloat(IN const OtherPointFloat &point)
	{
		x = point.x;
		y = point.y;
	}

	OtherPointFloat(IN float _x, IN float _y)
	{
		x = _x;
		y = _y;
	}

	OtherPointFloat operator+(IN const OtherPointFloat& point) const
	{
		return OtherPointFloat(x + point.x,
			y + point.y);
	}

	OtherPointFloat operator-(IN const OtherPointFloat& point) const
	{
		return OtherPointFloat(x - point.x,
			y - point.y);
	}

	BOOL Equals(IN const OtherPointFloat& point) 
	{
		float mindiff = 0.0001f;	//��Χ

		if ( -mindiff < (x - point.x) && (x - point.x) < mindiff )	//X�غ�
		{
			if ( ( -mindiff < (y - point.y) && (y - point.y) < mindiff ) )	//Y�غ�
			{
				return true;
			}
		}
		return false;
	}

public:

	float x;
	float X() const { return x; }
	void X(float val) { x = val; }
	float y;
	float Y() const { return y; }
	void Y(float val) { y = val; }
};
#endif

//�����ڲ�ʹ������Ϣ
#ifndef INFO_LINE
#define INFO_LINE
class InfoLine
{
public:
	InfoLine();
	InfoLine(IN const InfoLine& _line);

	InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end);

	InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end, float _r, int _dir,int _groupID,int _side,bool _visable);

	InfoLine operator=(IN const InfoLine& _line) const;

public:
	PointInfo begin;
	PointInfo end;
	float r;
	int dir;
	int groupID;
	int side;
	bool visable;
};
#endif


class OtherShapeHelper
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

	// ������й������β�������
	// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
	static OutlineParamShapeDict GetOtherShapePanelOutLineList();

	// ����string ת����
	// ID:2016;gapWidth:10;gapHeight:20;   =>    {[ x:50.0 , y:30.0 ]}
	static std::vector<PointInfo> GetPointInfoFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight);

	//���β����б�PW PDɾ��
	static void removeParamPoint_PanelWidthPanelHeight( std::vector<param_point>& pa_p );

	// �����б� ת ����string 
	// { ID : 2016 , gapWidth : 10 , gapHeight : 20 }   =>   ID:2016;gapWidth:10;gapHeight:20;
	static std::string GetOtherShapeStringFromParamPoints( std::vector<param_point> pa_p );

	//ͨ�������߶�����
	static std::vector<InfoLine> GetInfoLineFromPointInfo(const std::vector<PointInfo>& pointInfos );

	//���Ƶ���Ϣ
	static void clonePointInfo(PointInfo& newPointInfo,const PointInfo& oldPointInfo);

	//�����߶���PDC�ϻ�ͼ
	static void DrawLinesInPDC( CDC* pDC, const std::vector<InfoLine>& lines,CPoint offsetPoint, float _scale );

	//�ȶԵ��Ƿ��غ�
	static bool isPointFloatEquals(float p1_x ,float p1_y, float p2_x , float p2_y);

	static CRect buildCRect(RectF rect);

	//ͨ�������Լ�Բ���ͷ��򣬵õ�Բ������ɵĲ����� rect,beginAngle,endAngle ���������
	static void getArc(OtherPointFloat first, OtherPointFloat last,float radius,int sign,RectF& rect,double& beginAngle,double& sweepAngle);

	//����������׷�����⣬Բ��Բ�������㣬ĳһ���ؽ�����������׷����һ�����Ҫ�ƶ��ĽǶ���
	static void getAngularCoordinate_DegreeCatchShort( OtherPointFloat center, OtherPointFloat one, OtherPointFloat other, float radius, double &beginAngle, double &sweepAngle );

	//����������׷�����⣬Բ��Բ�������㣬һ���ؽ�����������׷���ڶ�����Ҫ�ƶ��ĽǶ���
	static void getAngularCoordinate_DegreeCatch( OtherPointFloat center, OtherPointFloat first, OtherPointFloat last, float radius, double &beginAngle, double &sweepAngle );

	//������Ƕ�  
	static double getRadFrom2Point(OtherPointFloat pointCenter,OtherPointFloat pointDis) ;
	
	//����ת�Ƕ�  
	static double getAngleFromRad(double rad) ;

	//�õ�Բ����Բ��
	static void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

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
	static OtherPointFloat getCircleCenter(OtherPointFloat first, OtherPointFloat last , float radius , int sign);

	//���sign����-1�ĵ�
	static std::vector<PointInfo> ClearUnnessaryPointInfo(const std::vector<PointInfo> pointInfos);

	// �������PointInfo���sign -1�� 
	static std::vector<PointInfo> ClearPointInfoSignNegative( std::vector<PointInfo> PointInfos );

	static std::vector<PointInfo> MoveOutlinePoints(std::vector<PointInfo> vOutlinePoint, float fXOffset, float fYOffset);

	//��÷���
	static std::vector<std::vector<InfoLine>> GetInfoLineGroup(const std::vector<InfoLine> infoLine);

	const static int ArcSignLeft;
	const static int ArcSignRight;
private:
	static bool isLoadPanelOutlineList;
	static TinyXml::TiXmlDocument docPanelOutlineList;
};

vector<vector<PointInfo>> SplitAllVertexToGroup(vector<PointInfo>& vAllOutlineVertex);

OutlineLine GenLineByVertex(PointInfo& vertex, PointInfo& vertexNext);

//int GetLineType(int nSign);

void CalcMillingTrackByOutlineLines(vector<OutlineLine>& vOneSegmentLines, float fKnifeDia, vector<OutlineLine>& vOneSegmentLines_MillingTrack);

void OutlineGroupRotateClockwise90(vector<vector<OutlineLine>>& vMillingOutlineGroup, float fOrgXLen, float fOrgYLen);

void SloveZeroLine(vector<OutlineLine>& vLine);

//LineType GetLineType(int nLineSign);

#endif