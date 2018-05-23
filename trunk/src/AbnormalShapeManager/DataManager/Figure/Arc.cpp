#include "stdafx.h"
#include "Arc.h"
#include "../CommonData.h"
#include <math.h>
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"

Cad_Arc::Cad_Arc()
{
	m_FigureID = 0;
	m_FigureType = FigureType_Arc;
	m_pLayer = NULL;

	m_CenterX = 0.0;
	m_CenterY = 0.0;
	m_radius = 0.0;
	m_StartAngle = 0.0;
	m_EndAngle = 0.0;


}

Cad_Arc::~Cad_Arc()
{

}


/*----------------------------------------------------------*/
//	purpose:
//		�ƶ�ͼ��
//		
//	param:
//		float rel_x -- �������x
//		float rel_y -- �������y
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Arc::MoveFigure(float rel_x, float rel_y)
{
	// �ƶ���״��
	GeneralInterface::MoveShapePointList(&m_ShapePointList, rel_x, rel_y);

	// ���¼���Բ�İ뾶
	CalCenterRadiusInfo();

	// ��������������
	CalOutlineList();

	// ���¼������
	CalFigureRect();
}


/*-----------------------------------------------------------------------*/
//	purpose:
//		����ƽ�е���״������
//		����Բ�ģ��뾶��������������
//		
//
//	param��
//		int dir -- ����
//		float dist -- ����
//
//	return:
//		vector<ShapePoint*> -- ƽ�к����״��
/*-----------------------------------------------------------------------*/
Figure*  Cad_Arc::CalParallelFigure(int dir, float dist)
{
	int arc_dir = GetDirection();
	float new_radius;
	float vec_start_x, vec_start_y, vec_mid_x, vec_mid_y, vec_end_x, vec_end_y;
	ShapePoint *pStart, *pMid, *pEnd;
	ShapePoint *pNewStart, *pNewMid, *pNewEnd;
	Cad_Arc* pParallelArc = new Cad_Arc;

	// ����ֵ���Ե�һ��Ϊԭ��
	pStart	= GetShapePoint(0);
	pMid	= GetShapePoint(1);
	pEnd	= GetShapePoint(2);

	// ��Բ��ָ���������
	vec_start_x = pStart->m_x - m_CenterX;
	vec_start_y = pStart->m_y - m_CenterY;
	vec_mid_x	= pMid->m_x - m_CenterX;
	vec_mid_y	= pMid->m_y - m_CenterY;
	vec_end_x	= pEnd->m_x - m_CenterX;
	vec_end_y	= pEnd->m_y - m_CenterY;

	// �淶������
	GeneralInterface::NormalVector(vec_start_x, vec_start_y, vec_start_x, vec_start_y);
	GeneralInterface::NormalVector(vec_mid_x, vec_mid_y, vec_mid_x, vec_mid_y);
	GeneralInterface::NormalVector(vec_end_x, vec_end_y, vec_end_x, vec_end_y);

	// �����°뾶
	if (arc_dir == Dir_CouterClockwise)	// ��ʱ��
	{
		if (dir == Dir_Left)	//	�� == ����
			new_radius = m_radius - dist;
		else					// �� == ����
			new_radius = m_radius + dist;

		if (new_radius <= 0.0)
		{
			AfxMessageBox("Բ���뾶������С��");
		}
	}
	else
	{
		if (dir == Dir_Left)	//	�� == ����
			new_radius = m_radius + dist;
		else					// �� == ����
			new_radius = m_radius - dist;

		if (new_radius <= 0.0)
		{
			AfxMessageBox("Բ���뾶������С��");
		}
	}

	// ����Ϊ�°뾶������
	vec_start_x *= new_radius;
	vec_start_y *= new_radius;
	vec_mid_x	*= new_radius;
	vec_mid_y	*= new_radius;
	vec_end_x	*= new_radius;
	vec_end_y	*= new_radius;

	// �������״��
	pNewStart = new ShapePoint(vec_start_x + m_CenterX, vec_start_y + m_CenterY);
	pNewMid = new ShapePoint(vec_mid_x + m_CenterX, vec_mid_y + m_CenterY);
	pNewEnd = new ShapePoint(vec_end_x + m_CenterX, vec_end_y + m_CenterY);

	pParallelArc->AddShapePoint(pNewStart);
	pParallelArc->AddShapePoint(pNewMid);
	pParallelArc->AddShapePoint(pNewEnd);

	// ����Բ�İ뾶��Ϣ ����ͬ��
	pParallelArc->CalCenterRadiusInfo();
	pParallelArc->UpdateShapePointInfo();
	pParallelArc->UpdateOutlineInfo();

	return pParallelArc;
}


// ������״������
void Cad_Arc::ReverseShapePointList()
{
	GeneralInterface::ReverseShapePointList(&m_ShapePointList);
	CalCenterRadiusInfo();
}



