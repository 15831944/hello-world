/*--------------------------------------------------------------------------------------------------------------------*/
//	GeneralInterface.cpp    -- ���������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.12
//	��ע��	���ݲ��������ӿ�
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "GeneralInterface.h"
#include "../DataManager/Layer/Layer.h"
#include "../DataManager/Figure/Arc.h"
#include "../DataManager/Figure/Complex.h"
#include "../DataManager/Point/ShapePoint.h"
#include "../DataManager/CommonData.h"




/*--------------------------------------------------------------------------------------*/
/*                                 ��������ز���										*/
/*--------------------------------------------------------------------------------------*/
// ���ͼ������
void GeneralInterface::ClearLayerList(vector<Layer*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<Layer*>::iterator it;

		// �ͷ��ڴ�
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Layer* pLayer = *it;

			delete pLayer;
			pLayer = NULL;
		}

		// �������
		pList->clear();
	}
}


/*--------------------------------------------------------------------------------------*/
/*                                 ͼ��������ز���                                     */
/*--------------------------------------------------------------------------------------*/


// ƽ��ͼ������
BOOL GeneralInterface::ParallelFigureList(vector<Figure*>* pSrcList, vector<Figure*>* pDstList, int dir, float dist)
{
	int i, nCount;

	nCount = pSrcList->size();
	for (i = 0; i < nCount; i++)
	{
		Figure* pFig = pSrcList->at(i);
		Figure* pParallelFig = pFig->CalParallelFigure(dir, dist);

		if (pParallelFig != NULL)
			pDstList->push_back(pParallelFig);
	}

	return TRUE;
}



// ���ͼ������
void GeneralInterface::ClearFigureList(vector<Figure*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<Figure*>::iterator it;

		// �ͷ��ڴ�
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Figure* pFig = *it;

			delete pFig;
			pFig = NULL;
		}

		// �������
		pList->clear();
	}
}


// ͼ���������� -- �����δ�С
void GeneralInterface::SortFigureListByRect(vector<Figure*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		CRect pre_rect, cur_rect;
		vector<Figure*>::iterator it;
		vector<Figure*> tmp_list;

		// �ͷ��ڴ�
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Figure* pFig = *it;
			Figure* pHeadFig = NULL;

			pFig->CalFigureRect();

			if (i == 0)
			{
				pre_rect = pFig->m_FigureRect;
				pre_rect.NormalizeRect();

				tmp_list.push_back(pFig);
			}
			else
			{
				cur_rect = pFig->m_FigureRect;
				cur_rect.NormalizeRect();

				pHeadFig = tmp_list.at(0);
				pre_rect = pHeadFig->m_FigureRect;

				int cur_width = cur_rect.Width();
				int cut_height = cur_rect.Height();
				int pre_width = pre_rect.Width();
				int pre_height = pre_rect.Height();
				

				// ��ǰ���δ���֮ǰ�ľ��Σ�����ǰ��
				if (cur_width >= pre_width && cut_height >= pre_height)
				{
					tmp_list.insert(tmp_list.begin(), pFig);
				}
				else
				{
					tmp_list.push_back(pFig);
				}
			}
		}

		pList->clear();
		*pList = tmp_list;

	}
}

// ��Բ���岹���ɸ���
vector<ShapePoint*> GeneralInterface::ArcInterpolate(ShapePoint* center, ShapePoint* start, ShapePoint* end, float r)
{
	float dx, dy, chord_len;
	float center_x, center_y, start_x, start_y, end_x, end_y;	// ԭʼ�߶ε�,ƽ�к���߶ε� 
	float vec_x,  vec_y;	// �岹ʱ�õ�����
	float second_Interpolate_x, second_Interpolate_y;	// �߶����ӵ�Բ���岹��

	ShapePoint* pStartPnt = new ShapePoint;									
	ShapePoint* pEndPnt = new ShapePoint;
	ShapePoint* pSecondInterPnt = new ShapePoint;
	vector<ShapePoint*> InterpolateVector;

	// ������ʼ��
	*pStartPnt = *start;
	*pEndPnt = *end;

	start->GetXY(start_x, start_y);
	end->GetXY(end_x, end_y);
	center->GetXY(center_x, center_y);

	// Բ��������
	dx = end_x - start_x;
	dy = end_y - start_y;

	// �ҳ�
	chord_len = sqrt(dx*dx + dy*dy);

	// �����ҽ��д���
	if(chord_len < ARC_INTERPOLATE_VALUE)
	{
		/*-------------ѹ�����------------*/
		InterpolateVector.push_back(pStartPnt);


		if (chord_len > ARC_INTERPOLATE_MIN_VALUE)
		{
			/*-------------���е�------------*/

			second_Interpolate_x = start_x + dx/2.0;
			second_Interpolate_y = start_y + dy/2.0;

			// Բ��ָ�����е������
			vec_x = second_Interpolate_x - center_x;
			vec_y = second_Interpolate_y - center_y;

			// �����淶����,���԰뾶�õ�ָ��Բ���е������
			NormalVector(vec_x, vec_y, vec_x, vec_y);
			vec_x *= r;
			vec_y *= r;

			// ���Բ���е�����
			second_Interpolate_x = center_x + vec_x;
			second_Interpolate_y = center_y + vec_y;

			// ѹ���е�
			pSecondInterPnt->SetXY(second_Interpolate_x, second_Interpolate_y);
			InterpolateVector.push_back(pSecondInterPnt);
		}

		/*-------------ѹ��β��------------*/
		InterpolateVector.push_back(pEndPnt);
	}
	else
	{
		// 1�� ��Բ���е�
		second_Interpolate_x = start_x + dx/2.0;
		second_Interpolate_y = start_y + dy/2.0;

		// Բ��ָ�����е������
		vec_x = second_Interpolate_x - center_x;
		vec_y = second_Interpolate_y - center_y;

		// �����淶����,���԰뾶�õ�ָ��Բ���е������
		NormalVector(vec_x, vec_y, vec_x, vec_y);
		vec_x *= r;
		vec_y *= r;

		// ���Բ���е�����
		second_Interpolate_x = center_x + vec_x;
		second_Interpolate_y = center_y + vec_y;
		pSecondInterPnt->SetXY(second_Interpolate_x, second_Interpolate_y);

		// 2��Բ������ɢ��
		Cad_Arc TmpArc;

		TmpArc.AddShapePoint(pStartPnt);
		TmpArc.AddShapePoint(pSecondInterPnt);
		TmpArc.AddShapePoint(pEndPnt);

		TmpArc.m_OutlineSpace = 0.5;
		TmpArc.CalCenterRadiusInfo();
		TmpArc.CalOutlineList();

		// 3����ɢ��ת�岹��
		int i, nCount;

		nCount = TmpArc.GetOutlineNum();
		for (i = 0; i < nCount; i++)
		{
			ShapePoint* pTmpShpPnt = new ShapePoint;
			FPoint* pTmpOutline = TmpArc.GetOutline(i);

			pTmpShpPnt->SetXY(pTmpOutline->m_x, pTmpOutline->m_y);
			InterpolateVector.push_back(pTmpShpPnt);
		}
	}

	return InterpolateVector;
}

