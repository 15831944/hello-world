#pragma once
#include "afxwin.h"
#include "../Resource.h"

// TheChangeCode �Ի���

class TheChangeCode : public CDialogEx
{
	DECLARE_DYNAMIC(TheChangeCode)

public:
	TheChangeCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TheChangeCode();

// �Ի�������
	enum { IDD = IDD_ChangeCode };
	virtual BOOL OnInitDialog();
	CString ChangeFirst1;//���ļ���λ1�ļ�ͷ
	CString ChangeFirst2;//���ļ���λ2�ļ�ͷ
	CString ChangeLast1;//���ļ���λ1�ļ�β
	CString ChangeLast2;//���ļ���λ2�ļ�β
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	
	DECLARE_MESSAGE_MAP()
public:
};
