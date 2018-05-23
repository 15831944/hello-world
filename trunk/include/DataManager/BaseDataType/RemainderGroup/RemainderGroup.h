/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderGroup.h      -- �������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.9.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_RemainderGroup_H
#define		_RemainderGroup_H

#include <vector>
#include "../RemainderItem/RemainderItem.h"

using namespace std;


class AFX_EXT_CLASS RemainderGroup
{
public:
	RemainderGroup(void);
	virtual ~RemainderGroup(void);

public:
	// �����ӿ�

	void AddRemainder(RemainderItem* pAddItem);
	void DelRemainder(int id);
	void UpdateRemainderItemID();


public:
	int m_GroupID;
	float	m_Thickness;							//	��
	CString m_Material;								//	����
	vector<RemainderItem*>	m_RemainderItemList;	//	�������б�




};


#endif