// ������ͼ���Ƿ��ཻ
BOOL GeneralInterface::CalFigureIntersect(Figure* pSrcFig, Figure* pDstFig, FPoint& inter_pnt)
{
	BOOL rt;
	BOOL LineIntersectFlag = TRUE;			// �����������ཻ����һ����ֱ���ཻ
	int i_src, i_dst, nSrcNum, nDstNum;
	FPoint *pSrcCur, *pSrcNext, *pDstCur, *pDstNext;

	if(pSrcFig == NULL || pDstFig == NULL)
		return FALSE;

	// ���������㣬����������������߶��ཻ����
	pSrcFig->CalOutlineList();
	pDstFig->CalOutlineList();

	// �ж��Ƿ��ཻ
	nSrcNum = pSrcFig->GetOutlineNum();
	nDstNum = pDstFig->GetOutlineNum();

	for(i_src = nSrcNum-2; i_src >= 0; i_src--)
	{
		for (i_dst = 0; i_dst < nDstNum-1; i_dst++)
		{
			pSrcCur = pSrcFig->GetOutline(i_src);
			pSrcNext = pSrcFig->GetOutline(i_src+1);

			pDstCur = pDstFig->GetOutline(i_dst);
			pDstNext = pDstFig->GetOutline(i_dst+1);

			if(pSrcFig->m_FigureType == FigureType_PolyLine && pDstFig->m_FigureType == FigureType_PolyLine && LineIntersectFlag == TRUE)
			{
				LineIntersectFlag = FALSE;	// ֻ��һ�Σ������β��ཻ��˵���ܿ��ܳ����ˣ����߶��ཻ

				float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
				float angle = GeneralInterface::CalVectorAngle(pSrcNext->m_x - pSrcCur->m_x, pSrcNext->m_y - pSrcCur->m_y, pDstNext->m_x - pDstCur->m_x, pDstNext->m_y - pDstCur->m_y);

				if (angle > flag_angle)	 // �����н�Ϊ�۽ǣ����߶μн�Ϊ���
				{
					rt = CalSegmenIntersect2(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);	// ������
				}
				else					// �����н�Ϊ��ǣ����߶μн�Ϊ�۽�
				{
					rt = CalLineIntersect(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);	// ������
				}
			}
			else
			{
				rt = CalSegmenIntersect2(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);

				//2017-5-23  yuanzb  ��һ��ͼ�ε�ͷ���˵ڶ���ͼ�ε�β�����������ʹ�ཻҲ�����ཻ,�˴�����ཻ�ǵ�һ��ͼ�ε�β���ڶ���ͼ�ε�ͷ��
				FPoint* pDstFigTailPnt = pDstFig->GetTailShapePoint();

				if (rt == TRUE && inter_pnt.IsCloseTo(pDstFigTailPnt) == TRUE)
				{
					rt = FALSE;
				}

			}
			
			
			if (rt == TRUE)
				return TRUE;
		}
	}

	return rt;
}


/*--------------------------------------------------------------------------------------*/
/*                                 ��״��������ز���                                   */
/*--------------------------------------------------------------------------------------*/


// �ƶ���״������
void GeneralInterface::MoveShapePointList(vector<ShapePoint*>* pList, float rel_x, float rel_y)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	nCount = pList->size();
	for (i = 0; i < nCount; i++)
	{
		it = pList->begin() + i;

		(*it)->m_x += rel_x;
		(*it)->m_y += rel_y;
	}
}


