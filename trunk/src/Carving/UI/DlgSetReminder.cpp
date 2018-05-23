// DlgSetReminder.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "Carving.h"
#include "DlgSetReminder.h"
//#include "afxdialogex.h"


// CDlgSetReminder �Ի���

IMPLEMENT_DYNAMIC(CDlgSetReminder, CDialog)

CDlgSetReminder::CDlgSetReminder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetReminder::IDD, pParent)
{

}

CDlgSetReminder::~CDlgSetReminder()
{
}

void CDlgSetReminder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetReminder, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSetReminder::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetReminder ��Ϣ�������
BOOL CDlgSetReminder::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < m_vReminder.size() ; i++)
	{
		((CComboBox*)(GetDlgItem(IDC_COMBO_REMINDER)))->InsertString(i, m_vReminder[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSetReminder::AppendReminder(LPCTSTR szReminder)
{
	for (int i = 0; i < m_vReminder.size() ; i++)
	{
		if (m_vReminder[i].Compare(szReminder) == 0)
		{
			return;
		}
	}
	m_vReminder.push_back(szReminder);
}

CString CDlgSetReminder::GetReminder()
{
	return m_strRet;
}

void CDlgSetReminder::OnBnClickedOk()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REMINDER);
	pComboBox->GetWindowText(m_strRet);
	CDialog::OnOK();
}