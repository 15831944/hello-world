
// ElectricSaw.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// �ж��Ƿ��ڲ��Ի�����
 #ifdef DEBUG 

	#pragma comment(lib, "../../lib/DataManagerd.lib")
	#pragma comment(lib, "../../lib/FileReadWrited.lib")
	#pragma comment(lib, "../../lib/tinyxmld.lib")
	#pragma  message(".......................................��ǰΪdebug�汾")

#else

	#pragma comment(lib, "../../lib/DataManager.lib")
	#pragma comment(lib, "../../lib/FileReadWrite.lib")
	#pragma comment(lib, "../../lib/tinyxml.lib")
	#pragma  message(".......................................��ǰΪrelease�汾")

 #endif



#define WM_REFRESH_PANEL_VIEW WM_USER+1
// CElectricSawApp:
// �йش����ʵ�֣������ ElectricSaw.cpp
//

class CElectricSawApp : public CWinApp
{
public:
	CElectricSawApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

protected:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
public:
	virtual int ExitInstance();
};

extern CElectricSawApp theApp;