// ������״������
void GeneralInterface::ReverseShapePointList(vector<ShapePoint*>* pList)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;
	vector<ShapePoint*> tmp_vector;

	nCount = pList->size();
	for (i = nCount-1; i >=0; i--)
	{
		it = pList->begin() + i;
		tmp_vector.push_back(*it);
	}

	pList->clear();
	*pList = tmp_vector;
}

// �����״������
void GeneralInterface::ClearShapePointList(vector<ShapePoint*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<ShapePoint*>::iterator it;

		// �ͷ��ڴ�
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			ShapePoint* pPnt = *it;

			delete pPnt;
			pPnt = NULL;
		}

		// �������
		pList->clear();
	}
}


// ������״���ж�
BOOL GeneralInterface::LinkShapePoint_Judge(ShapePoint* pSrcPnt, ShapePoint* pDstPnt, int* pStyle)
{
	Figure* pSrcFig, *pDstFig;
	int src_type, dst_type;


	// ���ж�
	if (pSrcPnt == NULL || pDstPnt == NULL || pSrcPnt == pDstPnt || pStyle == NULL)
	{
		return FALSE;
	}

	// Դ��Ŀ��ͼ��
	pSrcFig = pSrcPnt->m_pFigure;
	pDstFig = pDstPnt->m_pFigure;

	src_type = pSrcFig->m_FigureType;
	dst_type = pDstFig->m_FigureType;

	// ͼ���ж�
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig)
	{
		return FALSE;
	}

	// ͼ�������ж�
	if (src_type == FigureType_Circle || dst_type == FigureType_Circle)
	{
		return FALSE;
	}

	// �Ƿ�ͷβ��״��
	if (pSrcPnt != pSrcFig->GetHeadShapePoint() && pSrcPnt != pSrcFig->GetTailShapePoint())
	{
		return FALSE;
	}
	
	// �Ƿ�ͷβ��״��
	if (pDstPnt != pDstFig->GetHeadShapePoint() && pDstPnt != pDstFig->GetTailShapePoint())
	{
		return FALSE;
	}

	// ���ӷ�ʽ
	if (pSrcPnt == pSrcFig->GetHeadShapePoint())
	{
		if (pDstPnt == pDstFig->GetHeadShapePoint())
			*pStyle = Link_Head2Head;
		else
			*pStyle = Link_Head2Tail;
	}
	else
	{
		if (pDstPnt == pDstFig->GetHeadShapePoint())
			*pStyle = Link_Tail2Head;
		else
			*pStyle = Link_Tail2Tail;
	}

	return TRUE;
}

// ͼ��β����ͼ��ͷ
int GeneralInterface::LinkTail2Head(Figure* pSrcFig, Figure* pDstFig)
{
	int i, nSrcCount, nDstCount;
	int nSrcID, nDstID;
	ShapePoint *pTail = NULL, *pHead = NULL;
	ShapePoint* pNewPnt = NULL;
	Layer* pLayer = NULL;

	// 1�������ж�
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig )
	{
		return -1;
	}

	pLayer = pSrcFig->m_pLayer;
	pTail = pSrcFig->GetTailShapePoint();
	pHead = pDstFig->GetHeadShapePoint();
	nSrcID = pSrcFig->m_FigureID;
	nDstID = pDstFig->m_FigureID;


	// 2���½�����ͼ�Σ�ɾ���غϵ�
	nSrcCount = pSrcFig->GetShapePointNum();
	nDstCount = pDstFig->GetShapePointNum();

	// �����״��
	Cad_Complex* pComplex = new Cad_Complex;

	// ԭͼ��
	for (i = 0; i < nSrcCount-1; i++)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *(pSrcFig->GetShapePoint(i));
		pComplex->AddShapePoint(pNewPnt);
	}

	// ���ӵ�
	pNewPnt = new ShapePoint;

	*pNewPnt = *pTail;
	pComplex->AddShapePoint(pNewPnt);

	if (pTail->IsCloseTo(pHead) == FALSE)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *pHead;
		pNewPnt->m_FigureType = pTail->m_FigureType;
		pComplex->AddShapePoint(pNewPnt);
	}

	// Ŀ��ͼ�ε���״��
	for (i = 1; i < nDstCount; i++)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *(pDstFig->GetShapePoint(i));
		pComplex->AddShapePoint(pNewPnt);
	}

	// 3��ɾ������ͼ�Σ�����ͼ�㡣

	if (nSrcID > nDstID)
	{
		pLayer->DeleteFigure(nSrcID);
		pLayer->DeleteFigure(nDstID);
	}
	else
	{
		pLayer->DeleteFigure(nDstID);
		pLayer->DeleteFigure(nSrcID);
	}

	pComplex->CalOutlineList();
	pComplex->UpdateShapePointInfo();
	pLayer->AddFigureToHead(pComplex);


	return 0;
}


