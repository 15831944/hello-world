#pragma once
#include "../resource.h"

// MainCode �Ի���

class MainCode : public CDialogEx
{
	DECLARE_DYNAMIC(MainCode)

public:
	MainCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MainCode();

// �Ի�������
	enum { IDD = IDD_MAINCODE };
	CString MainFirst1;//���Ṥλ1�ļ�ͷ
	CString MainFirst2;//���Ṥλ2�ļ�ͷ
	CString MainLast1;//���Ṥλ1�ļ�β
	CString MainLast2;//���Ṥλ2�ļ�β
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
};
