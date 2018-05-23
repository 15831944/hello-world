#pragma once
#include "afxcmn.h"
#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
																
// CPanelListDlg �Ի���										 

class CPanelListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPanelListDlg)

public:
	CPanelListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPanelListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PANEL_LIST };

	void SetPanelInfo(vector<ComponentInputItem>& vComponentInputItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

protected:
	CListCtrl m_lcPanelList;

};