// ����ͼ�� 
// ���� 0 : �ɹ� ������ʧ��
int GeneralInterface::LinkFigure(Figure* pSrcFig, Figure* pDstFig)
{
	int rt;
	int src_type = pSrcFig->m_FigureType;
	int dst_type = pDstFig->m_FigureType;
	ShapePoint* pSrcPnt, * pDstPnt;
	ShapePoint* pSrcHead, * pSrcTail, *pDstHead, *pDstTail;


	// ͼ���ж�
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig)
	{
		return -1;
	}

	// ͼ�������ж�
	if (src_type == FigureType_Circle || dst_type == FigureType_Circle)
	{
		return -1;
	}

	// �ж����ӷ�ʽ

	pSrcHead = pSrcFig->GetHeadShapePoint();
	pSrcTail = pSrcFig->GetTailShapePoint();
	pDstHead = pDstFig->GetHeadShapePoint();
	pDstTail = pDstFig->GetTailShapePoint();


	if (pSrcTail->IsCloseTo(pDstHead) == TRUE)		// srcβ��dstͷ
	{
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}	
	else if (pSrcHead->IsCloseTo(pDstHead) == TRUE)		// srcͷ��dstͷ =  ������srcβ��dstͷ
	{
		pSrcFig->ReverseShapePointList();
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}
	else if (pSrcHead->IsCloseTo(pDstTail) == TRUE)			// srcͷ��dstβ = dstβ��srcͷ
	{
		pSrcPnt = pDstFig->GetTailShapePoint();
		pDstPnt = pSrcFig->GetHeadShapePoint();
	}
	else if (pSrcTail->IsCloseTo(pDstTail) == TRUE)		// srcβ��dstβ = srcβ��������dstͷ
	{
		pDstFig->ReverseShapePointList();
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}		
	else
		return -1;

	rt = LinkShapePoint(pSrcPnt, pDstPnt);

	return rt;
}

/*-----------------------------------------------------------------------*/
//	purpose:
//		������״�㣬����������ͼ�����ӳ�һ������ͼ�Σ�Ŀǰֻ����ͷβ����
//		���غϵ���״��ɾ��
//
//	param��
//		ShapePoint* pSrcPnt -- Դ��״��
//		ShapePoint* pDstPnt -- Ŀ����״��
//
//	return:
//		int -- 0 �ɹ�
/*-----------------------------------------------------------------------*/
int GeneralInterface::LinkShapePoint(ShapePoint* pSrcPnt, ShapePoint* pDstPnt)	
{
	int rt;
	Figure* pSrcFig, *pDstFig;
//	int style;

	// 1���Ϸ����жϼ���ʽ�ж�
// 	rt = LinkShapePoint_Judge(pSrcPnt, pDstPnt, &style);
// 	if (rt == TRUE)
// 	{
// 		Figure* pSrcFig, *pDstFig;
// 
// 		if (style == Link_Head2Tail)
// 		{
// 			// Դ��Ŀ��ͼ�� ������ֻ����βͷ����
// 			pSrcFig = pDstPnt->m_pFigure;
// 			pDstFig = pSrcPnt->m_pFigure;
// 		}
// 		else if (style == Link_Tail2Head)
// 		{
// 			pSrcFig = pSrcPnt->m_pFigure;
// 			pDstFig = pDstPnt->m_pFigure;
// 		}
// 		else if (style == Link_Head2Head)
// 		{
// 			pSrcFig->ReverseShapePointList();
// 			pSrcPnt = pSrcFig->GetTailShapePoint();
// 			pDstPnt = pDstFig->GetHeadShapePoint();
// 		}
// 		else
// 			return -1;	// ��ʱ������ͷͷ��ββ����
//
		// ͼ��β����ͼ��ͷ
// 		rt = LinkTail2Head(pSrcFig, pDstFig);
// 
// 		return rt;
//	}
//	return -1;

	// ͼ��β����ͼ��ͷ
	pSrcFig = pSrcPnt->m_pFigure;
	pDstFig = pDstPnt->m_pFigure;
	rt = LinkTail2Head(pSrcFig, pDstFig);

	return rt;
}


/*--------------------------------------------------------------------------------------*/
/*                                ������������ز���                                    */
/*--------------------------------------------------------------------------------------*/
// �������������
void GeneralInterface::ClearOutlineList(vector<FPoint*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<FPoint*>::iterator it;

		// �ͷ��ڴ�
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			FPoint* pPnt = *it;

			delete pPnt;
			pPnt = NULL;
		}

		// �������
		pList->clear();
	}
}


// �ƶ���״������
void GeneralInterface::MoveOutlineList(vector<FPoint*>* pList, float rel_x, float rel_y)
{
	int i, nCount;
	vector<FPoint*>::iterator it;

	nCount = pList->size();
	for (i = 0; i < nCount; i++)
	{
		it = pList->begin() + i;

		(*it)->m_x += rel_x;
		(*it)->m_y += rel_y;
	}
}



/*--------------------------------------------------------------------------------------*/
/*                                ��ѧ������ز���                                     */
/*--------------------------------------------------------------------------------------*/
// ��ȡ������ϵ�еĽǶȣ����ȱ�ʾ
float GeneralInterface::GetRadianFromXY(float dx, float dy)	
{
	float angle;

	if (dx == 0.0)
	{
		if (dy > 0.0)
		{
			angle = (float)(PI/2);
		}
		else
		{
			angle = (float)(-PI/2);
		}
	}
	else if (dx < 0.0)
	{
		angle = (float)(atan(dy/dx) + PI);
	}
	else
	{
		angle = atan(dy/dx);
	}

	if (angle < 0.0)
		angle += (float)(2*PI);

	return angle;
}


