#pragma once
#include "../resource.h"

// SelectPathDlg �Ի���

typedef enum {
	DlgSave = false,
	DlgLoad = true,
}DlgType;

class SelectPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectPathDlg)

public:
	SelectPathDlg(CString _title = "��ѡ��Ŀ¼" ,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SelectPathDlg();

// �Ի�������
	enum { IDD = IDD_FILE_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/************************************************************************/
	/* UI                                                                   */
	/************************************************************************/
public:
	afx_msg void OnBnClickedFindPath();		//���
	afx_msg void OnBnClickedOpenPath();		//��������Ŀ¼
	afx_msg void OnBnClickedClearPath();	//������Ŀ¼
	afx_msg void OnBnClickedOk();			//ȷ��ѡ��
	afx_msg void OnBnClickedCancel();		//ȡ��
	virtual BOOL OnInitDialog();			//��ʼ���Ի���

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	void loadSettings();			//��������
	void saveSettings();			//��������

	/************************************************************************/
	/* ���ߺ���                                                             */
	/************************************************************************/
protected:
	CString SelectPathDlgDoModel(CString findPath);	//�����û�ѡ���Ŀ¼
public:

	CString SelectFileDlgDoModel(CString title, CString fileType , CString defaultname, CString findPath  ,DlgType type );	//�����û�ѡ���Ŀ¼

private:
	CString m_currentPath;		//��ǰĿ¼
	CString m_title;			//��������ʾ
	CString m_defaultSavePath;	//Ĭ�ϱ���Ŀ¼
	CString m_defaultLoadPath;	//Ĭ�ϼ���Ŀ¼
public:
	//��ǰĿ¼
	CString CurrentPath() const { return m_currentPath; }
	//��ǰĿ¼
	void CurrentPath(CString val) { m_currentPath = val; }

	//��������ʾ
	CString Title() const { return m_title; }
	//��������ʾ
	void Title(CString val) { m_title = val; }

	//Ĭ�ϱ���Ŀ¼
	CString DefaultSavePath() const { return m_defaultSavePath; }
	//Ĭ�ϱ���Ŀ¼
	void DefaultSavePath(CString val) { m_defaultSavePath = val; }

	//Ĭ�ϼ���Ŀ¼
	CString DefaultLoadPath() const { return m_defaultLoadPath; }
	//Ĭ�ϼ���Ŀ¼
	void DefaultLoadPath(CString val) { m_defaultLoadPath = val; }
};
