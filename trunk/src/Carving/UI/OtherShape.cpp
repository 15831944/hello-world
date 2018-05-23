// OtherShape.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "OtherShape.h"
#include "afxdialogex.h"
//#include "hg3d/XmlFile.h"
//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include "hg3d/XmlNode.h"
#include <vector>
//#include "hgCst/Construction.h"
//#include "KnifeClass.h"
#include "../Misc/UTF8ToMBCS.h"
//#include "XmlHandlePlus.h"
//#include "YHInfoTools.h"
#include "../Misc/HGTools.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/Misc.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Drawing/Drawing.h"


bool COtherShape::isInitPanelOutlineList = false;
vector<Shape_NamePath> COtherShape::m_PanelOutlineList;
map<int,vector<param_list>> COtherShape::m_otherShapeParamList;
std::map<int,std::vector<std::map<std::string,std::string>>> COtherShape::m_outlineParam;

// COtherShape dialog

IMPLEMENT_DYNAMIC(COtherShape, CDialogEx)

COtherShape::COtherShape(CWnd* pParent /*=NULL*/)
	: CDialogEx(COtherShape::IDD, pParent)
{
	m_changetype = 0;
}

COtherShape::~COtherShape()
{
}

void COtherShape::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_PictureList);
	DDX_Control(pDX, IDC_LIST4, m_ParamList);
	DDX_Control(pDX, IDC_LISTEDIT, m_ParamListEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDC_BUTTON5, m_button5);
	DDX_Control(pDX, IDC_BUTTON6, m_button6);
	DDX_Control(pDX, IDC_BUTTON7, m_button7);
	DDX_Control(pDX, IDC_BUTTON8, m_button8);
	DDX_Control(pDX, IDC_BUTTON9, m_button9);
	DDX_Control(pDX, IDC_BUTTON10, m_button10);
	DDX_Control(pDX, IDC_BUTTON11, m_button11);
	DDX_Control(pDX, IDC_BUTTON12, m_button12);
	DDX_Control(pDX, IDC_BUTTON13, m_button13);
	DDX_Control(pDX, IDC_BUTTON14, m_button14);
	DDX_Control(pDX, IDC_BUTTON16, m_button15);
	DDX_Control(pDX, IDC_BUTTON17, m_button16);
	DDX_Control(pDX, IDC_TREE1, m_pictreelist);
	DDX_Control(pDX, IDC_LIST1, m_ClassifyList);
	DDX_Control(pDX, IDC_COMBO3, m_comchange);
}



BEGIN_MESSAGE_MAP(COtherShape, CDialogEx)
	ON_BN_CLICKED(IDOK, &COtherShape::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, &COtherShape::OnNMClickList4)
	ON_EN_KILLFOCUS(IDC_LISTEDIT, &COtherShape::OnEnKillfocusListedit)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &COtherShape::OnPictureListSelectChange)
	ON_EN_ERRSPACE(IDC_LISTEDIT, &COtherShape::OnEnErrspaceListedit)
	ON_EN_CHANGE(IDC_LISTEDIT, &COtherShape::OnEnChangeListedit)
	ON_BN_CLICKED(IDCANCEL, &COtherShape::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &COtherShape::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COtherShape::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &COtherShape::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &COtherShape::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &COtherShape::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &COtherShape::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &COtherShape::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &COtherShape::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &COtherShape::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &COtherShape::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &COtherShape::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &COtherShape::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &COtherShape::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &COtherShape::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON16, &COtherShape::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &COtherShape::OnBnClickedButton17)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &COtherShape::OnLvnItemchangedList4)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &COtherShape::OnNMClickTree1)
	ON_LBN_SELCHANGE(IDC_LIST1, &COtherShape::OnChangeSelectClassifyList)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO3, &COtherShape::OnCbnSelchangeCombo3)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &COtherShape::OnLvnItemchangedList4)
