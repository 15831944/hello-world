// TheChangeCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "TheChangeCode.h"
#include "afxdialogex.h"


// TheChangeCode �Ի���

IMPLEMENT_DYNAMIC(TheChangeCode, CDialogEx)

TheChangeCode::TheChangeCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(TheChangeCode::IDD, pParent)
{
	ChangeFirst1 = _T("");
	ChangeLast1 = _T("");
	ChangeFirst2 = _T("");
	ChangeLast2 = _T("");
}

TheChangeCode::~TheChangeCode()
{
}

void TheChangeCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_CHANGEEDIT1,ChangeFirst1);
	DDX_Text(pDX,IDC_CHANGEEDITL1,ChangeLast1);
	DDX_Text(pDX,IDC_CHANGEDIT2,ChangeFirst2);
	DDX_Text(pDX,IDC_CHANGEDITL2,ChangeLast2);
}

BOOL TheChangeCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
BEGIN_MESSAGE_MAP(TheChangeCode, CDialogEx)
END_MESSAGE_MAP()


// TheChangeCode ��Ϣ�������
