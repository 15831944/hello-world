#pragma once
//#include "stdafx.h"
#include <string>
#include <vector>
#include <map>


// CSelMachineDlg dialog

class CSelMachineDlg : public CDialogEx//����G��Ի���
{
	DECLARE_DYNAMIC(CSelMachineDlg)

public:
	CSelMachineDlg(CString title = "",CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelMachineDlg();

// Dialog Data
	enum { IDD = IDD_SEL_MACHINE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	/************************************************************************/
	/* �����¼�                                                             */
	/************************************************************************/
public:
	virtual BOOL OnInitDialog();		// ��ʼ��
	afx_msg void OnBnClickedOk();		// ȷ��
	afx_msg void OnBnClickedCancel3();	// ȡ��
	afx_msg void OnNMDblclkType(NMHDR *pNMHDR, LRESULT *pResult);	// ˫����
	void loadSettings();				//��������
	std::vector<std::map<std::string,std::string>> getMachineKnifes();	//��õ�ǰ���͵����е���

	
public:
	CString m_windowTitle;

	CComboBox m_combo;
	std::string m_nodename;
	std::string m_printname;
	std::string m_pname;
	std::string m_plast;
	std::string m_cname;
	std::string m_clast;
	std::string m_dvcut;			// 0,�����ף�1˫����1��W��,2˫����2��H��
	std::string m_pxhx;				// p���»���
	std::string m_cxhx;				// c���»���

	int m_usedLineX1;				// �û�������Ч�ߣ������ޱ�ƫ�ƣ���λ1X��
	int m_usedLineY1;				// �û�������Ч�ߣ������ޱ�ƫ�ƣ���λ1Y��
	int m_usedLineX2;				// �û�������Ч�ߣ������ޱ�ƫ�ƣ���λ2X��
	int m_usedLineY2;				// �û�������Ч�ߣ������ޱ�ƫ�ƣ���λ2Y��
	CString m_EachNCLineExtra;		// NC�ļ�ÿ��׷��
	CString m_SmallPanelNC;			//С��NC�Ƿ񵼳�
	CString m_fileClassifyMove;		// ����NC�������ļ�����
	CString m_NC_CommandChange;		// NC�����ֱ��
	CString m_NC_SpaceRemove;		// NC����ո�ɾ��
	CString m_NC_ModifyArcMillingFormat;// NC�޸�ϳ����ʽ

	int m_iscombine;				// �Ƿ�ϲ�����ͷβ
	int m_iscomline;				// �Ƿ�����NC�ļ�
	int m_iscomball;				// �Ƿ�ϲ�����NC�ļ�
	int m_lessKnifeChange;			// ���ٻ�������

	bool _cutdouble;
	bool _coorsysuse;
	int _coortype;
	int _coortype2;
	std::string _grouptype;
	std::string _groupstr;
	CListCtrl m_type;

	std::string m_header;		//���ļ�ͷGFirst1
	std::string m_ender;		//���ļ�βGLast1
	std::string m_GFirst2;		//���ļ�ͷGFirst2
	std::string m_GLast2;		//���ļ�βGLast2

	std::string m_GBFirst1;		//��B�ļ�ͷGFirst1
	std::string m_GBFirst2;		//���ļ�βGLast1
	std::string m_GBLast1;		//��B�ļ�WEI
	std::string m_GBLast2;		//���ļ�βGLast2
	 

	std::string m_MFirst1;		//�����ļ�ͷGFirst1
	std::string m_MLast1;		//�����ļ�βGLast1
	std::string m_MFirst2;		//�����ļ�ͷGFirst2
	std::string m_MLast2;		//�����ļ�βGLast2

	std::string m_PZFirst1;		//�����ļ�ͷGFirst1
	std::string m_PZLast1;		//�����ļ�βGLast1
	std::string m_PZFirst2;		//�����ļ�ͷGFirst2
	std::string m_PZLast2;		//�����ļ�βGLast2


	std::string m_SPFirst1;		//С���ļ�ͷGFirst1
	std::string m_SPLast1;		//С���ļ�βGLast1
	std::string m_SPFirst2;		//С���ļ�ͷGFirst2
	std::string m_SPLast2;		//С���ļ�βGLast2

	float m_trim1;				//�����ޱ�ֵ
	float m_trim2;
	float m_trim3;
	float m_trim4;

	float m_ftrim1;//�����ޱ�ֵ
	float m_ftrim2;
	float m_ftrim3;
	float m_ftrim4;

	bool m_bLabelInReverseSide;
};
