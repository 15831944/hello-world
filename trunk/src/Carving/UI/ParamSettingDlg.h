#pragma once

#include "../resource.h"
#include "afxwin.h"
#include "../Misc/XmlHandlePlus.h"
#include "../Work/WorkDef.h"
// ParamSettingDlg �Ի���

class ParamSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParamSettingDlg)

public:
	ParamSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ParamSettingDlg();

// �Ի�������
	enum { IDD = IDD_PARAM_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	/************************************************************************/
	/* UI ����                                                              */
	/************************************************************************/
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

private:

public:
	/************************************************************************/
	/* �߼�                                                                 */
	/************************************************************************/
private:
	//�����ļ���װ
	void ReloadFile();
	//�����ļ���װ
	void SaveFile();

public:
	//��������
	void loadSettings();
	//��������
	void saveSettings();

private:
	CString m_UISettingHgmFilePath;
	static long long m_UISettingLastTime;
	static TinyXml::TiXmlDocument m_UISetting_hgm;

public:
	/************************************************************************/
	/* �������                                                             */
	/************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	//����
#if 1
private:
	//��ǩ�����߷�������
	CString m_tiebiao_panel_edge;
	//�����߷����б�
	CListBox m_tiebiao_edge_list;

	//����ƫ��X
	float m_tiebiao_offset_x;
	//����ƫ��Y
	float m_tiebiao_offset_y;

	//�Զ��ܿ�
	BOOL m_tiebiao_auto_vec;
	//�Զ��ܲ�
	BOOL m_tiebiao_auto_slot;
	//�Զ���������
	BOOL m_tiebiao_auto_inner;
	//�Զ�����������
	BOOL m_tiebiao_auto_outline;

	// ÿ��ӡ�����ſ�ʼ��Ϣ
	int m_print_per_num;
	// ÿ��ӡ��������Ϣ���
	int m_print_sleep;

	//����λ��
	int m_nLablePosType;

	//���ϵ�����ʽ
	LingLiaoSortType m_eLingLiaoSortType;

	//���Ǻ��޸ķ�ʽ
	ReminderModifyType m_eReminderModifyType;

public:
	//��ǩ�����߷�������
	CString Tiebiao_panel_edge() const { return m_tiebiao_panel_edge; }
	//��ǩ�����߷�������
	void Tiebiao_panel_edge(CString val) { m_tiebiao_panel_edge = val; }

	//����ƫ��X
	float Tiebiao_offset_x() const { return m_tiebiao_offset_x; }
	//����ƫ��X
	void Tiebiao_offset_x(float val) { m_tiebiao_offset_x = val; }

	//����ƫ��Y
	float Tiebiao_offset_y() const { return m_tiebiao_offset_y; }
	//����ƫ��Y
	void Tiebiao_offset_y(float val) { m_tiebiao_offset_y = val; }


	//�Զ��ܿ�
	BOOL Tiebiao_auto_vec() const { return m_tiebiao_auto_vec; }
	//�Զ��ܿ�
	void Tiebiao_auto_vec(BOOL val) { m_tiebiao_auto_vec = val; }
	//�Զ��ܲ�
	BOOL Tiebiao_auto_slot() const { return m_tiebiao_auto_slot; }
	//�Զ��ܲ�
	void Tiebiao_auto_slot(BOOL val) { m_tiebiao_auto_slot = val; }
	//�Զ���������
	BOOL Tiebiao_auto_inner() const { return m_tiebiao_auto_inner; }
	//�Զ���������
	void Tiebiao_auto_inner(BOOL val) { m_tiebiao_auto_inner = val; }
	//�Զ�����������
	BOOL Tiebiao_auto_outline() const { return m_tiebiao_auto_outline; }
	//�Զ�����������
	void Tiebiao_auto_outline(BOOL val) { m_tiebiao_auto_outline = val; }
	//�Զ���ȫ��
	BOOL Tiebiao_auto_all_not() const { return (!Tiebiao_auto_vec() && !Tiebiao_auto_slot() && !Tiebiao_auto_inner() && !Tiebiao_auto_outline()); }

	//˫����ӷ��
	afx_msg void OnLbnDblclkTiebiaoEdge();
	//��ӷ��
	afx_msg void OnBnClickedTiebiaoEdgeAdd();
	//�༭���
	afx_msg void OnBnClickedTiebiaoEdgeEdit();
	//ɾ�����
	afx_msg void OnBnClickedTiebiaoEdgeDel();
	//����������ȼ�
	afx_msg void OnBnClickedTiebiaoEdgeUp();
	//����������ȼ�
	afx_msg void OnBnClickedTiebiaoEdgeDown();
	//�ӽ����л�÷������
	CString GetEdgeFromUI();
	//��÷�����õ�������
	void SetEdgeToUI(CString edge);

	
	// ÿ��ӡ�����ſ�ʼ��Ϣ
	int Print_per_num() const { return m_print_per_num; }
	// ÿ��ӡ�����ſ�ʼ��Ϣ
	void Print_per_num(int val) { m_print_per_num = val; }
	
	// ÿ��ӡ��������Ϣ���
	int Print_sleep() const { return m_print_sleep; }
	// ÿ��ӡ��������Ϣ���
	void Print_sleep(int val) { m_print_sleep = val; }

	//����λ��
	int LabelPosType() const { return m_nLablePosType;}
	void LabelPosType(int nType){ m_nLablePosType = nType; }

	//���ϵ�����ʽ
	LingLiaoSortType LingLiaoSort() const { return m_eLingLiaoSortType;}
	void LingLiaoSort(LingLiaoSortType eType){ m_eLingLiaoSortType = eType; }

	//���Ǻ��޸ķ�ʽ
	ReminderModifyType ReminderModify() const { return m_eReminderModifyType;}
	void ReminderModify(ReminderModifyType eType){ m_eReminderModifyType = eType; }
#endif

public:

	CComboBox m_cbxDecimaPlace;		// С��λ��
	int m_valDecimalPlace;

	int getDecimalPlace()  { return m_valDecimalPlace; }
	void setDecimalPlace(int val) { m_valDecimalPlace = val; }

	BOOL m_chkboxPanelComments;
};
