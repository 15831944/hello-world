/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfFile.cpp    -- dxf�ļ���ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	��ȡdxf�ļ���ͼԪ��Ϣ
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DxfFile.h"

#include "../DataManager/CSingleton.h"
#include "../DataManager/Layer/Layer.h"
#include "../DataManager/Figure/PolyLine.h"
#include "../DataManager/Point/ShapePoint.h"
#include "../DataManager/Figure/Arc.h"
#include "../DataManager/Figure/Circle.h"
#include "../DataManager/CommonData.h"
#include "../GeneralInterface/GeneralInterface.h"

// ��̬���ݳ�ʼ��
CString DxfFile::SectionStar = _T("SECTION");
CString DxfFile::SectionEnd = _T("ENDSEC");
CString DxfFile::EntitySectionStar = _T("ENTITIES");





// ����
DxfFile::DxfFile()
{
}

// ����
DxfFile::~DxfFile()
{
	m_File.Close();
}

// ����dxf�ļ�
int DxfFile::Load(CString file_name)
{
	BOOL b;
	TCHAR   szCause[255];
	CFileException* e = new CFileException;

	memset(szCause, 0, 255);

	b = m_File.Open(file_name, CStdioFile::modeRead, e);
	if (b != TRUE)
	{
		e->GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return -1;
	}

	return 0;
};

// ��ȡͼ��
void DxfFile::ReadFigureData()
{
	// �ж��ļ�����Ƿ���Ч
	if (m_File.m_hFile != CFile::hFileNull)
	{
		CString dxf_code, dxf_val;
		while(m_File.ReadString(dxf_code))
		{
			m_File.ReadString(dxf_val);

			// ��ȡʵ���
			if (dxf_val == EntitySectionStar)
			{
				ReadEntitySection();
			}
		}
	}
}

// ��ȡʵ���
void DxfFile::ReadEntitySection()
{
	CString dxf_code, dxf_val;

	while(m_File.ReadString(dxf_code))
	{
		m_File.ReadString(dxf_val);
		if (dxf_val == _T("ARC"))
		{
			ReadArc();
		}
		else  if (dxf_val == _T("LINE"))
		{
			ReadPolyline();
		}
		else  if (dxf_val == _T("CIRCLE"))
		{
			ReadCircle();
		}
	}
}

// ��ȡ����
void DxfFile::ReadPolyline()
{
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = NULL;

	int layer_id;
	float x, y;
	CString dxf_code, dxf_val;
	Cad_PolyLine* pPoly = new Cad_PolyLine(); 
	CString FirstX = _T(" 10");
	CString FirstY = _T(" 20");
	CString SecondX = _T(" 11");
	CString SecondY = _T(" 21");


	while(m_File.ReadString(dxf_code))
	{
		m_File.ReadString(dxf_val);

		if (dxf_code == _T("  8"))
		{
			layer_id = _ttoi(dxf_val);
		}
		else if (dxf_code == FirstX)		// ��һ�����x����
		{
			x = (float)_ttof(dxf_val);
		}
		else  if (dxf_code == FirstY)	// ��һ�����y����
		{
			y = (float)_ttof(dxf_val);

			ShapePoint* pPnt = new ShapePoint(x, y); 
			pPoly->AddShapePoint(pPnt);
		}
		else if (dxf_code == SecondX)
		{
			x = (float)_ttof(dxf_val);
		}
		else if (dxf_code == SecondY)
		{
			y = (float)_ttof(dxf_val);

			ShapePoint* pPnt = new ShapePoint(x, y); 
			pPoly->AddShapePoint(pPnt);
			pPoly->CalOutlineList();

			// ѹ������
			//m_FigureList.push_back(pPoly);

			pLayer = pSingleton->GetLayer(layer_id);
			while (pLayer == NULL)
			{
				pSingleton->NewLayer();
				pLayer = pSingleton->GetLayer(layer_id);
			}

			pLayer->AddFigure(pPoly);

			return ;
		}
	}
}


