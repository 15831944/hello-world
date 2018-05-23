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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Carving.h"

#include "MainFrm.h"

#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/BaseInfo/BaseInfo.h"
#include "UI/ParamDlg.h"
#include "UI/UICommon/RibbonButtonSelf.h"
#include "UI/UICommon/RibbonCategorySelf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
	: m_pDlgBaseParam(NULL)
	, m_pBtnAboutUs(NULL)
	, m_pBtnOurCustomers(NULL)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
	if(m_pDlgBaseParam)
	{
		if (m_pDlgBaseParam->GetSafeHwnd())
		{
			m_pDlgBaseParam->DestroyWindow();
		}
		delete m_pDlgBaseParam;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	//m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	CString strTmp;
	strTmp.Format(_T("%s ������Ʒ %s"), g_szCompanyFullName, g_szVersion);
	strTitlePane2 = strTmp;
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, /*strTitlePane1*/_T(""), TRUE), /*strTitlePane1*/_T(""));
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	//// ���������á�Outlook��������:
	//if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	//{
	//	TRACE0("δ�ܴ�����������\n");
	//	return -1;      // δ�ܴ���
	//}

	// ����������:
	//if (!CreateCaptionBar())
	//{
	//	TRACE0("δ�ܴ���������\n");
	//	return -1;      // δ�ܴ���
	//}

	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	m_pDlgBaseParam = new CParamDlg;
	m_pDlgBaseParam->loadSettings();



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

//BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
//{
//	bar.SetMode2003();
//
//	BOOL bNameValid;
//	CString strTemp;
//	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
//	ASSERT(bNameValid);
//	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
//	{
//		return FALSE; // δ�ܴ���
//	}
//
//	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();
//
//	if (pOutlookBar == NULL)
//	{
//		ASSERT(FALSE);
//		return FALSE;
//	}
//
//	pOutlookBar->EnableInPlaceEdit(TRUE);
//
//	static UINT uiPageID = 1;
//
//	// �ɸ��������Զ����أ��ɵ�����С�������ܹر�
//	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;
//
//	CRect rectDummy(0, 0, 0, 0);
//	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
//
//	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
//	bNameValid = strTemp.LoadString(IDS_FOLDERS);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);
//
//	calendar.Create(rectDummy, &bar, 1201);
//	bNameValid = strTemp.LoadString(IDS_CALENDAR);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);
//
//	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
//
//	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
//	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
//	pOutlookBar->RecalcLayout();
//
//	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
//	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);
//
//	bar.SetButtonsFont(&afxGlobalData.fontBold);
//
//	return TRUE;
//}

//BOOL CMainFrame::CreateCaptionBar()
//{
//	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
//	{
//		TRACE0("δ�ܴ���������\n");
//		return FALSE;
//	}
//
//	BOOL bNameValid;
//
//	CString strTemp, strTemp2;
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButtonToolTip(strTemp);
//
//	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);
//
//	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
//	ASSERT(bNameValid);
//	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);
//
//	return TRUE;
//}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//void CMainFrame::OnViewCaptionBar()
//{
//	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
//	RecalcLayout(FALSE);
//}

//void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
//}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::InitializeRibbon()//��ʼ��Ribbon������
{
	CString strTemp;
	//���ϽǵĲ˵���ť��ʼ��
	strTemp = L"�ļ�";
	m_PanelImages.SetImageSize(CSize(16, 16));//�������Ͻǲ˵���ťͼ���С
	m_PanelImages.Load(IDB_BUTTONS);// ���ز˵�ͼ��
	//��ʼ�����˵���ť
#ifndef YiWei
	m_MainButton.SetImage(IDB_MAIN);
#else
	m_MainButton.SetImage(IDB_MAIN_YW);
#endif
	
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));

	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	//strTemp = L"new";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	//strTemp = L"open";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	//strTemp = L"save";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	//strTemp = L"save as";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	//strTemp = L"print";
	//CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
	//pBtnPrint->SetKeys(_T("p"), _T("w"));
	//strTemp = L"print label";
	//pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
	//strTemp = L"print quick";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
	//strTemp = L"print preview";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
	//strTemp = L"print setup";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
	//pMainPanel->Add(pBtnPrint);

	//strTemp = L"close";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	//strTemp = L"recent doc";
	//pMainPanel->AddRecentFilesList(strTemp);

	//strTemp = L"exit";
	//pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));


#pragma region CreateRibbonBar
	strTemp = /*L"����"*/_T("��ʼ");
