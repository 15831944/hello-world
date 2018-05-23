/*--------------------------------------------------------------------------------------------------------------------*/
///	CSingleon.h    --  �����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.8
//	��ע��	�㷨��Ҫ���ݽṹ
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_CSingleon_H_ 
#define		_CSingleon_H_

#include "../BaseInfo/BaseInfo.h"
#include "../CommonData/CommonData.h"
#include "../RemainderManager/RemainderManager.h"
#include "../RemainderGroup/RemainderGroup.h"



#include "../../../../include/TinyXml/tinyxml.h"

using namespace TinyXml;


#ifdef DEBUG 

#pragma comment(lib, "../../lib/tinyxmld.lib")
#pragma comment(lib, "../../lib/KnifeDlld.lib")
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/tinyxml.lib")
#pragma comment(lib, "../../lib/KnifeDll.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif



class KnifeClass;

class Panel;
class Component;

class AFX_EXT_CLASS CSingleon
{
public:
	~CSingleon();
	static CSingleon* GetSingleton(void);

private:
	CSingleon();
	static CSingleon* m_pSingleton;

public:
	// �ӿں���
	int Layout(int Method, int CutStyle, int Org);	// �����Ż�
	void ReOptimizeSln(CSolution* pSln, int Org);	// �������ʵ͵İ����´�ɢ����

	void ClearAllData(void);				// �����������
	void ClearCurrentSolution(void);		// �����ǰ�Ż�����
	void ClearBackupSolution(void);			// ��������Ż�����
	void ClearRemainderManager(void);		// ������Ϲ�����
	void SetBackupSolution(vector<CSolution*> &sln_list);		// ���ñ����Ż�����

	void UpdateSlnNum(void);						// ���½��������
	void UpdateComponentMachiningInfo(void);		// ���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�
	void UpdatePreCombinedComponent(void);          // ����Ԥ��ϰ������������������Ԥ��ϰ����һЩ�ֶ�
	
public:
	// ������ؽӿ�
	int GetCurSolutionPanelNum(void);		// ��ȡ��ǰ�Ż������������
	int GetBackupSolutionPanelNum(void);	// ��ȡ��ǰ�Ż������������
	void BackupBestSolution(void);			// �������Ž������


	int GetCurSolutionNum(void);			// ��ȡ��ǰ�Ż���������
	int GetBackupSolutionNum(void);			// ��ȡ��ǰ�Ż���������

public:
	// ��������С��ӿ�
	void SetBackupComponentInputItem(vector<ComponentInputItem>& InputItem);	// ��������С��ӿ�

public:
	// �Ż���������
	vector<CSolution*> m_CurrentSolutionList;				// ��ǰ�Ż�����
	vector<CSolution*> m_BackupSolutionList;				// �����Ż�����
	vector<vector<Component*>> m_vComponentGroup;			// С����
	BaseInfo m_BaseInfo;									// ������Ϣ
	KnifeClass* m_pKnifeClass;								// ����NC���õ���  ԭ�е�̻����
	vector<ComponentInputItem> m_vBackupComponentInputItem;	// ����С�屸��
	RemainderManager m_RemainderManager;					// ���Ϲ�����
	UINT m_uComponentInputItemSerialID;              // ���а�����кţ����ڱ�ʶÿһ�����а��

public:
	// ������������NC�йص����ݽṹ

	// �����ļ�
	TiXmlDocument m_KnifetypeDoc;




};
#endif