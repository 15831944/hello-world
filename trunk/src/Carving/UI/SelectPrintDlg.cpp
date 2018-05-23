// SelectPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include "SelectPrintDlg.h"
#include "SelMachineDlg.h "
#include "../Misc/ProgramMisc.h"
#include "../Misc/Misc.h"
#include "../Drawing/Drawing.h"
#include "PanelPrintDlg.h"
#include "PrintCodeDlg.h"
#include <algorithm>

#include "../../../include/FileReadWrite/HgmReadWrite/HGTools.h"
#include "../../../include/KnifeDll/KnifeClass.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/FileReadWrite/Misc/HGCode.h"
#include "../../../include/FileReadWrite/Misc/XmlHandlePlus.h"
#include "../../../include/QRCode/qrcode/qrencode.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"



#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"
#include <io.h>

using namespace TinyXml;

//typedef Component* PCOMPONENT;
//bool SortComponentByIDInPanel(const PCOMPONENT& first, const PCOMPONENT& second);

IMPLEMENT_DYNAMIC(CSelectPrintDlg, CDialogEx)

CSelectPrintDlg::CSelectPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectPrintDlg::IDD, pParent)
	, m_PanelCodeEnable(FALSE)
{

}

CSelectPrintDlg::~CSelectPrintDlg()
{

}


void CSelectPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_LIST, m_panelList);									//����б�
	DDX_Control(pDX, IDC_SELECT_ALL_PANEL, m_checkSelectAllPanel);					//ȫѡ���
	DDX_Check(pDX, IDC_SELECT_ALL_PANEL_SETTING, m_checkSelectAllPanelSetting);	//Ĭ���Ƿ�ȫѡ�������

	DDX_Control(pDX, IDC_SMALLPANEL_LIST, m_smallpanelList);						//С���б�
	DDX_Control(pDX, IDC_SELECT_ALL_SMALLPANEL, m_checkSelectAllSmallpanel);		//ȫѡС��
	DDX_Check(pDX, IDC_SELECT_ALL_SMALLPANEL_SETTING, m_checkSelectAllSmallpanelSetting);	//Ĭ���Ƿ�ȫѡС������
	DDX_Control(pDX, IDC_PIC_STYLE, m_picStyle);									//ͼ��ѡ��
	DDX_Check(pDX, IDC_GENERATE_PANEL_CODE, m_PanelCodeEnable);						//����������ʹ��
}

//��ӡ��ǩ
BEGIN_MESSAGE_MAP(CSelectPrintDlg, CDialogEx)
 	ON_BN_CLICKED(IDC_SELECT_ALL_PANEL, &CSelectPrintDlg::OnSelectAllPanel)				//ȫѡ���
 	ON_NOTIFY(NM_CLICK, IDC_PANEL_LIST, &CSelectPrintDlg::OnCheckPanelItem)				//ѡ�д��Ӱ��С��ѡ��
 
 	ON_BN_CLICKED(IDC_SELECT_ALL_SMALLPANEL, &CSelectPrintDlg::OnSelectAllSmallpanel)	//ȫѡС��
 	ON_BN_CLICKED(IDC_PRINT_PANEL, &CSelectPrintDlg::OnBtnPrintPanel)					//��ӡͼ��
	ON_BN_CLICKED(IDC_PRINT_SMALLPANEL, &CSelectPrintDlg::OnBtnPrintSmallPanel)			//��ӡ����
 	ON_BN_CLICKED(IDCANCEL, &CSelectPrintDlg::OnBtnClose)								//�ر�
END_MESSAGE_MAP()


// CSelectPrintDlg message handlers

