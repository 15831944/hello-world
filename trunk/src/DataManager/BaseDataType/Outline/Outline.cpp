/*--------------------------------------------------------------------------------------------------------------------*
///	Outline.cpp   --  ��������ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.8
//	��ע��
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Outline.h"
#include "../CommonData/CommonData.h"
#include "../Component/Component.h"
#include "../Panel/Panel.h"

// ����
Outline::Outline()
{
	m_start_x = 0;
	m_end_x = 0;
	m_start_y = 0;
	m_end_y = 0;
	m_pParent = NULL;
}

// ����
Outline::~Outline()
{

}




// >����������,ˮƽ��y����,���ȴ�֮
bool Outline::operator> (const Outline& ln) const
{
	if (this->m_start_y < ln.m_start_y)			// y����
	{
		return true;
	}
	else if (this->m_start_y == ln.m_start_y)		// x��֮
	{
		float src_len = this->GetLength();
		float dst_len = ln.GetLength();

		if (src_len > dst_len)
		{
			return true;
		}
	}

	return false;
}



// <����������
bool Outline::operator< (const Outline& ln) const
{
	if (this->m_start_y < ln.m_start_y)			// y����
	{
		return true;
	}
	else if (this->m_start_y == ln.m_start_y)		// x��֮
	{
		float src_len = this->GetLength();
		float dst_len = ln.GetLength();

		if (src_len < dst_len)
		{
			return true;
		}
	}

	return false;
}

bool Outline::operator== (const Outline& ln) const		// ==����������
{
	if (this->m_start_x == ln.m_start_x 
		&& this->m_end_x == ln.m_end_x
		&& this->m_start_y == ln.m_start_y 
		&& this->m_end_y == ln.m_end_y)			// xy�����
	{
		return true;
	}

	return false;
}

// ��ȡ����
float Outline::GetLength(void) const
{
	return abs(m_end_x - m_start_x);
}

// ��ȡ���
float Outline::GetWidth(void) const
{
	return abs(m_end_y - m_start_y);
}

// �ܷ�����
bool Outline::Containable(Component* pCpn)
{
	float cur_len = GetLength();
	float cur_width = GetWidth();

	if (pCpn != NULL)
	{
		if (cur_len >= pCpn->m_RealLength && cur_width >= pCpn->m_RealWidth)
		{
			return true;
		}
	}

	return false;
}

// �������Ƿ�Ϸ�
bool Outline::IsLegal(void)
{
	if(m_start_x >= m_end_x)
		return false;

	if (m_start_y >= m_end_y)
		return false;

	return true;
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		����������
//	
//	param��
//		float start_x	--	���������x
//		float end_x		--	�������յ�x
//		float start_y	--	���������y
//		float end_y		--	�������յ�y
//		Panel* pPanel	--	���ָ�룬�����Ŀ���
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void Outline::SetOutline(float start_x, float end_x, float start_y, float end_y,  Component* pParent)
{
	m_start_x = start_x;
	m_end_x = end_x;
	m_start_y = start_y;
	m_end_y = end_y;
	m_pParent = pParent;
}

	
	