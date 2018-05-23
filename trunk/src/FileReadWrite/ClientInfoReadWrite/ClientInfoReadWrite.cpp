#include "stdafx.h"
#include "ClientInfoReadWrite.h"
#include "../Misc/EncryptionInterface.h"
#include "HardwareInfo.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"


CString ClientInfoReadWrite::m_ClientInfoFileName;
ClientInfo	ClientInfoReadWrite::m_Info;

// ��տͻ���Ϣ�ļ�
void ClientInfoReadWrite::ClearClientInfo()
{
	m_Info.m_ClientName.Empty();
	m_Info.m_PhoneNumber.Empty();
	m_Info.m_QQNumber.Empty();
	m_Info.m_Address.Empty();
	m_Info.m_MacAdress.Empty();
	m_Info.m_BaseBoardSerialNo.Empty();
	m_Info.m_CPUSerialNo.Empty();
	m_Info.m_HDSerialNo.Empty();

	m_Info.m_OutputNCTimes = 0;
	m_Info.m_RegisterTime = 0;	
}


/*---------------------------------------*/
//	����˵����
//		���ÿͻ���Ϣ�ļ�·��
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
void ClientInfoReadWrite::SetFileClientInfoPath()
{
	char ExeFullFilePath[2000];
	GetModuleFileName(NULL, ExeFullFilePath, 500);	// ��ȡӦ�ó�������·����
	CString exe_path = ExeFullFilePath;
	exe_path = exe_path.Left(exe_path.ReverseFind('\\') + 1);	// �����·ָ���"\"

	CString strTmp;
	strTmp.Format(_T("ClientInfo%s"), g_szSuffix);
	m_ClientInfoFileName = exe_path + strTmp;


}



/*---------------------------------------*/
//	����˵����
//		�ͻ���Ϣ�ļ��Ƿ����
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
BOOL ClientInfoReadWrite::IsClientInfoFileExist()
{

	SetFileClientInfoPath();

	// �жϿͻ���Ϣ�ļ��Ƿ����
	if (PathFileExists(m_ClientInfoFileName) == FALSE)
	{
		return FALSE;
	}
	else
		return TRUE;
}

#define		DebugRegister	0


