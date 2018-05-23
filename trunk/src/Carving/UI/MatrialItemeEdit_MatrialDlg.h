#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
#include "../resource.h"
// MatrialItemeEdit_MatrialDlg �Ի���

class MatrialItemeEdit_MatrialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MatrialItemeEdit_MatrialDlg)

public:
	MatrialItemeEdit_MatrialDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MatrialItemeEdit_MatrialDlg();

// �Ի�������
	enum { IDD = IDD_MATRIAL_ITEM_EDIT_MATRIAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_MatrialNum;						//��ѡ��������
	CString m_MatrialName;						//���ò�������
	CComboBox m_MatrialNameBox;					//��ѡ��������
	std::vector<std::string> m_MatrialNameList;	//��ѡ���������б�
public:
	//���ò�������
	CString GetMatrialName() const { return m_MatrialName; }

	//��ѡ���������б�
	void SetMatrialNameList(std::vector<std::string> val) { m_MatrialNameList = val; }

	//��ѡ���������б�
	void AppendMatrialName(std::string name);


public:
	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
};
