// Encryption.h : Encryption DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEncryptionApp
// �йش���ʵ�ֵ���Ϣ������� Encryption.cpp
//

class CEncryptionApp : public CWinApp
{
public:
	CEncryptionApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
