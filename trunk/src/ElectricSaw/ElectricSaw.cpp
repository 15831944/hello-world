
// ElectricSaw.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ElectricSaw.h"
#include "ElectricSawDlg.h"

#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/TinyXml/tinyxml.h"
#include "../../include/FileReadWrite/HgmReadWrite/HGTools.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CElectricSawApp

BEGIN_MESSAGE_MAP(CElectricSawApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CElectricSawApp ����

CElectricSawApp::CElectricSawApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CElectricSawApp ����

CElectricSawApp theApp;


// CElectricSawApp ��ʼ��

BOOL CElectricSawApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	Gdiplus::GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL); 

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CElectricSawDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�



	// ��������ǰ���������ļ�
	CSingleon* pSingleton = CSingleon::GetSingleton();

	// ���ص����ļ�
	//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
//	CString knifeType_hgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
// 	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hgm);
// 
// 	HGTools::decryptFile(knifeType_hgm,knifeType_xml);
// 	pSingleton->m_pKnifetypeDoc->LoadFile(knifeType_xml);
// 	HGTools::deleteFile(knifeType_xml);






	return FALSE;
}



int CElectricSawApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	GdiplusShutdown(m_pGdiToken);
	return CWinApp::ExitInstance();
}
