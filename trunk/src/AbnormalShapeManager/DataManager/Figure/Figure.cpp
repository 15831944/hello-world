/*--------------------------------------------------------------------------------------------------------------------*/
//	Figure.cpp     -- ͼ����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Figure.h"
#include "../CommonData.h"
#include "../Layer/Layer.h"
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"


Figure::Figure()
{
	m_FigureID = 0;
	m_FigureType = FigureType_Unknow;
	m_pLayer = NULL;
	m_bSelect = FALSE;
	m_OutlineSpace = OUTLINE_SPACE;
}

Figure::~Figure()
{
	ClearShapePointList();
	ClearOutlineList();
}





/*--------------------------------------------------------------------------------------*/
/*                                 ͼ����ز���										    */
/*--------------------------------------------------------------------------------------*/

/*----------------------------------------------------------*/
//	purpose:
//		�ƶ�ͼ��
//		
//	param:
//		float rel_x -- �������x
//		float rel_y -- �������y
//
//	return:
//		void
/*----------------------------------------------------------*/
void Figure::MoveFigure(float rel_x, float rel_y)
{
	// �ƶ���״��
	GeneralInterface::MoveShapePointList(&m_ShapePointList, rel_x, rel_y);


	// �ƶ�����������
	GeneralInterface::MoveOutlineList(&m_OutlineList, rel_x, rel_y);

	// ���¼������
	CalFigureRect();
}

// �ж�ͼ���Ƿ�պ�
BOOL Figure::IsClosed()
{
	int nCount = GetShapePointNum();
	ShapePoint* pHead, *pTail;

	if (nCount < 2)
		return FALSE;

	pHead = GetHeadShapePoint();
	pTail = GetTailShapePoint();

	if (pHead->IsCloseTo(pTail))
		return TRUE;

	return FALSE;
}



// ����ͼ�γ��� �����߳������
float Figure::CalFigureLen(void)
{
	int i, nCount;
	float total_len = 0;

	nCount = GetOutlineNum();
	for (i = 0; i < nCount-1; i++)
	{
		float len;
		FPoint* pCurPnt = GetOutline(i);
		FPoint* pNextPnt = GetOutline(i+1);

		len = pCurPnt->DistanceTo(pNextPnt);
		total_len += len;
	}

	return total_len;
}


// ����ͼ�ξ���
BOOL Figure::CalFigureRect(int &max_x, int& max_y, int& min_x, int& min_y)
{
	float  maxx, maxy, minx, miny;
	
	CalFigureRect(maxx, maxy, minx, miny);

	max_x = (int)maxx;
	max_y = (int)maxy;
	min_x = (int)minx;
	min_y = (int)miny;

	return TRUE;
}

// ����ͼ�ξ���
BOOL Figure::CalFigureRect(float &max_x, float& max_y, float& min_x, float& min_y)
{
	int nShpPntNum, nOutlineNum;
	float ShpPntMax_x, ShpPntMax_y, ShpPntMin_x, ShpPntMin_y;
	float OutlineMax_x,OutlineMax_y, OutlineMin_x, OutlineMin_y;
	float maxx, maxy, minx, miny;

	nShpPntNum = GetShapePointNum();
	nOutlineNum = GetOutlineNum();
	if (nShpPntNum < 1 && nOutlineNum < 1)
		return FALSE;
	else if (nShpPntNum > 1 && nOutlineNum < 1)
	{
		// ֻ������״��
		CalShapePointRect(max_x, max_y, min_x, min_y);
	}
	else if (nShpPntNum < 1 && nOutlineNum > 1)
	{
		// ֻ����������
		CalOutlineRect(max_x, max_y, min_x, min_y);
	}
	else
	{
		// ����������
		CalShapePointRect(ShpPntMax_x, ShpPntMax_y, ShpPntMin_x, ShpPntMin_y);
		CalOutlineRect(OutlineMax_x, OutlineMax_y, OutlineMin_x, OutlineMin_y);

		maxx = ShpPntMax_x > OutlineMax_x ? ShpPntMax_x : OutlineMax_x;
		minx = ShpPntMin_x < OutlineMin_x ? ShpPntMin_x : OutlineMin_x;
		maxy = ShpPntMax_y > OutlineMax_y ? ShpPntMax_y : OutlineMax_y;
		miny = ShpPntMin_y < OutlineMin_y ? ShpPntMin_y : OutlineMin_y;

		max_x = maxx;
		max_y = maxy;
		min_x = minx;
		min_y = miny;
	}

	return TRUE;
}

