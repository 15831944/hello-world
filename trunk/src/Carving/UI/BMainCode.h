#pragma once
#include "../resource.h"

// BMainCode �Ի���

class BMainCode : public CDialogEx
{
	DECLARE_DYNAMIC(BMainCode)

public:
	BMainCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BMainCode();
	CString BMainFirst1;//���湤λ1�ļ�ͷ
	CString BMainFirst2;//���湤λ2�ļ�ͷ
	CString BMainLast1;//���湤λ1�ļ�β
	CString BMainLast2;//���湤λ2�ļ�β
// �Ի�������
	enum { IDD = IDD_BMAINCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
