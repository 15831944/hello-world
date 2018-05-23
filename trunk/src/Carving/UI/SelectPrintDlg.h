/*--------------------------------------------------------------------------------------------------------------------*/
//	SelectPrintDlg.h      --	 ѡ���ӡ���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.2.20
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

class Panel;
class Component;


class CSelectPrintDlg : public CDialogEx//��ӡ�Ի���
{
	DECLARE_DYNAMIC(CSelectPrintDlg)

public:
	CSelectPrintDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPrintDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_PRINT };


	std::vector<print_info> _printinfo;			//����Դ
	map<int,Panel_Knife> m_pkcoor;				//�����Ϣ


	//����б�
	CListCtrl m_panelList;		
	//ȫѡ���
	CButton m_checkSelectAllPanel;

	//Ĭ���Ƿ�ȫѡ�������
	BOOL m_checkSelectAllPanelSetting;


	//С���б�
	CListCtrl m_smallpanelList;

	CButton m_checkSelectAllSmallpanel;

	//Ĭ���Ƿ�ȫѡС������
	BOOL m_checkSelectAllSmallpanelSetting;

	//ͼ��ѡ��
	CComboBox m_picStyle;


	//����������ʹ��
	BOOL m_PanelCodeEnable;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void InitPanelList();
	void InitComponentList();
	void GetAllPanelInPanelList(vector<Panel*>& vAllPanel);
	void GetAllPrintInfoOfComponent(vector<print_info>& vAllPrintInfo);
	int GetIndicatePanelCheckedComponentCount(int nPanelID);
public:


	virtual BOOL OnInitDialog();				//��ʼ��





	//���ô�ѡ�д��
	void setPanelListCheck(int bigPanelNum);
	//���ô�ѡ�д��
	void setPanelCheck(int panelNum);
	//��ȡ����NC������ļ�������Ϣ
	std::map<Panel*,std::map<CString,CString>> OnOutputgcodeFileName(vector<Panel*> vPanel, bool enable);
	//���ɶ�ά��
	void CreateQCodeInImage( CImage &image, CString codeWord, int code_x, int code_y , float code_size );
	//����Ĭ��ѡ������
	void loadSettings_DefaultSelect();
	//����Ĭ��ѡ������
	void saveSettings_DefaultSelect();


	afx_msg void OnSelectAllPanel();
	
	//ѡ�д��Ӱ��С��ѡ��
	afx_msg void OnCheckPanelItem(NMHDR *pNMHDR, LRESULT *pResult);





	//ȫѡС��
	afx_msg void OnSelectAllSmallpanel();






	//��ӡͼ��
	afx_msg void OnBtnPrintPanel();





	//��ӡ����
	afx_msg void OnBtnPrintSmallPanel();




	//�ر�
	afx_msg void OnBtnClose();




};