ON_BN_CLICKED(IDC_BTN_REFRESH, &COtherShape::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// �¼�

BOOL COtherShape::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	//���ý���Ԫ��


	//�����б�
	LONG lStyle;
	lStyle = GetWindowLong(m_ParamList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ParamList.m_hWnd, GWL_STYLE, lStyle);
	DWORD dwStyle = m_ParamList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_ParamList.SetExtendedStyle(dwStyle); 

	//�����б�����
	m_ParamList.InsertColumn(0,_T("������"),0,120);
	m_ParamList.InsertColumn(1,_T("Ӣ����"),0,120);
	m_ParamList.InsertColumn(2,_T("����ֵ"),0,100);


	//��ת�����б�
	m_comchange.InsertString(0,"����ת");
	m_comchange.InsertString(1,"˳ʱ����ת90��");
	m_comchange.InsertString(2,"��ʱ����ת90��");
	m_comchange.InsertString(3,"��ת180��");
	m_comchange.SetCurSel(0);


	m_ParamListEdit.ShowWindow(SW_HIDE);



	//////////////////////////////////////////////////////////////////////////



	m_panelID = HGTools::Convert2CString(m_item["info"]);
	m_othershapeString = HGTools::Convert2CString(m_item["pointInfo"]);
	m_changetype = atoi(m_item["rotate"].c_str());
	m_panelwidth = HGTools::Convert2CString(m_item["width"]);
	m_panelheight = HGTools::Convert2CString(m_item["height"]);

	m_checkDrawLegal = true;
	m_checkParamPointLegal = true;

	m_comchange.SetCurSel(m_changetype);

	//////////////////////////////////////////////////////////////////////////
	// ׼����������

	//�������outline����
	m_outlineParam = OtherShapeHelper::GetOtherShapePanelOutLineList();

	//���ط����ļ�
	load_OutlineList_ParamList();

	//����ˢ�µ������б���
	write_UIClassifyList_ByOutlineList();

	//�����ַ�����Ϊ��
	if (m_item.find("pointInfo") != m_item.end() && m_item["pointInfo"] != "" )
	{
		std::vector<param_point> pa_p = OtherShapeHelper::GetParamPointsFromOtherShapeString(m_othershapeString.GetBuffer(), m_panelwidth.GetBuffer(), m_panelheight.GetBuffer());
		write_UIPictureList_ByOtherShapeID(OtherShapeHelper::GetOtherShapeId(pa_p));
		write_UIParamList_ByOtherShapeID(OtherShapeHelper::GetOtherShapeId(pa_p),pa_p);
		m_pointInfos = OtherShapeHelper::GetPointInfoFromOtherShapeString(m_othershapeString.GetBuffer(),m_panelwidth.GetBuffer(), m_panelheight.GetBuffer());
		///////////////////////////////////////////////////////////////////////////////////////////////////
		OtherShapeHelper::GetPointsToYPlus(m_pointInfos, m_item["rotate"], m_item["width"], m_item["height"]);
		m_pointInfos = OtherShapeHelper::RotateOtherShapePointInfos(m_pointInfos,m_item["rotate"], m_item["width"], m_item["height"]);
		m_outline = OtherShapeHelper::GetOtherShapePanelOutLineListFromOtherShapeID(OtherShapeHelper::GetOtherShapeId(m_othershapeString.GetBuffer()));

		//OnEnChangeListedit();
		paint_UIPicture_ByOutline();
	}
	//���ε���Ϊ��
	else if (m_item.find("OtherPoints") != m_item.end() && m_item["OtherPoints"] != "" )
	{
		write_UIPictureList_ByOtherShapeID("");
		write_UIParamList_ByOtherShapeID("");
		m_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(m_item["OtherPoints"]);
		paint_UIPicture_ByOutline();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COtherShape::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Do not call CDialogEx::OnPaint() for painting messages
	if(isPaintable())
	{
		paint_UIPicture_ByOutline();
	}
}

//��⵽���ܳ�����Ӧ����ʾ�û��Ƿ��ٴθ���Ȼ�󱣴�
void COtherShape::OnBnClickedOk()
{
	std::vector<param_point> pa_p = convert_UIParamList_FromUI(read_UIParamList_FromUI());

	//��ͨ���ΰ�
	if (OtherShapeHelper::GetOtherShapeId(pa_p).compare("0") == 0)	
	{
		m_othershapeString = "";
		CDialogEx::OnOK();
	}
	//�������ΰ�
	else
	{
		OtherShapeHelper::removeParamPoint_PanelWidthPanelHeight(pa_p);
		checkParamPointLegal(pa_p);
		if (!m_checkParamPointLegal)
		{
			MessageBox("��������ݲ��Ϸ������飡");
			return;
		}

		checkDrawLegal(m_pointInfos);
		if (!m_checkDrawLegal)
		{
			MessageBox("�������������Χ�����飡");
			return;
		}

		std::string str_pointInfo = OtherShapeHelper::GetOtherShapeStringFromParamPoints(pa_p);
		m_othershapeString = HGTools::Convert2CString(str_pointInfo);

		CDialogEx::OnOK();
	}
}

//����ͼƬѡ��
void COtherShape::OnPictureListSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nIndex = m_PictureList.GetNextItem(-1,LVIS_SELECTED);
	CString imageindex;
	m_PictureList.GetImageFileName(nIndex,imageindex);
	imageindex = imageindex.Left(imageindex.Find('.'));
	if (nIndex < 0)
		return;
	else
	{
		int id_paramlist = atoi(imageindex.GetBuffer());
		write_UIParamList_ByOtherShapeID(imageindex.GetBuffer());	
		m_outline = m_outlineParam[atoi(imageindex)];				//�л��������β���
		
		std::vector<param_point> pa_p = read_UIParamList_FromUI();
		if (pa_p.size() < 5)
		{
			// 2017-4-21 yuanzb ����û�ж�y������д���

			std::vector<param_point> pa_p = read_UIParamList_FromUI();
			pa_p = convert_UIParamList_FromUI(pa_p);
			m_pointInfos = OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint(m_outline,pa_p);


			// 2017-4-21 yuanzb ���Ӷ�y��Ĵ��� ���ݽ���ѡ�����ת��ͼ�ν�����ת

			CString changeType;
			changeType.Format("%d",m_comchange.GetCurSel());
			m_changetype = m_comchange.GetCurSel();

			OtherShapeHelper::GetPointsToYPlus(m_pointInfos, changeType.GetBuffer(),m_panelwidth.GetBuffer(),m_panelheight.GetBuffer());


			paint_UIPicture_ByOutline();
		}
		else
		{
			// �����ж�y������д���
			OnEnChangeListedit();
		}

	}
	*pResult = 0;
}