/*---------------------------------------*/
//	����˵����
//		�ͻ��Ƿ�ע��
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
BOOL ClientInfoReadWrite::IsClientRegistered()
{
	CString CurMacAddress,	FileMacAddress;
	CString CurBaseboard,	FileBaseboard;
	CString CurCpu,			FileCpu;
	CString CurHD,			FileHD;
	
	SetFileClientInfoPath();

	// ��ȡ��ǰ��������Ϣ
	GetMacAdress(CurMacAddress);
	GetBaseboard(CurBaseboard);
	GetCpu(CurCpu);
	GetHDSerial(CurHD);

#if (DebugRegister == 1)
	CString rpt_msg;

	rpt_msg.Format("��ȡMac��ַ��" + CurMacAddress + "\n" + "��ȡ�������кţ�" + CurBaseboard  + "\n" + "��ȡӲ�����кţ�" + CurHD  + "\n" + "��ȡCPU���кţ�" + CurCpu);

	AfxMessageBox(rpt_msg);

#endif

	
	// ��ȡ�ͻ���Ϣ
	ReadClientInfo();

#if (DebugRegister == 1)
	

	rpt_msg.Format("�ļ�Mac��ַ��" + m_Info.m_MacAdress + "\n" + "�ļ��������кţ�" + m_Info.m_BaseBoardSerialNo  + "\n" + "�ļ�Ӳ�����кţ�" + m_Info.m_HDSerialNo  + "\n" + "�ļ�CPU���кţ�" + m_Info.m_CPUSerialNo);

	AfxMessageBox(rpt_msg);

#endif


	// �Ա�MAC��ַ
	FileMacAddress = m_Info.m_MacAdress;

	// MAC��ַ����ͬ������ע��
	int FileMacAddressLen = FileMacAddress.GetLength();
	int CurMacAddressLen = CurMacAddress.GetLength();

	// 2018-2-28 ���ֵ��Թ����ֻ�����ע��ʱ����ȡ����mac��ַʧ�ܣ��������ܻ�ȡ������mac��ַ��ƥ��
	if (CurMacAddressLen != 0 && FileMacAddressLen != 0)
	{
		if (CurMacAddress.Compare(FileMacAddress) != 0)
		{
			// ��ʾע��ԭ��
		//	AfxMessageBox(CString("�ļ�Mac��ַ��") +  FileMacAddress + CString("����ǰMac��ַ��") + CurMacAddress +  "��Mac��ַ��ƥ�䣬��Ҫע�ᣡ");
		//	AfxMessageBox(CString("�ļ���Ϣ1��") +  FileMacAddress + CString("����ǰ��Ϣ1��") + CurMacAddress +  "����Ϣ1��ƥ�䣬��Ҫע�ᣡ");
			AfxMessageBox("��Ϣ1��ƥ�䣬��Ҫע�ᣡ");


			return FALSE;
		}
	}

	// �Ա��������к�
	FileBaseboard = m_Info.m_BaseBoardSerialNo;
	int FileBaseboardLen = FileBaseboard.GetLength();
	int CurBaseboardLen = CurBaseboard.GetLength();

	if (CurBaseboardLen != 0)
	{
		if (CurBaseboard.Compare(FileBaseboard) != 0)
		{
			// ��ʾע��ԭ��
			//AfxMessageBox(CString("�ļ��������кţ�") +  FileBaseboard + CString("����ǰ�������кţ�") + CurBaseboard +  "���������кŲ�ƥ�䣬��Ҫע�ᣡ");
			//AfxMessageBox(CString("�ļ���Ϣ2��") +  FileBaseboard + CString("����ǰ��Ϣ2��") + CurBaseboard +  "����Ϣ2��ƥ�䣬��Ҫע�ᣡ");
			AfxMessageBox("��Ϣ2��ƥ�䣬��Ҫע�ᣡ");

			return FALSE;
		}
	}


	// Ӳ�����к� 
	// ���ִ˷�������©�������ֻ��������Ӳ�������кŻ�ı�����
// 	FileHD = m_Info.m_HDSerialNo;
// 	int FileHDLen = FileHD.GetLength();
// 	int CurHDLen = CurHD.GetLength();
// 
// 	if (CurHDLen != 0)
// 	{
// 		if (CurHD.Compare(FileHD) != 0)
// 		{
// 			// ��ʾע��ԭ��
// 			AfxMessageBox(CString("�ļ�Ӳ�����кţ�") +  FileHD + CString("����ǰӲ�����кţ�") + CurHD +  "��Ӳ�����кŲ�ƥ�䣬��Ҫע�ᣡ");
// 
// 
// 			return FALSE;
// 		}
// 	}


	// CPU���к�
	FileCpu = m_Info.m_CPUSerialNo;
	int FileCpuLen = FileCpu.GetLength();
	int CurCpuLen = CurCpu.GetLength();

	if (CurCpuLen != 0)
	{
		if (CurCpu.Compare(FileCpu) != 0)
		{
			// ��ʾע��ԭ��
			//AfxMessageBox(CString("�ļ�CPU���кţ�") +  FileCpu + CString("����ǰCPU���кţ�") + CurCpu +  "��CPU���кŲ�ƥ�䣬��Ҫע�ᣡ");
			//AfxMessageBox(CString("�ļ���Ϣ3��") +  FileCpu + CString("����ǰ��Ϣ3��") + CurCpu +  "����Ϣ3��ƥ�䣬��Ҫע�ᣡ");
			AfxMessageBox("��Ϣ3��ƥ�䣬��Ҫע�ᣡ");


			return FALSE;
		}
	}


	// ���ʱ�䳬��3���£�ע�ᣬ��¼��ǰʱ�� 3*30*24*60*60 = 7776000��
	CTime last_time(m_Info.m_RegisterTime);
	CTime cur_time = CTime::GetCurrentTime();
	CTimeSpan  timeSpan = cur_time - last_time;
	long days = timeSpan.GetDays();

	if (days > 90 || days < 0)
	{
		// ��ʾע��ԭ��
		CString msg_info;

		//msg_info.Format("ʱ������%d,ʱ�䳬��90�죬��Ҫע�ᣡ" , days);
		msg_info = "��Ϣ4����Ҫע�ᣡ";
		AfxMessageBox(msg_info);

		return FALSE;
	}

	// �������NC�ļ���������800�Σ�ע�ᣬ����
	if (m_Info.m_OutputNCTimes > 800)
	{
		// ��ʾע��ԭ��
		CString msg_info;

		//msg_info.Format("��ǰ����������%d,����NC�ļ���������800��,��Ҫע�ᣡ" , m_Info.m_OutputNCTimes);
		msg_info = "��Ϣ5,��Ҫע�ᣡ" ;
		AfxMessageBox(msg_info);

		return FALSE;
	}

	return TRUE;
}

