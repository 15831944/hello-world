// SelectPathDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelectPathDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
//#include "../Misc/XmlHandlePlus.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Misc/Misc.h"
#include "../Work/WorkDef.h"

// SelectPathDlg �Ի���

IMPLEMENT_DYNAMIC(SelectPathDlg, CDialogEx)

SelectPathDlg::SelectPathDlg(CString _title,CWnd* pParent /*=NULL*/)
	: CDialogEx(SelectPathDlg::IDD, pParent)
{
	Title(_title);
	CurrentPath("D:\\Ĭ��·��");
	loadSettings();
}

SelectPathDlg::~SelectPathDlg()
{
}

void SelectPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH_EDITER, m_currentPath);
}


BEGIN_MESSAGE_MAP(SelectPathDlg, CDialogEx)
	ON_BN_CLICKED(ID_FIND_PATH, &SelectPathDlg::OnBnClickedFindPath)		//���
	ON_BN_CLICKED(ID_OPEN_PATH, &SelectPathDlg::OnBnClickedOpenPath)		//��������Ŀ¼
	ON_BN_CLICKED(ID_CLEAR_PATH, &SelectPathDlg::OnBnClickedClearPath)		//������Ŀ¼
	ON_BN_CLICKED(IDOK, &SelectPathDlg::OnBnClickedOk)						//ȷ��ѡ��
	ON_BN_CLICKED(IDCANCEL, &SelectPathDlg::OnBnClickedCancel)				//ȡ��
END_MESSAGE_MAP()


/************************************************************************/
/* UI                                                                   */
/************************************************************************/
//���
void SelectPathDlg::OnBnClickedFindPath()
{
	UpdateData(TRUE);
	CString newPath = SelectPathDlgDoModel(m_currentPath);
	if (newPath != "")
	{
		m_currentPath = newPath;
		UpdateData(FALSE);
	}
}
//��������Ŀ¼
void SelectPathDlg::OnBnClickedOpenPath()
{
	UpdateData(TRUE);
	HGTools::createDir(m_currentPath);
	ShellExecute(NULL,"explore", m_currentPath ,NULL,NULL,SW_SHOWNORMAL);
}
//��������Ŀ¼
void SelectPathDlg::OnBnClickedClearPath()
{
	UpdateData(TRUE);
	if (MessageBox("�Ƿ�ɾ����Ŀ¼�������ļ���","ɾ����ʾ",MB_OKCANCEL|MB_ICONWARNING  ) == IDOK)
	{
		HGTools::deletePathFiles(m_currentPath);
		MessageBox("����ɹ�");
	}
}
//ȷ��ѡ��
void SelectPathDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	HGTools::createDir(m_currentPath);
	CDialogEx::OnOK();
}

//ȡ��
void SelectPathDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

//��ʼ���Ի���
BOOL SelectPathDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_title != "")
	{
		SetWindowText(m_title);
	}
	UpdateData(FALSE);
	return TRUE;  
}

/************************************************************************/
/* Logic                                                                */
/************************************************************************/

//��������
void SelectPathDlg::loadSettings()
{


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// �����ļ�
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif








	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;


	//Ĭ�ϱ���Ŀ¼
	//m_defaultSavePath = docHandler.getParam("DefaultSavePath","D:\\123\\123").c_str();
	strTmp = _T("D:\\123\\123");
	TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(doc.RootElement(), "Param", "Name", "DefaultSavePath");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_defaultSavePath = strTmp;

	//Ĭ�ϼ���Ŀ¼
	//m_defaultLoadPath = docHandler.getParam("DefaultLoadPath","D:\\123\\123").c_str();	
	strTmp = _T("D:\\123\\123");
	elmTmp = xmlHandler.GetIndicatedElement(doc.RootElement(), "Param", "Name", "DefaultLoadPath");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_defaultLoadPath = strTmp;


	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

//��������
void SelectPathDlg::saveSettings()
{
	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	//�����ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//����
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

#else


	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);
	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif






	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;

	//Ĭ�ϱ���Ŀ¼
	//docHandler.setParam("DefaultSavePath",m_defaultSavePath.GetBuffer());
	xmlHandler.SetIndicatedElmAttr(doc.RootElement(), "Param", "Name", "DefaultSavePath", "Value", m_defaultSavePath.GetBuffer());
	m_defaultSavePath.ReleaseBuffer();

	//Ĭ�ϼ���Ŀ¼
	//docHandler.setParam("DefaultLoadPath",m_defaultLoadPath.GetBuffer());
	xmlHandler.SetIndicatedElmAttr(doc.RootElement(), "Param", "Name", "DefaultLoadPath", "Value", m_defaultLoadPath.GetBuffer());
	m_defaultLoadPath.ReleaseBuffer();


	//����
	doc.SaveFile(xmlPath);




	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)



	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());

#else


	//HGTools::encryptFile(xmlPath,hgmPath);
#endif


	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

/************************************************************************/
/* ���ߺ���                                                             */
/************************************************************************/


//�����ҵ��ļ�·���Ļص�����
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
{
	if(uMsg == BFFM_INITIALIZED)
	{  
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;  
}


//�����û�ѡ���Ŀ¼
CString SelectPathDlg::SelectPathDlgDoModel( CString findPath )
{
	CString pathName;

	char  szDir[1000]; 
	BROWSEINFO bi; 
	ITEMIDLIST *pidl;  
	bi.hwndOwner = this->m_hWnd;  
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = szDir;  
	bi.lpszTitle = _T("��ѡ��Ŀ¼");   
	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS; 
	CFileFind   finder;
	if(finder.FindFile(findPath)==FALSE)
	{
		bi.lParam =0;
		bi.lpfn = NULL;
	}
	else
	{
		bi.lParam = (long)(m_currentPath.GetBuffer(findPath.GetLength()));//��ʼ��·��������(_T("c:\\Symbian"));
		bi.lpfn = BrowseCallbackProc;
	}
	finder.Close();
	bi.iImage = 0;  
	pidl = SHBrowseForFolder(&bi); 
	if(pidl == NULL) return "";
	if(!SHGetPathFromIDList(pidl, szDir)) return ""; 

	pathName = szDir;
	return pathName;
}







CString SelectPathDlg::SelectFileDlgDoModel(CString title, CString fileType , CString defaultname, CString findPath  ,DlgType type )
{
	char filterBuffer[100];
	CString filter;
	filter.Format("%s Files (*.%s)|*.%s|All Files (*.*)|*.*||",fileType,fileType,fileType);
	HGTools::Convert2FillChar(filter,filterBuffer);

	CString fileSaveName;
	fileSaveName.Format("%s.%s",defaultname,fileType);

	CFileDialog fileDlg (type, fileType, fileSaveName,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filterBuffer, this);
	fileDlg.m_ofn.lpstrTitle = title; 
	fileDlg.m_ofn.lpstrInitialDir = findPath;


	if (fileDlg.DoModal() == IDOK)
	{
		return fileDlg.GetPathName();
	}
	else
	{
		return "";
	}
}


