/*--------------------------------------------------------------------------------------------------------------------*/
//	Panel.h     -- ����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Panel_H_
#define		_Panel_H_

#include "../Component/Component.h"

// �����Ϣ
class AFX_EXT_CLASS Panel: public Component
{
public:
	Panel(void);
	Panel(Panel& panel);
	virtual ~Panel(void);

public:
	// ���ӿں���
	Component* GetComponent(int NumInPanel);								// ��ȡС��
	int GetComponentNum(void);												// ��ȡС������
	float GetUtilization(void);												// ��ȡ���������
	void Recover(vector<Component*>& cpn_list);								// ����廹ԭΪ
	static void RecoverOneComponent(Component* pCpn);						// ��С�廹ԭ
	bool IsBetterThan(Panel* pDstPanel);									// �Ƿ����һ�����

public:
	// ����
	void GetAllNeededComponent_Sorted(vector<Component*>& list);			// ��ȡ�Ź����������Ҫ��С��
	void GetAllNeedComponent_AreaLargeFirst(vector<Component*>& cpn_list);	// ��������� ��ȡ���е�С��


public:
	// �麯��
	virtual bool HaveDownerFaceInfo(void);											// �Ƿ��з�����Ϣ

public:
	int m_PanelID;					// �����
	int m_PanelType;				// ����������ڷ���		ԭ�ϣ�0 ���ϣ�1	
	float m_OrgLen;					// ԭʼ��				// ����ת��һ��Ҫ��ԭʼ��������мǣ�
	float m_OrgWidth;				// ԭʼ��				// ����ת��һ��Ҫ��ԭʼ��������мǣ�

	float m_OriginX;				// ԭ��X
	float m_OriginY;				// ԭ��Y



	vector<CutPath> m_vCutPath;		//�����и��߶�
	vector<CutPath> m_vPreCutPath;	//Ԥ�и��߶�
};


typedef vector<Panel*>	PanelList;


#endif
