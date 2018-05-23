
// TestBase64Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestBase64.h"
#include "TestBase64Dlg.h"
#include "DES/DES.h"
#include "afxdialogex.h"



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


// CTestBase64Dlg �Ի���




CTestBase64Dlg::CTestBase64Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestBase64Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_valSrcFilePath = _T("");
	m_valDstFilePath = _T("");
}

void CTestBase64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_valSrcFilePath);
}

BEGIN_MESSAGE_MAP(CTestBase64Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestBase64Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestBase64Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FIND_SRC_PATH, &CTestBase64Dlg::OnBnClickedButtonFindSrcPath)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, &CTestBase64Dlg::OnBnClickedButtonEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPT, &CTestBase64Dlg::OnBnClickedButtonDecrypt)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTestBase64Dlg::OnBnClickedButtonTest)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CTestBase64Dlg ��Ϣ�������

BOOL CTestBase64Dlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestBase64Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestBase64Dlg::OnPaint()
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
HCURSOR CTestBase64Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


						// HIJKLMNABCDEFGUVWXYZOPQRSTuvwxyzabcdefgopqrsthijklmn789654123#*/

static const char *codes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

static const unsigned char map[256] = {  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,  
	255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,  
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,  
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,  
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,  
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
	255, 255, 255, 255 };  

int base64_encode(const unsigned char *in,  unsigned long len,   
		unsigned char *out)  
	{  
		unsigned long i, len2, leven;  
		unsigned char *p;  
		/* valid output size ? */  
		len2 = 4 * ((len + 2) / 3);  
		p = out;  
		leven = 3*(len / 3);  
		for (i = 0; i < leven; i += 3) {  
			*p++ = codes[in[0] >> 2];  
			*p++ = codes[((in[0] & 3) << 4) + (in[1] >> 4)];  
			*p++ = codes[((in[1] & 0xf) << 2) + (in[2] >> 6)];  
			*p++ = codes[in[2] & 0x3f];  
			in += 3;  
		}  
		/* Pad it if necessary...  */  
		if (i < len) {  
			unsigned a = in[0];  
			unsigned b = (i+1 < len) ? in[1] : 0;  
			unsigned c = 0;  

			*p++ = codes[a >> 2];  
			*p++ = codes[((a & 3) << 4) + (b >> 4)];  
			*p++ = (i+1 < len) ? codes[((b & 0xf) << 2) + (c >> 6)] : '=';  
			*p++ = '=';  
		}  

		/* append a NULL byte */  
		*p = '\0';  

		return p - out;  
	}  
static int base64_decode_map[256] = {  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0   - 15  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16  - 31  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32  - 47  
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48  - 63  
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 64  - 79  
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80  - 95  
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96  - 111  
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112 - 127  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128 - 143  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144 - 159   
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160 - 175  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176 - 191  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192 - 207  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208 - 223  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224 - 239  
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 240 - 255  
};  

int base64_decode(const unsigned char *in, unsigned char *out)  
{  
	unsigned long t, x, y, z;  
	unsigned char c;  
	int g = 3;  

	for (x = y = z = t = 0; in[x]!=0;) {  
		c = map[in[x++]];  
		if (c == 255) return -1;  
		if (c == 253) continue;  
		if (c == 254) { c = 0; g--; }  
		t = (t<<6)|c;  
		if (++y == 4) {  
			//          if (z + g > *outlen) { return CRYPT_BUFFER_OVERFLOW; }  
			out[z++] = (unsigned char)((t>>16)&255);  
			if (g > 1) out[z++] = (unsigned char)((t>>8)&255);  
			if (g > 2) out[z++] = (unsigned char)(t&255);  
			y = t = 0;  
		}  
	}  
	//  if (y != 0) {  
	//      return -1;  
	//  }  
	return z;  
}  


/*---------------------------------------*/
//	����˵����
//		�����ļ�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
int encrypt_base64(char* src_path, char* dst_path) 
{
	int src_file_len;
	CFile src_file, dst_file;
	char* pSrcBuf = NULL;
	char* pDstBuf = NULL;

	try
	{
		// ��Դ�ļ�����������
		src_file.Open((LPCTSTR)src_path, CFile::modeRead);
		src_file_len = src_file.GetLength();
		pSrcBuf = new char[src_file_len];
		src_file.Read(pSrcBuf, src_file_len);
		src_file.Close();

		// ����������������base64����
		pDstBuf = new char[src_file_len*2];	// ���ܻ��ɣ������Բ��ᳬ��2��
		memset(pDstBuf, 0, src_file_len*2);

		base64_encode((const unsigned char*)pSrcBuf, src_file_len, (unsigned char*)pDstBuf);

		// д�����ļ��ļ�
		DeleteFile((LPCTSTR)dst_path);
		dst_file.Open((LPCTSTR)dst_path, CFile::modeReadWrite | CFile::modeCreate);
		CString str(pDstBuf);
		dst_file.Write(pDstBuf, str.GetLength());
		dst_file.Close();

		// ����ڴ�
		delete[]	pSrcBuf;
		delete[]	pDstBuf;

	}
	catch (CMemoryException* e)
	{

	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}


	return 0;
}