#ifndef YiWei
	//CMFCRibbonCategory* pCategoryImport = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL/*IDB_PNG1*/, IDB_WRITELARGE1/*IDB_PNG2*/);
	//CRibbonCategorySelf* pCategoryImport = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1, RUNTIME_CLASS(CRibbonCategorySelf));
	CRibbonCategorySelf* pCategoryStart = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
	//AddCategory(LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID,
	//	CSize sizeSmallImage = CSize(16, 16), CSize sizeLargeImage = CSize(32, 32), int nInsertAt = -1, CRuntimeClass* pRTI = NULL)
#else
	CRibbonCategorySelf* pCategoryStart = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif


	//strTemp = L"�Ż�";
	//CMFCRibbonCategory* pCategoryOptimize = m_wndRibbonBar.AddCategory(strTemp,IDB_WRITESMALL,IDB_WRITELARGE1);

	//strTemp = L"����";
	//CMFCRibbonCategory* pCategoryExport = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);

	strTemp = _T("����");
#ifndef YiWei
	//CMFCRibbonCategory* pCategorySetting = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);
	CRibbonCategorySelf* pCategorySetting = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
#else
	CRibbonCategorySelf* pCategorySetting = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif


	strTemp = L"����";
#ifndef YiWei
	//CMFCRibbonCategory* pCategoryHelp = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);
	CRibbonCategorySelf* pCategoryHelp = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
#else
	CRibbonCategorySelf* pCategoryHelp = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif

	strTemp = L"����";
	CMFCRibbonPanel* pPanelImportMethod = pCategoryStart->AddPanel(strTemp/*, m_PanelImages.ExtractIcon (1)*/);

#ifndef YiWei
	strTemp = L"HG�ļ�";
	CRibbonButtonSelf* pBtnImportHG = new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE, FALSE, true, true);
#else
	strTemp = L"YW�ļ�";
	CRibbonButtonSelf* pBtnImportHG = new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE_YW, FALSE, true, true);
#endif
	
	
	pBtnImportHG->SetMenu(IDR_MENU_OPEN_HGE, TRUE);
	pPanelImportMethod->Add(pBtnImportHG);

