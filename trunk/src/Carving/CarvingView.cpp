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

// CarvingView.cpp : CCarvingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Carving.h"
#endif

#include "CarvingDoc.h"
#include "CarvingView.h"
#include "MainFrm.h"

// �ⲿDataManager.dll��ͷ�ļ�
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../include/DataManager/BaseDataType/RemainderManager/RemainderManager.h"
#include "../../include/DataManager/BaseDataType/RemainderGroup/RemainderGroup.h"
#include "../../include/DataManager/BaseDataType/RemainderItem/RemainderItem.h"


// �ⲿFileReadWrite.dll��ͷ�ļ�
#include "../../include/FileReadWrite/ExcelReadWrite/ExcelReadWrite.h"
#include "../../include/FileReadWrite/HgmReadWrite/HgmReadWrite.h"
#include "../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"
#include "../../include/FileReadWrite/NCReadWrite/NCReadWrite.h"
#include "../../include/FileReadWrite/ClientInfoReadWrite/ClientInfoReadWrite.h"
#include "../../include/FileReadWrite/DxfReadWrite/DxfReadWrite.h"
#include "../../include/FileReadWrite/RemainderLibraryReadWrite/RemainderLibraryReadWrite.h"
#include "../../include/FileReadWrite/Misc/HGCode.h"



// �ⲿInternetManager.dll��ͷ�ļ�
#include "../../include/InternetManager/InternetChecking/InternetChecking.h"


#include "UI/ParamDlg.h"
#include "UI/SelMachineDlg.h"
#include "UI/SelectPathDlg.h"
#include "UI/DlgRemainder.h"
#include "UI/DlgRequirement.h"
#include "UI/DlgResult.h"
#include "UI/SelectPrintDlg.h"
#include "UI/ClientRegisterDlg.h"
#include "UI/KnifeAddDlg.h"
#include "UI/OPSetDlg.h"
#include "UI/LeftPanelDlg.h"
#include "UI/UISettingDlg.h"
#include "UI/PrintSetDlg.h"
#include "UI/InOutDlg.h"
#include "UI/LingLiaoDlg.h"
#include "UI/TiebiaoEdgeDlg.h"
#include "UI/OptimizeDlg.h"
#include "UI/DlgLabelPosCalc.h"
#include "UI/DlgWeiXin.h"
#include "UI/DlgExcelInputSetting.h"
#include "UI/RemainderDlg.h"

#include "DataProcess/DataProcess.h"
#include "NCProcess/SyntecProcess.h"
#include "NCProcess/XiaoFengLabelProcess.h"

#include "Misc/HGTools.h"
#include "Misc/ProgramMisc.h"
#include "Misc/Misc.h"

#include "Work/MachineDef.h"

#include "afx.h"

#include <fstream>
#include <io.h>

#include <algorithm>

#include "NCProcess/CNCDataConverter.h"

#include "Work/ProgramDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//class CSelMachineDlg;
//class KnifeClass;
//class CParamDlg;
struct PrepareExportGCodeData
{
	PrepareExportGCodeData()
		:m_pSelMachineDlg(NULL)
		,node_double(false)
		,key(1)
		,pParamDlg(NULL)
		,_cutDouble(false)
		,nodecoor_type(0)
		,nodecoor_type1(0)
		,nodecoor_type2(0)
		,node_print(0)
		,node_sysuse(false)
		,_lessKnifeChange(0)
		,_isLabelInReverseSide(false)
		,other_str(0)
		,_isComline(1)
		,_isComball(0)
	{}

	CSelMachineDlg* m_pSelMachineDlg;
	std::string node_name;
	bool node_double;
	int node_print;
	bool node_sysuse;
	std::string node_groupstr;

	int nodecoor_type;
	int nodecoor_type1;//����λ����ϵ
	int nodecoor_type2;//˫��λ����ϵ

	bool _cutDouble;//��˫��λ�ж�

	std::string node_cxhx;
	std::string node_pxhx;

	std::string node_pname;
	std::string node_plast;
	std::string node_cname;
	std::string node_clast;
	std::string node_dvcut;

	std::string temp_header1;//���湤λ1��2����ʱͷβ
	std::string temp_ender1;
	std::string temp_header2;
	std::string temp_ender2;

	std::string temp_GFirst2_1;//���湤λ2��2����ʱͷβ
	std::string temp_GLast2_1;
	std::string temp_GFirst2_2;
	std::string temp_GLast2_2;

	std::string temp_GBFirst1_1;//���湤λ1��2����ʱͷβ
	std::string temp_GBLast1_1;
	std::string temp_GBFirst1_2;
	std::string temp_GBLast1_2;

	std::string temp_GBFirst2_1;//���湤λ2��2����ʱͷβ
	std::string temp_GBLast2_1;
	std::string temp_GBFirst2_2;
	std::string temp_GBLast2_2;

	float fCutTwice_SmallSize;
	float fCutTwice_ReserveHeight;
	float fCutTwice_SecondSpeed;
	bool bCutTwice_Enable;
	float fTwoCutThreshold;
	float fTwoCutReserve;
	float fTwoCutSpeed;

	int key;

	CParamDlg* pParamDlg;
	CString pathName;
	CString pathPrint;

	int _lessKnifeChange;
	int _knife_shifting;
	bool _isLabelInReverseSide;
	int other_str;
	int _isComline;
	int _isComball;

	KnifeClass* pKnifeClass;
};
struct PrepareKnifeInfoData
{
	PrepareKnifeInfoData()
		:pThePanel(NULL)
	{}

	Panel* pThePanel;
	std::vector<KnifeType> kt;
};

struct TransCoordinateData
{
	std::string nc_font_temp_path;
	std::string nc_font_path;
	std::string nc_back_w_path;
	std::string nc_back_w_temp_path;
	std::string nc_back_h_path;
	std::string nc_back_h_temp_path;
	bool HaveDvsh;
	bool HaveDvsw;
	bool HaveDvs;

	int _usedLineX;
	int _usedLineY;

	int nDecimal;
};

struct CombineFileData
{
	std::string str_nextthin;
	std::string tempnextcname2;
	std::string str_thin;

	std::string nc_font_temp_path;
	std::string nc_font_path;
	std::string nc_back_h_path;
	std::string nc_back_w_path;
	std::string allpath2;
};

void ExcelSettingForTheFistTime(CString strFilePath);
//#ifndef REQUIREMENT_LIST_TITLE
//#define REQUIREMENT_LIST_TITLE

//#endif

// CCarvingView


IMPLEMENT_DYNCREATE(CCarvingView, CFormView)

BEGIN_MESSAGE_MAP(CCarvingView, CFormView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCarvingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_OPEN_SOURCE_HG, &CCarvingView::OnOpenSourceHG)
	ON_COMMAND(ID_OPEN_SOURCE_HG, &CCarvingView::OnOpenSourcePicInfo)
	ON_COMMAND(ID_MENU_OPEN_HGE_APPEND, &CCarvingView::OnOpenSourceHGAppend)
	ON_COMMAND(ID_MENU_OPEN_EXCEL_APPEND, &CCarvingView::OnOpenSourceExcelAppend)
	ON_COMMAND(ID_OPTIONS_PEN, &CCarvingView::OnOptionsPen)
	ON_COMMAND(ID_VIEW_REMAINDER, &CCarvingView::OnViewRemainder)
	ON_COMMAND(ID_VIEW_COMPONENT, &CCarvingView::OnViewComponent)
	ON_COMMAND(ID_VIEW_SOLUTION, &CCarvingView::OnViewSolution)
	ON_WM_SIZE()
	ON_COMMAND(ID_OPTIMIZE, &CCarvingView::OnOptimize)
	ON_COMMAND(ID_SAVE, &CCarvingView::OnSave)
	ON_COMMAND(ID_EDIT_PRAGMA_SAW_KERF, &CCarvingView::OnEditPragmaSawKerf)
	ON_COMMAND(ID_EDIT_PANEL_LENGTH, &CCarvingView::OnEditPanelLength)
	ON_COMMAND(ID_EDIT_PANEL_WIDTH, &CCarvingView::OnEditPanelWidth)
	ON_COMMAND(ID_EDIT_STEP1_COUNT, &CCarvingView::OnEditStep1Count)
	ON_COMMAND(ID_EDIT_STEP2_COUNT, &CCarvingView::OnEditStep2Count)
	ON_COMMAND(ID_EDIT_STEP3_COUNT, &CCarvingView::OnEditStep3Count)
	ON_COMMAND(ID_EDIT_STEP3_ACCEPTABLE_UTI, &CCarvingView::OnEditStep3AcceptableUti)
	ON_COMMAND(ID_COMBO_STEP1_ALG, &CCarvingView::OnComboStep1Alg)
	ON_COMMAND(ID_EXPORT_GCODE, &CCarvingView::OnExportGcode)
	ON_COMMAND(ID_COMBO_LAYOUT_ORIGIN, &CCarvingView::OnLayoutOrg)
	ON_COMMAND(ID_BUTTON_PRINT_LABEL, &CCarvingView::OnButtonPrintLabel)
	ON_COMMAND(ID_MENU_MACHINE_PARAM_SETTING, &CCarvingView::OnMenuMachineParamSetting)
	ON_COMMAND(ID_MENU_BASE_SETTING, &CCarvingView::OnMenuBaseSetting)
	ON_COMMAND(ID_MENU_OPTIMIZE_SETTING, &CCarvingView::OnMenuOptimizeSetting)
	ON_COMMAND(ID_MENU_REMAINDER_SETTING, &CCarvingView::OnRemainderSetting)
	ON_COMMAND(ID_MENU_DEFAULT_LOADING_DIR_SETTING, &CCarvingView::OnMenuDefaultLoadingDirSetting)
	ON_COMMAND(ID_MENU_DEFAULT_SAVING_DIR_SETTING, &CCarvingView::OnMenuDefaultSavingDirSetting)
	ON_COMMAND(ID_MENU_OTHER_SETTING, &CCarvingView::OnMenuOtherSetting)
	ON_COMMAND(ID_MENU_LABEL_SETTING, &CCarvingView::OnMenuLabelSetting)
	ON_COMMAND(ID_MENU_EXCEL_TEMPLATE_SETTING, &CCarvingView::OnMenuExcelTemplateSetting)
	ON_COMMAND(ID_MENU_LINGLIAO_TABLE_SETTING, &CCarvingView::OnMenuLingliaoTableSetting)
	ON_COMMAND(ID_EXPORT_METERIAL_TABLE1, &CCarvingView::OnExportMeterialTable1)
	ON_COMMAND(ID_EXPORT_METERIAL_TABLE2, &CCarvingView::OnExportMeterialTable2)
	ON_COMMAND(ID_OPEN_SOURCE_EXCEL, &CCarvingView::OnOpenSourceExcel)
	ON_COMMAND(ID_OPEN_SOLUTION, &CCarvingView::OnOpenSolution)
	ON_COMMAND(ID_EXPORT_PANEL_METERIAL_TABLE, &CCarvingView::OnExportPanelMeterialTable)
	ON_MESSAGE(WM_USER_EXPORT_MATERIAL_LIST, &CCarvingView::OnExportMaterialList)
	ON_MESSAGE(WM_USER_EXPORT_COMPONENT_NC, &CCarvingView::OnExportComponentNC)
	ON_COMMAND(ID_BUTTON_ABOUT_US, &CCarvingView::OnButtonAboutUs)
	ON_COMMAND(ID_BUTTON_OUR_CUSTOMERS, &CCarvingView::OnButtonOurCustomers)
	ON_COMMAND(ID_MENU_CONTACK_WITH_US_QQ, &CCarvingView::OnButtonContackWithUsQQ)
	ON_COMMAND(ID_MENU_CONTACK_WITH_US_WEIXIN, &CCarvingView::OnButtonContackWithUsWeiXin)
	ON_COMMAND(ID_MENU_HELP_TUTORIAL, &CCarvingView::OnButtonTutorial)
	ON_COMMAND(ID_MENU_VIDEO, &CCarvingView::OnButtonVideo)
	ON_COMMAND(ID_MENU_OPEN_TEMPLATE, &CCarvingView::OnButtonOpenTemplate)
	ON_COMMAND(ID_MENU_SETTING_SAVE, &CCarvingView::OnMenuSaveAllSettings)
	ON_COMMAND(ID_MENU_SETTING_REVERT, &CCarvingView::OnMenuRevertAllSettings)
END_MESSAGE_MAP()

// CCarvingView ����/����

CCarvingView::CCarvingView()
	: CFormView(IDD)
	, m_eCurViewType(VIEW_PANEL_LIST)
	, m_pDlgRemainder(NULL)
	, m_pDlgRequirement(NULL)
	, m_pDlgResult(NULL)
{
	// TODO: �ڴ˴���ӹ������

}

CCarvingView::~CCarvingView()
{
}

void CCarvingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TAB, m_Tab);
}

BOOL CCarvingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

// CCarvingView ����

void CCarvingView::OnDraw(CDC* /*pDC*/)
{
	CCarvingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CCarvingView ��ӡ


void CCarvingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCarvingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCarvingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCarvingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CCarvingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCarvingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCarvingView ���

#ifdef _DEBUG
void CCarvingView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCarvingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCarvingDoc* CCarvingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarvingDoc)));
	return (CCarvingDoc*)m_pDocument;
}
#endif //_DEBUG




// CCarvingView ��Ϣ�������
void CCarvingView::OnOpenSourceHG()
{

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CSingleon* pSingleton = CSingleon::GetSingleton();

#ifndef YiWei
	CString filter = "hge �ļ�(*.hge)|*.hge|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hge"), _T("*.hge"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(hge)");
#else
	CString filter = "ywe �ļ�(*.ywe)|*.ywe|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywe"), _T("*.ywe"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(ywe)");
#endif
	


	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		m_strOpenedFile = fileDlg.GetPathName();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

#ifndef YiWei
		if (strExtName == "hge")
#else
		if (strExtName == "ywe")
#endif
		{
			//GetParent()->SetWindowText("def");
			GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//�����
			param.b_downvecImportEnable		= param_dlg->DVecInput();//�����
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//�����
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//�����
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//���ι���
			param.i_comnum	= param_dlg->Editnum();//�и�����
			param.i_comchange = param_dlg->Comchange();//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
			param.f_changex = param_dlg->MoveX();//�ײ�ƫ��
			param.f_changey = param_dlg->MoveY();//�ײ�ƫ��	
			param.vecAdder = param_dlg->GetVecAdder();//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��		
			param._cxy = param_dlg->GetVecFilter();//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(HgmReadWrite::ReadPanelInfo(m_strOpenedFile, m_vComponentInputItem, param) == true)
			{
				//// ��������ظ�
				//CheckRepeatBarcode(m_vComponentInputItem);


				// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
				//CheckComponentList(m_vComponentInputItem);

				// С����Ϊ������
				if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
				{
					SetSmallCpnNoTexture(m_vComponentInputItem);
				}

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				ParamSettingDlg dlgOtherSetting;
				dlgOtherSetting.loadSettings();
				ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
				if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
				{
					ModifyReminderByDateAndOrderFormat(m_vComponentInputItem);
				}
			}

			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);

			// �Զ������Ż�����
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

		}


		// �л���ͼ
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
		
		RecordMaterial();
	}

	// ���ý�����������
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem);


	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);





}


#define  INCH_TO_MM		(25.4)		// Ӣ��ת���� 



// CCarvingView ��Ϣ�������
void CCarvingView::OnOpenSourcePicInfo()
{

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString filter = "xml �ļ�(*.xml)|*.xml|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("xml"), _T("*.xml"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("��������Դ����(xml)");




	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		m_strOpenedFile = fileDlg.GetPathName();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xml")
		{
			GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//�����
			param.b_downvecImportEnable		= param_dlg->DVecInput();//�����
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//�����
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//�����
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//���ι���
			param.i_comnum	= param_dlg->Editnum();//�и�����
			param.i_comchange = param_dlg->Comchange();//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
			param.f_changex = param_dlg->MoveX();//�ײ�ƫ��
			param.f_changey = param_dlg->MoveY();//�ײ�ƫ��	
			param.vecAdder = param_dlg->GetVecAdder();//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��		
			param._cxy = param_dlg->GetVecFilter();//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();


			// �ĵ�
			TiXmlDocument* doc = new TiXmlDocument;
			doc->LoadFile(m_strOpenedFile.GetBuffer());

			// ���ڵ�
			TiXmlElement *pRootElement = doc->RootElement();
			if (pRootElement == NULL)
			{
				AfxMessageBox("���ļ�");
				//return false;
			}

			//ѭ����ȡͼƬ��Ϣ
			for (TiXmlElement* pPicSetElem = pRootElement->FirstChildElement("ͼƬ��"); pPicSetElem != NULL; pPicSetElem = (TiXmlElement*)(pPicSetElem->NextSibling()))
			{
				for (TiXmlElement* pCurPic = pPicSetElem->FirstChildElement("ͼƬ"); pCurPic != NULL; pCurPic = (TiXmlElement*)(pCurPic->NextSibling()))
				{
					string pic_path = pCurPic->Attribute("·��");
					int	num = stoi(pCurPic->Attribute("����"));

					// ����ͼƬ����
					
					const wchar_t* pwc = HGCode::char_Gb2312_To_Unicode(pic_path.c_str());
					Image tmp_img(pwc);


					UINT w	= tmp_img.GetWidth();
					UINT h	= tmp_img.GetHeight();
					UINT hr = tmp_img.GetHorizontalResolution();		// dpi ÿӢ����ٸ����ص�
					UINT vr = tmp_img.GetVerticalResolution();			// dpi

					float w_inch = (1.0*w)/hr;
					float h_inch = (1.0*h)/vr;

					float w_mm = w_inch*INCH_TO_MM;
					float h_mm = h_inch*INCH_TO_MM;


					// �γ�һ������
					ComponentInputItem componentInputItem;

					componentInputItem.m_strBarcode = pic_path.c_str();
					componentInputItem.m_fLength	= w_mm;
					componentInputItem.m_fWidth		= h_mm;
					componentInputItem.m_nCount		= num;
					componentInputItem.m_strTexture = "����";

					m_vComponentInputItem.push_back(componentInputItem);

				}
			}



			//if(HgmReadWrite::ReadPanelInfo(m_strOpenedFile, m_vComponentInputItem, param) == true)
// 			{
// 				//// ��������ظ�
// 				//CheckRepeatBarcode(m_vComponentInputItem);
// 
// 
// 				// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
// 				//CheckComponentList(m_vComponentInputItem);
// 
// 				// С����Ϊ������
// 				if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
// 				{
// 					SetSmallCpnNoTexture(m_vComponentInputItem);
// 				}
// 
// 				for(int i = 0; i < m_vComponentInputItem.size(); i++)
// 					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;
// 
// 				ParamSettingDlg dlgOtherSetting;
// 				dlgOtherSetting.loadSettings();
// 				ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
// 				if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
// 				{
// 					ModifyReminderByDateAndOrderFormat(m_vComponentInputItem);
// 				}
// 			}

			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);

			// �Զ������Ż�����
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

		}


		// �л���ͼ
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();

		RecordMaterial();
	}

	// ���ý�����������
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem);


	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);


}

/*---------------------------------------*/
//	����˵����
//		����ظ�����
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
BOOL CCarvingView::IsRepeatBarcode(vector<ComponentInputItem>& src_list, vector<ComponentInputItem>& add_list)
{
	int nSrcCount = src_list.size();
	int nAddCount = add_list.size();

	if (nSrcCount == 0)
	{
		return FALSE;
	}

	for (int i_add = 0; i_add < nAddCount; i_add++)
	{
		ComponentInputItem& add_cpn = add_list.at(i_add);
		CString strAddBarcode = add_cpn.m_strBarcode;

		for (int i_src = 0; i_src < nSrcCount; i_src++)
		{
			ComponentInputItem& src_cpn = src_list.at(i_src);
			CString strSrcBarcode = src_cpn.m_strBarcode;

			// ����ͬ������
			if (strSrcBarcode == strAddBarcode)
			{

				//AfxMessageBox("��⵽���ظ�����:" + strSrcBarcode +  "�������׷�ӣ�");
				return TRUE;
			}
		}
	}

	return FALSE;
}



/*---------------------------------------*/
//	����˵����
//		����ظ�����
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
BOOL CCarvingView::IsRepeatBarcode(vector<ComponentInputItem>& src_list)
{
	int nSrcCount = src_list.size();

	if (nSrcCount == 0)
	{
		return FALSE;
	}

	CString msg;
	int i,j;
	BOOL flag = FALSE;
	for ( i = 0; i < nSrcCount-1; i++)
	{
		for ( j = i+1; j < nSrcCount; j++)
		{
			ComponentInputItem& cpn1 = src_list.at(i);
			CString strBarcode1 = cpn1.m_strBarcode;


			ComponentInputItem& cpn2 = src_list.at(j);
			CString strBarcode2 = cpn2.m_strBarcode;

			// ����ͬ������
			if (strBarcode1 == strBarcode2)
			{
				msg += "��⵽���ظ�����:" + strBarcode1 +  "\n";
				flag = TRUE;
			}
		}
	}

	if (flag == TRUE)
	{
		//AfxMessageBox(msg);
		return TRUE;
	}
	else
		return FALSE;
}





// ׷��
void CCarvingView::OnOpenSourceHGAppend()
{

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	CSingleon* pSingleton = CSingleon::GetSingleton();

#ifndef YiWei
	CString filter = "hge �ļ�(*.hge)|*.hge|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hge"), _T("*.hge"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(hge)");
#else
	CString filter = "ywe �ļ�(*.ywe)|*.ywe|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywe"), _T("*.ywe"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(ywe)");
#endif

	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		//m_vComponentInputItem.clear();
		vector<ComponentInputItem> vAppendComponentInputItem;


		m_strOpenedFile = fileDlg.GetPathName();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

#ifndef YiWei
		if (strExtName == "hge")
#else
		if (strExtName == "ywe")
#endif
		{
			//GetParent()->SetWindowText("def");
			GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//�����
			param.b_downvecImportEnable		= param_dlg->DVecInput();//�����
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//�����
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//�����
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//���ι���
			param.i_comnum	= param_dlg->Editnum();//�и�����
			param.i_comchange = param_dlg->Comchange();//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
			param.f_changex = param_dlg->MoveX();//�ײ�ƫ��
			param.f_changey = param_dlg->MoveY();//�ײ�ƫ��	
			param.vecAdder = param_dlg->GetVecAdder();//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��		
			param._cxy = param_dlg->GetVecFilter();//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(HgmReadWrite::ReadPanelInfo(m_strOpenedFile, vAppendComponentInputItem, param) == true)
			{
				// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
				//CheckComponentList(vAppendComponentInputItem);

				// С����Ϊ������
				if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
				{
					SetSmallCpnNoTexture(vAppendComponentInputItem);
				}

				// ׷��֮ǰ���ж��Ƿ��������ظ�
				//if (CheckRepeatBarcode(m_vComponentInputItem, vAppendComponentInputItem) == FALSE)
				{
					// ׷��
					for(int i = 0; i < vAppendComponentInputItem.size(); i++)
						vAppendComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

					ParamSettingDlg dlgOtherSetting;
					dlgOtherSetting.loadSettings();
					ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
					if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
					{
						ModifyReminderByDateAndOrderFormat(vAppendComponentInputItem);
					}
					m_vComponentInputItem.insert(m_vComponentInputItem.end(), vAppendComponentInputItem.begin(), vAppendComponentInputItem.end());
				}


				//// �ٴ��ж��ظ�����
				//CheckRepeatBarcode(m_vComponentInputItem);
			}

			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);

			// �Զ������Ż�����
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

		}


		// �л���ͼ
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// ���ý�����������
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem, false);

	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

}

