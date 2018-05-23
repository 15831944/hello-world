#pragma once
#include "../Resource.h"

// LingLiaoDlg �Ի���

class LingLiaoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LingLiaoDlg)

public:
	LingLiaoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LingLiaoDlg();

// �Ի�������
	enum { IDD = IDD_LINGLIAO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void loadSettings();
	void saveSettings();

private:
	// ��˾����
	CString m_CompanyName;
	// �������
	CString m_OrderID;
	// �Ų���Ա
	CString m_ShiftEmployee;
	// �Ų�����
	CString m_ShiftDate;
	// ������Ա
	CString m_UseEmployee;
	// ��������
	CString m_UseDate;
	// �ֹ���Ա
	CString m_ManageEmployee;
	// �ӵ�����
	CString m_OrderStore;
	// �����Ա
	CString m_AuditEmployee;
	// ��������
	CString m_UseMachineName;
	// ����ע��
	CString m_OrderMoreInfo;

public:
	// ��˾����
	CString CompanyName() const { return m_CompanyName; }
	void CompanyName(CString val) { m_CompanyName = val; }

	// �������
	CString OrderID() const { return m_OrderID; }
	void OrderID(CString val) { m_OrderID = val; }

	// �Ų���Ա
	CString ShiftEmployee() const { return m_ShiftEmployee; }
	void ShiftEmployee(CString val) { m_ShiftEmployee = val; }

	// �Ų�����
	CString ShiftDate() const { return m_ShiftDate; }
	void ShiftDate(CString val) { m_ShiftDate = val; }

	// ������Ա
	CString UseEmployee() const { return m_UseEmployee; }
	void UseEmployee(CString val) { m_UseEmployee = val; }

	// ��������
	CString UseDate() const { return m_UseDate; }
	void UseDate(CString val) { m_UseDate = val; }

	// �ֹ���Ա
	CString ManageEmployee() const { return m_ManageEmployee; }
	void ManageEmployee(CString val) { m_ManageEmployee = val; }

	// �ӵ�����
	CString OrderStore() const { return m_OrderStore; }
	void OrderStore(CString val) { m_OrderStore = val; }

	// �����Ա
	CString AuditEmployee() const { return m_AuditEmployee; }
	void AuditEmployee(CString val) { m_AuditEmployee = val; }

	// ��������
	CString UseMachineName() const { return m_UseMachineName; }
	void UseMachineName(CString val) { m_UseMachineName = val; }

	// ����ע��
	CString OrderMoreInfo() const { return m_OrderMoreInfo; }
	void OrderMoreInfo(CString val) { m_OrderMoreInfo = val; }

};
