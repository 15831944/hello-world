
// ElectricSawDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ElectricSaw.h"
#include "ElectricSawDlg.h"
#include "afxdialogex.h"

#include "Dlg/PanelListDlg.h"
#include "Dlg/ResultDlg.h"

#include "DataProcess/DataProcess.h"

// �ⲿDataManager.dll��ͷ�ļ�
#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"


// �ⲿFileReadWrite.dll��ͷ�ļ�
#include "../../include/FileReadWrite/ExcelReadWrite/ExcelReadWrite.h"
#include "../../include/FileReadWrite/HgmReadWrite/HgmReadWrite.h"
#include "../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"

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


// CElectricSawDlg �Ի���




CElectricSawDlg::CElectricSawDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CElectricSawDlg::IDD, pParent)
	, m_pPanelListDlg(NULL)
	, m_pResultDlg(NULL)
	, m_eCurViewType(VIEW_PANEL_LIST)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_valFirstSectionOPTimes = 1;						// ��ʼ�� ��һ�׶��Ż�����
	m_valPanelLength = 2440.0f;
	m_valPanelWidth = 1220.0f;
	m_valKerfWidth = 6.0f;	m_valThirdSectionOPTimes = 0;
	m_valSecondSectionOPTimes = 0;
	m_valThirdSectionAcceptableUtilization = 0.90f;
}

void CElectricSawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIRST_SECTION_OP_TIMES, m_valFirstSectionOPTimes);
	DDV_MinMaxInt(pDX, m_valFirstSectionOPTimes, 1, 1000000);
	DDX_Control(pDX, IDC_COMBO_FIRST_SECTION_OP_METHOD, m_cbxFirstSectionOPMethod);
	DDX_Text(pDX, IDC_EDIT_PANEL_LENGTH, m_valPanelLength);
	DDV_MinMaxFloat(pDX, m_valPanelLength, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_PANEL_WIDTH, m_valPanelWidth);
	DDV_MinMaxFloat(pDX, m_valPanelWidth, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_KERF_WIDTH, m_valKerfWidth);
	DDV_MinMaxFloat(pDX, m_valKerfWidth, 0.1f, 100.0f);
	DDX_Text(pDX, IDC_EDIT_THIRD_SECTION_OP_TIMES, m_valThirdSectionOPTimes);
	DDV_MinMaxInt(pDX, m_valThirdSectionOPTimes, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_SECOND_SECTION_OP_TIMES, m_valSecondSectionOPTimes);
	DDV_MinMaxInt(pDX, m_valSecondSectionOPTimes, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_THIRD_SECTION_ACCEPTABLE_UTILIZATION, m_valThirdSectionAcceptableUtilization);
	DDV_MinMaxFloat(pDX, m_valThirdSectionAcceptableUtilization, 0.1f, 1.0f);
}

BEGIN_MESSAGE_MAP(CElectricSawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CElectricSawDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CElectricSawDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PANEL_LIST, &CElectricSawDlg::OnBnClickedButtonPanelList)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &CElectricSawDlg::OnBnClickedButtonResult)
	ON_BN_CLICKED(IDC_BUTTON_OPTIMIZE, &CElectricSawDlg::OnBnClickedButtonOptimize)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CElectricSawDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_NC, &CElectricSawDlg::OnBnClickedButtonOutputNc)
END_MESSAGE_MAP()


// CElectricSawDlg ��Ϣ�������

BOOL CElectricSawDlg::OnInitDialog()
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

	ShowWindow(SW_MAXIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SwitchView(VIEW_PANEL_LIST);


	m_cbxFirstSectionOPMethod.InsertString(0, "�Ľ�����������㷨");
	m_cbxFirstSectionOPMethod.InsertString(1, "�Ľ�̰���㷨");
	m_cbxFirstSectionOPMethod.InsertString(2, "����㷨");

	m_cbxFirstSectionOPMethod.SetCurSel(1);





	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CElectricSawDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CElectricSawDlg::OnPaint()
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
HCURSOR CElectricSawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CElectricSawDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int i = 1;

	int j = 0;

	j = 1;

	CSingleon* p = CSingleon::GetSingleton();


	int a = 0;

	CDialogEx::OnOK();
}



void CElectricSawDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	// �������һ�ε�����
	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->ClearAllData();
	m_vComponentInputItem.clear();


	CString m_strPanelInfoFile;
	CString filter = "xls �ļ�(*.xls)|*.xls|hgm �ļ�(*.hgm)|*.hgm|xml �ļ�(*.xml)|*.xml|�����ļ� (*.*)|*.*||";

	CFileDialog fileDlg (true, _T("xls"), _T("*.xls"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	if ( fileDlg.DoModal() == IDOK)
	{
		m_strPanelInfoFile = fileDlg.GetPathName();

		int Which = m_strPanelInfoFile.ReverseFind('.');  
		CString strExtName = m_strPanelInfoFile.Right(m_strPanelInfoFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xls" || strExtName == "xlsx" )
		{
			if(ExcelReadWrite::ReadPanelInfo(m_strPanelInfoFile, m_vComponentInputItem) == true)
			{
				m_pPanelListDlg->SetPanelInfo(m_vComponentInputItem);
			}


			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);
				
		}
		else if (strExtName == "hgm")
		{
			if(HgmReadWrite::ReadPanelInfo(m_strPanelInfoFile, m_vComponentInputItem) == true)
			{
				m_pPanelListDlg->SetPanelInfo(m_vComponentInputItem);
			}

			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			AfxMessageBox(str);

		}
		else if (strExtName == "xml")
		{
			if(HgyReadWrite::ReadHgy(m_strPanelInfoFile) != true)
			{
				AfxMessageBox("hgy �ļ�����!");
			}
			else
			{
				m_vComponentInputItem = pSingleton->m_vBackupComponentInputItem;
			}

			SwitchView(VIEW_RESULT);
			m_pResultDlg->RefreshOptimizeResult();
		}
	}

	// ������������
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

}


void CElectricSawDlg::OnBnClickedButtonPanelList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SwitchView(VIEW_PANEL_LIST);
}


void CElectricSawDlg::OnBnClickedButtonResult()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SwitchView(VIEW_RESULT);
}

void CElectricSawDlg::SwitchView(ViewType eViewType)
{
	//if(eViewType == m_eCurViewType)
	//	return;

	switch(eViewType)
	{
	case VIEW_PANEL_LIST:
		if(m_pPanelListDlg == NULL || m_pPanelListDlg->GetSafeHwnd() == NULL)
		{
			delete m_pPanelListDlg;
			m_pPanelListDlg = new CPanelListDlg(this);
			m_pPanelListDlg->Create(CPanelListDlg::IDD, this);
			
		}
		if(m_pResultDlg != NULL && m_pResultDlg->GetSafeHwnd())
			m_pResultDlg->ShowWindow(SW_HIDE);
		//m_pResultDlg->SetWindowPos(NULL,10, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		m_pPanelListDlg->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_pPanelListDlg->ShowWindow(SW_SHOW);
		m_eCurViewType = VIEW_PANEL_LIST;
		break;

	case VIEW_RESULT:
		if(m_pResultDlg == NULL || m_pResultDlg->GetSafeHwnd() == NULL)
		{
			delete m_pResultDlg;
			m_pResultDlg = new CResultDlg(this);
			m_pResultDlg->Create(CResultDlg::IDD, this);
			
		}
		if(m_pPanelListDlg != NULL && m_pPanelListDlg->GetSafeHwnd())
			m_pPanelListDlg->ShowWindow(SW_HIDE);
		m_pResultDlg->ShowWindow(SW_SHOW);
		m_pResultDlg->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_eCurViewType = VIEW_RESULT;
		break;
	}
}

CRect CElectricSawDlg::GetViewRect()
{
	CRect rcRet;
	CRect rcClient;
	GetClientRect(rcClient);
	rcRet.CopyRect(rcClient);
	rcRet.InflateRect(0, -150, 0, 0);
	return rcRet;
}

void CElectricSawDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pPanelListDlg != NULL && m_pPanelListDlg->GetSafeHwnd())
		m_pPanelListDlg->MoveWindow(GetViewRect());
	if(m_pResultDlg != NULL && m_pResultDlg->GetSafeHwnd())
		m_pResultDlg->MoveWindow(GetViewRect());
}

void CElectricSawDlg::OnBnClickedButtonOptimize()
{
	// ��ȡ��������
	UpdateData(TRUE);

	// �л���ͼ���½�����
	SwitchView(VIEW_RESULT);

	vector<ComponentList> vComponentList;
	ComponentList componentList;
	CSingleon* pSingleton = CSingleon::GetSingleton();

	//pSingleton->ClearAllData();

	pSingleton->m_BaseInfo.m_SawKerfWidth = m_valKerfWidth;
	pSingleton->m_BaseInfo.m_PanelLength = m_valPanelLength;
	pSingleton->m_BaseInfo.m_PanelWidth = m_valPanelWidth;

	pSingleton->m_BaseInfo.m_FirstSectionOPTimes = m_valFirstSectionOPTimes;
	pSingleton->m_BaseInfo.m_FirstSectionOPMethod = m_cbxFirstSectionOPMethod.GetCurSel();
	pSingleton->m_BaseInfo.m_SecondSectionOPTimes = m_valSecondSectionOPTimes;
	pSingleton->m_BaseInfo.m_ThirdSectionOPTimes = m_valThirdSectionOPTimes;
	pSingleton->m_BaseInfo.m_ThirdSectionOAccptableUtilization = m_valThirdSectionAcceptableUtilization;


	// ����ԭ��
	int Org = LayoutOrg_LeftBottom;


	// ��һ���Ż�
	for(int i_first_op_times = 0; i_first_op_times < m_valFirstSectionOPTimes; i_first_op_times++)
	{
		// �����������
		//pSingleton->ClearAllData();
		pSingleton->ClearCurrentSolution();
		pSingleton->m_vComponentGroup.clear();


		// ���¸�ֵ
		ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
		SplitComponentList(componentList, pSingleton->m_vComponentGroup);

		// �Ż�
		if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// ���������
		{
			pSingleton->Layout(0, CutDir_Random, Org);
		}
		else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// ̰��
		{
			pSingleton->Layout(1, CutDir_Random, Org);
		}
		else
		{
			// ��� ̰��+���������
			int flag = m_valFirstSectionOPTimes/2;

			if (i_first_op_times > flag) // ���
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else
			{
				pSingleton->Layout(1, CutDir_Random, Org);
			}
		}
		
		pSingleton->BackupBestSolution();

	}

	// �ڶ����Ż�
	for (int i_second_op_times = 0; i_second_op_times < m_valSecondSectionOPTimes; i_second_op_times++)
	{
		for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
		{
			CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

			pSln->ReOptimizeEveryPanel(Org);
		}
	}

	


	// ����Ż��ʲ��ߵİ����������
	for (int i_third_op_times = 0; i_third_op_times < m_valThirdSectionOPTimes; i_third_op_times++)
	{
		for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
		{
			CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

			pSingleton->ReOptimizeSln(pSln, Org);
		}
	}

	// ���½������ID�Ͱ��ID
	pSingleton->UpdateSlnNum();



	CString str;

	str.Format("%d", pSingleton->GetBackupSolutionPanelNum());
	str += "����,";
	str = "Ŀǰ����:" + str;

	::MessageBox(NULL, str, "", MB_OK);
	
	m_pResultDlg->RefreshOptimizeResult();
}


// �����Ż�����ļ�
void CElectricSawDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString m_strFilePath;
	//CString filter = "hgy �ļ�(*.hgy)|*.hgy|�����ļ� (*.*)|*.*||";
	CString filter = "xml �ļ�(*.xml)|*.xml|�����ļ� (*.*)|*.*||";

	CFileDialog fileDlg (FALSE, _T("xml"), _T("HG001.xml"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	if ( fileDlg.DoModal() == IDOK)
	{
		m_strFilePath = fileDlg.GetPathName();
		HgyReadWrite::WriteHgy(m_strFilePath);
	}

}


void CElectricSawDlg::OnBnClickedButtonOutputNc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
