#pragma once

//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include <map>
//#include "afxcmn.h"
//
#include "TheChangeCode.h"
#include "MainCode.h"
#include "PzCode.h"
#include "SpCode.h"
#include "BMainCode.h"
//#include "afxwin.h"
#include "../../../include/TinyXml/tinyxml.h"

#include "../Resource.h"
#include <vector>
#include <map>

using namespace std;

// CKnifeAddDlg dialog

class CKnifeAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKnifeAddDlg)

public:
	CKnifeAddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKnifeAddDlg();

// Dialog Data
	enum { IDD = IDD_KNIFE_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_knifecode;
	float m_knifel;
	float m_knifer;
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddKnife();
	CString m_nearspeed;
	CString m_feedrate;
	CString m_mgcode;
	CString m_spindlespeed;
	CString m_tooldiameter;
	CString m_toollength;
	CString m_KnifeLength;
	CString m_typename;
	CListCtrl m_knifelist;
	afx_msg void OnBtnDelKnife();
	CComboBox m_comtype;
	CString m_mgcodee;
	std::map<int,int> map_comtype;
	CString m_endspeech;
	CString m_overspeech;
	afx_msg void OnNMDblclkKnifelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComtype();
	CString m_coorx;
	CString m_coory;
	CString m_knum;
	CString m_ksysa;
	CString m_ksysb;
	CString m_mgcode2;
	CString m_mgcodee2;
	CListCtrl m_knifetype;
	
	int t_Item;
	int l_Item;

	POINT oSize;
	bool ischeck;//�޸�m_Knifelistĳ��֮ǰ������ֵ�����Ƿ�ѡ��״̬����������ӵ�ʱ������д�뵽��������
	afx_msg void OnNMClickKnifetype(NMHDR *pNMHDR, LRESULT *pResult);

	void InputKnifeTypeParamToDialog( CString &str_name );

	afx_msg void OnBtnChangeKnife();
	afx_msg void OnNMDblclkKnifetype(NMHDR *pNMHDR, LRESULT *pResult);

	CTabCtrl m_tabChange;
	TheChangeCode page1;
	MainCode      page2;
	PzCode        page3;
	BMainCode     page4;
	SpCode		  page5;
	afx_msg void OnTcnSelchangingTabchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickKnifelist(NMHDR *pNMHDR, LRESULT *pResult);

	void InputMessageToDialog(CString theName);	//��������Ϣ������Ի���
	
	CComboBox m_double;				//��˫��λ����
	CComboBox m_coorsysuse;			//ƫ�Ʒ�ʽ����
	CComboBox m_dvcut;				//��˫��������
	CImageList m_imglist;			//��ͷ�ļ��������б���Ʊ���CImageList m_imglist;
	CComboBoxEx m_coortype;			//����ϵ����
	CString m_btrim1;				//�����ޱ�1
	CString m_btrim2;
	CString m_btrim3;
	CString m_btrim4;
	CString m_ftrim1;				//�����ޱ�1
	CString m_ftrim2;
	CString m_ftrim3;
	CString m_ftrim4;
	CString m_thepname;				//�����ļ���
	CString m_theplast;				//�����ļ���׺
	CString m_thecname;				//�����ļ���
	CString m_theclast;				//�����ļ���׺
	CString m_groupstr;				//�����ļ���׺
	//CString m_autop;				//�Զ�����ֵ
	CComboBox m_comboLabelMachine;  //�Զ�����ֵ

	afx_msg void OnBtnMachineSettingSave();

	int getSelect2Setting(CComboBox& combox);
	void setSelectText(CComboBox& combox, std::string theName );

	void EditKnifeList(int theIndex);	//�޸�
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_iscombine;
	CButton m_Iscomline;
	CButton m_iscomball;						//�Ƿ���������nc�ϲ�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_combineSet;
	CComboBoxEx m_coortype2;

	std::map<std::string,std::string> m_usedLineMap;
	CComboBox m_usedLineX1;// ��λ1��ЧXʹ��������
	CComboBox m_usedLineY1;// ��λ1��ЧYʹ��������
	CComboBox m_usedLineX2;// ��λ2��ЧXʹ��������
	CComboBox m_usedLineY2;// ��λ2��ЧYʹ��������
	
	CString m_EachNCLineExtra;	// ����ÿ��NC���������ָ������
	CButton m_lessKnifeChange;
	BOOL m_smallPanelNC;		// С���Ƿ����NC
	BOOL m_fileClassifyMove;	// ����NC�������ļ�����
	BOOL m_NC_CommandChange;	// NC�����ֱ��
	BOOL m_NC_SpaceRemove;		// NC����ո�ɾ��
	BOOL m_bModifyArcMillingFormat; //�޸�ϳ����ʽ

public:
	//����������Ϣ
	void loadSettings();
	//����������Ϣ
	void saveSettings();
	//���ػ�����
	std::vector<CString> getMachineNames();
	//���ػ���ʹ��״̬
	std::vector<CString> getMachineUseds();

private:
	TinyXml::TiXmlDocument m_doc;	//�ĵ����ݻ����ڵ�
public:
	
};
