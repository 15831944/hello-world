
// ElectricSawDlg.h : ͷ�ļ�
//

#pragma once

#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"

class CPanelListDlg;
class CResultDlg;
// CElectricSawDlg �Ի���
class CElectricSawDlg : public CDialogEx
{
// ����
public:
	CElectricSawDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ELECTRICSAW_DIALOG };
	enum ViewType{VIEW_PANEL_LIST, VIEW_RESULT};
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CRect GetViewRect();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonOptimize();

protected:
	CString m_strPanelInfoFile;
	CPanelListDlg* m_pPanelListDlg;
	CResultDlg* m_pResultDlg;
	ViewType m_eCurViewType;

	vector<ComponentInputItem> m_vComponentInputItem;
	//CSingleon m_data;
public:
	afx_msg void OnBnClickedButtonPanelList();
	afx_msg void OnBnClickedButtonResult();
	void SwitchView(ViewType eViewType);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_valFirstSectionOPTimes;
	CComboBox m_cbxFirstSectionOPMethod;
	float m_valPanelLength;
	float m_valPanelWidth;
	float m_valKerfWidth;
	int m_valThirdSectionOPTimes;
	int m_valSecondSectionOPTimes;
	float m_valThirdSectionAcceptableUtilization;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonOutputNc();
};
