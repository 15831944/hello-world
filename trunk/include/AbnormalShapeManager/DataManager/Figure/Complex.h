/*--------------------------------------------------------------------------------------------------------------------*/
//	Complex.h  -- ����ͼ���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.12
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		__Complex_H__
#define		__Complex_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Complex : public Figure
{
public:
	Cad_Complex();
	~Cad_Complex();

public:
	// ͼ��
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// ����ƽ��ͼ��

public:
	// ��״���������
	virtual void ReverseShapePointList(void);				// ������״������
	virtual void UpdateShapePointInfo(void);				// ����������״����Ϣ


public:
	virtual void CalShapePointOutlineInfo(void);			// ������״����������Ϣ

public:
	void ClearComplexFigureList();		// ��ո���ͼ������
	void CalComplexFigureList(void);	// ���㸴��ͼ������
	Figure* GetFigureFromShapePoint(int start_id, int end_id, int type);


	int AddFigure(Figure* pFigure);		// ���һ��ͼ��
	void UpdateFigureInfo(void);		// ��������ͼ����Ϣ

public:
	// ������������ز���
	virtual void CalOutlineList(void);						// ����������		


public:
	vector<Figure*> m_ComplexFigureList;

};

#endif