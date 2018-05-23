#pragma once
#include "afxcmn.h"
#include "ListRequirement.h"
#include "SkinHeaderCtrl.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include <map>
#include "./UICommon/DialogChildBase.h"
#include "ListBoxPreCombine.h"
#include "../UIData/UIData.h"

using namespace std;

class CDlgRequirement_PreCombine;
// CDlgRequirement �Ի���

class CDlgRequirement : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgRequirement)

public:
	CDlgRequirement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRequirement();

// �Ի�������
	enum { IDD = IDD_DIALOG_PANEL_LIST };


	//�ӽ�����ϻ��1��
	std::map<std::string,std::string> GetMatrialItemsOne( int index );
	vector<PreCombineItem> GetPreCombineItems();
	void SetRequirement(vector<ComponentInputItem>& vComponentInputItem, bool bResetPreCombine = true);
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickRequirmentList(NMHDR *pNMHDR, LRESULT *pResult);		//����һ�
	afx_msg void OnOtherShapeSetting();	//���ΰ�����
	afx_msg void OnDelOtherShape();		//ɾ�����ΰ�
	afx_msg void OnDeleteslot();		//ɾ����
	afx_msg void OnSlot();				//������
	afx_msg void OnSetdir();			//��������
	afx_msg void OnSetMatrial();		//���ò���
	afx_msg void OnEditMatrialList();	//�༭���г�Ʒ
	afx_msg void OnDelMatrialList();
	afx_msg void OnSelfOthershap();
	afx_msg void OnSetReminder();
	afx_msg void OnResetHoleInfo();
	afx_msg void OnPreCombine();
	afx_msg void OnExportComponentNC();
	afx_msg void OnNMDblclkListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitemListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnShowOrHidePreCombine(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//���ý������1��
	void SetMatrialItemsOne( int i,std::map<std::string,std::string> noProcessSmallPanelOne );
	bool SetOneItemTextAndData(int nIndex, ComponentInputItem& theComponentInputItem);

	void ReadSlotTemplate(std::string &Slotx,std::string &Sloty,std::string &SlotWidth,std::string &SlotDepth);
	//���ѡ�����
	std::vector<int> getSelectIndexFromMatrialList();
	ComponentInputItem* GetComponentInputItemByID(int nItem);

	void RefreshListCtrl();

	void OnResetMachiningInfo(int nType);

	void AddPreCombine(vector<CString> vPreCombineBarcode);
	void ShowOrHidePreCombineListBox();
	void ArrangeUI();

	void SetPanelInfo(vector<ComponentInputItem>& vComponentInputItem);

	void SelectAll();

	void RefreshAllItemState();
	void CheckDuplicatedBarcode(vector<vector<int>>& vDuplicatedBarcodeItem);
protected:
	CListRequirement m_lcRequirement;
	CSkinHeaderCtrl m_hcRequirement;
	vector<ComponentInputItem>* m_pvComponentInputItem;
	//CDlgRequirement_PreCombine* m_pDlgPreCombine;

	int m_eSortOrderType;
	int m_nSortintSubItemID;
	Image* m_pImgSortAscending;
	Image* m_pImgSortDescending;

	CListBoxPreCombine m_lbPreCombine;
};
