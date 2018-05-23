#pragma once
#include "afxwin.h"

#include "../Misc/CIniFile.h"
#include <map>
#include <vector>
#include "../resource.h"

// CParamDlg dialog


//��������
class CParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParamDlg)

public:
	CParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamDlg();

// Dialog Data
	enum { IDD = IDD_BASE_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	//UI
	virtual BOOL OnInitDialog();
	void update2Dialog();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	//logic
	void loadSettings();
	void saveSettings();

	// ���µ������еĻ�����Ϣ
	void UpdateBaseInfo(void);


public:
	/************************************************************************/
	/* �и����                                                             */
	/************************************************************************/
	//�и���� ���ִ��ڳ�ʼ��
	void CParamDlg::OnInitDialog_QieGeCanShu();

	//�������ݱ仯������
	void CParamDlg::updateToDialog_QieGeCanShu();

	//�и���� ���ݼ���
	void CParamDlg::loadSettings_QieGeCanShu();

	//�и���� ���ݼ���
	void CParamDlg::saveSettings_QieGeCanShu();

	//�ӽ����������
	void updateFromDialog_QieGeCanShu();

	//��Ӧ��������޸�
	void CParamDlg::OnCbnSelchangeComboYuandian();

	//��ȫ�߶�
	float m_upheight;
	float Upheight() const { return m_upheight; }
	void Upheight(float val) { m_upheight = val; }

	//�µ����
	float m_downheight;
	float Downheight() const { return m_downheight; }
	void Downheight(float val) { m_downheight = val; }

	//�Ե�ԭ��
	CComboBox m_DuiDaoYuanDian;

	//�µ����
	float m_knife_org_offset;
	float KnifeOrgOffset() const { return m_knife_org_offset; }
	void KnifeOrgOffset(float val) { m_knife_org_offset = val; }


	//�µ�ƫ��1
	float m_distance1;
	float Distance1() const { return m_distance1; }
	void Distance1(float val) { m_distance1 = val; }

	//�µ�ƫ��2
	float m_distance2;
	float Distance2() const { return m_distance2; }
	void Distance2(float val) { m_distance2 = val; }

	//ԭ�ϳ���
	float m_width;
	float Width() const { return m_width; }
	void Width(float val) { m_width = val; }

	//ԭ�Ͽ��
	float m_height;
	float Height() const { return m_height; }
	void Height(float val) { m_height = val; }

	//�ޱ߾���
	float m_Deburringwidth;
	float Deburringwidth() const { return m_Deburringwidth; }
	void Deburringwidth(float val) { m_Deburringwidth = val; }

	//������϶
	float m_Kerfwidth;
	float Kerfwidth() const { return m_Kerfwidth; }
	void Kerfwidth(float val) { m_Kerfwidth = val; }

	//�յ�ƫ��
	CComboBox control_knife_shifting;
	int m_knife_shifting;
	int Knife_shifting() const { return m_knife_shifting; }
	void Knife_shifting(int val) { m_knife_shifting = val; }

	//�и��
	//float m_OtherKerfWidth;
	//float OtherKerfWidth() const { return m_OtherKerfWidth; }
	//void OtherKerfWidth(float val) { m_OtherKerfWidth = val; }

	CComboBox control_arranging_origin;
	int m_arranging_origin;
	int Arranging_origin() const { return m_arranging_origin; }
	void Arranging_origin(int val) { m_arranging_origin = val; }

	/************************************************************************/
	/* С���и�                                                             */
	/************************************************************************/
	//С����Ϣ ���ִ��ڳ�ʼ��
	void CParamDlg::OnInitDialog_SmallPanelInfo();
	//�������ݱ仯������
	void CParamDlg::updateToDialog_SmallPanelInfo();
	//С����Ϣ ���ݼ���
	void CParamDlg::loadSettings_SmallPanelInfo();
	//С����Ϣ ���ݼ���
	void CParamDlg::saveSettings_SmallPanelInfo();
	void CParamDlg::loadSettings_MinimentPanelParam();
	void CParamDlg::saveSettings_MinimentPanelParam();
	//С�����������ع���ʹ��
	void CParamDlg::OnBnClickedLdoublecheck();


	//С���������	
	CButton m_ldoublecut;
	BOOL m_ldoublecutEnable;
	BOOL LdoublecutEnable() const { return m_ldoublecutEnable; }
	void LdoublecutEnable(BOOL val) { m_ldoublecutEnable = val; }

	//С����С�ߴ�
	float m_minarea;//��С�������
	float Minarea() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_minarea;} }
	void Minarea(float val) { m_minarea = val; }

	//Ԥ�������
	float m_keepthickness;//Ԥ�������
	float Keepthickness() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_keepthickness;} }
	void Keepthickness(float val) { m_keepthickness = val; }

	//�ڶ����ӹ��ٶ�
	float m_secondspeedrate;//�ڶ����ӹ�����
	float Secondspeedrate() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_secondspeedrate;} }
	void Secondspeedrate(float val) { m_secondspeedrate = val; }


	//�������� - ��Ƚ���
	float m_twocut_threshold;
	float Twocut_threshold() const { return m_twocut_threshold; }
	void Twocut_threshold(float val) { m_twocut_threshold = val; }

	//�������� - Ԥ�����
	float m_twocut_reserve;
	float Twocut_reserve() const { return m_twocut_reserve; }
	void Twocut_reserve(float val) { m_twocut_reserve = val; }

	//�������� - �ٶ�
	float m_twocut_workSpeed;
	float Twocut_workSpeed() const { return m_twocut_workSpeed; }
	void Twocut_workSpeed(float val) { m_twocut_workSpeed = val; }

	//////////////////////////////////////////////////////////////////////////
	//����û�е�����

	//�����ٶ�
	float m_cutspeed;
	float Cutspeed() const { return m_cutspeed; }
	void Cutspeed(float val) { m_cutspeed = val; }

	//�µ��ٶ�
	float m_prospeed;
	float Prospeed() const { return m_prospeed; }
	void Prospeed(float val) { m_prospeed = val; }

	//��β�ٶ�
	float m_endspeech;
	float Endspeech() const { return m_endspeech; }
	void Endspeech(float val) { m_endspeech = val; }

	//�����ٶ�
	float m_overspeed;
	float Overspeed() const { return m_overspeed; }
	void Overspeed(float val) { m_overspeed = val; }
	
	//////////////////////////////////////////////////////////////////////////

	//��С������ - ��С����
	CString m_minw;
	CString Minw() const { return m_minw; }
	void Minw(CString val) { m_minw = val; }

	//��С������ - ��С���
	CString m_minh;
	CString Minh() const { return m_minh; }
	void Minh(CString val) { m_minh = val; }

	//��С������ - ��С���
	CString m_minsize;
	CString Minsize() const { return m_minsize; }
	void Minsize(CString val) { m_minsize = val; }


	//�����滻����EXCEL
	BOOL m_IsOutPutExcel;
	BOOL IsOutPutExcel() const { return m_IsOutPutExcel; }
	void IsOutPutExcel(BOOL val) { m_IsOutPutExcel = val; }

	/************************************************************************/
	/* �ӹ���Ϣ                                                             */
	/************************************************************************/
	//�ӹ���Ϣ ���ִ��ڳ�ʼ��
	void CParamDlg::OnInitDialog_JiaGongXinXi();
	//�������ݱ仯������
	void CParamDlg::updateToDialog_JiaGongXinXi();
	//�ӹ���Ϣ ���ݼ���
	void CParamDlg::loadSettings_JiaGongXinXi();
	//�ӹ���Ϣ ���ݼ���
	void CParamDlg::saveSettings_JiaGongXinXi();
	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	void CParamDlg::OnDblclkVecFilter();
	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	void CParamDlg::OnDblclkVecAdder();


	//�����
	BOOL m_isVecInput;
	BOOL VecInput() const { return m_isVecInput; }
	void VecInput(BOOL val) { m_isVecInput = val; }
	//�����
	BOOL m_isSoltInput;
	BOOL SoltInput() const { return m_isSoltInput; }
	void SoltInput(BOOL val) { m_isSoltInput = val; }
	//�����
	BOOL m_isDVecInput;
	BOOL DVecInput() const { return m_isDVecInput; }
	void DVecInput(BOOL val) { m_isDVecInput = val; }
	//�����
	BOOL m_isDSoltInput;
	BOOL DSoltInput() const { return m_isDSoltInput; }
	void DSoltInput(BOOL val) { m_isDSoltInput = val; }
	// �Ƿ�������
	BOOL m_isOthershapeInput;
	BOOL OthershapeInput() const { return m_isOthershapeInput; }
	void OthershapeInput(BOOL val) { m_isOthershapeInput = val; }
	// �Ƿ�ӹ�ͨ�׿�
	BOOL m_isReserveDeepHole;
	BOOL ReverseDeepHole() const { return m_isReserveDeepHole; }
	void ReverseDeepHole(BOOL val) { m_isReserveDeepHole = val; }
	// �Ƿ�ӹ�ͨ�ײ�
	BOOL m_isReserveDeepSlot;
	BOOL ReverseDeepSlot() const { return m_isReserveDeepSlot; }
	void ReverseDeepSlot(BOOL val) { m_isReserveDeepSlot = val; }
	//�и��
	int m_editnum;
	int Editnum() const { return m_editnum; }
	void Editnum(int val) { m_editnum = val; }
	// �ײ���λX
	float m_moveX;
	float MoveX() const { return m_moveX; }
	void MoveX(float val) { m_moveX = val; }
	// �ײ���λY
	float m_moveY;
	float MoveY() const { return m_moveY; }
	void MoveY(float val) { m_moveY = val; }
	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	std::vector<float> GetVecFilter();
	CString m_vecFilter;
	CString VecFilter() const { return m_vecFilter; }
	void VecFilter(CString val) { m_vecFilter = val; }
	CListBox m_listVecFilter;
	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	std::map<float,float> GetVecAdder();
	CString m_vecAdder;
	CString VecAdder() const { return m_vecAdder; }
	void VecAdder(CString val) { m_vecAdder = val; }
	CListBox m_listVecAdder;


	//��ת��Ϣ
	CComboBox control_comchange;
	int m_comchange;
	int Comchange() const { return m_comchange; }
	void Comchange(int val) { m_comchange = val; }


	//���г�Ʒ�������������һ���Ƿ���ʾ
	int m_outlineColomnShowable;
	int OutlineColomnShowable() const { return m_outlineColomnShowable; }
	void OutlineColomnShowable(int val) { m_outlineColomnShowable = val; }


	/************************************************************************/
	/* ������                                                               */
	/************************************************************************/

	CString ReadIniSetting(CIniFile& inifile, CString classify, CString colomn);
	void WriteIniSetting(CIniFile& inifile, CString classify, CString colomn, CString value);
	
	// ���һ��С�忿��ʱԤ�ޱ�
	BOOL m_CheckPreDeburring;




	// С�嵼��ʱ����Ϊ������
	BOOL m_CheckSmallCpnNoTexture;
//	CString m_valMinLen1;
	float m_valHMinLen1;
	float m_valVMinLen2;
	float m_valHMinWidth1;
	float m_valVMinWidth2;

	afx_msg void OnBnClickedCheckSmallCpnNoTexture();

	BOOL m_CheckRemainderLibrary;
};