// 	strTemp = L"HG�ļ�";
// 	//pPanelImportMethod->Add(new CMFCRibbonButton(ID_OPEN_SOURCE_HG,strTemp,0,0));
// 	pPanelImportMethod->Add(new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE, FALSE, true, true));

	strTemp = L"Excel";
	CRibbonButtonSelf* pBtnImportExcel = new CRibbonButtonSelf(ID_OPEN_SOURCE_EXCEL,strTemp,0,0, IDB_PNG_EXCEL, FALSE, true, true);
	pBtnImportExcel->SetMenu(IDR_MENU_OPEN_EXCEL, TRUE);
	pPanelImportMethod->Add(pBtnImportExcel);





	strTemp = L"�Ż�����";
	//pPanelImportMethod->Add(new CMFCRibbonButton(ID_OPEN_SOLUTION,strTemp,2,2));
	pPanelImportMethod->Add(new CRibbonButtonSelf(ID_OPEN_SOLUTION,strTemp,0,0, IDB_PNG_TIME_FOLDER, FALSE, true, true));





	strTemp = L"�Ż�";
	CMFCRibbonPanel* pPanelOptimize = /*pCategoryOptimize*/pCategoryStart->AddPanel(strTemp);

	strTemp = L"�Ż�";
	//pPanelOptimize->Add(new CMFCRibbonButton(ID_OPTIMIZE,strTemp,3,3));
	pPanelOptimize->Add(new CRibbonButtonSelf(ID_OPTIMIZE,strTemp,0,0, IDB_PNG_TIME, FALSE, true, true));

	strTemp = L"����";
	CMFCRibbonPanel* pPanelSave = pCategoryStart->AddPanel(strTemp);

	strTemp = L"����";
	pPanelSave->Add(new CRibbonButtonSelf(ID_SAVE,strTemp,0,0, IDB_PNG_SAVE, FALSE, true, true));

	strTemp = L"����";
	CMFCRibbonPanel* pPanelExport = /*pCategoryExport*/pCategoryStart->AddPanel(strTemp);

	strTemp = L"��ӡ";
	//pPanelExport->Add(new CMFCRibbonButton(ID_BUTTON_PRINT_LABEL,strTemp,5,5));
	pPanelExport->Add(new CRibbonButtonSelf(ID_BUTTON_PRINT_LABEL,strTemp,0,0, IDB_PNG_PRINT, FALSE, true, true));

	strTemp = L"NC��";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_GCODE,strTemp,6,6));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_GCODE,strTemp,0,0, IDB_PNG_NC_CODE, FALSE, true, true));

	strTemp = L"�ϵ�1";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));

	strTemp = L"�ϵ�2";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));

	strTemp = L"��ӡ���ϵ�";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_PANEL_METERIAL_TABLE,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_PANEL_METERIAL_TABLE,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));


	strTemp = L"��ϵ����";
	CMFCRibbonPanel* pPanelContactWithUs = pCategoryStart->AddPanel(strTemp);

	strTemp = L"QQ";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19));
	pPanelContactWithUs->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));

	strTemp = L"΢��";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19));
	pPanelContactWithUs->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

	//strTemp = L"����";
	//CMFCRibbonPanel* pPanelPragma = /*pCategorySetting*/pCategoryImport->AddPanel(strTemp);

	//m_pEditKerf = new CMFCRibbonEdit(ID_EDIT_PRAGMA_SAW_KERF, 50, _T("�����:"));
	//pPanelPragma->Add(m_pEditKerf);
	//m_pEditKerf->SetEditText(_T("6"));
	//m_pEditPanelLength = new CMFCRibbonEdit(ID_EDIT_PANEL_LENGTH, 50, _T("��峤��:"));
	//pPanelPragma->Add(m_pEditPanelLength);
	//m_pEditPanelLength->SetEditText(_T("2432"));
	//m_pEditPanelWidth = new CMFCRibbonEdit(ID_EDIT_PANEL_WIDTH, 50, _T("�����:"));
	//pPanelPragma->Add(m_pEditPanelWidth);
	//m_pEditPanelWidth->SetEditText(_T("1212"));

	//m_pEditStep1Count = new CMFCRibbonEdit(ID_EDIT_STEP1_COUNT, 50, _T("��һ�׶��Ż�����:"));
	//pPanelPragma->Add(m_pEditStep1Count);
	//m_pEditStep1Count->SetEditText(_T("200"));

	//m_pComboStep1Alg = new CMFCRibbonComboBox(ID_COMBO_STEP1_ALG, 0, 100, _T("��һ�׶��Ż��㷨:"));
	//pPanelPragma->Add(m_pComboStep1Alg);
	//m_pComboStep1Alg->AddItem(_T("�Ľ�����������㷨"));
	//m_pComboStep1Alg->AddItem(_T("�Ľ�̰���㷨"));
	//m_pComboStep1Alg->AddItem(_T("����㷨"));
	//m_pComboStep1Alg->SelectItem(1);

	//m_pEditStep2Count = new CMFCRibbonEdit(ID_EDIT_STEP2_COUNT, 50, _T("�ڶ��׶��Ż�����:"));
	//pPanelPragma->Add(m_pEditStep2Count);
	//m_pEditStep2Count->SetEditText(_T("0"));

	//m_pEditStep3Count = new CMFCRibbonEdit(ID_EDIT_STEP3_COUNT, 50, _T("�����׶��Ż�����:"));
	//pPanelPragma->Add(m_pEditStep3Count);
	//m_pEditStep3Count->SetEditText(_T("0"));
	//m_pEditStep3AcceptableUti = new CMFCRibbonEdit(ID_EDIT_STEP3_ACCEPTABLE_UTI, 50, _T("�����׶οɽ���������:"));
	//pPanelPragma->Add(m_pEditStep3AcceptableUti);
	//m_pEditStep3AcceptableUti->SetEditText(_T("0.9"));


	//m_pComboLayoutOrg = new CMFCRibbonComboBox(ID_COMBO_LAYOUT_ORIGIN, 0, 100, _T("����ԭ��:"));
	//pPanelPragma->Add(m_pComboLayoutOrg);
	//m_pComboLayoutOrg->AddItem(_T("���½�"));
	//m_pComboLayoutOrg->AddItem(_T("���Ͻ�"));
	//m_pComboLayoutOrg->AddItem(_T("���½�"));
	//m_pComboLayoutOrg->AddItem(_T("���Ͻ�"));
	//m_pComboLayoutOrg->SelectItem(1);

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	m_pBtnAboutUs = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	m_pBtnAboutUs->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs = pCategoryStart->AddPanel(strTemp);
	pPanelAboutUs->Add(m_pBtnAboutUs);
	//strTemp = L"                             �ɹ��û�                                      ";
	//CMFCRibbonPanel* pPanelOurCustomers = pCategoryStart->AddPanel(strTemp);
	//pPanelOurCustomers->Add(m_pBtnOurCustomers);
