#pragma once
#include "../Resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "./UICommon/SkinListCtrlBase.h"
#include "SkinHeaderCtrl.h"
#include "../UIData/UIData.h"

// CDirSetDlg dialog


class OptimizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OptimizeDlg)

public:
	OptimizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptimizeDlg();

	// Dialog Data
	enum { IDD = IDD_OPTIMIZE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	// �Ż���غ���
	static int m_LastPanelNum;
	static vector<ComponentInputItem> m_vComponentInputItem;		// Ҫ�Ż��İ��
	static vector<PreCombineItem> m_vPreCombineItem;
	static vector<RemainderInputItem> m_vRemainderInputItem;		// ����




	static BOOL m_ThreadIsRunning;		// �Ż��߳����б�־
	HANDLE m_hdthread;					// �Ż��߳̾��

	void StartOptimizeThread();
	static DWORD WINAPI OptimizeThread( PVOID lpThreadParameter );				// �Ż��̺߳���
	afx_msg LRESULT OnWM_CALCULATE(WPARAM wParam, LPARAM lParam);				// ��Ӧ��Ϣ
	afx_msg LRESULT OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam);	// ��Ӧ��Ϣ
	afx_msg LRESULT OnWM_WM_UPDATE_REMAIN_TIME(WPARAM wParam, LPARAM lParam);	// ��Ӧ��Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedUseNow();
	virtual BOOL OnInitDialog();					//���ڳ�ʼ��




	CStatic m_TxtOpMessage;
	CProgressCtrl m_ProgressBar;







	CStatic m_TxtOpMessage2;
	CSkinListCtrlBase m_ListCtrlSolutionInfo;
	CSkinHeaderCtrl m_hcSolutionInfo;
	CButton m_BtnUseNow;
	CStatic m_TxtRemainTime;
};