/*----------------------------------------------------------*/
//	purpose:
//		ָ���㵽ֱ�ߵľ���
//		б��ʽ��y = kx + b   ����(x1��y1)  =��  b = y1 - kx1 
//		һ��ʽ��Ax + By + C = 0
//		A = k  B = -1  C = b 
//		�㵽ֱ�߹�ʽ|Ax + By + C|/����(A*A + B*B)
//
//	param:
//		float start_x	--	���x
//		float star_y	--	���y
//		float end_x		--	�յ�x
//		float end_y		--	�յ�y
//		float x			--  ����x
//		float y			--	����y
//
//	return:
//		float	--	����
/*----------------------------------------------------------*/
float GeneralInterface::GetDistanceFromPoint2Line(float start_x, float start_y, float end_x, float end_y, float x, float y)
{
	float dx, dy, k, dist, b;

	dx = end_x - start_x;
	dy = end_y - start_y;

	if (dx == 0)		// ��ֱx��
	{
		dist = abs(x - start_x);

	}
	else if (dy == 0)	// ��ֱy��
	{
		dist = abs(y - start_y);
	}
	else 
	{
		k = dy/dx;
		b = start_y - k*start_x;
		dist = abs(k*x + -1*y + b) / sqrt(1 + k*k);
	}

	return	dist;
}



/*----------------------------------------------------------------------*/
//	purpose:
//		�����������нǣ��õ�ˣ�a��b = |a||b|cos��
//
//	param��
//		dx1	-- ����1xƫ��
//		dy1	-- ����1yƫ��
//		dx2	-- ����2xƫ��
//		dy2	-- ����2yƫ��
//
//	return:
//		angle -- �Ƕ�
/*----------------------------------------------------------------------*/
float GeneralInterface::CalVectorAngle(float dx1, float dy1, float dx2, float dy2)
{
	float angle, dot_product, module_product;

	dot_product = dx1*dx2 + dy1*dy2;	// ���
	module_product = sqrt(dx1*dx1 + dy1*dy1) * sqrt(dx2*dx2 + dy2*dy2);	// ģ��
	angle = acos(dot_product/module_product);

	return angle;
}

/*----------------------------------------------------------*/
//	purpose:
//		ƽ��һ���߶�
//		���ݷ���ͨ��������ת��ʽƽ���߶�
//
//	param:
//		float start_x	--	���x
//		float star_y	--	���y
//		float end_x		--	�յ�x
//		float end_y		--	�յ�y
//		int dir			--	����	0�����	1���ұ�
//		float dist		--	����	
//		float new_start_x	--	�����x
//		float new_star_y	--	�����y
//		float new_end_x		--	���յ�x
//		float new_end_y		--	���յ�y
//
//	return:
//		TRUE	--	�ɹ�
//		FALSE	--	ʧ��
/*----------------------------------------------------------*/
BOOL GeneralInterface::ParallelSegment(float start_x, float start_y, float end_x, float end_y, int dir, float dist,
									   float & new_start_x, float & new_start_y, float & new_end_x, float & new_end_y)
{
	float dx, dy, len;
	float vec_x, vec_y, normal_vec_x, normal_vec_y;
	float arc = PI/2.0;	// 90��

	// �������
	dx = end_x - start_x;
	dy = end_y - start_y;
	len = sqrt(dx*dx + dy*dy);

	// �ж������Ƿ�Ϊ0
	if (len < CLOSE_VALUE)	// �غ�
	{
		start_x	= new_start_x;
		start_y	= new_start_y;
		end_x	= new_end_x;
		end_y	= new_end_y;

		return FALSE;
	}

	// ��ת����
	if (dir == 0)	// ��� ��ʱ��90��
	{
		CouterClockwiseRotate(dx, dy, vec_x, vec_y, arc);
	}
	else			// ˳ʱ��90��
	{
		ClockwiseRotate(dx, dy, vec_x, vec_y, arc);
	}

	// �����淶�������ƽ�о���
	NormalVector(vec_x, vec_y, normal_vec_x, normal_vec_y);
	normal_vec_x *= dist;
	normal_vec_y *= dist;	

	// ԭʼ�������ƽ�������͵õ�ƽ�к���߶���
	new_start_x	= start_x	+ normal_vec_x;
	new_start_y	= start_y	+ normal_vec_y;
	new_end_x	= end_x		+ normal_vec_x;
	new_end_y	= end_y		+ normal_vec_y;

	return TRUE;
}


// ��ʱ����ת
void GeneralInterface::CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

// ˳ʱ����ת
void GeneralInterface::ClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) + mid_y*sin(arc);
	float rotate_y = -mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

// �����淶��
void GeneralInterface::NormalVector(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}

// ����ȡԲ��
void GeneralInterface::GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float dx = beginx - endx;
	float dy = beginy - endy;
	float chord_len = sqrt(dx*dx + dy*dy);
	float sita = acos(0.5*chord_len/radius);	// �Ƕ�
	float angle = Radian2Angle(sita);
	float new_x, new_y;

	// ˳ʱ����ת��ָ��Բ�ĵ������� ������180��ʱ��Ҫ��ʱ����ת
	ClockwiseRotate(dx, dy, new_x, new_y, sita);

	// �����Ĺ淶��
	float normal_new_x, normal_new_y;
	NormalVector(new_x, new_y, normal_new_x, normal_new_y);

	// β��ָ��Բ�ĵ�������׼ȷֵ
	float radius_x, radius_y;
	radius_x = normal_new_x * radius;
	radius_y = normal_new_y * radius;

	// β��+ָ��Բ�������� ���Բ�����ꡣ
	center_x = endx + radius_x;
	center_y = endy + radius_y;

}


