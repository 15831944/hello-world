#pragma once
#include "../Resource.h"

// CDlgResetMachiningInfo �Ի���

class CDlgResetMachiningInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgResetMachiningInfo)

public:
	CDlgResetMachiningInfo(int nResetType, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResetMachiningInfo();

// �Ի�������
	enum { IDD = IDD_DLG_RESET_MACHINING_INFO };
	enum {RESET_OTHERSHAPE, RESET_SLOT, RESET_HOLE};

	int m_nResetOtherShape;
	int m_nResetSlot_UpperFace;
	int m_nResetSlot_DownerFace;
	int m_nResetHole_UpperFace;
	int m_nResetHole_DownerFace;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
};