BOOL CSelectPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	//////////////////////////////////////////////////////////////////////////
	// ����б�
	m_panelList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	m_panelList.InsertColumn(0,"",LVCFMT_CENTER,20);
	m_panelList.InsertColumn(1,"����",LVCFMT_LEFT,50);
	m_panelList.InsertColumn(2,"С������",LVCFMT_LEFT,80);

	//////////////////////////////////////////////////////////////////////////
	// С���б�
	m_smallpanelList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	m_smallpanelList.InsertColumn(0,"",LVCFMT_CENTER,20);
	m_smallpanelList.InsertColumn(1,"����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(2,"С���",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(3,"�ͻ���Ϣ",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(4,"������",LVCFMT_LEFT,80);
	m_smallpanelList.InsertColumn(5,"�����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(6,"�����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(7,"�����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(8,"�������",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(9,"�������",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(10,"���",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(11,"�����Ϣ",LVCFMT_LEFT,100);
	m_smallpanelList.InsertColumn(12,"��Ʒ��",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(13,"��Ʒ��",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(14,"����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(15,"����",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(16,"���Ǻ�",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(17,"˫��",LVCFMT_LEFT,50);
	m_smallpanelList.InsertColumn(18,"���˵�",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(19,"��Ʒ����",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(20,"��������",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(21,"��������",LVCFMT_LEFT,80);
	m_smallpanelList.InsertColumn(22,"�ͻ���ַ",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(23,"��۱�ʶ",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(24,"������",LVCFMT_LEFT,120);
	m_smallpanelList.InsertColumn(25,"�Ƿ�ת",LVCFMT_LEFT,120);

	//////////////////////////////////////////////////////////////////////////
	// ���ش���б�����
	InitPanelList();

	//////////////////////////////////////////////////////////////////////////
	// ����С���б�����
	InitComponentList();

	//////////////////////////////////////////////////////////////////////////
	// ��ӡ��ͼ��ʽ
	m_picStyle.InsertString(0,"A4-1��ͼ");
	m_picStyle.InsertString(1,"A4-4��ͼ");
	m_picStyle.SetCurSel(0);

	//////////////////////////////////////////////////////////////////////////
	// Ĭ��ѡ��
	//��ȡ����
	loadSettings_DefaultSelect();
	//���������õ����棨���ȫѡ��С��ȫѡ��
	m_checkSelectAllPanel.SetCheck(m_checkSelectAllPanelSetting);
	m_checkSelectAllSmallpanel.SetCheck(m_checkSelectAllSmallpanelSetting);
	//����on�¼��Խ���Ԫ�ؽ���ѡ��
	OnSelectAllPanel();
	OnSelectAllSmallpanel();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}






//ȫѡ���
void CSelectPrintDlg::OnSelectAllPanel()
{
	if(m_checkSelectAllPanel.GetCheck() == 1)
	{
		for (int i = 0;i < m_panelList.GetItemCount();i++)
		{
			m_panelList.SetCheck(i,true);
		}
	}
	else
	{
		for (int i = 0;i < m_panelList.GetItemCount();i++)
		{
			m_panelList.SetCheck(i,false);
		}
	}
}




//ȫѡ����
void CSelectPrintDlg::OnSelectAllSmallpanel()
{

	if(m_checkSelectAllSmallpanel.GetCheck() == 1)
	{
		for (int i = 0;i < m_smallpanelList.GetItemCount();i++)
		{
			m_smallpanelList.SetCheck(i,true);
		}
	}
	else
	{
		for (int i = 0;i < m_smallpanelList.GetItemCount();i++)
		{
			m_smallpanelList.SetCheck(i,false);
		}
	}
}


//����Ĭ��ѡ������
void CSelectPrintDlg::loadSettings_DefaultSelect()
{

	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	//�����ļ�·��
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	hgePath = HGTools::getRealPathFromReleativePath(hgePath);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//����
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

#else

	//�����ļ�·��
	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	hgmPath = HGTools::getRealPathFromReleativePath(hgmPath);
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


	//Ĭ���Ƿ�ȫѡ�������
	m_checkSelectAllPanelSetting = atoi(docHandler.getParam("SelectAllPanelSetting","1").c_str());

	//Ĭ���Ƿ�ȫѡС������
	m_checkSelectAllSmallpanelSetting = atoi(docHandler.getParam("SelectAllSmallpanelSetting","1").c_str());


	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}



//���ô�ѡ�д��
void CSelectPrintDlg::setPanelListCheck(int bigPanelNum)
{
	CString strPanel;
	strPanel.Format("%d",bigPanelNum);
	for (int i = 0;i < m_panelList.GetItemCount();i++)
	{
		if (strPanel == m_panelList.GetItemText(i,1))
		{
			m_panelList.SetCheck(i,true);
		}
		else
		{
			m_panelList.SetCheck(i,false);
		}
	}
}

//���ô�ѡ�д������С��
void CSelectPrintDlg::setPanelCheck(int panelNum)
{
	CString strPanel;
	strPanel.Format("%d",panelNum);
	for (int i = 0;i < m_smallpanelList.GetItemCount();i++)
	{
		if (strPanel == m_smallpanelList.GetItemText(i,1))
		{
			m_smallpanelList.SetCheck(i,true);
		}
		else
		{
			m_smallpanelList.SetCheck(i,false);
		}
	}
}

//TODO: ������ƿ�����Ҫ���ģ���ʱ�����ģ���Ϊû�����������
//��ȡ����NC������ļ�������Ϣ
std::map<Panel*,std::map<CString,CString>> CSelectPrintDlg::OnOutputgcodeFileName(vector<Panel*> vPanel, bool enable)
{
	//��ö�Ӧ����ӵ�е�����nc�ļ���
	std::map<Panel*,std::map<CString,CString>> mapWithBarCode;
	if (!enable)
	{
		return mapWithBarCode;
	}


	std::string node_name = "";
	int node_print = 0;
	bool node_sysuse = false;
	bool node_double = false;
	std::string node_groupstr = "";

	bool _cutDouble = false;//��˫��λ�ж�

	std::string node_cxhx = "";
	std::string node_pxhx = "";

	std::string node_pname = "";
	std::string node_plast = "";
	std::string node_cname = "";
	std::string node_clast = "";
	std::string node_dvcut = "";

	int _iscombine = 0;
	int _isComline = 1;
	int _isComball = 0;
	int _lessKnifeChange = 0;
	CSelMachineDlg dlg("ѡ�����������ɹ���");
	if (dlg.DoModal() == IDOK)
	{
		node_name = dlg.m_nodename;
		node_print = atoi(dlg.m_printname.c_str());
		node_sysuse = dlg._coorsysuse;
		node_double = dlg._cutdouble;
		node_groupstr = dlg._groupstr;

		node_pname = dlg.m_pname;
		node_plast = dlg.m_plast;
		node_cname = dlg.m_cname;
		node_clast = dlg.m_clast;
		node_dvcut = dlg.m_dvcut;//0,�����ף�1˫����1��W��,2˫����2��H��
		node_pxhx = dlg.m_pxhx;
		node_cxhx = dlg.m_cxhx;


		_iscombine = dlg.m_iscombine;//�Ƿ�ϲ�����ͷβ
		_isComline = dlg.m_iscomline;//�Ƿ�����NC�ļ�
		_isComball = dlg.m_iscomball;


	}



	if (node_name == "")
	{
		return mapWithBarCode;
	}

	std::map<int,int> print_map;



	//nc�ļ�������
	{
		CString pathName = HGTools::getRelativePath("\\kdata");

		if (vPanel.size() != 0)
		{
			int key = 1;
			//for (auto iter_map = coor.begin();iter_map != coor.end();iter_map++)
			for(vector<Panel*>::iterator itr = vPanel.begin(); itr != vPanel.end(); itr++)
			{
				//auto iter_next_map2 = iter_map;
				vector<Panel*>::iterator itrNext2 = itr;
				//iter_next_map2++;
				itrNext2++;


				std::string tempnode_pname = "";
				std::string tempnode_plast = "";
				std::string tempnode_cname = "";
				std::string tempnode_clast = "";

				//Panel_Knife pk = iter_map->second;
				//PanelNode * node = pk._panel;
				//KnifeClass * kc = pk._kc;
				Panel* pThePanel = *itr;

				//����NC�ļ���
#if 1
				std::string str_thinmatrial = /*kc->getPanelMatrial()*/pThePanel->m_Material;
				CString s_thinmatrial,s_thin,s_nextthin;
				std::string str_nextthin = "";//��һ�����İ��
				std::string str_nextthinmatrial = "";//��һ���Ĳ���
				s_thinmatrial.Format("%s",str_thinmatrial.c_str());
				s_thin.Format("%0.0f",/*kc->getThickness()*/pThePanel->m_Thickness);
				s_thinmatrial.Replace("\/","");
				s_thinmatrial.Replace("\\","");
				str_thinmatrial = s_thinmatrial.GetBuffer();
				std::string str_thin = s_thin.GetBuffer();
				//auto iter_next_map = iter_map;
				//iter_next_map++;
				vector<Panel*>::iterator itrNext = itr;
				itrNext++;
				if (/*iter_next_map != coor.end()*/itrNext != vPanel.end())
				{
					//KnifeClass * nextkc = iter_next_map->second._kc;
					s_nextthin.Format("%0.1f",/*nextkc->getThickness()*/(*itrNext)->m_Thickness);
					str_nextthin = s_nextthin.GetBuffer();
					str_nextthinmatrial = /*nextkc->getPanelMatrial()*/(*itrNext)->m_Material;
				}
				CString s_nextmaterial;
				s_nextmaterial.Format("%s",str_nextthinmatrial.c_str());
				s_nextthin.Format("%s",str_nextthin.c_str());
				s_nextmaterial.Replace("\/","");
				s_nextmaterial.Replace("\\","");
				str_nextthinmatrial = s_nextmaterial.GetBuffer();
				std::string filename,filenamed,filenamed1,allfile,pfilename,filenameo,pfilenameo,filenamedo,filenamed1o;
				char ch_filename[1000],ch_filenamed[1000],ch_filenamed1[1000], ch_allfile[1000],ch_pfilename[1000],ch_filenameo[1000],ch_pfilenameo[1000],ch_filenamedo[1000],ch_filenamed1o[1000];
				char ch_allFileName[1000];
				char theKey[1000],theKey2[1000];//theKey2��ʾ��һ��������ˮ��
				if (key<10)
				{
					sprintf_s(theKey,"0%d",key);
				}
				else
					sprintf_s(theKey,"%d",key);

				if (key+1<10)
				{
					sprintf_s(theKey2,"0%d",key+1);
				}
				else
					sprintf_s(theKey2,"%d",key+1);

				std::string tempcname2;//�ļ���+��ˮ��+�������+������cname
				std::string temppname2;//�ļ���+��ˮ��+�������+������pname
				std::string tempnextcname2;//�ļ���+��ˮ��+�������+������cname
				std::string tempnextpname2;//�ļ���+��ˮ��+�������+������pname
				std::string theAllCname;
				theAllCname = node_cname;
				//����nc������ļ���
				{				 
					if (node_cname.find("@")!=std::string::npos)//nc�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = node_cname.find_first_of('@');
						std::string theCname = node_cname.substr(0,thePos1);//nc�ļ���
						std::string theCname2 = node_cname.substr(0,thePos1);//nc�ļ���
						theAllCname = theCname2;
						std::string theCMaterial = str_thinmatrial;//����
						std::string theCThickness = str_thin;//���
						std::string thenextCMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextCThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextCThickness = HGTools::Convert2CString(thenextCThickness);
						cstr_thenextCThickness.Replace(".0","");
						thenextCThickness = HGTools::Convert2StdString(cstr_thenextCThickness);


						if (node_cname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							theCname = theCname + node_cxhx +theCMaterial;
							tempcname2 = theCname2 + node_cxhx + theKey + node_cxhx+theCMaterial; 
							tempnextcname2 = theCname2+ node_cxhx + theKey2+ node_cxhx + thenextCMaterial;
						}
						if (node_cname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							theCname = theCname + node_cxhx +theCThickness + node_cxhx;
							if (tempcname2=="")
							{
								tempcname2 = theCname2 + node_cxhx + theKey+ node_cxhx + theCThickness;
							}
							else
								tempcname2 = tempcname2+ node_cxhx + theCThickness;
							if (tempnextcname2=="")
							{
								tempnextcname2 = theCname2 + node_cxhx + theKey2 + node_cxhx+ thenextCThickness;
							}
							else
								tempnextcname2 = tempnextcname2 + node_cxhx + thenextCThickness;
						}
						tempnode_cname = theCname+ node_cxhx + theKey;
					}
					else
					{
						tempnode_cname = node_cname + node_cxhx+theKey;
						tempcname2 = node_cname + node_cxhx+ theKey; 
						tempnextcname2 = node_cname + node_cxhx+ theKey2;
					}
					if (node_pname.find("@")!=std::string::npos)//p�ļ�������Ҫ�в��ʻ��߰��
					{
						int thePos1 = node_pname.find_first_of('@');
						std::string thePname = node_pname.substr(0,thePos1);//p�ļ���
						std::string thePname2 = node_pname.substr(0,thePos1);//p�ļ���
						std::string thePMaterial = str_thinmatrial;//����
						std::string thePThickness = str_thin;//���
						std::string thenextPMaterial = str_nextthinmatrial;//��һ��������
						std::string thenextPThickness = str_nextthin;//��һ�������
						//��������ǡ�.0��ȥ����.0����������ȡ�0.n����������.n��
						CString cstr_thenextPThickness = HGTools::Convert2CString(thenextPThickness);
						cstr_thenextPThickness.Replace(".0","");
						thenextPThickness = HGTools::Convert2StdString(cstr_thenextPThickness);


						if (node_pname.find("@Material@")!=std::string::npos)//���ڲ���
						{
							thePname = thePname + node_pxhx +thePMaterial;
							temppname2 = thePname2 + node_pxhx + theKey + node_pxhx+thePMaterial; 
							tempnextpname2 = thePname2 + node_pxhx + theKey2 + node_pxhx + thenextPMaterial;
						}
						if (node_pname.find("@Thickness@")!=std::string::npos)//���ڰ��
						{
							thePname = thePname + node_pxhx +thePThickness + node_pxhx;
							if (temppname2=="")
							{
								temppname2 = thePname2 + node_pxhx + theKey + node_pxhx + thePThickness;
							}else
								temppname2 = temppname2 + node_pxhx + thePThickness;
							if (tempnextpname2=="")
							{
								tempnextpname2 = thePname2+ node_pxhx + theKey2 + node_pxhx + thenextPThickness;
							}else
								tempnextpname2 = tempnextpname2 + node_pxhx+ thenextPThickness;
						}
						tempnode_pname = thePname+ node_pxhx + theKey;
					}
					else
					{
						tempnode_pname = node_pname + node_pxhx +theKey;
						temppname2 = node_pname + node_pxhx + theKey; 
						tempnextpname2 = node_pname + node_pxhx + theKey2;
					}
					//sprintf_s(ch_allfile,"\\All-%s.nc",tempcname2.c_str());
					sprintf_s(ch_filename,"%s%s",tempcname2.c_str(),node_clast.c_str());
					sprintf_s(ch_pfilename,"%s%s",temppname2.c_str(),node_plast.c_str());

					sprintf_s(ch_filenameo,"%s%s",tempcname2.c_str(),node_clast.c_str());
					sprintf_s(ch_pfilenameo,"%s%s",temppname2.c_str(),node_plast.c_str());
				}

				theAllCname = theAllCname + node_cxhx + str_thinmatrial + node_cxhx + str_thin;//�ϲ�ͬһ���ͺ���ļ���
				sprintf_s(ch_allFileName,"All%s.nc",theAllCname.c_str());
				tempnextpname2 = tempnextpname2;
				tempnextcname2 = tempnextcname2;

				std::string thCnameW = tempcname2+"@W";
				std::string thCnameH = tempcname2+"@H";
				std::string thCname_W = tempcname2+"@-W";
				std::string thCname_H = tempcname2+"@-H";
				sprintf_s(ch_filenamed,"%s%s",thCnameW.c_str(),node_clast.c_str());
				sprintf_s(ch_filenamed1,"%s%s",thCnameH.c_str(),node_clast.c_str());
				sprintf_s(ch_filenamedo,"%s%s",thCname_W.c_str(),node_clast.c_str());
				sprintf_s(ch_filenamed1o,"%s%s",thCname_H.c_str(),node_clast.c_str());

				filename = ch_filename;//nc
				pfilename = ch_pfilename;//p
				filenamed = ch_filenamed;//@W
				filenamed1 = ch_filenamed1;//@H
				allfile = ch_allfile;
				filenameo = ch_filenameo;//-nc
				pfilenameo = ch_pfilenameo;//-p
				filenamedo = ch_filenamedo;//@-W
				filenamed1o = ch_filenamed1o;//@-H

				std::string print_str1,print_str2,print_str3;
				char ch_print_str1[1000],ch_print_str2[1000],ch_print_str3[1000];
				sprintf_s(ch_print_str1,"%s%s",tempcname2.c_str(),node_clast.c_str());



				sprintf_s(ch_print_str3,"%s%s",tempnextcname2.c_str(),node_clast.c_str());
				sprintf_s(ch_print_str2,"%s%s",tempnextpname2.c_str(),node_plast.c_str());
				print_str1 = ch_print_str1;
				if (key+1 > /*coor.size()*/vPanel.size())
				{
					print_str2 = "";
					print_str3 = "";
				}
				else
				{
					print_str2 = ch_print_str2;
					print_str3 = ch_print_str3;
				}

				//׼��������������ļ���
				std::string path=pathName.GetBuffer(0) + filename;//nc			//nc�ļ�
				std::string pathp = pathName.GetBuffer(0) + pfilename;//p		//�Զ�����nc�ļ�
				std::string patho=pathName.GetBuffer(0) + filenameo;//-nc		//nc��ʱ�ļ���
				std::string pathpo = pathName.GetBuffer(0) + pfilenameo;//-p	//�Զ�������ʱ�ļ�
				std::string pathd = pathName.GetBuffer(0) + filenamed;//@W		//W��תnc�ļ�
				std::string pathd1 = pathName.GetBuffer(0) + filenamed1;//@H	//H��תnc�ļ�
				std::string allpath = pathName.GetBuffer(0) + allfile;
				std::string pathdo = pathName.GetBuffer(0) + filenamedo;//@-W	//W��תnc��ʱ�ļ�
				std::string pathd1o = pathName.GetBuffer(0) + filenamed1o;//@-H	//H��תnc��ʱ�ļ�
				std::string tempFilename = ch_allFileName;
				std::string allpath2 = pathName.GetBuffer(0) + tempFilename;	//�ϲ�NC�ļ�
				//std::vector<KnifeType> kt;


				std::map<CString,CString> currentPanelNCFileNames;
				currentPanelNCFileNames["nc"]= (filename).c_str();//nc
				currentPanelNCFileNames["tb"] = (pfilename).c_str();//p
				currentPanelNCFileNames["@W"] = (filenamed).c_str();//@W
				currentPanelNCFileNames["@H"] = (filenamed1).c_str();//@H
				currentPanelNCFileNames["all"] = (tempFilename).c_str();


#endif

				// �滻���е���hg3d����

				float _changex, _changey;



				// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

				//�����ļ�·��
				CString strTmp;
				strTmp.Format(_T("kdata\\VecSolt%s"), g_szEncyptSubfix);
				CString hgePath_VecSolt = HGTools::getRelativePath(strTmp);
				CString xmlPath_VecSolt = HGTools::getXmlPathFromHgxPath(hgePath_VecSolt);

				//����
				decrypt_base64(hgePath_VecSolt.GetBuffer(), xmlPath_VecSolt.GetBuffer());

#else

				//�����ļ�·��
				CString hgmPath_VecSolt = HGTools::getRelativePath("kdata\\VecSolt.hgm");
				CString xmlPath_VecSolt = HGTools::getXmlPathFromHgxPath(hgmPath_VecSolt);

				//����
			//	HGTools::decryptFile(hgmPath_VecSolt, xmlPath_VecSolt);

#endif




				//�����ļ�
				TinyXml::TiXmlDocument org_doc_VecSolt;
				org_doc_VecSolt.LoadFile(xmlPath_VecSolt);

				TiXmlElement* pRoot_VecSolt = org_doc_VecSolt.RootElement();

				// �ж�
				if (pRoot_VecSolt != NULL)
				{
					for (TiXmlElement * pCurNode = pRoot_VecSolt->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
					{
						string strCurNodeName =  pCurNode->Attribute("Name");
						string str_value;

						if (strCurNodeName == "ChangeX")
						{
							str_value = pCurNode->Attribute("Value");
							_changex = atof(str_value.c_str());
						}
						else if (strCurNodeName == "ChangeY")
						{
							str_value = pCurNode->Attribute("Value");
							_changey = atof(str_value.c_str());
						}
						else
						{

						}
					}
				}


				vector<float> _cxy;

				// �滻���е���hg3d����

				// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

				//�����ļ�·��
				//CString strTmp;
				strTmp.Format(_T("kdata\\VecSoltChange%s"), g_szEncyptSubfix);
				CString hgePath_VecSoltChange = HGTools::getRelativePath(strTmp);
				CString xmlPath_VecSoltChange = HGTools::getXmlPathFromHgxPath(hgePath_VecSoltChange);

				//����
				decrypt_base64(hgePath_VecSoltChange.GetBuffer(), xmlPath_VecSoltChange.GetBuffer());

#else

				//�����ļ�·��
				CString hgmPath_VecSoltChange = HGTools::getRelativePath("kdata\\VecSoltChange.hgm");
				CString xmlPath_VecSoltChange = HGTools::getXmlPathFromHgxPath(hgmPath_VecSoltChange);

				//����
				//HGTools::decryptFile(hgmPath_VecSoltChange, xmlPath_VecSoltChange);

#endif







				//�����ļ�
				TinyXml::TiXmlDocument org_doc_VecSoltChange;
				org_doc_VecSoltChange.LoadFile(xmlPath_VecSoltChange);


				TiXmlElement* pRoot_VecSoltChange = org_doc_VecSoltChange.RootElement();

				// �ж�
				if (pRoot_VecSoltChange != NULL)
				{
					for (TiXmlElement * pCurNode = pRoot_VecSoltChange->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
					{
						string strCurNodeName =  pCurNode->Value();

						if (strCurNodeName == "Param")
						{
							string str_size = pCurNode->Attribute("Size");
							_cxy.push_back(atof(str_size.c_str()));
						}

					}
				}


				//��λƫ�Ƽ���
				bool HaveDvsh = false,HaveDvsw = false,HaveDvs = false;

				//���������G����
				if (node_dvcut=="0")//������
				{
					HaveDvsw = FALSE;
					HaveDvs = FALSE;
				}else//˫����
				{
					if (_cutDouble == false)//����λ
					{
					}
					else//˫��λ
					{
						if (key%2!=0)//��λ2
						{
						}
						else//��λ1
						{
						}
					}

				}



				key++;

				{//@Wʱ
					if (node_dvcut == "0")//�����׽��ļ�ͷβ�����ļ�
					{}
					if (node_dvcut=="1")//˫����1��w��
					{
						currentPanelNCFileNames["@H"] = "";
						if (_iscombine==1)//ɾ�������ļ�
						{
							currentPanelNCFileNames["@W"] = "";
						}
					}
				}
				{//@Hʱ
					if (node_dvcut=="2")//˫����2��w��
					{
						currentPanelNCFileNames["@W"] = "";
						if (_iscombine==1)//ɾ�������ļ�
						{
							currentPanelNCFileNames["@H"] = "";
						}	
					}	
				}


				mapWithBarCode[pThePanel] = currentPanelNCFileNames;

			}


			if (_isComball==1)
			{
			}
			else
			{
			}
		}
		else
		{
		}
	}
	return mapWithBarCode;
}



//����Ĭ��ѡ������
void CSelectPrintDlg::saveSettings_DefaultSelect()
{
	//�����ļ�·��


	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	hgePath = HGTools::getRealPathFromReleativePath(hgePath);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//����
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

#else

	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	hgmPath = HGTools::getRealPathFromReleativePath(hgmPath);
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

	//Ĭ���Ƿ�ȫѡ�������
	inputvalue.Format("%d",m_checkSelectAllPanelSetting);
	docHandler.setParam("SelectAllPanelSetting",inputvalue.GetBuffer());

	//Ĭ���Ƿ�ȫѡС������
	inputvalue.Format("%d",m_checkSelectAllSmallpanelSetting);
	docHandler.setParam("SelectAllSmallpanelSetting",inputvalue.GetBuffer());



	//����
	doc.SaveFile(xmlPath);



	// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

	//����
	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
#else

	//����
	HGTools::encryptFile(xmlPath, hgmPath);

#endif





	//�ر��ļ�
	HGTools::deleteFile(xmlPath);
}



//ѡ������Ŀ��ͬʱѡ��С����Ŀ
void CSelectPrintDlg::OnCheckPanelItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);


	CRect rc;
	if(pNMItemActivate->iItem!=-1 && pNMItemActivate->iSubItem == 0)
	{
		if (m_panelList.GetCheck(pNMItemActivate->iItem) == false)
		{
			CString panel_no = m_panelList.GetItemText(pNMItemActivate->iItem,1);
			for (int i = 0;i < m_smallpanelList.GetItemCount();i++)
			{
				if (panel_no == m_smallpanelList.GetItemText(i,1))
				{
					m_smallpanelList.SetCheck(i,true);
				}
			}
		}
		if (m_panelList.GetCheck(pNMItemActivate->iItem) == true)
		{
			CString panel_no = m_panelList.GetItemText(pNMItemActivate->iItem,1);
			for (int i = 0;i < m_smallpanelList.GetItemCount();i++)
			{
				if (panel_no == m_smallpanelList.GetItemText(i,1))
				{
					m_smallpanelList.SetCheck(i,false);
				}
			}
		}		
	}

	*pResult = 0;
}



//�ر�
void CSelectPrintDlg::OnBtnClose()
{
	//////////////////////////////////////////////////////////////////////////
	// Ĭ��ѡ��
	//�������ݱ��浽�ļ�
	UpdateData(TRUE);
	saveSettings_DefaultSelect();

	CDialogEx::OnCancel();
}




//������ά��
void CSelectPrintDlg::CreateQCodeInImage( CImage &image, CString codeWord, int code_x, int code_y , float code_size )
{
	//��NC��ת��ά����ʶ���ַ������� - begin
	char szSourceSring[200];
	HGTools::Convert2FillChar(codeWord,szSourceSring);


	std::string result;
	HGCode::GB2312_To_UTF8(result,szSourceSring,strlen(szSourceSring));
	std::string str_bar_code = result;


	for (int q = 0;q < str_bar_code.length();q++)
	{
		*(szSourceSring + q) = str_bar_code.at(q);
	}
	*(szSourceSring + str_bar_code.length()) = '\0';
	//��NC��ת��ά����ʶ���ַ������� - end


	unsigned char*	pSourceData;
	QRcode*			pQRC;

	int code_pixel = 1;
	float code_plus = code_size;

	if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
	{
		int unWidth = pQRC->width;

		int p_x = 0;
		int p_y = 0;

		CDC* pDC=CDC::FromHandle(image.GetDC());
		int _paperWidth = image.GetWidth();
		int _paperHeight = image.GetHeight();


		CPen pen(PS_SOLID,1,RGB(0,0,0));
		CPen pen_1(PS_DASH,1,RGB(0,0,0));
		CPen pen_2(PS_SOLID,2,RGB(0,0,0));
		pDC->SelectObject(pen_1);
		//pDC->Rectangle(1,1,_paperWidth-2,_paperHeight-2);
		pDC->SelectObject(pen);

		pSourceData = pQRC->data;
		for (int c_i = 0;c_i < unWidth;c_i++)
		{
			for (int c_j = 0;c_j < unWidth;c_j++)
			{
				for (int c_k = 0;c_k < code_pixel;c_k++)
				{
					p_x = c_i * code_pixel + c_k;
					for(int c_m = 0;c_m < code_pixel;c_m++)
					{
						p_y = c_j * code_pixel + c_m;

						if (*pSourceData & 1)
						{
							pDC->SetPixel(p_y+code_x , p_x+code_y , RGB(0,0,0));
						}
						else
						{
							pDC->SetPixel(p_y+code_x , p_x+code_y , RGB(255,255,255));
						}
					}

				}
				pSourceData++;
			}
		}

		pDC->StretchBlt( code_x , code_y , 40*code_plus , 40*code_plus , pDC ,code_x , code_y , unWidth , unWidth , SRCCOPY );
		QRcode_free(pQRC);
		image.ReleaseDC();
	}
}

//��ӡ��ͼ
void CSelectPrintDlg::OnBtnPrintPanel()
{
	vector<int> check_num;//�������

	for (int i = 0;i < m_panelList.GetItemCount();i++)
	{
		if (m_panelList.GetCheck(i) == true)
			check_num.push_back(i);
	}


	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	file_path = file_path + _T("\\picture");

	int _panelno = 0;
	CPanelPrintDlg dlg;

	CFont font200;  
	font200.CreatePointFont(200, _T("Arial"));
	CFont font80;  
	font80.CreatePointFont(80, _T("Arial"));
	if (m_picStyle.GetCurSel() == 0)//A4-1��ͼ
	{
		for (int i = 0;i < check_num.size();i++)
		{
			CString src_file_name,file_name;
			src_file_name = "design.bmp";
			file_name.Format("design-%d.bmp",i);

			CImage image;
			image.Load(file_path + "\\" + src_file_name);

			//DrawPanelInfoToDC(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i)), 2.2, 10, 10, check_num.at(i)+1, /*&font200*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);
			CRect rcDrawingArea;
			rcDrawingArea.SetRect(0, 0, A4_TEMPLATE_X, A4_TEMPLATE_Y);
			DrawPanelDiagramToA4(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i)), rcDrawingArea, check_num.at(i)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);

			image.Save(file_path + "\\" + file_name);
			image.ReleaseDC();
			image.Destroy();
		}
		dlg.m_panelNum = check_num.size();
	}
	else if (m_picStyle.GetCurSel() == 1)//A4-4��ͼ
	{
		for (int i = 0;i < check_num.size();i=i+4)
		{
			CString src_file_name,file_name;
			src_file_name = "design.bmp";
			file_name.Format("design-%d.bmp",(int)(i/4));

			CImage image;
			image.Load(file_path + "\\" + src_file_name);

			if (i < check_num.size())
			{
				//DrawPanelInfoToDC(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i)), 4.5, 10, 10, check_num.at(i)+1, /*&font80*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);
				CRect rcDrawingArea;
				rcDrawingArea.SetRect(0, 0, A4_TEMPLATE_X/2, A4_TEMPLATE_Y/2);
				DrawPanelDiagramToA4(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i)), rcDrawingArea, check_num.at(i)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);

				image.ReleaseDC();
			}
			if (i+1 < check_num.size())
			{
				//DrawPanelInfoToDC(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+1)), 4.5, 604, 10, check_num.at(i+1)+1, /*&font80*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);
				CRect rcDrawingArea;
				rcDrawingArea.SetRect(A4_TEMPLATE_X/2, 0, A4_TEMPLATE_X, A4_TEMPLATE_Y/2);
				DrawPanelDiagramToA4(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+1)), rcDrawingArea, check_num.at(i+1)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);

				image.ReleaseDC();
			}
			if (i+2 < check_num.size())
			{
				//DrawPanelInfoToDC(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+2)), 4.5, 10, 430, check_num.at(i+2)+1, /*&font80*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);
				CRect rcDrawingArea;
				rcDrawingArea.SetRect(0, A4_TEMPLATE_Y/2, A4_TEMPLATE_X/2, A4_TEMPLATE_Y);
				DrawPanelDiagramToA4(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+2)), rcDrawingArea, check_num.at(i+2)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);

				image.ReleaseDC();
			}
			if (i+3 < check_num.size())
			{
				//DrawPanelInfoToDC(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+3)), 4.5, 604, 430, check_num.at(i+3)+1, /*&font80*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);
				CRect rcDrawingArea;
				rcDrawingArea.SetRect(A4_TEMPLATE_X/2, A4_TEMPLATE_Y/2, A4_TEMPLATE_X, A4_TEMPLATE_Y);
				DrawPanelDiagramToA4(image.GetDC(), (Panel*)m_panelList.GetItemData(check_num.at(i+3)), rcDrawingArea, check_num.at(i+3)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_4);

				image.ReleaseDC();
			}

			image.Save(file_path + "\\" + file_name);
			
			image.Destroy();
		}
		dlg.m_panelNum = (check_num.size()-1)/4+1;
	}

	font200.DeleteObject();
	font80.DeleteObject();

	dlg.DoModal();
}








//��ӡ����
void CSelectPrintDlg::OnBtnPrintSmallPanel()
{
	UpdateData(TRUE);

	vector<Panel*> vAllPanel;
	GetAllPanelInPanelList(vAllPanel);

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	file_path = file_path + _T("\\log.txt");
	std::string str_file_path = file_path.GetBuffer();
	
	CPrintCodeDlg dialog;

	{//����ͼ����Ϣ - begin
		CString strConfigFile = GetModulePath() +  _T("picture");

		// �µļӽ��ܷ�ʽ
#if (NEW_ENCRYPT_BASE64 == 1)

		CString strTmp;
		strTmp.Format(_T("\\printcode%s"), g_szEncyptSubfix);
		strConfigFile += strTmp;
		CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);

		decrypt_base64(strConfigFile.GetBuffer(), xmlpath.GetBuffer());


#else

		strConfigFile += _T("\\printcode.hgm");
		CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
		//HGTools::decryptFile(strConfigFile,xmlpath);

#endif






		XmlHandler xmlHandler;
		bool bLoadSuccess = xmlHandler.LoadFile(xmlpath.GetBuffer());
		HGTools::deleteFile(xmlpath);

		if (!bLoadSuccess)
		{
			AfxMessageBox("��ȡprintcode.xmlʧ��",MB_OK);
			return;
		}

		TiXmlElement* elmUsed = NULL; 
		TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
		vector<TiXmlElement*> vChildNodes = xmlHandler.GetChildElms(elmRoot, NULL);

		for (int i = 0;i < vChildNodes.size();i++)
		{
			TiXmlElement* elmChild = vChildNodes[i];
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmChild, "Used", strTmp);
			if (strTmp.CompareNoCase(_T("1")) == 0)
			{
				elmUsed = elmChild;
				break;
			}
		}

		if (elmUsed == NULL)
		{
			AfxMessageBox("δ��ȡ������ʹ�õĴ�ӡģ��",MB_OK);
			return;
		}
		//����ͼ����Ϣ - end




		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture");
		//CString strTmp;
		/*print_node->GetAttributeValue("pictureName")*/xmlHandler.GetXmlAttribute(elmUsed, "pictureName", strTmp);
		std::string s_f = strTmp;


		char ExeFile2[2000];
		GetModuleFileName(NULL,ExeFile2,500);
		CString file_path2 = ExeFile;
		file_path2 = file_path2.Left(file_path2.ReverseFind('\\'));
		file_path2 = file_path2 + _T("\\picture");

		int _panelno = 0;
		vector<int> check_num;
		for (int i = 0;i < m_panelList.GetItemCount();i++)
		{
			if (m_panelList.GetCheck(i) == true)
				check_num.push_back(i);
		}

		bool bDiagramUsed = false;
		int nDiagramXLen = 0, nDiagramYLen = 0, nDiagramRotateAngle = 0;
		CString strDiagramTmpDir;
		TiXmlElement* elmDiagram = xmlHandler.GetIndicatedElement(elmUsed, "Parameter", "Name", "�Ű�ͼ");
		xmlHandler.GetXmlAttribute(elmDiagram, "Used", bDiagramUsed);
		if(elmDiagram != NULL && bDiagramUsed)
		{	
			CString strValue;
			xmlHandler.GetXmlAttribute(elmDiagram, "Value", strValue);
			
			vector<CString> vSplittedString;
			vSplittedString = SplitStringByChar(strValue, '*');
			nDiagramXLen = atoi(vSplittedString[0]);
			nDiagramYLen = atoi(vSplittedString[1]);
			nDiagramRotateAngle = atoi(vSplittedString[2]);
	
			strDiagramTmpDir = file_path2 + _T("\\Tmp\\");
			if(access(strDiagramTmpDir, 0) != 0)
				CreateMultipleDirectory(strDiagramTmpDir);
		}

		//���ɴ���ǩͼ
		{
			auto filemap = OnOutputgcodeFileName(/*m_pkcoor*/vAllPanel, m_PanelCodeEnable);//��������

			int indexOfAllLabel = 0;
			int nPanelLabelCount = 0;
			int nPanelSerial = 0;
			//for (int i = 0;i < check_num.size();i++)
			for(int i = 0; i < m_panelList.GetItemCount(); i++)
			{
				Panel* pThePanel = (Panel*)m_panelList.GetItemData(/*check_num.at(i)*/i);

				if(m_panelList.GetCheck(i) == true)
				{
					//���ɴ����ͼ
					{
						CString src_file_name,file_name;
						src_file_name = "design.bmp";
						file_name.Format("design-%d.bmp",indexOfAllLabel);

						CImage image;
						image.Load(file_path + "\\" + src_file_name);

						CFont font;  
						font.CreatePointFont(200, _T("Arial"));

						//DrawPanelInfoToDC(image.GetDC(), pThePanel, 2.2, 10, 10, /*check_num.at(i)+1*/i+1, /*&font200*/PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);
						CRect rcDrawingArea;
						rcDrawingArea.SetRect(0, 0, A4_TEMPLATE_X, A4_TEMPLATE_Y);
						DrawPanelDiagramToA4(image.GetDC(), pThePanel, rcDrawingArea, i+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);

						image.Save(file_path + "\\" + file_name);
						dialog.m_panelBigPicPathList.push_back(file_path2 + "\\" + file_name);
						image.ReleaseDC();


					}

					//����С��
					{
						CString src_file_name,file_name,file_code;
						src_file_name.Format("%s.bmp",s_f.c_str());
						file_name.Format("%s-%d.bmp",s_f.c_str(),indexOfAllLabel);
						file_code.Format("code.bmp");

						CImage image;
						image.Load(file_path2 + "\\" + src_file_name);

						CFont font;  
						font.CreatePointFont(90, _T("Arial"));

						float image_width = image.GetWidth();
						float image_height = image.GetHeight();
						float panel_width = /*m_pkcoor[check_num.at(i)]._panel->_width*/pThePanel->m_RealLength;
						float panel_height = /*m_pkcoor[check_num.at(i)]._panel->_height*/pThePanel->m_RealWidth;
						float scale = HGTools::getMinScale(image_width,image_height,panel_width,panel_height);
						scale = 1.0/scale + 2.3;

						//���ɴ����Ϣ
						//DrawPanelInfoToDC(image.GetDC(), pThePanel, scale, 0, 0, /*check_num.at(i)+1*/i+1, /*&font200*/PanelDiagramPrintingStyle::PRINT_STYLE_LABEL);
						CRect rcDrawingArea;
						rcDrawingArea.SetRect(0, 0, image_width, image_height);
						DrawPanelDiagramToA4(image.GetDC(), pThePanel, rcDrawingArea, i+1, PanelDiagramPrintingStyle::PRINT_STYLE_LABEL);

						//����ͼƬ
						image.Save(file_path2 + "\\" + file_name);
						image.ReleaseDC();
						//���������ã���壬��Ӧ��С�壬��һ���壬��Ӧ��С�壩
						dialog.m_panelNumList.push_back(/*check_num.at(i)+1*/i+1);
						dialog.m_panelSmallPicPathList.push_back(file_path2 + "\\" + file_name);
						indexOfAllLabel++;
						nPanelLabelCount++;
					}



					//�Ա�ǩ��С���ɴ������
					//�Ƿ���Ҫ�����������
					if (m_PanelCodeEnable)
					{
						CString src_file_name,file_name,file_code;
						src_file_name.Format("%s.bmp",s_f.c_str());
						file_name.Format("%s-%d.bmp",s_f.c_str(),indexOfAllLabel);
						file_code.Format("code.bmp");

						CImage image;
						image.Load(file_path2 + "\\" + src_file_name);

						CFont font;  
						font.CreatePointFont(90, _T("Arial"));


						float scale = HGTools::getMinScale(image.GetWidth(), image.GetHeight(), pThePanel->m_RealLength, pThePanel->m_RealWidth);
						scale = 1.0/scale + 2.3;

						//���ɴ����Ϣ
						/////////////m_pkcoor[check_num.at(i)]._kc->printdrawKnifeSmall(image.GetDC(),m_pkcoor[check_num.at(i)]._panel,scale,0,0,check_num.at(i)+1,&font);

						float code_size = 1.5;	//1.5 �²�ֵ��ͬ8*4��ʱ����
						int codeSize = code_size * 40;
						int code_x = 4;
						int code_y = 4;
						CDC* pDC=CDC::FromHandle(image.GetDC());
						pDC->SelectObject(&font);
						pDC->SetTextColor(RGB(0,0,0)); 


						//��ʾ�����Ϣ
						code_x = 4;
						code_y = code_y;
						CString panelString;
						//Panel_Knife &k = m_pkcoor[check_num.at(i)];
						CString strThicknessAndMeterial;
						strThicknessAndMeterial.Format(_T("%s-%s"), GetFloatString(pThePanel->m_Thickness, 1), pThePanel->m_Material);
						panelString.Format(_T("������%d����С����%d����%s��"), /*i+1*/nPanelSerial+1, check_num.size(), strThicknessAndMeterial);
						pDC->TextOut(code_x,code_y,panelString);

						//��ʾ��ά���Ӧ������
						code_x = 4;
						code_y = code_y + pDC->GetTextExtent(panelString).cy + 5;
						CString ncFileName = filemap[/*m_pkcoor[check_num.at(i)]._panel*/pThePanel]["nc"]; 
						pDC->TextOut(code_x,code_y,ncFileName);

						//��ά������
						code_x = 4;
						code_y = code_y + pDC->GetTextExtent(ncFileName).cy + 2;
						CreateQCodeInImage(image, ncFileName, code_x, code_y , code_size );

						//����ͼƬ
						image.Save(file_path2 + "\\" + file_name);
						image.ReleaseDC();
						//���������ã���壬��Ӧ��С�壬��һ���壬��Ӧ��С�壩
						dialog.m_panelNumList.push_back(/*check_num.at(i)+1*/i+1);
						dialog.m_panelSmallPicPathList.push_back(file_path2 + "\\" + file_name);
						dialog.m_panelNCCode = true;
						indexOfAllLabel++;
						nPanelLabelCount++;
					}

					nPanelSerial++;
				}
					

				CString strPanelIDToCmp;
				strPanelIDToCmp.Format(_T("%d"), i + 1);
				for(int j = 0; j < m_smallpanelList.GetItemCount(); j++)
				{
					if (m_smallpanelList.GetCheck(j) == true)
					{
						CString strPanelIDOfComponentToCmp;
						strPanelIDOfComponentToCmp = m_smallpanelList.GetItemText(j, 1);
						if(strPanelIDToCmp.CompareNoCase(strPanelIDOfComponentToCmp) == 0)
						{
							if(bDiagramUsed)
							{
								CImage image;
								image.Create(nDiagramXLen, nDiagramYLen, 32, 0);

								int nComponentIDInPanel;
								nComponentIDInPanel = atoi(m_smallpanelList.GetItemText(j, 2));
								DrawPanelDiagram_ComponentLabel(image.GetDC(), nDiagramXLen, nDiagramYLen, pThePanel, nComponentIDInPanel);

								//����ͼƬ
								CString strDiagram;
								strDiagram.Format(_T("%s%d.bmp"), strDiagramTmpDir, indexOfAllLabel);
								image.Save(strDiagram);
								image.ReleaseDC();
								image.Destroy();
							}

							dialog.m_vComponentLableID.push_back(indexOfAllLabel);
							indexOfAllLabel++;
						}
					}
				}
			}

			dialog.m_panelPictureNumCount = nPanelLabelCount;
		}
	}

	std::vector<print_info> vAllPrintInfo;
	GetAllPrintInfoOfComponent(vAllPrintInfo);
	int size2 = vAllPrintInfo.size();
	dialog.m_info.clear();
	for (int i = 0;i < size2;i++)
	{
		if(m_smallpanelList.GetCheck(i) == true)
		{
			//��ӡС����Ϣ
			dialog.m_info.push_back(vAllPrintInfo.at(i));
		}
	}

	if (dialog.m_info.size() > 0)
	{
		dialog.DoModal();
	}
	else
	{
		MessageBox("��ѡ���ӡ����","��ʾ",MB_OK);
	}                                

}

