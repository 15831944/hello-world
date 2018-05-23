#ifndef		_ClientInfoReadWrite_h_
#define		_ClientInfoReadWrite_h_

#pragma  once

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"




class AFX_EXT_CLASS  ClientInfoReadWrite
{

public:
	static BOOL IsClientInfoFileExist();			// �ͻ���Ϣ�ļ��Ƿ����
	static BOOL IsClientRegistered();				// �ͻ��Ƿ�ע��
	static void ReadClientInfo();					// ��ȡ�ͻ���Ϣ
	static void WriteClientInfo();					// д��ͻ���Ϣ
	static void SetFileClientInfoPath();			// ���ÿͻ���Ϣ�ļ�·��
	static void ResetClientInfo(void);				// ���ÿͻ���Ϣ
	static void ClearClientInfo();					// ��տͻ���Ϣ�ļ�

public:
	static CString		m_ClientInfoFileName;
	static ClientInfo	m_Info;

};


#endif