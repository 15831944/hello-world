/*--------------------------------------------------------------------------------------------------------------------*/
//	FPoint.cpp     -- �����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FPoint.h"
#include "../Figure/Figure.h"
#include "../CommonData.h"

// ����
FPoint::FPoint()
{
	m_x = 0;
	m_y = 0;
	m_PntID = 0;
	m_pFigure = NULL;
}


FPoint::FPoint(float x, float y)
{
	m_x = x;
	m_y = y;
}

// ����
FPoint::~FPoint()
{

}

// ����XY����
void FPoint::SetXY(float x, float y)		
{
	m_x = x;
	m_y = y;
}

// ��ȡXY����
void FPoint::GetXY(float& x, float& y)
{
	x = m_x;
	y = m_y;
}


// �ܿ����������غ�
BOOL FPoint::IsCloseTo(FPoint& pnt)
{
	float dx, dy, dis;
	
	dx = pnt.m_x - m_x;
	dy = pnt.m_y - m_y;

	dis	 = sqrt(dx*dx + dy*dy);

	if (dis < CLOSE_VALUE)
		return TRUE;

	return FALSE;
}

// �ܿ����������غ�
BOOL FPoint::IsCloseTo(FPoint* pPnt)
{
	if (pPnt != NULL)
	{
		float dx, dy, dis;

		dx = pPnt->m_x - m_x;
		dy = pPnt->m_y - m_y;

		dis	 = sqrt(dx*dx + dy*dy);

		if (dis < CLOSE_VALUE)
			return TRUE;
	}

	return FALSE;
}

// �����
float FPoint::DistanceTo(FPoint& pnt)
{
	float dx, dy, dis;

	dx = pnt.m_x - m_x;
	dy = pnt.m_y - m_y;

	dis	 = sqrt(dx*dx + dy*dy);

	return dis;
}

// �����
float FPoint::DistanceTo(FPoint* pPnt)
{
	float dx, dy, dis = 0;

	if (pPnt != NULL)
	{
		dx = pPnt->m_x - m_x;
		dy = pPnt->m_y - m_y;

		dis	 = sqrt(dx*dx + dy*dy);
	}

	return dis;
}




// +����������
FPoint FPoint::operator+(FPoint& pnt)
{
	m_x += pnt.m_x;
	m_y += pnt.m_y;

	return *this;
}

// -����������
FPoint FPoint::operator-(FPoint& pnt)			
{
	m_x -= pnt.m_x;
	m_y -= pnt.m_y;

	return *this;
}

// =����������
FPoint& FPoint::operator = (const FPoint& pnt)
{
	m_x = pnt.m_x;
	m_y = pnt.m_y;
	m_PntID = pnt.m_PntID;	
	m_pFigure = pnt.m_pFigure;

	return *this;
}