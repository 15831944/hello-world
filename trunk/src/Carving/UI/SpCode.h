#pragma once
#include "../resource.h"

// SpCode �Ի���

class SpCode : public CDialogEx
{
	DECLARE_DYNAMIC(SpCode)

public:
	SpCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SpCode();

// �Ի�������
	enum { IDD = IDD_SPCODE };

	CString SpFirst1;	//С�幤λ1�ļ�ͷ
	CString SpFirst2;	//С�幤λ2�ļ�ͷ
	CString SpLast1;	//С�幤λ1�ļ�β
	CString SpLast2;	//С�幤λ2�ļ�β

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
