/*--------------------------------------------------------------------------------------------------------------------*/
//	Complex.cpp   -- ����ͼ����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.12
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Complex.h"
#include "../CommonData.h"
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "PolyLine.h"
#include "Arc.h"
#include "../Layer/Layer.h"



Cad_Complex::Cad_Complex()
{
	m_FigureType = FigureType_Complex;
}


Cad_Complex::~Cad_Complex()
{

}






/*-----------------------------------------------------------------------*/
//	purpose:
//		����ƽ�е�ͼ��
//		���ݻ���ͼ����������ƽ�У�
//		1��������ͼ�����ཻ�������ཻ���޸�ǰ�����ͼ�ε�β������
//		2�������ཻ������Բ���岹�����ӻ���ͼ��
//		
//
//	param��
//		int dir -- ����
//		float dist -- ����
//
//	return:
//		vector<ShapePoint*> -- ƽ�к����״��
/*-----------------------------------------------------------------------*/
Figure* Cad_Complex::CalParallelFigure(int dir, float dist)
{
	BOOL ClosedFlag = FALSE;			// ͼ�αպ�
	BOOL InterFlag = FALSE;
	BOOL IntersectWithPrevFig = FALSE;
	FPoint inter_pnt;
	Figure* pCurFig, *pCurParallelFig;
	Figure* pNextFig, *pNextParallelFig;
	ShapePoint* pCurFigEndPnt, *pCurParallelFigStartPnt, *pCurParallelFigEndPnt, *pNextParallelFigStartPnt;
	Layer ParallelLayer;

	// �ж��Ƿ�պ�
	ClosedFlag = IsClosed();

	// �������ͼ������
	CalComplexFigureList();

	// ����ƽ��
	int i, nCount;
	nCount = m_ComplexFigureList.size();

	if (nCount == 1)			// ֻ��һ������ͼ��	ֱ����ƽ��ͼ�μ���
	{
		pCurFig = m_ComplexFigureList.at(0);

		// ƽ��
		pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);

		return pCurParallelFig;
	}
	else					// �������ͼ��		��ƽ�У��ཻ��������
	{

		for (i = 0; i < nCount; i++)
		{
			if (i == nCount-1)	// ���һ��ͼ��
			{
				pCurFig = m_ComplexFigureList.at(i);
				pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);

				// ��ӵ�ǰƽ��ͼ��
				ParallelLayer.AddFigure(pCurParallelFig);

				// ������Ҫ�ж��Ƿ�����һͼ���ཻ��������ǰƽ��ͼ�ε����
				pCurParallelFigStartPnt = pCurParallelFig->GetHeadShapePoint();
				if (IntersectWithPrevFig == TRUE)
				{
					pCurParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
				}

				if(ClosedFlag == TRUE)	// �պϣ���Ҫ�ж����һ��ͼ���Ƿ��ཻ
				{
					pNextParallelFig = ParallelLayer.GetFigure(0);	// ֱ��ȡ��һ��ƽ��ͼ��


					// �ཻ
					InterFlag = GeneralInterface::CalFigureIntersect(pCurParallelFig, pNextParallelFig, inter_pnt);
					if (InterFlag == TRUE)
					{
						// �ཻ���޸�ƽ��ͼ�ε���״��

						pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	���
						pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	�յ�

						pCurParallelFigEndPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
						pNextParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);

						// ��������һͼ���ཻ��־�����ཻ��
						IntersectWithPrevFig = TRUE;
					}
					else
					{
						// ��Ϊ���ཻ
						IntersectWithPrevFig = FALSE;

						// ���ཻ����Բ���岹
						pCurFigEndPnt = pCurFig->GetTailShapePoint();						//	��ǰͼ��β�� == Բ��
						pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	��ǰƽ��ͼ��β�� == Բ�����
						pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	��һƽ��ͼ���׵� == Բ���յ�

						Cad_PolyLine* pLinkArc = new Cad_PolyLine;
						pLinkArc->m_ShapePointList = GeneralInterface::ArcInterpolate(pCurFigEndPnt, pCurParallelFigEndPnt, pNextParallelFigStartPnt, dist);

						// ����ͼ����Ϣ
						pLinkArc->UpdateShapePointInfo();
						ParallelLayer.AddFigure(pLinkArc);
					}
				}
// 				else					// ���պϣ�ֱ��ƽ�����
// 				{
// 					ParallelLayer.AddFigure(pCurParallelFig);
// 				}

			}
			else
			{
				pCurFig = m_ComplexFigureList.at(i);
				pNextFig = m_ComplexFigureList.at(i+1);

				// ƽ��
				pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);
				pNextParallelFig = pNextFig->CalParallelFigure(dir, dist);

				// ��ӵ�ǰƽ��ͼ��
				ParallelLayer.AddFigure(pCurParallelFig);

				// ������Ҫ�ж��Ƿ�����һͼ���ཻ��������ǰƽ��ͼ�ε����
				pCurParallelFigStartPnt = pCurParallelFig->GetHeadShapePoint();
				if (IntersectWithPrevFig == TRUE)
				{
					pCurParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
				}

				// �ཻ
				InterFlag = GeneralInterface::CalFigureIntersect(pCurParallelFig, pNextParallelFig, inter_pnt);
				if (InterFlag == TRUE)
				{
					// �ཻ���޸�ƽ��ͼ�ε���״��

					pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	���
					pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	�յ�

					pCurParallelFigEndPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
					pNextParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);

					// ��������һͼ���ཻ��־�����ཻ��
					IntersectWithPrevFig = TRUE;

				}
				else
				{
					// ��Ϊ���ཻ
					IntersectWithPrevFig = FALSE;

					// ���ཻ����Բ���岹
					pCurFigEndPnt = pCurFig->GetTailShapePoint();						//	��ǰͼ��β�� == Բ��
					pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	��ǰƽ��ͼ��β�� == Բ�����
					pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	��һƽ��ͼ���׵� == Բ���յ�

					// ������ӻ�
					Cad_PolyLine* pLinkArc = new Cad_PolyLine;
					pLinkArc->m_ShapePointList = GeneralInterface::ArcInterpolate(pCurFigEndPnt, pCurParallelFigEndPnt, pNextParallelFigStartPnt, dist);

					// ����ͼ����Ϣ
					pLinkArc->UpdateShapePointInfo();
					ParallelLayer.AddFigure(pLinkArc);
				}


				//ParallelLayer.AddFigure(pCurParallelFig);
			}
		}
	}

	// ������ͼ��,�ڼ���ͼ�������Ϣ
	ParallelLayer.LinkFigureList();

	Figure* pFinalFig = ParallelLayer.GetFigure(0);
	pFinalFig->CalFigureRect();
	pFinalFig->UpdateShapePointInfo();
	pFinalFig->UpdateOutlineInfo();
	

	return pFinalFig;
}



