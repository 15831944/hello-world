#include "stdafx.h"
#include "BaseInfo.h"

#if 1

BaseInfo::BaseInfo(void)
{

	m_SawKerfWidth						= 6;		// ��� ��������
	m_PanelLength						= 2440;		// ��峤��		
	m_PanelWidth						= 1220;		// �����		
	m_DeburringWidth					= 3;		// �ޱ߾��� 4�߶���		

	m_UpHeight							= 45;		// ��ȫ�߶�
	m_DownHeight						= 0.2;		// �µ����
	m_distance1							= 30;		// �µ�ƫ��1
	m_distance2							= 20;		// �µ�ƫ��2
	m_MinLength							= 300;		// ��С������ - ��С����
	m_MinWidth							= 300;		// ��С������ - ��С���
	m_MinArea							= 90000;	// ��С������ - ��С���

	m_FirstSectionOPTimes				= 1;		// ��һ�׶��Ż�����
	m_FirstSectionOPMethod				= 0;		// ��һ�׶��Ż�����
	m_SecondSectionOPTimes				= 0;		// �ڶ��׶��Ż�����
	m_ThirdSectionOPTimes				= 0;		// �����׶��Ż�����
	m_ThirdSectionOAccptableUtilization	= 0;		// �����׿ɽ����Ż���
	m_LayoutOrg							= 0;		// ����ԭ��
	m_bDownerFaceFirst					= true;	// ������Ϣ��������		
}


BaseInfo::~BaseInfo(void)
{

}

#endif