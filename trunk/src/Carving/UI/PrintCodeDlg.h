#pragma once
#include <vector>
#include <map>
#include "ImageListCtrl.h"
//#include "SelectPrintDlg.h"
#include "afxwin.h"
#include "../Resource.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

using namespace TinyXml;

// CPrintCodeDlg dialog
extern class CPrintDialog;
struct code_all_info
{
	std::vector<CString> m_codeinfo;
	CString m_def;
	float m_width;
	float m_height;
	CString s_width;
	CString s_height;
	CString m_cstno;
};

struct panel_picture
{
	float _width;
	float _height;
	float left_edge;
	float right_edge;
	float up_edge;
	float down_edge;
	std::string s_width;
	std::string s_height;
	int _dir;
	int nRotatedAngle;
};

struct recode_print
{
	std::map<std::string,std::string> printtext;
	std::string code_info;
	std::string logo_path;
	panel_picture _picture;
	std::string code_info2;
};

struct xml_module
{
	std::string _x;
	std::string _y;
	std::string _fontsize;
	std::string _fontname;
	std::string _frontname;
	std::string _leftstr;
	std::string _rightstr;
	std::string _used;
};

struct def_pos
{
	int x;
	int y;
};



class CPrintCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintCodeDlg)

public:
	CPrintCodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrintCodeDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_PRINTCODEDLG };
	std::vector<std::vector<std::string>> m_pall_cominfo;
	CImageList image_list;
	wstring ANSIToUnicode(const string& str);
	void Wchar_tToString(std::string& szDst, wchar_t *wchar);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDaYinAnNiu();
	CImageListCtrl m_codelist;
	std::map<int,code_all_info> m_list_info;

	afx_msg void OnPaint();

	void BuildCodeBmp();

	//ͨ��·�����ͼƬ�ߴ�
	void getImageWidthHeight(CString path , int& image_width, int& image_height);

	void Paint_image(CDC* pDC, CString path ,int rotate, int pos_x, int pos_y );

	//���ɰ��ͼƬ
	void Paint_PanelImage(CDC* pDC, int offset_x, int offset_y,std::string s_width, std::string s_height, CString fontName,int fontSize,
		CString edge_img_width_0,CString edge_img_width_1,CString edge_img_height_0,CString edge_img_height_1,
		CString arrow_img_path,CString shape_img_path);

	void BuildOneCodeImage( CString sourceImagePath, CString destImagePath, /*hg3d::CXmlNode * print_settings*/TiXmlElement* elmPrintSetting, recode_print& printInfomation,bool dash=false);

public:
	BOOL PrintPic();
	void OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight);


	//�����ݷֶν�ȡ��չ��Դ�ַ��� ǰ��ֱ��ȡn���ַ����ȣ�
	void BuildCodeExtend(std::map<std::string,std::string>& dst,const std::string keyname);

	void TransitionPrint(TiXmlElement* elmPrintSetting);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnExportPanelA4();			//��ӡ��ǩ������Ǵ�ͼ
	int MyBarcode(CDC* cdc,int x1,int y1,int y2,int codetype,int ipen,const char * cCode);
	int GetBarcodeLen(int ipen,const char * cCode);
	afx_msg void OnDaoChuAnNiu();

	void ExportPanelPic(bool isPanelUseBigPic);

	void ScaleSaveImage(double scale, CString srcFile, CString dstFile );

	CComboBox m_comboScaling;
//	afx_msg void OnCbnEditchangeBqcombo();
	afx_msg void OnCbnSelchangeBqcombo();
	double getDaoChuBiaoQianBiLi();
	void sortFile(CString file_path ,CString prefix_name);

public:
	//�����Ϣ
	int m_panelPictureNumCount;				//���ͼƬ����
	std::vector<int> m_panelNumList;		//���������
	std::vector<CString> m_panelSmallPicPathList;	//���СͼƬ��ַ
	std::vector<CString> m_panelBigPicPathList;	//����ͼƬ��ַ
	bool m_panelNCCode;						//�Ƿ������˴��NC��

	//С����Ϣ
	std::vector<recode_print> m_print_info;	//С���ӡ�����ݣ�ʹ�ã�
	std::vector<CString> m_print_path;		//С���ӡ��Ŀ¼��ʹ�ã�
	std::vector<print_info> m_info;			//С���ӡ��Ϣ�����룩

	std::vector<int> m_vComponentLableID;
};
