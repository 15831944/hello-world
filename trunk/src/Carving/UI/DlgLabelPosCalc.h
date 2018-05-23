#pragma once
#include "../Resource.h"

// CDlgLabelPosCalc �Ի���

class CDlgLabelPosCalc : public CDialog
{
	DECLARE_DYNAMIC(CDlgLabelPosCalc)

public:
	CDlgLabelPosCalc(LPCTSTR szMachineName, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLabelPosCalc();

// �Ի�������
	enum { IDD = IDD_DLG_LABEL_POS_CALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();					//���ڳ�ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam);	// ��Ӧ��Ϣ
	DECLARE_MESSAGE_MAP()

	void StartTieBiaoCalcThread();
	static DWORD WINAPI TieBiaoPosCalcThread( PVOID lpThreadParameter );				// �Ż��̺߳���
	
	CProgressCtrl m_ProgressBar;
	HANDLE m_hdthreadTieBiaoCalc;					// �����߳̾��
	CString m_strMachineName;
};
