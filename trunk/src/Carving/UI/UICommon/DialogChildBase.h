#pragma once

// CDialogChildBase �Ի���

class CDialogChildBase : public CDialog
{
	DECLARE_DYNAMIC(CDialogChildBase)

public:
	//CDialogChildBase(CWnd* pParent = NULL);   // ��׼���캯��
	CDialogChildBase(UINT uID, CWnd* pParent = NULL);
	virtual ~CDialogChildBase();

// �Ի�������
	//enum { IDD = IDD_DIALOGCHILDBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();

};