/*---------------------------------------*/
//	����˵����
//		�����ļ�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
int decrypt_base64(char* src_path, char* dst_path)
{
	int src_file_len;
	CFile src_file, dst_file;
	char* pSrcBuf = NULL;
	char* pDstBuf = NULL;

	// ��Դ�ļ�����������
	src_file.Open((LPCTSTR)src_path, CFile::modeRead);
	src_file_len = src_file.GetLength();
	pSrcBuf = new char[src_file_len];
	src_file.Read(pSrcBuf, src_file_len);
	src_file.Close();

	// �������������ݽ���
	pDstBuf = new char[src_file_len];	// ���ܳ���һ���ȼ��ܺ�Ķ�

	memset(pDstBuf, 0, src_file_len);

	int dst_file_len ;
	base64_decode((const unsigned char*)pSrcBuf, (unsigned char *)pDstBuf);

	CString tmp_str = pDstBuf;

	dst_file_len = tmp_str.GetLength();


	// д�����ļ��ļ�
	DeleteFile((LPCTSTR)dst_path);


	dst_file.Open((LPCTSTR)dst_path, CFile::modeReadWrite | CFile::modeCreate);
	dst_file.Write(pDstBuf, dst_file_len);
 	dst_file.Close();

	// ����ڴ�
	delete[]	pSrcBuf;
	delete[]	pDstBuf;

	return 0;
}







void CTestBase64Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	

	CDialogEx::OnOK();
}


void CTestBase64Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	




	CDialogEx::OnCancel();
}





void CTestBase64Dlg::OnBnClickedButtonFindSrcPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString filter = "hge �ļ�(*.hge)|*.hge|xml �ļ�(*.xml)|*.xml|�����ļ� (*.*)|*.*||";

	CFileDialog fileDlg (TRUE, _T("xml"), _T("*.xml"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	if ( fileDlg.DoModal() == IDOK)
	{
		m_valSrcFilePath = fileDlg.GetPathName();
		UpdateData(FALSE);
	}

}


void CTestBase64Dlg::OnBnClickedButtonEncrypt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_valDstFilePath = m_valSrcFilePath.Left(m_valSrcFilePath.ReverseFind('.')) + ".hge";


	//encrypt_base64(m_valSrcFilePath.GetBuffer(), m_valDstFilePath.GetBuffer());

	func_drawing0(m_valSrcFilePath.GetBuffer(), m_valDstFilePath.GetBuffer());


	AfxMessageBox("������ɣ�");

}


void CTestBase64Dlg::OnBnClickedButtonDecrypt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	m_valDstFilePath = m_valSrcFilePath.Left(m_valSrcFilePath.ReverseFind('.')) + ".xml";

	//decrypt_base64(m_valSrcFilePath.GetBuffer(), m_valDstFilePath.GetBuffer());

	func_drawing1(m_valSrcFilePath.GetBuffer(), m_valDstFilePath.GetBuffer());

	AfxMessageBox("������ɣ�");

}



void CTestBase64Dlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	char* pSrc = "123456789";
	char Dst[20] = {0};
	char buf[20] = {0};

	//base64_encode((const  char * )pSrc, buf, 10);
	//base64_decode(buf, ( char*)Dst);


	int a = 0;

}


void CTestBase64Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//ȡ�ñ��϶��ļ�����Ŀ  
	for(int i=0;i< DropCount;i++)  
	{  
		char Str[MAX_PATH];  
		DragQueryFile(hDropInfo,i,Str,MAX_PATH);//�����ҷ�ĵ�i���ļ����ļ���  
		
		m_valSrcFilePath.SetString(Str);
		UpdateData(FALSE);
	}  


	DragFinish(hDropInfo);  //�ϷŽ�����,�ͷ��ڴ�  

	CDialogEx::OnDropFiles(hDropInfo);
}