// ��ȡԲ 
void DxfFile::ReadCircle()
{
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = NULL;

	int layer_id;
	float x, y, radius;
	CString dxf_code, dxf_val;
	Cad_Circle* pCircle = new Cad_Circle; 

	while(m_File.ReadString(dxf_code))
	{
		m_File.ReadString(dxf_val);

		if (dxf_code == _T("  8"))
		{
			layer_id = _ttoi(dxf_val);
		}
		else if (dxf_code == _T(" 10"))		// ��һ�����x����
		{
			x = (float)_ttof(dxf_val);
		}
		else  if (dxf_code == _T(" 20"))	// ��һ�����y����
		{
			y = (float)_ttof(dxf_val);
		}
		else if (dxf_code == _T(" 40"))
		{
			radius = (float)_ttof(dxf_val);

			// �����Բ
			delete pCircle;
			pCircle = NULL;
// 			pCircle->m_CenterX = x;
// 			pCircle->m_CenterY = y;
// 			pCircle->m_radius = radius;
// 
// 			// ������״��,����״����Ϣ���¼���Բ�ģ��뾶
// 			pCircle->CenterRadius2ShapePoint();
// 			pCircle->UpdateCenterRadius();
// 			pCircle->CalOutlineList();

			//m_FigureList.push_back(pCircle);
			//pLayer->AddFigure(pCircle);

			pLayer = pSingleton->GetLayer(layer_id);
			while (pLayer == NULL)
			{
				pSingleton->NewLayer();
				pLayer = pSingleton->GetLayer(layer_id);
			}


			// ��Ҫ�����Բ��
			Cad_Arc* pArc1 = new Cad_Arc;
			Cad_Arc* pArc2 = new Cad_Arc;
			Cad_Arc* pArc3 = new Cad_Arc;
			Cad_Arc* pArc4 = new Cad_Arc;


			ShapePoint* pShpPnt1_1 = new ShapePoint(radius + x,				y);
			ShapePoint* pShpPnt1_2 = new ShapePoint(x + radius/sqrt(2.0),	y + radius/sqrt(2.0));
			ShapePoint* pShpPnt1_3 = new ShapePoint(x,						y + radius);

			ShapePoint* pShpPnt2_1 = new ShapePoint(x,						y + radius);
			ShapePoint* pShpPnt2_2 = new ShapePoint(x - radius/sqrt(2.0),		y + radius/sqrt(2.0));
			ShapePoint* pShpPnt2_3 = new ShapePoint(x - radius,				y);

			ShapePoint* pShpPnt3_1 = new ShapePoint(x - radius,				y);
			ShapePoint* pShpPnt3_2 = new ShapePoint(x - radius/sqrt(2.0),		y - radius/sqrt(2.0));
			ShapePoint* pShpPnt3_3 = new ShapePoint(x ,						y - radius);

			ShapePoint* pShpPnt4_1 = new ShapePoint(x ,						y - radius);
			ShapePoint* pShpPnt4_2 = new ShapePoint(x + radius/sqrt(2.0),		y - radius/sqrt(2.0));
			ShapePoint* pShpPnt4_3 = new ShapePoint(x + radius,				y);

			pArc1->AddShapePoint(pShpPnt1_1);
			pArc1->AddShapePoint(pShpPnt1_2);
			pArc1->AddShapePoint(pShpPnt1_3);
			pArc1->CalCenterRadiusInfo();
			pArc1->CalOutlineList();

			pArc2->AddShapePoint(pShpPnt2_1);
			pArc2->AddShapePoint(pShpPnt2_2);
			pArc2->AddShapePoint(pShpPnt2_3);
			pArc2->CalCenterRadiusInfo();
			pArc2->CalOutlineList();

			pArc3->AddShapePoint(pShpPnt3_1);
			pArc3->AddShapePoint(pShpPnt3_2);
			pArc3->AddShapePoint(pShpPnt3_3);
			pArc3->CalCenterRadiusInfo();
			pArc3->CalOutlineList();

			pArc4->AddShapePoint(pShpPnt4_1);
			pArc4->AddShapePoint(pShpPnt4_2);
			pArc4->AddShapePoint(pShpPnt4_3);
			pArc4->CalCenterRadiusInfo();
			pArc4->CalOutlineList();

			pLayer->AddFigure(pArc1);
			pLayer->AddFigure(pArc2);
			pLayer->AddFigure(pArc3);
			pLayer->AddFigure(pArc4);

			return ;
		}
	}
}

