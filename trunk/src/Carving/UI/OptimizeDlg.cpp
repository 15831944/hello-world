// DirSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptimizeDlg.h"

#include "../DataProcess/DataProcess.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"


// ÿ�Ż�һ�ַ�һ�ν��
#define WM_CALCULATE			WM_USER + 10086
#define WM_UPDATE_PROGRESS_BAR	WM_USER + 10087
#define WM_UPDATE_REMAIN_TIME	WM_USER + 10088

int OptimizeDlg::m_LastPanelNum = 0;
BOOL OptimizeDlg::m_ThreadIsRunning = FALSE;
vector<ComponentInputItem> OptimizeDlg::m_vComponentInputItem;
vector<PreCombineItem> OptimizeDlg::m_vPreCombineItem;
vector<RemainderInputItem>  OptimizeDlg::m_vRemainderInputItem;

// CDirSetDlg dialog

IMPLEMENT_DYNAMIC(OptimizeDlg, CDialogEx)

OptimizeDlg::OptimizeDlg(CWnd* pParent /*=NULL*/)
			: CDialogEx(OptimizeDlg::IDD, pParent)
{
	// ���ݳ�ʼ��
	m_LastPanelNum = 0;
	m_ThreadIsRunning = FALSE;
	m_vComponentInputItem.clear();






}

OptimizeDlg::~OptimizeDlg()
{
}

void OptimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE, m_TxtOpMessage);
	DDX_Control(pDX, IDC_BTN_PROGRESS_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE2, m_TxtOpMessage2);
	DDX_Control(pDX, IDC_LIST_SOLUTION_INFO, m_ListCtrlSolutionInfo);
	DDX_Control(pDX, IDC_BTN_USE_NOW, m_BtnUseNow);
	DDX_Control(pDX, IDC_STATIC_OP_REMAIN_TIME, m_TxtRemainTime);
}


BEGIN_MESSAGE_MAP(OptimizeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &OptimizeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &OptimizeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_USE_NOW, &OptimizeDlg::OnBnClickedUseNow)

	ON_MESSAGE(WM_CALCULATE, &OptimizeDlg::OnWM_CALCULATE)								//��Ӧ
	ON_MESSAGE(WM_UPDATE_PROGRESS_BAR, &OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR)		//��Ӧ
	ON_MESSAGE(WM_UPDATE_REMAIN_TIME, &OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME)			//��Ӧ
	



END_MESSAGE_MAP()


void OptimizeDlg::OnBnClickedOk()
{
	

	CDialogEx::OnOK();
}


void OptimizeDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}






/*---------------------------------------*/
//	����˵����
//		���ʹ�ã�ֹͣ�߳�
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
void OptimizeDlg::OnBnClickedUseNow() 
{
	if (m_ThreadIsRunning == TRUE)
	{
		m_ThreadIsRunning = FALSE;
		DWORD dw = WaitForSingleObject(m_hdthread, INFINITE);	// �ȴ��߳̽���
		switch(dw)
		{
		case WAIT_OBJECT_0:	
			// The process terminated.
			// �߳��˳�
			CloseHandle(m_hdthread);
			CDialogEx::OnOK();
			break;

		case WAIT_TIMEOUT:
			// The process did not terminate within 5000 milliseconds.
			CloseHandle(m_hdthread);
			AfxMessageBox("�Ż��̳߳�ʱ������");
			CDialogEx::OnOK();
			break;

		case WAIT_FAILED:
			// Bad call to function (invalid handle?)
			CloseHandle(m_hdthread);
			AfxMessageBox("�Ż��߳��˳���Ч��");
			CDialogEx::OnOK();
			break;
		}
	}
	else
	{
		CDialogEx::OnOK();
	}

}

