#pragma once
#include "../Resource.h"

// CDlgWeiXin �Ի���

class CDlgWeiXin : public CDialog
{
	DECLARE_DYNAMIC(CDlgWeiXin)

public:
	CDlgWeiXin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWeiXin();

// �Ի�������
	enum { IDD = IDD_DLG_WEIXIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	Image* m_pImg;
};
