// ParamSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParamSettingDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
#include "TiebiaoEdgeDlg.h"
#include "../Misc/Misc.h"


#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// ParamSettingDlg �Ի���

IMPLEMENT_DYNAMIC(ParamSettingDlg, CDialogEx)

ParamSettingDlg::ParamSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParamSettingDlg::IDD, pParent)
	, m_tiebiao_offset_x(0)
	, m_tiebiao_offset_y(0)
	, m_tiebiao_auto_vec(FALSE)
	, m_tiebiao_auto_slot(FALSE)
	, m_tiebiao_auto_inner(FALSE)
	, m_tiebiao_auto_outline(FALSE)
	, m_print_per_num(0)
	, m_print_sleep(0)
{


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	m_UISettingHgmFilePath =  HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(strTmp));

#else

	m_UISettingHgmFilePath =  HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\UISetting.hgm"));

#endif


	m_valDecimalPlace = 3;
}

ParamSettingDlg::~ParamSettingDlg()
{
}

void ParamSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIEBIAO_EDGE, m_tiebiao_edge_list);
	DDX_Text(pDX, IDC_TIEBIAO_OFFSET_X, m_tiebiao_offset_x);
	DDX_Text(pDX, IDC_TIEBIAO_OFFSET_Y, m_tiebiao_offset_y);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_VEC, m_tiebiao_auto_vec);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_SLOT, m_tiebiao_auto_slot);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_INNER, m_tiebiao_auto_inner);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_OUTLINE, m_tiebiao_auto_outline);
	DDX_Text(pDX, IDC_PRINT_PERNUM, m_print_per_num);
	DDV_MinMaxInt(pDX, m_print_per_num, 1, 9999);
	DDX_Text(pDX, IDC_PRINT_SLEEP, m_print_sleep);
	DDX_Control(pDX, IDC_COMBO_DECIMAL_PLACE, m_cbxDecimaPlace);
	DDX_CBIndex(pDX, IDC_COMBO_DECIMAL_PLACE, m_valDecimalPlace);
	DDV_MinMaxInt(pDX, m_valDecimalPlace, 0, 4);
	DDX_Check(pDX, IDC_CHECK_PANEL_COMMENTS, m_chkboxPanelComments);
}


BEGIN_MESSAGE_MAP(ParamSettingDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_TIEBIAO_EDGE, &ParamSettingDlg::OnLbnDblclkTiebiaoEdge)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_ADD, &ParamSettingDlg::OnBnClickedTiebiaoEdgeAdd)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_EDIT, &ParamSettingDlg::OnBnClickedTiebiaoEdgeEdit)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_DEL, &ParamSettingDlg::OnBnClickedTiebiaoEdgeDel)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_UP, &ParamSettingDlg::OnBnClickedTiebiaoEdgeUp)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_DOWN, &ParamSettingDlg::OnBnClickedTiebiaoEdgeDown)
END_MESSAGE_MAP()


/************************************************************************/
/* UI ����                                                              */
/************************************************************************/