/*---------------------------------------*/
//	����˵����
//		׷��excel
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void CCarvingView::OnOpenSourceExcelAppend()
{
	// TODO: �ڴ���������������

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString filter = "xls �ļ�(*.xls)|*.xls|�����ļ� (*.*)|*.*||";

	CFileDialog fileDlg(true, _T("xls"), _T("*.xls"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(excel)");
	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		//m_vComponentInputItem.clear();
		vector<ComponentInputItem> vAppendComponentInputItem;


		m_strOpenedFile = fileDlg.GetPathName();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xls" || strExtName == "xlsx" )
		{
			GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//�����
			param.b_downvecImportEnable		= param_dlg->DVecInput();//�����
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//�����
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//�����
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//���ι���
			param.i_comnum	= param_dlg->Editnum();//�и�����
			param.i_comchange = param_dlg->Comchange();//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
			param.f_changex = param_dlg->MoveX();//�ײ�ƫ��
			param.f_changey = param_dlg->MoveY();//�ײ�ƫ��	
			param.vecAdder = param_dlg->GetVecAdder();//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��		
			param._cxy = param_dlg->GetVecFilter();//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
			param.f_width = param_dlg->Width();
			param.f_height = param_dlg->Height();
			param.f_Deburringwidth = param_dlg->Deburringwidth();
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(ExcelReadWrite::ReadPanelInfo(m_strOpenedFile, vAppendComponentInputItem, param) == true)
			{
				
			}
			else
			{
				if (strExtName == "xlsx")
				{
					AfxMessageBox("����office���������⣬ϵͳ�޷�ʹ��Excel 2003 �Զ���ȡ Excel 2007����� xlsx�ļ�! \n��˫��" + m_strOpenedFile + ",���Ϊxls�ļ����ٵ��룡");
				}

			}

			// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
			//CheckComponentList(vAppendComponentInputItem);
			// С����Ϊ������
			if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
			{
				SetSmallCpnNoTexture(vAppendComponentInputItem);
			}

			//// ׷��֮ǰ���ж��Ƿ��������ظ�
			//if (CheckRepeatBarcode(m_vComponentInputItem, vAppendComponentInputItem) == FALSE)
			{
				// ׷��
				for(int i = 0; i < vAppendComponentInputItem.size(); i++)
					vAppendComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				ParamSettingDlg dlgOtherSetting;
				dlgOtherSetting.loadSettings();
				ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
				if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
				{
					ModifyReminderByDateAndOrderFormat(vAppendComponentInputItem);
				}

				m_vComponentInputItem.insert(m_vComponentInputItem.end(), vAppendComponentInputItem.begin(), vAppendComponentInputItem.end());
			}

			//// �ٴ��ж��ظ�����
			//CheckRepeatBarcode(m_vComponentInputItem);


			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);


			// �Զ������Ż�����
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);


		}

		// �л���ͼ
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// ���ý�����������
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem, false);

	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

}







void CCarvingView::OnOptionsPen()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//m_Tab.SetUIInfo(10, IDB_TAB1_N, IDB_TAB1_H);
	//m_Tab.SetUIInfo(0, IDB_TAB_UNSELECTED_NORMAL, IDB_TAB_UNSELECTED_HOVER, IDB_TAB_SELECTED_NORMAL, IDB_TAB_SELECTED_NORMAL);
	m_Tab.SetUIInfo(10, IDB_TAB1_N, IDB_TAB1_H, IDB_TAB1_N, IDB_TAB1_H);
	m_Tab.AddTab(_T("�Ű�����"), ID_VIEW_REMAINDER);
	m_Tab.AddTab(_T("���г�Ʒ"), ID_VIEW_COMPONENT);
	m_Tab.AddTab(_T("�Ż�����"), ID_VIEW_SOLUTION);
	//m_Tab.AddTab(L"c", 3);

	m_pDlgRemainder = new CDlgRemainder(this);
	m_pDlgRemainder->Create(CDlgRemainder::IDD, this);

	//vector<RemainderInputItem> vRemainderInputItem;
	//RemainderInputItem remainder = {100, 50, 18, _T("ľ"), 1};
	//vRemainderInputItem.push_back(remainder);
	//m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);

	SwitchView(VIEW_PANEL_LIST);

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);


	if (param_dlg->m_CheckRemainderLibrary == TRUE)
	{

		// ��ȡ���Ͽ�

		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_name = ExeFile;
		file_name = file_name.Left(file_name.ReverseFind('\\')+1) ;


		CString filename = file_name + "RemainderLibrary.hge";

		RemainderLibraryReadWrite::Read(filename.GetBuffer());
		vector<RemainderInputItem> vRemainderInputItem;

		vector<RemainderItem*> all_item_list;
		CSingleon::GetSingleton()->m_RemainderManager.GetAllItems(all_item_list);

		// ���ϸ�ֵ
		for(int i_rmd = 0; i_rmd < all_item_list.size(); i_rmd++)
		{
			RemainderItem* pRmd = all_item_list.at(i_rmd);
			RemainderInputItem rmd_item;

			rmd_item.m_nXLen		=	pRmd->m_Length		;
			rmd_item.m_nYLen		=	pRmd->m_Width		;
			rmd_item.m_strMaterial	=	pRmd->m_Material	;
			rmd_item.m_fThickness	=	pRmd->m_Thickness	;
			rmd_item.m_nCount		=	pRmd->m_nCount		;

			vRemainderInputItem.push_back(rmd_item);
		}

		m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);


	}



	// �ж��Ƿ���Ҫע�� �������Щ�����޷���������ʹ�ã�������ע�͵�



#if 0

	if (ClientInfoReadWrite::IsClientInfoFileExist() == FALSE)
	{
		// ��ʾע��ԭ��
		AfxMessageBox("δע�ᣡ");


		ClientRegisterDlg RegDlg;

		RegDlg.m_RegisterOrUpdate = ClientRegisterDlg::Dlg_Register;

		RegDlg.DoModal();
	}


	// ����Ƿ�ע��
	if (ClientInfoReadWrite::IsClientRegistered() == FALSE)
	{
		ClientRegisterDlg RegDlg;


		RegDlg.m_RegisterOrUpdate = ClientRegisterDlg::Dlg_Update;

		RegDlg.DoModal();
	}

	// �ճ���¼ͳ�� �����̷߳�ʽ��������������
	HANDLE hdthread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoginStatisticsDailyThread, NULL, NULL, NULL);
	if (hdthread == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("��¼ͳ���߳�����ʧ�ܣ�");
	}
	

#endif
}

/*---------------------------------------*/
//	����˵����
//		�ճ���½ͳ���̺߳���
//
//
//	������
//		PVOID lpThreadParameter		-- ����
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
DWORD CCarvingView::LoginStatisticsDailyThread( PVOID lpThreadParameter )
{
	try
	{
		InternetChecking::LoginStatisticsDaily();
	}
	catch(CException* e)
	{
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024);   //  e.GetErrorMessage(szError,1024); 
		::AfxMessageBox(szError); 
	}


	return 0;
}

void CCarvingView::OnViewRemainder()
{
	// TODO: �ڴ���������������
	SwitchView(VIEW_REMAINDER);
}

void CCarvingView::OnViewComponent()
{
	// TODO: �ڴ���������������
	SwitchView(VIEW_PANEL_LIST);
}


void CCarvingView::OnViewSolution()
{
	// TODO: �ڴ���������������
	SwitchView(VIEW_RESULT);
}

void CCarvingView::SwitchView(ViewType eViewType)
{
	//if(eViewType == m_eCurViewType)
	//	return;

	switch(eViewType)
	{
	case VIEW_REMAINDER:
		if(m_pDlgRemainder == NULL || m_pDlgRemainder->GetSafeHwnd() == NULL)
		{
			delete m_pDlgRemainder;
			m_pDlgRemainder = new CDlgRemainder(this);
			m_pDlgRemainder->Create(CDlgRemainder::IDD, this);
		}
		if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
			m_pDlgRequirement->ShowWindow(SW_HIDE);
		if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
			m_pDlgResult->ShowWindow(SW_HIDE);
		m_pDlgRemainder->MoveWindow(GetViewRect());
		m_pDlgRemainder->ShowWindow(SW_SHOW);
		m_eCurViewType = VIEW_REMAINDER;
		m_Tab.SetTab(VIEW_REMAINDER);
		break;

	case VIEW_PANEL_LIST:
		if(m_pDlgRequirement == NULL || m_pDlgRequirement->GetSafeHwnd() == NULL)
		{
			delete m_pDlgRequirement;
			m_pDlgRequirement = new CDlgRequirement(this);
			m_pDlgRequirement->Create(CDlgRequirement::IDD, this);

		}
		if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
			m_pDlgRemainder->ShowWindow(SW_HIDE);
		if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
			m_pDlgResult->ShowWindow(SW_HIDE);
		m_pDlgRequirement->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_pDlgRequirement->ShowWindow(SW_SHOW);
		m_eCurViewType = VIEW_PANEL_LIST;
		m_Tab.SetTab(VIEW_PANEL_LIST);
		break;

	case VIEW_RESULT:
		if(m_pDlgResult == NULL || m_pDlgResult->GetSafeHwnd() == NULL)
		{
			delete m_pDlgResult;
			m_pDlgResult = new CDlgResult(this);
			m_pDlgResult->Create(CDlgResult::IDD, this);

		}
		if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
			m_pDlgRemainder->ShowWindow(SW_HIDE);
		if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
			m_pDlgRequirement->ShowWindow(SW_HIDE);
		m_pDlgResult->ShowWindow(SW_SHOW);
		m_pDlgResult->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_eCurViewType = VIEW_RESULT;
		m_Tab.SetTab(VIEW_RESULT);
		break;
	}
}

CRect CCarvingView::GetViewRect()
{
	CRect rcRet;
	CRect rcClient, rcTab;
	GetClientRect(rcClient);
	m_Tab.GetWindowRect(rcTab);
	ScreenToClient(rcTab);
	rcRet.CopyRect(rcClient);
	rcRet.InflateRect(0, -rcTab.Height(), 0, 0);
	return rcRet;
}

void CCarvingView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
		m_pDlgRemainder->MoveWindow(GetViewRect());
	if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
		m_pDlgRequirement->MoveWindow(GetViewRect());
	if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
		m_pDlgResult->MoveWindow(GetViewRect());
}


void CCarvingView::OnOptimize()
{
	// TODO: �ڴ���������������



	// ��ȡ��������
	UpdateData(TRUE);

	// ������Ҫ�Ż��İ������ֱ�Ӵ�����Ҫ�Ż��İ��������ԭʼ�����⵽�ƻ�
	vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;

	// ���������
	CheckAndDeleteOverSizeComponentList(vOptimizeComponent);
	if(!CheckDeepHoleSlot(vOptimizeComponent))
		return;

	// �ж��Ƿ����Ż�����
	if (vOptimizeComponent.size() == 0)
	{
		AfxMessageBox("û�п��Ż��İ����");
		return;
	}

	if(IsRepeatBarcode(vOptimizeComponent))
	{
		AfxMessageBox("�����»��Ŷ��������ֶ��޸������š�");
		return;
	}
	
	// ��ȡ������Ϣ
	vector<RemainderInputItem> vRemainderInputItem = m_pDlgRemainder->GetRemainderInputItem();

	SwitchView(VIEW_RESULT);

	CSingleon* pSingleton = CSingleon::GetSingleton();

	// �����Ż������Ϣ

	//pSingleton->ClearAllData();
	//((CMainFrame*)theApp.GetMainWnd())->GetAlgBaseInfo(pSingleton->m_BaseInfo);


	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	COPSetDlg dlgOptimizeSetting;
	dlgOptimizeSetting.loadSettings();
	
	
	pSingleton->m_BaseInfo.m_PanelLength = param_dlg->m_width;				// ����
	pSingleton->m_BaseInfo.m_PanelWidth = param_dlg->m_height;				// ���
	pSingleton->m_BaseInfo.m_SawKerfWidth = param_dlg->m_Kerfwidth;			// ���
	pSingleton->m_BaseInfo.m_DeburringWidth = param_dlg->m_Deburringwidth;	// �ޱ�
	pSingleton->m_BaseInfo.m_LayoutOrg = param_dlg->m_arranging_origin;		// ����ԭ��

	// ��һ�׶β���
	pSingleton->m_BaseInfo.m_FirstSectionOPTimes = dlgOptimizeSetting.m_nEditStep1Count;
	pSingleton->m_BaseInfo.m_FirstSectionOPMethod = dlgOptimizeSetting.m_nComboStep1Alg;
	// �ڶ��׶β���
	pSingleton->m_BaseInfo.m_SecondSectionOPTimes = dlgOptimizeSetting.m_nEditStep2Count;
	// �����׶β���
	pSingleton->m_BaseInfo.m_ThirdSectionOPTimes = dlgOptimizeSetting.m_nEditStep3Count;
	pSingleton->m_BaseInfo.m_ThirdSectionOAccptableUtilization = dlgOptimizeSetting.m_fEditStep3AcceptableUti;

	// ������Ϣ��������
	pSingleton->m_BaseInfo.m_bDownerFaceFirst = dlgOptimizeSetting.m_valDownerInfoFirst;


	// ����ԭʼ�������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// �����������,׼���Ż�
	ClearAllData();

	// ����ʽ�����Ż����ڣ�������ʹ���߳��Ż�
	OptimizeDlg OpDlg;

	OpDlg.m_vComponentInputItem = vOptimizeComponent;
	OpDlg.m_vPreCombineItem = m_pDlgRequirement->GetPreCombineItems();
	OpDlg.m_vRemainderInputItem = vRemainderInputItem;

	OpDlg.DoModal();

	// ��ʱ�Ż�����ɣ������������Ҫ�����Ż��ʴӸߵ�������
	for (int i_sln = 0; i_sln < pSingleton->m_BackupSolutionList.size(); i_sln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

		pSln->SortPanelList();
	}
	


	// ���½������ID�Ͱ��ID
	pSingleton->UpdateSlnNum();

	//���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�
	pSingleton->UpdateComponentMachiningInfo();
	pSingleton->UpdatePreCombinedComponent();

	// �����µ���
	CParamDlg paramdlg;
	paramdlg.loadSettings();

	//С��������
	bool bCutTwice_Enable = paramdlg.LdoublecutEnable();
	float fCutTwice_SmallSize = paramdlg.Minarea();
	float fCutTwice_ReserveHeight = paramdlg.Keepthickness();
	float fCutTwice_SecondSpeed = paramdlg.Secondspeedrate();

	//��������
	float fTwoCutThreshold = paramdlg.Twocut_threshold();
	float fTwoCutReserve = paramdlg.Twocut_reserve();
	float fTwoCutSpeed = paramdlg.Twocut_workSpeed();


	KnifeClass* pKnife = pSingleton->m_pKnifeClass;
	for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
	{
		CSolution* pTheSolution = pSingleton->m_BackupSolutionList.at(i_sln);


		// ������Ϣ
		pKnife->setKerfwidth(pTheSolution->m_BaseInfo.m_SawKerfWidth);		//���
		pKnife->setPanelWidth(pTheSolution->m_BaseInfo.m_PanelLength);		//�����
		pKnife->setPanelHeight(pTheSolution->m_BaseInfo.m_PanelWidth);		//��峤��
		pKnife->setThickness(pTheSolution->m_fThickness);					//�����					
		pKnife->setDeburringwidth(paramdlg.Deburringwidth());				//�ޱ߾���				
		pKnife->SetInputOrderNum("a1");										//������

		string strThicknessAndMetrial;
		CString cstrThicknessAndMetrial = GetFloatString(pTheSolution->m_fThickness, 1) + "-" + pTheSolution->m_strMaterial;
		strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
		cstrThicknessAndMetrial.ReleaseBuffer();

		pKnife->setThinMatrial(strThicknessAndMetrial);						//����� + ����
		pKnife->setPanelMatrial(pTheSolution->m_strMaterial.GetBuffer());	//�������
		pTheSolution->m_strMaterial.ReleaseBuffer();


		//����G�����
		//kc->setGcodeParam(gcodeparam);		



		// �����в��� //С����С�ߴ� //Ԥ������� //�ڶ����ӹ�����
		pKnife->SetLeftParam(fCutTwice_SmallSize,fCutTwice_ReserveHeight,fCutTwice_SecondSpeed);

		// �����µ���
		//for(int i_panel = 0; i_panel < pTheSolution->GetPanelNum(); i_panel++)
		//{
		//	Panel* pThePanel = pTheSolution->GetPanel(i_panel);

			//pKnife->SetKnifeDownPos(pThePanel);
		//}
	}
	CalcKnifeDowPos();


	m_pDlgResult->ResetResultDlg();




	if (param_dlg->m_CheckRemainderLibrary == TRUE)
	{

		RemainderDlg rmd_dlg;

		if (rmd_dlg.DoModal() == IDOK)
		{
			vector<RemainderInputItem> vRemainderInputItem;

			vector<RemainderItem*> all_item_list;
			pSingleton->m_RemainderManager.GetAllItems(all_item_list);

			// ���ϸ�ֵ
			for(int i_rmd = 0; i_rmd < all_item_list.size(); i_rmd++)
			{
				RemainderItem* pRmd = all_item_list.at(i_rmd);
				RemainderInputItem rmd_item;

				rmd_item.m_nXLen		=	pRmd->m_Length		;
				rmd_item.m_nYLen		=	pRmd->m_Width		;
				rmd_item.m_strMaterial	=	pRmd->m_Material	;
				rmd_item.m_fThickness	=	pRmd->m_Thickness	;
				rmd_item.m_nCount		=	pRmd->m_nCount		;

				vRemainderInputItem.push_back(rmd_item);
			}

			m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);


			char ExeFile[2000];
			GetModuleFileName(NULL,ExeFile,500);
			CString file_name = ExeFile;
			file_name = file_name.Left(file_name.ReverseFind('\\')+1) ;


			CString filename = file_name + "RemainderLibrary.hge";
			RemainderLibraryReadWrite::Write(filename.GetBuffer());

		}
	}

}


void CCarvingView::OnSave()
{
#if 1

	// TODO: �ڴ���������������
	CString strFilePath;

#ifndef YiWei
	CString filter = "hgo �ļ�(*.hgo)|*.hgo|�����ļ� (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_strOpenedFile) + _T(".hgo");
	CFileDialog fileDlg (FALSE, _T("hgo"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
#else
	CString filter = "ywo �ļ�(*.ywo)|*.ywo|�����ļ� (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_strOpenedFile) + _T(".ywo");
	CFileDialog fileDlg (FALSE, _T("ywo"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
#endif

	CString strDefDir = SelectPathDlg().DefaultSavePath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	
	if ( fileDlg.DoModal() == IDOK)
	{
		strFilePath = fileDlg.GetPathName();
		
		if (HgyReadWrite::WriteHgy(strFilePath) == true)
		{
			AfxMessageBox("�ļ�����ɹ���");
		}
		else
		{
			AfxMessageBox("�ļ�����ʧ�ܣ�");
		}
	}

#else

	CString strFilePath;

	CString filter = "dxf �ļ�(*.dxf)|*.dxf|�����ļ� (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_strOpenedFile) + _T(".dxf");
	CFileDialog fileDlg (FALSE, _T("dxf"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultSavePath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;

	if ( fileDlg.DoModal() == IDOK)
	{
		strFilePath = fileDlg.GetPathName();

		if (DxfReadWrite::OutputDownerFaceDxf(strFilePath) == true)
		{
			AfxMessageBox("�ļ�����ɹ���");
		}
		else
		{
			AfxMessageBox("�ļ�����ʧ�ܣ�");
		}
	}


#endif




}


void CCarvingView::OnEditPragmaSawKerf()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditPanelLength()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditPanelWidth()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditStep1Count()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditStep2Count()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditStep3Count()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnEditStep3AcceptableUti()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnComboStep1Alg()
{
	// TODO: �ڴ���������������
}



// ����һ���ַ����е�����
void DelChineseInString(string& str)
{
	int i, nCount;

	nCount = str.size();
	for(int i = 0; i < nCount; i++)
	{
		char c = str[i];

		//����ȫ���ַ���
		if( c >= 0 && c <= 127 ) 
		{
			// Ӣ��
		}
		else 
		{
			//��ȫ���ַ� ����
			str.erase(str.begin()+i);
			str.erase(str.begin()+i);

			return DelChineseInString(str);
		}
	}

}

bool CCarvingView::PrepareExportGCode(PrepareExportGCodeData* pPrepareExportGCodeData)
{
	
	pPrepareExportGCodeData->_knife_shifting = pPrepareExportGCodeData->pParamDlg->Knife_shifting();

	//С��������
	pPrepareExportGCodeData->bCutTwice_Enable = pPrepareExportGCodeData->pParamDlg->LdoublecutEnable();
	pPrepareExportGCodeData->fCutTwice_SmallSize = pPrepareExportGCodeData->pParamDlg->Minarea();
	pPrepareExportGCodeData->fCutTwice_ReserveHeight = pPrepareExportGCodeData->pParamDlg->Keepthickness();
	pPrepareExportGCodeData->fCutTwice_SecondSpeed = pPrepareExportGCodeData->pParamDlg->Secondspeedrate();

	//��������
	pPrepareExportGCodeData->fTwoCutThreshold = pPrepareExportGCodeData->pParamDlg->Twocut_threshold();
	pPrepareExportGCodeData->fTwoCutReserve = pPrepareExportGCodeData->pParamDlg->Twocut_reserve();
	pPrepareExportGCodeData->fTwoCutSpeed = pPrepareExportGCodeData->pParamDlg->Twocut_workSpeed();

	//CSelMachineDlg selectMachine;
	if (pPrepareExportGCodeData->m_pSelMachineDlg->DoModal() == IDOK)
	{
		pPrepareExportGCodeData->node_name = pPrepareExportGCodeData->m_pSelMachineDlg->m_nodename;
		pPrepareExportGCodeData->node_print = atoi(pPrepareExportGCodeData->m_pSelMachineDlg->m_printname.c_str());
		pPrepareExportGCodeData->node_sysuse = pPrepareExportGCodeData->m_pSelMachineDlg->_coorsysuse;
		pPrepareExportGCodeData->node_double = pPrepareExportGCodeData->m_pSelMachineDlg->_cutdouble;
		pPrepareExportGCodeData->nodecoor_type = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype;//����λ����ϵ(Ĭ��)
		pPrepareExportGCodeData->nodecoor_type1 = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype;
		pPrepareExportGCodeData->nodecoor_type2 = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype2;//˫��λ����ϵ
		pPrepareExportGCodeData->node_groupstr = pPrepareExportGCodeData->m_pSelMachineDlg->_groupstr;

		pPrepareExportGCodeData->node_pname = pPrepareExportGCodeData->m_pSelMachineDlg->m_pname;
		pPrepareExportGCodeData->node_plast = pPrepareExportGCodeData->m_pSelMachineDlg->m_plast;
		pPrepareExportGCodeData->node_cname = pPrepareExportGCodeData->m_pSelMachineDlg->m_cname;
		pPrepareExportGCodeData->node_clast = pPrepareExportGCodeData->m_pSelMachineDlg->m_clast;
		pPrepareExportGCodeData->node_dvcut = pPrepareExportGCodeData->m_pSelMachineDlg->m_dvcut;//0,�����ף�1˫����1��W��,2˫����2��H��
		pPrepareExportGCodeData->node_pxhx = pPrepareExportGCodeData->m_pSelMachineDlg->m_pxhx;
		pPrepareExportGCodeData->node_cxhx = pPrepareExportGCodeData->m_pSelMachineDlg->m_cxhx;

		_trim1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim1;
		_trim2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim2;
		_trim3 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim3;
		_trim4 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim4;
		_ftrim1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim1;
		_ftrim2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim2;
		_ftrim3 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim3;
		_ftrim4 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim4;

		_iscombine = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscombine;							//	�Ƿ�ϲ�����ͷβ
		pPrepareExportGCodeData->_isComline = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscomline;							//	�Ƿ�����NC�ļ�
		pPrepareExportGCodeData->_isComball = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscomball;							//	�Ƿ�ϲ�����NC�ļ�
		pPrepareExportGCodeData->_isLabelInReverseSide = pPrepareExportGCodeData->m_pSelMachineDlg->m_bLabelInReverseSide;	//	�Ƿ��ǩ���ڷ���

		pPrepareExportGCodeData->_lessKnifeChange = pPrepareExportGCodeData->m_pSelMachineDlg->m_lessKnifeChange;

		CString theUpHeigt;//��ȡ��ȫ�߶�
		theUpHeigt.Format(_T("%0.1f"),pPrepareExportGCodeData->pParamDlg->Upheight());
		std::string theUPH = theUpHeigt.GetBuffer();

		CString theOverSpeed;//��ȡ�����ٶ�
		theOverSpeed.Format(_T("%0.1f"),pPrepareExportGCodeData->pParamDlg->Overspeed());
		std::string theFs = theOverSpeed.GetBuffer();

		_header = pPrepareExportGCodeData->m_pSelMachineDlg->m_header;
		//ChangeHeader(_header,theUPH,theFs);
		_ender = pPrepareExportGCodeData->m_pSelMachineDlg->m_ender;
		// ChangeEnder(_ender,theUPH,theFs);
		_GFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GFirst2;//��λ2�ļ�ͷ
		_GLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GLast2;//��λ2�ļ�β
		//_cutdouble = true˫��λ��_cutdouble = false����λ
		pPrepareExportGCodeData->_cutDouble = pPrepareExportGCodeData->m_pSelMachineDlg->_cutdouble;
		_MFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MFirst1;
		_MLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MLast1;
		_MFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MFirst2;
		_MLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MLast2;

		_PZFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZFirst1;
		_PZLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZLast1;
		_PZFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZFirst2;
		_PZLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZLast2;

		_GBFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBFirst1;//��λ1�����ļ�ͷ
		_GBFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBFirst2;//��λ2�����ļ�ͷ
		_GBLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBLast1;//��λ1�����ļ�β
		_GBLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBLast2;//��λ2�����ļ�β
	}
	else
	{
		return false;
	}

	/*std::string temp_header1,temp_ender1,temp_header2,temp_ender2,temp_GFirst2_1,temp_GLast2_1,temp_GFirst2_2,temp_GLast2_2,temp_GBFirst1_1,temp_GBLast1_1,
	temp_GBFirst1_2,temp_GBLast1_2,temp_GBFirst2_1,temp_GBLast2_1,temp_GBFirst2_2,temp_GBLast2_2;*/
	{
		pPrepareExportGCodeData->temp_header1 = _header;//������ʱ�ļ�ͷ1
		pPrepareExportGCodeData->temp_ender1 = _ender;//������ʱ�ļ�β1
		DeleteSpecialCode(pPrepareExportGCodeData->temp_header1);//ɾ������
		DeleteSpecialCode(pPrepareExportGCodeData->temp_ender1);
	
		
		pPrepareExportGCodeData->temp_header2 = _header;//������ʱ�ļ�ͷ2
		pPrepareExportGCodeData->temp_ender2 = _ender;//������ʱ�ļ�β2
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_header2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_ender2);
	    

		pPrepareExportGCodeData->temp_GFirst2_1 = _GFirst2;//���湤λ2��2����ʱͷβ
		pPrepareExportGCodeData->temp_GLast2_1 = _GLast2;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GFirst2_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GLast2_1);

		pPrepareExportGCodeData->temp_GFirst2_2 = _GFirst2;
		pPrepareExportGCodeData->temp_GLast2_2 = _GLast2;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GFirst2_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GLast2_2);

		pPrepareExportGCodeData->temp_GBFirst1_1 = _GBFirst1;//���湤λ1��2����ʱͷβ
		pPrepareExportGCodeData->temp_GBLast1_1 = _GBLast1;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBFirst1_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBLast1_1);

		pPrepareExportGCodeData->temp_GBFirst1_2 = _GBFirst1;
		pPrepareExportGCodeData->temp_GBLast1_2 = _GBLast1;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBFirst1_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBLast1_2);

		pPrepareExportGCodeData->temp_GBFirst2_1 = _GBFirst2;//���湤λ2��2����ʱͷβ
		pPrepareExportGCodeData->temp_GBLast2_1 = _GBLast2;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBFirst2_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBLast2_1);

		pPrepareExportGCodeData->temp_GBFirst2_2 = _GBFirst2;
		pPrepareExportGCodeData->temp_GBLast2_2 = _GBLast2;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBFirst2_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBLast2_2);
	}

	if (pPrepareExportGCodeData->nodecoor_type < 0)
	{
		pPrepareExportGCodeData->other_str = 1;
		pPrepareExportGCodeData->nodecoor_type = abs(pPrepareExportGCodeData->nodecoor_type);
	}

	if (pPrepareExportGCodeData->node_name == "")
	{
		return false;
	}

	//CString pathName;

	SelectPathDlg pathDlg;
	pathDlg.CurrentPath(pathDlg.DefaultSavePath());
	if (pathDlg.DoModal() == IDCANCEL)
		return false;

	pPrepareExportGCodeData->pathName = pathDlg.CurrentPath();
	//pathName = pathName+"\\nc";
	//HGTools::deletePath(pathName);
	//HGTools::createDir(pathName);

	NCSavePath = pPrepareExportGCodeData->pathName;
	CString pathCut = pPrepareExportGCodeData->pathName;
	pPrepareExportGCodeData->pathPrint = pPrepareExportGCodeData->pathName;

	//������Ӧ��Ŀ¼
	//��Ӧ�� NC �ļ��ƶ�����ǰĿ¼�� cutĿ¼ ��
	//��Ӧ�� TB���� �ļ��ƶ�����ǰĿ¼�� printĿ¼ ��
	if (pPrepareExportGCodeData->m_pSelMachineDlg->m_fileClassifyMove == "1")
	{
		//���ɵ�ʱ��Ͱ�ȫ���ļ��ŵ�cutĿ¼��
		//�����������ļ�������printĿ¼

		pathCut = NCSavePath + "\\cut";
		pPrepareExportGCodeData->pathName = pathCut;
		pPrepareExportGCodeData->pathPrint = NCSavePath + "\\print";

		if (!HGTools::isDir(pathCut))
		{
			CreateDirectory(pathCut,NULL);
		}

		if (!HGTools::isDir(pPrepareExportGCodeData->pathPrint))
		{
			CreateDirectory(pPrepareExportGCodeData->pathPrint,NULL);
		}
	}

	return true;
}

