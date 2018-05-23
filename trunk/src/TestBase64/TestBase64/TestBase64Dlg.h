
// TestBase64Dlg.h : ͷ�ļ�
//

#pragma once


// CTestBase64Dlg �Ի���
class CTestBase64Dlg : public CDialogEx
{
// ����
public:
	CTestBase64Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTBASE64_DIALOG };

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

public:

	CString m_valSrcFilePath;
	CString m_valDstFilePath;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFindSrcPath();
	afx_msg void OnBnClickedButtonEncrypt();
	afx_msg void OnBnClickedButtonDecrypt();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
