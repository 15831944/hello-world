// TaoBaoSender.h : TaoBaoSender DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTaoBaoSenderApp
// �йش���ʵ�ֵ���Ϣ������� TaoBaoSender.cpp
//

class CTaoBaoSenderApp : public CWinApp
{
public:
	CTaoBaoSenderApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
