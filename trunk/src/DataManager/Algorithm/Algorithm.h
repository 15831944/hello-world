#pragma once
#include "../BaseDataType/CommonData/CommonData.h"
#include "../BaseDataType/BaseInfo/BaseInfo.h"
#include "../BaseDataType/Component/Component.h"
#include "../BaseDataType/Panel/Panel.h"
#include "../BaseDataType/Outline/Outline.h"


// �㷨�ӿ� �ṩ���ֲ����Ż��Ű桢�����Ż��Ű桢������Ż��Ű桢�����Ż��Ű�
class AFX_EXT_CLASS ALGORITHM_API
{
public:
	// ̰���㷨

	
	//	�ڴ�����Ż�����
	static int LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& base_info, vector<Component*>& SrcCpnList, int cut_style, int Org);

	// ��ʼ������������
	static int BuildRemainderList(Panel* pParent, vector<Component*>& RemainderList);

	// ƥ����ʵ�С�������
	static bool MatchSuitableComponentNRemaider(vector<Component*>& RemainderList, vector<Component*>& ComponentList, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder, BaseInfo& base_info);

	// �����Ͻ��дӴ�С����
	static int SortRemainderList_LengthFirst(vector<Component*>& RemainderList);

	// �Ƚϵ�һ���Ƿ�С�ڵڶ����
	static bool ComponentCompareSmaller(const Component* pfirst, const Component* psecond) ;

	// �иʽ
	static bool KnifeOneRemainder(Component* pRemainder, Component* pComponent, int cut_dir, float kerf_width, int Org);

public:
	// ����������㷨
	static int LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& base_info, vector<Component*>& SrcCpnList, int cut_style, int Org);

	// �����������������
	static int BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList);

	// ��С��������
	static bool CompareOutlineGreater(const Outline& first, const Outline& second) ;

	// ����������
	static int SortComponentList_Random(vector<Component*>& ComponentList);

	// ƥ����ʵ�С���������
	static bool MatchSuitableComponentNOutline2(vector<Component*>& ComponentList,vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList, int &nCpnID, int &nOutlineID);

public:
	// һЩ���ݲ���

	// ����С������
	static  void CopyComponentList(vector<Component*>& SrcList, vector<Component*>& DstList);

public:
	// һЩ���β���
	static void TravelNeedComponent_AreaLargeFirst(Component* pCpn, vector<Component*>& cpn_list);



public:
	// ��ѧ��ؼ��㺯��
	static float GetPointsDistance(PointInfo p1, PointInfo p2);									// ��������
	static vector<PointInfo> FiltrateCoincidentPoints(vector<PointInfo>& oldpoint);				// ��Ҫ���һ��ֱ�ߵ�
	static vector<PointInfo> FiltrateCoincidentPoints2(vector<PointInfo>& oldpoint);			// Ҫ���һ��ֱ�ߵ�
	static void rotatepoint(float x0,float y0,float & x,float & y,float arc);					// ��ת��
	static void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);	// ��ȡԲ�� �˺����д���������



	static void CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc);	//		��ʱ����ת
	static void ClockwiseRotate(float x0,float y0,float & x,float & y,float arc);		//		˳ʱ����ת
	static void NormalVector(float x0,float y0,float & x,float & y);					//		�����淶��
	static void GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);	// ����ȡԲ��,�Ľ���getCircleCenter�Ĵ���


};