#else
	strTemp = L"a                                                                                                                                                   a";
	m_pBtnAboutUs = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	m_pBtnAboutUs->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs = pCategoryStart->AddPanel(strTemp);
	pPanelAboutUs->Add(m_pBtnAboutUs);
#endif

	CMFCRibbonLabel* pLabel = new CMFCRibbonLabel(_T("hello"));

	POINT ptIconCenter;
	ptIconCenter.x = 70;
	ptIconCenter.y = 45;

#ifndef YiWei
	pCategoryStart->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
	pCategorySetting->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
	pCategoryHelp->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
#else
	pCategoryStart->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
	pCategorySetting->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
	pCategoryHelp->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
#endif

	//strTemp = L"a                                                                                              a";
	///*CRibbonButtonSelf* */m_pBtnAboutUs = new CRibbonButtonSelf/*CMFCRibbonButton*/(ID_BUTTON_PRINT_LABEL,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE);
	
	strTemp = L"����";
	CMFCRibbonPanel* pPanelSetting = /*pCategorySetting*/pCategorySetting->AddPanel(strTemp);

	strTemp = L"��������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_BASE_SETTING,strTemp,8,8));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_BASE_SETTING,strTemp,0,0, IDB_PNG_OPERATION, FALSE, true, true));

	strTemp = L"����";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_MACHINE_PARAM_SETTING,strTemp,7,7));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_MACHINE_PARAM_SETTING,strTemp,0,0, IDB_PNG_MACHINE, FALSE, true, true));

	strTemp = L"ģ������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_EXCEL_TEMPLATE_SETTING,strTemp,15,15));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_EXCEL_TEMPLATE_SETTING,strTemp,0,0, IDB_PNG_EXCEL_FILE, FALSE, true, true));

	strTemp = L"��ǩ����";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_LABEL_SETTING,strTemp,14,14));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_LABEL_SETTING,strTemp,0,0, IDB_PNG_LABEL_SETTING, FALSE, true, true));

	strTemp = L"�Ż�����";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_OPTIMIZE_SETTING,strTemp,9,9));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OPTIMIZE_SETTING,strTemp,19,19, IDB_PNG_TIME, FALSE, true, true));

	strTemp = L"��������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_REMAINDER_SETTING,strTemp,10,10));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_REMAINDER_SETTING,strTemp,19,19, IDB_PNG_REMAINDER, FALSE, true, true));

	strTemp = L"���ϵ�����";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_LINGLIAO_TABLE_SETTING,strTemp,16,16));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_LINGLIAO_TABLE_SETTING,strTemp,19,19, IDB_PNG_FILE, FALSE, true, true));

	strTemp = L"Ĭ�ϼ�������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_DEFAULT_LOADING_DIR_SETTING,strTemp,11,11));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_DEFAULT_LOADING_DIR_SETTING,strTemp,19,19, IDB_PNG_LOAD_SETTING, FALSE, true, true));

	strTemp = L"Ĭ�ϱ�������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_DEFAULT_SAVING_DIR_SETTING,strTemp,12,12));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_DEFAULT_SAVING_DIR_SETTING,strTemp,19,19, IDB_PNG_SAVE_SETTING, FALSE, true, true));

	strTemp = L"��������";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_OTHER_SETTING,strTemp,13,13));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OTHER_SETTING,strTemp,19,19, IDB_PNG_SETTINGS, FALSE, true, true));

	strTemp = L"��ģ��";
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OPEN_TEMPLATE,strTemp,19,19, IDB_PNG_EXCEL_FILE, FALSE, true, true));

	strTemp = L"������������";
	CMFCRibbonPanel* pPanelSettingSave = pCategorySetting->AddPanel(strTemp);

	strTemp = L"����";
	pPanelSettingSave->Add(new CRibbonButtonSelf(ID_MENU_SETTING_SAVE,strTemp,0,0, IDB_PNG_SETTINGS_SAVE, FALSE, true, true));

	strTemp = L"��ԭ";
	pPanelSettingSave->Add(new CRibbonButtonSelf(ID_MENU_SETTING_REVERT,strTemp,0,0, IDB_PNG_SETTINGS_REVERT, FALSE, true, true));

	strTemp = L"��ϵ����";
	CMFCRibbonPanel* pPanelContactWithUs_InSetting = pCategorySetting->AddPanel(strTemp);
	strTemp = L"QQ";
	pPanelContactWithUs_InSetting->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));
	strTemp = L"΢��";
	pPanelContactWithUs_InSetting->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	CRibbonButtonSelf* pBtnAboutUs_InSetting = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	pBtnAboutUs_InSetting->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs_InSetting = pCategorySetting->AddPanel(strTemp);
	pPanelAboutUs_InSetting->Add(pBtnAboutUs_InSetting);
