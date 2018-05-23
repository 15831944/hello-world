
// ConfigConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigConverter.h"
#include "ConfigConverterDlg.h"
#include "afxdialogex.h"
#include "FileCompressandUnCompress.h"
#include <vector>
#include "Misc/Misc.h"
#include "Misc/ProgramMisc.h"
#include "ConvertingDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConfigConverterDlg �Ի���




CConfigConverterDlg::CConfigConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConfigConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_ROOT_DIR, &CConfigConverterDlg::OnBnClickedButtonSourceRootDir)
	ON_BN_CLICKED(IDC_BUTTON_TARGET_ROOT_DIR, &CConfigConverterDlg::OnBnClickedButtonTargetRootDir)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CConfigConverterDlg::OnBnClickedButtonConvert)
END_MESSAGE_MAP()


// CConfigConverterDlg ��Ϣ�������

BOOL CConfigConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->SetWindowText(GetModulePath());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CConfigConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CConfigConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CConfigConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConfigConverterDlg::OnBnClickedButtonSourceRootDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));  

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "�ɰ��Ŀ¼";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->SetWindowText(szPath);
	}
/*	else   
		AfxMessageBox("Ŀ¼��Ч!"); */  

}


void CConfigConverterDlg::OnBnClickedButtonTargetRootDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 

	ZeroMemory(szPath, sizeof(szPath));  

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "�°��Ŀ¼";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		CString str = szPath;
		if(str.GetAt(str.GetLength()-1) != _T('\\'))
			str += _T("\\");
		GetDlgItem(IDC_EDIT_TARGET_ROOT_DIR)->SetWindowText(str);
	}
}


void CConfigConverterDlg::OnBnClickedButtonConvert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//vector<CString> vAllSourceDir;

	CString strSourceDir, strTargetDir;
	GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->GetWindowText(strSourceDir);
	//vAllSourceDir = TraversFile(strSourceDir, _T("hgm"));
	GetDlgItem(IDC_EDIT_TARGET_ROOT_DIR)->GetWindowText(strTargetDir);

	if(strSourceDir.GetAt(strSourceDir.GetLength()-1) != _T('\\'))
		strSourceDir += _T("\\");
	if(strTargetDir.GetAt(strTargetDir.GetLength()-1) != _T('\\'))
		strTargetDir += _T("\\");
	CConvertingDlg dlg(strSourceDir, strTargetDir);
	dlg.DoModal();

	//CString strFindFile = strTargetDir + _T("*.*")
 //   _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�  
 //   WIN32_FIND_DATAA FindFileData;        
 //   ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
 //   HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);  
 //   BOOL IsFinded = TRUE;  
 //   while(IsFinded)  
 //   {  
 //       IsFinded = FindNextFileA(hFile, &FindFileData); //�ݹ������������ļ�  
 //       if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //�������"." ".."Ŀ¼  
 //       {  
 //           std::string strFileName = "";  
 //           strFileName = strFileName + DirName + "//" + FindFileData.cFileName;  
 //           std::string strTemp;  
 //           strTemp = strFileName;  
 //           if( isDir(strFileName.c_str()) ) //�����Ŀ¼����ݹ�ص���  
 //           {     
 //               printf("Ŀ¼Ϊ:%s/n", strFileName.c_str());  
 //               deletePath(strTemp.c_str());  
 //           }  
 //           else  
 //           {  
 //               DeleteFileA(strTemp.c_str());  
 //           }  
 //       }  
 //   }  
 //   FindClose(hFile); 

}

void CConfigConverterDlg::ConvertAllFile(vector<CString> vAllSourceDir, CString strTargetRootDir)
{

}