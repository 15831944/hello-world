// Algorithm_CSharp.h : Algorithm_CSharp DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAlgorithm_CSharpApp
// �йش���ʵ�ֵ���Ϣ������� Algorithm_CSharp.cpp
//

class CAlgorithm_CSharpApp : public CWinApp
{
public:
	CAlgorithm_CSharpApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
