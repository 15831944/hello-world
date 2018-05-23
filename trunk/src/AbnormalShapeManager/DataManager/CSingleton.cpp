/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.cpp    -- ������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "CSingleton.h"
#include "Layer/Layer.h"
#include "Figure/Figure.h"
#include "../GeneralInterface/GeneralInterface.h"
#include <algorithm>
#include "CommonData.h"


// ����ʵ���ʼ��
CAbnormalToolsSingleton* CAbnormalToolsSingleton::m_pSingleton = NULL;

CAbnormalToolsSingleton* CAbnormalToolsSingleton::GetSingleton(void)
{
	if (m_pSingleton == NULL)
		m_pSingleton = new CAbnormalToolsSingleton;
	
	return m_pSingleton;
}

// ����
CAbnormalToolsSingleton::CAbnormalToolsSingleton()
{

}

// ����
CAbnormalToolsSingleton::~CAbnormalToolsSingleton()
{
	delete m_pSingleton;
}


// ͼ����ز���

// �¼�һ��ͼ��
int CAbnormalToolsSingleton::NewLayer(void)
{
	Layer* pNewLayer = new Layer;
	return AddLayer(pNewLayer);
}

// ���һ��ͼ��
int CAbnormalToolsSingleton::AddLayer(Layer* pLayer)
{
	if (pLayer != NULL)
	{
		m_LayerList.push_back(pLayer);
		return 0;
	}
	
	return -1;
}

// ɾ��һ��ͼ��
int CAbnormalToolsSingleton::DeleteLayer(int layer_id)
{
	int nCount;
	vector<Layer*>::iterator it;

	// �����ж�
	nCount = m_LayerList.size();
	if (layer_id >= nCount)
		return -1;

	it = m_LayerList.begin() + layer_id;
	if (it != m_LayerList.end())
	{
		m_LayerList.erase(it);
		return 0;
	}

	return -1;
}

// ��ȡͼ��
Layer* CAbnormalToolsSingleton::GetLayer(int layer_id)
{
	int nCount;
	vector<Layer*>::iterator it;

	// �����ж�
	nCount = m_LayerList.size();
	if (layer_id >= nCount)
		return NULL;

	it = m_LayerList.begin() + layer_id;
	if (it != m_LayerList.end())
	{
		return *(it);
	}

	return NULL;
}

// ��ȡͼ����
int CAbnormalToolsSingleton::GetLayerNum(void)
{
	return m_LayerList.size();
}

// ��������ͼ����Ϣ
void CAbnormalToolsSingleton::UpdateLayerInfo(void)
{
	int i, nCount;
	vector<Layer*>::iterator it;

	nCount = m_LayerList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_LayerList.begin() + i;
		(*it)->m_LayerID = i;
	}
}

// ���ͼ������
void CAbnormalToolsSingleton::ClearLayerList(void)
{
	GeneralInterface::ClearLayerList(&m_LayerList);
}

// ���ȫ������
void CAbnormalToolsSingleton::ClearAllData(void)		
{
	ClearLayerList();

	m_SelectFigureList.clear();
}



/*--------------------------------------------------------------------------------------*/
/*                                �������ݽ��в���										*/
/*--------------------------------------------------------------------------------------*/

// ��ȡͼ����
int CAbnormalToolsSingleton::GetFigureNum()
{
	int i, nLayerCount, nFigCount = 0;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		nFigCount += pLayer->GetFigureNum(); 
	}

	return nFigCount;
}


// ͼ������ ����ͼ������ǰʹ��
void CAbnormalToolsSingleton::SortFigure()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->SortFigureListByArcFirst(); 
	}

}

// ����ͼ����Ϣ
void CAbnormalToolsSingleton::UpdateFigureInfo()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->UpdateFigureInfo(); 
	}
}

// ����ͼ���е�ͼ��
void CAbnormalToolsSingleton::LinkFigureList()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->LinkFigureList(); 
	}
}

// �ռ����е�ͼ�Σ�׼��дxml�ļ��򷵻���������Ϣ�ַ���������Ҫ���ݾ��δ�С����,���δ������ǰ��
void CAbnormalToolsSingleton::CollectAllFigures(vector<Figure*>& FigureList)
{
	int i_fig, i_layer, nLayerNum, nFigNum;

	nLayerNum = GetLayerNum();
	for (i_layer = 0; i_layer < nLayerNum; i_layer++)
	{
		Layer* pLayer = GetLayer(i_layer);

		pLayer->SortFigureListByRect();	// ����ȷ����Χ����������ǰ��

		nFigNum = pLayer->GetFigureNum();
		for (i_fig = 0; i_fig < nFigNum; i_fig++)
		{
			Figure* pFig = pLayer->GetFigure(i_fig);

			pFig->CalShapePointOutlineInfo();	// ������������״����Ϣ

			FigureList.push_back(pFig);
		}
	}
}


