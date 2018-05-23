/*--------------------------------------------------------------------------------------------------------------------*/
//	ShapePoint.h      -- ��״���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "ShapePoint.h"
#include "../CommonData.h"

// ����
ShapePoint::ShapePoint()
{
	m_FigureType = FigureType_Unknow;		// ͼ������

	m_radius = 0;				// Բ���İ뾶��ֱ�ߵ�Ϊ0
	m_sign = 0;					// 0��ֱ�ߵ� 1:Բ����� 2��Բ���յ� 3������Բ�����ӵ� -1�����账��ĵ�
	m_dir = 0;					// 0��ֱ�ߵ� 1����ʱ�� 2��˳ʱ��
	m_cut = 0;					// 0������ 1����Ҫ�� 2����ʾ�����µ�
	m_type = 0;					// 0������ 1��CNC��ϳ��С����ת���ף�
	m_group = Group_Regular;	// 0������(ͼ���еĵ�) 1��ͼ�ο�ʼ  2��ͼ�ν���
	m_side = Side_Out;			// 1������  2������   3��������
	
	m_x = 0;
	m_y = 0;
}


// ����
ShapePoint::ShapePoint(float x, float y)
{
	m_FigureType = FigureType_Unknow;		// ͼ������

	m_radius = 0;		// Բ���İ뾶��ֱ�ߵ�Ϊ0
	m_sign = 0;			// 0��ֱ�ߵ� 1:Բ����� 2��Բ���յ� 3������Բ�����ӵ� -1�����账��ĵ�
	m_dir = 0;			// 0��ֱ�ߵ� 1����ʱ�� 2��˳ʱ��
	m_cut = 0;			// 0������ 1����Ҫ�� 2����ʾ�����µ�
	m_type = 0;			// 0������ 1��CNC��ϳ��С����ת���ף�
	m_group = Group_Regular;	// 0������(ͼ���еĵ�) 1��ͼ�ο�ʼ  2��ͼ�ν���
	m_side = Side_Out;			// 1������  2������   3��������

	m_x = x;
	m_y = y;
}

// ����
ShapePoint::~ShapePoint()
{

}


// +����������
ShapePoint ShapePoint::operator+(ShapePoint& pnt)
{
	m_x += pnt.m_x;
	m_y += pnt.m_y;

	return *this;
}

// -����������
ShapePoint ShapePoint::operator-(ShapePoint& pnt)			
{
	m_x -= pnt.m_x;
	m_y -= pnt.m_y;

	return *this;
}


// =����������
ShapePoint& ShapePoint::operator = (const ShapePoint& pnt)
{
	m_x = pnt.m_x;
	m_y = pnt.m_y;
	m_PntID = pnt.m_PntID;	
	m_pFigure = pnt.m_pFigure;

	m_FigureType = pnt.m_FigureType;
	m_radius = pnt.m_radius;		
	m_sign = pnt.m_sign;			
	m_dir = pnt.m_dir;			
	m_cut = pnt.m_cut;			
	m_type = pnt.m_type;	
	m_group = pnt.m_group;			
	m_side = pnt.m_side;	

	return *this;
}