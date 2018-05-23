/*--------------------------------------------------------------------------------------------------------------------*/
//	Component.h      -- С���������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Component_H_
#define		_Component_H_

#include "../Node/Node.h"
#include "../CommonData/CommonData.h"
#include "../CSingleton/CSingleon.h"

// С����Ϣ
class AFX_EXT_CLASS Component : public Node
{
public:
	Component(void);
	virtual ~Component(void);


public:
	int			m_CpnID;					// ���ID	Ψһ��ʶ�� Ϊ�˽��С���ж������
	int			m_NumberInPanel;			// ������� �ڼ����ŵ�С��
	CString 	m_BarCode;					// ����
	CString     m_strCabinetName;			// ��������
	CString     m_strComponentName;			// �������
	CString     m_Material;					// ����
	int         m_Texture;					// ����	0��������  1������ 2������	
	float       m_Thickness;				// ���
	float       m_RealLength;				// ��ʵ��
	float       m_RealWidth;				// ��ʵ��
	float       m_x;						// x����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	float       m_y;						// y����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	float       m_fXLabelCenter;            // ��ǩ�е�x���� ���С��Ķ��㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	float       m_fYLabelCenter;            // ��ǩ�е�y���� ���С��Ķ��㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��

	int m_nRotatedAngle;                    // ��������ԭʼ������ת�˶��ٶȣ�˳ʱ��Ϊ������ʱ��Ϊ��
	int m_nOtherShapeID;					// ����ID
	vector<PointInfo> m_vOutlinePoint;		// ��������Ϣ
	int m_nKnifeDownPos;                    // �µ���
	vector<AllVec> m_vUpperFaceHole;        // �ϱ����
	vector<AllSlot> m_vUpperFaceSlot;       // �ϱ����
	vector<AllVec> m_vDownerFaceHole;       // �±����
	vector<AllSlot> m_vDownerFaceSlot;      // �±����


	CString m_strOrderID;               //������
	CString m_strCabinetID;             //���
	float m_afBanding[4];               //���
	bool m_bSlotFlipped;                //�۷�ת
	bool m_bVHoleFlipped;                //�׷�ת
	CString m_strCustomerInfo;          //�ͻ���Ϣ
	CString m_strJoinedStore;           //���˵�
	CString m_strSlottingFlag;          //���۱�ʶ
	CString m_strReminder;              //���Ǻ�
	CString m_strDrilling;              //���
	CString m_strOrderType;             //��������
	CString m_strReverseSideBarcode;    //��������
	float m_fProductLength;             //��Ʒ����
	float m_fProductWidth;              //��Ʒ���
	float m_fProductThickness;          //��Ʒ���
	CString m_strCustomerAddress;       //�ͻ���ַ
	CString m_strHoleSlotFlag;          //��۱�ʶ
	CString m_strPanelClass;            //�������,"Door"Ϊ�Ű壬"Panel"Ϊ��ͨ��
	CString m_strSendingDate;           //��������

	//float m_fDist2PanelCenter;			// ���������ĵľ���
	//float m_fCenterX;					// �������X
	//float m_fCenterY;					// �������Y



public:
	// �ڵ���ؽӿ�
	virtual Node* CopyNode(void);										// �����ڵ�
	virtual bool Containable(Component* pCpn);							// �ܷ�����ڵ�
	virtual void GetAllRemainder(vector<Component*>& list);				// ��ȡ���е�����
	virtual void GetAllRemainder(vector<Component*>& list, float area);	// ��ȡ�����������area������
	virtual void GetAllNeededComponent(vector<Component*>& list);		// ��ȡ������Ҫ��С��
		
	virtual Component* GetLargestRemainder(void);						// ��ȡ��������
	virtual Component* GetLargestNeedComponent(void);					// ��ȡ����С��




public:
	// ����������
	bool operator> (const Component& dst_cpn) const;		// >����������
	bool operator< (const Component& dst_cpn) const;		// <����������
	bool operator== (const Component& dst_cpn) const;		// ==����������

	// �Ƚ���ؽӿ�
	bool IsAreaLargeThan(const Component& dst_cpn) const;

	static bool  ComponentCompareAreaLess(const Component* pfirst, const Component* psecond) ;


public:
	// �����ӿ�
	bool IsLegal(void);						// �Ƿ�Ϸ�
	bool IsRotatable(void);					// �ܷ���ת
	void ClockwiseRotate90(void);			// ˳ʱ����ת90��
	void CouterClockwiseRotate90(void);		// ��ʱ����ת90��

	float GetArea(void);					// ��ȡ������
	float GetAreaContainKerf(void);			// ��ȡ�������������

	void CalRectOutlinePoint(void);			// �������������

public:
	// �麯��
	virtual bool HaveDownerFaceHole(void);			// �Ƿ��з����
	virtual bool HaveDownerFaceSlot(void);			// �Ƿ��з����
	virtual bool HaveDownerFaceInfo(void);			// �Ƿ��з�����Ϣ

};

typedef vector<Component*> ComponentList;

#endif