/*---------------------------------------*/
//	����˵����
//		���ÿͻ���Ϣ
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
void ClientInfoReadWrite::ResetClientInfo(void)
{

	CString CurMacAddress;

	GetMacAdress(CurMacAddress);


	// ͳһ����ڴ�
	ClearClientInfo();


	// ����mע���ac��ַ

	//Mac��ַ
	GetMacAdress(m_Info.m_MacAdress);

	// �������к�
	char lpszBaseBoard[128] = {0};  
	GetBaseBoardByCmd(lpszBaseBoard);
	m_Info.m_BaseBoardSerialNo = lpszBaseBoard;

	// Ӳ�����к�
	char lpszHD[128] = {0};  
	GetHDSerialByCmd(lpszHD); 
	m_Info.m_HDSerialNo = lpszHD;

	// CPU���к�
	char lpszCPU[128] = {0};  
	GetCpuByCmd(lpszCPU);
 	m_Info.m_CPUSerialNo = lpszCPU;


	// ����ע��ʱ��
	CTime time = CTime::GetCurrentTime();
	m_Info.m_RegisterTime = (long)(time.GetTime());

	// ���浼������
	m_Info.m_OutputNCTimes = 1;
}


/*---------------------------------------*/
//	����˵����
//		��ȡ�ͻ���Ϣ
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
void ClientInfoReadWrite::ReadClientInfo()
{
	CFile file;

	SetFileClientInfoPath();

	// ͳһ����ڴ�
	ClearClientInfo();

	// ����

	//�����ļ���ȡ��׼
	CString HgePath = m_ClientInfoFileName;
	CString XmlPath = m_ClientInfoFileName.Left(m_ClientInfoFileName.GetLength()-4) + ".xml";

	// �µļӽ��ܷ�ʽ
	decrypt_base64(HgePath.GetBuffer(), XmlPath.GetBuffer());

	// �����ļ�
	TiXmlDocument* m_xmlDoc = new TiXmlDocument();
	m_xmlDoc->LoadFile(XmlPath);

	TiXmlElement* pRoot = m_xmlDoc->RootElement();

	// �ж�
	if (pRoot == NULL)
	{
		//AfxMessageBox("ע���ļ�������ɾ��ClientInfo.hge�ļ�������ע�ᣡ");
		return ;
	}

	string strValue =  pRoot->Value();
	if (strValue != "ע����Ϣ")
	{
		//AfxMessageBox("ע���ļ�������ɾ��ClientInfo.hge�ļ�������ע�ᣡ");
		return ;
	}

	// ��ȡ�ļ���Ϣ
	for (TiXmlElement * pCurNode = pRoot->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
	{
		string strCurNodeName =  pCurNode->Value();

		if (strCurNodeName == "�û���Ϣ")
		{

			m_Info.m_ClientName		= pCurNode->Attribute("Name"			);		// ����
			m_Info.m_PhoneNumber	= pCurNode->Attribute("Phone"			);		// �ֻ�
			m_Info.m_QQNumber		= pCurNode->Attribute("QQ"				);		// qq
			m_Info.m_Address		= pCurNode->Attribute("Address"			);		// ��ַ
			m_Info.m_RegisterTime	= _ttoi(pCurNode->Attribute("RegisterTime"	));		// ע��ʱ��
			m_Info.m_OutputNCTimes	= _ttoi(pCurNode->Attribute("OutputNcTimes"));		// ����NC�ļ�����

		}
		else if (strCurNodeName == "������Ϣ")
		{

			m_Info.m_MacAdress			= pCurNode->Attribute("MacAddress"			);	// MAC��ַ
			m_Info.m_BaseBoardSerialNo	= pCurNode->Attribute("BaseBoardSerialNo"	);	// �������к�
			m_Info.m_CPUSerialNo		= pCurNode->Attribute("HDSerialNo"			);	// Ӳ�����к�
			m_Info.m_HDSerialNo			= pCurNode->Attribute("CPUSerialNo"			);	// CPU���к�
		}

	}

	delete m_xmlDoc;
	m_xmlDoc = NULL;

	// ��ȡ��֮��ɾ�������ļ�
	DeleteFile(XmlPath); 


}

/*---------------------------------------*/
//	����˵����
//		д��ͻ���Ϣ
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
void ClientInfoReadWrite::WriteClientInfo()
{
	CFile file;

	SetFileClientInfoPath();


	CString HgePath = m_ClientInfoFileName;
	CString XmlPath = m_ClientInfoFileName.Left(m_ClientInfoFileName.GetLength()-4) + ".xml";

	//��ΪXML�ļ���ʽ����

	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 

	// ������
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	TiXmlElement* pRegInfoElem = new TiXmlElement("ע����Ϣ");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pRegInfoElem);

	// �û���Ϣ
	TiXmlElement* pClientInfoElem = new TiXmlElement("�û���Ϣ");
	pRegInfoElem->LinkEndChild(pClientInfoElem);

	pClientInfoElem->SetAttribute("Name",			m_Info.m_ClientName);		// ����
	pClientInfoElem->SetAttribute("Phone",			m_Info.m_PhoneNumber);		// �ֻ�
	pClientInfoElem->SetAttribute("QQ",				m_Info.m_QQNumber);			// qq
	pClientInfoElem->SetAttribute("Address",		m_Info.m_Address);			// ��ַ
	pClientInfoElem->SetAttribute("RegisterTime",	m_Info.m_RegisterTime);		// ע��ʱ��
	pClientInfoElem->SetAttribute("OutputNcTimes",	m_Info.m_OutputNCTimes);	// ����NC�ļ�����


	// ���Ի�����Ϣ
	TiXmlElement* pMachineInfoElem = new TiXmlElement("������Ϣ");
	pRegInfoElem->LinkEndChild(pMachineInfoElem);



	pMachineInfoElem->SetAttribute("MacAddress",			m_Info.m_MacAdress);			// MAC��ַ
	pMachineInfoElem->SetAttribute("BaseBoardSerialNo",	m_Info.m_BaseBoardSerialNo);	// �������к�
	pMachineInfoElem->SetAttribute("HDSerialNo",			m_Info.m_CPUSerialNo);			// Ӳ�����к�
	pMachineInfoElem->SetAttribute("CPUSerialNo",		m_Info.m_HDSerialNo);			// CPU���к�



	// �����ļ�
	m_xmlDoc->SaveFile(XmlPath);
	delete m_xmlDoc;


	// ����
	encrypt_base64(XmlPath.GetBuffer(), HgePath.GetBuffer());
	DeleteFile(XmlPath);


}

