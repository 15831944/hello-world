// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "Resource.h"
#include "UI/UICommon/RibbonButtonSelf.h"
#include "UI/UICommon/RibbonBarSelf.h"

class BaseInfo;
class CParamDlg;

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

	CParamDlg* m_pDlgBaseParam;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//void GetAlgBaseInfo(BaseInfo& info);

protected:  // �ؼ���Ƕ���Ա
	/*CMFCRibbonBar*/CRibbonBarSelf     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;
	//CMFCCaptionBar    m_wndCaptionBar;
	CRibbonButtonSelf/*CMFCRibbonButton*/* m_pBtnAboutUs;
	CRibbonButtonSelf* m_pBtnOurCustomers;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	//BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	//BOOL CreateCaptionBar();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

protected:
	void InitializeRibbon();

	CMFCRibbonEdit* m_pEditKerf;
	CMFCRibbonEdit* m_pEditPanelLength;
	CMFCRibbonEdit* m_pEditPanelWidth;
	CMFCRibbonEdit* m_pEditStep1Count;
	CMFCRibbonComboBox* m_pComboStep1Alg;
	CMFCRibbonComboBox* m_pComboLayoutOrg;
	CMFCRibbonEdit* m_pEditStep2Count;
	CMFCRibbonEdit* m_pEditStep3Count;
	CMFCRibbonEdit* m_pEditStep3AcceptableUti;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