// ����ͼ����״�����
BOOL Figure::CalShapePointRect(float &max_x, float& max_y, float& min_x, float& min_y)
{
	int i, nCount;
	float x, y, maxx, maxy, minx, miny;
	ShapePoint* pPnt = NULL;

	nCount = GetShapePointNum();
	if (nCount < 1)
		return FALSE;

	// ���ʼ��
	pPnt = GetHeadShapePoint();
	pPnt->GetXY(minx, miny);
	pPnt->GetXY(maxx, maxy);

	for (i = 1; i < nCount; i++)
	{
		pPnt = GetShapePoint(i);
		pPnt->GetXY(x, y);

		if (x > maxx)
			maxx = x;

		if (x < minx)
			minx = x;

		if (y > maxy)
			maxy = y;

		if (y < miny)
			miny = y;
	}

	max_x = maxx;
	max_y = maxy;
	min_x = minx;
	min_y = miny;

	return TRUE;
}

// ����ͼ�����������
BOOL Figure::CalOutlineRect(float &max_x, float& max_y, float& min_x, float& min_y)	
{
	int i, nCount;
	float x, y, maxx, maxy, minx, miny;
	FPoint* pPnt = NULL;

	nCount = GetOutlineNum();
	if (nCount < 1)
		return FALSE;

	// ���ʼ��
	pPnt = GetOutline(0);
	pPnt->GetXY(minx, miny);
	pPnt->GetXY(maxx, maxy);

	for (i = 1; i < nCount; i++)
	{
		pPnt = GetOutline(i);
		pPnt->GetXY(x, y);

		if (x > maxx)
			maxx = x;

		if (x < minx)
			minx = x;

		if (y > maxy)
			maxy = y;

		if (y < miny)
			miny = y;
	}

	max_x = maxx;
	max_y = maxy;
	min_x = minx;
	min_y = miny;

	return TRUE;
}




// ����ͼ�ξ���
BOOL Figure::CalFigureRect()
{
	BOOL rtn;
	int maxx, maxy, minx, miny;

	rtn = CalFigureRect(maxx, maxy, minx, miny);
	if (rtn == TRUE)
	{
		m_FigureRect.SetRect(maxx, maxy, minx, miny);
		m_FigureRect.NormalizeRect();
	}

	return rtn;
}






// ������״����������Ϣ
void Figure::CalShapePointOutlineInfo(void)
{
	int i, nCount;
	ShapePoint* pPnt = NULL;

	nCount = GetShapePointNum();
	for (i = 0; i < nCount; i++)
	{
		pPnt = GetShapePoint(i);

		pPnt->m_radius = 0;
		pPnt->m_sign = Sign_Line;
		pPnt->m_dir = Dir_Line;
		pPnt->m_cut = Cut_KnifeDown;
		pPnt->m_type = Type_Regular;
		pPnt->m_group = Group_Regular;

		// ͼ��0Ϊ���У�����ͼ��Ϊ����
		if (m_pLayer != NULL && m_pLayer->m_LayerID > 0)
			pPnt->m_side = Side_In;
		else
			pPnt->m_side = Side_Out;
	}
}



Figure* Figure::CalParallelFigure(int dir , float dist)
 {
	 return NULL;
 }



/*--------------------------------------------------------------------------------------*/
/*                                 ��״��������ز���                                   */
/*--------------------------------------------------------------------------------------*/

// ���һ����״��
int Figure::AddShapePoint(ShapePoint* pShapePoint)
{
	if (pShapePoint != NULL)
	{
		m_ShapePointList.push_back(pShapePoint);
		UpdateShapePointInfo();
		return 0;
	}

	return -1;
}

// ɾ��һ����״��
int Figure::DeleteShapePoint(int ShapePoint_id)
{
	int nCount;
	vector<ShapePoint*>::iterator it;

	// �����ж�
	nCount = m_ShapePointList.size();
	if (ShapePoint_id >= nCount)
		return -1;

	it = m_ShapePointList.begin() + ShapePoint_id;
	if (it != m_ShapePointList.end())
	{
		m_ShapePointList.erase(it);
		return 0;
	}

	return -1;
}


