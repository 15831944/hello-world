// DirSetDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "DirSetDlg.h"
#include "afxdialogex.h"


// CDirSetDlg dialog

IMPLEMENT_DYNAMIC(CDirSetDlg, CDialogEx)

CDirSetDlg::CDirSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirSetDlg::IDD, pParent)
{

}

CDirSetDlg::~CDirSetDlg()
{
}

void CDirSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}


BEGIN_MESSAGE_MAP(CDirSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDirSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDirSetDlg message handlers


void CDirSetDlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);
	int nItem = m_combo.GetCurSel();
	if (nItem == 0)
	{
		m_value = "������";
	}
	else if (nItem == 1)
	{
		m_value = "����";
	}
	else
	{
		m_value = "����";
	}
	UpdateData(FALSE);
	CDialogEx::OnOK();
}


BOOL CDirSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	UpdateData(TRUE);
	/*m_combo.InsertString(0,"������");
	m_combo.InsertString(1,"����");
	m_combo.InsertString(2,"����");*/
	m_combo.InsertString(0,"������ת");
	m_combo.InsertString(1,"����ת");
	m_combo.InsertString(2,"��ת90��");
	m_combo.SetCurSel(0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}