/*--------------------------------------------------------------------------------------------------------------------*/
//	Circle.h   -- Բ�������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.19
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef	__Circle_H__
#define __Circle_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Circle : public Figure
{
public:
	Cad_Circle();
	~Cad_Circle();

public:
	// ������������ز���
	virtual void CalOutlineList(void);			// ����������		

public:
	int GetDirection(void);						// ��ȡ˳��ʱ�뷽��
	void UpdateCenterRadius();					// ͨ����״�����Բ�İ뾶
 	void CenterRadius2ShapePoint(void);			// ͨ��Բ�İ뾶������״��
 	int CalCenterRadius(float* pCx, float* pCy, float* r);		// ͨ����״�����Բ�İ뾶
	void SetCenterRadius(float pCx, float pCy, float r);

public:
	float m_CenterX;
	float m_CenterY;
	float m_radius;
};

#endif