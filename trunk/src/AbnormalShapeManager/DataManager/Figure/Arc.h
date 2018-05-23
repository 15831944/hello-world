/*--------------------------------------------------------------------------------------------------------------------*/
//	Arc.h      -- Բ���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.12
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef	__Arc_H__
#define __Arc_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Arc : public Figure
{
public:
	Cad_Arc();
	~Cad_Arc();


public:
	// ͼ�����
	virtual void MoveFigure(float rel_x, float rel_y);					// �ƶ�ͼ��
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// ����ƽ��ͼ��

public:
	// ��״��������ز���
	virtual void ReverseShapePointList(void);	// ������״������

public:
	// ������������ز���
	virtual void CalOutlineList(void);			// ����������		

public:
	virtual void CalShapePointOutlineInfo();	// ������״���������Ϣ

public:
	int GetDirection(void);						// ��ȡ˳��ʱ�뷽��
	float GetCentralAngle(void);				// ��ȡԲ�Ľ�

public:
	void CalCenterRadiusInfo();					// ͨ����״�����Բ�İ뾶��Ϣ����������ͬ������ʹ��
	void CenterRadius2ShapePoint(void);			// ͨ��Բ�İ뾶������״�㣬һ���ڶ�ȡdxf��Ϣ��ʹ��
	int CalCenterRadius(float* pCx, float* pCy, float* r, float* s_angle, float* e_angle);		// ͨ����״�����Բ�İ뾶
	void SetCenterRadius(float pCx, float pCy, float r, float s_angle, float e_angle);			// ����Բ�İ뾶

public:
	float m_CenterX;
	float m_CenterY;
	float m_radius;
	float m_StartAngle;		// ��һ����״��ĽǶ�
	float m_EndAngle;		// ���һ����״��ĽǶ�

};

#endif