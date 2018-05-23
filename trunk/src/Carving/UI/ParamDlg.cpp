// ParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "ParamDlg.h"
#include "afxdialogex.h"
#include <iostream>

#include "VecOffset.h"
#include "Vecsoltchangedlg.h"

#include "../Misc/HGTools.h"
#include "../Misc/XmlHandlePlus.h"
#include "../Misc/Misc.h"
#include "../Carving.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// CParamDlg dialog

IMPLEMENT_DYNAMIC(CParamDlg, CDialogEx)

CParamDlg::CParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParamDlg::IDD, pParent)
{
	//��ȫ�߶�
	m_upheight = 0;
	//�µ����
	m_downheight = 0;

	// ԭ��ƫ��
	m_knife_org_offset = 0;
	//�µ�ƫ��1
	m_distance1 = 0;
	//�µ�ƫ��2
	m_distance2 = 0;
	//ԭ�ϳ���
	m_width = 0;
	//ԭ�Ͽ��
	m_height = 0;
	//�ޱ߾���
	m_Deburringwidth = 0;
	//������϶
	m_Kerfwidth = 0;
	//�յ�ƫ��
	int m_knife_shifting = 0;
	//�и��
	//m_OtherKerfWidth = 0;

	m_arranging_origin = 1;

	//С���������	
	m_ldoublecutEnable = 0;
	//С����С�ߴ�
	m_minarea = 0;//��С�������
	//Ԥ�������
	m_keepthickness = 0;//Ԥ�������
	//�ڶ����ӹ��ٶ�
	m_secondspeedrate = 0;//�ڶ����ӹ�����


	//�������� - ��Ƚ���
	m_twocut_threshold = 0;
	//�������� - Ԥ�����
	m_twocut_reserve = 0;
	//�������� - �ٶ�
	m_twocut_workSpeed = 0;
	//�����ٶ�
	m_cutspeed = 0;

	//�µ��ٶ�
	m_prospeed = 0;
	//��β�ٶ�
	m_endspeech = 0;
	//�����ٶ�
	m_overspeed = 0;

	//���浼�����г�Ʒexcel���ܣ����ڰ�ťʧЧ
	m_IsOutPutExcel = FALSE;

	//////////////////////////////////////////////////////////////////////////

	//��С������ - ��С����
	m_minw = "";
	//��С������ - ��С���
	m_minh = "";
	//��С������ - ��С���
	m_minsize = "";


	//�����
	m_isVecInput = 0;
	//�����
	m_isSoltInput = 0;
	//�����
	m_isDVecInput = 0;
	//�����
	m_isDSoltInput = 0;
	// �Ƿ�������
	m_isOthershapeInput = 0;
	// �Ƿ�ӹ�ͨ�׿�
	m_isReserveDeepHole = 1;
	// �Ƿ�ӹ�ͨ�ײ�
	m_isReserveDeepSlot = 1;

	//�и��
	int m_editnum = 0;
	// �ײ���λX
	m_moveX = 0;
	// �ײ���λY
	m_moveY = 0;
	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	m_vecFilter = "";
	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	m_vecAdder = "";


	//��ת��Ϣ
	int m_comchange = 0;
	//  m_valMinLen1 = _T("");
	m_valHMinLen1 = 0.0f;
	m_valVMinLen2 = 0.0f;
	m_valHMinWidth1 = 0.0f;
	m_valVMinWidth2 = 0.0f;
}

CParamDlg::~CParamDlg()
{
}