void CCarvingView::PrepareKnifeInfoAndHoleSetting(PrepareExportGCodeData* pPrepareExportGCodeData, PrepareKnifeInfoData* pPrepareKnifeInfoData)
{
	if (1)
	{
		std::vector<std::map<std::string,std::string>> knifes = pPrepareExportGCodeData->m_pSelMachineDlg->getMachineKnifes();

		KnifeType tempkt;//0,5,4�е�������ʵĵ�
		tempkt.kt = -1;
		int ktnum_kl = 0;

		//��ȡ��е������Ϣ
		for (int knife_i = 0;knife_i < knifes.size();knife_i++)
		{
			std::map<std::string,std::string> cur_knife = knifes[knife_i];


			//��ȡ����
			std::string str_Index			= cur_knife["Index"];
			std::string str_TypeName		= cur_knife["TypeName"];
			std::string str_Diameter		= cur_knife["Diameter"];
			std::string str_Length			= cur_knife["Length"];
			std::string str_ChangeCode		= cur_knife["ChangeCode"];
			std::string str_FeedRate		= cur_knife["FeedRate"];
			std::string str_SpindleSpeed	= cur_knife["SpindleSpeed"];
			std::string str_EndSpeed		= cur_knife["EndSpeed"];
			std::string str_OverSpeed		= cur_knife["OverSpeed"];
			std::string str_kcoor			= cur_knife["Kcoor"];
			std::string str_knum			= cur_knife["KNum"];
			std::string str_CoorSys			= cur_knife["CoorSys"];
			std::string str_KnifeLength		= cur_knife["KnifeLength"];
			std::string str_NearSpeed		= cur_knife["NearSpeed"];
			std::string str_ischeck			= cur_knife["Used"];//�жϵ����Ƿ�ʹ��

			if (pPrepareExportGCodeData->node_double == true && pPrepareExportGCodeData->key%2 == 0)
			{
				str_ChangeCode	= cur_knife["ChangeCode1"];
				str_CoorSys		= cur_knife["CoorSys1"];
			}

			//��֯��λ��ʼ��������
			std::string str_ChangeCodeB = "";
			std::string str_ChangeCodeE = "";
			bool IsBegin = true;
			for (int c_i = 0;c_i < str_ChangeCode.size();c_i++)
			{
				if (str_ChangeCode.at(c_i) == '#')
				{
					IsBegin = false;
					continue;
				}
				if (IsBegin == true)
				{
					str_ChangeCodeB = str_ChangeCodeB + str_ChangeCode.at(c_i);
				}
				else
				{
					str_ChangeCodeE = str_ChangeCodeE + str_ChangeCode.at(c_i);
				}
			}

			std::string str_kx = "";
			std::string str_ky = "";
			IsBegin = true;
			for (int c_i = 0;c_i < str_kcoor.size();c_i++)
			{
				if (str_kcoor.at(c_i) == ',')
				{
					IsBegin = false;
					continue;
				}
				if (IsBegin == true)
				{
					str_kx = str_kx + str_kcoor.at(c_i);
				}
				else
				{
					str_ky = str_ky + str_kcoor.at(c_i);
				}
			}
			if (str_kx == "")
			{
				str_kx = "0";
			}
			if (str_ky == "")
			{
				str_ky = "0";
			}

			KnifeType onekt;

			onekt._diameter		= atof(str_Diameter.c_str());
			onekt._feedrate		= atof(str_FeedRate.c_str());
			onekt._spindlespeed = atof(str_SpindleSpeed.c_str());
			onekt._tgcodeb		= str_ChangeCodeB;;
			onekt._tgcodee		= str_ChangeCodeE;
			onekt.kt			= atoi(str_TypeName.c_str());
			onekt._length		= atof(str_Length.c_str());
			onekt._endspeed		= atof(str_EndSpeed.c_str());
			onekt._overspend	= atof(str_OverSpeed.c_str());
			onekt._ky			= atof(str_kx.c_str());
			onekt._kx			= atof(str_ky.c_str());
			onekt._kn			= atoi(str_knum.c_str());
			onekt._coorsys		= str_CoorSys;
			onekt._kinfelength	= atof(str_KnifeLength.c_str());
			onekt._facespeed	= atof(str_NearSpeed.c_str());



			if ((	onekt.kt == KnifeType_Spindle_Hole_Slot_Material
				||	onekt.kt == KnifeType_Spindle_Material
				||	onekt.kt == KnifeType_Spindle_Slot_Material)
				&&(str_ischeck!="0"))//��Ҫ�жϵ��߳���
			{
				float thickness = /*kc->getThickness()*/pPrepareKnifeInfoData->pThePanel->m_Thickness;
				if (thickness>onekt._kinfelength)
				{
					if (ktnum_kl==0&&onekt._kinfelength==0)
					{
						tempkt = onekt;
						ktnum_kl++;
					}
				}
				else//���߳��ȴ��ڰ�����
				{
					if (ktnum_kl>0)
					{
						//KnifeType tempKt = tempkt;
						if (tempkt._kinfelength>onekt._kinfelength||tempkt._kinfelength==0)
						{
							tempkt = onekt;
						}
						//kt.push_back(onekt);
					}
					else
					{
						tempkt = onekt;
						ktnum_kl++;
					}
				}
			}
			else
			{
				if (str_ischeck!="0")
				{
					pPrepareKnifeInfoData->kt.push_back(onekt);
				}

			}
		}

		//yuanzb 2017-6-23  ���ϵ�һ���������Ķ���ֻ����һ�� 
		if (tempkt.kt != -1)
		{
			pPrepareKnifeInfoData->kt.push_back(tempkt);//�����ʵĿ��ϵ�����
		}

		pPrepareExportGCodeData->pKnifeClass->setKnifeType(pPrepareKnifeInfoData->kt);
	}

	vector<float> _cxy = pPrepareExportGCodeData->pParamDlg->GetVecFilter();
	pPrepareExportGCodeData->pKnifeClass->setvecchangexy(_cxy);


	float _changex = pPrepareExportGCodeData->pParamDlg->MoveX();
	float _changey = pPrepareExportGCodeData->pParamDlg->MoveY();

	pPrepareExportGCodeData->pKnifeClass->setvecchangexy(_changex,_changey);
}

void CCarvingView::AfterExportGCode_TransCoordinate(PrepareExportGCodeData* pPrepareExportGCodeData, TransCoordinateData* pTransCoordinateData)
{

	//�ٴδ�NC���� path ����
#if 1
	try
	{
		// ��һ�� 

		//L4Cpp::Log()->debug("6.coor change");
		/*******************************************����ײ�����**************************************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)//��׼ԭ������
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);

			//L4Cpp::Log()->debug("6.coor ofstream patho");
			//L4Cpp::Log()->debug(patho.c_str());

			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);

			//L4Cpp::Log()->debug("6.coor ifstream path");
			//L4Cpp::Log()->debug(path.c_str());

			try
			{
				while(!rfile.eof())
				{	
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//other_y��Ӧ����X
						{
							ischangex = false;
							float other_y;
							if (pTransCoordinateData->HaveDvs)//���ޱߣ�����ϵ0ʱ���X
							{
								other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;//X��ƫ��
							}
							else
								other_y =atof(tmp_stry.c_str());


							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							if (pTransCoordinateData->HaveDvs)//���ޱߣ�����ϵ0ʱ���Y
							{
								other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;//Y��ƫ��
							}
							else
								other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
			}
			catch (...)
			{
				//L4Cpp::Log()->debug("6.coor catch while(!rfile.eof())");
			}

			wfile<<endl;

			//L4Cpp::Log()->debug("6.coor wfile<<endl; close");

			wfile.close();
			rfile.close();

			//L4Cpp::Log()->debug("6.coor DeleteFile");

			DeleteFile(pTransCoordinateData->nc_font_path.c_str());

			//L4Cpp::Log()->debug("6.coor rename");

			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());


			//L4Cpp::Log()->debug("6.coor eachLineAddChar");
			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(0,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)	//����ԭ�������½ǣ���������ԭ�������Ͻ�Ϊ׼
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}					
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
					{
						ischangex = false;
						float other_y;



						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
						}

						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�//y
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}				
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(1,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)//����ԭ�������Ͻǣ�����Ϊ��Y������Ϊ��X��
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//�з����ޱ�//X
						{
							other_y = -1.0 *(atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX);
						}
						else
							other_y = -1.0 * atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}		
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(2,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//����ԭ�������½�
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}	
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//���ޱ�,����ϵ3ʱ���X
						{
							other_y = atof(tmp_stry.c_str())+pTransCoordinateData->_usedLineX;
							other_y = other_y - pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						}
						else
							other_y =atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}	
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(3,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
					{
						ischangex = false;
						float other_y;//������ʱ���൱��X���������Ļ��൱��Y
						if (pTransCoordinateData->HaveDvs)//temp_stryΪX1,temp_strxΪY1��
						{
							other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
							other_y =atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱߣ�����ϵ0ʱ���Y
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())+pTransCoordinateData->_usedLineY;//Y��ƫ��
						}
						else
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (1)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}	
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(0,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)	
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}					
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//���ޱ�X
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight()-atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;

						}
						else
							other_y =  pPrepareExportGCodeData->pKnifeClass->getPanelHeight()-atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//y
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}				
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(1,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//�з����ޱ�
						{
							other_y = -atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
							other_y = -atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_x = -1.0 *(pPrepareExportGCodeData->pKnifeClass->getPanelWidth()-atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY);

						}
						else
							other_x = -1.0 *(pPrepareExportGCodeData->pKnifeClass->getPanelWidth()-atof(tmp_strx.c_str()));
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}		
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);

			//MovePositonInDifCordinate(2,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//����ԭ�������½�
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_header;//�¼�
			//}	
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//���ޱ�,����ϵ3ʱ���X
						{
							other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
							other_y = other_y - pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						}
						else
							other_y =atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//���ޱ�
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x =  -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
			//{
			//	wfile<<_ender;//�¼�
			//}	
			wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//����ÿ���������
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);

			//MovePositonInDifCordinate(3,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}


		/*******************************************����ײ�����**************************************************/

		//L4Cpp::Log()->debug("6.coor change @W");
		//�ٴδ�NC���� pathd @W
		/*******************************************����ײ����ɣ�W��ת��ʽ���ҷ�ת��******************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);		
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�//д�뷴���ļ�ͷ
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						char tmp_char = get_str.at(g);

						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)//����ף�����ԭ�������½ǣ�w��ת
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}					
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}			
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)//����ף�����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;//ת�����Y
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}	
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(3,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);		
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�//д�뷴���ļ�ͷ
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)//����ף�����ԭ�������½ǣ�w��ת
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}					
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}			
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)//����ף�����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;//ת�����Y
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}	
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(3,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}


		/*******************************************����ײ����ɣ�W��ת��ʽ���ҷ�ת��******************************/

		//L4Cpp::Log()->debug("6.coor change @H");
		//�ٴδ�NC���� pathd1 @H
		/*******************************************����ײ����ɣ�H��ת��ʽ���·�ת��******************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)//h��ת
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}		
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}	
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}

			//MovePositonInDifCordinate(3,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ1
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ2
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)//h��ת
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}		
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}	
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//����ԭ�������½�
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�ͷд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBFirst1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBFirst1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBFirst2<<std::endl;//�¼�
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//ת�����Y = height - x0(��׼�µ�x����);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//������ϲ�ʱ�����ļ�βд��
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_GBLast1<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GBLast1<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_GBLast2<<std::endl;//�¼�
						}
					}
				}
				wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//����ÿ���������
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}

			//MovePositonInDifCordinate(3,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}

		/*******************************************����ײ����ɣ�H��ת��ʽ���·�ת��******************************/
	}
	catch (...)
	{
		MessageBox("NC����ϵƫ�ƴ���");
		return;
	}

#endif
}

void CCarvingView::AfterExportGCode_CombineFile(PrepareExportGCodeData* pPrepareExportGCodeData, CombineFileData* pCombineFileData)
{
	try
	{

		//����˫��λ����NC�ļ��ϲ�
		std::string tempend = "";
		std::string tempend2 = "";
		if (pCombineFileData->str_nextthin == "")
		{
			pCombineFileData->tempnextcname2 = "";
		}
		if (pPrepareExportGCodeData->_isComline== 1)//��Ҫ���ӵ�ʱ��
		{
			tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
			tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//��λ2�ļ�β��*<>����
		}
		else//�����ӵ�ʱ��
		{
			if (pCombineFileData->str_nextthin==pCombineFileData->str_thin)//�������Ҳ������һ��
			{
				tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
				tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//��λ2�ļ�β��*<>����
			}
			else
			{
				if(pCombineFileData->tempnextcname2=="")
				{
					tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
					tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//��λ2�ļ�β��*<>����
				}
				else
				{
					tempend = _ender;
					tempend2 = _GLast2;
				}		
			}
		}

		//   tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
		//tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//��λ2�ļ�β��*<>����
		{//@Wʱ
			if (pPrepareExportGCodeData->node_dvcut == "0")//�����׽��ļ�ͷβ�����ļ�
			{
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//�����ļ�
				if (rfile)
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_header<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GFirst2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_header<<std::endl;//�¼�
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);
						wfile<<str<<std::endl;
					}
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<tempend<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<tempend2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<tempend<<std::endl;//�¼�
						}
					}
					wfile<<endl;
					rfile.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}	
			}
			if (pPrepareExportGCodeData->node_dvcut=="1")//˫����1��w��
			{
				DeleteFile(pCombineFileData->nc_back_h_path.c_str());//ɾ��@H
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//�����ļ�
				std::ifstream rfile2(pCombineFileData->nc_back_w_path.c_str(),std::ios::in);//�����ļ�
				if (rfile)
				{

					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_header<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GFirst2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_header<<std::endl;//�¼�
						}
					}
					if (rfile2&&_iscombine==1)
					{
						while(!rfile2.eof())
						{
							char str[1023];
							rfile2.getline(str,1023);
							wfile<<str<<std::endl;
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);
						wfile<<str<<std::endl;
					}

					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<tempend<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<tempend2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<tempend<<std::endl;//�¼�
						}
					}

					wfile<<endl;
					rfile.close();
					rfile2.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());
					if (_iscombine==1)//ɾ�������ļ�
					{
						DeleteFile(pCombineFileData->nc_back_w_path.c_str());
					}	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}						
			}
		}
		{//@Hʱ
			if (pPrepareExportGCodeData->node_dvcut=="2")//˫����2��w��
			{
				DeleteFile(pCombineFileData->nc_back_w_path.c_str());//ɾ��@W
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//�����ļ�
				std::ifstream rfile2(pCombineFileData->nc_back_h_path.c_str(),std::ios::in);//�����ļ�
				if (rfile)
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<_header<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<_GFirst2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<_header<<std::endl;//�¼�
						}
					}
					if (rfile2&&_iscombine==1)
					{
						while(!rfile2.eof())
						{
							char str[1023];
							rfile2.getline(str,1023);
							wfile<<str<<std::endl;
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);
						wfile<<str<<std::endl;
					}
					if (pPrepareExportGCodeData->_cutDouble == false)//����λ
					{
						wfile<<tempend<<std::endl;//�¼�
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//��λ2
						{
							wfile<<tempend2<<std::endl;//�¼�
						}
						else//��λ1
						{
							wfile<<tempend<<std::endl;//�¼�
						}
					}
					wfile<<endl;
					rfile.close();
					rfile2.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());
					if (_iscombine==1)//ɾ�������ļ�
					{
						DeleteFile(pCombineFileData->nc_back_h_path.c_str());
					}	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}	
			}
		}

		//�ϲ�����NC�ļ���һ��
		if (pPrepareExportGCodeData->_isComball==1)
		{


			std::ofstream wfile(pCombineFileData->allpath2.c_str(),std::ios::out | std::ios::app );
			std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//�����ļ�
			if (rfile)
			{
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					wfile<<str<<std::endl;
				}
				wfile<<endl;
				rfile.close();
				wfile.close();
			}
		}	

	}
	catch (...)
	{
		MessageBox("�ļ��ϲ� ����");
		return;
	}
}