#else
	strTemp = L"a                                                                                                                                                   a";
	CRibbonButtonSelf* pBtnAboutUs_InSetting = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	pBtnAboutUs_InSetting->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs_InSetting = pCategorySetting->AddPanel(strTemp);
	pPanelAboutUs_InSetting->Add(pBtnAboutUs_InSetting);
#endif


	strTemp = L"����";
	CMFCRibbonPanel* pPanelHelp = pCategoryHelp->AddPanel(strTemp);

	strTemp = L"�̳�";
	pPanelHelp->Add(new CRibbonButtonSelf(ID_MENU_HELP_TUTORIAL,strTemp,0,0, IDB_PNG_TUTORIAL, FALSE, true, true));

#ifndef YiWei
	strTemp = L"��ѧ��Ƶ";
	pPanelHelp->Add(new CRibbonButtonSelf(ID_MENU_VIDEO,strTemp,0,0, IDB_PNG_VIDEO, FALSE, true, true));
#endif

	strTemp = L"��ϵ����";
	CMFCRibbonPanel* pPanelContactWithUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	strTemp = L"QQ";
	pPanelContactWithUs_InHelp->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));
	strTemp = L"΢��";
	pPanelContactWithUs_InHelp->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	CRibbonButtonSelf* pBtnAboutUs_InHelp = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	pBtnAboutUs_InHelp->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	pPanelAboutUs_InHelp->Add(pBtnAboutUs_InHelp);
#else
	strTemp = L"a                                                                                                                                                   a";
	CRibbonButtonSelf* pBtnAboutUs_InHelp = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	pBtnAboutUs_InHelp->SetToolTipText(_T("�򿪹���"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	pPanelAboutUs_InHelp->Add(pBtnAboutUs_InHelp);
#endif

#pragma endregion


	//// ��ӿ��ٷ��ʹ���������:
	//CList<UINT, UINT> lstQATCmds;

	//lstQATCmds.AddTail(ID_FILE_NEW);
	//lstQATCmds.AddTail(ID_FILE_OPEN);
	//lstQATCmds.AddTail(ID_FILE_SAVE);
	//lstQATCmds.AddTail(ID_EDIT_UNDO);

	//m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds,true);
	CMFCRibbonQuickAccessToolBarDefaultState* qaToolBarState = new CMFCRibbonQuickAccessToolBarDefaultState();
	qaToolBarState->AddCommand(ID_FILE_NEW, true);
	qaToolBarState->AddCommand(ID_FILE_OPEN, true);
	qaToolBarState->AddCommand(ID_FILE_SAVE, true);
	m_wndRibbonBar.SetQuickAccessDefaultState(*qaToolBarState);


	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

//void CMainFrame::GetAlgBaseInfo(BaseInfo& info)
//{
//	CString strTmp;
//
//	// ȡ��ʹ�ã����û��������е�����
//	// ��������� 
//// 	strTmp = m_pEditKerf->GetEditText();
//// 	info.m_SawKerfWidth = atof(strTmp);
//// 	strTmp = m_pEditPanelLength->GetEditText();
//// 	info.m_PanelLength = atof(strTmp);
//// 	strTmp = m_pEditPanelWidth->GetEditText();
//// 	info.m_PanelWidth = atof(strTmp);
//
//	// ��һ�׶β���
//	strTmp = m_pEditStep1Count->GetEditText();
//	info.m_FirstSectionOPTimes = atoi(strTmp);
//	info.m_FirstSectionOPMethod = m_pComboStep1Alg->GetCurSel();
//
//	// �ڶ��׶β���
//	strTmp = m_pEditStep2Count->GetEditText();
//	info.m_SecondSectionOPTimes = atoi(strTmp);
//
//	// �����׶β���
//	strTmp = m_pEditStep3Count->GetEditText();
//	info.m_ThirdSectionOPTimes = atoi(strTmp);
//	strTmp = m_pEditStep3AcceptableUti->GetEditText();
//	info.m_ThirdSectionOAccptableUtilization = atof(strTmp);
//
//	//// ����ԭ��
//	//info.m_LayoutOrg = m_pComboLayoutOrg->GetCurSel();
//
//}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pBtnAboutUs)
	{
		//
		//CRect rc(0, 0, 100, 30);
		//m_pBtnSelf->SetRect(rc);
	}
}
