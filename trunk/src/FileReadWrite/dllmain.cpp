// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/tinyxmld.lib")
#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/Encryptiond.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManagerd.lib")
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/tinyxml.lib")
#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/Encryption.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManager.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif








static AFX_EXTENSION_MODULE FileReadWriteDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FileReadWrite.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(FileReadWriteDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(FileReadWriteDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FileReadWrite.DLL ������ֹ!\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(FileReadWriteDLL);
	}
	return 1;   // ȷ��
}
