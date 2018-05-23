/*--------------------------------------------------------------------------------------------------------------------*/
//	GeneralInterface.h    -- ��������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.12
//	��ע��	���ݲ��������ӿ�
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		GeneralInterface_H
#define		GeneralInterface_H

#include <vector>
using namespace std;

class Layer;
class Figure;
class ShapePoint;
class FPoint;

class AFX_EXT_CLASS GeneralInterface
{
public:
	//���������
	static void ClearLayerList(vector<Layer*>* pList);						// ���ͼ������
public:
	//ͼ���������
	//static void ClearLayerList(vector<Figure*>* pList);					// ���ͼ������

public:
	// ƽ��ͼ������
	static BOOL ParallelFigureList(vector<Figure*>* pSrcList, vector<Figure*>* pDstList, int dir, float dist);

	//ͼ���������
	static void ClearFigureList(vector<Figure*>* pList);					// ���ͼ������

	// ͼ���������� -- �����δ�С
	static void SortFigureListByRect(vector<Figure*>* pList);					// ���ͼ������

	static BOOL CalFigureIntersect(Figure* pSrcFig, Figure* pDstFig, FPoint& inter_pnt);			// ������ͼ���Ƿ��ཻ

	static vector<ShapePoint*> ArcInterpolate(ShapePoint* center, ShapePoint* start, ShapePoint* end, float r);	// ��Բ���岹���ɸ���

public:
	// ��״������
	static void MoveShapePointList(vector<ShapePoint*>* pList, float rel_x, float rel_y);	// �ƶ���״������
	static void ReverseShapePointList(vector<ShapePoint*>* pList);			// ������״������
	static void ClearShapePointList(vector<ShapePoint*>* pList);			// �����״������

	// ����
	static int LinkFigure(Figure* pSrcFig, Figure* pDstFig);				// ����ͼ��
	static int LinkShapePoint(ShapePoint* pSrcPnt, ShapePoint* pDstPnt);	// ������״��
	static int LinkTail2Head(Figure* pSrcFig, Figure* pDstFig);				// ͼ��β����ͼ��ͷ
	static BOOL LinkShapePoint_Judge(ShapePoint* pSrcPnt, ShapePoint* pDstPnt, int* pStyle);	// ������״���ж�


public:
	// ����������
	static void MoveOutlineList(vector<FPoint*>* pList, float rel_x, float rel_y);	// �ƶ�����������
	static void ClearOutlineList(vector<FPoint*>* pList);					// �������������

public:
	// ��ѧ����

	// ��ȡ������ϵ�еĽǶȣ����ȱ�ʾ
	static float GetRadianFromXY(float dx, float dy);	

	// ָ���㵽ֱ�ߵľ���
	static float GetDistanceFromPoint2Line(float start_x, float start_y, float end_x, float end_y, float x, float y);	

	// ���������н�
	static float CalVectorAngle(float dx1, float dy1, float dx2, float dy2);


	// ƽ��һ���߶�
	static BOOL ParallelSegment(float start_x, float start_y, float end_x, float end_y, int dir, float dist,
								float & new_start_x, float & new_start_y, float & new_end_x, float & new_end_y);

	// ��ʱ����ת
	static void CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc);

	// ˳ʱ����ת
	static void ClockwiseRotate(float x0,float y0,float & x,float & y,float arc);

	// �����淶��
	static void NormalVector(float x0,float y0,float & x,float & y);

	// ����Բ���ϵ��е�
	static BOOL CalArcMidPoint(float start_x, float start_y, float end_x, float end_y, float center_x, float center_y, float r, float &mid_x, float &mid_y);

	// ����ȡԲ��
	static void GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

	// ���߶��ཻ
	static BOOL CalSegmenIntersect(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y);

	// ���߶��ཻ
	static BOOL CalSegmenIntersect2(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y);

	// ��ֱ���ཻ
	 static BOOL CalLineIntersect(float start_x1, float start_y1, float end_x1, float end_y1, float start_x2, float start_y2, float end_x2, float end_y2, float& inter_x, float& inter_y);
};


#endif