void CSelectPrintDlg::InitPanelList()
{
	vector<Panel*> vAllPanel;
	FindAllPanelInSingleton(vAllPanel);
	for(int i = 0; i < vAllPanel.size(); i++)
	{
		Panel* pThePanel = vAllPanel[i];
		CString strPanelID;
		CString strComponentCountInThePanel;
		strPanelID.Format(_T("%d"), i+1);
		strComponentCountInThePanel.Format(_T("%d"), GetComponentCountInPanel(*((Component*)pThePanel)));

		int nItem = m_panelList.InsertItem(i,"");
		m_panelList.SetItemText(nItem,1,strPanelID);
		m_panelList.SetItemText(nItem,2,strComponentCountInThePanel);
		m_panelList.SetItemData(nItem, (DWORD_PTR)pThePanel);
	}
}

void CSelectPrintDlg::InitComponentList()
{
	vector<Panel*> vAllPanel;
	FindAllPanelInSingleton(vAllPanel);
	int nCurTail = 0;
	for(int i = 0; i < vAllPanel.size(); i++)
	{
		Panel* pThePanel = vAllPanel[i];
		CString strPanelID;
		strPanelID.Format(_T("%d"), i+1);
		vector<Component*> vAllComponent;
		FindAllComponentInPanel(*((Component*)pThePanel), vAllComponent);

		std::sort(vAllComponent.begin(), vAllComponent.end(), SortComponentByIDInPanel);

		for (int j = 0; j < vAllComponent.size(); j++)
		{
			Component& theComponent = *(vAllComponent[j]);
			int nItem = m_smallpanelList.InsertItem(nCurTail, "");
			m_smallpanelList.SetItemText(nItem,1,strPanelID);
			m_smallpanelList.SetItemText(nItem,2, GetFloatString(theComponent.m_NumberInPanel, 0));
			m_smallpanelList.SetItemText(nItem,3, theComponent.m_strCustomerInfo);
			m_smallpanelList.SetItemText(nItem,4, theComponent.m_BarCode);
			m_smallpanelList.SetItemText(nItem,5, GetFloatString(theComponent.m_RealLength, 0));
			m_smallpanelList.SetItemText(nItem,6, GetFloatString(theComponent.m_RealWidth, 0));
			m_smallpanelList.SetItemText(nItem,7, GetFloatString(theComponent.m_Thickness, 0));
			m_smallpanelList.SetItemText(nItem,8, theComponent.m_strComponentName);
			m_smallpanelList.SetItemText(nItem,9, theComponent.m_Material);
			m_smallpanelList.SetItemText(nItem,10, theComponent.m_strCabinetID);
			CString strTmp;
			strTmp.Format(_T("%s %s %s %s"), GetFloatString(theComponent.m_afBanding[0], 1), GetFloatString(theComponent.m_afBanding[1], 1), \
				GetFloatString(theComponent.m_afBanding[2], 1), GetFloatString(theComponent.m_afBanding[3], 1));
			m_smallpanelList.SetItemText(nItem,11, strTmp);
			m_smallpanelList.SetItemText(nItem,12, GetFloatString(theComponent.m_fProductLength, 0));
			m_smallpanelList.SetItemText(nItem,13, GetFloatString(theComponent.m_fProductWidth, 0));
			if(theComponent.m_nOtherShapeID == 0)
				strTmp = _T("");
			else
				strTmp.Format(_T("��%d"), theComponent.m_nOtherShapeID);
			m_smallpanelList.SetItemText(nItem,14, strTmp);
			m_smallpanelList.SetItemText(nItem,15, theComponent.m_strSlottingFlag);
			m_smallpanelList.SetItemText(nItem,16, theComponent.m_strReminder);
			m_smallpanelList.SetItemText(nItem,17, theComponent.m_strDrilling);
			m_smallpanelList.SetItemText(nItem,18, theComponent.m_strJoinedStore);
			m_smallpanelList.SetItemText(nItem,19, theComponent.m_strCabinetName);
			m_smallpanelList.SetItemText(nItem,20, theComponent.m_strOrderType);
			m_smallpanelList.SetItemText(nItem,21, theComponent.m_strReverseSideBarcode);
			m_smallpanelList.SetItemText(nItem,22, theComponent.m_strCustomerAddress);
			m_smallpanelList.SetItemText(nItem,23, theComponent.m_strHoleSlotFlag);
			m_smallpanelList.SetItemText(nItem,24, GetTextureString((TextureDirectionType)theComponent.m_Texture));
			if(!theComponent.m_bSlotFlipped && !theComponent.m_bVHoleFlipped)
				strTmp = _T("��");
			else
				strTmp = _T("��");
			m_smallpanelList.SetItemText(nItem,25, strTmp);
			m_smallpanelList.SetItemData(nItem, (DWORD_PTR)(vAllComponent[j]));

			nCurTail++;
		}
	}


}

