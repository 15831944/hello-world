// DlgLabelPosCalc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgLabelPosCalc.h"
#include "../DataProcess/DataProcess.h"
#include "ParamSettingDlg.h"
#include "../Misc/ProgramMisc.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"

#define WM_UPDATE_PROGRESS_BAR	WM_USER + 10087
// CDlgLabelPosCalc �Ի���

IMPLEMENT_DYNAMIC(CDlgLabelPosCalc, CDialog)

CDlgLabelPosCalc::CDlgLabelPosCalc(LPCTSTR szMachineName, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLabelPosCalc::IDD, pParent)
	, m_strMachineName(szMachineName)
{
	m_hdthreadTieBiaoCalc = NULL;
}

CDlgLabelPosCalc::~CDlgLabelPosCalc()
{
}

void CDlgLabelPosCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_LABEL_POS_CALC, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CDlgLabelPosCalc, CDialog)
	ON_MESSAGE(WM_UPDATE_PROGRESS_BAR, &CDlgLabelPosCalc::OnWM_WM_UPDATE_PROGRESS_BAR)		//��Ӧ
END_MESSAGE_MAP()


// CDlgLabelPosCalc ��Ϣ�������
//���ڳ�ʼ��
BOOL CDlgLabelPosCalc::OnInitDialog()					
{
	CDialog::OnInitDialog();

	// ��ʼ�Ż��߳�
	StartTieBiaoCalcThread();

	return  TRUE;

}

LRESULT CDlgLabelPosCalc::OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam)
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

	strProgress.Format("��ǰ���ȣ�%0.2f%%......", progress);
	GetDlgItem(IDC_STATIC_LABEL_POS_CALC)->SetWindowText(strProgress);

	return 0;
}

DWORD CDlgLabelPosCalc::TieBiaoPosCalcThread( PVOID lpThreadParameter )
{
	CDlgLabelPosCalc* self = (CDlgLabelPosCalc*)( lpThreadParameter);


	vector<Component*> vAllComponent;
	FindAllComponentInSingleton(vAllComponent);


	float tiebiao_width = 0;
	float tiebiao_height = 0;
	int nLabelPosType = LABEL_POS_CENTER;
	GetTiebiaoSize(tiebiao_width,tiebiao_height);
	ParamSettingDlg settings;
	settings.loadSettings();
	nLabelPosType = settings.LabelPosType();
	bool bLabelInUpperSide = GetLabelSide(self->m_strMachineName) == 0 ? true : false;

	//��ǩ��������һ���ߴ���ùε�����
	float expandSize = 10.0;
	tiebiao_width = tiebiao_width + expandSize + expandSize;
	tiebiao_height = tiebiao_height + expandSize + expandSize;

	if (!settings.Tiebiao_auto_all_not())
	{
		for(int i = 0; i < vAllComponent.size() ; i++)
		{
			//��õ��Ǳ�ǩ�����ĵ�
			PointFloat tiePoint = createTiebiaoPosition(*(vAllComponent[i]),tiebiao_width,tiebiao_height, nLabelPosType, settings.Tiebiao_offset_x(), settings.Tiebiao_offset_y(), bLabelInUpperSide);

			PointFloat fpoint;
			fpoint.x = tiePoint.x/* + settings.Tiebiao_offset_x()*/;
			fpoint.y = (vAllComponent[i]->m_RealWidth-tiePoint.y) /*+ settings.Tiebiao_offset_y()*/;

			vAllComponent[i]->m_fXLabelCenter = fpoint.x;
			vAllComponent[i]->m_fYLabelCenter = fpoint.y;

			if(i % 10 == 0)
				::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, vAllComponent.size(), i);
		}
	}
	else
	{
		for(int i = 0; i < vAllComponent.size() ; i++)
		{
			PointFloat ptLabelCenter_NotConverted = GetDefaultLabelPos(*(vAllComponent[i]), tiebiao_width, tiebiao_height, nLabelPosType, settings.Tiebiao_offset_x(), settings.Tiebiao_offset_y(), true);
			PointFloat ptLabelCenter_Converted;
			ptLabelCenter_Converted.x = ptLabelCenter_NotConverted.x/* + settings.Tiebiao_offset_x()*/;
			ptLabelCenter_Converted.y = (vAllComponent[i]->m_RealWidth-ptLabelCenter_NotConverted.y) /*+ settings.Tiebiao_offset_y()*/;

			vAllComponent[i]->m_fXLabelCenter = ptLabelCenter_Converted.x;
			vAllComponent[i]->m_fYLabelCenter = ptLabelCenter_Converted.y;

			if(i % 10 == 0)
				::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, vAllComponent.size(), i);
		}
	}


	::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);
	return 0;
}

void CDlgLabelPosCalc::StartTieBiaoCalcThread()
{
	m_hdthreadTieBiaoCalc = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TieBiaoPosCalcThread, this, NULL, NULL);
}

BOOL CDlgLabelPosCalc::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}