//���ڳ�ʼ��
BOOL OptimizeDlg::OnInitDialog()					
{
	CDialogEx::OnInitDialog();

	// �ؼ���ʼ��
	//CFont font;
	//font.CreatePointFont(100, "Arial");

	//m_ListCtrlSolutionInfo.SetFont(&font);
	HWND hwnd = ::GetDlgItem(m_ListCtrlSolutionInfo.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcSolutionInfo.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcSolutionInfo.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE);
	SetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_ListCtrlSolutionInfo.SetExtendedStyle((m_ListCtrlSolutionInfo.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	CRect rcListCtrl;
	m_ListCtrlSolutionInfo.GetWindowRect(rcListCtrl);
	ScreenToClient(rcListCtrl);
	rcListCtrl.right+=5;
	m_ListCtrlSolutionInfo.MoveWindow(rcListCtrl);

	m_ListCtrlSolutionInfo.InsertColumn(0,"���ϲ���",0,180);
	m_ListCtrlSolutionInfo.InsertColumn(1,"���",0,60);
	m_ListCtrlSolutionInfo.InsertColumn(2,"���",0,120);
	m_ListCtrlSolutionInfo.InsertColumn(3,"����",0,60);

	// ����Ӧ�ð���
	m_BtnUseNow.EnableWindow(FALSE);



	// ��ʼ�Ż��߳�
	StartOptimizeThread();

	return  TRUE;

}

/*---------------------------------------*/
//	����˵����
//		�����Ż��߳�
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
void OptimizeDlg::StartOptimizeThread()
{
	
	m_hdthread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OptimizeThread, this, NULL, NULL);
	if (m_hdthread != INVALID_HANDLE_VALUE)
	{
		m_ThreadIsRunning = TRUE;	// ����
	}



}



/*---------------------------------------*/
//	����˵����
//		�̺߳����������Ż�
//
//
//	������
//		PVOID lpThreadParameter		-- �Ż�����ָ��
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
DWORD OptimizeDlg::OptimizeThread( PVOID lpThreadParameter )
{
	OptimizeDlg* self = (OptimizeDlg*)( lpThreadParameter);

	// ʱ�����
	CTime last_time = CTime::GetCurrentTime();
	CTime cur_time;
	CTimeSpan  timeSpan;



	// ����ԭ�㡢�Ż�����
	CSingleon* pSingleton = CSingleon::GetSingleton();
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;
	//vector<ComponentList> vComponentList;
	ComponentList componentList;

	int nTotalCount = pSingleton->m_BaseInfo.m_FirstSectionOPTimes;

	// �Ż�ѭ����ʼ
	for(int i_progress = 0; i_progress < nTotalCount; i_progress++)
	{
		


// ��һ���Ż�
#if 1


		int i_first_op_times = i_progress;
		//for(int i_first_op_times = 0; i_first_op_times < 1/*pSingleton->m_BaseInfo.m_FirstSectionOPTimes*/; i_first_op_times++)
		{
			// �ͷŽ������ 
			pSingleton->ClearCurrentSolution();
			pSingleton->ClearRemainderManager();

			// �ͷ�С�����
			pSingleton->m_vComponentGroup.clear();

			// ����С�����
			//ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
			ConvertInputInfoToComponentList(m_vComponentInputItem, self->m_vPreCombineItem, componentList/*, mapComponentToPreCombine*/);

			// ���ڴ���������������������ʸ����������������Ż�ʱ���Ⱥ����Ƹ���һ��
			int text_index = i_progress%5;
			float rotate_limit = pSingleton->m_BaseInfo.m_PanelLength >  pSingleton->m_BaseInfo.m_PanelWidth ?  pSingleton->m_BaseInfo.m_PanelWidth :  pSingleton->m_BaseInfo.m_PanelLength ;

			rotate_limit -= 2* pSingleton->m_BaseInfo.m_DeburringWidth;


			if (text_index == 1)
			{
				for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
				{
					Component* pCpn = componentList.at(i_cpn);

					// ȫ���ú�����һ��, ������ת�ĳ���
					if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
						(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
					{
						pCpn->m_Texture = TextureType_H_TEXTURE;
					}
					else
					{
						int a = 0;
					}
				}

			}
			else if (text_index == 2)
			{
				for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
				{
					Component* pCpn = componentList.at(i_cpn);

					// ȫ���ú�����һ��
					if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
						(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
					{
						pCpn->m_Texture = TextureType_V_TEXTURE;
					}
					else
					{
						int a = 0;
					}
				}
			}

			// ��ֵ����������Ż�ԭ��
			SplitComponentList(componentList, pSingleton->m_vComponentGroup);

			// ���ϸ�ֵ
			for(int i_rmd = 0; i_rmd < m_vRemainderInputItem.size(); i_rmd++)
			{
				RemainderInputItem& rmd_item =  m_vRemainderInputItem.at(i_rmd);

				RemainderItem* pRmd = new RemainderItem();

				pRmd->m_Length = rmd_item.m_nXLen;
				pRmd->m_Width = rmd_item.m_nYLen;
				pRmd->m_Material = rmd_item.m_strMaterial;
				pRmd->m_Thickness = rmd_item.m_fThickness;
				pRmd->m_nCount = rmd_item.m_nCount;

				pSingleton->m_RemainderManager.AddRemainderItem(pRmd);
			}






			// �Ż�
			if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// ���������
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// ̰��
			{
				if (i_first_op_times == 2)
				{
					pSingleton->Layout(1, CutDir_Horizon, Org);
				}
				else if (i_first_op_times == 3)
				{
					pSingleton->Layout(1, CutDir_Vertical, Org);
				}
				else if (i_first_op_times == 4)
				{
					pSingleton->Layout(1, CutDir_Default, Org);
				}
				else
				{
					pSingleton->Layout(1, CutDir_Random, Org);
				}
			}
			else
			{
				// ��� ̰��+���������
				int flag = pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

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


#endif

// ��ʱ���õڶ������׶��Ż�
#if 0

		// �ڶ����Ż�
		for (int i_second_op_times = 0; i_second_op_times < 0/*pSingleton->m_BaseInfo.m_SecondSectionOPTimes*/; i_second_op_times++)
		{
			for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
			{
				CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

				pSln->ReOptimizeEveryPanel(Org);
			}
		}




		// ����Ż��ʲ��ߵİ����������
		for (int i_third_op_times = 0; i_third_op_times < 0/*pSingleton->m_BaseInfo.m_ThirdSectionOPTimes*/; i_third_op_times++)
		{
			for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
			{
				CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

				pSingleton->ReOptimizeSln(pSln, Org);
			}
		}
 
#endif


		// ÿ������һ�ַ�һ�μ��������Ϣ
		int nPanelCount = pSingleton->GetBackupSolutionPanelNum(); 


		cur_time = CTime::GetCurrentTime();
		timeSpan = cur_time - last_time;

		int seconds = timeSpan.GetTotalSeconds();


		// ������һ���򴰿ڷ���ˢ�����ֺͽ�������Ϣ
		if (i_progress%10 == 0)
		{
			::PostMessage(self->GetSafeHwnd(), WM_CALCULATE, 0, nPanelCount);
			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, nTotalCount, i_progress);
			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_REMAIN_TIME, (nTotalCount<<16) |  i_progress,  seconds);
		}

		// �ж��߳��Ƿ��������
		if (m_ThreadIsRunning == FALSE)
		{
			return 0;
		}
	}

	// �Ż�����
	::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);

	return 0;
}


//��ɹر�
LRESULT OptimizeDlg::OnWM_CALCULATE(WPARAM wParam, LPARAM lParam)
{
	int messageinfo = (int)wParam;
	int showmessage = (int)lParam;
	CString strMsg =  "Ŀǰ����:";

	
	switch(messageinfo)
	{
	case 0:

		if (m_LastPanelNum == 0)
		{
			m_LastPanelNum = showmessage;
		}
		else
		{
			if (showmessage < m_LastPanelNum)
			{
				m_LastPanelNum = showmessage;
				AfxMessageBox("�и��ŵķ������֣�");
			}
		}
		strMsg.Format("����һ�׽������: %d ���塣�����Ӧ�á���ť���ã�", showmessage);

		m_TxtOpMessage.SetWindowText(strMsg);
		break;
	default:
		break;
	}

	

	return 0;
}

/*---------------------------------------*/
//	����˵����
//	���½�����
//
//
//	������
//		WPARAM wParam	--	�ܴ���
//		LPARAM lParam	--	��ǰ����
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
LRESULT OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam)
{
	int first_message = (int)wParam;
	int second_massage = (int)lParam;

	// ���½�����
	m_ProgressBar.SetRange32(0, first_message);
	second_massage = second_massage % first_message+1;
	m_ProgressBar.SetPos(second_massage);

	// �����ı�����
	float cur_time = lParam;
	float total_time = wParam;
	float progress = cur_time*100/total_time;
	CString strProgress;

	strProgress.Format("��ǰ�Ż����ȣ�%0.2f%%......", progress);
	m_TxtOpMessage2.SetWindowText(strProgress);




	// �����Ż���Ϣ

	CSingleon* pSingleton = CSingleon::GetSingleton();
	int nSlnCount = pSingleton->GetBackupSolutionNum();

	m_ListCtrlSolutionInfo.DeleteAllItems();
	for (int i = 0 ; i< nSlnCount ; i++)
	{
		m_ListCtrlSolutionInfo.InsertItem(i,"");
	}

	for (int iSln = 0; iSln < nSlnCount; iSln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(iSln);

		CString str_thickness;
		CString str_panel_size;
		CString str_panel_num;

		str_thickness.Format("%0.0f", pSln->m_fThickness);
		str_panel_size.Format("%0.0f x %0.0f", pSln->m_BaseInfo.m_PanelLength, pSln->m_BaseInfo.m_PanelWidth);
		str_panel_num.Format("%d", pSln->GetPanelNum());

		m_ListCtrlSolutionInfo.SetItemText(iSln, 0, pSln->m_strMaterial);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 1, str_thickness);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 2, str_panel_size);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 3, str_panel_num);
	}

	// ʹ��Ӧ�ð���
	m_BtnUseNow.EnableWindow(TRUE);




	return 0;
}


LRESULT OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME(WPARAM wParam, LPARAM lParam)
{
	float total_count = wParam>>16;
	float cur_count = wParam & 0xffff;
	float seconds_cur_count = lParam;
	
	// 
	if (cur_count < 1)
	{
		cur_count = 1;
	}

	float remain_time = seconds_cur_count/cur_count*total_count - seconds_cur_count;

	CString strProgress;
	strProgress.Format("ʣ��ʱ�䣺%0.0f��......", remain_time);



	m_TxtRemainTime.SetWindowText(strProgress);

	return 0;
}

BOOL OptimizeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			if(m_BtnUseNow.IsWindowEnabled())
			{
				PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_USE_NOW, BN_CLICKED), (LPARAM)0/*(GetDlgItem(IDC_BTN_USE_NOW)->GetSafeHwnd())*/);  
			}
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}