void CSelectPrintDlg::GetAllPanelInPanelList(vector<Panel*>& vAllPanel)
{
	vAllPanel.clear();
	for(int i = 0; i < m_panelList.GetItemCount(); i++)
	{
		vAllPanel.push_back((Panel*)m_panelList.GetItemData(i));
	}
}

void CSelectPrintDlg::GetAllPrintInfoOfComponent(vector<print_info>& vAllPrintInfo)
{
	for(int i = 0; i < m_smallpanelList.GetItemCount(); i++)
	{
		Component* pComponent = (Component*)(m_smallpanelList.GetItemData(i));
		print_info printInfo;
		printInfo.bar_code = pComponent->m_BarCode;
		printInfo.com_depth = IsFloatEqual(pComponent->m_Thickness, floorf(pComponent->m_Thickness)) ? GetFloatString(pComponent->m_Thickness, 0) : GetFloatString(pComponent->m_Thickness, 1);
		printInfo.com_height = IsFloatEqual(pComponent->m_RealWidth, floorf(pComponent->m_RealWidth)) ? GetFloatString(pComponent->m_RealWidth, 0) : GetFloatString(pComponent->m_RealWidth, 1);
		printInfo.com_width = IsFloatEqual(pComponent->m_RealLength, floorf(pComponent->m_RealLength)) ? GetFloatString(pComponent->m_RealLength, 0) : GetFloatString(pComponent->m_RealLength, 1);
		printInfo.com_matrial = pComponent->m_Material;
		printInfo.com_name = pComponent->m_strComponentName;
		printInfo.com_no = GetFloatString(pComponent->m_NumberInPanel, 0);
		printInfo.cst_no = pComponent->m_strCabinetID;
		CString strTmp;
		strTmp.Format(_T("%s %s %s %s"), GetFloatString(pComponent->m_afBanding[0], 1), GetFloatString(pComponent->m_afBanding[1], 1), \
			GetFloatString(pComponent->m_afBanding[2], 1), GetFloatString(pComponent->m_afBanding[3], 1));
		printInfo.edge_info = strTmp;
		printInfo.panel_no =  m_smallpanelList.GetItemText(i, 1);
		float fProductLen, fProductWidth;
		//fProductLen = pComponent->m_RealLength + pComponent->m_afBanding[BANDING_ID_RIGHT] + pComponent->m_afBanding[BANDING_ID_LEFT];
		//fProductWidth = pComponent->m_RealWidth + pComponent->m_afBanding[BANDING_ID_FRONT] + pComponent->m_afBanding[BANDING_ID_BACK];
		//���������Ԥϳ�ߴ磬���ϳߴ�ӷ��������ڳ��ͳߴ磬����ֱ���ó��ͳߴ��ֶ�
		fProductLen = pComponent->m_fProductLength;
		fProductWidth = pComponent->m_fProductWidth;
		printInfo.product_width = GetFloatStringTrimming(fProductLen, 1);
		printInfo.product_height = GetFloatStringTrimming(fProductWidth, 1);
		if(pComponent->m_nOtherShapeID == 0)
			printInfo.other_com = _T("");
		else
			printInfo.other_com.Format(_T("��%d"), pComponent->m_nOtherShapeID);
		printInfo.cst_name = pComponent->m_strCabinetName;
		printInfo.customer_info = pComponent->m_strCustomerInfo;
		printInfo.franchisee = pComponent->m_strJoinedStore;
		printInfo.slot_com = pComponent->m_strSlottingFlag;
		printInfo.punching_com = pComponent->m_strReminder;
		printInfo.double_com = pComponent->m_strDrilling;
		printInfo.ordertype = pComponent->m_strOrderType;
		printInfo.rebar_code = pComponent->m_strReverseSideBarcode;
		printInfo.customer_address = pComponent->m_strCustomerAddress;
		printInfo.zc_identify = pComponent->m_strHoleSlotFlag;
		printInfo.panel_dir = GetComponentRotetedTextureDirType((TextureDirectionType)(pComponent->m_Texture), pComponent->m_nRotatedAngle) == TEXTURE_V_DIR ? _T("0") : _T("1");
		if(!pComponent->m_bSlotFlipped && !pComponent->m_bVHoleFlipped)
			strTmp = _T("��");
		else
			strTmp = _T("��");
		printInfo.panel_reversal = strTmp;
		printInfo.nRotatedAngle = pComponent->m_nRotatedAngle;
		printInfo.bIsDoorPanel = (pComponent->m_strPanelClass.CompareNoCase(_T("Door")) == 0) ? true : false;
		printInfo.m_strSendingDate = pComponent->m_strSendingDate;

		vAllPrintInfo.push_back(printInfo);
	}
}

//bool SortComponentByIDInPanel(const PCOMPONENT& first, const PCOMPONENT& second)
//{
//	return first->m_NumberInPanel < second->m_NumberInPanel;
//}

int CSelectPrintDlg::GetIndicatePanelCheckedComponentCount(int nPanelID)
{
	int nRet = 0;
	CString strPanelIDToCmp;
	strPanelIDToCmp.Format(_T("%d"), nPanelID + 1);
	for(int i = 0; i < m_smallpanelList.GetItemCount(); i++)
	{
		if (m_smallpanelList.GetCheck(i) == true)
		{
			CString strPanelIDOfComponentToCmp;
			strPanelIDOfComponentToCmp = m_smallpanelList.GetItemText(i, 1);
			if(strPanelIDToCmp.CompareNoCase(strPanelIDOfComponentToCmp) == 0)
				nRet++;
		}
	}
	return nRet;
}