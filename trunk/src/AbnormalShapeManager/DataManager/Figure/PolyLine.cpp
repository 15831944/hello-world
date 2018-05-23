#include "stdafx.h"
#include "PolyLine.h"
#include "../CommonData.h"
#include "../Layer/Layer.h"
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "Arc.h"


Cad_PolyLine::Cad_PolyLine()
{
	m_FigureType = FigureType_PolyLine;
}

Cad_PolyLine::~Cad_PolyLine()
{

}


/*-----------------------------------------------------------------------*/
//	purpose:
//		����ƽ�е���״������
//		1��ƽ�к��ͼ��һ��Ϊ���ͼ��
//		2��ÿ������ƽ�г�һ��ֱ�ߣ��ཻ��ȡ�ཻ�㣬���ཻ�����Բ�����ӣ�
//
//	param��
//		int dir -- ����
//		float dist -- ����
//
//	return:
//		vector<ShapePoint*> -- ƽ�к����״��
/*-----------------------------------------------------------------------*/
Figure* Cad_PolyLine::CalParallelFigure(int dir, float dist)
{
	BOOL rt = FALSE;
	BOOL ClosedFlag = FALSE;			// ͼ�αպ�
	BOOL IsInterWithPrevSeg = FALSE;	// ����һ���߶��ཻ
	BOOL IsInterWithNextSeg = FALSE;	//����һ���߶��ཻ
	int i, nCount;
	float inter_x = 0, inter_y = 0;		// ��������
	float pre_x, pre_y, cur_x, cur_y, next_x, next_y;			// ԭʼ�߶ε�
	float new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y;		// ƽ�к���߶ε�
	float dx, dy, mid_Interpolate_x, mid_Interpolate_y;			// �߶����ӵ�Բ���岹��

	ShapePoint *pNewHead;								// ͷβ��
	ShapePoint* pPrePnt, *pCurPnt, *pNextPnt;							// ԭʼ��
	ShapePoint* pNewPrePnt, *pNewCurPnt1, *pNewCurPnt2;					// ƽ�к�ĵ�
	vector<ShapePoint*> ParallelShapePointList;							// ƽ�е���״������
	Cad_PolyLine* pParallelPolyLine = new Cad_PolyLine;					// ƽ��ͼ��

	// �ж��Ƿ�պ�
	ClosedFlag = IsClosed();

	// �ж���״�����Ƿ�Ϸ�
	nCount = GetShapePointNum();
	if (nCount < 2)
	{
		return NULL;
	}

	for (i = 1; i < nCount; i++)
	{
		if (i == nCount-1)	// ���һ��
		{
			if (ClosedFlag == TRUE)	// �պϵĵ�ǰ�߶����һ���߶��ཻ
			{
				pPrePnt = GetShapePoint(i-1);
				pCurPnt = GetShapePoint(i);
				pNextPnt = GetShapePoint((i+2)%nCount);

				pPrePnt->GetXY(pre_x, pre_y);
				pCurPnt->GetXY(cur_x, cur_y);
				pNextPnt->GetXY(next_x, next_y);

				// ƽ�к��������
				rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
				if (rt == FALSE)
				{
					AfxMessageBox("���غϵ㣬�߶�ƽ�г���");
				}

				// ����һ���߶�֮ǰ�Ƿ��н��㣬�еĻ��׵�Ϊ�ཻ��
				if (IsInterWithPrevSeg == TRUE)
				{
					new_pre_x = inter_x;
					new_pre_y = inter_y;
					IsInterWithPrevSeg = FALSE;
				}
				else
				{
					// ��һ����ƽ�е�
					pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
					pParallelPolyLine->AddShapePoint(pNewPrePnt);
					pNewPrePnt = NULL;
				}



				rt = GeneralInterface::ParallelSegment(cur_x, cur_y, next_x, next_y, dir, dist, new_cur_x2, new_cur_y2, new_next_x, new_next_y);
				if (rt == FALSE)
				{
					AfxMessageBox("���غϵ㣬�߶�ƽ�г���");
				}

				// �ж�����һ�����Ƿ��ཻ������120�ȣ�ֱ��ƽ�У�С��120���߶�ƽ��
				float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
				float angle = GeneralInterface::CalVectorAngle(new_cur_x1 - new_pre_x, new_cur_y1 - new_pre_y, new_next_x - new_cur_x2, new_next_y - new_cur_y2);
				 
				if (angle > flag_angle)	 // ���
				{
					IsInterWithNextSeg = GeneralInterface::CalSegmenIntersect2(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// ������
				}
				else					// �۽�
				{
					IsInterWithNextSeg = GeneralInterface::CalLineIntersect(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// ������
				}

				if (IsInterWithNextSeg == TRUE)
				{
					new_cur_x1 = inter_x;
					new_cur_y1 = inter_y;

					IsInterWithPrevSeg = TRUE;	// ��һ��ֱ�����Ϊ����
					IsInterWithNextSeg = FALSE;

					// �ڶ�����ƽ�е�
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);


					// ����һ���߶��ཻ����ӵ�ͼ�㣬����Ϊֹ
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					pNewCurPnt1 = NULL;

					// �պ�ͼ�������һ���߶����һ���߶��ཻ����һ���߶������Ҫ�ı�����
					pNewHead = pParallelPolyLine->GetHeadShapePoint();
					pNewHead->SetXY(new_cur_x1, new_cur_y1);

				}
				else	// ������һ���߶��ཻ������Ҫ������ӵ�Բ����Բ��Ϊ��ǰ�㣬���Ϊ��ǰ�߶ε�β�㣬�յ�Ϊ��һ�߶ε���㣬�뾶dist������Ϊ��ʱ��
				{
					// �ڶ�����ƽ�е�
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					// Բ��β��
					pNewCurPnt2 = new ShapePoint(new_cur_x2, new_cur_y2);

#if 0
					// �������Բ����Բ�İ뾶�����յ���Բ����ֱ����Բ���ϲ岹3�������

					// ��
					dx = new_cur_x2 - new_cur_x1;
					dy = new_cur_y2 - new_cur_y1;

					// ���е�
					mid_Interpolate_x = new_cur_x1 + dx/2.0;
					mid_Interpolate_y = new_cur_y1 + dy/2.0;

					// Բ��ָ�����е������
					float mid_vec_x = mid_Interpolate_x - cur_x;
					float mid_vec_y = mid_Interpolate_y - cur_y;

					// �����淶����,���԰뾶�õ�ָ��Բ���е������
					GeneralInterface::NormalVector(mid_vec_x, mid_vec_y, mid_vec_x, mid_vec_y);
					mid_vec_x *= dist;
					mid_vec_y *= dist;

					// ���Բ���е�����
					mid_Interpolate_x = cur_x + mid_vec_x;
					mid_Interpolate_y = cur_y + mid_vec_y;

					// Բ���е�
					ShapePoint* pMidPnt = new ShapePoint(mid_Interpolate_x, mid_Interpolate_y);


					pParallelPolyLine->AddShapePoint(pMidPnt);
					pMidPnt = NULL;

#else				// ������ӻ�

					Cad_PolyLine LinkArc;
					LinkArc.m_ShapePointList = GeneralInterface::ArcInterpolate(pCurPnt, pNewCurPnt1, pNewCurPnt2, dist);

					// Բ����β����ƽ�е��غϣ����Բ��ӣ�����м�岹�㼴��
					int index, nLinkArcShpPntNum;
					nLinkArcShpPntNum = LinkArc.GetShapePointNum();
					for (index = 1; index < nLinkArcShpPntNum-1; index++)
					{
						ShapePoint* pArcPnt = LinkArc.GetShapePoint(index);
						ShapePoint* pInterPoint = new ShapePoint(pArcPnt->m_x, pArcPnt->m_y);

						pParallelPolyLine->AddShapePoint(pInterPoint);
					}

#endif
					pParallelPolyLine->AddShapePoint(pNewCurPnt2);
					pNewCurPnt2 = NULL;

					// ��Ӻ��ÿգ�����Ժ������Ӱ��
					pNewCurPnt1 = NULL;

				}
			}
			else					// �Ǳպ�ͼ��
			{
				pPrePnt = GetShapePoint(i-1);
				pCurPnt = GetShapePoint(i);

				pPrePnt->GetXY(pre_x, pre_y);
				pCurPnt->GetXY(cur_x, cur_y);

				// ƽ��
				rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
				if (rt == FALSE)
				{
					AfxMessageBox("���غϵ㣬�߶�ƽ�г���");
				}

				// ����һ���߶�֮ǰ�Ƿ��н��㣬�еĻ��׵�Ϊ�ཻ��
				if (IsInterWithPrevSeg == TRUE)
				{
					new_pre_x = inter_x;
					new_pre_y = inter_y;
					IsInterWithPrevSeg = FALSE;

					// ����һ��Բ���ཻ��Ӧ��������ཻ��
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);
					pNewCurPnt1 = NULL;
				}
				else
				{
					pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);

					pParallelPolyLine->AddShapePoint(pNewPrePnt);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					pNewPrePnt = NULL;
					pNewCurPnt1 = NULL;

				}
			}
		}
		else		// �м��
		{
			pPrePnt = GetShapePoint(i-1);
			pCurPnt = GetShapePoint(i);
			pNextPnt = GetShapePoint(i+1);

			pPrePnt->GetXY(pre_x, pre_y);
			pCurPnt->GetXY(cur_x, cur_y);
			pNextPnt->GetXY(next_x, next_y);

			// ƽ�к��������
			rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
			if (rt == FALSE)
			{
				AfxMessageBox("���غϵ㣬�߶�ƽ�г���");
			}

			// ����һ���߶�֮ǰ�Ƿ��н��㣬�еĻ��׵�Ϊ�ཻ�㣬 ����Ӧ������׵�
			if (IsInterWithPrevSeg == TRUE)
			{
				new_pre_x = inter_x;
				new_pre_y = inter_y;
				IsInterWithPrevSeg = FALSE;
			}
			else
			{
				// ��һ����ƽ�е�
				pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
				pParallelPolyLine->AddShapePoint(pNewPrePnt);
				pNewPrePnt = NULL;
			}
			
			


			rt = GeneralInterface::ParallelSegment(cur_x, cur_y, next_x, next_y, dir, dist, new_cur_x2, new_cur_y2, new_next_x, new_next_y);
			if (rt == FALSE)
			{
				AfxMessageBox("���غϵ㣬�߶�ƽ�г���");
			}

			// �ж�����һ�����Ƿ��ཻ������120�ȣ�ֱ��ƽ�У�С��120���߶�ƽ��
			float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
			float angle = GeneralInterface::CalVectorAngle(new_cur_x1 - new_pre_x, new_cur_y1 - new_pre_y, new_next_x - new_cur_x2, new_next_y - new_cur_y2);

			if (angle > flag_angle)	 // ���
			{
				IsInterWithNextSeg = GeneralInterface::CalSegmenIntersect2(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// ������
			}
			else					// �۽�
			{
				IsInterWithNextSeg = GeneralInterface::CalLineIntersect(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// ������
			}
			if (IsInterWithNextSeg == TRUE)
			{
				new_cur_x1 = inter_x;
				new_cur_y1 = inter_y;

				IsInterWithPrevSeg = TRUE;	// ��һ��ֱ�����Ϊ����
				IsInterWithNextSeg = FALSE;

				// �ڶ�����ƽ�е�
				pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);


				// ����һ���߶��ཻ����ӵ�ͼ�㣬����Ϊֹ
				pParallelPolyLine->AddShapePoint(pNewCurPnt1);
				pNewCurPnt1 = NULL;

			}
			else	// ������һ���߶��ཻ������Ҫ������ӵ�Բ����Բ��Ϊ��ǰ�㣬���Ϊ��ǰ�߶ε�β�㣬�յ�Ϊ��һ�߶ε���㣬�뾶dist������Ϊ��ʱ��
			{
				// �ڶ�����ƽ�е�
				pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
				pParallelPolyLine->AddShapePoint(pNewCurPnt1);		// ��ǰ���ƽ�е�



				// �������Բ����Բ�İ뾶�����յ���Բ����ֱ����Բ���ϲ岹3�������
#if 0
				// ��
				dx = new_cur_x2 - new_cur_x1;
				dy = new_cur_y2 - new_cur_y1;

				// ���е�
				mid_Interpolate_x = new_cur_x1 + dx/2.0;
				mid_Interpolate_y = new_cur_y1 + dy/2.0;

				// Բ��ָ�����е������
				float mid_vec_x = mid_Interpolate_x - cur_x;
				float mid_vec_y = mid_Interpolate_y - cur_y;

				// �����淶����,���԰뾶�õ�ָ��Բ���е������
				GeneralInterface::NormalVector(mid_vec_x, mid_vec_y, mid_vec_x, mid_vec_y);
				mid_vec_x *= dist;
				mid_vec_y *= dist;

				// ���Բ���е�����
				mid_Interpolate_x = cur_x + mid_vec_x;
				mid_Interpolate_y = cur_y + mid_vec_y;

				// Բ���е�
				ShapePoint* pMidPnt = new ShapePoint(mid_Interpolate_x, mid_Interpolate_y);
				pParallelPolyLine->AddShapePoint(pMidPnt);
				pMidPnt = NULL;
#else
				pNewCurPnt2 = new ShapePoint(new_cur_x2, new_cur_y2);

				Cad_PolyLine LinkArc;
				LinkArc.m_ShapePointList = GeneralInterface::ArcInterpolate(pCurPnt, pNewCurPnt1, pNewCurPnt2, dist);

				// Բ����β����ƽ�е��غϣ����Բ��ӣ�����м�岹�㼴��
				int index, nLinkArcShpPntNum;
				nLinkArcShpPntNum = LinkArc.GetShapePointNum();
				for (index = 1; index < nLinkArcShpPntNum-1; index++)
				{
					ShapePoint* pArcPnt = LinkArc.GetShapePoint(index);
					ShapePoint* pInterPoint = new ShapePoint(pArcPnt->m_x, pArcPnt->m_y);

					pParallelPolyLine->AddShapePoint(pInterPoint);
				}

				delete pNewCurPnt2;
				pNewCurPnt2 = NULL;
#endif

				// ��Ӻ��ÿգ�����Ժ������Ӱ��
				pNewCurPnt1 = NULL;

			}
		}
	}

	// ������Ӧ��Ϣ
	pParallelPolyLine->CalFigureRect();
	pParallelPolyLine->UpdateShapePointInfo();
	pParallelPolyLine->UpdateOutlineInfo();

	return pParallelPolyLine;
}