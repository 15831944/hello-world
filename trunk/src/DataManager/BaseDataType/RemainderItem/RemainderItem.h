/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderItem.h      -- �������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.9.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_RemainderItem_H
#define		_RemainderItem_H


class Component;


class AFX_EXT_CLASS RemainderItem
{
public:
	RemainderItem(void);
	virtual ~RemainderItem(void);


public:
	int m_GroupID;			// ������ID
	int m_ItemID;		// ����ID

	CString m_Material;		// ����
	float m_Thickness;		// ��
	float m_Length;			// ��		
	float m_Width;			// ��		
	int m_nCount;			// ����
	
};


#endif