BOOL ParamSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	loadSettings();

	SetEdgeToUI(Tiebiao_panel_edge());

	UpdateData(FALSE);

	int nItemID = 0;
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("�м�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("���Ͻ�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("���Ͻ�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("���½�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("���½�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->SetCurSel(LabelPosType());

	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->InsertString(0, _T("������������"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->InsertString(1, _T("�����������"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->SetCurSel(LingLiaoSort());

	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->InsertString(0, _T("���޸�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->InsertString(1, _T("�������ںͶ����Ÿ�ʽ�޸�"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->SetCurSel(ReminderModify());

	// ����NC�ļ�С����λ��
	m_cbxDecimaPlace.InsertString(0, _T("1"));
	m_cbxDecimaPlace.InsertString(1, _T("2"));
	m_cbxDecimaPlace.InsertString(2, _T("3"));
	m_cbxDecimaPlace.InsertString(3, _T("4"));
	m_cbxDecimaPlace.SetCurSel(m_valDecimalPlace);



	return TRUE;  
}

void ParamSettingDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{

		Tiebiao_panel_edge(GetEdgeFromUI());

		LabelPosType(((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->GetCurSel());

		LingLiaoSort((LingLiaoSortType)(((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->GetCurSel()));

		ReminderModify((ReminderModifyType)(((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->GetCurSel()));

		saveSettings();
	}


	//CDialogEx::OnOK();
}


void ParamSettingDlg::OnCancel()
{
	//CDialogEx::OnCancel();
}


/************************************************************************/
/* �߼�                                                                 */
/************************************************************************/

long long ParamSettingDlg::m_UISettingLastTime;
TinyXml::TiXmlDocument ParamSettingDlg::m_UISetting_hgm;

//�����ļ���װ
void ParamSettingDlg::ReloadFile()
{
	CString xmlPath = HGTools::getXmlPathFromHgxPath(m_UISettingHgmFilePath);
	long long lastTime = HGTools::GetFileLastTime(m_UISettingHgmFilePath);
	if (m_UISettingLastTime != lastTime)
	{



		// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

		//����
		decrypt_base64(m_UISettingHgmFilePath.GetBuffer(), xmlPath.GetBuffer());
#else

		//����
		//HGTools::decryptFile(m_UISettingHgmFilePath,xmlPath);

#endif



		//�����ļ�
		m_UISetting_hgm.LoadFile(xmlPath);
		//�ر��ļ�
		HGTools::deleteFile(xmlPath);
	}
}

//�����ļ���װ
void ParamSettingDlg::SaveFile()
{
	CString xmlPath = HGTools::getXmlPathFromHgxPath(m_UISettingHgmFilePath);

	//����
	m_UISetting_hgm.SaveFile(xmlPath);

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	//����
	encrypt_base64(xmlPath.GetBuffer(), m_UISettingHgmFilePath.GetBuffer());
#else

	//����
	//HGTools::encryptFile(xmlPath, m_UISettingHgmFilePath);

#endif




	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}

//��������
void ParamSettingDlg::loadSettings()
{
	ReloadFile();

	//���ظ���
	//TinyXml::XmlHandlePlus docHandler(&m_UISetting_hgm);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_UISetting_hgm.RootElement();


	TiXmlElement* elmTheElm = NULL;
	CString strTmp;
	//��ǩ�����߷�������
	//Tiebiao_panel_edge(docHandler.getParam("TiebiaoPanelEdge","").c_str());
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoPanelEdge");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_panel_edge(strTmp);

	//����ƫ��X
	//Tiebiao_offset_x(_ttof(docHandler.getParam("TiebiaoOffsetX","0.0").c_str()));
	strTmp = _T("0.0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoOffsetX");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_offset_x(_ttof(strTmp));

	//����ƫ��Y
	//Tiebiao_offset_y(_ttof(docHandler.getParam("TiebiaoOffsetY","0.0").c_str()));
	strTmp = _T("0.0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoOffsetY");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_offset_y(_ttof(strTmp));

	int nLabelPosType = 0;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoPosType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nLabelPosType);
	LabelPosType(nLabelPosType);

	int nLingLiaoSortType = LingLiaoSortType::LingLiao_SORT_MATERIAL_FIRST;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "LingLiaoSortType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nLingLiaoSortType);
	LingLiaoSort((LingLiaoSortType)nLingLiaoSortType);

	int nReminderModifyType = ReminderModifyType::REMINDER_MODIFY_NO;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ReminderModifyType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nReminderModifyType);
	ReminderModify((ReminderModifyType)nReminderModifyType);

	//�Զ��ܿ�
	//Tiebiao_auto_vec(_ttoi(docHandler.getParam("TiebiaoAutoVec","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoVec");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_vec(_ttoi(strTmp));

	//�Զ��ܲ�
	//Tiebiao_auto_slot(_ttoi(docHandler.getParam("TiebiaoAutoSlot","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoSlot");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_slot(_ttoi(strTmp));

	//�Զ���������
	//Tiebiao_auto_inner(_ttoi(docHandler.getParam("TiebiaoAutoInner","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoInner");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_inner(_ttoi(strTmp));

	//�Զ�����������
	//Tiebiao_auto_outline(_ttoi(docHandler.getParam("TiebiaoAutoOutline","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoOutline");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_outline(_ttoi(strTmp));

	// ÿ��ӡ�����ſ�ʼ��Ϣ
	//Print_per_num(_ttoi(docHandler.getParam("PrintPerNum","1").c_str()));
	strTmp = _T("1");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PrintPerNum");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Print_per_num(_ttoi(strTmp));

	// ÿ��ӡ��������Ϣ���
	//Print_sleep(_ttoi(docHandler.getParam("PrintSleep","200").c_str()));
	strTmp = _T("200");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PrintSleep");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Print_sleep(_ttoi(strTmp));

	// С��λ��
	strTmp = _T("3");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "DecimalPlace");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	setDecimalPlace(_ttoi(strTmp));

	// ���ע��
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PanelComments");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	m_chkboxPanelComments = _ttoi(strTmp);

}

//��������
void ParamSettingDlg::saveSettings()
{
	ReloadFile();

	//���ظ���
	TinyXml::XmlHandlePlus docHandler(&m_UISetting_hgm);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");

	CString inputvalue;

	//��ǩ�����߷�������
	docHandler.setParam("TiebiaoPanelEdge",Tiebiao_panel_edge().GetBuffer());


	//����ƫ��X
	inputvalue.Format("%f",Tiebiao_offset_x());
	docHandler.setParam("TiebiaoOffsetX",inputvalue.GetBuffer());
	//����ƫ��Y
	inputvalue.Format("%f",Tiebiao_offset_y());
	docHandler.setParam("TiebiaoOffsetY",inputvalue.GetBuffer());

	inputvalue.Format("%d",LabelPosType());
	docHandler.setParam("TiebiaoPosType",inputvalue.GetBuffer());

	inputvalue.Format("%d",LingLiaoSort());
	docHandler.setParam("LingLiaoSortType",inputvalue.GetBuffer());

	inputvalue.Format("%d",ReminderModify());
	docHandler.setParam("ReminderModifyType",inputvalue.GetBuffer());

	//�Զ��ܿ�
	inputvalue.Format("%d",Tiebiao_auto_vec());
	docHandler.setParam("TiebiaoAutoVec",inputvalue.GetBuffer());
	//�Զ��ܲ�
	inputvalue.Format("%d",Tiebiao_auto_slot());
	docHandler.setParam("TiebiaoAutoSlot",inputvalue.GetBuffer());
	//�Զ���������
	inputvalue.Format("%d",Tiebiao_auto_inner());
	docHandler.setParam("TiebiaoAutoInner",inputvalue.GetBuffer());
	//�Զ�����������
	inputvalue.Format("%d",Tiebiao_auto_outline());
	docHandler.setParam("TiebiaoAutoOutline",inputvalue.GetBuffer());

	// ÿ��ӡ�����ſ�ʼ��Ϣ
	inputvalue.Format("%d",Print_per_num());
	docHandler.setParam("PrintPerNum",inputvalue.GetBuffer());
	// ÿ��ӡ��������Ϣ���
	inputvalue.Format("%d",Print_sleep());
	docHandler.setParam("PrintSleep",inputvalue.GetBuffer());

	// С��λ��
	inputvalue.Format("%d",getDecimalPlace());
	docHandler.setParam("DecimalPlace",inputvalue.GetBuffer());


	// ���ע��
	inputvalue.Format("%d",m_chkboxPanelComments);
	docHandler.setParam("PanelComments",inputvalue.GetBuffer());



	SaveFile();
}



/************************************************************************/
/* �������                                                             */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//����
#if 1
//˫����ӷ��
void ParamSettingDlg::OnLbnDblclkTiebiaoEdge()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);

		TiebiaoEdgeDlg dlg;
		dlg.OneEdgeString(str);
		if (dlg.DoModal() == IDOK)
		{
			m_tiebiao_edge_list.DeleteString(index);
			m_tiebiao_edge_list.InsertString(index,dlg.OneEdgeString());
		}
	}
}

//��ӷ��
void ParamSettingDlg::OnBnClickedTiebiaoEdgeAdd()
{
	TiebiaoEdgeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_tiebiao_edge_list.AddString(dlg.OneEdgeString());
	}
}

//�༭���
void ParamSettingDlg::OnBnClickedTiebiaoEdgeEdit()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);

		TiebiaoEdgeDlg dlg;
		dlg.OneEdgeString(str);
		if (dlg.DoModal() == IDOK)
		{
			m_tiebiao_edge_list.DeleteString(index);
			m_tiebiao_edge_list.InsertString(index,dlg.OneEdgeString());
		}
	}
}

//ɾ�����
void ParamSettingDlg::OnBnClickedTiebiaoEdgeDel()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0<= index && index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.DeleteString(index);
	}
}

//����������ȼ�
void ParamSettingDlg::OnBnClickedTiebiaoEdgeUp()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0 < index && index <= m_tiebiao_edge_list.GetCount() - 1)
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.DeleteString(index);
		m_tiebiao_edge_list.InsertString(index - 1,str);
		m_tiebiao_edge_list.SetCurSel(index - 1);
	}
}

//����������ȼ�
void ParamSettingDlg::OnBnClickedTiebiaoEdgeDown()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0 <= index && index < m_tiebiao_edge_list.GetCount() - 1)
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.InsertString(index + 2,str);
		m_tiebiao_edge_list.SetCurSel(index + 2);
		m_tiebiao_edge_list.DeleteString(index);
	}
}

//�ӽ����л�÷������
CString ParamSettingDlg::GetEdgeFromUI()
{
	CString result;
	for (int i = 0; i <  m_tiebiao_edge_list.GetCount() ; i++)
	{
		CString str;
		m_tiebiao_edge_list.GetText(i,str);
		str.Replace(";","");

		result = result + str + ";";
	}
	return result;
}

//��÷�����õ�������
void ParamSettingDlg::SetEdgeToUI( CString edge )
{
	CStringArray lines;
	HGTools::SplitCString(edge,lines,';');
	for (int line_i = 0;line_i<lines.GetSize();line_i++)
	{
		CString csoneEdgeString = lines.GetAt(line_i);
		if (csoneEdgeString != "")
		{
			m_tiebiao_edge_list.InsertString(line_i,csoneEdgeString);
		}
	}
}

#endif