void CCarvingView::OnExportGcode()
{
	// TODO: �ڴ���������������

	// ����ͻ���Ϣ
	ClientInfoReadWrite::m_Info.m_OutputNCTimes++ ;
	ClientInfoReadWrite::WriteClientInfo();

	vector<SyntecDataItem> vSyntecData;
	vector<XiaoFengDataItem> vXiaoFengData;
	std::string stringActualLabelNCFilePath;
	


	KnifeClass& knifeClass = *(CSingleon::GetSingleton()->m_pKnifeClass);


	// ��ʾ��Ϣ��ʼ����ֻ��ʾһ���
	knifeClass.EnaleMsgReport();

	//������Ŀ�������NC����ź�
	knifeClass.ResetOutputTBNum();	

	CSelMachineDlg selectMachine;
	CParamDlg paramdlg;
	paramdlg.loadSettings();

	PrepareExportGCodeData prepareExportGCodeData;
	prepareExportGCodeData.m_pSelMachineDlg = &selectMachine;
	prepareExportGCodeData.pParamDlg = &paramdlg;
	prepareExportGCodeData.pKnifeClass = &knifeClass;

	bool bPrepareResult = PrepareExportGCode(&prepareExportGCodeData);
	if(!bPrepareResult)
		return;

	// ��ȡС��λ�������ע��
	ParamSettingDlg settings;
	settings.loadSettings();
	int DecimalPlace = settings.m_valDecimalPlace;
	BOOL PanelCommnets = settings.m_chkboxPanelComments;



	vector<Panel*> total_check_panel = m_pDlgResult->GetPanels(true);
	int total_check_panel_num = total_check_panel.size();
	vector<PanelAndTheSolution> vPanelAndTheSolution;
	GetSolutionPanel(vPanelAndTheSolution);
	if (vPanelAndTheSolution.size() == 0)
	{
		AfxMessageBox(_T("��δ���Ż�����"));
		return;
	}


		CDlgLabelPosCalc dlgLabelPosCalc(selectMachine.m_nodename.c_str());
		dlgLabelPosCalc.DoModal();

		/////////////////////////////////////////////////////////////////////////////////
		////���ÿ�����Ϣ������������
		vector<P_Info> _printinfo;
		for(int i = 0; i < m_vComponentInputItem.size(); i++)
		{
			P_Info temp;
			temp._info = m_vComponentInputItem[i].m_strBarcode;
			temp._content = getPrintInfoAsString(m_vComponentInputItem[i]);
			_printinfo.push_back(temp);
		}





			// ����knifeClass��Ϣ
			for(int i = 0; i < vPanelAndTheSolution.size(); i++)
			{
				Panel* pThePanel =vPanelAndTheSolution[i].m_pPanel;
				CSolution* pTheSolution = vPanelAndTheSolution[i].m_pSolution;

				// ������Ϣ����
				knifeClass.setKerfwidth(pTheSolution->m_BaseInfo.m_SawKerfWidth);
				knifeClass.setPanelWidth(pTheSolution->m_BaseInfo.m_PanelLength);
				knifeClass.setPanelHeight(pTheSolution->m_BaseInfo.m_PanelWidth);
				knifeClass.setThickness(pTheSolution->m_fThickness);
				knifeClass.setDeburringwidth(paramdlg.Deburringwidth());

				string strThicknessAndMetrial;
				CString cstrThicknessAndMetrial = GetFloatString(pTheSolution->m_fThickness, 1) + "-" + pTheSolution->m_strMaterial;
				strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
				cstrThicknessAndMetrial.ReleaseBuffer();

				knifeClass.setThinMatrial(strThicknessAndMetrial);
				knifeClass.setPanelMatrial(pTheSolution->m_strMaterial.GetBuffer());
				pTheSolution->m_strMaterial.ReleaseBuffer();
				knifeClass.SetInputOrderNum("a1");
				knifeClass.SetLeftParam(prepareExportGCodeData.fCutTwice_SmallSize,//С����С�ߴ�
					prepareExportGCodeData.fCutTwice_ReserveHeight,//Ԥ�������
					prepareExportGCodeData.fCutTwice_SecondSpeed);//�ڶ����ӹ�����

				// ����gcodeparam����Ϣ
				knifeClass.m_gcodeparam.upheight = paramdlg.Upheight();
				knifeClass.m_gcodeparam.downheight = paramdlg.Downheight();
				knifeClass.m_gcodeparam.distance1 = paramdlg.Distance1();
				knifeClass.m_gcodeparam.distance2 = paramdlg.Distance2();
				knifeClass.m_gcodeparam.org_offset = paramdlg.KnifeOrgOffset();

// 
// 				GcodeParam gcodeparam;
// 				gcodeparam.cutspeech = paramdlg.Cutspeed();
// 				gcodeparam.prospeech = paramdlg.Prospeed();
// 				gcodeparam.distance1 = paramdlg.Distance1();
// 				gcodeparam.distance2 = paramdlg.Distance2();
// 				gcodeparam.ender = "";
// 				gcodeparam.header = "";
// 				gcodeparam.upheight = paramdlg.Upheight();
// 				gcodeparam.endspeech = paramdlg.Endspeech();
// 				gcodeparam.strRinget = _strRinget;
// 				gcodeparam.downheight = paramdlg.Downheight();








				//�������Ƿ�ѡ�У������ѡ�У�������NC
				//if (/*m_picturedlg->m_piclist.m_panelChecked[panel_index] == false*/0)//////////////////////////////////////////////////
				if(!m_pDlgResult->IsPanelChecked(pThePanel))
				{
					continue;
				}

				if (prepareExportGCodeData._cutDouble==false)//����λʱ
				{
					prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;//����λ����ϵ
				}
				else//˫��λʱ��
				{
					if (prepareExportGCodeData.key%2!=0)//��λ1
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;
					}
					else//��λ2
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type2;
					}
				}

				/************************************************************************/
				/* ����indexλ�ã���ȡ�����Ϣ                                          */
				/************************************************************************/
				//��ȡ��ǰ���
				//Panel_Knife pk = m_picturedlg->_picturecoor[panel_index];
				//PanelNode * node = pk._panel;
				//KnifeClass * kc = pk._kc;

				////��ȡ��һ����
				//int next_panel_index = panel_index;	//��ʼֵ
				//do 
				//{
				//	next_panel_index++;
				//}while((next_panel_index < m_picturedlg->_picturecoor.size())
				//	&&(m_picturedlg->m_piclist.m_panelChecked[next_panel_index] == false));

				////�¿����Ƿ����
				//bool isHaveNextPanelUsable = (next_panel_index < m_picturedlg->_picturecoor.size());
				bool isHaveNextPanelUsable = (i < (vPanelAndTheSolution.size() - 1)) ? true : false;

				//////////////////////////////////////////////////////////////////////////
				//�ļ�ͷβȡֵ
				//������¸��ļ�������ʽ
				if (isHaveNextPanelUsable)
				{
					_header = prepareExportGCodeData.temp_header1;
					_ender = prepareExportGCodeData.temp_ender1;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_1;//��λ2�ļ�ͷ
					_GLast2 = prepareExportGCodeData.temp_GLast2_1;//��λ2�ļ�β

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_1;//��λ1�����ļ�ͷ
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_1;//��λ2�����ļ�ͷ
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_1;//��λ1�����ļ�β
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_1;//��λ2�����ļ�β
				}
				else
				{
					_header = prepareExportGCodeData.temp_header2;
					_ender = prepareExportGCodeData.temp_ender2;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_2;//��λ2�ļ�ͷ
					_GLast2 = prepareExportGCodeData.temp_GLast2_2;//��λ2�ļ�β

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_2;//��λ1�����ļ�ͷ
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_2;//��λ2�����ļ�ͷ
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_2;//��λ1�����ļ�β
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_2;//��λ2�����ļ�β
				}


				//2018-5-3 yuanzb ȥ��\r\n�е�\r�������Ļ���,��Щ�����޷���ȡ����
#if 1
				
				if (_header.find("\r\n") != std::string::npos)
				{
					_header.erase(std::remove(_header.begin(), _header.end(), '\r'), _header.end());
				}

				if (_ender.find("\r\n") != std::string::npos)
				{
					_ender.erase(std::remove(_ender.begin(), _ender.end(), '\r'), _ender.end());
				}

				if (_GFirst2.find("\r\n") != std::string::npos)
				{
					_GFirst2.erase(std::remove(_GFirst2.begin(), _GFirst2.end(), '\r'), _GFirst2.end());
				}

				if (_GLast2.find("\r\n") != std::string::npos)
				{
					_GLast2.erase(std::remove(_GLast2.begin(), _GLast2.end(), '\r'), _GLast2.end());
				}
				
				if (_GBFirst1.find("\r\n") != std::string::npos)
				{
					_GBFirst1.erase(std::remove(_GBFirst1.begin(), _GBFirst1.end(), '\r'), _GBFirst1.end());
				}

				if (_GBFirst2.find("\r\n") != std::string::npos)
				{
					_GBFirst2.erase(std::remove(_GBFirst2.begin(), _GBFirst2.end(), '\r'), _GBFirst2.end());
				}
					
				if (_GBLast1.find("\r\n") != std::string::npos)
				{
					_GBLast1.erase(std::remove(_GBLast1.begin(), _GBLast1.end(), '\r'), _GBLast1.end());
				}
					
				if (_GBLast2.find("\r\n") != std::string::npos)
				{
					_GBLast2.erase(std::remove(_GBLast2.begin(), _GBLast2.end(), '\r'), _GBLast2.end());
				}

					
					
					 
					
#endif

				 



				//�����ļ���
#if 1

				std::string tempnode_pname = "";
				std::string tempnode_plast = "";
				std::string tempnode_cname = "";
				std::string tempnode_clast = "";

				//����¿�����Ϣ
				std::string str_thinmatrial = /*kc->getPanelMatrial()*/pThePanel->m_Material.GetBuffer();
				pThePanel->m_Material.ReleaseBuffer();
				CString s_thinmatrial,s_thin,s_nextthin;
				std::string str_nextthin = "";//��һ�����İ��
				std::string str_nextthinmatrial = "";//��һ���Ĳ���
				s_thinmatrial.Format("%s",str_thinmatrial.c_str());
				s_thin.Format("%0.0f",/*kc->getThickness()*/pThePanel->m_Thickness);
				s_thinmatrial.Replace("\/","");
				s_thinmatrial.Replace("\\","");
				str_thinmatrial = s_thinmatrial.GetBuffer();
				std::string str_thin = s_thin.GetBuffer();

				//����ѡ����һ����������ȡ�����Ϣ
				if (isHaveNextPanelUsable)
				{
					//KnifeClass * nextkc = m_picturedlg->_picturecoor[next_panel_index]._kc;
					Panel* pNextPanel = vPanelAndTheSolution[i+1].m_pPanel;
					s_nextthin.Format("%0.1f",/*nextkc->getThickness()*/pNextPanel->m_Thickness);
					str_nextthin = s_nextthin.GetBuffer();
					str_nextthinmatrial = /*nextkc->getPanelMatrial()*/pNextPanel->m_Material.GetBuffer();
					pNextPanel->m_Material.ReleaseBuffer();
				}

				CString s_nextmaterial;
				s_nextmaterial.Format("%s",str_nextthinmatrial.c_str());
				s_nextthin.Format("%s",str_nextthin.c_str());
				s_nextmaterial.Replace("\/","");
				s_nextmaterial.Replace("\\","");
				str_nextthinmatrial = s_nextmaterial.GetBuffer();
				std::string filename,filenamed,filenamed1,allfile,pfilename,filenameo,pfilenameo,filenamedo,filenamed1o;
				char ch_filename[1000],ch_filenamed[1000],ch_filenamed1[1000], ch_allfile[1000],ch_pfilename[1000],ch_filenameo[1000],ch_pfilenameo[1000],ch_filenamedo[1000],ch_filenamed1o[1000];
				char ch_allFileName[1000];
				char theKey[1000],theKey2[1000];//theKey2��ʾ��һ��������ˮ��

				//��ˮ��
				if (/*panel_index*/i<10)
				{
					sprintf_s(theKey,"0%d",/*panel_index*/i + 1);
				}
				else
				{
					sprintf_s(theKey,"%d",/*panel_index*/i + 1);
				}

				if (/*next_panel_index*/i+1 <10)
				{
					sprintf_s(theKey2,"0%d",/*next_panel_index*/(i+1) + 1);
				}
				else
				{
					sprintf_s(theKey2,"%d",/*next_panel_index*/(i+1) + 1);
				}



				std::string tempcname2;//�ļ���+��ˮ��+�������+������cname
				std::string temppname2;//�ļ���+��ˮ��+�������+������pname
				std::string tempnextcname2;//�ļ���+��ˮ��+�������+������cname
				std::string tempnextpname2;//�ļ���+��ˮ��+�������+������pname
				std::string theAllCname;
				theAllCname = prepareExportGCodeData.node_cname;

				//����nc������ļ���
				{
					/*sprintf_s(ch_allfile,"\\All-%s-%s.nc",node_fname.c_str(),str_thin.c_str());
					sprintf_s(ch_filename,"\\C_%s%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilename,"\\P_%s%d.nc",node_fname.c_str(),key);
					sprintf_s(ch_filenameo,"\\C_%s-%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilenameo,"\\P_%s-%d.nc",node_fname.c_str(),key);*/

					if (prepareExportGCodeData.node_cname.find("@")!=std::string::npos)//nc�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = prepareExportGCodeData.node_cname.find_first_of('@');
						std::string theCname = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc�ļ���
						std::string theCname2 = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc�ļ���
						theAllCname = theCname2;
						std::string theCMaterial = str_thinmatrial;//����
						std::string theCThickness = str_thin;//���
						std::string thenextCMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextCThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextCThickness = HGTools::Convert2CString(thenextCThickness);
						cstr_thenextCThickness.Replace(".0","");
						thenextCThickness = HGTools::Convert2StdString(cstr_thenextCThickness);

						if (prepareExportGCodeData.node_cname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCMaterial;
							tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey + prepareExportGCodeData.node_cxhx+theCMaterial; 
							tempnextcname2 = theCname2+ prepareExportGCodeData.node_cxhx + theKey2+ prepareExportGCodeData.node_cxhx + thenextCMaterial;
						}
						if (prepareExportGCodeData.node_cname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCThickness + prepareExportGCodeData.node_cxhx;
							if (tempcname2=="")
							{
								tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey+ prepareExportGCodeData.node_cxhx + theCThickness;
							}
							else
								tempcname2 = tempcname2+ prepareExportGCodeData.node_cxhx + theCThickness;
							if (tempnextcname2=="")
							{
								tempnextcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey2 + prepareExportGCodeData.node_cxhx+ thenextCThickness;
							}
							else
								tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_cxhx + thenextCThickness;
						}
						tempnode_cname = theCname+ prepareExportGCodeData.node_cxhx + theKey;
					}
					else
					{
						tempnode_cname = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+theKey;
						tempcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey; 
						tempnextcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey2;
					}

					if (prepareExportGCodeData.node_pname.find("@")!=std::string::npos)//p�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = prepareExportGCodeData.node_pname.find_first_of('@');
						std::string thePname = prepareExportGCodeData.node_pname.substr(0,thePos1);//p�ļ���
						std::string thePname2 = prepareExportGCodeData.node_pname.substr(0,thePos1);//p�ļ���
						std::string thePMaterial = str_thinmatrial;//����
						std::string thePThickness = str_thin;//���
						std::string thenextPMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextPThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextPThickness = HGTools::Convert2CString(thenextPThickness);
						cstr_thenextPThickness.Replace(".0","");
						thenextPThickness = HGTools::Convert2StdString(cstr_thenextPThickness);


						if (prepareExportGCodeData.node_pname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePMaterial;
							temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx+thePMaterial; 
							tempnextpname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPMaterial;
						}
						if (prepareExportGCodeData.node_pname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePThickness + prepareExportGCodeData.node_pxhx;
							if (temppname2=="")
							{
								temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx + thePThickness;
							}else
								temppname2 = temppname2 + prepareExportGCodeData.node_pxhx + thePThickness;
							if (tempnextpname2=="")
							{
								tempnextpname2 = thePname2+ prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPThickness;
							}else
								tempnextpname2 = tempnextpname2 + prepareExportGCodeData.node_pxhx+ thenextPThickness;
						}
						tempnode_pname = thePname+ prepareExportGCodeData.node_pxhx + theKey;
					}
					else
					{
						tempnode_pname = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx +theKey;
						temppname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey; 
						tempnextpname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey2;
					}
					//sprintf_s(ch_allfile,"\\All-%s.nc",tempcname2.c_str());
					sprintf_s(ch_filename,"\\%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilename,"\\%s%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());

					sprintf_s(ch_filenameo,"\\%s_%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilenameo,"\\%s_%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				}

				theAllCname = theAllCname + prepareExportGCodeData.node_cxhx + str_thinmatrial + prepareExportGCodeData.node_cxhx + str_thin;//�ϲ�ͬһ���ͺ���ļ���

				sprintf_s(ch_allfile,"\\All%s_.nc",theAllCname.c_str());
				sprintf_s(ch_allFileName,"\\All%s.nc",theAllCname.c_str());
				/*tempnextpname2 = tempnextpname2 + node_plast;
				tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_clast;*/
				tempnextpname2 = tempnextpname2;
				tempnextcname2 = tempnextcname2;

				std::string thCnameW = tempcname2+"@W";
				std::string thCnameH = tempcname2+"@H";
				std::string thCname_W = tempcname2+"@-W";
				std::string thCname_H = tempcname2+"@-H";
				/*sprintf_s(ch_filenamed,"\\HG_%d-%s@W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1,"\\HG_%d-%s@H.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamedo,"\\HG_%d-%s@-W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1o,"\\HG_%d-%s@-H.nc",key,str_thin.c_str());*/
				sprintf_s(ch_filenamed,"\\%s%s",thCnameW.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1,"\\%s%s",thCnameH.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamedo,"\\%s%s",thCname_W.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1o,"\\%s%s",thCname_H.c_str(),prepareExportGCodeData.node_clast.c_str());

				filename = ch_filename;//nc
				pfilename = ch_pfilename;//p
				filenamed = ch_filenamed;//@W
				filenamed1 = ch_filenamed1;//@H
				allfile = ch_allfile;
				filenameo = ch_filenameo;//-nc
				pfilenameo = ch_pfilenameo;//-p
				filenamedo = ch_filenamedo;//@-W
				filenamed1o = ch_filenamed1o;//@-H

				std::string print_str1,next_tb_font_path,next_nc_font_path;
				char ch_print_str1[1000],ch_print_str2[1000],ch_print_str3[1000];
				sprintf_s(ch_print_str1,"%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				

				//������һ���
				sprintf_s(ch_print_str3,"%s%s",tempnextcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_print_str2,"%s%s",tempnextpname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				/*sprintf_s(ch_print_str1,"C_HG%d-%s.nc",key,str_thin.c_str());
				sprintf_s(ch_print_str3,"C_HG%d-%s.nc",key+1,str_nextthin.c_str());
				sprintf_s(ch_print_str2,"P_HG%d.nc",key+1);*/
				print_str1 = ch_print_str1;


				if (!isHaveNextPanelUsable)
				{
					next_tb_font_path = "";
					next_nc_font_path = "";
				}
				else
				{
					next_tb_font_path = ch_print_str2;
					next_nc_font_path = ch_print_str3;
				}



				std::string prev_tempFilename		=	ch_allFileName;
#if 1

				// 2016-11-11	yzb		����HM16110101���ǻԵĻ����޷�ʶ�����ģ��������е�Ӣ���豣��
				// 2017-5-56	yuanzb	�´���֧������
				if (prepareExportGCodeData.node_name.find("�ǻ�") != string::npos ||
					prepareExportGCodeData.node_print == 14)
				{
					DelChineseInString(filename	);
					DelChineseInString(pfilename);
					DelChineseInString(pfilename);
					DelChineseInString(filenameo);

					DelChineseInString(pfilenameo);
					DelChineseInString(filenamed	);
					DelChineseInString(filenamed1);
					DelChineseInString(allfile	);

					DelChineseInString(filenamedo	 );
					DelChineseInString(filenamed1o	 );
					DelChineseInString(prev_tempFilename);

					DelChineseInString(next_nc_font_path);
					DelChineseInString(next_tb_font_path);

				}

#endif



				//׼��������������ļ���
				std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ filename		;//nc			//nc�ļ�
				std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//�Զ�����nc�ļ�
				std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//�Զ�����nc�ļ�
				std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenameo		;//-nc			//nc��ʱ�ļ���

				std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//�Զ�������ʱ�ļ�
				std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed		;//@W			//W��תnc�ļ�
				std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1	;//@H			//H��תnc�ļ�
				std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ allfile		;

				std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamedo	;//@-W			//W��תnc��ʱ�ļ�
				std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1o	;//@-H			//H��תnc��ʱ�ļ�
				std::string tempFilename		=							  prev_tempFilename							;
				std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ tempFilename	;				//�ϲ�NC�ļ�

				next_nc_font_path;																			//��һ����nc�ļ���
				next_tb_font_path;																			//��һ����tb�ļ���


#endif


				//��ȡ������Ϣ
#if 1
				PrepareKnifeInfoData prepareKnifeInfoData;
				prepareKnifeInfoData.pThePanel = pThePanel;
				//prepareKnifeInfoData.pKnifeClass = &knifeClass;
				//��ȡ������Ϣ
				//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
				PrepareKnifeInfoAndHoleSetting(&prepareExportGCodeData, &prepareKnifeInfoData);
#endif

				//��λƫ�Ƽ���
#if 1
				bool HaveDvsh = false,HaveDvsw = false,HaveDvs = false;
				std::vector<float> v_trim;
				std::vector<float> v_trimh;
				std::vector<float> v_trimw;

				int _usedLineX = 0;
				int _usedLineY = 0;

				if (prepareExportGCodeData._cutDouble==false)//����λʱ
				{
					v_trim.push_back(_trim1);
					v_trim.push_back(_trim2);
					v_trim.push_back(_trim3);
					v_trim.push_back(_trim4);

					v_trimh.push_back(_trim1);
					v_trimh.push_back(_trim4);
					v_trimh.push_back(_trim3);
					v_trimh.push_back(_trim2);

					v_trimw.push_back(_trim3);
					v_trimw.push_back(_trim2);
					v_trimw.push_back(_trim1);
					v_trimw.push_back(_trim4);


					_usedLineX = v_trim[selectMachine.m_usedLineX1];
					_usedLineY = v_trim[selectMachine.m_usedLineY1];

				}
				else//˫��λʱ��
				{
					if (prepareExportGCodeData.key%2!=0)//��λ1
					{
						v_trim.push_back(_trim1);
						v_trim.push_back(_trim2);
						v_trim.push_back(_trim3);
						v_trim.push_back(_trim4);

						v_trimh.push_back(_trim1);
						v_trimh.push_back(_trim4);
						v_trimh.push_back(_trim3);
						v_trimh.push_back(_trim2);

						v_trimw.push_back(_trim3);
						v_trimw.push_back(_trim2);
						v_trimw.push_back(_trim1);
						v_trimw.push_back(_trim4);



						_usedLineX = v_trim[selectMachine.m_usedLineX1];
						_usedLineY = v_trim[selectMachine.m_usedLineY1];
					}
					else//��λ2
					{
						v_trim.push_back(_ftrim1);
						v_trim.push_back(_ftrim2);
						v_trim.push_back(_ftrim3);
						v_trim.push_back(_ftrim4);

						v_trimh.push_back(_ftrim1);
						v_trimh.push_back(_ftrim4);
						v_trimh.push_back(_ftrim3);
						v_trimh.push_back(_ftrim2);

						v_trimw.push_back(_ftrim3);
						v_trimw.push_back(_ftrim2);
						v_trimw.push_back(_ftrim1);
						v_trimw.push_back(_ftrim4);


						_usedLineX = v_trim[selectMachine.m_usedLineX2];
						_usedLineY = v_trim[selectMachine.m_usedLineY2];
					}
				}	
#endif
				knifeClass.SetSmallPanelCutTwicePara(prepareExportGCodeData.bCutTwice_Enable,prepareExportGCodeData.fCutTwice_SmallSize,prepareExportGCodeData.fCutTwice_ReserveHeight,prepareExportGCodeData.fCutTwice_SecondSpeed);		// ����С������������
				knifeClass.SetThicknessThresholdPara(prepareExportGCodeData.fTwoCutThreshold,prepareExportGCodeData.fTwoCutReserve,prepareExportGCodeData.fTwoCutSpeed);											// ����С��������

				// �Զ���������λ�ù���
				float tiebiao_width = 0;
				float tiebiao_height = 0;
				GetTiebiaoSize(tiebiao_width,tiebiao_height);


				knifeClass.LabelHeight(tiebiao_height);				//���������ǩ�߶�
				knifeClass.LabelWidth(tiebiao_width);				//���������ǩ���

				PreProcessOfExportGCode();


				// ���һ��С��Ԥ�ޱ߹���
				if (paramdlg.m_CheckPreDeburring == TRUE)
				{
					// ��ǰPanel������һ��С�忿�ߣ��ޱߣ���ֹ�ܰ�
					CSingleon* pSingleton = CSingleon::GetSingleton();
					float knife_radius = pSingleton->m_BaseInfo.m_SawKerfWidth/2;

					int nCpnNum = pThePanel->GetComponentNum();
					Component* pLastCpn = pThePanel->GetComponent(nCpnNum);

					// ���洦������˫���޷�����Ϣʱ��Ԥ�ޱ�
					if (prepareExportGCodeData.node_dvcut == "0" || pThePanel->HaveDownerFaceInfo() == false)
					{

						if (pLastCpn != NULL)
						{
							float cpn_max_x, cpn_max_y, cpn_min_x, cpn_min_y;

							cpn_min_x = pLastCpn->m_x;
							cpn_min_y = pLastCpn->m_y;
							cpn_max_x = pLastCpn->m_x + pLastCpn->m_RealLength;
							cpn_max_y = pLastCpn->m_y + pLastCpn->m_RealWidth;


							// ����ĸ������ꡣ
							float panel_max_x, panel_max_y, panel_min_x, panel_min_y;

							panel_min_x = pThePanel->m_x;
							panel_min_y = pThePanel->m_y;
							panel_max_x = pThePanel->m_x + pThePanel->m_RealLength;
							panel_max_y = pThePanel->m_y + pThePanel->m_RealWidth;

							// �ж�������

#if 1
							// 2017-10-10 yuanzb ����Ҫ���洦������˫���޷�����Ϣʱ��Ԥ�ޱ�

							if (cpn_min_x == panel_min_x)	// ��߻�һ��
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_min_x - knife_radius;
								one_cut._y1 = cpn_max_y;
								one_cut._x2 = cpn_min_x - knife_radius;
								one_cut._y2 = cpn_min_y;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_min_y == panel_min_y)	// �±߻�һ��
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_min_x;
								one_cut._y1 = cpn_min_y - knife_radius;
								one_cut._x2 = cpn_max_x;
								one_cut._y2 = cpn_min_y - knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_max_x == panel_max_x)	// �ұ߻�һ��
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_max_x + knife_radius;
								one_cut._y1 = cpn_min_y;
								one_cut._x2 = cpn_max_x + knife_radius;
								one_cut._y2 = cpn_max_y;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_max_y == panel_max_y)	// �ϱ߻�һ��
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_max_x;
								one_cut._y1 = cpn_max_y + knife_radius;
								one_cut._x2 = cpn_min_x;
								one_cut._y2 = cpn_max_y + knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}



#else

							// 2017-8-16 yuanzb ����Ҫ��ֻ�޳��ߵ�һ��
							if (cpn_max_y == panel_max_y)	// �ϱ߻�һ��
							{
								CutPath one_cut ;

								one_cut._x1 = /*cpn_max_x*/pThePanel->m_RealLength/2;
								one_cut._y1 = cpn_max_y + knife_radius;
								one_cut._x2 = /*cpn_min_x*//*pThePanel->m_OriginX*/0;
								one_cut._y2 = cpn_max_y + knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

#endif

						}
					}


				}

				try
				{
					int panel_index = i;

					//���������G����
					if (prepareExportGCodeData.node_dvcut=="0")//������
					{
						HaveDvsw = FALSE;
						HaveDvs = FALSE;
					}
					else//˫����
					{
						if (prepareExportGCodeData._cutDouble == false)//����λ
						{
							HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	1,					v_trimw,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
							
							HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	2,					v_trimh,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
						}
						else//˫��λ
						{
							if (prepareExportGCodeData.key%2!=1)//��λ2
							{
								HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
																		prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1 ,
																		next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																		next_nc_font_path,	1,					v_trimw,		_GBFirst2,	
																		_GBLast2,			_iscombine,			_MFirst2,		_MLast2,	
																		_PZFirst2,			_PZLast2, total_check_panel_num,	PanelCommnets);
								
								HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
																		prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
																		next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																		next_nc_font_path,	2,					v_trimh,		_GBFirst2,
																		_GBLast2,			_iscombine,			_MFirst2,		_MLast2,
																		_PZFirst2,			_PZLast2, total_check_panel_num,	PanelCommnets);
							}
							else//��λ1
							{
								HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
																		prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
																		next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																		next_nc_font_path,	1,					v_trimw,		_GBFirst1,
																		_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																		_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);

								HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
																		prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
																		next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																		next_nc_font_path,	2,					v_trimh,		_GBFirst1,
																		_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																		_PZFirst1,		_PZLast1, total_check_panel_num,	PanelCommnets);
							}
						}
					}

					//L4Cpp::Log()->debug("4.output font face");

					//����NC����
					//path HG��ͷ����ͨNC�ļ���pathp TB��ͷ���Զ�����
					//print_str2, ��ͨNC�ļ�����һ��nc�ļ�����print_str3 �����ļ�����һ��nc�ļ���

					// ˫��λʱ �����İ�Ϊ��λ1 ˫���İ�Ϊ��λ2

					if (prepareExportGCodeData.node_double == true)
					{
						// ��λ2
						if ((panel_index+1) % 2 == 0)
						{
							knifeClass.OutputGcode(	pThePanel,			nc_font_path,		tb_font_path,		prepareExportGCodeData.node_print,			
													prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,					panel_index+1,		
													next_tb_font_path,	next_nc_font_path,	_printinfo,			prepareExportGCodeData.node_groupstr,	
													next_nc_font_path,	_MFirst2,			_MLast2,			_PZFirst2,		
													_PZLast2,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
						}
						else		// ��λ1
						{
							knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
													prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
													_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
													_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
						}
					}
					else
					{

						knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
												prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
												_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
												_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
												prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
					}

					PostProcessOfExportGCode();
				}
				catch(...)
				{
					MessageBox("NC ���ɳ���");
					PostProcessOfExportGCode();
					return;
				}

				TransCoordinateData transCoordinateData;
				transCoordinateData.nc_font_temp_path = nc_font_temp_path;
				transCoordinateData.nc_font_path = nc_font_path;
				transCoordinateData.nc_back_w_path = nc_back_w_path;
				transCoordinateData.nc_back_w_temp_path = nc_back_w_temp_path;
				transCoordinateData.nc_back_h_path = nc_back_h_path;
				transCoordinateData.nc_back_h_temp_path = nc_back_h_temp_path;
				transCoordinateData.HaveDvsh = HaveDvsh;
				transCoordinateData.HaveDvsw = HaveDvsw;
				transCoordinateData.HaveDvs = HaveDvs;
				transCoordinateData._usedLineX = _usedLineX;
				transCoordinateData._usedLineY = _usedLineY;
				transCoordinateData.nDecimal = DecimalPlace + 1;
				AfterExportGCode_TransCoordinate(&prepareExportGCodeData, &transCoordinateData);

				prepareExportGCodeData.key++;
				//�ļ��ϲ�

