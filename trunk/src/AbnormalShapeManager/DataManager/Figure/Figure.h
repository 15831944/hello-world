/*--------------------------------------------------------------------------------------------------------------------*/
//	Figure.h     -- ͼ���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		Figure_H
#define		Figure_H

#include <vector>
using namespace std;

class Layer;
class ShapePoint;
class FPoint;

class AFX_EXT_CLASS Figure
{
public:
	Figure();
	virtual ~Figure();

public:
	// ͼ�����
	virtual void MoveFigure(float rel_x, float rel_y);		// �ƶ�ͼ��

	virtual BOOL IsClosed();				// �ж�ͼ���Ƿ�պ�

	virtual float CalFigureLen(void);		// ����ͼ�γ���
	virtual BOOL CalFigureRect();			// ����ͼ�ξ���
	virtual BOOL CalFigureRect(int &max_x, int& max_y, int& min_x, int& min_y);				// ����ͼ�ξ���
	virtual BOOL CalFigureRect(float &max_x, float& max_y, float& min_x, float& min_y);		// ����ͼ�ξ���
	virtual BOOL CalShapePointRect(float &max_x, float& max_y, float& min_x, float& min_y);	// ����ͼ����״�����
	virtual BOOL CalOutlineRect(float &max_x, float& max_y, float& min_x, float& min_y);	// ����ͼ�����������

public:
	virtual void CalShapePointOutlineInfo(void);			// ������״����������Ϣ
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// ����ƽ����״������

public:
	// ��״����ز���
	virtual int AddShapePoint(ShapePoint* pShapePoint);		// ���һ����״��
	virtual int DeleteShapePoint(int ShapePoint_id);		// ɾ��һ����״��
	virtual ShapePoint* GetShapePoint(int ShapePoint_id);	// ��ȡ��״������
	virtual int GetShapePointNum(void);						// ��ȡ��״������
	virtual ShapePoint* GetHeadShapePoint(void);			// ��ȡ��һ����״��
	virtual ShapePoint* GetTailShapePoint(void);			// ��ȡ���һ����״��
	virtual void UpdateShapePointInfo(void);				// ����������״����Ϣ
	
	// ��״��������ز���
	virtual void ReverseShapePointList(void);				// ������״������
	virtual void ClearShapePointList(void);					// �����״������
	


public:
	// ��������ز���
	virtual int AddOutline(FPoint* pPnt);					// ���һ��������
	virtual FPoint* GetOutline(int Point_id);				// ��ȡ����������
	virtual int DeleteOutline(int Pnt_id);					// ɾ��һ��������
	virtual int GetOutlineNum(void);						// ��ȡ�����ߵ�����
	virtual void UpdateOutlineInfo(void);					// ����������������Ϣ

	// ������������ز���
	virtual void CalOutlineList(void);						// ����������		
	virtual void ClearOutlineList(void);					// �������������

public:
	int m_FigureID;			// ͼ�κ�
	int m_FigureType;		// ͼ������
	Layer* m_pLayer;		// ͼ��
	float m_OutlineSpace;	// ��������

public:
	vector<ShapePoint*> m_ShapePointList;	// ��״������
	vector<FPoint*> m_OutlineList;			// ����������

public:
	// ѡ�����
	BOOL	m_bSelect;
	CRect	m_FigureRect;

	BOOL IsSelect(void);
	void Select(void);
	void UnSelect(void);

};	



#endif