/*----------------------------------------------------------*/
//	purpose:
//		���߶��ཻ
//
//	param:
//		float start_x	--	���x
//		float start_y	--	���y
//		float end_x		--	�յ�x
//		float end_y		--	�յ�y
//		float center_x	--	Բ��x
//		float center_y	--	Բ��y
//		float mid_x		--	�е�x
//		float mid_y		--	�е�y
//
//	return:
//		TRUE	--	��ȷ
//		FALSE	--	����
/*----------------------------------------------------------*/
BOOL GeneralInterface::CalArcMidPoint(float start_x, float start_y, float end_x, float end_y, float center_x, float center_y, float r, float &mid_x, float &mid_y)
{
	float dx, dy, chord_len;
	float vec_x,  vec_y;	// �岹ʱ�õ�����
	float second_Interpolate_x, second_Interpolate_y;

	// Բ��������
	dx = end_x - start_x;
	dy = end_y - start_y;

	// �ҳ�
	chord_len = sqrt(dx*dx + dy*dy);
	if (chord_len < 0)
		return FALSE;

	/*-------------���е�------------*/

	second_Interpolate_x = start_x + dx/2.0;
	second_Interpolate_y = start_y + dy/2.0;

	// Բ��ָ�����е������
	vec_x = second_Interpolate_x - center_x;
	vec_y = second_Interpolate_y - center_y;

	// �����淶����,���԰뾶�õ�ָ��Բ���е������
	NormalVector(vec_x, vec_y, vec_x, vec_y);
	vec_x *= r;
	vec_y *= r;

	// ���Բ���е�����
	second_Interpolate_x = center_x + vec_x;
	second_Interpolate_y = center_y + vec_y;

	mid_x = second_Interpolate_x;
	mid_y = second_Interpolate_y;
	
	return TRUE;
}


/*----------------------------------------------------------*/
//	purpose:
//		���߶��ཻ
//
//	param:
//		float start_x1	--	��һ���߶����x
//		float start_y1	--	��һ���߶����y
//		float end_x1	--	��һ���߶��յ�x
//		float end_y1	--	��һ���߶��յ�y
//		float start_x2	--	�ڶ����߶����x
//		float start_y2	--	�ڶ����߶����y
//		float end_x2	--	�ڶ����߶��յ�x
//		float end_y2	--	�ڶ����߶��յ�y
	
//		float &inter_x	--	����x
//		float &inter_y	--	����y
//
//	return:
//		TRUE	--	�ཻ
//		FALSE	--	���ཻ
/*----------------------------------------------------------*/