#if 1
				CombineFileData combineFileData;
				combineFileData.str_nextthin = str_nextthin;
				combineFileData.tempnextcname2 = tempnextcname2;
				combineFileData.str_thin = str_thin;
				combineFileData.nc_font_temp_path = nc_font_temp_path;
				combineFileData.nc_font_path = nc_font_path;
				combineFileData.nc_back_h_path = nc_back_h_path;
				combineFileData.nc_back_w_path = nc_back_w_path;
				combineFileData.allpath2 = allpath2;
				AfterExportGCode_CombineFile(&prepareExportGCodeData, &combineFileData);

#endif
				stringActualLabelNCFilePath = tb_font_path;
				//L4Cpp::Log()->debug("7.file clear");
				//��Ӧ�� NC �ļ��ƶ�����ǰĿ¼�� cutĿ¼ ��
				//��Ӧ�� TB���� �ļ��ƶ�����ǰĿ¼�� printĿ¼ ��
				if (selectMachine.m_fileClassifyMove == "1")
				{
					rename(tb_font_path.c_str(),tb_font_new_path.c_str());
					stringActualLabelNCFilePath = tb_font_new_path;
				}
#if 1			
				//NC�ļ������ֱ��
				if (selectMachine.m_NC_CommandChange == "1")
				{
					try
					{
						Change_G0X_To_GX(nc_font_path);
						Change_G0X_To_GX(nc_back_w_path);
						Change_G0X_To_GX(nc_back_h_path);
						Change_G0X_To_GX(allpath2);
					}
					catch (...)
					{
						MessageBox("NC�ļ�ɨ�账�� ����CMD������ ����");
						return;
					}
				}

				//NC����ո�ɾ��
				if (selectMachine.m_NC_SpaceRemove == "1")
				{
					try
					{
						Change_PosSpace_To_Empty(nc_font_path);
						Change_PosSpace_To_Empty(nc_back_w_path);
						Change_PosSpace_To_Empty(nc_back_h_path);
						Change_PosSpace_To_Empty(allpath2);
					}
					catch (...)
					{
						MessageBox("NC�ļ�ɨ�账�� NC����ո�ɾ�� ����");
						return;
					}
				}
					//NC�޸�ϳ����ʽ,����G90 G03 X-134.0000 Y-568.0000 R14.0000����ʽ����Ϊ��G03 X-134.0000 Y-568.0000 U14.0000��
					if (selectMachine.m_NC_ModifyArcMillingFormat == "1")
					{
						try
						{
							Change_Arc_Milling_Format(nc_font_path);
							Change_Arc_Milling_Format(nc_back_w_path);
							Change_Arc_Milling_Format(nc_back_h_path);
							Change_Arc_Milling_Format(allpath2);
						}
						catch (...)
						{
							MessageBox("NC�ļ�ɨ�账�� �޸�ϳ����ʽ ����");
							return;
						}
					}
#endif

				//L4Cpp::Log()->debug("7.one coor end");
				SyntecDataItem syntecDataItem = {nc_font_path.c_str(), pThePanel};
				vSyntecData.push_back(syntecDataItem);

				XiaoFengDataItem xiaoFengDataItem = {pThePanel, stringActualLabelNCFilePath.c_str(), nc_font_path.c_str()};
				vXiaoFengData.push_back(xiaoFengDataItem);
			}

			if (selectMachine.m_SmallPanelNC == "1")
			{
				try
				{
					//OnOutputSmallPanelGcode(&selectMachine,NCSavePath);
				}
				catch (...)
				{
					MessageBox("����С��NC�����쳣");
					return;
				}
			}

			if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::SyntecCycle)
			{
				LabelPastingSide eSide = LabelPastingSide::LABEL_UP_SIDE;
				if(prepareExportGCodeData._isLabelInReverseSide)
				{
					if (prepareExportGCodeData.node_dvcut.compare("1") == 0)
						eSide = LabelPastingSide::LABEL_BACK_SIDE_H_FLIP;
					else if (prepareExportGCodeData.node_dvcut.compare("2") == 0)		
						eSide = LabelPastingSide::LABEL_BACK_SIDE_V_FLIP;
				}

				SyntecSummaryOutput(vSyntecData, /*prepareExportGCodeData.nodecoor_type*/0, prepareExportGCodeData.pathName, eSide);
			}
			else if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::XiaoFeng)
				XiaoFentLabelProcess(vXiaoFengData, prepareExportGCodeData.pathName);


			AfxMessageBox(_T("�������"));

	//L4Cpp::Log()->info("nc ok");
}

LRESULT CCarvingView::OnExportComponentNC(WPARAM wparam, LPARAM lparam)
{
	vector<CString>& vComponentBarcode = *((vector<CString>*)wparam);
	vector<Panel*> vComponentPanel;
	for(int i = 0; i < vComponentBarcode.size(); i++)
	{
		for(int j = 0; j < m_vComponentInputItem.size(); j++)
		{
			if(m_vComponentInputItem[j].m_strBarcode.Compare(vComponentBarcode[i]) == 0)
			{
				Component* pComponent = ConvertOneInputInfoToOneComponent(m_vComponentInputItem[j]);
				pComponent->m_x = pComponent->m_y = 0;
				Panel* pPanel = new Panel;
				pPanel->m_OrgLen = pPanel->m_RealLength = pComponent->m_RealLength;
				pPanel->m_OrgWidth = pPanel->m_RealWidth = pComponent->m_RealWidth;
				pPanel->m_OriginX = pPanel->m_OriginY = 0;
				pPanel->m_Material = pComponent->m_Material;
				pPanel->m_Texture = pComponent->m_Texture;	
				pPanel->m_Thickness = pComponent->m_Thickness;
				pPanel->m_type = NodeType_CombinePanel;
				pPanel->m_pParent = NULL;
				pPanel->m_ChildrenList.push_back(pComponent);

				vComponentPanel.push_back(pPanel);

				break;
			}
		}
	}
	
	ExportComponentNC(vComponentPanel);

	for(int i = 0; i < vComponentPanel.size(); i++)
	{
		delete (Component*)(vComponentPanel[i]->m_ChildrenList[0]);
		vComponentPanel[i]->m_ChildrenList.clear();
		delete vComponentPanel[i];
	}

	return 0;
}

int CCarvingView::ExportComponentNC(vector<Panel*>& vComponentPanel)
{
	KnifeClass& knifeClass = *(CSingleon::GetSingleton()->m_pKnifeClass);

	// ��ʾ��Ϣ��ʼ����ֻ��ʾһ���
	knifeClass.EnaleMsgReport();


	// ��ȡС��λ�������ע��
	ParamSettingDlg settings;
	settings.loadSettings();
	int DecimalPlace = settings.m_valDecimalPlace;
	BOOL PanelCommnets = settings.m_chkboxPanelComments;





	CSelMachineDlg selectMachine;
	CParamDlg paramdlg;
	paramdlg.loadSettings();
	PrepareExportGCodeData prepareExportGCodeData;
	prepareExportGCodeData.m_pSelMachineDlg = &selectMachine;
	prepareExportGCodeData.pParamDlg = &paramdlg;
	prepareExportGCodeData.pKnifeClass = &knifeClass;
	bool bPrepareResult = PrepareExportGCode(&prepareExportGCodeData);
	if(!bPrepareResult)
		return 1;
	prepareExportGCodeData.node_print = 0;

	int total_check_panel_num = 1;

		//CDlgLabelPosCalc dlgLabelPosCalc(selectMachine.m_nodename.c_str());
		//dlgLabelPosCalc.DoModal();

		/////////////////////////////////////////////////////////////////////////////////
		////���ÿ�����Ϣ������������
		vector<P_Info> _printinfo;
		//for(int i = 0; i < m_vComponentInputItem.size(); i++)
		//{
		//	P_Info temp;
		//	temp._info = m_vComponentInputItem[i].m_strBarcode;
		//	temp._content = getPrintInfoAsString(m_vComponentInputItem[i]);
		//	_printinfo.push_back(temp);
		//}





			// ����knifeClass��Ϣ
			//for(int i = 0; i < prepareExportGCodeData.vPanelAndTheSolution.size(); i++)
			//{
				//Panel* pThePanel = prepareExportGCodeData.vPanelAndTheSolution[i].m_pPanel;
				//CSolution* pTheSolution = prepareExportGCodeData.vPanelAndTheSolution[i].m_pSolution;
			for(int j = 0; j < vComponentPanel.size(); j++)
			{
				Panel* pThePanel = vComponentPanel[j];
				Component& theComponent = *((Component*)(pThePanel->m_ChildrenList[0]));

				int i = 0;

				// ������Ϣ����
				knifeClass.setKerfwidth(0);
				knifeClass.setPanelWidth(pThePanel->m_RealLength);
				knifeClass.setPanelHeight(pThePanel->m_RealWidth);
				knifeClass.setThickness(pThePanel->m_Thickness);
				knifeClass.setDeburringwidth(/*paramdlg.Deburringwidth()*/0);

				string strThicknessAndMetrial;
				CString cstrThicknessAndMetrial = GetFloatString(pThePanel->m_Thickness, 1) + "-" + pThePanel->m_Material;
				strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
				cstrThicknessAndMetrial.ReleaseBuffer();

				knifeClass.setThinMatrial(strThicknessAndMetrial);
				knifeClass.setPanelMatrial(pThePanel->m_Material.GetBuffer());
				pThePanel->m_Material.ReleaseBuffer();
				knifeClass.SetInputOrderNum("a1");
				knifeClass.SetLeftParam(prepareExportGCodeData.fCutTwice_SmallSize,//С����С�ߴ�
					prepareExportGCodeData.fCutTwice_ReserveHeight,//Ԥ�������
					prepareExportGCodeData.fCutTwice_SecondSpeed);//�ڶ����ӹ�����

				// ����gcodeparam����Ϣ
				knifeClass.m_gcodeparam.upheight = paramdlg.Upheight();
				knifeClass.m_gcodeparam.downheight = paramdlg.Downheight();
				knifeClass.m_gcodeparam.distance1 = paramdlg.Distance1();
				knifeClass.m_gcodeparam.distance2 = paramdlg.Distance2();


				if (prepareExportGCodeData._cutDouble==false)//����λʱ
				{
					prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;//����λ����ϵ
				}
				else//˫��λʱ��
				{
					if (prepareExportGCodeData.key%2!=0)//��λ1
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;
					}
					else//��λ2
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type2;
					}
				}

				/************************************************************************/
				/* ����indexλ�ã���ȡ�����Ϣ                                          */
				/************************************************************************/
				//��ȡ��ǰ���
				//Panel_Knife pk = m_picturedlg->_picturecoor[panel_index];
				//PanelNode * node = pk._panel;
				//KnifeClass * kc = pk._kc;

				////��ȡ��һ����
				//int next_panel_index = panel_index;	//��ʼֵ
				//do 
				//{
				//	next_panel_index++;
				//}while((next_panel_index < m_picturedlg->_picturecoor.size())
				//	&&(m_picturedlg->m_piclist.m_panelChecked[next_panel_index] == false));

				////�¿����Ƿ����
				bool isHaveNextPanelUsable = false;

				//////////////////////////////////////////////////////////////////////////
				//�ļ�ͷβȡֵ
				//������¸��ļ�������ʽ
				if (isHaveNextPanelUsable)
				{
					_header = prepareExportGCodeData.temp_header1;
					_ender = prepareExportGCodeData.temp_ender1;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_1;//��λ2�ļ�ͷ
					_GLast2 = prepareExportGCodeData.temp_GLast2_1;//��λ2�ļ�β

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_1;//��λ1�����ļ�ͷ
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_1;//��λ2�����ļ�ͷ
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_1;//��λ1�����ļ�β
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_1;//��λ2�����ļ�β
				}
				else
				{
					_header = prepareExportGCodeData.temp_header2;
					_ender = prepareExportGCodeData.temp_ender2;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_2;//��λ2�ļ�ͷ
					_GLast2 = prepareExportGCodeData.temp_GLast2_2;//��λ2�ļ�β

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_2;//��λ1�����ļ�ͷ
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_2;//��λ2�����ļ�ͷ
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_2;//��λ1�����ļ�β
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_2;//��λ2�����ļ�β
				}


				//2018-5-3 yuanzb ȥ��\r\n�е�\r�������Ļ���,��Щ�����޷���ȡ����
#if 1

				if (_header.find("\r\n") != std::string::npos)
				{
					_header.erase(std::remove(_header.begin(), _header.end(), '\r'), _header.end());
				}

				if (_ender.find("\r\n") != std::string::npos)
				{
					_ender.erase(std::remove(_ender.begin(), _ender.end(), '\r'), _ender.end());
				}

				if (_GFirst2.find("\r\n") != std::string::npos)
				{
					_GFirst2.erase(std::remove(_GFirst2.begin(), _GFirst2.end(), '\r'), _GFirst2.end());
				}

				if (_GLast2.find("\r\n") != std::string::npos)
				{
					_GLast2.erase(std::remove(_GLast2.begin(), _GLast2.end(), '\r'), _GLast2.end());
				}

				if (_GBFirst1.find("\r\n") != std::string::npos)
				{
					_GBFirst1.erase(std::remove(_GBFirst1.begin(), _GBFirst1.end(), '\r'), _GBFirst1.end());
				}

				if (_GBFirst2.find("\r\n") != std::string::npos)
				{
					_GBFirst2.erase(std::remove(_GBFirst2.begin(), _GBFirst2.end(), '\r'), _GBFirst2.end());
				}

				if (_GBLast1.find("\r\n") != std::string::npos)
				{
					_GBLast1.erase(std::remove(_GBLast1.begin(), _GBLast1.end(), '\r'), _GBLast1.end());
				}

				if (_GBLast2.find("\r\n") != std::string::npos)
				{
					_GBLast2.erase(std::remove(_GBLast2.begin(), _GBLast2.end(), '\r'), _GBLast2.end());
				}





#endif




				//�����ļ���
