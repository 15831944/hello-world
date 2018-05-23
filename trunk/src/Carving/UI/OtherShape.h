#pragma once

#include <vector>
#include <map>
#include "ImageListCtrl.h"
//#include "KnifeClass.h"

//#include "YHInfoTools.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Resource.h"

// COtherShape dialog
struct point_vector
{
	std::string _x;
	std::string _y;
};


struct param_list
{
	std::string _name;
	std::string _remark;
	std::string _abbreviation;
};

struct Shape_NamePath 
{
	std::string _name;
	std::string _path;
	std::string _index;
};

//�������öԻ���
class COtherShape : public CDialogEx
{
	DECLARE_DYNAMIC(COtherShape)

public:
	COtherShape(CWnd* pParent = NULL);   // standard constructor
	virtual ~COtherShape();

// Dialog Data
	enum { IDD = IDD_OTHERSHAPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	//���ط����ļ� ��
	//����ˢ�µ������б��� ��
	//������ǰotherShapeID ��
	//����OtherShapeID���ڷ���ͼ��ͼƬ����ѡ�� ��
	//����OtherShapeID���ڲ����б� ��
	//���ز����������б��� ��
	//ͨ�������б�����outline ��
	//���outline�������� ��
	//����outline��ͼ	��
	//��������������תoutline ��
	//ȷ������outline������string ��


	//��ת���
	//�����仯�ػ� ��
	//���ƹ�������������ֹͣ���� ��

private:
	static bool isInitPanelOutlineList;
	static vector<Shape_NamePath> m_PanelOutlineList;									//���κͷ���
	static map<int,vector<param_list>> m_otherShapeParamList;							//���β����б�
	static std::map<int,std::vector<std::map<std::string,std::string>>> m_outlineParam;	//�������ε���Ϣ



	std::vector<std::map<std::string,std::string>> m_outline;	//���β�������
	std::map<std::string,std::string> m_item;					//��������
	std::vector<PointInfo> m_pointInfos;						//���ε���
	int m_changetype;											//��ת�Ƕ�


	//////////////////////////////////////////////////////////////////////////
	// �¼�
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo3();

	//����ͼƬѡ��
	afx_msg void OnPictureListSelectChange(NMHDR *pNMHDR, LRESULT *pResult);

	//�޸����ΰ����ݲ���
	afx_msg void OnEnChangeListedit();

	//�������η���ѡ��
	afx_msg void OnChangeSelectClassifyList();

	//ˢ����������
	afx_msg void OnBnClickedBtnRefresh();
public:
	std::map<std::string,std::string> Item() const { return m_item; }
	void Item(std::map<std::string,std::string> val) { m_item = val; }

	vector<PointInfo> GetPointInfos(){ return m_pointInfos; }
	std::string GetPointInfosString();
	std::string GetOtherShapeString();

	int Changetype() const { return m_changetype; }
	std::string ChangetypeStr() const;
	void Changetype(int val) { m_changetype = val; }
	void Changetype(std::string val) { m_changetype = atoi(val.c_str()); }

	//////////////////////////////////////////////////////////////////////////
	// ��д����
public:
	//���ļ��м�������������������
	void load_OutlineList_ParamList();

	//�������������ݸ��µ����ݷ�����
	void write_UIClassifyList_ByOutlineList();

	//������ID�ַ�����ӳ������ͼƬѡ���Լ����ݷ���ѡ��
	void write_UIPictureList_ByOtherShapeID(std::string otherShapeId = "");

	//��������ID��Ӧ�Ĳ����б�
	void write_UIParamList_ByOtherShapeID(std::string otherShapeId, std::vector<param_point> pa_p = std::vector<param_point>());

	//���ڲ�m_pointInfos����������
	void paint_UIPicture_ByOutline();

	//�ӽ����ϼ���������
	std::vector<param_point> read_UIParamList_FromUI();

	//////////////////////////////////////////////////////////////////////////
	// �ж�
public:
	bool isPaintable();
	bool checkParamPointLegal(const std::vector<param_point>& pa_p);
	bool checkDrawLegal(const std::vector<PointInfo>& pointInfos);

	bool checkOtherShapeIDRectangle();
	bool checkAll();

private:
	bool m_checkParamPointLegal;
	bool m_checkDrawLegal;
	bool m_paint_UIPicture_ByOutline_ok;

	//////////////////////////////////////////////////////////////////////////
	// ����
public:
	void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

	std::vector<param_point> convert_UIParamList_FromUI(std::vector<param_point> ui_pa_p);










public:
	int m_width;
	int m_height;
	std::string m_resultvalue;
	std::string m_stypeid;
	std::string m_isrot;

	

	CString m_othershapeString;	//����string

	CString m_panelwidth;
	CString m_panelheight;

	vector<std::string> m_param;
	vector<point_vector> m_allpoint;

	CString m_panelID;
	afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusListedit();
private:
	int m_row;				//���ڶ�λ�༭�����б������
	int m_column;			//���ڶ�λ�༭�����б������

public:
	afx_msg void OnEnErrspaceListedit();


	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_button5;
	CButton m_button6;
	CButton m_button7;
	CButton m_button8;
	CButton m_button9;
	CButton m_button10;
	CButton m_button11;
	CButton m_button12;
	CButton m_button13;
	CButton m_button14;
	CButton m_button15;
	CButton m_button16;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	CTreeCtrl m_pictreelist;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	

private:
	CListBox m_ClassifyList;		//�����б�
	CImageListCtrl m_PictureList;	//ͼƬ�б�
	CListCtrl m_ParamList;			//�����б�
	CEdit m_ParamListEdit;			//�༭�����б��õ��������
	CComboBox m_comchange;			//��תѡ��

public:
	
};
