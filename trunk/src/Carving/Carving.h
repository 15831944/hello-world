// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// Carving.h : Carving Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/FileReadWrited.lib")
#pragma comment(lib, "../../lib/tinyxmld.lib")
//#pragma comment(lib, "../../lib/KnifeDlld.lib")
#pragma comment(lib, "../../lib/QRCoded.lib")
#pragma comment(lib, "../../lib/Encryptiond.lib")
#pragma comment(lib, "../../lib/InternetManagerd.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManagerd.lib")
#pragma comment(lib, "../../lib/TaoBaoSenderd.lib")

#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/FileReadWrite.lib")
#pragma comment(lib, "../../lib/tinyxml.lib")
//#pragma comment(lib, "../../lib/KnifeDll.lib")
#pragma comment(lib, "../../lib/QRCode.lib")
#pragma comment(lib, "../../lib/FileCompressandUnCompress.lib")
#pragma comment(lib, "../../lib/hg3d.lib")
#pragma comment(lib, "../../lib/Encryption.lib")
#pragma comment(lib, "../../lib/InternetManager.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManager.lib")
#pragma comment(lib, "../../lib/TaoBaoSender.lib")

#pragma  message(".......................................��ǰΪrelease�汾")

#endif


#define WM_REFRESH_PANEL_VIEW               WM_USER+1
#define WM_USER_EXPORT_MATERIAL_LIST        WM_USER+2
#define WM_USER_EXPORT_COMPONENT_NC         WM_USER+3

// CCarvingApp:
// �йش����ʵ�֣������ Carving.cpp
//

class CCarvingApp : public CWinAppEx
{
public:
	CCarvingApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	ULONG_PTR m_pGdiToken;
};

extern CCarvingApp theApp;