#if 1

				std::string tempnode_pname = "";
				std::string tempnode_plast = "";
				std::string tempnode_cname = "";
				std::string tempnode_clast = "";

				//����¿�����Ϣ
				std::string str_thinmatrial = /*kc->getPanelMatrial()*/pThePanel->m_Material.GetBuffer();
				pThePanel->m_Material.ReleaseBuffer();
				CString s_thinmatrial,s_thin,s_nextthin;
				std::string str_nextthin = "";//��һ�����İ��
				std::string str_nextthinmatrial = "";//��һ���Ĳ���
				s_thinmatrial.Format("%s",str_thinmatrial.c_str());
				s_thin.Format("%0.0f",/*kc->getThickness()*/pThePanel->m_Thickness);
				s_thinmatrial.Replace("\/","");
				s_thinmatrial.Replace("\\","");
				str_thinmatrial = s_thinmatrial.GetBuffer();
				std::string str_thin = s_thin.GetBuffer();

				CString s_nextmaterial;
				s_nextmaterial.Format("%s",str_nextthinmatrial.c_str());
				s_nextthin.Format("%s",str_nextthin.c_str());
				s_nextmaterial.Replace("\/","");
				s_nextmaterial.Replace("\\","");
				str_nextthinmatrial = s_nextmaterial.GetBuffer();
				std::string filename,filenamed,filenamed1,allfile,pfilename,filenameo,pfilenameo,filenamedo,filenamed1o;
				char ch_filename[1000],ch_filenamed[1000],ch_filenamed1[1000], ch_allfile[1000],ch_pfilename[1000],ch_filenameo[1000],ch_pfilenameo[1000],ch_filenamedo[1000],ch_filenamed1o[1000];
				char ch_allFileName[1000];
				char theKey[1000],theKey2[1000];//theKey2��ʾ��һ��������ˮ��

				//��ˮ��
				if (/*panel_index*/i<10)
				{
					sprintf_s(theKey,"0%d",/*panel_index*/i + 1);
				}
				else
				{
					sprintf_s(theKey,"%d",/*panel_index*/i + 1);
				}

				if (/*next_panel_index*/i+1 <10)
				{
					sprintf_s(theKey2,"0%d",/*next_panel_index*/(i+1) + 1);
				}
				else
				{
					sprintf_s(theKey2,"%d",/*next_panel_index*/(i+1) + 1);
				}



				std::string tempcname2;//�ļ���+��ˮ��+�������+������cname
				std::string temppname2;//�ļ���+��ˮ��+�������+������pname
				std::string tempnextcname2;//�ļ���+��ˮ��+�������+������cname
				std::string tempnextpname2;//�ļ���+��ˮ��+�������+������pname
				std::string theAllCname;
				theAllCname = prepareExportGCodeData.node_cname;

				//����nc������ļ���
				{
					/*sprintf_s(ch_allfile,"\\All-%s-%s.nc",node_fname.c_str(),str_thin.c_str());
					sprintf_s(ch_filename,"\\C_%s%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilename,"\\P_%s%d.nc",node_fname.c_str(),key);
					sprintf_s(ch_filenameo,"\\C_%s-%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilenameo,"\\P_%s-%d.nc",node_fname.c_str(),key);*/

					if (prepareExportGCodeData.node_cname.find("@")!=std::string::npos)//nc�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = prepareExportGCodeData.node_cname.find_first_of('@');
						std::string theCname = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc�ļ���
						std::string theCname2 = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc�ļ���
						theAllCname = theCname2;
						std::string theCMaterial = str_thinmatrial;//����
						std::string theCThickness = str_thin;//���
						std::string thenextCMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextCThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextCThickness = HGTools::Convert2CString(thenextCThickness);
						cstr_thenextCThickness.Replace(".0","");
						thenextCThickness = HGTools::Convert2StdString(cstr_thenextCThickness);

						if (prepareExportGCodeData.node_cname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCMaterial;
							tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey + prepareExportGCodeData.node_cxhx+theCMaterial; 
							tempnextcname2 = theCname2+ prepareExportGCodeData.node_cxhx + theKey2+ prepareExportGCodeData.node_cxhx + thenextCMaterial;
						}
						if (prepareExportGCodeData.node_cname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCThickness + prepareExportGCodeData.node_cxhx;
							if (tempcname2=="")
							{
								tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey+ prepareExportGCodeData.node_cxhx + theCThickness;
							}
							else
								tempcname2 = tempcname2+ prepareExportGCodeData.node_cxhx + theCThickness;
							if (tempnextcname2=="")
							{
								tempnextcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey2 + prepareExportGCodeData.node_cxhx+ thenextCThickness;
							}
							else
								tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_cxhx + thenextCThickness;
						}
						tempnode_cname = theCname+ prepareExportGCodeData.node_cxhx + theKey;
					}
					else
					{
						tempnode_cname = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+theKey;
						tempcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey; 
						tempnextcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey2;
					}

					if (prepareExportGCodeData.node_pname.find("@")!=std::string::npos)//p�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = prepareExportGCodeData.node_pname.find_first_of('@');
						std::string thePname = prepareExportGCodeData.node_pname.substr(0,thePos1);//p�ļ���
						std::string thePname2 = prepareExportGCodeData.node_pname.substr(0,thePos1);//p�ļ���
						std::string thePMaterial = str_thinmatrial;//����
						std::string thePThickness = str_thin;//���
						std::string thenextPMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextPThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextPThickness = HGTools::Convert2CString(thenextPThickness);
						cstr_thenextPThickness.Replace(".0","");
						thenextPThickness = HGTools::Convert2StdString(cstr_thenextPThickness);


						if (prepareExportGCodeData.node_pname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePMaterial;
							temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx+thePMaterial; 
							tempnextpname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPMaterial;
						}
						if (prepareExportGCodeData.node_pname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePThickness + prepareExportGCodeData.node_pxhx;
							if (temppname2=="")
							{
								temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx + thePThickness;
							}else
								temppname2 = temppname2 + prepareExportGCodeData.node_pxhx + thePThickness;
							if (tempnextpname2=="")
							{
								tempnextpname2 = thePname2+ prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPThickness;
							}else
								tempnextpname2 = tempnextpname2 + prepareExportGCodeData.node_pxhx+ thenextPThickness;
						}
						tempnode_pname = thePname+ prepareExportGCodeData.node_pxhx + theKey;
					}
					else
					{
						tempnode_pname = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx +theKey;
						temppname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey; 
						tempnextpname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey2;
					}
					//sprintf_s(ch_allfile,"\\All-%s.nc",tempcname2.c_str());
					sprintf_s(ch_filename,"\\%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilename,"\\%s%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());

					sprintf_s(ch_filenameo,"\\%s_%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilenameo,"\\%s_%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				}

				theAllCname = theAllCname + prepareExportGCodeData.node_cxhx + str_thinmatrial + prepareExportGCodeData.node_cxhx + str_thin;//�ϲ�ͬһ���ͺ���ļ���

				sprintf_s(ch_allfile,"\\All%s_.nc",theAllCname.c_str());
				sprintf_s(ch_allFileName,"\\All%s.nc",theAllCname.c_str());
				/*tempnextpname2 = tempnextpname2 + node_plast;
				tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_clast;*/
				tempnextpname2 = tempnextpname2;
				tempnextcname2 = tempnextcname2;

				std::string thCnameW = tempcname2+"@W";
				std::string thCnameH = tempcname2+"@H";
				std::string thCname_W = tempcname2+"@-W";
				std::string thCname_H = tempcname2+"@-H";
				/*sprintf_s(ch_filenamed,"\\HG_%d-%s@W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1,"\\HG_%d-%s@H.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamedo,"\\HG_%d-%s@-W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1o,"\\HG_%d-%s@-H.nc",key,str_thin.c_str());*/
				sprintf_s(ch_filenamed,"\\%s%s",thCnameW.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1,"\\%s%s",thCnameH.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamedo,"\\%s%s",thCname_W.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1o,"\\%s%s",thCname_H.c_str(),prepareExportGCodeData.node_clast.c_str());

				filename = ch_filename;//nc
				pfilename = ch_pfilename;//p
				filenamed = ch_filenamed;//@W
				filenamed1 = ch_filenamed1;//@H
				allfile = ch_allfile;
				filenameo = ch_filenameo;//-nc
				pfilenameo = ch_pfilenameo;//-p
				filenamedo = ch_filenamedo;//@-W
				filenamed1o = ch_filenamed1o;//@-H

				std::string print_str1,next_tb_font_path,next_nc_font_path;
				char ch_print_str1[1000],ch_print_str2[1000],ch_print_str3[1000];
				sprintf_s(ch_print_str1,"%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				

				//������һ���
				sprintf_s(ch_print_str3,"%s%s",tempnextcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_print_str2,"%s%s",tempnextpname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				/*sprintf_s(ch_print_str1,"C_HG%d-%s.nc",key,str_thin.c_str());
				sprintf_s(ch_print_str3,"C_HG%d-%s.nc",key+1,str_nextthin.c_str());
				sprintf_s(ch_print_str2,"P_HG%d.nc",key+1);*/
				print_str1 = ch_print_str1;


				if (!isHaveNextPanelUsable)
				{
					next_tb_font_path = "";
					next_nc_font_path = "";
				}
				else
				{
					next_tb_font_path = ch_print_str2;
					next_nc_font_path = ch_print_str3;
				}



				std::string prev_tempFilename		=	ch_allFileName;
#if 1

				// 2016-11-11	yzb		����HM16110101���ǻԵĻ����޷�ʶ�����ģ��������е�Ӣ���豣��
				// 2017-5-56	yuanzb	�´���֧������
				if (prepareExportGCodeData.node_name.find("�ǻ�") != string::npos ||
					prepareExportGCodeData.node_print == 14)
				{
					DelChineseInString(filename	);
					DelChineseInString(pfilename);
					DelChineseInString(pfilename);
					DelChineseInString(filenameo);

					DelChineseInString(pfilenameo);
					DelChineseInString(filenamed	);
					DelChineseInString(filenamed1);
					DelChineseInString(allfile	);

					DelChineseInString(filenamedo	 );
					DelChineseInString(filenamed1o	 );
					DelChineseInString(prev_tempFilename);

					DelChineseInString(next_nc_font_path);
					DelChineseInString(next_tb_font_path);

				}

#endif



				//׼��������������ļ���
				//std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ filename		;//nc			//nc�ļ�
				//std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//�Զ�����nc�ļ�
				//std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//�Զ�����nc�ļ�
				//std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenameo		;//-nc			//nc��ʱ�ļ���

				//std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//�Զ�������ʱ�ļ�
				//std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed		;//@W			//W��תnc�ļ�
				//std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1	;//@H			//H��תnc�ļ�
				//std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ allfile		;

				//std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamedo	;//@-W			//W��תnc��ʱ�ļ�
				//std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1o	;//@-H			//H��תnc��ʱ�ļ�
				//std::string tempFilename		=							  prev_tempFilename							;
				//std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ tempFilename	;				//�ϲ�NC�ļ�

				//next_nc_font_path;																			//��һ����nc�ļ���
				//next_tb_font_path;																			//��һ����tb�ļ���

				CString strTmp;
				strTmp.Format(_T("%s%s%s"), _T("\\"), theComponent.m_BarCode, prepareExportGCodeData.node_clast.c_str());
				std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//nc			//nc�ļ�
				std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//�Զ�����nc�ļ�
				std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//�Զ�����nc�ļ�
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), theComponent.m_BarCode, _T("_"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//-nc			//nc��ʱ�ļ���

				std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//�Զ�������ʱ�ļ�
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), theComponent.m_BarCode, _T("@W"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//@W			//W��תnc�ļ�
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), theComponent.m_BarCode, _T("@H"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@H			//H��תnc�ļ�
				strTmp.Format(_T("%s%s%s%s%s"), _T("\\"), _T("All"), theComponent.m_BarCode, _T("_"), prepareExportGCodeData.node_clast.c_str());
				std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ 		strTmp;

				strTmp.Format(_T("%s%s%s%s"), _T("\\"), theComponent.m_BarCode, _T("@-W"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@-W			//W��תnc��ʱ�ļ�
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), theComponent.m_BarCode, _T("@-H"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@-H			//H��תnc��ʱ�ļ�
				std::string tempFilename		=							  prev_tempFilename							;
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), _T("All"), theComponent.m_BarCode, prepareExportGCodeData.node_clast.c_str());
				std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;				//�ϲ�NC�ļ�

				next_nc_font_path;																			//��һ����nc�ļ���
				next_tb_font_path;																			//��һ����tb�ļ���


#endif


				//��ȡ������Ϣ
#if 1
				PrepareKnifeInfoData prepareKnifeInfoData;
				prepareKnifeInfoData.pThePanel = pThePanel;
				//prepareKnifeInfoData.pKnifeClass = &knifeClass;
				//��ȡ������Ϣ
				//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
				PrepareKnifeInfoAndHoleSetting(&prepareExportGCodeData, &prepareKnifeInfoData);
#endif

				//��λƫ�Ƽ���
#if 1
				bool HaveDvsh = false,HaveDvsw = false,HaveDvs = false;
				std::vector<float> v_trim;
				std::vector<float> v_trimh;
				std::vector<float> v_trimw;

				int _usedLineX = 0;
				int _usedLineY = 0;

				//if (prepareExportGCodeData._cutDouble==false)//����λʱ
				//{
				//	v_trim.push_back(_trim1);
				//	v_trim.push_back(_trim2);
				//	v_trim.push_back(_trim3);
				//	v_trim.push_back(_trim4);

				//	v_trimh.push_back(_trim1);
				//	v_trimh.push_back(_trim4);
				//	v_trimh.push_back(_trim3);
				//	v_trimh.push_back(_trim2);

				//	v_trimw.push_back(_trim3);
				//	v_trimw.push_back(_trim2);
				//	v_trimw.push_back(_trim1);
				//	v_trimw.push_back(_trim4);


				//	_usedLineX = v_trim[selectMachine.m_usedLineX1];
				//	_usedLineY = v_trim[selectMachine.m_usedLineY1];

				//}
				//else//˫��λʱ��
				//{
				//	if (prepareExportGCodeData.key%2!=0)//��λ1
				//	{
				//		v_trim.push_back(_trim1);
				//		v_trim.push_back(_trim2);
				//		v_trim.push_back(_trim3);
				//		v_trim.push_back(_trim4);

				//		v_trimh.push_back(_trim1);
				//		v_trimh.push_back(_trim4);
				//		v_trimh.push_back(_trim3);
				//		v_trimh.push_back(_trim2);

				//		v_trimw.push_back(_trim3);
				//		v_trimw.push_back(_trim2);
				//		v_trimw.push_back(_trim1);
				//		v_trimw.push_back(_trim4);



				//		_usedLineX = v_trim[selectMachine.m_usedLineX1];
				//		_usedLineY = v_trim[selectMachine.m_usedLineY1];
				//	}
				//	else//��λ2
				//	{
				//		v_trim.push_back(_ftrim1);
				//		v_trim.push_back(_ftrim2);
				//		v_trim.push_back(_ftrim3);
				//		v_trim.push_back(_ftrim4);

				//		v_trimh.push_back(_ftrim1);
				//		v_trimh.push_back(_ftrim4);
				//		v_trimh.push_back(_ftrim3);
				//		v_trimh.push_back(_ftrim2);

				//		v_trimw.push_back(_ftrim3);
				//		v_trimw.push_back(_ftrim2);
				//		v_trimw.push_back(_ftrim1);
				//		v_trimw.push_back(_ftrim4);


				//		_usedLineX = v_trim[selectMachine.m_usedLineX2];
				//		_usedLineY = v_trim[selectMachine.m_usedLineY2];
				//	}
				//}	
#endif
				knifeClass.SetSmallPanelCutTwicePara(prepareExportGCodeData.bCutTwice_Enable,prepareExportGCodeData.fCutTwice_SmallSize,prepareExportGCodeData.fCutTwice_ReserveHeight,prepareExportGCodeData.fCutTwice_SecondSpeed);		// ����С������������
				knifeClass.SetThicknessThresholdPara(prepareExportGCodeData.fTwoCutThreshold,prepareExportGCodeData.fTwoCutReserve,prepareExportGCodeData.fTwoCutSpeed);											// ����С��������

				// �Զ���������λ�ù���
				float tiebiao_width = 0;
				float tiebiao_height = 0;
				GetTiebiaoSize(tiebiao_width,tiebiao_height);


				knifeClass.LabelHeight(tiebiao_height);				//���������ǩ�߶�
				knifeClass.LabelWidth(tiebiao_width);				//���������ǩ���

				//PreProcessOfExportGCode();

				for(int i = 0; i < theComponent.m_vDownerFaceHole.size(); i++)
				{
					theComponent.m_vDownerFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceHole[i]._x;
				}
				for(int i = 0; i < theComponent.m_vDownerFaceSlot.size(); i++)
				{
					theComponent.m_vDownerFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceSlot[i]._x;
				}

				try
				{
					int panel_index = i;

					//���������G����
					if (/*prepareExportGCodeData.node_dvcut=="0"*/false)//������
					{
						HaveDvsw = FALSE;
						HaveDvs = FALSE;
					}
					else//˫����
					{
						if (/*prepareExportGCodeData._cutDouble == false*/true)//����λ
						{
							HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	1,					v_trimw,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
							
							HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	2,					v_trimh,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
						}
						else//˫��λ
						{
							//if (prepareExportGCodeData.key%2!=1)//��λ2
							//{
							//	HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1 ,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	1,					v_trimw,		_GBFirst2,	
							//											_GBLast2,			_iscombine,			_MFirst2,		_MLast2,	
							//											_PZFirst2,			_PZLast2, prepareExportGCodeData.total_check_panel_num);
							//	
							//	HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	2,					v_trimh,		_GBFirst2,
							//											_GBLast2,			_iscombine,			_MFirst2,		_MLast2,
							//											_PZFirst2,			_PZLast2, prepareExportGCodeData.total_check_panel_num);
							//}
							//else//��λ1
							//{
							//	HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	1,					v_trimw,		_GBFirst1,
							//											_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
							//											_PZFirst1,			_PZLast1, prepareExportGCodeData.total_check_panel_num);

							//	HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	2,					v_trimh,		_GBFirst1,
							//											_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
							//											_PZFirst1,		_PZLast1, prepareExportGCodeData.total_check_panel_num);
							//}
						}
					}

					//L4Cpp::Log()->debug("4.output font face");

					//����NC����
					//path HG��ͷ����ͨNC�ļ���pathp TB��ͷ���Զ�����
					//print_str2, ��ͨNC�ļ�����һ��nc�ļ�����print_str3 �����ļ�����һ��nc�ļ���

					// ˫��λʱ �����İ�Ϊ��λ1 ˫���İ�Ϊ��λ2

					if (/*prepareExportGCodeData.node_double == true*/false)
					{
						// ��λ2
						if ((panel_index+1) % 2 == 0)
						{
							knifeClass.OutputGcode(	pThePanel,			nc_font_path,		tb_font_path,		prepareExportGCodeData.node_print,			
													prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,					panel_index+1,		
													next_tb_font_path,	next_nc_font_path,	_printinfo,			prepareExportGCodeData.node_groupstr,	
													next_nc_font_path,	_MFirst2,			_MLast2,			_PZFirst2,		
													_PZLast2,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
						}
						else		// ��λ1
						{
							knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
													prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
													_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
													_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
						}
					}
					else
					{

						knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
												prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
												_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
												_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
												prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
					}

					//PostProcessOfExportGCode();
				}
				catch(...)
				{
					MessageBox("NC ���ɳ���");
					//PostProcessOfExportGCode();
					return 1;
				}

				TransCoordinateData transCoordinateData;
				transCoordinateData.nc_font_temp_path = nc_font_temp_path;
				transCoordinateData.nc_font_path = nc_font_path;
				transCoordinateData.nc_back_w_path = nc_back_w_path;
				transCoordinateData.nc_back_w_temp_path = nc_back_w_temp_path;
				transCoordinateData.nc_back_h_path = nc_back_h_path;
				transCoordinateData.nc_back_h_temp_path = nc_back_h_temp_path;
				transCoordinateData.HaveDvsh = HaveDvsh;
				transCoordinateData.HaveDvsw = HaveDvsw;
				transCoordinateData.HaveDvs = HaveDvs;
				transCoordinateData._usedLineX = _usedLineX;
				transCoordinateData._usedLineY = _usedLineY;
				transCoordinateData.nDecimal = DecimalPlace + 1;
				AfterExportGCode_TransCoordinate(&prepareExportGCodeData, &transCoordinateData);

				prepareExportGCodeData.key++;
				//�ļ��ϲ�

#if 1
				CombineFileData combineFileData;
				combineFileData.str_nextthin = str_nextthin;
				combineFileData.tempnextcname2 = tempnextcname2;
				combineFileData.str_thin = str_thin;
				combineFileData.nc_font_temp_path = nc_font_temp_path;
				combineFileData.nc_font_path = nc_font_path;
				combineFileData.nc_back_h_path = nc_back_h_path;
				combineFileData.nc_back_w_path = nc_back_w_path;
				combineFileData.allpath2 = allpath2;
				AfterExportGCode_CombineFile(&prepareExportGCodeData, &combineFileData);

#endif

				//L4Cpp::Log()->debug("7.file clear");
				//��Ӧ�� NC �ļ��ƶ�����ǰĿ¼�� cutĿ¼ ��
				//��Ӧ�� TB���� �ļ��ƶ�����ǰĿ¼�� printĿ¼ ��
				if (selectMachine.m_fileClassifyMove == "1")
				{
					rename(tb_font_path.c_str(),tb_font_new_path.c_str());
				}
#if 1			
				//NC�ļ������ֱ��
				if (selectMachine.m_NC_CommandChange == "1")
				{
					try
					{
						Change_G0X_To_GX(nc_font_path);
						Change_G0X_To_GX(nc_back_w_path);
						Change_G0X_To_GX(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC�ļ�ɨ�账�� ����CMD������ ����");
						return 1;
					}
				}

				//NC����ո�ɾ��
				if (selectMachine.m_NC_SpaceRemove == "1")
				{
					try
					{
						Change_PosSpace_To_Empty(nc_font_path);
						Change_PosSpace_To_Empty(nc_back_w_path);
						Change_PosSpace_To_Empty(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC�ļ�ɨ�账�� NC����ո�ɾ�� ����");
						return 1;
					}
				}

				//NC�޸�ϳ����ʽ,����G90 G03 X-134.0000 Y-568.0000 R14.0000����ʽ����Ϊ��G03 X-134.0000 Y-568.0000 U14.0000��
				if (selectMachine.m_NC_ModifyArcMillingFormat == "1")
				{
					try
					{
						Change_Arc_Milling_Format(nc_font_path);
						Change_Arc_Milling_Format(nc_back_w_path);
						Change_Arc_Milling_Format(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC�ļ�ɨ�账�� �޸�ϳ����ʽ ����");
						return 1;
					}
				}
#endif


/*				SyntecDataItem syntecDataItem = {nc_font_path.c_str(), pThePanel};
				vSyntecData.push_back(syntecDataItem)*/;
			}

			if (selectMachine.m_SmallPanelNC == "1")
			{
				try
				{
					//OnOutputSmallPanelGcode(&selectMachine,NCSavePath);
				}
				catch (...)
				{
					MessageBox("����С��NC�����쳣");
					return 1;
				}
			}

			//if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::SyntecCycle)
			//	SyntecSummaryOutput(vSyntecData, /*prepareExportGCodeData.nodecoor_type*/0, prepareExportGCodeData.pathName);

			AfxMessageBox(_T("�������"));
			return 0;
}

void CCarvingView::DeleteSpecialCode(std::string &_code)//ɾ��^123^֮������
{
	int posbegin = _code.find("^");
	if (posbegin<0)
	{
		return ;
	}
	int posend = 0;
	posend = _code.rfind('^');
	if(posend<0)
	{
		return;
	}
	int num = posend - posbegin;
	if (num<=0)
	{
		return;
	}
	else
	{
		_code.erase(posbegin,posend+1);
	}
}

void CCarvingView::DeleteSpecialCode2(std::string &_code)//ɾ����^������
{
	int posbegin = _code.find("^");
	std::string::size_type startpos = 0;
	while(startpos!=std::string::npos)
	{
		startpos = _code.find("^");
		if (startpos!=std::string::npos)
		{
			_code.replace(startpos,1,""); //����\\���滻Ϊ�� 
		} 
	}
}

//void CCarvingView::TMP_ConvertInputItemToOldInputItem(vector<map<string,string>>& vItem)
//{
//	for (int i = 0;i < m_vComponentInputItem.size();i++)
//	{
//		map<string,string> mapItem;
//		mapItem["order"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,0));	//������
//		mapItem["cabinet"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,1));	//���
//		mapItem["info"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,2));	//����
//		mapItem["PorductName"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,3));	//��Ʒ����
//		mapItem["porduct"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,4));	//��Ʒ����
//		mapItem["matrial"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,5));	//����
//		mapItem["width"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,6));	//��
//		mapItem["height"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,7));	//��
//		mapItem["thin"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,8));	//���
//		mapItem["num"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,9));	//�и�����
//		mapItem["dir"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,10));	//������
//		mapItem["edgeLen1"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,11));	//�ⳤ1
//		mapItem["edgeWidth1"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,12));	//���1
//		mapItem["edgeLen2"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,13));	//�ⳤ2
//		mapItem["edgeWidth2"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,14));	//���2
//		mapItem["pointInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,15));	//������Ϣ
//		mapItem["rotate"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,16));	//�Ƿ���ת
//		mapItem["slotInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,17));	//��
//		mapItem["slotRotate"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,18));	//�۷�ת
//		mapItem["dslotInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,19));	//�����
//		mapItem["vecInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,20));	//��
//		mapItem["vecRotate"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,21));	//�׷�ת
//		mapItem["dvecInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,22));	//�����
//		mapItem["customeInfo"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,23));	//�ͻ���Ϣ
//		mapItem["store"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,24));	//���˵�
//		mapItem["SawMark"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,25));	//���۱�ʶ
//		mapItem["HelpSign"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,26));	//���Ǻ�
//		mapItem["Vertical"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,27));	//���
//		mapItem["OrderType"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,28));	//��������
//		mapItem["BackCode"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,29));	//��������
//		mapItem["ProductWidth"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,30));	//��Ʒ����
//		mapItem["ProductHeight"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,31));	//��Ʒ���
//		mapItem["ProductThin"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,32));	//��Ʒ���
//		mapItem["otherID"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,33));	//����ID
//		mapItem["customeAddr"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,34));	//�ͻ���ַ
//		mapItem["DrillingLogo"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,35));	//��۱�ʶ
//		mapItem["OtherPoints"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,36));	//���ε���Ϣ
//	}

//����ÿ���������
bool CCarvingView::eachLineAddChar(std::string patho,CString word)
{
	if (HGTools::isFileExists(patho.c_str()))
	{
		CString path;
		path.Format("%s",patho.c_str());

		CStdioFile m_filew;
		CStdioFile m_filer;
		if(m_filew.Open(path+"t",CFile::modeWrite|CFile::modeCreate)){
			if(m_filer.Open(path,CFile::modeRead)){
				try
				{
					CString contents;  
					while (m_filer.ReadString(contents))  
					{  
						if (contents != "")
						{
							contents+=word;
						}

						contents+="\n";

						m_filew.WriteString(contents);
					}  
					m_filer.Close();
					m_filew.Close();
					DeleteFile(path);
					MoveFile(path+"t",path);
					return true;
				}
				catch (...)
				{
					return false;
				}
			}
		}
		return false;
	}
	return false;
}

std::string CCarvingView::ReplaceWithNextPath(std::string _code,std::string theNextPath)//�滻*<abc>
{
	int posbegin = _code.find_first_of("*<");
	if (posbegin<0)
	{
		return _code;
	}
	int posend = 0;
	char temp[1000] = { };
	int j = 0;
	for (int i = posbegin+2;i<_code.size();i++)
	{
		if (_code.at(i)!='>')
		{
			temp[j++] = _code.at(i);
		}
		else
		{
			temp[j] ='\0';
			posend = i;
			break;
		}		
	}
	if (theNextPath!="")
	{
		_code.replace(posbegin,abs(posend-posbegin+1),theNextPath.c_str());
	}
	else
	{
		_code.replace(posbegin,abs(posend-posbegin+1),temp);
	}
	return _code;
}

//ˢ��NC�ļ��������� G0X ��Ϊ GX
void CCarvingView::Change_G0X_To_GX(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			for (int g = 0;g < get_str.size();g++)
			{
				new_str.push_back(get_str.at(g));
				if (g > 0)
				{
					bool g_command = (get_str.at(g-1) == 'G' && get_str.at(g) == '0');

					if (g_command) //G�����0����һ�β������������
					{
						new_str.pop_back();
					}
				}
			}

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}

//ˢ��NC�ļ������� X *** Y *** Z *** F *** R *** -> X*** Y*** Z*** F*** R***
void CCarvingView::Change_PosSpace_To_Empty(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			for (int g = 0;g < get_str.size();g++)
			{
				new_str.push_back(get_str.at(g));
				if (g > 0)
				{
					bool xchange = (get_str.at(g-1) == 'X' && get_str.at(g) == ' ');
					bool ychange = (get_str.at(g-1) == 'Y' && get_str.at(g) == ' ');
					bool zchange = (get_str.at(g-1) == 'Z' && get_str.at(g) == ' ');
					bool fchange = (get_str.at(g-1) == 'F' && get_str.at(g) == ' ');
					bool rchange = (get_str.at(g-1) == 'R' && get_str.at(g) == ' ');

					int compare_size = 5;
					bool compare[] = {xchange,ychange,zchange,fchange,rchange};

					for (int c_index = 0; c_index < compare_size ; c_index++)
					{
						if (compare[c_index]) //�����ո�
						{
							new_str.pop_back();
						}
					}

				}
			}

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}

//ˢ��NC�ļ����޸�ϳ����ʽ,����G90 G03 X*** Y*** R***����ʽ����Ϊ��G03 X*** Y*** U***��
void CCarvingView::Change_Arc_Milling_Format(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			CStringA strOrg = get_str.c_str();
			if(strOrg.Find("G90") != -1 && (strOrg.Find("G02") != -1 || strOrg.Find("G2") != -1) || strOrg.Find("G03") != -1 || strOrg.Find("G3") != -1)
			{
				strOrg.Replace("G90 ", "");
				strOrg.Replace("G90", "");
				strOrg.Replace("R", "U");
				new_str = strOrg.GetBuffer();
				strOrg.ReleaseBuffer();
			}
			else
				new_str = get_str;

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}

void CCarvingView::OnLayoutOrg()
{
	// TODO: �ڴ���������������
}


void CCarvingView::OnButtonPrintLabel()
{
	// TODO: �ڴ���������������
	CSelectPrintDlg dlg;
	dlg.DoModal();
}


BOOL CCarvingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CCarvingView::OnMenuMachineParamSetting()
{
	// TODO: �ڴ���������������
	CKnifeAddDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuBaseSetting()
{
	// TODO: �ڴ���������������
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_pDlgBaseParam->DoModal();
}


void CCarvingView::OnMenuOptimizeSetting()
{
	// TODO: �ڴ���������������
	COPSetDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnRemainderSetting()
{
	// TODO: �ڴ���������������
	CLeftPanelDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuDefaultLoadingDirSetting()
{
	// TODO: �ڴ���������������
	SelectPathDlg dlg("�����ļ�Ĭ�ϼ���·��");
	dlg.CurrentPath(dlg.DefaultLoadPath());
	if (dlg.DoModal() == IDOK)
	{
		dlg.DefaultLoadPath(dlg.CurrentPath());
		dlg.saveSettings();
	}
}


void CCarvingView::OnMenuDefaultSavingDirSetting()
{
	// TODO: �ڴ���������������
	SelectPathDlg dlg("�����ļ�Ĭ�ϱ���·��");
	dlg.CurrentPath(dlg.DefaultSavePath());
	if (dlg.DoModal() == IDOK)
	{
		dlg.DefaultSavePath(dlg.CurrentPath());
		dlg.saveSettings();
	}
}


void CCarvingView::OnMenuOtherSetting()
{
	// TODO: �ڴ���������������
	UISettingDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuLabelSetting()
{
	// TODO: �ڴ���������������
	CPrintSetDlg dlg;
	dlg.DoModal();
}



void CCarvingView::OnMenuExcelTemplateSetting()
{
	// TODO: �ڴ���������������
	//CInOutDlg dlg;
	//dlg.DoModal();

	SelectPathDlg pathDlg;
	CString strFilePath = pathDlg.SelectFileDlgDoModel(
		"���뿪�ϵ�(excel)",
		"xls",
		"*",
		pathDlg.DefaultLoadPath(),
		DlgLoad
		);

	CDlgExcelInputSetting dlg(strFilePath);
	dlg.DoModal();
}

void CCarvingView::OnMenuLingliaoTableSetting()
{
	// TODO: �ڴ���������������
	LingLiaoDlg dlg;
	dlg.DoModal();
}


void CCarvingView::ExportMeterialTable1(vector<Panel*>& vPanelToExport)
{
	CString tempReadName = GetProjectName();
	tempReadName.Replace("��̻��Խ�-","");

	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"����",
		"csv",
		"�Ƴ����-" + tempReadName,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( pathName != "" )
	{
		ParamSettingDlg settings;
		settings.loadSettings();
		TiebiaoEdgeDlg tiebiao;
		tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());


		int nComponentInputItemCount = m_vComponentInputItem.size();
		int* aExportCount = new int[nComponentInputItemCount];
		for(int i = 0; i < nComponentInputItemCount; i++)
			aExportCount[i] = 0;

		for(int i = 0; i < vPanelToExport.size(); i++)
		{
			vector<Component*> vAllComponent;
			FindAllComponentInPanel((Component&)(*(vPanelToExport[i])), vAllComponent);
			for(int j = 0; j < vAllComponent.size(); j++)
			{
				int nIndexOfTheComponent = -1;
				for(int k = 0; k < nComponentInputItemCount; k++)
				{
					if(m_vComponentInputItem[k].m_strBarcode.CompareNoCase(vAllComponent[j]->m_BarCode) == 0)
					{
						nIndexOfTheComponent = k;
						break;
					}
				}
				if(nIndexOfTheComponent != -1)
					aExportCount[nIndexOfTheComponent]++;
			}

		}

		//����С������
		std::vector<std::map<std::string,std::string>> resultArr;
		for(int i = 0; i < nComponentInputItemCount; i++)
		{
			if(aExportCount[i] > 0)
			{
				map<std::string,std::string> mapOneExportItem = GetMapByComponentInputItem(m_vComponentInputItem[i]);//m_pDlgRequirement->GetMatrialItemsOne(i);
				CString strNum;
				strNum.Format("%d", aExportCount[i]);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				mapOneExportItem["num"] = strNum;
				mapOneExportItem["pointInfo"] = ""	;
				mapOneExportItem["slotInfo"]	 = ""	;
				mapOneExportItem["dslotInfo"] = ""	;
				mapOneExportItem["vecInfo"] = ""		;
				mapOneExportItem["dvecInfo"] = ""	;
				mapOneExportItem["OtherPoints"] = ""	;
				mapOneExportItem["edgeLen1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen1"].c_str())).c_str();//"�ⳤ1");
				mapOneExportItem["edgeWidth1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth1"].c_str())).c_str();//"���1");
				mapOneExportItem["edgeLen2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen2"].c_str())).c_str();//"�ⳤ2");
				mapOneExportItem["edgeWidth2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth2"].c_str())).c_str();//"���2");
				resultArr.push_back(mapOneExportItem);
			}

		}

		delete[] aExportCount;

		std::vector<std::string> title;
		for(int i = 0; i < sizeof(g_aRequirementListTitle) / sizeof(char*); i++)
		{
			title.push_back(g_aRequirementListTitle[i]);
		}
		std::vector<std::string> dataCol;
		for(int i = 0; i < sizeof(g_aRequirementListTitle_ENU) / sizeof(char*); i++)
		{
			dataCol.push_back(g_aRequirementListTitle_ENU[i]);
		}
		//����EXCEL�Ķ�������excel
		OnWriteDataCSV(pathName, title, dataCol, resultArr);

		MessageBox("�������");
	}
}