/*----------------------------------------------------------*/
//	purpose:
//		������״��������Ϊ����ͼ�ε�ԭ�򣬻����״���������һ���ĵ���
//		�ȵ�ת���ٰ����Ĳ嵽��ǰ��
//		
//	param:
//		void
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Complex::ReverseShapePointList(void)
{
	// ��״���ͼ�����ԣ��ȵ�ת���ٰ����Ĳ嵽��ǰ��
 	int i, nCount;
 	ShapePoint* pPnt = NULL;
	vector<int> type_vector;
	vector<int>::iterator type_it;

	nCount = GetShapePointNum();
	for (i = nCount-1; i >=0; i--)
	{
		pPnt = GetShapePoint(i);
		type_vector.push_back(pPnt->m_FigureType);
	}
	
	// �����һ��ͼ�����Ͳ嵽��ǰ��
	int end_type = type_vector.back();
	type_vector.pop_back();
	type_vector.insert(type_vector.begin(), end_type);
	
	GeneralInterface::ReverseShapePointList(&m_ShapePointList);

	for (i = 0; i < nCount; i++)
	{
		pPnt = GetShapePoint(i);
		pPnt->m_FigureType = type_vector[i];
	}

	UpdateShapePointInfo();
}


// ����������״����Ϣ
void Cad_Complex::UpdateShapePointInfo(void)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	nCount = m_ShapePointList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_ShapePointList.begin() + i;
		(*it)->m_PntID = i;
		(*it)->m_pFigure = this;
	}
}