// ��ȡ��״������
ShapePoint* Figure::GetShapePoint(int ShapePoint_id)
{
	int nCount;
	vector<ShapePoint*>::iterator it;

	// �����ж�
	nCount = m_ShapePointList.size();
	if (ShapePoint_id >= nCount)
		return NULL;

	it = m_ShapePointList.begin() + ShapePoint_id;
	if (it != m_ShapePointList.end())
	{
		return *it;
	}

	return NULL;
}

// ����������״����Ϣ
void Figure::UpdateShapePointInfo(void)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	nCount = m_ShapePointList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_ShapePointList.begin() + i;
		(*it)->m_PntID = i;
		(*it)->m_pFigure = this;
		(*it)->m_FigureType = m_FigureType;
	}
}

// ɾ����״������
void Figure::ClearShapePointList()
{
	GeneralInterface::ClearShapePointList(&m_ShapePointList);
}

// ������״������
void Figure::ReverseShapePointList()
{
	GeneralInterface::ReverseShapePointList(&m_ShapePointList);
}

// ��ȡ��״������
int Figure::GetShapePointNum(void)
{
	return m_ShapePointList.size();
}

// ��ȡ��һ����״��
ShapePoint* Figure::GetHeadShapePoint(void)	
{
	int nCount = GetShapePointNum();

	if (nCount > 0)
		return m_ShapePointList[0];
	else
		return NULL;
}
	
// ��ȡ���һ����״��	
ShapePoint* Figure::GetTailShapePoint(void)	
{
	int nCount = GetShapePointNum();

	if (nCount > 0)
		return m_ShapePointList[nCount-1];
	else
		return NULL;
}



/*--------------------------------------------------------------------------------------*/
/*                                ������������ز���                                    */
/*--------------------------------------------------------------------------------------*/

// ���һ�������ߵ�
int Figure::AddOutline(FPoint* pPnt)
{
	if (pPnt != NULL)
	{
		m_OutlineList.push_back(pPnt);
		UpdateOutlineInfo();
		return 0;
	}

	return -1;
}

// ��ȡ������
FPoint* Figure::GetOutline(int Point_id)
{
	int nCount;
	vector<FPoint*>::iterator it;

	// �����ж�
	nCount = m_OutlineList.size();
	if (Point_id >= nCount)
		return NULL;

	it = m_OutlineList.begin() + Point_id;
	if (it != m_OutlineList.end())
	{
		return *it;
	}

	return NULL;
}


// ɾ��һ�������ߵ�
int Figure::DeleteOutline(int Pnt_id)
{
	int nCount;
	vector<FPoint*>::iterator it;

	// �����ж�
	nCount = m_OutlineList.size();
	if (Pnt_id >= nCount)
		return -1;

	it = m_OutlineList.begin() + Pnt_id;
	if (it != m_OutlineList.end())
	{
		m_OutlineList.erase(it);
		return 0;
	}

	return -1;
}

// ��ȡ�����ߵ�����
int Figure::GetOutlineNum(void)
{
	return m_OutlineList.size();
}


// �������������ߵ���Ϣ
void Figure::UpdateOutlineInfo(void)
{
	int i, nCount;
	vector<FPoint*>::iterator it;

	nCount = m_OutlineList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_OutlineList.begin() + i;
		(*it)->m_PntID = i;
		(*it)->m_pFigure = this;
	}
}






// ���������� ����ֱ��ֱ�ӽ���״��תΪ�����߼���
void Figure::CalOutlineList(void)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	// �����
	ClearOutlineList();

	nCount = m_ShapePointList.size();
	for (i = 0; i < nCount; i++)
	{
		FPoint *pPnt = NULL;

		it = m_ShapePointList.begin() + i;
		pPnt = new FPoint((*it)->m_x, (*it)->m_y);
		m_OutlineList.push_back(pPnt);
	}

	UpdateOutlineInfo();
}

// �������������
void Figure::ClearOutlineList(void)
{
	GeneralInterface::ClearOutlineList(&m_OutlineList);
}


/*----------------------------------------------------------*/
//	purpose:
//		ͼ���Ƿ�ѡ��
//		
//	param:
//		void
//
//	return:
//		TRUE �� ѡ��
//		FALSE:  ûѡ��
/*----------------------------------------------------------*/
BOOL Figure::IsSelect(void)
{
	return m_bSelect;
}

// ѡ��
void Figure::Select(void)
{
	m_bSelect = TRUE;
}

// ȡ��ѡ��
void Figure::UnSelect(void)
{
	m_bSelect = FALSE;
}