void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//�и����
	DDX_Text(pDX, IDC_UPHEIGHT, m_upheight);
	DDX_Text(pDX, IDC_DOWNHEIGHT, m_downheight);
	DDX_Text(pDX, IDC_ORG_OFFSET, m_knife_org_offset);
	DDX_Control(pDX, IDC_COMBO_YUANDIAN, m_DuiDaoYuanDian);
	DDX_Text(pDX, IDC_DISTANCE1, m_distance1);
	DDX_Text(pDX, IDC_DISTANCE2, m_distance2);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_DWIDTH, m_Deburringwidth);
	DDX_Text(pDX, IDC_KWIDTH, m_Kerfwidth);
	DDX_Control(pDX, IDC_KNIFE_SHIFTING, control_knife_shifting);
	DDX_CBIndex(pDX, IDC_KNIFE_SHIFTING, m_knife_shifting);
	//DDX_Text(pDX, IDC_OTHER_KERF_WIDTH, m_OtherKerfWidth);
	DDX_Control(pDX, IDC_COMBO_LAYOUT_ORIGIN, control_arranging_origin);
	DDX_CBIndex(pDX, IDC_COMBO_LAYOUT_ORIGIN, m_arranging_origin);


	//С���и�
	DDX_Control(pDX, IDC_LDOUBLECHECK, m_ldoublecut);
	DDX_Check(pDX, IDC_LDOUBLECHECK, m_ldoublecutEnable);
	DDX_Text(pDX, IDC_MINSBAREA, m_minarea);
	DDX_Text(pDX, IDC_KEEPTHICKNESS, m_keepthickness);
	DDX_Text(pDX, IDC_SECENDSPEEDRATE, m_secondspeedrate);


	//DDX_Text(pDX, IDC_TWO_CUT_THRESHOLD, m_twocut_threshold);
	//DDX_Text(pDX, IDC_TWO_CUT_RESERVE, m_twocut_reserve);
	//DDX_Text(pDX, IDC_TWO_CUT_WORK_SPEED, m_twocut_workSpeed);


	DDX_Text(pDX, IDC_MINW, m_minw);
	DDX_Text(pDX, IDC_MINH, m_minh);
	DDX_Text(pDX, IDC_MINSIZE, m_minsize);



	//�ӹ���Ϣ
	DDX_Check(pDX, IDC_DOWNSOLT, m_isDSoltInput);
	DDX_Check(pDX, IDC_DOWNVEC, m_isDVecInput);
	DDX_Check(pDX, IDC_UPSOLT, m_isSoltInput);
	DDX_Check(pDX, IDC_UPVEC, m_isVecInput);
	DDX_Check(pDX, IDC_OTHERSHAPE_INPUT, m_isOthershapeInput);
	DDX_Check(pDX, IDC_CHECK_DEEP_HOLE, m_isReserveDeepHole);
	DDX_Check(pDX, IDC_CHECK_DEEP_SLOT, m_isReserveDeepSlot);
	DDX_Text(pDX, IDC_EDITNUM, m_editnum);
	DDX_Text(pDX, IDC_EDITX, m_moveX);
	DDX_Text(pDX, IDC_EDITY, m_moveY);
	DDX_Control(pDX, IDC_COMBO1, control_comchange);
	DDX_CBIndex(pDX, IDC_COMBO1, m_comchange);



	DDX_Control(pDX, IDC_LIST5, m_listVecFilter);
	DDX_Control(pDX, IDC_LIST1, m_listVecAdder);
	DDX_Check(pDX, IDC_CHECK_PRE_DEBURRING, m_CheckPreDeburring);
	DDX_Check(pDX, IDC_CHECK_SMALL_COMPONENT_NO_TEXTURE, m_CheckSmallCpnNoTexture);
	//  DDX_Text(pDX, IDC_EDIT_MIN_LEN1, m_valMinLen1);
	DDX_Text(pDX, IDC_EDIT_MIN_LEN1, m_valHMinLen1);
	DDX_Text(pDX, IDC_EDIT_MIN_LEN2, m_valVMinLen2);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH1, m_valHMinWidth1);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH2, m_valVMinWidth2);
	DDX_Check(pDX, IDC_CHECKBOX_REMAINDER_LIBRARY, m_CheckRemainderLibrary);
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParamDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST5, &CParamDlg::OnDblclkVecFilter)
	ON_LBN_DBLCLK(IDC_LIST1, &CParamDlg::OnDblclkVecAdder)
	ON_CBN_SELCHANGE(IDC_COMBO_YUANDIAN, &CParamDlg::OnCbnSelchangeComboYuandian)
	ON_BN_CLICKED(IDC_LDOUBLECHECK, &CParamDlg::OnBnClickedLdoublecheck)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_SMALL_COMPONENT_NO_TEXTURE, &CParamDlg::OnBnClickedCheckSmallCpnNoTexture)

END_MESSAGE_MAP()


// ��ʼ��
BOOL CParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnInitDialog_QieGeCanShu();
	OnInitDialog_SmallPanelInfo();
	OnInitDialog_JiaGongXinXi();

	loadSettings();

	UpdateData(FALSE);

	updateToDialog_QieGeCanShu();
	updateToDialog_SmallPanelInfo();
	updateToDialog_JiaGongXinXi();
	

	// 
	OnBnClickedCheckSmallCpnNoTexture();





	return TRUE;  
}

void CParamDlg::update2Dialog()
{

}