void CCarvingView::ExportMeterialTable2(vector<Panel*>& vPanelToExport)
{
	CString tempReadName = GetProjectName();
	tempReadName.Replace("��̻��Խ�-","");

	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"����",
		"csv",
		"�Ƴ����-" + tempReadName,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( pathName != "" )
	{
		ParamSettingDlg settings;
		settings.loadSettings();
		TiebiaoEdgeDlg tiebiao;
		tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());

		
		int nComponentInputItemCount = m_vComponentInputItem.size();
		int* aExportCount = new int[nComponentInputItemCount];
		for(int i = 0; i < nComponentInputItemCount; i++)
			aExportCount[i] = 0;

		for(int i = 0; i < vPanelToExport.size(); i++)
		{
			vector<Component*> vAllComponent;
			FindAllComponentInPanel((Component&)(*(vPanelToExport[i])), vAllComponent);
			for(int j = 0; j < vAllComponent.size(); j++)
			{
				int nIndexOfTheComponent = -1;
				for(int k = 0; k < nComponentInputItemCount; k++)
				{
					if(m_vComponentInputItem[k].m_strBarcode.CompareNoCase(vAllComponent[j]->m_BarCode) == 0)
					{
						nIndexOfTheComponent = k;
						break;
					}
				}
				if(nIndexOfTheComponent != -1)
					aExportCount[nIndexOfTheComponent]++;
			}

		}

		//����С������
		std::vector<std::map<std::string,std::string>> resultArr;
		for(int i = 0; i < nComponentInputItemCount; i++)
		{
			if(aExportCount[i] > 0)
			{
				map<std::string,std::string> mapOneExportItem = GetMapByComponentInputItem(m_vComponentInputItem[i]);//m_pDlgRequirement->GetMatrialItemsOne(i);
				CString strNum;
				strNum.Format("%d", aExportCount[i]);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				mapOneExportItem["num"] = strNum;
				mapOneExportItem["pointInfo"] = ""	;
				mapOneExportItem["slotInfo"]	 = ""	;
				mapOneExportItem["dslotInfo"] = ""	;
				mapOneExportItem["vecInfo"] = ""		;
				mapOneExportItem["dvecInfo"] = ""	;
				mapOneExportItem["OtherPoints"] = ""	;
				mapOneExportItem["edgeLen1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen1"].c_str())).c_str();//"�ⳤ1");
				mapOneExportItem["edgeWidth1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth1"].c_str())).c_str();//"���1");
				mapOneExportItem["edgeLen2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen2"].c_str())).c_str();//"�ⳤ2");
				mapOneExportItem["edgeWidth2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth2"].c_str())).c_str();//"���2");

				CString strHoleSlotFlag = mapOneExportItem["DrillingLogo"].c_str();
				if(strHoleSlotFlag.Find(_T("#")) == -1)
				{
					mapOneExportItem["DrillingLogoA"] = "";
					mapOneExportItem["DrillingLogoB"] = "";
				}
				else
				{
					int nPos = strHoleSlotFlag.Find(_T("#"));
					CString strA, strB;
					strA = strHoleSlotFlag.Left(nPos);
					strB = strHoleSlotFlag.Right(strHoleSlotFlag.GetLength() - nPos - 1);

					mapOneExportItem["DrillingLogoA"] = strA.GetBuffer();
					mapOneExportItem["DrillingLogoB"] = strB.GetBuffer();
					strA.ReleaseBuffer();
					strB.ReleaseBuffer();
				}

				resultArr.push_back(mapOneExportItem);
			}

		}

		delete[] aExportCount;

		std::vector<std::string> title;
		for(int i = 0; i < sizeof(g_aMaterialListItem) / sizeof(int); i++)
		{
			title.push_back(g_aRequirementListTitle[g_aMaterialListItem[i]]);
		}
		title.push_back("A����۱�ʶ");
		title.push_back("B����۱�ʶ");
		std::vector<std::string> dataCol;
		for(int i = 0; i < sizeof(g_aMaterialListItem) / sizeof(int); i++)
		{
			dataCol.push_back(g_aRequirementListTitle_ENU[g_aMaterialListItem[i]]);
		}
		dataCol.push_back("DrillingLogoA");
		dataCol.push_back("DrillingLogoB");
		//����EXCEL�Ķ�������excel
		OnWriteDataCSV(pathName, title, dataCol, resultArr);

		MessageBox("�������");
	}
}

void CCarvingView::OnExportMeterialTable1()
{
	// TODO: �ڴ���������������

	//����ѡ�еİ��(�ѵ�̻����ó��ӹ��İ������ϵ�)
	if(m_pDlgResult == NULL)
		return;
	vector<Panel*> vUncheckedPanel = m_pDlgResult->GetPanels(false);
	ExportMeterialTable1(vUncheckedPanel);
}

CString CCarvingView::GetProjectName()
{
	CString strRet;
	if(!m_strOpenedFile.IsEmpty())
		strRet = HGTools::getFileName(m_strOpenedFile);
	return strRet;
}

//дCSV
void CCarvingView::OnWriteDataCSV(CString savePath, std::vector<std::string> title,std::vector<std::string> dataMap, std::vector<std::map<std::string,std::string>> dataSrc)
{
	if (HGTools::isFileExists(savePath))
	{
		HGTools::deleteFile(savePath);
	}


	std::ofstream csv_file(savePath.GetBuffer(), std::ios::out);

	//����
	if (1)
	{
		for (int i = 0; i < title.size() ; i++)
		{
			csv_file << title[i] << ",";
		}
		csv_file << std::endl;
	}

	//����
	if (1)
	{
		for (int i = 0; i < dataSrc.size() ; i++)
		{
			for (int col = 0; col < dataMap.size() ; col++)
			{
				csv_file << dataSrc[i][dataMap[col]] << ",";
			}
			csv_file << std::endl ;
		}
	}
	csv_file.close();
}


void CCarvingView::OnExportMeterialTable2()
{
	// TODO: �ڴ���������������
	//����ѡ�еİ��(�ѵ�̻����ó��ӹ��İ������ϵ�)
	if(m_pDlgResult == NULL)
		return;
	vector<Panel*> vUncheckedPanel = m_pDlgResult->GetPanels(false);
	ExportMeterialTable2(vUncheckedPanel);
}


void CCarvingView::OnOpenSourceExcel()
{
	// TODO: �ڴ���������������
	CSingleon* pSingleton = CSingleon::GetSingleton();
	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CString filter = "xls �ļ�(*.xls)|*.xls|�����ļ� (*.*)|*.*||";

	CFileDialog fileDlg(true, _T("xls"), _T("*.xls"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("���뿪�ϵ�(excel)");
	if ( fileDlg.DoModal() == IDOK)
	{
		m_strOpenedFile = fileDlg.GetPathName();
		ExcelSettingForTheFistTime(m_strOpenedFile);

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xls" || strExtName == "xlsx" )
		{
			GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

			SourceFilePreProccesParam param;
			param.b_upvecImportEnable		= param_dlg->VecInput();			//�����
			param.b_downvecImportEnable		= param_dlg->DVecInput();			//�����
			param.b_upsoltImportEnable		= param_dlg->SoltInput();			//�����
			param.b_downsoltImportEnable	= param_dlg->DSoltInput();			//�����
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();		//���ι���
			param.i_comnum					= param_dlg->Editnum();				//�и�����
			param.i_comchange				= param_dlg->Comchange();			//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
			param.f_changex					= param_dlg->MoveX();				//�ײ�ƫ��
			param.f_changey					= param_dlg->MoveY();				//�ײ�ƫ��	
			param.vecAdder					= param_dlg->GetVecAdder();			//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��		
			param._cxy						= param_dlg->GetVecFilter();		//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
			param.f_width					= param_dlg->Width();
			param.f_height					= param_dlg->Height();
			param.f_Deburringwidth			= param_dlg->Deburringwidth();
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(ExcelReadWrite::ReadPanelInfo(m_strOpenedFile, m_vComponentInputItem, param) == true)
			{

				//// ��������ظ�
				//CheckRepeatBarcode(m_vComponentInputItem);
			}
			else
			{
				if (strExtName == "xlsx")
				{
					AfxMessageBox("����office���������⣬ϵͳ�޷�ʹ��Excel 2003 �Զ���ȡ Excel 2007����� xlsx�ļ�! \n��˫��" + m_strOpenedFile + ",���Ϊxls�ļ����ٵ��룡");
				}

			}


			//CheckComponentList(m_vComponentInputItem);
			// С����Ϊ������
			if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
			{
				SetSmallCpnNoTexture(m_vComponentInputItem);
			}

			for(int i = 0; i < m_vComponentInputItem.size(); i++)
				m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

			ParamSettingDlg dlgOtherSetting;
			dlgOtherSetting.loadSettings();
			ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
			if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
			{
				ModifyReminderByDateAndOrderFormat(m_vComponentInputItem);
			}


			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);

			
			// �Զ������Ż�����
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

			RecordMaterial();
		}

		// �л���ͼ
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// ���ý�������С�����ݽṹ
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem);

	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);


}


void CCarvingView::OnOpenSolution()
{
	// TODO: �ڴ���������������


	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString strTmp;
#ifndef YiWei
	strTmp = _T("hgo");
	CString filter = "hgo �ļ�(*.hgo)|*.hgo|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hgo"), _T("*.hgo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
#else
	strTmp = _T("ywo");
	CString filter = "ywo �ļ�(*.ywo)|*.ywo|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywo"), _T("*.ywo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
#endif
	
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		m_strOpenedFile = fileDlg.GetPathName();

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName.CompareNoCase(strTmp) == 0)
		{
			if(HgyReadWrite::ReadHgy(m_strOpenedFile) != true)
			{
				AfxMessageBox("�Ż��ļ�����!");
			}
			else
			{
				GetDocument()->SetTitle(GetFileNameInPath(m_strOpenedFile));

				m_vComponentInputItem = pSingleton->m_vBackupComponentInputItem;

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
				//CheckComponentList(m_vComponentInputItem);
			}

		}

		// ����С��
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}


		// �Զ������Ż�����
		COPSetDlg dlgOptimizeSetting;

		dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);


		// �л���ͼ
		SwitchView(VIEW_RESULT);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();

		if (m_pDlgRequirement != NULL)
			m_pDlgRequirement->SetRequirement(m_vComponentInputItem);


		// ������������
		pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);
	}
}


void CCarvingView::OnExportPanelMeterialTable()
{
	// TODO: �ڴ���������������
	CString tempReadName = GetProjectName();
	tempReadName.Replace("��̻��Խ�-","");

	SelectPathDlg pathDlg;
	CString savePath = pathDlg.SelectFileDlgDoModel(
		"����",
		"xlsx",
		"���ϵ�-" + tempReadName ,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( savePath != "" )
	{
		//����Ŀ¼
		CString path = HGTools::getContainPath(savePath);


		//////////////////////////////////////////////////////////////////////////
		// ����ͷ
#if 1
		std::vector<std::vector<std::string>> infos;

		//�û���Ϣ
		std::vector<std::string> cusInfo;
		cusInfo.push_back("CompanyLogoUrl");
		cusInfo.push_back("CompanyName");
		cusInfo.push_back("OrderCodeUrl");
		cusInfo.push_back("OrderID");
		cusInfo.push_back("ShiftEmployee");
		cusInfo.push_back("ShiftDate");
		cusInfo.push_back("UseEmployee");
		cusInfo.push_back("UseDate");
		cusInfo.push_back("ManageEmployee");
		cusInfo.push_back("OrderStore");
		cusInfo.push_back("AuditEmployee");
		cusInfo.push_back("UseMachineName");
		cusInfo.push_back("OrderMoreInfo");
		infos.push_back(cusInfo);

		//���ʹ����Ϣ
		std::vector<std::string> dataCol;
		dataCol.push_back("Index");
		dataCol.push_back("Thickness");
		dataCol.push_back("WidthHeight");
		dataCol.push_back("Code");
		dataCol.push_back("PanelMatrial");
		dataCol.push_back("PanelNum");
		dataCol.push_back("Units");
		dataCol.push_back("Brand");
		dataCol.push_back("Remark");
		infos.push_back(dataCol);

		//ռ�������С��
		std::vector<std::string> smallCol;
		smallCol.push_back("PanelMatrial");
		smallCol.push_back("Thickness");
		smallCol.push_back("SmallPanelNum");
		smallCol.push_back("TotalArea");
		infos.push_back(smallCol);


		CString excelFormat = path + "\\excel.xsd" ;
		SaveExcelFormat(excelFormat,infos);

#endif

		LingLiaoDlg lingliao;
		lingliao.loadSettings();

		//////////////////////////////////////////////////////////////////////////
		// ��������
#if 1
		std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs;

		//�û���Ϣ
		std::vector<std::map<std::string,std::string>> cusData;
		std::map<std::string,std::string> cusDataMap;
		cusDataMap["CompanyLogoUrl"] =	"";
		cusDataMap["CompanyName"] =		lingliao.CompanyName().GetBuffer();
		cusDataMap["OrderCodeUrl"] =	"";
		cusDataMap["OrderID"] =			lingliao.OrderID().GetBuffer();
		cusDataMap["ShiftEmployee"] =	lingliao.ShiftEmployee().GetBuffer();
		cusDataMap["ShiftDate"] =		lingliao.ShiftDate().GetBuffer();
		cusDataMap["UseEmployee"] =		lingliao.UseEmployee().GetBuffer();
		cusDataMap["UseDate"] =			lingliao.UseDate().GetBuffer();
		cusDataMap["ManageEmployee"] =	lingliao.ManageEmployee().GetBuffer();
		cusDataMap["OrderStore"] =		lingliao.OrderStore().GetBuffer();
		cusDataMap["AuditEmployee"] =	lingliao.AuditEmployee().GetBuffer();
		cusDataMap["UseMachineName"] =	lingliao.UseMachineName().GetBuffer();
		cusDataMap["OrderMoreInfo"] =	lingliao.OrderMoreInfo().GetBuffer();
		cusData.push_back(cusDataMap);
		dataSrcs.push_back(cusData);

		//���ʹ����Ϣ
		std::vector<std::map<std::string,std::string>> panelInfos;
		std::map<std::string,std::string> panelInfo;


		CString inputvalue;
		//�������д��
		int panelMatrialIndex = 1;
		//for (int i = 0;i<m_picturedlg->_picturecoor.size();i++)
		//{
		//	//����ѡ�еİ��(�ѵ�̻���Ҫ�ӹ��Ĳ����ϵ�����)
		//	if (m_picturedlg->m_piclist.m_panelChecked[i] == false)
		//	{
		//		continue;
		//	}

		//	Panel_Knife panelKnife = m_picturedlg->_picturecoor[i];	//�ҵ���ǰ���
		//	vector<Coordinate> _coors = panelKnife._coor;
		//	KnifeClass* _knifeClass = panelKnife._kc;
		//	PanelNode* rootNode = panelKnife._panel;
		//	vector<gcodeCoor> handcoor = panelKnife._kc->getNewcoor(rootNode);

		//	//�����Ƿ����в���
		//	bool isFindSameMatrial = false;
		//	int find_index = 0;
		//	for (; find_index < panelInfos.size() ; find_index++)
		//	{
		//		if (panelInfos[find_index]["ThinMatrial"].compare( _knifeClass->getThinMatrial() ) == 0)
		//		{
		//			isFindSameMatrial = true;
		//			break;
		//		}
		//	}

		//	//���ֲ���͸����б��еĺĲ�
		//	if (isFindSameMatrial)
		//	{
		//		int panelNum = atoi(panelInfos[find_index]["PanelNum"].c_str());
		//		inputvalue.Format("%d",panelNum + panelKnife._panel->_num);
		//		panelInfos[find_index]["PanelNum"] = inputvalue.GetBuffer();

		//		int smallPanelNum = atoi(panelInfos[find_index]["SmallPanelNum"].c_str());
		//		inputvalue.Format("%d",_knifeClass->getcomnum(rootNode) + smallPanelNum);
		//		panelInfos[find_index]["SmallPanelNum"] = inputvalue.GetBuffer();


		//		float panelArea = atof(panelInfos[find_index]["TotalArea"].c_str());
		//		//�ҵ�����С��
		//		for (int c = 0;c < handcoor.size();c++)
		//		{
		//			panelArea += (handcoor[c]._height / 1000) * (handcoor[c]._width / 1000);
		//		}
		//		inputvalue.Format("%0.2f",panelArea);
		//		panelInfos[find_index]["TotalArea"] = inputvalue.GetBuffer();
		//	}
		//	else
		//	{
		//		inputvalue.Format("%d",panelMatrialIndex++);
		//		panelInfo["Index"] = inputvalue.GetBuffer();

		//		panelInfo["Code"] = "";
		//		panelInfo["Brand"] = "";
		//		panelInfo["Remark"] = "";
		//		panelInfo["Units"] = "��";

		//		panelInfo["ThinMatrial"] = _knifeClass->getThinMatrial();
		//		panelInfo["PanelMatrial"] = _knifeClass->getPanelMatrial();

		//		inputvalue.Format("%0.1f",_knifeClass->getThickness());
		//		panelInfo["Thickness"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%0.1f x %0.1f",_knifeClass->getPanelWidth(),_knifeClass->getPanelHeight());
		//		panelInfo["WidthHeight"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%d",panelKnife._panel->_num);
		//		panelInfo["PanelNum"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%d",_knifeClass->getcomnum(rootNode));
		//		panelInfo["SmallPanelNum"] = inputvalue.GetBuffer();


		//		float panelArea = 0.0;
		//		//�ҵ�����С��
		//		for (int c = 0;c < handcoor.size();c++)
		//		{
		//			panelArea += (handcoor[c]._height / 1000) * (handcoor[c]._width / 1000);
		//		}
		//		inputvalue.Format("%0.2f",panelArea);
		//		panelInfo["TotalArea"] = inputvalue.GetBuffer();

		//		panelInfos.push_back(panelInfo);
		//		panelInfo.clear();
		//	}
		//}

		vector<Panel*> vCheckedPanel = m_pDlgResult->GetPanels(true);
		for(int i = 0; i < vCheckedPanel.size(); i++)
		{
			Panel& thePanel = *(vCheckedPanel[i]);
			CString strThicknessAndMeterial;
			strThicknessAndMeterial.Format(_T("%s-%s"), GetFloatString(thePanel.m_Thickness, 1), thePanel.m_Material);
			//�����Ƿ����в���
			bool isFindSameMatrial = false;
			int find_index = 0;
			for (; find_index < panelInfos.size() ; find_index++)
			{
				
				string stringThicknessAndMeterial = strThicknessAndMeterial;
				if (panelInfos[find_index]["ThinMatrial"].compare(stringThicknessAndMeterial) == 0)
				{
					isFindSameMatrial = true;
					break;
				}
			}

			//���ֲ���͸����б��еĺĲ�
			if (isFindSameMatrial)
			{
				int panelNum = atoi(panelInfos[find_index]["PanelNum"].c_str());
				inputvalue.Format("%d",panelNum + 1);
				panelInfos[find_index]["PanelNum"] = inputvalue.GetBuffer();

				int smallPanelNum = atoi(panelInfos[find_index]["SmallPanelNum"].c_str());
				inputvalue.Format("%d", GetComponentCountInPanel(thePanel) + smallPanelNum);
				panelInfos[find_index]["SmallPanelNum"] = inputvalue.GetBuffer();


				float panelArea = atof(panelInfos[find_index]["TotalArea"].c_str());
				//�ҵ�����С��
				vector<Component*> vAllComponent;
				FindAllComponentInPanel(thePanel, vAllComponent);
				for(int j = 0; j < vAllComponent.size(); j++)
				{
					Component& theComponent = *(vAllComponent[j]);
					panelArea += (theComponent.m_RealLength / 1000) * (theComponent.m_RealWidth / 1000);
				}
				inputvalue.Format("%0.2f",panelArea);
				panelInfos[find_index]["TotalArea"] = inputvalue.GetBuffer();
			}
			else
			{
				panelInfo["Code"] = "";
				panelInfo["Brand"] = "";
				panelInfo["Remark"] = "";
				panelInfo["Units"] = "��";

				panelInfo["ThinMatrial"] = strThicknessAndMeterial;
				panelInfo["PanelMatrial"] = thePanel.m_Material;

				inputvalue.Format("%0.1f", thePanel.m_Thickness);
				panelInfo["Thickness"] = inputvalue.GetBuffer();

				inputvalue.Format("%0.1f x %0.1f",thePanel.m_OrgLen, thePanel.m_OrgWidth);
				panelInfo["WidthHeight"] = inputvalue.GetBuffer();

				inputvalue.Format("%d", 1);
				panelInfo["PanelNum"] = inputvalue.GetBuffer();

				inputvalue.Format("%d", GetComponentCountInPanel(thePanel));
				panelInfo["SmallPanelNum"] = inputvalue.GetBuffer();


				float panelArea = 0.0;
				//�ҵ�����С��
				vector<Component*> vAllComponent;
				FindAllComponentInPanel(thePanel, vAllComponent);
				for(int j = 0; j < vAllComponent.size(); j++)
				{
					Component& theComponent = *(vAllComponent[j]);
					panelArea += (theComponent.m_RealLength / 1000) * (theComponent.m_RealWidth / 1000);
				}
				inputvalue.Format("%0.2f",panelArea);
				panelInfo["TotalArea"] = inputvalue.GetBuffer();

				panelInfos.push_back(panelInfo);
				panelInfo.clear();
			}
		}

		ParamSettingDlg dlgOtherSetting;
		dlgOtherSetting.loadSettings();
		LingLiaoSortType eLingLiaoSortType = dlgOtherSetting.LingLiaoSort();
		if(eLingLiaoSortType == LingLiao_SORT_THICKNESS_FIRST)
			std::sort(panelInfos.begin(), panelInfos.end(), SortMaterialListData);

		for(int i = 0; i < panelInfos.size(); i++)
			panelInfos[i]["Index"] = GetFloatString(i+1, 0).GetBuffer();

		dataSrcs.push_back(panelInfos);
		dataSrcs.push_back(panelInfos);

		CString excelData = path + "\\excel.xml" ;
		SaveExcelData(excelData, infos, dataSrcs);
#endif

		//��������
#if 1

#endif

		//�������
#if 1
		// �����ⲿ�����ָ���� report ����Щ����
		CString ExcelExportCmdPath = HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("report\\ExcelExportCmd.exe"));
		CString ReportPath = HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("report\\lingliaobiao.frx"));

		CString Param;
		//Param.Format("preview \"%s\" \"%s\"",ReportPath,excelData);
		Param.Format("excel \"%s\" \"%s\" \"%s\"",ReportPath,excelData,savePath);
		if (HGTools::isFileExists(ExcelExportCmdPath)
			&& HGTools::isFileExists(ReportPath)
			&& HGTools::isFileExists(excelData))
		{
			ShellExecute(NULL,"open",ExcelExportCmdPath,Param,NULL,SW_HIDE);
			MessageBox("�������");
		}
		else
		{
			MessageBox("���ɹ��߶�ʧ");
		}
#endif
		HGTools::deleteFile(excelFormat);
		HGTools::deleteFile(excelData);
	}
}