/*----------------------------------------------------------*/
//	purpose:
//		������״����������Ϣ
//		1���ȼ����ͼ�������������Ƶ���״��
//		2����������״�㣬��������¼���ֲ�Ĺ�����д���
//		3������Ӧ����״����Ϣ��ֵ������ͼ�ε���״������
//
//	param:
//		void
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Complex::CalShapePointOutlineInfo(void)
{
	int i,j, n, nCount;
	int nLastDelIndex = -1;								//yuanzb 2016-9-29 ��Ϊͼ�μ�����ӻ����ܾ���ܽ������ж��غϣ�ɾ���˻ᵼ�¼���㲻ƥ�䣬������ӱ������һ��ɾ����������������ɾ��ͼ�μ�����ӵ㵼��������ƥ��  
	int nComplexFigNum, nTmpListTotalNum, nTmpListNum;	// ����ͼ��������tmplist��״�������ܺͣ�tmplistɾ�����ֱ�߽�Բ����Բ����Բ����Բ����ֱ�ߣ��غ���״��������
	Figure* pFig = NULL;
	ShapePoint* pPnt = NULL;
	ShapePoint* pSrcPnt = NULL;
	vector<ShapePoint*> tmp_list;

	ShapePoint* pCurPnt = NULL;
	ShapePoint* pNextPnt = NULL;

	// 1������ͼ������
	CalComplexFigureList();

	nComplexFigNum = m_ComplexFigureList.size();
	for (i = 0; i < nComplexFigNum; i++)
	{
		pFig = m_ComplexFigureList[i];
		pFig->CalShapePointOutlineInfo();

		n = pFig->GetShapePointNum();
		for (j = 0; j < n; j++)
		{
			pPnt = new ShapePoint;
			pSrcPnt = pFig->GetShapePoint(j);

			*pPnt = *pSrcPnt;

			tmp_list.push_back(pPnt);
		}
	}

	// 2����������¼�������������״��
	vector<ShapePoint*>::iterator del_it;
	nTmpListTotalNum = nTmpListNum = tmp_list.size();
	for (i = 0; i < nTmpListNum-2; i++)
	{
		j = i+1;
		pCurPnt = tmp_list[i];
		pNextPnt = tmp_list[j];

		if (pCurPnt->IsCloseTo(pNextPnt) == TRUE && nLastDelIndex != i)
		{
			if (pCurPnt->m_FigureType == FigureType_PolyLine && pNextPnt->m_FigureType == FigureType_Arc)
			{
				// ��¼��һ��ɾ��������
				nLastDelIndex = i;	

				// ֱ�߽�Բ����ɾ��ֱ�ߵ�
				del_it = tmp_list.begin() + i;

				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
			else if (pCurPnt->m_FigureType == FigureType_Arc && pNextPnt->m_FigureType == FigureType_PolyLine)
			{
				// ��¼��һ��ɾ��������
				nLastDelIndex = i;	

				// Բ����ֱ�ߣ�ɾ��ֱ�ߵ�
				del_it = tmp_list.begin() + j;

				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
			else if (pCurPnt->m_FigureType == FigureType_Arc && pNextPnt->m_FigureType == FigureType_Arc)
			{
				// ��¼��һ��ɾ��������
				nLastDelIndex = i;	

				// Բ����Բ����ɾ��ǰһ�㣬��һ��sign��Ϊ3
				//pNextPnt->m_sign = Sign_ArcLink;

				//del_it = tmp_list.begin() + i;

				pCurPnt->m_sign = Sign_ArcLink;
				del_it = tmp_list.begin() + j;


				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
		}
	}

	// 3����˳��ֵ������ͼ�ε���״������
	nCount = GetShapePointNum();
	n = tmp_list.size();

	// �ж���״�������Ƿ�ƥ��
	if (n != nCount)
	{
		if(nCount != nTmpListTotalNum-nComplexFigNum+1)
		{
			AfxMessageBox("����ͼ����״����Ϣ���㲻ƥ�䣡");
		}
		else
		{
			AfxMessageBox("ɾ���˲����غϵ㣡");
		}
	}


	// ͷβ��״���group��ֵ
	tmp_list[0]->m_group = Group_Start;
	tmp_list[nCount-1]->m_group = Group_End;

	for (i = 0; i < nCount; i++)
	{
		pSrcPnt = GetShapePoint(i);
		pPnt = tmp_list[i];

		pSrcPnt->m_radius	= pPnt->m_radius;
		pSrcPnt->m_sign		= pPnt->m_sign;		
		pSrcPnt->m_dir		= pPnt->m_dir;		
		pSrcPnt->m_cut		= pPnt->m_cut;		
		pSrcPnt->m_type		= pPnt->m_type;		
		pSrcPnt->m_group	= pPnt->m_group;	
		pSrcPnt->m_side		= pPnt->m_side;	
	}


}




// ��ո���ͼ������
void Cad_Complex::ClearComplexFigureList()
{
	GeneralInterface::ClearFigureList(&m_ComplexFigureList);
}

// ����ͼ������
void Cad_Complex::CalComplexFigureList(void)
{
	int i, nArcNum, nCount, start_id, end_id;
	int cur_type, next_type;
	ShapePoint *pNext;
	Figure* pFig = NULL;

	// ���Դ����
	ClearComplexFigureList();

	nCount = GetShapePointNum();
	if (nCount < 2)
		return ;
	else if (nCount = 2)
	{
		start_id = 0;
		end_id = 1;
	}

	start_id = 0;
	pNext = GetShapePoint(1);
	cur_type = pNext->m_FigureType;

	// �м��ͼ��
	nCount = GetShapePointNum();
	for (i = 2; i < nCount; i++)
	{
		end_id = i-1;
		pNext = GetShapePoint(i);
		next_type = pNext->m_FigureType;

		if (cur_type != next_type)	// ͼ�����Ͳ�ͬ������һ��ͼ��
		{
			pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
			pFig->CalOutlineList();
			AddFigure(pFig);

			cur_type = next_type;
			start_id = i-1;
			end_id = i;
		}
		else if (cur_type == FigureType_Arc)
		{
			// Բ����������Ϳ��������

			nArcNum = end_id - start_id;
			if (nArcNum == 2)
			{
				pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
				pFig->CalOutlineList();
				AddFigure(pFig);

				cur_type = next_type;
				start_id = i-1;
				end_id = i;
			}
			else
				end_id++;
		}
		else
		{
			end_id++;
		}
	}

	// ���һ��ͼ��
	pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
	pFig->CalOutlineList();
	AddFigure(pFig);

}


// ��ȡͼ��
Figure* Cad_Complex::GetFigureFromShapePoint(int start_id, int end_id, int type)
{
	int i, nShpPntCount;
	Figure* pFig = NULL;
	Cad_Arc* pArc = NULL;
	ShapePoint* pPnt = NULL;
	ShapePoint* pNewPnt = NULL;

	nShpPntCount = end_id - start_id + 1;

	// �½�ͼ��
	switch(type)
	{
	case FigureType_PolyLine:
		pFig = new Cad_PolyLine;
		break;
	case FigureType_Arc:
		pFig = new Cad_Arc;
		break;
	default:
		break;
	}

	// �����״��
	for (i = start_id; i <= end_id; i++ )
	{
		pNewPnt = new ShapePoint;
		pPnt = GetShapePoint(i);

		*pNewPnt = *pPnt;
		pFig->AddShapePoint(pNewPnt);
	}

	// ����һЩ��Ҫ��Ϣ
	switch(type)
	{
	case FigureType_Arc:
		pArc = dynamic_cast<Cad_Arc*>(pFig);

		if (pArc == NULL)
		{
			AfxMessageBox("��̬ת��ʧ�ܣ�");
			pArc = static_cast<Cad_Arc*>(pFig);
		}


		pArc->CalCenterRadiusInfo();
		break;
	default:
		break;
	}


	return pFig;
}

// ���һ��ͼ��
int Cad_Complex::AddFigure(Figure* pFigure)	
{
	if (pFigure != NULL)
	{
		m_ComplexFigureList.push_back(pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}


// ��������ͼ����Ϣ
void Cad_Complex::UpdateFigureInfo(void)
{
	int i, nCount;
	vector<Figure*>::iterator it;

	nCount = m_ComplexFigureList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_ComplexFigureList.begin() + i;
		(*it)->m_FigureID = i;
		(*it)->m_pLayer = NULL;
	}
}

// ����������	
void Cad_Complex::CalOutlineList(void)
{
	// �����
	ClearOutlineList();

	// ����ͼ������
	CalComplexFigureList();

	// ���Ӹ���ͼ�ε�������
	int i, j, nFigCount, nOutlineCount;
	Figure* pFig = NULL;

	nFigCount = m_ComplexFigureList.size();
	for (i = 0; i < nFigCount; i++)
	{
		pFig = m_ComplexFigureList[i];

		nOutlineCount = pFig->GetOutlineNum();
		for (j = 0; j < nOutlineCount; j++)
		{
			FPoint* pPnt = new FPoint;
			*pPnt = *(pFig->GetOutline(j));
			AddOutline(pPnt);
		}
	}

}