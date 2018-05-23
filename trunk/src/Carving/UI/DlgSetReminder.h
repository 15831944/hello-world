#pragma once
#include "../resource.h"
#include <vector>

using namespace std;

// CDlgSetReminder �Ի���

class CDlgSetReminder : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetReminder)

public:
	CDlgSetReminder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetReminder();

// �Ի�������
	enum { IDD = IDD_DLG_SET_REMINDER };

	void AppendReminder(LPCTSTR szReminder);
	CString GetReminder();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

	vector<CString> m_vReminder;
	CString m_strRet;
};
