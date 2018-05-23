// UISettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UISettingDlg.h"
#include "afxdialogex.h"


// UISettingDlg �Ի���

IMPLEMENT_DYNAMIC(UISettingDlg, CDialogEx)

UISettingDlg::UISettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(UISettingDlg::IDD, pParent)
{

}

UISettingDlg::~UISettingDlg()
{
}

void UISettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_DLG_SCORLLBAR, m_settingScroll);
}


BEGIN_MESSAGE_MAP(UISettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &UISettingDlg::OnOK)
	ON_BN_CLICKED(IDCANCEL, &UISettingDlg::OnCancel)
	ON_BN_CLICKED(IDC_SAVE_DEFAULT, &UISettingDlg::OnBnClickedSaveDefault)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


/************************************************************************/
/* ui �¼�                                                              */
/************************************************************************/


BOOL UISettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �������Ӵ���
	param.Create(IDD_PARAM_SETTING_DLG,GetDlgItem(IDC_SUB_DLG_CONTAINER));
	param.ShowWindow(SW_SHOW);

	//��������С
	GetDlgItem(IDC_SUB_DLG_CONTAINER)->GetWindowRect(m_settingDlgContainerRect);
	param.GetWindowRect(m_settingDlgRect);
	m_settingDlgRect.top = 0;
	m_settingDlgRect.left = 0;
	param.MoveWindow(m_settingDlgRect);

	//������
	int scorllmax = m_settingDlgRect.Height() - m_settingDlgContainerRect.Height();
	m_settingScroll.SetScrollRange(0,scorllmax);
	m_settingScroll.SetScrollPos(0);

	return TRUE;
}

void UISettingDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//�������������
	if (&m_settingScroll)
	{
		int newPos = nPos;
		int oldPos = m_settingScroll.GetScrollPos();
		int iMin = 0, iMax = 0;
		m_settingScroll.GetScrollRange(&iMin, &iMax);
		switch (nSBCode)
		{
		case SB_THUMBTRACK://�϶�����
			if ( iMin <= newPos && newPos <= iMax)
			{
				m_settingScroll.SetScrollPos(newPos,TRUE);
				m_settingDlgRect.top = -nPos;
				param.MoveWindow(m_settingDlgRect);
			}
			break;
		case SB_LINEUP://����ϱߵļ�ͷ
			if(oldPos > iMin)
			{
				oldPos--;
				m_settingScroll.SetScrollPos(oldPos, TRUE);
				m_settingDlgRect.top = -oldPos;
				param.MoveWindow(m_settingDlgRect);
			}
			break;
		case SB_LINEDOWN://����±ߵļ�ͷ
			if(oldPos < iMax)
			{
				oldPos++;
				m_settingScroll.SetScrollPos(oldPos, TRUE);
				m_settingDlgRect.top = -oldPos;
				param.MoveWindow(m_settingDlgRect);
			}
			break;
		case SB_PAGEUP://����������Ϸ��հ�
			if(oldPos > iMin)
			{
				oldPos--;
				m_settingScroll.SetScrollPos(oldPos, TRUE);
				m_settingDlgRect.top = -oldPos;
				param.MoveWindow(m_settingDlgRect);
			}
			break;
		case SB_PAGEDOWN://����������·��հ�
			if(oldPos < iMax)
			{
				oldPos++;
				m_settingScroll.SetScrollPos(oldPos, TRUE);
				m_settingDlgRect.top = -oldPos;
				param.MoveWindow(m_settingDlgRect);
			}
			break;
		case SB_ENDSCROLL:
			//m_iEdtPos = pScrollBar->GetScrollPos();
			//UpdateData(FALSE);
			break;
		default:
			break;
		}
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL UISettingDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int iMin = 0, iMax = 0, TempPos = m_settingScroll.GetScrollPos() , TempPosNew = 0;
	int step = 20;
	m_settingScroll.GetScrollRange(&iMin, &iMax);
	if(zDelta>0)//�������Ϲ���
	{
		if (TempPos > iMin)
		{
			TempPosNew = TempPos - step;
			TempPos = TempPosNew >  iMin ? TempPosNew : iMin;

			m_settingScroll.SetScrollPos(TempPos, TRUE);

			m_settingDlgRect.top = -TempPos;
			param.MoveWindow(m_settingDlgRect);
		}
	}
	else//�������¹���
	{
		if (TempPos < iMax)
		{
			TempPosNew = TempPos + step	;
			TempPos = TempPosNew <  iMax ? TempPosNew : iMax;

			m_settingScroll.SetScrollPos(TempPos, TRUE);

			m_settingDlgRect.top = -TempPos;
			param.MoveWindow(m_settingDlgRect);
		}
	}
	UpdateData(FALSE);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void UISettingDlg::OnBnClickedSaveDefault()
{
	//TODO: ��ǰ�ļ���Ĭ��
}

void UISettingDlg::OnOK()
{
	param.OnOK();
	CDialogEx::OnOK();
}


void UISettingDlg::OnCancel()
{
	CDialogEx::OnCancel();
}


/************************************************************************/
/* �߼��¼�                                                             */
/************************************************************************/

void UISettingDlg::loadSettings()
{
	param.loadSettings();
}

void UISettingDlg::saveSettings()
{
	param.saveSettings();
}