// ����Ƿ�����ͼ�ζ��Ϸ� ɾ���Ǹ����Լ����պ�ͼ��ͼ��
void CAbnormalToolsSingleton::CheckIfAllFiguresLegal(void)
{
	int i_fig, i_layer, nLayerNum, nFigNum;

	nLayerNum = GetLayerNum();
	for (i_layer = 0; i_layer < nLayerNum; i_layer++)
	{
		Layer* pLayer = GetLayer(i_layer);

		nFigNum = pLayer->GetFigureNum();
		for (i_fig = 0; i_fig < nFigNum; i_fig++)
		{
			Figure* pFig = pLayer->GetFigure(i_fig);

			if (pFig->IsClosed() == FALSE)
			{
				// ���պ�
// 				pLayer->DeleteFigure(pFig->m_FigureID);
// 				i_fig = 0;
// 				nFigNum = pLayer->GetFigureNum();
				AfxMessageBox("��⵽�в��պϵ�ͼ�Σ���ͼ���Ƶ��м䣬��Ctrl+Aȫѡͼ�Σ��Ŵ󣬲鿴С���Σ������޸�dxf�ļ������µ��룬��������");
			}
			else if (pFig->m_FigureType != FigureType_Complex)
			{
				// δ���ӵĵ�����ͼ��
// 				pLayer->DeleteFigure(pFig->m_FigureID);
// 				i_fig = 0;
// 				nFigNum = pLayer->GetFigureNum();
				AfxMessageBox("��⵽�в��պϵ�ͼ�Σ���ͼ���Ƶ��м䣬��Ctrl+Aȫѡͼ�Σ��Ŵ󣬲鿴С���Σ������޸�dxf�ļ������µ��룬��������");
			}
			else if (pFig->CalFigureLen() < FIGURE_MIN_LEN)
			{
				AfxMessageBox("��⵽��С��10mm��ͼ�Σ���ͼ���Ƶ��м䣬��Ctrl+Aȫѡͼ�Σ��Ŵ󣬲鿴С���Σ������Ƿ������Ҫ��ͼ�ι�С���ܻᵼ�³���");
			}
		}
	}
}



/*--------------------------------------------------------------------------------------*/
/*                                ͼ��ѡ����ز���										*/
/*--------------------------------------------------------------------------------------*/


/*----------------------------------------------------------*/
//	purpose:
//		��ȡѡ��ͼ������
//		
//	param:
//		
//
//	return:
//		vector<Figure*>* -- ����
/*----------------------------------------------------------*/
vector<Figure*>* CAbnormalToolsSingleton::GetSelectFigureList()
{
	return &m_SelectFigureList;
}

/*----------------------------------------------------------*/
//	purpose:
//		ѡ��һ��ͼ��
//		
//	param:
//		Figure* pFig -- ͼ��ָ��
//
//	return:
//		void
/*----------------------------------------------------------*/
void CAbnormalToolsSingleton::SelectOneFigure(Figure* pFig)
{
	if (pFig != NULL)
	{
		pFig->Select();		// ѡ��ͼ��

		// δ��ӹ�������ӽ���ѡ��ͼ������
		vector<Figure*>::iterator result = std::find(m_SelectFigureList.begin(), m_SelectFigureList.end(), pFig); 
		if ( result == m_SelectFigureList.end( ))
			m_SelectFigureList.push_back(pFig);
	}
}

// ȡ��ѡ��һ��ͼ��
void CAbnormalToolsSingleton::UnSelectOneFigure(Figure* pFig)
{
	if (pFig != NULL)
	{
		pFig->UnSelect();
 		vector<Figure*>::iterator result = std::find(m_SelectFigureList.begin(), m_SelectFigureList.end(), pFig); 

		if ( result != m_SelectFigureList.end( )) //�ҵ�
		{
			m_SelectFigureList.erase(result);
		}
	}
}

// ѡ��ȫ��ͼ��
void CAbnormalToolsSingleton::SelectAllFigure(void)
{
	int i, j, nLayerNum, nFigNum;
	Layer* pLayer = NULL;
	Figure* pFig = NULL;

	nLayerNum = GetLayerNum();
	for (i = 0; i < nLayerNum; i++)
	{
		pLayer = GetLayer(i);
		nFigNum = pLayer->GetFigureNum();

		for (j = 0; j < nFigNum; j++)
		{
			pFig = pLayer->GetFigure(j);
			SelectOneFigure(pFig);
		}
	}
}

// ȡ��ѡ��ȫ��ͼ��
void CAbnormalToolsSingleton::UnSelectAllFigure(void)
{
	int i, j, nLayerNum, nFigNum;
	Layer* pLayer = NULL;
	Figure* pFig = NULL;

	nLayerNum = GetLayerNum();
	for (i = 0; i < nLayerNum; i++)
	{
		pLayer = GetLayer(i);
		nFigNum = pLayer->GetFigureNum();

		for (j = 0; j < nFigNum; j++)
		{
			pFig = pLayer->GetFigure(j);
			UnSelectOneFigure(pFig);
		}
	}

	m_SelectFigureList.clear();
}

// �ƶ�ѡ��ͼ��
void CAbnormalToolsSingleton::MoveSelectFigure(float rel_x, float rel_y)
{
	int i, nFigNum;
	Figure* pFig = NULL;

	nFigNum = m_SelectFigureList.size();
	for (i = 0; i < nFigNum; i++)
	{
		pFig = m_SelectFigureList.at(i);
		pFig->MoveFigure(rel_x, rel_y);
		pFig->CalFigureRect();
	}
}


// ɾ��ѡ��ͼ��
void CAbnormalToolsSingleton::DelSelectFigure(void)
{
	int i, nFigNum;
	Figure* pFig = NULL;
	Layer* pLayer = NULL;

	nFigNum = m_SelectFigureList.size();
	for (i = 0; i < nFigNum; i++)
	{
		pFig = m_SelectFigureList.at(i);
		pLayer = pFig->m_pLayer;

		pLayer->DeleteFigure(pFig->m_FigureID);
		pLayer->UpdateFigureInfo();
	}
}