void CCarvingView::SaveExcelFormat(CString savePath, std::vector<std::vector<std::string>> infos)
{
	//////////////////////////////////////////////////////////////////////////
	/* //�����ͷschema��ʽ
	<?xml version="1.0" encoding="GB2312" ?>
	<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
		targetNamespace="http://www.techtimesun.com"
		xmlns="http://www.techtimesun.com"
		elementFormDefault="unqualified">
		<xs:element name="data">
			<xs:complexType>
				<xs:sequence>
					<xs:element name="item" maxOccurs="unbounded">
						<xs:complexType>
							<xs:all>
								<xs:element name="order" type="xs:string" minOccurs="0"/>
								<xs:element name="cabinet" type="xs:string" minOccurs="0"/>
								<xs:element name="info" type="xs:string" minOccurs="0"/>
								<xs:element name="PorductName" type="xs:string" minOccurs="0"/>
							</xs:all>
						</xs:complexType>
					</xs:element>
				</xs:sequence>
			</xs:complexType>
		</xs:element>
	</xs:schema>
	*/
	TinyXml::TiXmlDocument docSchema;
	docSchema.Parse(
		"<?xml version=\"1.0\" encoding=\"GB2312\" ?>"
		"<xs:schema xmlns:xs=\"http://www.w3.org\/2001\/XMLSchema\" "
		"targetNamespace=\"http://www.techtimesun.com\" "
		"xmlns=\"\http://www.techtimesun.com\" "
		"elementFormDefault=\"unqualified\">"
		"<xs:element name=\"data\">"
		"<xs:complexType>"
		"<xs:sequence>"
		"</xs:sequence>"
		"</xs:complexType>"
		"</xs:element>"
		"</xs:schema>"
		);
	//TinyXml::XmlHandlePlus handleSchema(&docSchema);
	//handleSchema.changeDirectoryTo("/xs:schema/xs:element/xs:complexType/xs:sequence");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = docSchema.RootElement();
	TiXmlElement* elmTmp = xmlHandler.GetChildElm(elmRoot, "xs:element");
	elmTmp = xmlHandler.GetChildElm(elmTmp, "xs:complexType");
	elmTmp = xmlHandler.GetChildElm(elmTmp, "xs:sequence");
	{
		for (int info_index = 0; info_index < infos.size() ; info_index++)
		{
			std::vector<std::string> info = infos[info_index];
			CString infoName;
			infoName.Format("info_%d",info_index);
			TinyXml::TiXmlElement element("xs:element");
			element.SetAttribute("name",infoName.GetBuffer());
			element.SetAttribute("maxOccurs","unbounded");
			{
				TinyXml::TiXmlElement complexType("xs:complexType");
				{
					TinyXml::TiXmlElement all("xs:all");
					{
						for (int i = 0; i < info.size() ; i++)
						{
							TinyXml::TiXmlElement element("xs:element");
							element.SetAttribute("name",info[i].c_str());
							element.SetAttribute("type","xs:string");
							element.SetAttribute("minOccurs","0");
							all.InsertEndChild(element);
						}
					}
					complexType.InsertEndChild(all);
				}
				element.InsertEndChild(complexType);
			}
			//handleSchema.Node()->InsertEndChild(element);
			elmTmp->InsertEndChild(element);
		}
	}
	docSchema.SaveFile(savePath.GetBuffer());
}

void CCarvingView::SaveExcelData(CString savePath, std::vector<std::vector<std::string>> infos,std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs)
{
	//////////////////////////////////////////////////////////////////////////
	/* //��������Ϊxml��ʽ
	<?xml version="1.0" encoding="GB2312" ?>
	<data xmlns="http://www.techtimesun.com"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.techtimesun.com excel.xsd">
		<item>
			<order>1</order>
			<cabinet>11</cabinet>
			<info>111</info>
			<PorductName>1111</PorductName>
		</item>
		<item>
			<order>2</order>
			<cabinet>22</cabinet>
			<info>222</info>
			<PorductName>2222</PorductName>
		</item>
		<item>
			<order>3</order>
			<cabinet>33</cabinet>
			<info>333</info>
			<PorductName>3333</PorductName>
		</item>
	</data>
	*/
	TinyXml::TiXmlDocument docXmlData;
	docXmlData.Parse(
		"<?xml version=\"1.0\" encoding=\"GB2312\" ?>"
		"<data xmlns=\"http://www.techtimesun.com\" "
		"	xmlns:xsi=\"http://www.w3.org\/2001\/XMLSchema-instance\" "
		"	xsi:schemaLocation=\"http://www.techtimesun.com excel.xsd\"> "
		"</data>"
		);
	//TinyXml::XmlHandlePlus handleData(&docXmlData);
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = docXmlData.RootElement();
	//handleData.changeDirectoryTo("/data");
	{
		for (int dataIndex = 0; dataIndex < dataSrcs.size() ; dataIndex++)
		{
			std::vector<std::string> info = infos[dataIndex];
			std::vector<std::map<std::string,std::string>> dataSrc = dataSrcs[dataIndex];

			for (int i = 0; i < dataSrc.size() ; i++)
			{
				//���￪ʼ�ж��
				CString infoName;
				infoName.Format("info_%d",dataIndex);
				TinyXml::TiXmlElement item(infoName.GetBuffer());
				{
					for (int col = 0; col < info.size() ; col++)
					{
						TinyXml::TiXmlElement colitem(info[col].c_str());
						{
							TinyXml::TiXmlText text(dataSrc[i][ info[col] ].c_str());
							colitem.InsertEndChild(text);
						}
						item.InsertEndChild(colitem);
					}
				}
				//handleData.Node()->InsertEndChild(item);
				elmRoot->InsertEndChild(item);
			}
		}
	}
	docXmlData.SaveFile(savePath.GetBuffer());
}



/*---------------------------------------*/
//	����˵����
//		����С��������
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void CCarvingView::SetSmallCpnNoTexture(vector<ComponentInputItem>& vComponentInputItem)
{
	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	//float small_len		= _ttof(param_dlg->m_minw);			// ��С������ - ��С����
	//float small_width	= _ttof(param_dlg->m_minh);			// ��С������ - ��С���
	//float small_Area	= _ttof(param_dlg->m_minsize);		// ��С������ - ��С���



	float small_h_len1		= param_dlg->m_valHMinLen1;				// ��С����1
	float small_h_width1	= param_dlg->m_valHMinWidth1;			// ��С���1
	float small_w_len2		= param_dlg->m_valVMinLen2;				// ��С����2
	float small_w_width2	= param_dlg->m_valVMinWidth2;			// ��С���2

	for(vector<ComponentInputItem>::iterator it = vComponentInputItem.begin(); it != vComponentInputItem.end(); it++)
	{
		ComponentInputItem& cpn = *it;

		float cpn_len		= 	cpn.m_fLength;
		float cpn_width		=	cpn.m_fWidth;
		
		// ����С�� ��Ϊ������
		if (cpn.m_strTexture == "����" && cpn_len	<= small_h_len1 && cpn_width <= small_h_width1)
		{
			cpn.m_strTexture = "������";
		}

		// ����С�� ��Ϊ������
		if (cpn.m_strTexture == "����" && cpn_len	<= small_w_len2 && cpn_width <= small_w_width2)
		{
			cpn.m_strTexture = "������";
		}

	}
}


/*---------------------------------------*/
//	����˵����
//		�������С�Ƿ񳬳������泬�����
//
//
//	������
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
//void CCarvingView::CheckAndWarnOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
//{
//	CSingleon* pSingleton = CSingleon::GetSingleton();
//	BaseInfo base_info = pSingleton->m_BaseInfo;
//	vector<ComponentInputItem>::iterator it, it_begin, it_end;
//	CString strMsg;
//
//	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();it++)
//	{
//		ComponentInputItem& pCpn = *it;
//
//		if (pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth)
//		{
//			if (pCpn.m_strTexture == "������")
//			{
//				// ��ת���ٴ��ж�
//				if (pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth)
//				{
//					// ����
//					strMsg += "������Χ���������ţ�" + pCpn.m_strBarcode + "\n";
//
//				}
//			}
//			else
//			{
//
//				// ����
//				strMsg += "������Χ���������ţ�" + pCpn.m_strBarcode + "\n";
//
//
//			}
//		}
//		else if (pCpn.m_strTexture == "����" &&
//			(pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth))
//		{
//			// ����
//			strMsg += "������Χ���������ţ�" + pCpn.m_strBarcode + "\n";
//		}
//
//	}
//
//	// ����Ϣ
//	if (strMsg.IsEmpty() != true)
//	{
//		AfxMessageBox(strMsg);
//	}
//
//}

/*---------------------------------------*/
//	����˵����
//		�������С�Ƿ񳬳���ɾ���������
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void CCarvingView::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "������")
		{
			if(pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ��ת���ٴ��ж�
				if (pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
					|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
					|| pCpn.m_fLength <= 0
					|| pCpn.m_fWidth <= 0)
				{
					// ���ǳ�����ɾ��
					bOverSize = true;
				}
			}
		}
		else if(pCpn.m_strTexture == "����")
		{
			if (pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// ����
			strMsg += "ɾ��������Χ���������ţ�" + pCpn.m_strBarcode + "\n";

			// ɾ��
			it = vComponentInputItem.erase(it);
		}
		else
		{
			it++;
		}
	}

	// ����Ϣ
	if (strMsg.IsEmpty() != true)
	{
		AfxMessageBox(strMsg);
	}


}

bool CCarvingView::CheckDeepHoleSlot(vector<ComponentInputItem>& vComponent)
{
	bool bCancelOptimize = false;

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	bool bNoDeepHole = true;
	for(int i = 0; i < vComponent.size(); i++)
	{
		if(IsVHoleDeeperThanThickness(vComponent[i]))
		{
			bNoDeepHole = false;
			break;
		}
	}
	bool bNoDeepSlot = true;
	for(int i = 0; i < vComponent.size(); i++)
	{
		if(IsSlotDeeperThanThickness(vComponent[i]))
		{
			bNoDeepSlot = false;
			break;
		}
	}

	if(!bNoDeepHole || !bNoDeepSlot)
	{
		if(MessageBox(_T("�������ڰ��Ĵ�ֱ��/�ۣ��Ƿ�����Ż���"), NULL, MB_YESNO) != IDYES)
			bCancelOptimize = true;
	}

	return !bCancelOptimize;
}

// ��ȡG�������
GcodeParam GetGcodeParam()
{
	

	float _CutSpeech;			//�����ٶ�
	float _ProSpeech;			//�µ��ٶ�
	float _EndSpeech;			//��β�ٶ�
	float _OverSpeech;			//�����ٶ�
	float _distance1;
	float _distance2;
	float _upHeight;
	float _downHeight;
	std::string _strRinget;

	GcodeParam gcodeparam;

	gcodeparam.cutspeech = _CutSpeech;
	gcodeparam.prospeech = _ProSpeech;
	gcodeparam.overspeech = _OverSpeech;
	gcodeparam.distance1 = _distance1;
	gcodeparam.distance2 = _distance2;
	CString s_ender1;
	CString s_header1;

	{
		CIniFile inifile;
		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_name = ExeFile;
		file_name = file_name.Left(file_name.ReverseFind('\\'));
		inifile.SetFilePath(file_name);
		inifile.SetFileName(_T("Knife.ini"));
		inifile.OpenIniFileForRead(); 

		inifile.GetItemString(_T("Machine"),_T("header"),s_header1);
		inifile.GetItemString(_T("Machine"),_T("ender"),s_ender1);
	}

	gcodeparam.ender = s_ender1.GetBuffer();
	gcodeparam.header = s_header1.GetBuffer();

	gcodeparam.upheight = _upHeight;
	gcodeparam.downheight = _downHeight;
	gcodeparam.endspeech = _EndSpeech;
	gcodeparam.strRinget = _strRinget;

	return gcodeparam;
}







LRESULT CCarvingView::OnExportMaterialList(WPARAM wparam, LPARAM lparam)
{
	if(wparam == 0)
	{
		if(lparam == 0)
			OnExportMeterialTable1();
		else if (lparam == 1)
		{
			PanelViewingParam* pParam = m_pDlgResult->GetSelectedItemViewingParam();
			if(pParam && pParam->m_pPanel)
			{
				vector<Panel*> vExportPanel;
				vExportPanel.push_back(pParam->m_pPanel);
				ExportMeterialTable1(vExportPanel);
			}
		}
	}
	else if(wparam == 1)
	{
		if(lparam == 0)
			OnExportMeterialTable2();
		else if (lparam == 1)
		{
			PanelViewingParam* pParam = m_pDlgResult->GetSelectedItemViewingParam();
			if(pParam && pParam->m_pPanel)
			{
				vector<Panel*> vExportPanel;
				vExportPanel.push_back(pParam->m_pPanel);
				ExportMeterialTable2(vExportPanel);
			}
		}
	}

	return 0;
}

void CCarvingView::OnButtonAboutUs()
{
	// TODO: �ڴ���������������
	ShellExecute( \
		NULL, \
		_T("open"), \
		g_szCompanyWebSide, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}


void CCarvingView::OnButtonOurCustomers()
{
	// TODO: �ڴ���������������
}

void CCarvingView::OnButtonContackWithUsQQ()
{
	// TODO: �ڴ���������������
	CString strTmp = _T("tencent://AddContact/?fromId=50&fromSubId=1&subcmd=all&uin=%s");
	CString strCommand;
	strCommand.Format(strTmp, g_szCustomerServiceQQ);
	
	ShellExecute( \
		NULL, \
		_T("open"), \
		strCommand, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}

void CCarvingView::OnButtonContackWithUsWeiXin()
{
	// TODO: �ڴ���������������
	CDlgWeiXin dlg;
	dlg.DoModal();
}

void CCarvingView::OnButtonTutorial()
{
	// TODO: �ڴ���������������
	CString sHelp = HGTools::getCurExeContainPath() + "\\Help.chm";
	if (HGTools::isFileExists(sHelp))
	{
		ShellExecute(NULL, "open", sHelp, NULL, NULL, SW_SHOWMAXIMIZED);
	}
}

void CCarvingView::OnButtonVideo()
{
	// TODO: �ڴ���������������
	CString strLink = _T("http://www.techtimesun.com/list-166-1.html");
	ShellExecute( \
		NULL, \
		_T("open"), \
		strLink, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}

void CCarvingView::OnButtonOpenTemplate()
{
	// TODO: �ڴ���������������
	CString strTemplate = HGTools::getCurExeContainPath() + "\\���ϴ�ʦģ��.xls";
	if (HGTools::isFileExists(strTemplate))
	{
		ShellExecute(this->m_hWnd,"Open",strTemplate,"","",SW_SHOW);
	}
}

void CCarvingView::ClearAllData()
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	if (m_pDlgResult != NULL)
		m_pDlgResult->ResetResultDlg();
}

void CCarvingView::OnMenuSaveAllSettings()
{
	// TODO: �ڴ���������������

#ifndef YiWei
	CString backup_path = HGTools::getRelativePath("kdata\\�������������ļ�.bat");
#else
	CString backup_path = HGTools::getRelativePath("kdata\\YW_�������������ļ�.bat");
#endif
	

	//AfxMessageBox(backup_path);

	system("\"" + backup_path + "\"");
	
	AfxMessageBox("���������ļ���ɣ�");

}

void CCarvingView::OnMenuRevertAllSettings()
{
	// TODO: �ڴ���������������
#ifndef YiWei
	CString recover_path = HGTools::getRelativePath("kdata\\�ָ����������ļ�.bat");
#else
	CString recover_path = HGTools::getRelativePath("kdata\\YW_�ָ����������ļ�.bat");
#endif

	system("\"" + recover_path + "\"");
	AfxMessageBox("�ָ������ļ���ɣ�");
}

void CCarvingView::RecordMaterial()
{
	vector<CString> vRecordedMaterial;

	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	TiXmlElement * elmHistoryMaterial = NULL;
	TiXmlDocument * xmlDoc = new TiXmlDocument();
	if(access(strTemporaryRecordFile, 0) != 0)
	{
		TiXmlElement* elmRoot = new TiXmlElement("Root");
		xmlDoc->LinkEndChild(elmRoot);
		elmHistoryMaterial = new TiXmlElement("HistoryMaterial");
		elmRoot->LinkEndChild(elmHistoryMaterial);

	}
	else
	{
		bool bLoadRes = xmlDoc->LoadFile(strTemporaryRecordFile);
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			elmHistoryMaterial = xmlHandler.GetChildElm(xmlDoc->RootElement(), "HistoryMaterial");
			if(!elmHistoryMaterial)
			{
				elmHistoryMaterial = new TiXmlElement("HistoryMaterial");
				 xmlDoc->RootElement()->LinkEndChild(elmHistoryMaterial);
			}
			else
			{
				vector<TiXmlElement*> vPreExistItem = xmlHandler.GetChildElms(elmHistoryMaterial, "Item");
				for(int i = 0; i < vPreExistItem.size(); i++)
				{
					CString strMaterial;
					xmlHandler.GetXmlAttribute(vPreExistItem[i], "Name", strMaterial);
					vRecordedMaterial.push_back(strMaterial);
				}
			}
		}
		else
		{
			delete xmlDoc;
			return;
		}
	}

	for(int i = 0; i < m_vComponentInputItem.size(); i++)
	{
		CString strMaterial = m_vComponentInputItem[i].m_strMaterial;
		bool bNeedToInsert = true;
		for(int j = 0; j < vRecordedMaterial.size(); j++)
		{
			if(strMaterial.Compare(vRecordedMaterial[j]) == 0)
			{
				bNeedToInsert = false;
				break;
			}
		}
		if(bNeedToInsert)
		{
			TiXmlElement* elmItem = new TiXmlElement("Item");
			elmHistoryMaterial->LinkEndChild(elmItem);
			elmItem->SetAttribute("Name", strMaterial.GetBuffer());
			strMaterial.ReleaseBuffer();
			vRecordedMaterial.push_back(strMaterial);
		}
	}

	xmlDoc->SaveFile(strTemporaryRecordFile);
	delete xmlDoc;
}

void ExcelSettingForTheFistTime(CString strFilePath)
{
	bool bImportedExcel = false;

	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	if(access(strTemporaryRecordFile, 0) == 0)
	{
		TinyXml::TiXmlDocument doc(strTemporaryRecordFile);
		bool bLoadRes = doc.LoadFile();
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			TiXmlElement* elmUsingState = xmlHandler.GetChildElm(doc.RootElement(), "UsingState");
			xmlHandler.GetXmlAttribute(elmUsingState, "ImportedExcel", bImportedExcel);
		}
	}
	if(!bImportedExcel)
	{
		//_T("��������һ�δ�Excel����뿪�����ݰɣ�\n�Ƿ���ȥ����һ��Excel�������ĸ���������������迪������֮��Ķ�Ӧ��ϵ��");
		CString strPrompt = _T("�Ƿ���Ҫ����Excel�����ʽ��\n��������ͨ�������������á�->��ģ�����á��������á�");
		if(AfxMessageBox(strPrompt, MB_YESNO) == IDYES)
		{
			CDlgExcelInputSetting dlg(strFilePath);
			dlg.DoModal();
		}
	}

	if(access(strTemporaryRecordFile, 0) != 0)
	{
		TiXmlDocument * xmlDoc = new TiXmlDocument();

		TiXmlElement * elmRoot = new TiXmlElement("Root");
		xmlDoc->LinkEndChild(elmRoot);
		TiXmlElement * elmUsingState = new TiXmlElement("UsingState");
		elmRoot->LinkEndChild(elmUsingState);
		elmUsingState->SetAttribute("ImportedExcel", "1");

		xmlDoc->SaveFile(strTemporaryRecordFile);
		delete xmlDoc;
	}
	else
	{
		TinyXml::TiXmlDocument doc(strTemporaryRecordFile);
		bool bLoadRes = doc.LoadFile();
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			xmlHandler.SetElmAttr(doc.RootElement(), "UsingState", "ImportedExcel", "1");
		}
	}
}