//�޸����ΰ����ݲ���
void COtherShape::OnEnChangeListedit()
{
	//����û���ǰ����
	CString str_get;
	m_ParamListEdit.GetWindowText(str_get);
	m_ParamList.SetItemText(m_row,m_column,str_get);

	//�û���ת
	CString changeType;
	changeType.Format("%d",m_comchange.GetCurSel());
	m_changetype = m_comchange.GetCurSel();


	//��ý�������в�������У��
	std::vector<param_point> pa_p = read_UIParamList_FromUI();
	pa_p = convert_UIParamList_FromUI(pa_p);
	OtherShapeHelper::RotateOtherShapeParamPoint(pa_p,changeType.GetBuffer());

	if (!checkParamPointLegal(pa_p)) return;
	
	//���ƽ�����������ɵ�ͼ��

	for(int i = 0; i < m_outline.size(); i++)
	{
		map<std::string,std::string>& theMap = m_outline[i];
		for(map<string, string>::iterator itr = theMap.begin(); itr != theMap.end(); itr++)
		{
			CString strTmp;
			strTmp.Format(_T("%s  %s\n"), itr->first.c_str(), itr->second.c_str());
			OutputDebugString(strTmp);
		}
	}
	OutputDebugString(_T("\n\n\n"));
	m_pointInfos = OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint(m_outline,pa_p);
	if (!checkDrawLegal(m_pointInfos)) return;

	//////////////////////////////////////////////////////////////////////////////////////////
	OtherShapeHelper::GetPointsToYPlus(m_pointInfos, changeType.GetBuffer(),m_panelwidth.GetBuffer(),m_panelheight.GetBuffer());

	//���ݽ���ѡ�����ת��ͼ�ν�����ת
	m_pointInfos = OtherShapeHelper::RotateOtherShapePointInfos(m_pointInfos,changeType.GetBuffer(),m_panelwidth.GetBuffer(),m_panelheight.GetBuffer());

	//�������ĵ�
	//m_pointInfos = OtherShapeHelper::ClearUnnessaryPointInfo(m_pointInfos);
	
	//����������
	paint_UIPicture_ByOutline();
}

//�������η���ѡ��
void COtherShape::OnChangeSelectClassifyList()
{
	m_PictureList.DeleteAllItems();
	int nItem = m_ClassifyList.GetCurSel();
	if (nItem < 0)
	{
		return;
	}
	else
	{
		std::string str_picture = m_PanelOutlineList.at(nItem)._path;
		//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
		std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
		CString file_name,_picture;
		file_name.Format("%s",sfile_name.c_str());
		_picture.Format("%s",str_picture.c_str());

		file_name = HGTools::getRealPathFromReleativePath(file_name);
		m_PictureList.SetFolder(128,128,file_name,_picture);
	}
}


//�������η���ѡ��
void COtherShape::OnCbnSelchangeCombo3()
{
	OnEnChangeListedit();
}

//ˢ����������
void COtherShape::OnBnClickedBtnRefresh()
{
	Invalidate();
}


//////////////////////////////////////////////////////////////////////////
// ��д����

