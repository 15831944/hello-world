#pragma once
#include "../Resource.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <map>
#include "afxpropertygridctrl.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"


class MatrialItemEditDlg : public CDialogEx//���г�Ʒ�Ի���
{
	DECLARE_DYNAMIC(MatrialItemEditDlg)

public:
	MatrialItemEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MatrialItemEditDlg();
	// �Ի�������
	enum { IDD = IDD_MATERIAL_ITEM_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
private:

	//////////////////////////////////////////////////////////////////////////
	//����༭
private:
	int m_currentIndex;										//��ǰ������Ŀ
	int m_totalLine;										//����Ŀ��
	CString m_checkInfo;									//��ѡ��Ϣ
	CMFCPropertyGridCtrl m_editPropertyTable;				//���Դ���

public:
	afx_msg void OnBnClickedNewItem();						//�¶���
	afx_msg void OnBnClickedPrevItem();						//��һ��
	afx_msg void OnBnClickedNextItem();						//��һ��
	afx_msg void OnBnClickedFindItem();						//����
	afx_msg void OnBnClickedAddItem();						//���
	afx_msg void OnBnClickedSaveItem();						//����
	afx_msg void OnBnClickedDelItem();						//ɾ��

	virtual BOOL OnInitDialog();							//��ʼ��
	virtual void OnOK();									//ok
	virtual void OnCancel();								//�ر�
 

	//////////////////////////////////////////////////////////////////////////
	//���ݴ洢
private:
	int m_currentShowIndex;												
	std::vector<int> m_selectList;										//ѡ�е���Ŀ
	//std::vector<std::map<std::string,std::string>> m_matrialList;		//������Ŀ
	vector<ComponentInputItem> m_vComponentInputItem;                 //������Ŀ
public:
	void setSourceMatrial(/*const std::vector<std::map<std::string,std::string>> matrialList*/vector<ComponentInputItem>& vComponentInputItem);	//���ݴ洢����������
	//std::vector<std::map<std::string,std::string>> getResultMatrial();							//���ݴ洢����ȡ����
	vector<ComponentInputItem> getResultMatrial();							//���ݴ洢����ȡ����
	void setSourceSelectIndex(const std::vector<int> selectList);								//����ѡ����Ŀ


	//////////////////////////////////////////////////////////////////////////
	// �����߼�

private:
	//�����µ�����
	CString newBarCode(int seed);
	//���Ψһ����
	CString uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck);
	//Ψһ��У׼
	std::vector<CString> m_barCodeCheck;


#if 1
	//������
	CString m_edit_order_num ;
	BOOL m_check_order_num ;

	//���
	CString m_edit_cabinet_num ;
	BOOL m_check_cabinet_num ;

	//��������
	CString m_edit_matrail_name ;
	BOOL m_check_matrail_name ;

	//�������
	CString m_edit_barcode ;
	BOOL m_check_barcode ;

	//��Ʒ����
	CString m_edit_porduct_name ;
	BOOL m_check_porduct_name ;

	//���ϳ���
	CString m_edit_cut_len ;
	BOOL m_check_cut_len ;

	//���Ͽ��
	CString m_edit_cut_width ;
	BOOL m_check_cut_width ;

	//���Ϻ��
	CString m_edit_cut_thin ;
	BOOL m_check_cut_thin ;

	//��������
	CString m_edit_cut_num ;
	BOOL m_check_cut_num ;

	//��·����
	CString m_edit_cut_dir ;
	BOOL m_check_cut_dir ;

	//�ⳤ1
	CString m_edit_edge_len_1 ;
	BOOL m_check_edge_len_1 ;

	//�ⳤ2
	CString m_edit_edge_len_2 ;
	BOOL m_check_edge_len_2 ;

	//���1
	CString m_edit_edge_width_1 ;
	BOOL m_check_edge_width_1 ;

	//���2
	CString m_edit_edge_width_2 ;
	BOOL m_check_edge_width_2 ;

	//�ͻ���Ϣ
	CString m_edit_custome_info ;
	BOOL m_check_custome_info ;

	//���˵�
	CString m_edit_store ;
	BOOL m_check_store ;

	//�Ƿ񿪲�
	CString m_edit_slot ;
	BOOL m_check_slot ;

	//����
	CString m_edit_other ;
	BOOL m_check_other ;

	//�ͻ���ַ
	CString m_edit_custome_addr ;
	BOOL m_check_custome_addr ;
#endif

public:
	void clearModel();
	//void fromMap(std::map<std::string,std::string> item );
	void fromInputItem(ComponentInputItem item);
	//std::map<std::string,std::string> toMap(std::map<std::string,std::string> inputBase = std::map<std::string,std::string>());
	ComponentInputItem toInputItem(ComponentInputItem inputBase = ComponentInputItem());

	bool isIndexAvalible();
	bool isCurrDataOK();
	void IndexPrev();
	void IndexNext();
	void InsertNewItem();
	void LoadCurrentItem();
	void SaveCurrentItem();
	void DelCurrentItem();

	afx_msg void OnBnClickedReloadItem();
};
