/*--------------------------------------------------------------------------------------------------------------------*/
//	ShapePoint.h      -- ��״���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		ShapePoint_H
#define		ShapePoint_H

#include "FPoint.h"

class Figure;

class AFX_EXT_CLASS ShapePoint : public FPoint
{
public:
	ShapePoint();
	ShapePoint(float x, float y);
	~ShapePoint();


public:
	// ����������
	ShapePoint operator+(ShapePoint& pnt);			// +����������
	ShapePoint operator-(ShapePoint& pnt);			// -����������
	ShapePoint& operator = (const ShapePoint& pnt);	// =����������


public:
	int m_FigureType;

public:
	float m_radius;		// Բ���İ뾶��ֱ�ߵ�Ϊ0
	int m_sign;			// 0��ֱ�ߵ� 1:Բ����� 2��Բ���յ� 3������Բ�����ӵ� -1�����账��ĵ�
	int m_dir;			// 0��ֱ�ߵ� 1����ʱ�� 2��˳ʱ��
	int m_cut;			// 0������ 1����Ҫ�� 2����ʾ�����µ�
	int m_type;			// 0������ 1��CNC��ϳ��С����ת���ף�
	int m_group;		// 0������(ͼ���еĵ�) 1��ͼ�ο�ʼ  2��ͼ�ν���
	int m_side;			// 1������  2������   3��������
};
#endif