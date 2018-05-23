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

// CarvingView.h : CCarvingView ��Ľӿ�
//

#pragma once

#include "./UI/UICommon/TabBar.h"
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../include/KnifeDll/KnifeClass.h"
#include <map>

class CDlgRemainder;
class CDlgRequirement;
class CDlgResult;
class Panel;

struct PrepareExportGCodeData;
struct PrepareKnifeInfoData;
struct TransCoordinateData;
struct CombineFileData;

class CCarvingView : public CFormView
{
	enum ViewType{VIEW_REMAINDER, VIEW_PANEL_LIST, VIEW_RESULT};
	enum { IDD = IDD_INSTANCE };
protected: // �������л�����
	CCarvingView();
	DECLARE_DYNCREATE(CCarvingView)

// ����
public:
	CCarvingDoc* GetDocument() const;

// ����
public:
	vector<ComponentInputItem>& GetAllComponentInputItem(){return m_vComponentInputItem;}

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
public:
	virtual ~CCarvingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	//���G�����
	GcodeParam GetGcodeParam();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnOpenSourceHG();
	afx_msg void OnOpenSourceHGAppend();
	afx_msg void OnOpenSourceExcelAppend();
	afx_msg void OnOptionsPen();
	virtual void OnInitialUpdate();
	afx_msg void OnViewRemainder();
	afx_msg void OnViewComponent();
	afx_msg void OnViewSolution();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptimize();
	afx_msg void OnSave();
	afx_msg void OnEditPragmaSawKerf();
	afx_msg void OnEditPanelLength();
	afx_msg void OnEditPanelWidth();
	afx_msg void OnEditStep1Count();
	afx_msg void OnEditStep2Count();
	afx_msg void OnEditStep3Count();
	afx_msg void OnEditStep3AcceptableUti();
	afx_msg void OnComboStep1Alg();
	afx_msg void OnExportGcode();
	DECLARE_MESSAGE_MAP()