#define   ACCURACY			0.1				// ����
#define   ACCURACYTOW		0.00000001
BOOL GeneralInterface::CalSegmenIntersect(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y)
{

	if(fabs(start_x1 - end_x1) < ACCURACYTOW && fabs (start_y1 - end_y1) < ACCURACYTOW || fabs(start_x2 - end_x2) < ACCURACYTOW && fabs(start_y2 - end_y2) < ACCURACYTOW)
	{
		return FALSE;
	}

	if(fabs( start_x1 - end_x1) > ACCURACYTOW && fabs(start_y1 - end_y1) < ACCURACYTOW)	
	{
		if(fabs( start_x2 - end_x2) > ACCURACYTOW && fabs(start_y2- end_y2)<ACCURACYTOW )
		{
			if(fabs(start_y1-start_y2)<ACCURACYTOW &&(!((start_x2> (start_x1+ACCURACY)&&start_x2> (end_x1+ACCURACY)&&end_x2>(start_x1+ACCURACY)&&end_x2> (end_x1+ACCURACY))||((start_x2+ACCURACY)< start_x1&&(start_x2+ACCURACY)< end_x1&&(end_x2+ACCURACY)<start_x1&&(end_x2+ACCURACY)<end_x1))))
			{ 
				if(!(start_x2> start_x1&&start_x2> end_x1&&start_x2> end_x2||start_x2< start_x1&&start_x2<end_x1&&start_x2< end_x2))

				{
					inter_x=start_x2;
					inter_y=start_y2;
					return TRUE;
				}
				if(!(start_x1> start_x2&&start_x1> end_x1&&start_x1> end_x2||start_x1< start_x2&&start_x1<end_x1&&start_x1< end_x2))

				{
					inter_x=start_x1;
					inter_y=start_y1;
					return TRUE;
				}
				if(!(end_x2> start_x1&&end_x2> end_x1&&end_x2> start_x2||end_x2< start_x1&&end_x2<end_x1&&end_x2< start_x2))

				{
					inter_x=end_x2;
					inter_y=end_y2;
					return TRUE;
				}if(!(end_x1> start_x1&&end_x1> start_x2&&end_x1> end_x2||end_x1< start_x1&&end_x1<start_x2&&end_x1< end_x2))

				{
					inter_x=end_x1;
					inter_y=end_x2;
					return TRUE;
				}
			}
			else
				return FALSE;

		}
		else
		{
			float xpoint=( start_x2- end_x2)/(start_y2- end_y2)*end_y1+(end_x2*start_y2-start_x2*end_y2)/(start_y2- end_y2);
			inter_y=end_y1;
			inter_x=xpoint;
			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x>+ACCURACY)>=start_x1))
				return TRUE;
			else
				return FALSE;


		}
	}
	if(fabs( start_x1- end_x1)<ACCURACYTOW &&fabs(start_y1- end_y1)>ACCURACYTOW )
	{
		if(fabs( start_x2- end_x2)<ACCURACYTOW &&fabs(start_y2- end_y2)>ACCURACYTOW )
		{ 
			if(fabs(start_x1-start_x2)<ACCURACYTOW &&(!((start_y2> (start_y1+ACCURACY)&&start_y2> (end_y1+ACCURACY)&&end_y2>(start_y1+ACCURACY)&&end_y2> (end_y1+ACCURACY))||((start_y2+ACCURACY)< start_y1&&(start_y2+ACCURACY)< end_y1&&(end_y2+ACCURACY)<start_y1&&(end_y2+ACCURACY)<end_y1))))
			{ 
				if(!(start_y2> start_y1&&start_y2> end_y1&&start_y2> end_y2||start_y2< start_y1&&start_y2<end_y1&&start_y2< end_y2))

				{
					inter_x=start_x2;
					inter_y=start_y2;
					return TRUE;
				}
				if(!(start_y1> start_y2&&start_y1> end_y1&&start_y1> end_y2||start_y1< start_y2&&start_y1<end_y1&&start_y1< end_y2))

				{
					inter_x=start_x1;
					inter_y=start_y1;
					return TRUE;
				}
				if(!(end_y2> start_y1&&end_y2> end_y1&&end_y2> start_y2||end_y2< start_y1&&end_y2<end_y1&&end_y2< start_y2))

				{
					inter_x=end_x2;
					inter_y=end_y2;
					return TRUE;
				}if(!(end_y1> start_y1&&end_y1> start_y2&&end_y1> end_y2||end_y1< start_y1&&end_y1<start_y2&&end_y1< end_y2))

				{
					inter_x=end_x1; inter_y=end_x2; return TRUE;
				}
			}
			else
				return FALSE;
		}
		else
		{
			float ypoint=( start_y2- end_y2)/(start_x2- end_x2)*end_x1+(end_y2*start_x2-start_y2*end_x2)/(start_x2- end_x2); 
			inter_x=end_x1;
			inter_y=ypoint;
			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x1))
				return TRUE;
			else
				return FALSE;
		}
	}
	else
	{
		float k=-( start_y1- end_y1)/( start_x1- end_x1);
		float b=-start_y1-k* start_x1;
		if((k* start_x2+b+start_y2)<0&&(k* end_x2+b+end_y2)>0||(k* start_x2+b+start_y2)>0&&(k* end_x2+b+end_y2)<0)
		{
			inter_x=(-(start_y2+b)*(start_x2-end_x2)+start_x2*(start_y2-end_y2))/(k*(start_x2-end_x2)+(start_y2-end_y2));
			inter_y=-inter_x*k-b;

			if((end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x1)&&(end_y1<=(inter_y+ACCURACY)&&(inter_y-ACCURACY)<=start_y1||end_y1>=(inter_y-ACCURACY)&&(inter_y+ACCURACY)>=start_y1))
				return TRUE;
			else
				return FALSE;
		}

		if(fabs(( start_x1- end_x1)*( start_y2- end_y2)-( start_x2- end_x2)*( start_y1- end_y1))>ACCURACYTOW)
		{

			inter_x=(-(start_y2+b)*(start_x2-end_x2)+start_x2*(start_y2-end_y2))/(k*(start_x2-end_x2)+(start_y2-end_y2));
			inter_y=-inter_x*k-b;

			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_y2<=(inter_y+ACCURACY)&&(inter_y-ACCURACY)<=start_y2||end_y2>=(inter_y-ACCURACY)&&(inter_y+ACCURACY)>=start_y2))
				return TRUE;
			else
				return FALSE;

		}
		if(fabs(( start_x1- end_x1)*( start_y2- end_y2)-( start_x2- end_x2)*( start_y1- end_y1))<ACCURACYTOW&&fabs(k* start_x2+b+start_y2)<ACCURACYTOW)
		{ 	if(!((start_y2> (start_y1+ACCURACY)&&start_y2> (end_y1+ACCURACY)&&end_y2>(start_y1+ACCURACY)&&end_y2> (end_y1+ACCURACY))||((start_y2+ACCURACY)< start_y1&&(start_y2+ACCURACY)< end_y1&&(end_y2+ACCURACY)<start_y1&&(end_y2+ACCURACY)<end_y1)))
		{
			if(!(start_y2> start_y1&&start_y2> end_y1&&start_y2> end_y2||start_y2< start_y1&&start_y2<end_y1&&start_y2< end_y2))

			{
				inter_x=start_x2;
				inter_y=start_y2;
				return TRUE;
			}
			if(!(start_y1> start_y2&&start_y1> end_y1&&start_y1> end_y2||start_y1< start_y2&&start_y1<end_y1&&start_y1< end_y2))

			{
				inter_x=start_x1;
				inter_y=start_y1;
				return TRUE;
			}
			if(!(end_y2> start_y1&&end_y2> end_y1&&end_y2> start_y2||end_y2< start_y1&&end_y2<end_y1&&end_y2< start_y2))

			{
				inter_x=end_x2;
				inter_y=end_y2;
				return TRUE;
			}if(!(end_y1> start_y1&&end_y1> start_y2&&end_y1> end_y2||end_y1< start_y1&&end_y1<start_y2&&end_y1< end_y2))

			{
				inter_x=end_x1;
				inter_y=end_x2;
				return TRUE;
			}
		}
		}

		else return FALSE;
	}


	return FALSE;
}





