/*--------------------------------------------------------------------------------------------------------------------*/
//	Layer.h     -- ͼ���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		Layer_H
#define		Layer_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Figure;

class Layer
{
public:
	Layer();
	~Layer();

public:
	// ͼ����ز���
	int AddFigure(Figure* pFigure);				// ���һ��ͼ��
	int AddFigureToHead(Figure* pFigure);		// ���һ��ͼ������ǰ��
	int GetFigureNum(void);						// ���һ��ͼ��
	Figure* GetFigure(int figure_id);			// ���һ��ͼ��
	int DeleteFigure(int figure_id);			// ɾ��һ��ͼ��
	void UpdateFigureInfo(void);				// ��������ͼ����Ϣ

	void LinkFigureList(void);					// ����ͼ��

	void SortFigureListByArcFirst();			// ͼ����������Բ������
	void SortFigureListByRect();				// ͼ���������� -- �����δ�С	

public:
	int m_LayerID;

	vector<Figure*> m_FigureList;
};



#endif