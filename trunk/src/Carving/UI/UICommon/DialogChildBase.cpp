// DialogChildBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "Carving.h"
#include "DialogChildBase.h"
//#include "afxdialogex.h"


// CDialogChildBase �Ի���

IMPLEMENT_DYNAMIC(CDialogChildBase, CDialog)

//CDialogChildBase::CDialogChildBase(CWnd* pParent /*=NULL*/)
//	: CDialog(CDialogChildBase::IDD, pParent)
//{
//
//}

CDialogChildBase::CDialogChildBase(UINT uID, CWnd* pParent)
	: CDialog(uID, pParent)
{

}

CDialogChildBase::~CDialogChildBase()
{
}

void CDialogChildBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogChildBase, CDialog)
END_MESSAGE_MAP()


// CDialogChildBase ��Ϣ�������
BOOL CDialogChildBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)  
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogChildBase::OnOK()
{
	return;
}