/*---------------------------------------*/
//	����˵����
//		���С��������
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
void CParamDlg::OnBnClickedCheckSmallCpnNoTexture()
{
	UpdateData(TRUE);

	if (m_CheckSmallCpnNoTexture == TRUE)
	{
		
		GetDlgItem(IDC_STATIC_H_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_H_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_WIDTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_H_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_H_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_WIDTH)->EnableWindow(FALSE);

	}
}






//����
void CParamDlg::OnBnClickedOk()
{
	updateFromDialog_QieGeCanShu();
	UpdateData(TRUE);

	if ((m_distance2 + m_distance1) > 50)
	{
		MessageBox(_T("����λ�ƾ�����Ӳ��ɳ���50mm"));
		return;
	}

	saveSettings();

	// ͬ�����û�����Ϣ
	UpdateBaseInfo();



	CDialogEx::OnOK();
}




HBRUSH CParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(pWnd->GetDlgCtrlID())	//�޸Ľ��澲̬�ı�����ɫ
	{
		case IDC_STATIC_CUT:	//�и����
		case IDC_STATIC_SMALL:	//С���и�
		case IDC_STATIC_ADD:	//�ӹ���Ϣ
		{
			pDC->SetBkMode(TRANSPARENT);   //���ñ���͸��
			pDC->SetTextColor(RGB(0,0,255));
			return   HBRUSH(GetStockObject(NULL_BRUSH));
		}
		break;
		default:
			break;
	}
	return hbr;
}


// ��������
void CParamDlg::loadSettings()
{
	loadSettings_QieGeCanShu();
	loadSettings_SmallPanelInfo();
	loadSettings_MinimentPanelParam();
	loadSettings_JiaGongXinXi();

	// ���»�����Ϣ
	UpdateBaseInfo();
}

// ��������
void CParamDlg::saveSettings()
{
	saveSettings_QieGeCanShu();
 	saveSettings_SmallPanelInfo();
 	saveSettings_MinimentPanelParam();
 	saveSettings_JiaGongXinXi();
}



// ���µ������еĻ�����Ϣ
void CParamDlg::UpdateBaseInfo(void)
{
	// ��ʼ��������Ϣ
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->m_BaseInfo.m_PanelLength		= m_width;				// ����
	pSingleton->m_BaseInfo.m_PanelWidth			= m_height;				// ���
	pSingleton->m_BaseInfo.m_SawKerfWidth		= m_Kerfwidth;			// ���
	pSingleton->m_BaseInfo.m_DeburringWidth		= m_Deburringwidth;		// �ޱ�
	pSingleton->m_BaseInfo.m_LayoutOrg			= m_arranging_origin;	// ����ԭ��

	pSingleton->m_BaseInfo.m_UpHeight			= m_upheight;			// ��ȫ�߶�
	pSingleton->m_BaseInfo.m_DownHeight			= m_downheight;			// �µ����
	pSingleton->m_BaseInfo.m_distance1			= m_distance1;			// �µ�ƫ��1
	pSingleton->m_BaseInfo.m_distance2			= m_distance2;			// �µ�ƫ��2
	pSingleton->m_BaseInfo.m_MinLength			= _ttof(m_minw);		// ��С������ - ��С����
	pSingleton->m_BaseInfo.m_MinWidth			= _ttof(m_minh);		// ��С������ - ��С���
	pSingleton->m_BaseInfo.m_MinArea			= _ttof(m_minsize);		// ��С������ - ��С���
}





/************************************************************************/
/* ������                                                               */
/************************************************************************/

CString CParamDlg::ReadIniSetting(CIniFile& inifile, CString classify, CString colomn)
{
	CString result;
	inifile.GetItemString(classify,colomn,result);
	return result;
}
void CParamDlg::WriteIniSetting(CIniFile& inifile, CString classify, CString colomn, CString value)
{
	inifile.WriteItemString(classify,colomn,value);
}

/************************************************************************/
/* �и����                                                             */
/************************************************************************/
#if 1
//�и���� ���ִ��ڳ�ʼ��
void CParamDlg::OnInitDialog_QieGeCanShu()
{
	m_DuiDaoYuanDian.Clear();
	m_DuiDaoYuanDian.InsertString(0,"�����");
	m_DuiDaoYuanDian.InsertString(1,"���");

	control_knife_shifting.Clear();
	control_knife_shifting.InsertString(0,"�µ�ƫ��2");
	control_knife_shifting.InsertString(1,"�������");

	control_arranging_origin.Clear();
	control_arranging_origin.InsertString(0,_T("���½�"));
	control_arranging_origin.InsertString(1,_T("���Ͻ�"));
	control_arranging_origin.InsertString(2,_T("���½�"));
	control_arranging_origin.InsertString(3,_T("���Ͻ�"));
}

