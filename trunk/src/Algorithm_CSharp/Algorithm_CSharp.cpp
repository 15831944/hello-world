// Algorithm_CSharp.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Algorithm_CSharp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CAlgorithm_CSharpApp

BEGIN_MESSAGE_MAP(CAlgorithm_CSharpApp, CWinApp)
END_MESSAGE_MAP()


// CAlgorithm_CSharpApp ����

CAlgorithm_CSharpApp::CAlgorithm_CSharpApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAlgorithm_CSharpApp ����

CAlgorithm_CSharpApp theApp;


// CAlgorithm_CSharpApp ��ʼ��

BOOL CAlgorithm_CSharpApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
