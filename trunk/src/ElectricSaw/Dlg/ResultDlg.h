#pragma once
#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"

// CResultDlg �Ի���
class CTotalResultDlg;

class CResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResultDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESULT };

	void RefreshOptimizeResult();
	void StopDrawing();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRefreshPanelVeiw(WPARAM wparam, LPARAM lparam);

	CRect GetPanelViewRect();
	void DrawDiagram(PanelViewingParam* param);
	void DrawDiagram();
	void FindAllcomponentFromPanel(Component& thePanel, vector<Component*>& vAllComponent);
public:
	virtual BOOL OnInitDialog();

protected:
	CTotalResultDlg* m_pTotalResultDlg;
	bool m_bDrawPanel;
	Image* m_pImg;
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
