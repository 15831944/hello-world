/*--------------------------------------------------------------------------------------------------------------------*/
//	Component.cpp       -- С����ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Component.h"
#include "../CommonData/CommonData.h"
#include <algorithm>

using namespace std;

enum
{
	Length_First,
	Width_First,
	Area_First,
	Special_1
};


#define  Component_Sort_Priority		Length_First

Component::Component(void)
{
	m_CpnID = 0;				
	m_NumberInPanel = 0;	
	m_BarCode = "";			
	m_strCabinetName = "";	
	m_strComponentName = "";	
	m_Material = "";			
	m_Texture = TextureType_NO_TEXTURE;			
	m_Thickness = 0;		
	m_RealLength = 0;		
	m_RealWidth = 0;		
	m_x = 0;				
	m_y = 0;
	m_fXLabelCenter = 0;
	m_fYLabelCenter = 0;

	m_type = NodeType_Remainder;

	//m_bRotated = false;
	m_nRotatedAngle = 0;
	m_nOtherShapeID = 0;
	m_nKnifeDownPos = 0;

// 	m_fDist2PanelCenter = 0;
// 	m_fCenterX = 0;
// 	m_fCenterY = 0;
}

Component::~Component(void)
{

}

// �����ڵ�
Node* Component::CopyNode(void)
{
	Component* pCpn = new Component;

	// Node��Ϣ
	pCpn->m_type					= this->m_type;								// 0�����ϣ��ӽڵ㣩  1��Ҫ����С�壨�ӽڵ㣩   2����ϰ壨���ڵ㣩

	// Component��Ϣ
	pCpn->m_CpnID					= this->m_CpnID				;				// ���ID		Ψһ��ʶ�� Ϊ�˽��С���ж������
	pCpn->m_NumberInPanel			= this->m_NumberInPanel		;				// �������		�ڼ����ŵ�С��
	pCpn->m_BarCode					= this->m_BarCode			;				// ����
	pCpn->m_strCabinetName			= this->m_strCabinetName	;				// ��������
	pCpn->m_strComponentName		= this->m_strComponentName	;				// �������
	pCpn->m_Material				= this->m_Material			;				// ����
	pCpn->m_Texture					= this->m_Texture			;				// ����			0��������  1������ 2������	
	pCpn->m_Thickness				= this->m_Thickness			;				// ���
	pCpn->m_RealLength				= this->m_RealLength		;				// ��ʵ��
	pCpn->m_RealWidth				= this->m_RealWidth			;				// ��ʵ��
	pCpn->m_x						= this->m_x					;				// x����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	pCpn->m_y						= this->m_y					;				// y����		��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	pCpn->m_fXLabelCenter           = this->m_fXLabelCenter;                    // ��ǩ�е�x���� ��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��
	pCpn->m_fYLabelCenter           = this->m_fYLabelCenter;                    // ��ǩ�е�y���� ��Դ���ԭ�㣬ֱ������ϵ����һ���ޣ����½�Ϊԭ��

	pCpn->m_nRotatedAngle			= this->m_nRotatedAngle		;				// ��ת�Ƕ�
	pCpn->m_nOtherShapeID			= this->m_nOtherShapeID		;				// ����ID
	pCpn->m_vOutlinePoint			= this->m_vOutlinePoint		;				// ��������Ϣ
	pCpn->m_nKnifeDownPos			= this->m_nKnifeDownPos		;				// �µ���
	pCpn->m_vUpperFaceHole			= this->m_vUpperFaceHole	;				// �ϱ����
	pCpn->m_vUpperFaceSlot			= this->m_vUpperFaceSlot	;				// �ϱ����
	pCpn->m_vDownerFaceHole			= this->m_vDownerFaceHole	;				// �±����
	pCpn->m_vDownerFaceSlot			= this->m_vDownerFaceSlot	;				// �±����


	pCpn->m_strOrderID				= this->m_strOrderID			;           // ������
	pCpn->m_strCabinetID			= this->m_strCabinetID			;			// ���

	//pCpn->m_afBanding[4]			= this->m_afBanding[4]			;           // ���
	memcpy(pCpn->m_afBanding, this->m_afBanding, sizeof(this->m_afBanding));	   

	pCpn->m_bSlotFlipped			= this->m_bSlotFlipped			;           // �۷�ת
	pCpn->m_bVHoleFlipped			= this->m_bVHoleFlipped			;           // �׷�ת
	pCpn->m_strCustomerInfo			= this->m_strCustomerInfo		;			// �ͻ���Ϣ
	pCpn->m_strJoinedStore			= this->m_strJoinedStore		;			// ���˵�
	pCpn->m_strSlottingFlag			= this->m_strSlottingFlag		;			// ���۱�ʶ
	pCpn->m_strReminder				= this->m_strReminder			;           // ���Ǻ�
	pCpn->m_strDrilling				= this->m_strDrilling			;           // ���
	pCpn->m_strOrderType			= this->m_strOrderType			;			// ��������
	pCpn->m_strReverseSideBarcode	= this->m_strReverseSideBarcode	;			// ��������
	pCpn->m_fProductLength			= this->m_fProductLength		;			// ��Ʒ����
	pCpn->m_fProductWidth			= this->m_fProductWidth			;			// ��Ʒ���
	pCpn->m_fProductThickness		= this->m_fProductThickness		;			// ��Ʒ���
	pCpn->m_strCustomerAddress		= this->m_strCustomerAddress	;			// �ͻ���ַ
	pCpn->m_strHoleSlotFlag			= this->m_strHoleSlotFlag		;			// ��۱�ʶ




	return pCpn;
}