// ������״����������Ϣ
void Cad_Arc::CalShapePointOutlineInfo(void)
{
	int dir;
	float angle, r, negative_r_angle;
	ShapePoint* pPnt = NULL;

	// ��ȡ����Բ�Ľ�
	dir = GetDirection();
	angle = GetCentralAngle();
	negative_r_angle = PI;

	// G������>180�ȵ�Բ��rΪ������Ӧ���ڳ�G�����д����������������� ��������ʱδ�Գ���
// 	if (angle > negative_r_angle)
// 		r = -m_radius;
// 	else
		r = m_radius;

	// ��һ����
	pPnt = GetShapePoint(0);

	pPnt->m_radius = r;
	pPnt->m_sign = Sign_ArcStart;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;
	

	pPnt = GetShapePoint(1);
	pPnt->m_radius = r;
	pPnt->m_sign = Sign_Ignore;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;


	pPnt = GetShapePoint(2);
	pPnt->m_radius = r;
	pPnt->m_sign = Sign_ArcEnd;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;

}



// ����������	
void Cad_Arc::CalOutlineList(void)
{
	int i, num, dir;
	float x, y;
	float L, space, Radian, dRadian;
	FPoint *pNewPnt = NULL;

	// �������������
	ClearOutlineList();

	// ����һ��Բ�İ뾶����Ϣ��ȷ������һ��
	CalCenterRadiusInfo();

	space = m_OutlineSpace;	// m_OutlineSpace mmΪ��������
	dir = GetDirection();

	if (dir == Dir_CouterClockwise)	// ��ʱ�룬΢�ֻ�Ϊ��
	{
		if (m_EndAngle > m_StartAngle)
		{
			Radian = m_EndAngle - m_StartAngle;
		}
		else
		{
			Radian = (float)(m_EndAngle + 2*PI - m_StartAngle);
		}
	}
	else	// ˳ʱ�룬΢�ֻ�Ϊ��
	{
		if (m_EndAngle > m_StartAngle)
		{
			Radian = (float)(m_EndAngle - m_StartAngle - 2*PI);
		}
		else
		{
			Radian = m_EndAngle - m_StartAngle; 
		}
	}

	L = abs(m_radius * Radian);

	num = (int)(L/space + 1);

	dRadian = Radian / num;

	// ��һ����
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(m_StartAngle);
	y = m_CenterY + m_radius *sin(m_StartAngle);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;

	// �м��
	for (i = 1; i < num; i++)
	{
		pNewPnt = new FPoint;
		x = m_CenterX + m_radius *cos(m_StartAngle + i * dRadian);
		y = m_CenterY + m_radius *sin(m_StartAngle + i * dRadian);

		pNewPnt->SetXY(x, y);
		AddOutline(pNewPnt);
		pNewPnt = NULL;
	}

	// ��β��
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(m_EndAngle);
	y = m_CenterY + m_radius *sin(m_EndAngle);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;
}


// ��ȡ˳��ʱ�뷽��
int Cad_Arc::GetDirection(void)
{
	int nCount;
	float sign;
	float x1, y1, x2, y2;
	float dx1, dy1, dx2, dy2;
	ShapePoint p0, p1, p2;

	nCount = m_ShapePointList.size();
	if (nCount != 3)
		return -1;

	// ����ֵ���Ե�һ��Ϊԭ��
	p0 = *(m_ShapePointList[0]);
	p1 = *(m_ShapePointList[1]);
	p2 = *(m_ShapePointList[2]);

	p1 = p1 - p0;
	p2 = p2 - p0;

	// xy����
	x1 = p1.m_x;
	y1 = p1.m_y;
	x2 = p2.m_x;
	y2 = p2.m_y; 

	// xyƫ��
	dx1 = p1.m_x;
	dy1 = p1.m_y;
	dx2 = p2.m_x - p1.m_x;
	dy2 = p2.m_y - p1.m_y;

	// �ж�б���Ƿ����
	sign = dy2*dx1 - dx2*dy1;
	if (sign == 0.0)	//ƽ��
	{
		return Dir_Parallel;
	}
	else if (sign > 0)
	{
		return Dir_CouterClockwise;
	}
	else
	{
		return Dir_Clockwise;
	}
}

// ��ȡԲ�Ľ�
float Cad_Arc::GetCentralAngle(void)
{
	int dir;
	float angle;

	angle = 0;
	dir = GetDirection();
	if (dir == Dir_Clockwise)
	{
		if (m_StartAngle > m_EndAngle)
		{
			angle = m_StartAngle - m_EndAngle;
		}
		else	
		{
			angle = m_StartAngle + 2*PI - m_EndAngle;
		}
	}
	else if (Dir_CouterClockwise)
	{
		if (m_EndAngle > m_StartAngle)
		{
			angle = m_EndAngle - m_StartAngle;
		}
		else	
		{
			angle = m_EndAngle + 2*PI - m_StartAngle;
		}
	}

	return angle;
}