//���ļ��м�������������������
void COtherShape::load_OutlineList_ParamList()
{
	if (!isInitPanelOutlineList)
	{
		isInitPanelOutlineList = true;
		//�����ļ�·��
		CString strTmp;
		strTmp.Format(_T("kdata\\Param%s"), g_szEncyptSubfix);
		CString hgePath = HGTools::getRelativePath(strTmp);
		CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
		//����
		//HGTools::decryptFile(hgmPath,xmlPath);
		decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
		//�����ļ�
		TinyXml::TiXmlDocument doc(xmlPath);
		doc.LoadFile();
		//TinyXml::XmlHandlePlus docHandler(&doc);
		XmlHandler xmlHandler;

		//�����ظ���ӷ���
		std::map<string,int> repeatControl;

		//��ȡ�������ΰ�
		//std::vector<TinyXml::TiXmlNode*> PanelOutlineArr = docHandler.findAll("/PanelOutlineList/PanelOutline");
		vector<TiXmlElement*> vPanelOutline = xmlHandler.GetChildElms(doc.RootElement(), "PanelOutline");
		//for (int i = 0; i<PanelOutlineArr.size() ; i++)
		for (int i = 0; i<vPanelOutline.size() ; i++)
		{
			//TinyXml::XmlHandlePlus PanelOutlineHandler(PanelOutlineArr[i]);
			TiXmlElement* elmPanelOutline = vPanelOutline[i];
			//std::string p_name = PanelOutlineHandler.getAttr("Category");		//����
			//std::string p_index = PanelOutlineHandler.getAttr("StartSerial");	//��ʼ���
			//std::string p_nameword = PanelOutlineHandler.getAttr("Name");		//����
			//std::string p_id = PanelOutlineHandler.getAttr("StyleId");			//����ID
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmPanelOutline, "Category", strTmp);
			std::string p_name = strTmp;
			xmlHandler.GetXmlAttribute(elmPanelOutline, "StartSerial", strTmp);
			std::string p_index = strTmp;
			xmlHandler.GetXmlAttribute(elmPanelOutline, "Name", strTmp);
			std::string p_nameword = strTmp;
			xmlHandler.GetXmlAttribute(elmPanelOutline, "StyleId", strTmp);
			std::string p_id = strTmp;


			//������ӣ��״γ��ֵķ�����ӵ������У�
			if (repeatControl.find(p_index) == repeatControl.end())
			{
				Shape_NamePath one_namepath;		// ���η���
				one_namepath._name = p_name;
				one_namepath._index = p_index;
				one_namepath._path = p_id + ";";
				m_PanelOutlineList.push_back(one_namepath);
				repeatControl[p_index] = m_PanelOutlineList.size()-1;
			}
			//���γ��ֵķ���׷�����ĸ���Ԫ��
			else
			{
				m_PanelOutlineList[repeatControl[p_index]]._path = m_PanelOutlineList[repeatControl[p_index]]._path + p_id + ";";
			}


			//���ò���
			vector<param_list> oneShapeParamList;
			//std::vector<TinyXml::TiXmlNode*> ParameterArr = PanelOutlineHandler.findAll("Parameter");
			vector<TiXmlElement*> vParameter = xmlHandler.GetChildElms(elmPanelOutline, "Parameter");
			//for (int j = 0;j < ParameterArr.size();j++)
			for (int j = 0;j < vParameter.size();j++)
			{
				//TinyXml::XmlHandlePlus ParameterHandler(ParameterArr[j]);
				TiXmlElement* elmParameter = vParameter[j];
				param_list paramlistItem;
				//paramlistItem._name = ParameterHandler.getAttr("Name","---");
				//paramlistItem._remark = ParameterHandler.getAttr("Remark","---");
				//paramlistItem._abbreviation = ParameterHandler.getAttr("Abbreviation","---");
				CString strTmp = "---";
				xmlHandler.GetXmlAttribute(elmParameter, "Name", strTmp);
				paramlistItem._name = strTmp;
				strTmp = "---";
				xmlHandler.GetXmlAttribute(elmParameter, "Remark", strTmp);
				paramlistItem._remark = strTmp;
				strTmp = "---";
				xmlHandler.GetXmlAttribute(elmParameter, "Abbreviation", strTmp);
				paramlistItem._abbreviation = strTmp;

				oneShapeParamList.push_back(paramlistItem);
			}
			m_otherShapeParamList[atoi(p_id.c_str())] = oneShapeParamList;
		}

		//�ر��ļ�
		HGTools::deleteFile(xmlPath);

	}
}

//�������������ݸ��µ����ݷ�����
void COtherShape::write_UIClassifyList_ByOutlineList()
{
	for (int i = 0;i < m_PanelOutlineList.size();i++)	//���Ҳ�ѡ���б��У�����ID�ַ���ռλ
	{
		CString str_name;
		std::string s_name = m_PanelOutlineList.at(i)._name;
		str_name.Format("%s",s_name.c_str());
		m_ClassifyList.InsertString(i,str_name);
	}
}

