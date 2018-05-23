/*--------------------------------------------------------------------------------------------------------------------*/
//	Circle.cpp   -- Բ��ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.19
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Circle.h"
#include "../CommonData.h"
#include "../Point/ShapePoint.h"


Cad_Circle::Cad_Circle()
{
	m_CenterX = 0;
	m_CenterY = 0;
	m_radius = 0;
}


Cad_Circle::~Cad_Circle()
{

}

// ����������		
void Cad_Circle::CalOutlineList(void)		
{
	int i, num, dir;
	float x, y;
	float L, space, Radian, dRadian;
	FPoint *pNewPnt = NULL;

	// �������������
	ClearOutlineList();

	space = OUTLINE_SPACE;	// 10mmΪһ����
	dir = GetDirection();

	if (dir == Dir_CouterClockwise)	// ��ʱ�룬΢�ֻ�Ϊ��
	{
		Radian = (float)(2*PI );
	}
	else	// ˳ʱ�룬΢�ֻ�Ϊ��
	{
		Radian = (float)(-2*PI );
	}

	L = abs(m_radius * Radian);

	num = (int)(L/space + 1);

	dRadian = Radian / num;

	// ��һ����
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(0.0f);
	y = m_CenterY + m_radius *sin(0.0f);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;

	// �м��
	for (i = 1; i < num-1; i++)
	{
		pNewPnt = new FPoint;
		x = m_CenterX + m_radius *cos(i * dRadian);
		y = m_CenterY + m_radius *sin(i * dRadian);

		pNewPnt->SetXY(x, y);
		AddOutline(pNewPnt);
		pNewPnt = NULL;
	}

	// ��β��
	pNewPnt = new FPoint;
	*pNewPnt = *(GetHeadShapePoint());

	AddOutline(pNewPnt);
	pNewPnt = NULL;
}


// ��ȡ˳��ʱ�뷽��
int Cad_Circle::GetDirection(void)
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


// Բ�İ뾶��ʼ�����Ƕȣ���������״��,dxf����Ķ�����ʱ�룬���Ƿ񳬹�360��
void Cad_Circle::CenterRadius2ShapePoint(void)
{
	float x, y, angle;
	ShapePoint* pPnt = NULL;

	// ��һ����
	angle = 0.0f;
	x = m_radius + m_CenterX;
	y = m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);

	// �ڶ�����
	angle = (float)(PI/2);
	x = m_CenterX;
	y = m_radius + m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);

	// ��������
	angle = (float)(PI);
	x = -m_radius + m_CenterX;
	y = m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);
}


// ����Բ�ĺͰ뾶 ������Բ��һ�㹫ʽ x^2+y^2+Dx+Ey+F=0 (D^2+E^2-4F>0) ������뷽�����
//��X+D/2��^2+��Y+E/2��^2=(D^2+E^2-4F)/4
int Cad_Circle::CalCenterRadius(float* pCx, float* pCy, float* r)
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
		return -1;
	}
	else				
	{
		// ������D�Ļ�����
		numerator = y1*(x2*x2 + y2*y2) - y2*(x1*x1 + y1*y1);
		denominator = x1*y2 - x2*y1;

		D = numerator/denominator;

		// ����һ��y�п���Ϊ0
		if (y1 != 0.0)
			E = -(x1*x1 + y1*y1 + D*x1)/y1;
		else
			E = -(x2*x2 + y2*y2 + D*x2)/y2;

		// 
		float dx, dy;
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

		// ����ֵ
		if (pCx != NULL)
			*pCx = center_x;	

		if (pCy != NULL)
			*pCy = center_y;	

		if (r != NULL)
			*r = radius;
	}

	return 0;
}

// ͨ����״�����Բ�İ뾶
void Cad_Circle::UpdateCenterRadius()
{
	int dir;
	float Cx, Cy, r;

	dir = GetDirection();
	CalCenterRadius(&Cx, &Cy, &r);

	m_CenterX = Cx;
	m_CenterY = Cy;
	m_radius = r;
}

// ����Բ�İ뾶
void Cad_Circle::SetCenterRadius(float pCx, float pCy, float r)
{
	m_CenterX = pCx;
	m_CenterY = pCy;
	m_radius = r;
}