	void SwitchView(ViewType eViewType);
	CRect GetViewRect();
	CString GetProjectName();
	void ExportMeterialTable1(vector<Panel*>& vPanelToExport);
	void ExportMeterialTable2(vector<Panel*>& vPanelToExport);
	void ClearAllData();
	void CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);		// ��鲢ɾ�������ߴ�İ��
	//void CheckAndWarnOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);		// ��鲢���泬���ߴ�İ��
	void SetSmallCpnNoTexture(vector<ComponentInputItem>& vComponentInputItem);						// ����С��������
	bool CheckDeepHoleSlot(vector<ComponentInputItem>& vComponent);
	BOOL IsRepeatBarcode(vector<ComponentInputItem>& src_list, vector<ComponentInputItem>& add_list);
	BOOL IsRepeatBarcode(vector<ComponentInputItem>& src_list);
	void RecordMaterial();



	//����CSV��ʽ
	void OnWriteDataCSV(CString savePath, std::vector<std::string> title,std::vector<std::string> dataMap, std::vector<std::map<std::string,std::string>> dataSrc);
	//����excelʹ�õ�����Դxml��ʽ
	void SaveExcelFormat(CString savePath, std::vector<std::vector<std::string>> infos);
	//����excelʹ�õ�����Դxml����
	void SaveExcelData(CString savePath, std::vector<std::vector<std::string>> infos,std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs);

	CTabBar m_Tab;
	CDlgRemainder* m_pDlgRemainder;
	CDlgRequirement* m_pDlgRequirement;
	CDlgResult* m_pDlgResult;
	ViewType m_eCurViewType;

	vector<ComponentInputItem> m_vComponentInputItem;
	CString m_strOpenedFile;

	void DeleteSpecialCode(std::string &_code);
	void DeleteSpecialCode2(std::string &_code);

	bool eachLineAddChar(std::string patho,CString word);
	std::string ReplaceWithNextPath(std::string _code,std::string theNextPath);//�滻*<>
	//ˢ��NC�ļ��������� G0X ��Ϊ GX
	void Change_G0X_To_GX(std::string fileName);
	//ˢ��NC�ļ������� X *** Y *** Z *** F *** -> X*** Y*** Z*** F***
	void Change_PosSpace_To_Empty(std::string fileName);
	//ˢ��NC�ļ����޸�ϳ����ʽ,����G90 G03 X*** Y*** R***����ʽ����Ϊ��G03 X*** Y*** U***��
	void Change_Arc_Milling_Format(std::string fileName);

	bool PrepareExportGCode(PrepareExportGCodeData* pPrepareExportGCodeData);
	void PrepareKnifeInfoAndHoleSetting(PrepareExportGCodeData* pPrepareExportGCodeData, PrepareKnifeInfoData* pPrepareKnifeInfoData);
	void AfterExportGCode_TransCoordinate(PrepareExportGCodeData* pPrepareExportGCodeData, TransCoordinateData* pTransCoordinateData);
	void AfterExportGCode_CombineFile(PrepareExportGCodeData* pPrepareExportGCodeData, CombineFileData* pCombineFileData);
	int ExportComponentNC(vector<Panel*>& vComponentPanel);

	float _trim1;//�����ޱ�ֵ
	float _trim2;
	float _trim3;
	float _trim4;

	float _ftrim1;//�����ޱ�ֵ
	float _ftrim2;
	float _ftrim3;
	float _ftrim4;

	int _iscombine;//�Ƿ�ϲ�����ͷβ

	std::string _header;//���ļ�ͷGFirst1
	std::string _ender;//���ļ�βGLast1
	std::string _GFirst2;//���ļ�ͷGFirst2
	std::string _GLast2;//���ļ�βGLast2

	std::string _GBFirst1;//��B�ļ�ͷGFirst1
	std::string _GBFirst2;//���ļ�βGLast1
	std::string _GBLast1;//��B�ļ�WEI
	std::string _GBLast2;//���ļ�βGLast2

	std::string _MFirst1;//�����ļ�ͷGFirst1
	std::string _MLast1;//�����ļ�βGLast1
	std::string _MFirst2;//�����ļ�ͷGFirst2
	std::string _MLast2;//�����ļ�βGLast2

	std::string _PZFirst1;//�����ļ�ͷGFirst1
	std::string _PZLast1;//�����ļ�βGLast1
	std::string _PZFirst2;//�����ļ�ͷGFirst2
	std::string _PZLast2;//�����ļ�βGLast2

	CString NCSavePath;


	//CString m_strProjectName;

	//void TMP_ConvertInputItemToOldInputItem(vector<map<string,string>>& vItem);

public:
	afx_msg void OnLayoutOrg();
	afx_msg void OnButtonPrintLabel();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnMenuMachineParamSetting();
	afx_msg void OnMenuBaseSetting();
	afx_msg void OnMenuOptimizeSetting();
	afx_msg void OnRemainderSetting();
	afx_msg void OnMenuDefaultLoadingDirSetting();
	afx_msg void OnMenuDefaultSavingDirSetting();
	afx_msg void OnMenuOtherSetting();
	afx_msg void OnMenuLabelSetting();
	afx_msg void OnMenuExcelTemplateSetting();
	afx_msg void OnMenuLingliaoTableSetting();
	afx_msg void OnExportMeterialTable1();
	afx_msg void OnExportMeterialTable2();
	afx_msg void OnOpenSourceExcel();
	afx_msg void OnOpenSolution();
	afx_msg void OnExportPanelMeterialTable();
	afx_msg LRESULT OnExportMaterialList(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnExportComponentNC(WPARAM wparam, LPARAM lparam);
	afx_msg void OnButtonAboutUs();
	afx_msg void OnButtonOurCustomers();
	afx_msg void OnButtonContackWithUsQQ();
	afx_msg void OnButtonContackWithUsWeiXin();
	afx_msg void OnButtonTutorial();
	afx_msg void OnButtonVideo();
	afx_msg void OnButtonOpenTemplate();
	afx_msg void OnMenuSaveAllSettings();
	afx_msg void OnMenuRevertAllSettings();


	static DWORD WINAPI LoginStatisticsDailyThread( PVOID lpThreadParameter );				// �ճ���½ͳ���̺߳���

};

#ifndef _DEBUG  // CarvingView.cpp �еĵ��԰汾
inline CCarvingDoc* CCarvingView::GetDocument() const
   { return reinterpret_cast<CCarvingDoc*>(m_pDocument); }
#endif

