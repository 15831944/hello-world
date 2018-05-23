#pragma once
#include "../Resource.h"
#include <vector>
#include "./UICommon/SkinListCtrlBase.h"
#include "SkinHeaderCtrl_ExcelSetting.h"

using namespace std;

// CDlgExcelInputSetting �Ի���

class CDlgExcelInputSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgExcelInputSetting)

public:
	CDlgExcelInputSetting(LPCTSTR szFilePath, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExcelInputSetting();

// �Ի�������
	enum { IDD = IDD_DLG_EXCEL_INPUT_SETTING };

	enum Row1Type{ROW1_AS_NAME, ROW1_AS_DATA};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio_Row1AsName();
	afx_msg void OnBnClickedRadio_Row1AsData();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHdnItemChanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	Row1Type GetCurRow1Type();
	void SwitchRow1Type(Row1Type eRow1Type);
	void ArrangeUI();

	//vector<CString> m_vExcelColumn;
	vector<vector<CString>> m_vAllRow;
	CSkinListCtrlBase m_lcDataMapping;
	CSkinHeaderCtrl_ExcelSetting m_hcDataMapping;
	CString m_strFilePath;
};
