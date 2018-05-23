#pragma once
#include "../Resource.h"
#include <map>

using namespace std;

class Panel;

// CDlgSetMachiningOrder �Ի���

class CDlgSetMachiningOrder : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetMachiningOrder)

public:
	CDlgSetMachiningOrder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetMachiningOrder();

// �Ի�������
	enum { IDD = IDD_DLG_SET_MACHINING_ORDER };
	
	void SetPanel(Panel* pPanel){ m_pPanel = pPanel; };
	map<int, int> GetResettedOrderMap(){ return m_mapResettedOrder; };
	CRect GetPanelViewRect();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnReset();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void DrawSettingOrder(CDC* pDC, Panel* pPanel, CRect rcDrawArea, map<int, int>& m_mapResettedOrder_tmp);
	int GetClickedComponentID(CPoint point);
	void ArrangeUI();

	map<int, int> m_mapResettedOrder;
	map<int, int> m_mapResettedOrder_tmp;
	Panel* m_pPanel;
};
