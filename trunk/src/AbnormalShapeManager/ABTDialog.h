#ifndef			_ABTDialog_h_ 
#define			_ABTDialog_h_


#include "Resource.h "

#include <string>
using namespace std;

// ��ͼGDI+
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")



// TinyXml��
#include "../../include/tinyxml/tinyxml.h"
#ifdef DEBUG
#pragma  comment(lib, "../../lib/tinyxmld.lib")
#pragma  message(".......................................��ǰΪdebug�汾")
#else
#pragma  comment(lib, "../../lib/tinyxml.lib")
#pragma  message(".......................................��ǰΪrelease�汾")
#endif




// �ⲿ������

class Graphics;
class Figure;

#define Program_2_LiJun					(0)			// ����ĳ�����Ҫ����xml�ͽ�ͼ

#define		MIN_MOVE_STEP				(2)			// ��С�ƶ����� ��λ������


#define		ZOOM_STEP					(30)		// ÿ�����ŵĲ���
#define		ZOOM_MAX_RANGE				(5000)		// �����С��������Ϊ5000mm
#define		ZOOM_MIN_RANGE				(10)		// ���Ŵ󣬰�����Ϊ50mm



#define		BK_CLOLOR					(Color::White)
#define		CROSS_CLOLOR				(Color::LightGray)
#define		OUTLINE_CLOLOR_UNSELECT		(Color::Blue)
#define		OUTLINE_CLOLOR_SELECT		(Color::Red)
#define		SHAPEPOINT_CLOLOR			(Color::Red)
#define		FIGURE_RECT_CLOLOR			(Color::Gray)


class AFX_EXT_CLASS ABTDialog : public CDialog
{
public:
	ABTDialog(CWnd* pParentWnd = NULL);
	~ABTDialog();

private:
	enum {IDD = IDD_ABT_DLG};

private:
	// �˵���Դ
	CMenu m_Menu2;
	HACCEL m_hAccel;

public:
	// ��ͼʹ��

	CRect m_ShowRect;			//�����ϻ滭���������
	CPoint m_ShowRectCenter;	//�����ϻ滭�������������
	int m_nPanelWidth;			//�������ߴ�
	int m_nPanelLength;			//�������ߴ�

	BOOL m_ShowShapePointFlag;
	BOOL m_ShowOutlineFlag;
	BOOL m_ShowFigStartDirFlag;


public:
	// XML���� 
	string m_strXmlNode;
	void SaveXml(CString FilePath);
	void SaveJPG(CString FilePath);

public:
	void DrawWindowBackground(Gdiplus::Graphics* g);
	void ShowRect_DrawBackground(Gdiplus::Graphics* g, CRect& rt);
	void ShowRect_DrawCross(Gdiplus::Graphics* g, CRect& rt);


	void ShowRect_DrawFgure(Gdiplus::Graphics* g);			// ��ͼ��
	void ShowRect_DrawFgureRect(Gdiplus::Graphics* g);		// ��ѡ��ͼ�ξ���
	void ShowRect_DrawFgureDir(Gdiplus::Graphics* g);		// ��ͼ�η���
	void ShowRect_DrawShapePoint(Gdiplus::Graphics* g);		// ����״��
	void ShowRect_DrawPolygon(Gdiplus::Graphics* g);		// ����

	void ShowRect_TransCoorFromCad2Window(float cx, float cy, int& wx, int& wy);		// CAD->���� ����ת�� 
	void ShowRect_TransCoorFromCad2Window(float cx, float cy, float& wx, float& wy);	// CAD->���� ����ת�� 
	void ShowRect_TransCoorFromCad2Window(CRect cad_rt, CRect& win_rt);					// CAD->���� ����ת��

public:
	 TinyXml::TiXmlElement* m_pPanelOutlineNode;
	 CString m_strPanelOutlineNode;


public:
	// �ж�ͼ��ѡ��
	BOOL JudgeIfFigureSelect(Figure* pFig, CPoint pt);
	void SelectFigure(CPoint pt);


public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpenDxf();
	afx_msg void OnBnClickedBtnReverseFigure();
	afx_msg void OnBnClickedBtnSaveXml();



	afx_msg void OnBnClickedOk();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnMove2StandardPosition();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnScreenCapture();
	afx_msg void OnAccelSelectAllFigure();
	afx_msg void OnUpdateAccelSelectAllFigure(CCmdUI *pCmdUI);
	afx_msg void OnSelectAllFigure();
	afx_msg void OnBnClickedButtonParallel();
	afx_msg void OnDelFigure();
	afx_msg void OnShowShapePoint();
	afx_msg void OnShowOutline();
	afx_msg void OnUpdateShowShapePoint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowOutline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowFigureStartDir(CCmdUI *pCmdUI);
	afx_msg void OnShowFigureStartDir();
	afx_msg void OnMenuImportRequirement();
};

#endif