//������ID�ַ�����ӳ������ͼƬѡ���Լ����ݷ���ѡ��
void COtherShape::write_UIPictureList_ByOtherShapeID(std::string otherShapeId)
{

	if (otherShapeId == "")
	{
		otherShapeId = "0";
	}

	//�������ѡ���ͼƬ
	m_PictureList.DeleteAllItems();
	
	//ѡ��ͼƬ
	//hg3d::TString OtherPicture = _T("kdata");
	//OtherPicture = hg3d::getFullFileName(OtherPicture);
	//OtherPicture += _T("\\Param.xml");

	//hg3d::CXmlFile * OtherShape_doc = new hg3d::CXmlFile;
	//OtherShape_doc->Load(OtherPicture.c_str());
	CString strTmp;
	strTmp.Format(_T("kdata\\Param%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;
	CString strXML = HGTools::getXmlPathFromHgxPath(strHGE);
	decrypt_base64(strHGE.GetBuffer(), strXML.GetBuffer());
	TinyXml::TiXmlDocument doc(strXML);
	doc.LoadFile();
	HGTools::deleteFile(strXML);
	XmlHandler xmlHandler;

	//�ҵ��������ID���ڵ��飬ȫ�����ڵ��Ի����Ҳ�ͼ���У�������ʾͼ����
	//if (OtherShape_doc->GetChildByAttribute("StyleId",otherShapeId.c_str()) != NULL)
	TiXmlElement* elmThePanelOutline = xmlHandler.GetIndicatedElement(doc.RootElement(), "PanelOutline", "StyleId", otherShapeId.c_str());
	if (elmThePanelOutline != NULL)
	{
		//std::string p_StartSerial = OtherShape_doc->GetChildByAttribute("StyleId",otherShapeId.c_str())->GetAttributeValue("StartSerial");
		CString strTmp;
		xmlHandler.GetXmlAttribute(elmThePanelOutline, "StartSerial", strTmp);
		std::string p_StartSerial = strTmp;
		std::string str_picture;
		for (int _p = 0;_p < m_PanelOutlineList.size();_p++)
		{
			std::string p_dex = m_PanelOutlineList.at(_p)._index;
			if (atoi(p_StartSerial.c_str()) == atoi(p_dex.c_str()))
			{
				str_picture = m_PanelOutlineList.at(_p)._path;
				m_ClassifyList.SetCurSel(_p);								//����ѡ��
				break;
			}
		}
		//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
		std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
		CString file_name,_picture;
		file_name.Format("%s",sfile_name.c_str());
		_picture.Format("%s",str_picture.c_str());

		file_name = HGTools::getRealPathFromReleativePath(file_name);
		m_PictureList.SetFolder(128,128,file_name,_picture);


		//Ѱ��ͼƬ�������
		std::string str_pindex = "";
		int i_pindex = 0;
		for (int k = 0;k < str_picture.size();k++)
		{
			if (str_picture.at(k) == ';')
			{
				if (strcmp(str_pindex.c_str(),otherShapeId.c_str()) == 0)
				{
					break;
				}
				else
				{
					i_pindex++;
					str_pindex = "";
				}
			}
			else
			{
				str_pindex = str_pindex + str_picture.at(k);
			}
		}
		m_PictureList.SetCurselA(i_pindex);									//ͼƬѡ��
	}
}

//��������ID��Ӧ�Ĳ����б�
void COtherShape::write_UIParamList_ByOtherShapeID( std::string otherShapeId, std::vector<param_point> pa_p /*= std::vector<param_point>()*/ )
{
	//����б�
	m_ParamList.DeleteAllItems();

	//�����б�
	int nItem1 = m_ParamList.InsertItem(0,"�������");
	m_ParamList.SetItemText(nItem1,1,"barcode");
	m_ParamList.SetItemText(nItem1,2,m_panelID);

	int nItem0 = m_ParamList.InsertItem(1,"ͼƬ���");
	m_ParamList.SetItemText(nItem0,1,"ID");
	m_ParamList.SetItemText(nItem0,2,otherShapeId.c_str());

	//��������
	if (otherShapeId.compare("") == 0)
	{
		return;
	}
	//����������ˢ��UI��
	vector<param_list> pick_list = m_otherShapeParamList[atoi(otherShapeId.c_str())];
	for (int p_i = 0;p_i < pick_list.size();p_i++)
	{
		std::string list_name = pick_list.at(p_i)._name;
		std::string list_remark = pick_list.at(p_i)._remark;
		std::string list_abbreviation = pick_list.at(p_i)._abbreviation;

		CString str_listname,str_listremark,str_listvalue,str_listabbreviation;
		str_listname.Format("%s",list_name.c_str());
		str_listremark.Format("%s",list_remark.c_str());
		str_listabbreviation.Format("%s",list_abbreviation.c_str());

		//�����ݾ��������
		str_listvalue = "";
		if (pa_p.size()>0)
		{
			std::string temp = OtherShapeHelper::GetParamPoint(pa_p,str_listname.GetBuffer());
			str_listvalue = HGTools::Convert2CString(temp);
		}

		//û�����ݾ����ָ���������
		if (str_listvalue != "")
		{
		}
		else if (str_listname == "PW")
		{
			str_listvalue = m_panelwidth;
		}
		else if (str_listname == "PD")
		{
			str_listvalue = m_panelheight;
		}
		else
		{
			str_listvalue = "";
		}

		int nItem = m_ParamList.InsertItem(p_i+2,str_listremark);
		m_ParamList.SetItemText(nItem,1,str_listabbreviation);
		m_ParamList.SetItemText(nItem,2,str_listvalue);
	}
}

//���ڲ�m_pointInfos����������
void COtherShape::paint_UIPicture_ByOutline()
{
	//CRect imageRect;
	//GetDlgItem(IDC_OTHER_SHAPE_IMG)->GetWindowRect(imageRect);
	//ScreenToClient(imageRect);
	//CPoint offsetPoint = imageRect.TopLeft();
	//offsetPoint.x += 5;
	//offsetPoint.y += 5;

	////�õ����ű�����ȡ��С��������Ǹ�
	//float w_scale = (imageRect.Width() - 10 )/atof(m_panelwidth);
	//float h_scale = (imageRect.Height() - 10)/atof(m_panelheight);
	//float _scale = w_scale < h_scale ? w_scale : h_scale;
	//
	////����߿�
	//HDC hdc = this->GetDC()->GetSafeHdc();
	//CDC* pDC=CDC::FromHandle(hdc);

	//
	//pDC->Rectangle(imageRect);

	//m_paint_UIPicture_ByOutline_ok = true;
	//float maxWidth = imageRect.Width() ;
	//float maxHeight = imageRect.Height() ;
	////�ж��Ƿ�Ϸ�outline����
	//for (int i = 0; i < m_pointInfos.size() ; i++)
	//{
	//	if ( (m_pointInfos[i].x * _scale )< (0 - 10) || maxWidth < ((m_pointInfos[i].x*_scale) +10) )
	//	{
	//		if ( (m_pointInfos[i].y* _scale) < (0 - 10) || maxHeight < ((m_pointInfos[i].y* _scale) + 10) )
	//		{
	//			m_paint_UIPicture_ByOutline_ok = false;
	//			break;
	//		}
	//	}
	//}
	//if (m_paint_UIPicture_ByOutline_ok)
	//{
	//	std::vector<InfoLine> lines = OtherShapeHelper::GetInfoLineFromPointInfo(m_pointInfos);
	//	OtherShapeHelper::DrawLinesInPDC(pDC, lines, offsetPoint, _scale);

	//}

	CRect rcDrawArea;
	GetDlgItem(IDC_OTHER_SHAPE_IMG)->GetWindowRect(rcDrawArea);
	ScreenToClient(rcDrawArea);

	HDC hdc = this->GetDC()->GetSafeHdc();
	CDC* pDC=CDC::FromHandle(hdc);

	pDC->Rectangle(rcDrawArea);

	m_paint_UIPicture_ByOutline_ok = true;

	int nComponentXLen = atoi(m_panelwidth.GetBuffer());
	m_panelwidth.ReleaseBuffer();
	int nComponentYLen = atoi(m_panelheight.GetBuffer());
	m_panelheight.ReleaseBuffer();
	//�ж��Ƿ�Ϸ�outline����
	for (int i = 0; i < m_pointInfos.size() ; i++)
	{
		if ( (m_pointInfos[i].x < -1) || (m_pointInfos[i].y < -1) || (m_pointInfos[i].x > nComponentXLen + 1) || (m_pointInfos[i].y > nComponentYLen + 1))
		{
			m_paint_UIPicture_ByOutline_ok = false;
			break;
		}
	}
	if (m_paint_UIPicture_ByOutline_ok)
	{
		Graphics g(pDC->GetSafeHdc());
		g.SetSmoothingMode(SmoothingModeHighQuality);

		float fScale = GetScale(nComponentXLen, nComponentYLen, rcDrawArea.Width()-10, rcDrawArea.Height()-10);

		CRect rcPanelArea(10/2, 10/2, rcDrawArea.Width()-10/2, rcDrawArea.Height()-10/2);

		CRect rcPanel((rcDrawArea.Width()-nComponentXLen*fScale)/2, \
			(rcDrawArea.Height()-nComponentYLen*fScale)/2, \
			(rcDrawArea.Width()-nComponentXLen*fScale)/2 + nComponentXLen*fScale, \
			(rcDrawArea.Height()-nComponentYLen*fScale)/2 + nComponentYLen*fScale);

		rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

		g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

		vector<PointInfo>& vOutlinePoint = m_pointInfos;
		if(vOutlinePoint.size() > 0)
		{
			for(int j = 1; j < vOutlinePoint.size(); j++)
			{
				PointInfo& vertexStart = vOutlinePoint[j - 1];
				PointInfo& vertexEnd = vOutlinePoint[j];

				DrawOneLine(g, rcPanel, fScale, vertexStart, vertexEnd);
			}
		}
		else
		{
			g.DrawRectangle(&Pen(Color::Black, 1), rcPanel.left, rcPanel.top, rcPanel.Width(), rcPanel.Height());
		}
	}

	this->ReleaseDC(pDC);
}

//�ӽ����ϼ���������
std::vector<param_point> COtherShape::read_UIParamList_FromUI()
{
	std::vector<param_point> result;
	for ( int i = 0 ; i < m_ParamList.GetItemCount() ; i++ )
	{
		CString param_name = m_ParamList.GetItemText(i,1);
		CString param_value = m_ParamList.GetItemText(i,2);

		param_point pa;
		pa._name = param_name.GetBuffer();
		pa._value = param_value.GetBuffer();
		result.push_back(pa);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
// �ж�

bool COtherShape::isPaintable()
{
	return (m_checkDrawLegal && m_checkParamPointLegal);
}

//У���������
bool COtherShape::checkParamPointLegal(const std::vector<param_point>& pa_p)
{
	std::string othershapeID = OtherShapeHelper::GetOtherShapeId(pa_p);
	if (othershapeID.compare("0") == 0 || othershapeID.compare("") == 0)	//����������
	{
		m_checkParamPointLegal = false;
		return false;
	}
	for(int i  = 0; i<pa_p.size() ; i++)			//�����б��ж�
	{
		if (pa_p[i]._value.compare("") == 0)		//����Ϊ��
		{
			m_checkParamPointLegal = false;
			return false;
		}
		if (!(atof(pa_p[i]._value.c_str()) > 0.0 - 0.0001 ))	//���������ַ�Χ 0.0 ��������
		//if (!(atof(pa_p[i]._value.c_str()) - 0.0001 > 0.0 ))	//���������ַ�Χ 0.0 ����������
		{
			m_checkParamPointLegal = false;
			return false;
		}
		if (1)										//�ж�����ı���������
		{
			std::stringstream sstream;
			sstream<<pa_p[i]._value;
			float outputData  = FLT_MIN;
			sstream >> outputData;
			if (outputData == FLT_MIN || !sstream.eof())
			{
				m_checkParamPointLegal = false;
				return false;
			}
		}
	}
	m_checkParamPointLegal = true;
	return true;
}

//У�黭ͼ����
bool COtherShape::checkDrawLegal(const std::vector<PointInfo>& pointInfos)
{
	float panelWidth = _ttof(m_panelwidth);
	float panelHeight = _ttof(m_panelheight);
// 	for (int i = 0; i < pointInfos.size() ; i++)
// 	{
// 		if (!(0.0 <= pointInfos[i].x && pointInfos[i].x <= panelWidth))
// 		{
// 			m_checkDrawLegal = false;
// 			return false;
// 		}
// 		if (!(0.0 <= pointInfos[i].y && pointInfos[i].y <= panelHeight))
// 		{
// 			m_checkDrawLegal = false;
// 			return false;
// 		}
// 	}
	//m_checkDrawLegal = true;
	m_checkDrawLegal = m_paint_UIPicture_ByOutline_ok;
	return true;
}

bool COtherShape::checkOtherShapeIDRectangle()
{
	return true;
}

bool COtherShape::checkAll()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ����

void COtherShape::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
	float dis_x = (beginx - endx)/len_beginToend*radius;
	float dis_y = (beginy - endy)/len_beginToend*radius;

	float mid_x = endx + dis_x;
	float mid_y = endy + dis_y;

	float mid_x_move = mid_x - endx;
	float mid_y_move = mid_y - endy;

	float arc = (3.1415926-2*asin((0.5*len_beginToend)/radius))/2;

	center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
	center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);

	center_x = center_x + endx;
	center_y = center_y + endy;
}

std::vector<param_point> COtherShape::convert_UIParamList_FromUI(std::vector<param_point> ui_pa_p)
{
	std::vector<param_point> pa_p;
	std::string othershapeID = OtherShapeHelper::GetOtherShapeId(ui_pa_p);

	//����������ˢ��UI��
	vector<param_list> pick_list = m_otherShapeParamList[atoi(othershapeID.c_str())];


	//����ʱ����Ʃ��a�滻��gapwidth
	for (int p_j = 0;p_j < ui_pa_p.size();p_j++)
	{
		if (ui_pa_p[p_j]._name.compare("ID") == 0)
		{
			pa_p.push_back(ui_pa_p[p_j]);
		}
		for (int p_i = 0;p_i < pick_list.size();p_i++)
		{
			std::string list_name = pick_list.at(p_i)._name;
			std::string list_remark = pick_list.at(p_i)._remark;
			std::string list_abbreviation = pick_list.at(p_i)._abbreviation;
			if (ui_pa_p[p_j]._name.compare(list_abbreviation) == 0)
			{
				param_point pa;
				pa._name = list_name;
				pa._value = ui_pa_p[p_j]._value;
				pa_p.push_back(pa);
			}

		}
	}
	return pa_p;
}



//////////////////////////////////////////////////////////////////////////
// �������

std::string COtherShape::GetPointInfosString()
{
	return OtherShapeHelper::GetPointInfoStringFromPointInfo(GetPointInfos());
}

std::string COtherShape::GetOtherShapeString()
{
	return m_othershapeString.GetBuffer();
}



std::string COtherShape::ChangetypeStr() const
{
	char temp[10];
	memset(temp,0,10);
	itoa(m_changetype,temp,10);
	return HGTools::Convert2StdString(temp);
}
































void COtherShape::OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CRect rc,listrc,dlgrc;
	m_ParamList.GetWindowRect(listrc);
	this->GetWindowRect(dlgrc);
	if(pNMItemActivate->iItem!=-1 && pNMItemActivate->iSubItem == 2 && pNMItemActivate->iItem > 3)
	{
		m_row=pNMItemActivate->iItem;
		m_column=pNMItemActivate->iSubItem;
		m_ParamList.GetSubItemRect(pNMItemActivate->iItem, pNMItemActivate->iSubItem,LVIR_LABEL,rc);
		m_ParamList.GetClientRect(listrc);
		rc.left = rc.left + 12;
		rc.top = rc.top + 12;
		rc.right = rc.right + 12;
		rc.bottom = rc.bottom + 12;

		char * ch=new char [128];
		m_ParamList.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem,ch,128);
		m_ParamListEdit.SetWindowText(ch);
		m_ParamListEdit.ShowWindow(SW_SHOW);
		m_ParamListEdit.MoveWindow(&rc);
		m_ParamListEdit.SetFocus();
		m_ParamListEdit.CreateSolidCaret(1,rc.Height()-5);
		m_ParamListEdit.ShowCaret();
		m_ParamListEdit.SetSel(-1);
	}
	else
	{
		m_ParamListEdit.ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}


void COtherShape::OnEnKillfocusListedit()
{
	CString str;
	m_ParamListEdit.GetWindowText(str);
	m_ParamList.SetItemText(m_row,m_column,str);
	m_ParamListEdit.ShowWindow(SW_HIDE);
}


void COtherShape::OnEnErrspaceListedit()
{
}




/************************************************************************/
/*                                                                      */
/************************************************************************/

void COtherShape::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}


