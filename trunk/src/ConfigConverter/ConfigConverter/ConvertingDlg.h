#pragma once
#include "afxcmn.h"

#define WM_UPDATE_PROGRESS_BAR WM_USER+1

class CConvertingDlg;
struct ConvertThreadData
{
	CConvertingDlg* m_pDlg;
	CString m_strSourceDir;
	CString m_strTargetDir;
};

// CConvertingDlg �Ի���

class CConvertingDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertingDlg)

public:
	CConvertingDlg(LPCTSTR szSourceDir, LPCTSTR szTargetDir, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConvertingDlg();

// �Ի�������
	enum { IDD = IDD_CONVERTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	void StartConvertThread();
	static DWORD WINAPI FuncConvert( PVOID lpThreadParameter );
	afx_msg LRESULT OnUpdateProgressBar(WPARAM wParam, LPARAM lParam);

	CString m_strSourceDir;
	CString m_strTargetDir;
	HANDLE m_hConvertThread;
	ConvertThreadData m_ThreadData;
	CProgressCtrl m_ProgressBar;
};