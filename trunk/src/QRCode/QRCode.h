// QRCode.h : QRCode DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQRCodeApp
// �йش���ʵ�ֵ���Ϣ������� QRCode.cpp
//

class CQRCodeApp : public CWinApp
{
public:
	CQRCodeApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