void COtherShape::OnBnClickedButton1()
{
	
	std::string str_picture = m_PanelOutlineList.at(0)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton2()
{
	
	std::string str_picture = m_PanelOutlineList.at(1)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton3()
{
	
	std::string str_picture = m_PanelOutlineList.at(2)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton4()
{
	
	std::string str_picture = m_PanelOutlineList.at(3)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton5()
{
	
	std::string str_picture = m_PanelOutlineList.at(4)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton6()
{
	
	std::string str_picture = m_PanelOutlineList.at(5)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton7()
{
	
	std::string str_picture = m_PanelOutlineList.at(6)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton8()
{
	
	std::string str_picture = m_PanelOutlineList.at(7)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton9()
{
	
	std::string str_picture = m_PanelOutlineList.at(8)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton10()
{
	
	std::string str_picture = m_PanelOutlineList.at(9)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton11()
{
	
	std::string str_picture = m_PanelOutlineList.at(10)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton12()
{
	
	std::string str_picture = m_PanelOutlineList.at(11)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton13()
{
	
	std::string str_picture = m_PanelOutlineList.at(12)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton14()
{
	
	std::string str_picture = m_PanelOutlineList.at(13)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton16()
{
	
	std::string str_picture = m_PanelOutlineList.at(14)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}


void COtherShape::OnBnClickedButton17()
{
	
	std::string str_picture = m_PanelOutlineList.at(15)._path;
	//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
	std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
	CString file_name,_picture;
	file_name.Format("%s",sfile_name.c_str());
	_picture.Format("%s",str_picture.c_str());
	m_PictureList.SetFolder(128,128,file_name,_picture);
}



//////////////////////////////////////////////////////////////////////////
//

void COtherShape::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int nItem = (int)m_pictreelist.GetSelectedItem();
	if (nItem < 0)
	{
		return;
	}
	else
	{
		std::string str_picture = m_PanelOutlineList.at(nItem)._path;
		//std::string sfile_name = hg3d::getFullFileName("pdata\\PanelStyle");
		std::string sfile_name = GetModulePath() + "pdata\\PanelStyle";
		CString file_name,_picture;
		file_name.Format("%s",sfile_name.c_str());
		_picture.Format("%s",str_picture.c_str());
		m_PictureList.SetFolder(128,128,file_name,_picture);
	}
	*pResult = 0;
}
