//�������ݱ仯������
void CParamDlg::updateToDialog_QieGeCanShu()
{
	if (m_downheight == 10000.0)
	{
		m_DuiDaoYuanDian.SetCurSel(1);		
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000");
	}
	else
	{
		m_DuiDaoYuanDian.SetCurSel(0);
	}
}

//�и���� ���ݼ���
void CParamDlg::loadSettings_QieGeCanShu()
{
	// ���������ļ�
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFileForRead();

	//��ȫ�߶�
	m_upheight = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("UpHeight")));

	//�µ����
	m_downheight = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("DownHeight")));

	//�Ե�ԭ��ƫ��
	m_knife_org_offset = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("KnifeOrgOffset")));

	//�µ�ƫ��1
	m_distance1 = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("Distance1")));

	//�µ�ƫ��2
	m_distance2 = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("Distance2")));

	//ԭ�ϳ���
	m_width = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Width")));

	//ԭ�Ͽ��
	m_height = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Height")));

	//������϶
	m_Kerfwidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Kerfwidth")));

	//�ޱ߾���
	m_Deburringwidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Deburringwidth")));
	m_Deburringwidth = m_Deburringwidth + m_Kerfwidth;

	//�յ�ƫ��
	m_knife_shifting = _ttoi(ReadIniSetting(inifile,_T("KnifeParam"),_T("KnifeShifting")));

	//�и��
	//m_OtherKerfWidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("OtherKerfWidth")));

	// ����ԭ��
	m_arranging_origin = _ttoi(ReadIniSetting(inifile,_T("KnifeParam"),_T("ArrangingOrigin")));

	// ���һ����Ԥ�ޱ�
	CString str_pre_deburring = ReadIniSetting(inifile,_T("KnifeParam"),_T("PreDeburring"));

	if (str_pre_deburring.IsEmpty() == TRUE)
	{
		m_CheckPreDeburring = FALSE;
	}
	else
	{
		m_CheckPreDeburring = _ttoi(str_pre_deburring);
	}

	// ���Ͽ⹦��
	CString str_rmd_lib = ReadIniSetting(inifile,_T("KnifeParam"),_T("RemainderLibrary"));

	if (str_rmd_lib.IsEmpty() == TRUE)
	{
		m_CheckRemainderLibrary	 = FALSE;
	}
	else
	{
		m_CheckRemainderLibrary = _ttoi(str_rmd_lib);
	}





	// ����С��������
	CString str_small_cpn_no_texture = ReadIniSetting(inifile,_T("KnifeParam"),_T("SmallCpnNoTexture"));

	if (str_small_cpn_no_texture.IsEmpty() == TRUE)
	{
		m_CheckSmallCpnNoTexture = FALSE;
	}
	else
	{
		m_CheckSmallCpnNoTexture = _ttoi(str_small_cpn_no_texture);
	}


	// ����С����Ϊ������--��С����1
	m_valHMinLen1 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinLen1")));

	// ����С����Ϊ������--��С���1
	m_valHMinWidth1 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinWidth1")));

	// ����С����Ϊ������--��С����2
	m_valVMinLen2 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinLen2")));

	// ����С����Ϊ������--��С���2
	m_valVMinWidth2 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinWidth2")));


}

