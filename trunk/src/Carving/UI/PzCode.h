#pragma once
#include "../Resource.h"

// PzCode �Ի���

class PzCode : public CDialogEx
{
	DECLARE_DYNAMIC(PzCode)

public:
	PzCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PzCode();

// �Ի�������
	enum { IDD = IDD_PZCODE };
	CString PzFirst1;//���깤λ1�ļ�ͷ
	CString PzFirst2;//���깤λ2�ļ�ͷ
	CString PzLast1;//���깤λ1�ļ�β
	CString PzLast2;//���깤λ2�ļ�β
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
