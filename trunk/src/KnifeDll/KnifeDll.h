// KnifeDll.h : KnifeDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif









// CKnifeDllApp
// �йش���ʵ�ֵ���Ϣ������� KnifeDll.cpp
//

class CKnifeDllApp : public CWinApp
{
public:
	CKnifeDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
