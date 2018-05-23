// DlgWeiXin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgWeiXin.h"
#include "afxdialogex.h"
#include "../Misc/Misc.h"


// CDlgWeiXin �Ի���

IMPLEMENT_DYNAMIC(CDlgWeiXin, CDialog)

CDlgWeiXin::CDlgWeiXin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWeiXin::IDD, pParent)
	, m_pImg(NULL)
{

}

CDlgWeiXin::~CDlgWeiXin()
{
	if(m_pImg)
		delete m_pImg;
}

void CDlgWeiXin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgWeiXin, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgWeiXin ��Ϣ�������
BOOL CDlgWeiXin::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// TODO:  �ڴ���Ӷ���ĳ�ʼ��
#ifndef YiWei
	m_pImg = LoadPngImgFromRes(IDB_PNG_QRCODE_LARGE);
#else
	m_pImg = LoadPngImgFromRes(IDB_PNG_QRCODE_LARGE_YW);
#endif
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgWeiXin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	COLORREF colBK = RGB(224, 241, 252);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);

	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());


	if(m_pImg)
	{
		RectF rfDes((REAL)(rcClient.left+(rcClient.Width()-m_pImg->GetWidth())/2), \
			(REAL)(rcClient.top+(rcClient.Height()-m_pImg->GetHeight())/2), \
			(REAL)m_pImg->GetWidth(), \
			(REAL)m_pImg->GetHeight());
		g.DrawImage(m_pImg, rfDes, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight(), UnitPixel);
	}



	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgWeiXin::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}