// �ܷ�����ڵ�
bool Component::Containable(Component* pCpn)
{
	float cur_len = m_RealLength;
	float cur_width = m_RealWidth;

	if (pCpn != NULL)
	{
		if (cur_len >= pCpn->m_RealLength && cur_width >= pCpn->m_RealWidth)
		{
			return true;
		}
	}

	return false;
}

// ��ȡ���е�����
 void Component::GetAllRemainder(vector<Component*>& list)
 {
	 vector<Node*> tmp_list;
	 GetAllLeafNodes(tmp_list, NodeType_Remainder);	

	 int nCount = tmp_list.size();

	 for(int i = 0; i < nCount; i++)
	 {
		 Node* pNode = tmp_list.at(i);
		 Component* pCpn = dynamic_cast<Component*>(pNode);

		 list.push_back(pCpn);
	 }

 }



 // ��ȡ�����������area������
 void Component::GetAllRemainder(vector<Component*>& list, float area)
 {
	 vector<Node*> tmp_list;
	 GetAllLeafNodes(tmp_list, NodeType_Remainder);	

	 int nCount = tmp_list.size();

	 for(int i = 0; i < nCount; i++)
	 {
		 Node* pNode = tmp_list.at(i);
		 Component* pCpn = dynamic_cast<Component*>(pNode);

		 float cur_area = pCpn->m_RealLength*pCpn->m_RealWidth;
		 if (cur_area > area)
		 {
			 list.push_back(pCpn);
		 }

	 }
 }


 // ��ȡ������Ҫ��С��
void Component::GetAllNeededComponent(vector<Component*>& list)
{
	vector<Node*> tmp_list;
	GetAllLeafNodes(tmp_list, NodeType_NeededComponent);	

	int nCount = tmp_list.size();

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = tmp_list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		list.push_back(pCpn);
	}
}

// ����Ӵ�С����
bool  Component::ComponentCompareAreaLess(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return p1->IsAreaLargeThan(*p2);	
}


// ��ȡ��������
Component* Component::GetLargestRemainder(void)
{
	vector<Component*> list;

	GetAllRemainder(list);

	if (list.size() == 0)
		return NULL;

	// �Ӵ�С����
	sort(list.begin(), list.end(), ComponentCompareAreaLess);

	return list.front();
}

// ��ȡ����С��
Component* Component::GetLargestNeedComponent(void)				
{
	vector<Component*> list;

	GetAllNeededComponent(list);

	if (list.size() == 0)
		return NULL;

	// �Ӵ�С����
	sort(list.begin(), list.end(), ComponentCompareAreaLess);

	return list.front();
}




// �������� �����֮
bool Component::operator> (const Component& dst_cpn) const
{
#if (Component_Sort_Priority == Length_First)

	if (this->m_RealLength > dst_cpn.m_RealLength)			// ��������
	{
		return true;
	}
	else if (this->m_RealLength == dst_cpn.m_RealLength)		// �����֮
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area > dst_area)
		{
			return true;
		}
	}

#elif	(Component_Sort_Priority == Width_First)

	if (this->m_RealWidth > dst_cpn.m_RealWidth)			// ��������
	{
		return true;
	}
	else if (this->m_RealWidth == dst_cpn.m_RealWidth)		// �����֮
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area > dst_area)
		{
			return true;
		}
	}
#elif	(Component_Sort_Priority == Area_First)
	float src_area = this->m_RealLength * this->m_RealWidth;
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;
	
	if (src_area > dst_area)
	{
		return true;
	}
	else if (src_area == dst_area)		// �����֮
	{
		if (this->m_RealWidth > dst_cpn.m_RealWidth)			// ��������
		{
			return true;
		}
	}

#elif	(Component_Sort_Priority == Special_1)

	float src_area = this->m_RealLength * this->m_RealWidth +��2 * (this->m_RealLength + this->m_RealWidth);
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth +��2 * (dst_cpn.m_RealLength + dst_cpn.m_RealWidth);
	
	if (src_area > dst_area)
	{
		return true;
	}


#endif

	return false;
}

// <����������
bool Component::operator< (const Component& dst_cpn) const
{
	if (this->m_RealLength < dst_cpn.m_RealLength)			// ��������
	{
		return true;
	}
	else if (this->m_RealLength == dst_cpn.m_RealLength)		// �����֮
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area < dst_area)
		{
			return true;
		}
	}

	return false;
}