//�и���� ���ݼ���
void CParamDlg::saveSettings_QieGeCanShu()
{
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFile();




	CString inputvalue;

	//��ȫ�߶�
	inputvalue.Format("%f",m_upheight);
	WriteIniSetting(inifile,_T("Machine"),_T("UpHeight"),inputvalue);

	//�µ����
	inputvalue.Format("%f",m_downheight);
	WriteIniSetting(inifile,_T("Machine"),_T("DownHeight"),inputvalue);



	//�Ե�ԭ��ƫ��
	inputvalue.Format("%f",m_knife_org_offset);
	WriteIniSetting(inifile,_T("Machine"),_T("KnifeOrgOffset"),inputvalue);

	
	//�µ�ƫ��1
	inputvalue.Format("%f",m_distance1);
	WriteIniSetting(inifile,_T("Machine"),_T("Distance1"),inputvalue);

	//�µ�ƫ��2
	inputvalue.Format("%f",m_distance2);
	WriteIniSetting(inifile,_T("Machine"),_T("Distance2"),inputvalue);

	//ԭ�ϳ���
	inputvalue.Format("%f",m_width);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Width"),inputvalue);

	//ԭ�Ͽ��
	inputvalue.Format("%f",m_height);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Height"),inputvalue);

	//������϶
	inputvalue.Format("%f",m_Kerfwidth);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Kerfwidth"),inputvalue);

	//�ޱ߾���
	m_Deburringwidth = m_Deburringwidth - m_Kerfwidth;
	inputvalue.Format("%f",m_Deburringwidth);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Deburringwidth"),inputvalue);

	//�յ�ƫ��
	inputvalue.Format("%d",m_knife_shifting);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("KnifeShifting"),inputvalue);

	//�и��
	//inputvalue.Format("%f",m_OtherKerfWidth);
	//WriteIniSetting(inifile,_T("KnifeParam"),_T("OtherKerfWidth"),inputvalue);

	// ����ԭ��
	inputvalue.Format("%d",m_arranging_origin);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("ArrangingOrigin"),inputvalue);
	
	// Ԥ�ޱ�
	inputvalue.Format("%d",m_CheckPreDeburring);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("PreDeburring"), inputvalue);

	// ���Ͽ�
	inputvalue.Format("%d",m_CheckRemainderLibrary);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("RemainderLibrary"), inputvalue);
	


	// ����С����Ϊ������
	inputvalue.Format("%d",m_CheckSmallCpnNoTexture);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("SmallCpnNoTexture"), inputvalue);


	// ����С����Ϊ������--��С����1
	inputvalue.Format("%0.1f",m_valHMinLen1);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinLen1"), inputvalue);

	// ����С����Ϊ������--��С���1
	inputvalue.Format("%0.1f",m_valHMinWidth1);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinWidth1"), inputvalue);

	// ����С����Ϊ������--��С����2
	inputvalue.Format("%0.1f",m_valVMinLen2);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinLen2"), inputvalue);

	// ����С����Ϊ������--��С���2
	inputvalue.Format("%0.1f",m_valVMinWidth2);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinWidth2"), inputvalue);

	inifile.OpenIniFileForWrite();
}

//�ӽ����������
void CParamDlg::updateFromDialog_QieGeCanShu()
{
	if (m_DuiDaoYuanDian.GetCurSel() == 1)
	{
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000.0");
	}
}

//��Ӧ��������޸�
void CParamDlg::OnCbnSelchangeComboYuandian()
{
	int select = m_DuiDaoYuanDian.GetCurSel();
	if (select == 0)//����
	{
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("0");
		m_downheight = 0.0;
	}
	else//���
	{
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000");
		m_downheight = 10000.0;
		UpdateData(FALSE);
	}
}

#endif
/************************************************************************/
/* С����Ϣ                                                             */
/************************************************************************/
#if 1
//С����Ϣ ���ִ��ڳ�ʼ��
void CParamDlg::OnInitDialog_SmallPanelInfo()
{

}

//�������ݱ仯������
void CParamDlg::updateToDialog_SmallPanelInfo()
{
	int changeDisableArr[] = {IDC_STATIC5,IDC_STATIC4,IDC_STATIC6,IDC_MINSBAREA,IDC_KEEPTHICKNESS,IDC_SECENDSPEEDRATE};

	for (int i = 0;i<sizeof(changeDisableArr)/sizeof(int);i++)
	{
		if (m_ldoublecutEnable)
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(FALSE);
		}
	}	
}

//С����Ϣ ���ݼ���
void CParamDlg::loadSettings_SmallPanelInfo()
{
	// ���������ļ�
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFileForRead();




	//С���������	
	m_ldoublecutEnable = _ttoi(ReadIniSetting(inifile,_T("SmallCut"),_T("LDoubleCut")));

	//С����С�ߴ�
	m_minarea = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("MinArea")));

	//Ԥ�������
	m_keepthickness = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("KeepThickness")));

	//�ڶ����ӹ��ٶ�
	m_secondspeedrate = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("SecondSpeedRate")));


	//�������� - ��Ƚ���
	m_twocut_threshold = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_threshold")));

	//�������� - Ԥ�����
	m_twocut_reserve = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_reserve")));

	//�������� - �ٶ�
	m_twocut_workSpeed = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_speed")));

	//////////////////////////////////////////////////////////////////////////
	//����û�е�����

	//�����ٶ�
	m_cutspeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("CutSpeech")));

	//�µ��ٶ�
	m_prospeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("ProcessSpeech")));

	//��β�ٶ�
	m_endspeech = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("EndSpeech")));

	//�����ٶ�
	m_overspeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("OverSpeech")));


	//�����滻����EXCEL
	m_IsOutPutExcel = _ttoi(ReadIniSetting(inifile,_T("Machine"),_T("IsOutPutExcel")));
}

