/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.cpp    -- ������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Layer.h"

#include "../Figure/Figure.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "../Point/ShapePoint.h"
#include "../CommonData.h"

Layer::Layer()
{
	m_LayerID = 0;
}

Layer::~Layer()
{

}

	
// ͼ����ز���

// ���һ��ͼ��
int Layer::AddFigure(Figure* pFigure)
{
	if (pFigure != NULL)
	{
		m_FigureList.push_back(pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}


// ���һ��ͼ������ǰ��
int Layer::AddFigureToHead(Figure* pFigure)
{
	if (pFigure != NULL)
	{
		m_FigureList.insert(m_FigureList.begin(), pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}

// ���һ��ͼ��
int Layer::GetFigureNum(void)
{
	return m_FigureList.size();
}

// ��ȡһ��ͼ��
Figure* Layer::GetFigure(int figure_id)
{
	int nCount;
	vector<Figure*>::iterator it;

	// �����ж�
	nCount = m_FigureList.size();
	if (figure_id >= nCount)
		return NULL;

	it = m_FigureList.begin() + figure_id;
	if (it != m_FigureList.end())
	{
		return *(it);
	}

	return NULL;
}


// ɾ��һ��ͼ��
int Layer::DeleteFigure(int figure_id)
{
	int nCount;
	vector<Figure*>::iterator it;

	// �����ж�
	nCount = m_FigureList.size();
	if (figure_id >= nCount)
		return -1;

	it = m_FigureList.begin() + figure_id;
	if (it != m_FigureList.end())
	{
		// �ͷ��ڴ�
		delete (*it);

		m_FigureList.erase(it);
		UpdateFigureInfo();			// ����ͼ����Ϣ

		return 0;
	}


	return -1;
}

// ��������ͼ����Ϣ
void Layer::UpdateFigureInfo(void)
{
	int i, nCount;
	vector<Figure*>::iterator it;

	nCount = m_FigureList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_FigureList.begin() + i;
		(*it)->m_FigureID = i;				// ����ͼ��ID
		(*it)->m_pLayer = this;				// ����ͼ��ָ��

		(*it)->CalFigureRect();				// ����ͼ�ξ���
	}


}

// ����ͼ��
void Layer::LinkFigureList(void)
{

// ѭ����ǩ
loop_label:
	int i = 0;
	int j = 0;
	int nCount = GetFigureNum();

	for (; i < nCount-1; i++)
	{
		for (j = i+1; j < nCount; j++)
		{
			Figure* pSrc, *pDst;
			
			pSrc = GetFigure(i);
			pDst = GetFigure(j);

			if (GeneralInterface::LinkFigure(pSrc, pDst) == 0)
				goto loop_label;
		}
	}
}

// ͼ������Բ������
void Layer::SortFigureListByArcFirst()
{
	int i, nCount;
	Figure* pFig;
	vector<Figure*>  tmp_list;

	nCount = GetFigureNum();
	for (i = 0; i < nCount; i++)
	{
		pFig = GetFigure(i);

		if (pFig->m_FigureType == FigureType_Arc)
			tmp_list.insert(tmp_list.begin(), pFig);
		else
			tmp_list.push_back(pFig);
	}

	m_FigureList.clear();
	m_FigureList.insert(m_FigureList.begin(),tmp_list.begin(), tmp_list.end());

	UpdateFigureInfo();
}

// ͼ���������� -- �����δ�С	
void Layer::SortFigureListByRect()
{
	GeneralInterface::SortFigureListByRect(&m_FigureList);
	UpdateFigureInfo();
}