bool Component::operator== (const Component& dst_cpn) const		// ==����������
{
	if (this->m_RealLength == dst_cpn.m_RealLength && this->m_RealWidth == dst_cpn.m_RealWidth)			// ��������
	{
		return true;
	}

	return false;
}



bool Component::IsAreaLargeThan(const Component& dst_cpn) const
{
	float src_area = this->m_RealLength * this->m_RealWidth;
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

	if (src_area > dst_area)
	{
		return true;
	}

	return false;
}











// ��תС��
void Component::ClockwiseRotate90(void)
{
	float org_len = m_RealLength;
	float org_width = m_RealWidth;

	m_RealLength = org_width;
	m_RealWidth	= org_len ;

	//m_bRotated = !m_bRotated;
	m_nRotatedAngle -= 90;                     //˳ʱ��Ϊ��
	m_nRotatedAngle = m_nRotatedAngle % 360;
}

// ��ʱ����ת90��
void Component::CouterClockwiseRotate90(void)
{
	if(IsRotatable() == true)
	{
		float org_len = m_RealLength;
		float org_width = m_RealWidth;

		m_RealLength = org_width;
		m_RealWidth	= org_len ;

		//m_bRotated = !m_bRotated;
		m_nRotatedAngle += 90;              //��ʱ��Ϊ��
		m_nRotatedAngle = m_nRotatedAngle % 360;
	}

}


// �ܷ���ת
bool Component::IsRotatable(void)
{
	if(m_Texture == TextureType_NO_TEXTURE)
		return true;
	else
		return false;
}


// �Ƿ�Ϸ�
bool Component::IsLegal(void)
{
	if (m_RealLength > 0 && m_RealWidth > 0)
	{
		return true;
	}

	return false;
}


// ��ȡ������
float Component::GetArea(void)
{
	float area = m_RealLength * m_RealWidth;

	if (area > 0)
		return area;
	else
		return 0;
}

// ��ȡ�������������
float  Component::GetAreaContainKerf(void)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	float half_kerf = pSingleton->m_BaseInfo.m_SawKerfWidth/2;
	float area = (m_RealLength+half_kerf) * (m_RealWidth+half_kerf);

	if (area > 0)
		return area;
	else
		return 0;
}

/*---------------------------------------*/
//	����˵����
//	�������������
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void Component::CalRectOutlinePoint(void)
{
	m_vOutlinePoint.clear();

	PointInfo p1;

	p1.x	= 0;
	p1.y	= m_RealWidth;
	p1.r	= 0;
	p1.sign	= 0;
	p1.dir	= 0;;
	p1.cut	= 0;;
	p1.side	= 0;;
	p1.type	= 0;;
	p1.group = 1;

	PointInfo p2;

	p2.x = 0;
	p2.y = 0;
	p2.r	= 0;
	p2.sign	= 0;
	p2.dir	= 0;;
	p2.cut	= 0;;
	p2.side	= 0;;
	p2.type	= 0;;
	p2.group = 0;


	PointInfo p3;

	p3.x = m_RealLength;
	p3.y = 0;
	p3.r	= 0;
	p3.sign	= 0;
	p3.dir	= 0;;
	p3.cut	= 0;;
	p3.side	= 0;;
	p3.type	= 0;;
	p3.group = 0;

	PointInfo p4;

	p4.x = m_RealLength;
	p4.y = m_RealWidth;
	p4.r	= 0;
	p4.sign	= 0;
	p4.dir	= 0;;
	p4.cut	= 0;;
	p4.side	= 0;;
	p4.type	= 0;;
	p4.group = 0;

	PointInfo p5;

	p5.x	= 0;
	p5.y	= m_RealWidth;
	p5.r	= 0;
	p5.sign	= 0;
	p5.dir	= 0;;
	p5.cut	= 0;;
	p5.side	= 0;;
	p5.type	= 0;;
	p5.group = 2;

	m_vOutlinePoint.push_back(p1);
	m_vOutlinePoint.push_back(p2);
	m_vOutlinePoint.push_back(p3);
	m_vOutlinePoint.push_back(p4);
	m_vOutlinePoint.push_back(p5);
}



/*---------------------------------------*/
//	����˵����
//		�Ƿ��п�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool Component::HaveDownerFaceHole(void)
{
	if (m_vDownerFaceHole.size() > 0)
		return true;

	return false;
}



/*---------------------------------------*/
//	����˵����
//		�Ƿ��в�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool  Component::HaveDownerFaceSlot(void)
{

	if (m_vDownerFaceSlot.size() > 0)
		return true;

	return false;
}

/*---------------------------------------*/
//	����˵����
//		�Ƿ��з�����Ϣ
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool  Component::HaveDownerFaceInfo(void)
{
	if (m_vDownerFaceHole.size() > 0 || m_vDownerFaceSlot.size() > 0)
		return true;

	return false;
}