//���㽻��˻�(P1-P0)x(P2-P0)
double xmult(point p1,point p2,point p0){
	return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}

//�е��Ƿ����߶���,�����˵�
int dot_online_in(point p,point l1,point l2){
	return zero(xmult(p,l1,l2))&&(l1.x-p.x)*(l2.x-p.x)<eps&&(l1.y-p.y)*(l2.y-p.y)<eps;
}

//���������߶�ͬ��,�����߶��Ϸ���0
int same_side(point p1,point p2,point l1,point l2){
	return xmult(l1,p1,l2)*xmult(l1,p2,l2)>eps;
}

//����ֱ��ƽ��
int parallel(point u1,point u2,point v1,point v2){
	return zero((u1.x-u2.x)*(v1.y-v2.y)-(v1.x-v2.x)*(u1.y-u2.y));
}

//�����㹲��
int dots_inline(point p1,point p2,point p3){
	return zero(xmult(p1,p2,p3));
}

//�����߶��ཻ,�����˵�Ͳ����غ�
int intersect_in(point u1,point u2,point v1,point v2){
	if (!dots_inline(u1,u2,v1)||!dots_inline(u1,u2,v2))
		return !same_side(u1,u2,v1,v2)&&!same_side(v1,v2,u1,u2);
	return dot_online_in(u1,v1,v2)||dot_online_in(u2,v1,v2)||dot_online_in(v1,u1,u2)||dot_online_in(v2,u1,u2);
}

//�������߶ν���,�����߶��Ƿ��ཻ(ͬʱ����Ҫ�ж��Ƿ�ƽ��!)
point intersection(point u1,point u2,point v1,point v2)
{
	point ret=u1;
	double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
		/((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
	ret.x+=(u2.x-u1.x)*t;
	ret.y+=(u2.y-u1.y)*t;
	return ret;
}


 BOOL GeneralInterface::CalSegmenIntersect2(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y)
{
	point u1,u2,v1,v2;
	point ans;

	u1.x = start_x1;
	u1.y = start_y1;

	u2.x = end_x1;
	u2.y = end_y1;

	v1.x = start_x2;
	v1.y = start_y2;

	v2.x = end_x2;
	v2.y = end_y2;

	if (parallel(u1,u2,v1,v2) || !intersect_in(u1,u2,v1,v2))
	{
		return FALSE;
	}
	else
	{
		ans = intersection(u1,u2,v1,v2);
		inter_x = ans.x;
		inter_y = ans.y;
		return TRUE;
	}

	return FALSE;
}




 /*----------------------------------------------------------------------*/
 //	purpose:
 //		������ֱ���ཻ��
 //
 //	param��
 //		(start_x1, start_y1)	-- ��һ��ֱ�����
 //		(end_x1, end_y1)	-- ��һ��ֱ���յ�
 //		(start_x2, start_y2)	-- �ڶ���ֱ�����
 //		(end_x2, end_y2)	-- �ڶ���ֱ���յ�
 //		(inter_x, inter_y) -- ����
 //
 //	return:
 //		TRUE -- �޽���
 //		FALSE  -- �н���
 /*----------------------------------------------------------------------*/

#define		EQUAL_VALUE		0.01
 BOOL GeneralInterface::CalLineIntersect(float start_x1, float start_y1, float end_x1, float end_y1, float start_x2, float start_y2, float end_x2, float end_y2, float& inter_x, float& inter_y)
 {
	 float k1, k2, dx1, dy1, dx2, dy2;

	 dx1 = end_x1 - start_x1;
	 dy1 = end_y1 - start_y1;
	 dx2 = end_x2 - start_x2;
	 dy2 = end_y2 - start_y2;

	 if (abs(dx1) < EQUAL_VALUE)		// ��һ��ֱ�ߴ�ֱY��
	 {
		 if (abs(dx2) < EQUAL_VALUE)  
			 return FALSE;
		 else
		 {
			 k2 = dy2/dx2;
			 inter_x = start_x1;
			 inter_y = k2*(inter_x - start_x2)+ start_y2 ;

			 return TRUE;
		 }
	 }
	 else if (abs(dx2) < EQUAL_VALUE)	// �ڶ���ֱ�ߴ�ֱY�� 
	 {
		 k1 = dy1/dx1;
		 inter_x = start_x2;
		 inter_y = k1*(inter_x - start_x1)+ start_y1 ;

		 return TRUE;
	 }
	 else
	 {
		 k1 = dy1/dx1;
		 k2 = dy2/dx2;

		 if(k1 == k2)		// ƽ�У�����Ҫ�ж��Ƿ��غ� 
		 {
			 float dist = GetDistanceFromPoint2Line(start_x1, start_y1, end_x1, end_y1, start_x2, start_y2);
			 if (dist < CLOSE_VALUE)	// ����ֱ����
			 {
				 inter_x = start_x2;
				 inter_y = start_y2;

				 return TRUE;
			 }
			 else
			 {
				 return FALSE;
			 }
		 }
		 else
		 {
			 inter_x = (start_y2 - start_y1 - (k2*start_x2 - k1*start_x1))/(k1 - k2);
			 inter_y = k1*(inter_x - start_x1) + start_y1;

			 return TRUE;
		 }
	 }
 }
