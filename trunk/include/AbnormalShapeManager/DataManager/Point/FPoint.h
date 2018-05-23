/*--------------------------------------------------------------------------------------------------------------------*/
//	FPoint.h     -- ����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		FPoint_H
#define		FPoint_H

class Figure;

class AFX_EXT_CLASS FPoint
{
public:
	FPoint();
	FPoint(float x, float y);
	~FPoint();

public:
	void SetXY(float x, float y);		// ����XY����
	void GetXY(float& x, float& y);		// ��ȡXY����

public:
	BOOL IsCloseTo(FPoint& pnt);		// �ܿ����������غ�
	BOOL IsCloseTo(FPoint* pPnt);		// �ܿ����������غ�
	float DistanceTo(FPoint& pnt);		// �����
	float DistanceTo(FPoint* pPnt);		// �����

public:
	// ����������
	FPoint operator+(FPoint& pnt);			// +����������
	FPoint operator-(FPoint& pnt);			// -����������
	FPoint& operator = (const FPoint& pnt);	// =����������

public:
	float m_x;
	float m_y;
	int	m_PntID;	

	Figure* m_pFigure;
};


#endif
