
// ConfigConverterDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

using namespace std;

// CConfigConverterDlg �Ի���
class CConfigConverterDlg : public CDialogEx
{
// ����
public:
	CConfigConverterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONFIGCONVERTER_DIALOG };

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

	void ConvertAllFile(vector<CString> vAllSourceDir, CString strTargetRootDir);
public:
	afx_msg void OnBnClickedButtonSourceRootDir();
	afx_msg void OnBnClickedButtonTargetRootDir();
	afx_msg void OnBnClickedButtonConvert();
};