//С����Ϣ ���ݼ���
void CParamDlg::saveSettings_SmallPanelInfo()
{
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFile();



	CString inputvalue;
	
	//С���������	
	inputvalue.Format("%d",m_ldoublecutEnable);
	WriteIniSetting(inifile,_T("SmallCut"),_T("LDoubleCut"),inputvalue);

	//С����С�ߴ�
	inputvalue.Format("%f",m_minarea);
	WriteIniSetting(inifile,_T("SmallCut"),_T("MinArea"),inputvalue);

	//Ԥ�������
	inputvalue.Format("%f",m_keepthickness);
	WriteIniSetting(inifile,_T("SmallCut"),_T("KeepThickness"),inputvalue);

	//�ڶ����ӹ��ٶ�
	inputvalue.Format("%f",m_secondspeedrate);
	WriteIniSetting(inifile,_T("SmallCut"),_T("SecondSpeedRate"),inputvalue);


	//�������� - ��Ƚ���
	inputvalue.Format("%f",m_twocut_threshold);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_threshold"),inputvalue);

	//�������� - Ԥ�����
	inputvalue.Format("%f",m_twocut_reserve);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_reserve"),inputvalue);

	//�������� - �ٶ�
	inputvalue.Format("%f",m_twocut_workSpeed);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_speed"),inputvalue);

	//////////////////////////////////////////////////////////////////////////
	//����û�е�����

	//�����ٶ�
	inputvalue.Format("%f",m_cutspeed);
	WriteIniSetting(inifile,_T("Machine"),_T("CutSpeech"),inputvalue);

	//�µ��ٶ�
	inputvalue.Format("%f",m_prospeed);
	WriteIniSetting(inifile,_T("Machine"),_T("ProcessSpeech"),inputvalue);

	//��β�ٶ�
	inputvalue.Format("%f",m_endspeech);
	WriteIniSetting(inifile,_T("Machine"),_T("EndSpeech"),inputvalue);

	//�����ٶ�
	inputvalue.Format("%f",m_overspeed);
	WriteIniSetting(inifile,_T("Machine"),_T("OverSpeech"),inputvalue);

	//�����滻����EXCEL
	inputvalue.Format("%d",m_IsOutPutExcel);
	WriteIniSetting(inifile,_T("Machine"),_T("IsOutPutExcel"),inputvalue);

	inifile.OpenIniFileForWrite();
}

void CParamDlg::loadSettings_MinimentPanelParam()
{

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// �����ļ�
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\Op.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif








	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");




	//��ȡ����
	m_minw.Format("%s",docHandler.getParam("MinW").c_str());
	m_minh.Format("%s",docHandler.getParam("MinD").c_str());
	m_minsize.Format("%s",docHandler.getParam("MinSize").c_str());



	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

void CParamDlg::saveSettings_MinimentPanelParam()
{

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// �����ļ�
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\Op.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif







	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");




	//��������
	docHandler.setParam("MinW",m_minw.GetBuffer());
	docHandler.setParam("MinD",m_minh.GetBuffer());
	docHandler.setParam("MinSize",m_minsize.GetBuffer());





	//����
	doc.SaveFile(xmlPath);


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	encrypt_base64( xmlPath.GetBuffer(), hgePath.GetBuffer());

#else

	HGTools::encryptFile(xmlPath,hgmPath);

#endif




	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}



//С�����������ع���ʹ��
//������Ԫ�������Ƿ���
void CParamDlg::OnBnClickedLdoublecheck()
{
	int changeDisableArr[] = {IDC_STATIC5,IDC_STATIC4,IDC_STATIC6,IDC_MINSBAREA,IDC_KEEPTHICKNESS,IDC_SECENDSPEEDRATE};

	for (int i = 0;i<sizeof(changeDisableArr)/sizeof(int);i++)
	{
		if (m_ldoublecut.GetCheck())
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(FALSE);
		}
	}	
}

#endif
/************************************************************************/
/* �ӹ���Ϣ                                                             */
/************************************************************************/
#if 1

//�ӹ���Ϣ ���ִ��ڳ�ʼ��
void CParamDlg::OnInitDialog_JiaGongXinXi()
{
	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	m_listVecFilter.ResetContent();
	m_listVecFilter.AddString("˫�����");


	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	m_listVecAdder.ResetContent();
	m_listVecAdder.AddString("˫�����");

	//��ת��Ϣ
	control_comchange.InsertString(0,"����ת");
	control_comchange.InsertString(1,"��������Ϣ��ת");
	control_comchange.InsertString(2,"������ȷ�ת");
	control_comchange.InsertString(3,"�������ȷ�ת");
	control_comchange.InsertString(4,"����׶෭ת");
}

