#pragma once
#include "afxwin.h"
#include "../Resource.h"
#include "ParamSettingDlg.h"
// UISettingDlg �Ի���

class UISettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UISettingDlg)

public:
	UISettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UISettingDlg();

// �Ի�������
	enum { IDD = IDD_UI_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	/************************************************************************/
	/* ui �¼�                                                              */
	/************************************************************************/

	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedSaveDefault();
	virtual void OnOK();
	virtual void OnCancel();

private:
	CScrollBar m_settingScroll;				// �Ӵ��ڹ�����
	CRect m_settingDlgContainerRect;		// �Ӵ��ڰ�Χ����
	CRect m_settingDlgRect;					// �Ӵ��ڶԻ�������

public:
	/************************************************************************/
	/* �߼��¼�                                                             */
	/************************************************************************/
	void loadSettings();
	void saveSettings();
public:
	ParamSettingDlg param;	
	
};