// Բ�İ뾶��ʼ�����Ƕȣ���������״��,dxf����Ķ�����ʱ�룬���Ƿ񳬹�360��
void Cad_Arc::CenterRadius2ShapePoint(void)
{
	float x, y, angle;
	ShapePoint* pPnt = NULL;

	if (m_StartAngle > m_EndAngle)	// ���360��
	{
		// ��һ����
		angle = m_StartAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// �ڶ�����
		angle = (float)((m_StartAngle + m_EndAngle + 2*PI)/2);
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// ��������
		angle = m_EndAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);
	}
	else
	{
		// ��һ����
		angle = m_StartAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// �ڶ�����
		angle = (m_StartAngle + m_EndAngle)/2;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// ��������
		angle = m_EndAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);
	}
}

// ͨ����״�����Բ�İ뾶
void Cad_Arc::CalCenterRadiusInfo()
{
	int dir;
	float Cx, Cy, r, s_angle,e_angle;
	
	dir = GetDirection();
	CalCenterRadius(&Cx, &Cy, &r, &s_angle, &e_angle);

	m_CenterX = Cx;
	m_CenterY = Cy;
	m_radius = r;
	m_StartAngle = s_angle;
	m_EndAngle = e_angle;
}



// ����Բ�ĺͰ뾶 ������Բ��һ�㹫ʽ x^2+y^2+Dx+Ey+F=0 (D^2+E^2-4F>0) ������뷽�����
//��X+D/2��^2+��Y+E/2��^2=(D^2+E^2-4F)/4
int Cad_Arc::CalCenterRadius(float* pCx, float* pCy, float* r, float* s_angle, float* e_angle)
{
	int nCount;
	float sign, D, E;
	float numerator, denominator;	// ���ӣ���ĸ
	float x1, y1, x2, y2;
	float dx1, dy1, dx2, dy2;
	ShapePoint p0, p1, p2;

	nCount = m_ShapePointList.size();
	if (nCount != 3)
		return -1;

	// ����ֵ���Ե�һ��Ϊԭ��
	p0 = *(m_ShapePointList[0]);
	p1 = *(m_ShapePointList[1]);
	p2 = *(m_ShapePointList[2]);

	p1 = p1 - p0;
	p2 = p2 - p0;

	// xy����
	x1 = p1.m_x;
	y1 = p1.m_y;
	x2 = p2.m_x;
	y2 = p2.m_y; 

	// xyƫ��
	dx1 = p1.m_x;
	dy1 = p1.m_y;
	dx2 = p2.m_x - p1.m_x;
	dy2 = p2.m_y - p1.m_y;

	// �ж�б���Ƿ����
	sign = dy2*dx1 - dx2*dy1;
	if (sign == 0.0)	//ƽ��
	{
		AfxMessageBox("Բ�����ݳ����޷����㣡");
		return -1;
	}
	else				
	{
		// ������D�Ļ�����
		numerator = y1*(x2*x2 + y2*y2) - y2*(x1*x1 + y1*y1);
		denominator = x1*y2 - x2*y1;

		D = numerator/denominator;

		// ����һ��y�п���Ϊ0
		if (abs(y1) > abs(y2) )
			E = -(x1*x1 + y1*y1 + D*x1)/y1;
		else
			E = -(x2*x2 + y2*y2 + D*x2)/y2;

		// 
		float dx, dy, start_angle, end_angle;
		float center_x, center_y, radius;

		// ���Բ��(�˴�Բ������Ե�һ����״������꣬��������ϵ�һ����״��ľ���������������ľ�������),�뾶����ȷ�� 
		center_x = -(D/2);
		center_y = -(E/2);

		dx = x1 - center_x;
		dy = y1 - center_y;
		radius = sqrt(dx*dx + dy*dy);

		// �����ľ�������
		center_x += p0.m_x;
		center_y += p0.m_y;

// 		if (sign > 0)	// ��ʱ��
// 		{
			dx = m_ShapePointList[0]->m_x - center_x;
			dy = m_ShapePointList[0]->m_y - center_y;
			start_angle = GeneralInterface::GetRadianFromXY(dx, dy);

			dx = m_ShapePointList[2]->m_x - center_x;
			dy = m_ShapePointList[2]->m_y - center_y;
			end_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 		}
// 		else			// ˳ʱ��
// 		{
// 			dx = m_ShapePointList[0]->m_x - center_x;
// 			dy = m_ShapePointList[0]->m_y - center_y;
// 			start_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 
// 			dx = m_ShapePointList[2]->m_x - center_x;
// 			dy = m_ShapePointList[2]->m_y - center_y;
// 			end_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 		}



		// ����ֵ
		if (pCx != NULL)
			*pCx = center_x;	

		if (pCy != NULL)
			*pCy = center_y;	

		if (r != NULL)
			*r = radius;

		if (s_angle != NULL)
			*s_angle = start_angle;

		if (e_angle != NULL)
			*e_angle = end_angle;
	}

	return 0;
}

// ����Բ�İ뾶
void Cad_Arc::SetCenterRadius(float pCx, float pCy, float r, float s_angle, float e_angle)
{
	m_CenterX = pCx;
	m_CenterY = pCy;
	m_radius = r;
	m_StartAngle = s_angle;
	m_EndAngle = e_angle;
}