//�������ݱ仯������
void CParamDlg::updateToDialog_JiaGongXinXi()
{
	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	m_listVecFilter.ResetContent();
	CStringArray arrVecFilter;
	HGTools::SplitCString(m_vecFilter,arrVecFilter,';');
	for (int i = 0; i<arrVecFilter.GetSize();i++)
	{
		CString vecfilter = arrVecFilter.GetAt(i);
		if (vecfilter != "")
		{
			m_listVecFilter.AddString(vecfilter);
		}
	}
	m_listVecFilter.AddString("˫�����");



	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	m_listVecAdder.ResetContent();
	CStringArray arrVecAdder;
	HGTools::SplitCString(m_vecAdder,arrVecAdder,';');
	for (int i = 0;i<arrVecAdder.GetSize();i++)
	{
		CString vecadd = arrVecAdder.GetAt(i);
		if (vecadd != "")
		{
			m_listVecAdder.AddString(vecadd);
		}
	}
	m_listVecAdder.AddString("˫�����");


}


//�ӹ���Ϣ ���ݼ���
void CParamDlg::loadSettings_JiaGongXinXi()
{

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\VecSolt%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// �����ļ�
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\VecSolt.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif






	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");


	CString inputvalue;

	//��ȡ����
	//�����
	m_isVecInput = atoi(docHandler.getParam("VecUp","1").c_str());

	//�����
	m_isSoltInput = atoi(docHandler.getParam("VecDown","1").c_str());

	//�����
	m_isDVecInput = atoi(docHandler.getParam("SoltUp","1").c_str());

	//�����
	m_isDSoltInput = atoi(docHandler.getParam("SoltDown","1").c_str());

	// �Ƿ�������
	m_isOthershapeInput = atoi(docHandler.getParam("OthershapeInput","1").c_str());

	// �Ƿ�ӹ�ͨ�׿�
	m_isReserveDeepHole = atoi(docHandler.getParam("ReverseDeepHole","1").c_str());

	// �Ƿ�ӹ�ͨ�ײ�
	m_isReserveDeepSlot = atoi(docHandler.getParam("ReverseDeepSlot","1").c_str());

	//�и��
	m_editnum = atoi(docHandler.getParam("ComNum","1").c_str());

	// �ײ���λX
	m_moveX = atof(docHandler.getParam("ChangeX").c_str());

	// �ײ���λY
	m_moveX = atof(docHandler.getParam("ChangeY").c_str());

	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	m_vecFilter = (docHandler.getParam("VecFilter").c_str());

	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	m_vecAdder = (docHandler.getParam("VecAdder").c_str());

	//��ת��Ϣ
	m_comchange = atoi(docHandler.getParam("ComChange").c_str());


	//���г�Ʒ�������������һ���Ƿ���ʾ
	m_outlineColomnShowable = atoi(docHandler.getParam("OutlineColomnShowable","0").c_str());


	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

//�ӹ���Ϣ ���ݼ���
void CParamDlg::saveSettings_JiaGongXinXi()
{


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge�ļ�·����xml�ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\VecSolt%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// �����ļ�
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else
	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\VecSolt.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//����
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif

	

	//�����ļ�
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//���ظ���
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");

	CString inputvalue;




	//��������

	//�����
	inputvalue.Format("%d",m_isVecInput);
	docHandler.setParam("VecUp",inputvalue.GetBuffer());

	//�����
	inputvalue.Format("%d",m_isSoltInput);
	docHandler.setParam("VecDown",inputvalue.GetBuffer());

	//�����
	inputvalue.Format("%d",m_isDVecInput);
	docHandler.setParam("SoltUp",inputvalue.GetBuffer());

	//�����
	inputvalue.Format("%d",m_isDSoltInput);
	docHandler.setParam("SoltDown",inputvalue.GetBuffer());

	// �Ƿ�������
	inputvalue.Format("%d",m_isOthershapeInput);
	docHandler.setParam("OthershapeInput",inputvalue.GetBuffer());

	// �Ƿ�ӹ�ͨ�׿�
	inputvalue.Format("%d",m_isReserveDeepHole);
	docHandler.setParam("ReverseDeepHole",inputvalue.GetBuffer());

	// �Ƿ�ӹ�ͨ�ײ�
	inputvalue.Format("%d",m_isReserveDeepSlot);
	docHandler.setParam("ReverseDeepSlot",inputvalue.GetBuffer());

	//�и��
	inputvalue.Format("%d",m_editnum);
	docHandler.setParam("ComNum",inputvalue.GetBuffer());

	// �ײ���λX
	inputvalue.Format("%f",m_moveX);
	docHandler.setParam("ChangeX",inputvalue.GetBuffer());

	// �ײ���λY
	inputvalue.Format("%f",m_moveX);
	docHandler.setParam("ChangeY",inputvalue.GetBuffer());

	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	inputvalue.Format("%s",m_vecFilter);
	docHandler.setParam("VecFilter",inputvalue.GetBuffer());

	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	inputvalue.Format("%s",m_vecAdder);
	docHandler.setParam("VecAdder",inputvalue.GetBuffer());

	//��ת��Ϣ
	inputvalue.Format("%d",m_comchange);
	docHandler.setParam("ComChange",inputvalue.GetBuffer());

	//���г�Ʒ�������������һ���Ƿ���ʾ
	inputvalue.Format("%d",m_outlineColomnShowable);
	docHandler.setParam("OutlineColomnShowable",inputvalue.GetBuffer());


	//����
	doc.SaveFile(xmlPath);


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
#else

	//HGTools::encryptFile(xmlPath, hgmPath);

#endif




	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
void CParamDlg::OnDblclkVecFilter()
{

	CString str;
	m_listVecFilter.GetText(m_listVecFilter.GetCurSel(),str);
	if (str == "˫�����")
	{
		CVecsoltchangedlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_listVecFilter.AddString(dlg.m_result);
		}
	}
	else
	{
		if (MessageBox("ȷ��ɾ����","��ʾ",MB_OKCANCEL) == IDOK)
		{
			m_listVecFilter.DeleteString(m_listVecFilter.GetCurSel());
		}
	}

	if (m_listVecFilter.GetCount()>0)
	{
		m_vecFilter = "";
		for (int i = 0; i<m_listVecFilter.GetCount()-1 ; i++)
		{
			CString str;
			m_listVecFilter.GetText(i,str);
			m_vecFilter = m_vecFilter + str + ";";
		}
	}
}

//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
void CParamDlg::OnDblclkVecAdder()
{
	CString str;
	m_listVecAdder.GetText(m_listVecAdder.GetCurSel(),str);
	if (str == "˫�����")
	{
		CVecOffset dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_listVecAdder.AddString(dlg.m_result);
		}
	}
	else
	{
		if (MessageBox("ȷ��ɾ����","��ʾ",MB_OKCANCEL) == IDOK)
		{
			m_listVecAdder.DeleteString(m_listVecAdder.GetCurSel());
		}
	}

	if (m_listVecAdder.GetCount()>0)
	{
		m_vecAdder = "";
		for (int i = 0; i<m_listVecAdder.GetCount()-1 ; i++)
		{
			CString str;
			m_listVecAdder.GetText(i,str);
			m_vecAdder = m_vecAdder + str + ";";
		}
	}
}

