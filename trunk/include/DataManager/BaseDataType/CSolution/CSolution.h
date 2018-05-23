/*--------------------------------------------------------------------------------------------------------------------*
///	CSolution.h    --  ��������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	ͬ�ֲ��ʣ�ͬ�ֺ�ȵ�С�壬����n������
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_CSolution_H_ 
#define		_CSolution_H_

#include "../BaseInfo/BaseInfo.h"
#include "../Outline/Outline.h"
#include "../CommonData/CommonData.h"

#include <vector>
using namespace std;

class Panel;
class Component;

class AFX_EXT_CLASS CSolution
{
public:
	CSolution();
	virtual  ~CSolution();



public:
	// �Ż�����ؽӿ� 
	int GetPanelNum(void);				// ��ȡ�������
	Panel* GetPanel(int panel_num);		// ��ȡ���


public:
	// �����ӿ�
	//int LayoutOptimization(void);						// �����Ż�	������������ + ��������� + ��������и� 
	int LayoutOptimization_RandomSortCut(int CutStyle, int Org);			// �����Ż�	������� + ��������� + ��������и� 

public:
	int LayoutOptimization_MinWaste(int CutStyle, int Org);				// �����Ż�	����ʣ����С������������������
//	bool MatchSuitableComponentNRemaider(Panel* pPanel, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder);			// ƥ����ʵ�С���������
//	int BuildRemainderList(Panel* pParent);				// ��ʼ������������
//	int SortRemainderList_LengthFirst(void);			// ������򣬳������ȣ������֮



	int SortComponentList_LengthFirst(void);				// ������򣬳������ȣ������֮
	int SortComponentList_Random(void);						// ������򣬳������ȣ������֮



	void CheckComponentList(void);						// �������С�Ƿ񳬳���
	int BuildOutlineList(Panel* pParent);				// ��ʼ������������
	int PlaceOneComponent(Component* pCpn);				// �ڷ�һ��С��
	bool MatchSuitableComponentNOutline(int &nCpnID, int &nOutlineID);			// ƥ����ʵ�С���������
	bool MatchSuitableComponentNOutline2(int &nCpnID, int &nOutlineID);			// ƥ����ʵ�С���������

public:
	// ��������ڲ������Ż�
	void ReOptimizeEveryPanel(int Org);			// ��������ڲ������Ż�


	// �Դ���б����Ż��ʽ�������
	void SortPanelList(void);


public:
	int	m_SlnID;						// ���������					
	float m_fThickness;					// ������						
	CString m_strMaterial;				// �������						

	BaseInfo m_BaseInfo;				// ������Ϣ						

	vector<Panel*> m_PanelList;			// ����б� �Ż�����			
	vector<Component*> m_ComponentList;	// С���б� ԭʼ���


	// ����������㷨ʹ��
	vector<Outline>	m_OutlineList;			// ����������
	vector<Outline>	m_LowestOutlineList;	// �������������
};

#endif