/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.h    -- �����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	������
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		CSingleton_H
#define		CSingleton_H

#include <vector>
using namespace std;

class Layer;
class Figure;

class AFX_EXT_CLASS CAbnormalToolsSingleton
{
public:
	static CAbnormalToolsSingleton* GetSingleton(void);

public:
	// ͼ����ز���
	int NewLayer(void);										// �¼�һ��ͼ��
	int AddLayer(Layer* pLayer);							// ���һ��ͼ��
	int DeleteLayer(int layer_id);							// ɾ��һ��ͼ��
	Layer* GetLayer(int layer_id);							// ��ȡͼ��
	int GetLayerNum(void);									// ��ȡͼ����
	void UpdateLayerInfo(void);								// ��������ͼ����Ϣ

	void ClearLayerList(void);								// ���ͼ������
	void ClearAllData(void);								// ���ȫ������

public:
	// ���������ݽ��в���
	int GetFigureNum();										// ��ȡͼ����
	void SortFigure();										// ͼ������
	void UpdateFigureInfo();								// ����ͼ����Ϣ
	void LinkFigureList();									// ����ͼ���е�ͼ��
	void CollectAllFigures(vector<Figure*>& FigureList);	// �ռ����е�ͼ��
	void CheckIfAllFiguresLegal(void);						// ����Ƿ�����ͼ�ζ��Ϸ� ɾ���Ǹ����Լ����պ�ͼ��ͼ��

public:
	vector<Layer*> m_LayerList;

public:
	// ����ѡ��ͼ��

	vector<Figure*> m_SelectFigureList;
	vector<Figure*>* GetSelectFigureList();


	void SelectOneFigure(Figure* pFig);					// ѡ��һ��ͼ��
	void UnSelectOneFigure(Figure* pFig);				// ȡ��ѡ��һ��ͼ��
	void SelectAllFigure(void);							// ѡ��ȫ��ͼ��
	void UnSelectAllFigure(void);						// ȡ��ѡ��ȫ��ͼ��
	void MoveSelectFigure(float rel_x, float rel_y);	// �ƶ�ѡ��ͼ��
	void DelSelectFigure(void);							// ɾ��ѡ��ͼ��


private:
	CAbnormalToolsSingleton();
	~CAbnormalToolsSingleton();
	static CAbnormalToolsSingleton* m_pSingleton;
};



#endif