std::vector<float> CParamDlg::GetVecFilter()
{
	std::vector<float> _cxy;	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	CString sVecFilter = VecFilter();
	CStringArray sVecFilter_Lines;
	HGTools::SplitCString(sVecFilter,sVecFilter_Lines,';');
	for (int line_i = 0;line_i<sVecFilter_Lines.GetSize();line_i++)
	{
		CString vecadd = sVecFilter_Lines.GetAt(line_i);
		if (vecadd != "")
		{
			_cxy.push_back(_ttof(vecadd));
		}
	}
	return _cxy;
}

std::map<float,float> CParamDlg::GetVecAdder()
{
	std::map<float,float> vecAdder;	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	CString sVecAdder = VecAdder();
	CStringArray sVecAdder_Lines;
	HGTools::SplitCString(sVecAdder,sVecAdder_Lines,';');
	for (int line_i = 0;line_i<sVecAdder_Lines.GetSize();line_i++)
	{
		CString vecadd = sVecAdder_Lines.GetAt(line_i);
		if (vecadd != "")
		{
			CStringArray sVecAdder_Item;
			HGTools::SplitCString(vecadd,sVecAdder_Item,',');

			if (sVecAdder_Item.GetSize() == 2)
			{
				CString vecitem = sVecAdder_Item.GetAt(0);
				CString vecvalue = sVecAdder_Item.GetAt(1);
				vecAdder[_ttof(vecitem)] = _ttof(vecvalue);
			}
		}
	}
	return vecAdder;
}


#endif