// ��ȡԲ��
void DxfFile::ReadArc()
{
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = NULL;

	int layer_id;
	float x, y, radius, star_angle, end_angle;
	CString dxf_code, dxf_val;
	Cad_Arc* pArc = new Cad_Arc; 


	while(m_File.ReadString(dxf_code))
	{
		m_File.ReadString(dxf_val);

		if (dxf_code == _T("  8"))
		{
			layer_id = _ttoi(dxf_val);
		}
		else if (dxf_code == _T(" 10"))		// ��һ�����x����
		{
			x = (float)_ttof(dxf_val);
		}
		else  if (dxf_code == _T(" 20"))	// ��һ�����y����
		{
			y = (float)_ttof(dxf_val);
		}
		else if (dxf_code == _T(" 40"))
		{
			radius = (float)_ttof(dxf_val);
		}
		else if (dxf_code == _T(" 50"))
		{
			star_angle = (float)_ttof(dxf_val);
			star_angle = (float)Angle2Radian(star_angle);
		}	
		else if (dxf_code == _T(" 51"))
		{
			end_angle = (float)_ttof(dxf_val);
			end_angle = (float)Angle2Radian(end_angle);

			pArc->m_CenterX = x;
			pArc->m_CenterY = y;
			pArc->m_radius = radius;
			pArc->m_StartAngle = star_angle;
			pArc->m_EndAngle = end_angle;
			
			// ������״��,����״����Ϣ���¼���Բ�ģ��뾶����ʼ�����Ƕ�
			pArc->CenterRadius2ShapePoint();
			pArc->CalCenterRadiusInfo();

			pLayer = pSingleton->GetLayer(layer_id);
			while (pLayer == NULL)
			{
				pSingleton->NewLayer();
				pLayer = pSingleton->GetLayer(layer_id);
			}


			// ��Բ�ĽǴ���180�ȵ�Բ����һ��Ϊ��
			float center_angle = pArc->GetCentralAngle();	// ��ȡԲ�Ľǣ�
			if (center_angle > PI*2/3)	
			{
				float mid_x1, mid_y1, mid_x2, mid_y2;

				ShapePoint* pHeadPnt = pArc->GetShapePoint(0);
				ShapePoint* pMidPnt = pArc->GetShapePoint(1);
				ShapePoint* pTailPnt = pArc->GetShapePoint(2);


				// ��ӵ�һ��Բ��
				Cad_Arc* pNewArc1 = new Cad_Arc;
				ShapePoint* pNewHeadPnt1 = new ShapePoint;
				ShapePoint* pNewMidPnt1 = new ShapePoint;
				ShapePoint* pNewTailPnt1 = new ShapePoint;

				// �����е�
				GeneralInterface::CalArcMidPoint(pHeadPnt->m_x, pHeadPnt->m_y, pMidPnt->m_x, pMidPnt->m_y, pArc->m_CenterX, pArc->m_CenterY, pArc->m_radius, mid_x1, mid_y1);
				
				// ������״��
				*pNewHeadPnt1 = *pHeadPnt;
				pNewMidPnt1->SetXY(mid_x1, mid_y1);
				*pNewTailPnt1 = *pMidPnt;

				// �����״��
				pNewArc1->AddShapePoint(pNewHeadPnt1);
				pNewArc1->AddShapePoint(pNewMidPnt1);
				pNewArc1->AddShapePoint(pNewTailPnt1);

				// ������Ϣ
				pNewArc1->UpdateShapePointInfo();
				pNewArc1->CalCenterRadiusInfo();
				pNewArc1->CalOutlineList();


				// ��ӵڶ���Բ��
				Cad_Arc* pNewArc2 = new Cad_Arc;
				ShapePoint* pNewHeadPnt2 = new ShapePoint;
				ShapePoint* pNewMidPnt2 = new ShapePoint;
				ShapePoint* pNewTailPnt2 = new ShapePoint;

				// �����е�
				GeneralInterface::CalArcMidPoint(pMidPnt->m_x, pMidPnt->m_y, pTailPnt->m_x, pTailPnt->m_y, pArc->m_CenterX, pArc->m_CenterY, pArc->m_radius, mid_x2, mid_y2);

				// ������״��
				*pNewHeadPnt2 = *pMidPnt;
				pNewMidPnt2->SetXY(mid_x2, mid_y2);
				*pNewTailPnt2 = *pTailPnt;

				// �����״��
				pNewArc2->AddShapePoint(pNewHeadPnt2);
				pNewArc2->AddShapePoint(pNewMidPnt2);
				pNewArc2->AddShapePoint(pNewTailPnt2);

				// ������Ϣ
				pNewArc2->UpdateShapePointInfo();
				pNewArc2->CalCenterRadiusInfo();
				pNewArc2->CalOutlineList();


				// ���
				pLayer->AddFigure(pNewArc1);
				pLayer->AddFigure(pNewArc2);
			}
			else
			{
				pArc->CalOutlineList();
				pLayer->AddFigure(pArc);
			}

			return ;
		}
	}
}