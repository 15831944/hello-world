#include "stdafx.h"
#include "InternetChecking.h"
#include "../JsonCpp/json.h"

#include <afxinet.h>
#include <iphlpapi.h >



#include "../../../include/FileReadWrite/Misc/HGCode.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"


#pragma comment(lib,"Iphlpapi.lib")
//#pragma comment(lib, "../../lib/jsoncpp.lib")

// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/FileReadWrited.lib")

#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/FileReadWrite.lib")

#pragma  message(".......................................��ǰΪrelease�汾")

#endif



wstring AnsiToUnicode(const char* szStr)
{
	int nLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen+1];
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	wstring strReturn(pResult);
	delete[] pResult;
	return strReturn;
}



/*---------------------------------------*/
//	����˵����
//		�ճ���¼ͳ��
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
bool InternetChecking::LoginStatisticsDaily(void)
{
	//������֤
	CString str_mac;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// ��ȡmac��ַ
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	if (ERROR_SUCCESS==nRel)
	{
		for (int i = 0;i < 6;i++)
		{
			CString tmp_mac; 
			tmp_mac.Format(_T("%x "),pIpAdapterInfo->Address[i]);
			str_mac = str_mac + tmp_mac;
		}
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	// ����汾  http://119.23.20.26:8100/
#if (CUR_EDITION == YW_EDITION)

	CString hostName = "119.23.20.26";

#elif (CUR_EDITION == HG_EDITION)

	CString hostName = "119.23.246.117";

#endif



	CString insert_pathName = "api/hgsoftapi.aspx?method=insert&mac=";
	insert_pathName = insert_pathName + str_mac;

	// ��Ʒ���ֲ���ת��
	string url_product_name		=   HGCode::UrlUTF8("��̻����");   
	CString insert_ProductName	= CString("&product=")	+ url_product_name.c_str();

	insert_pathName += insert_ProductName ;


#if (CUR_EDITION == HG_EDITION)
	// ��Ʒid ����ͳ��ʹ�� 
	CString insert_ProductID	= CString("&productid=159");	

	insert_pathName += insert_ProductID ;
#endif



	// ����汾��Ҫ����type=1����Ϊ����
#if (CUR_EDITION == YW_EDITION)

	string url_type			=   HGCode::UrlUTF8("1");
	CString insert_type		= CString("&type=")	+ url_type.c_str();


	insert_pathName += insert_type;

#endif






	// ����http����

	
#if (CUR_EDITION == YW_EDITION)
	
	// ����Ķ˿�Ϊ8100
	int port = 8100;

#elif (CUR_EDITION == HG_EDITION)

	// ���Ķ˿�Ϊ8060
	int port = 8060;

#endif





	CString pathName = insert_pathName; 
	string postData = "insert";

	// ����ػ�
	CInternetSession session(_T("your app agent name")); 

	INTERNET_PORT nPort = (INTERNET_PORT)port; 
	DWORD dwRet = 0;
	CHttpConnection* pServer = session.GetHttpConnection((LPCTSTR)hostName, nPort); 

	CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)pathName);   
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // ����ͷ   

	//��ʼ��������   
	pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size()); 
	pFile->QueryInfoStatusCode(dwRet);

	if (dwRet == HTTP_STATUS_OK) 
	{ 
		CString result, newline;   
		while(pFile->ReadString(newline)) 
		{
			//ѭ����ȡÿ������ 
			result = result + newline+"\r\n"; 
		}   

		if (result.Find("true") < 0)
		{
			//AfxMessageBox("���º�̨����ʧ��");
		}
	} 

	delete pFile; 
	delete pServer;   

	session.Close();   
	return true; 

}



/*---------------------------------------*/
//	����˵����
//		������֤
//
//
//	������
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
bool InternetChecking::AccessCRM(CString ClientName, CString ClientPhoneNumber, CString ClientQQNumber, CString ClientAddress)
{
	//������֤
	CString str_mac;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// ��ȡmac��ַ
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	if (ERROR_SUCCESS==nRel)
	{
		for (int i = 0;i < 6;i++)
		{
			CString tmp_mac; 
			tmp_mac.Format(_T("%x "),pIpAdapterInfo->Address[i]);
			str_mac = str_mac + tmp_mac;
		}
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}


	// ����汾  http://119.23.20.26:8100/
#if (CUR_EDITION == YW_EDITION)

	CString hostName = "119.23.20.26";

#elif (CUR_EDITION == HG_EDITION)

	CString hostName = "119.23.246.117";

#endif

	// check�������
	CString check_pathName = "API/LockAPI.aspx?action=check&mac=";
	check_pathName = check_pathName + str_mac;

	// ����汾��Ҫ����type=1����Ϊ����
#if (CUR_EDITION == YW_EDITION)

	string url_type			=   HGCode::UrlUTF8("1");
	CString check_type		= CString("&type=")	+ url_type.c_str();


	check_pathName += check_type;

#endif








	CString insert_pathName = "API/LockAPI.aspx?action=insert&mac=";
	insert_pathName = insert_pathName+str_mac.GetBuffer();

	// ���ֲ���ת��
	string url_name =   HGCode::UrlUTF8(ClientName.GetBuffer());
	//HGCode::CString_GB2312_To_Unicode(ClientName);
	CString insert_ClientName			= CString("&name=")	+ url_name.c_str();

	CString insert_ClientPhoneNumber	= CString("&mobilephone=")	+ ClientPhoneNumber;
	CString insert_ClientQQNumber		= CString("&qq=")	+ ClientQQNumber;

	// ��ַ����ת��
	//HGCode::CString_GB2312_To_Unicode(ClientAddress);

	string url_Address =   HGCode::UrlUTF8(ClientAddress.GetBuffer());
	CString insert_ClientAddress		= CString("&address=")	+ url_Address.c_str();





	insert_pathName += insert_ClientName ;
	insert_pathName += insert_ClientPhoneNumber;
	insert_pathName += insert_ClientQQNumber;
	insert_pathName += insert_ClientAddress;



	// ����汾��Ҫ����type=1����Ϊ����
#if (CUR_EDITION == YW_EDITION)

	CString insert_type		= CString("&type=")	+ url_type.c_str();


	insert_pathName += insert_type;

#endif



#if 0
	// ����
	int name_len = insert_ClientName.GetLength();
	char* pNameBuf = new char[name_len+1];

	memset(pNameBuf, 0, name_len+1);
	memcpy(pNameBuf, insert_ClientName.GetBuffer(), name_len);

	// �ֻ�
	int phone_len = insert_ClientPhoneNumber.GetLength();
	char* pPhoneBuf = new char[phone_len+1];

	memset(pPhoneBuf, 0, phone_len+1);
	memcpy(pPhoneBuf, insert_ClientPhoneNumber.GetBuffer(), phone_len);


	// qq
	int qq_len = insert_ClientQQNumber.GetLength();
	char* pQQBuf = new char[qq_len+1];

	memset(pQQBuf, 0, phone_len+1);
	memcpy(pQQBuf, insert_ClientQQNumber.GetBuffer(), qq_len);


	// ��ַ
	int address_len = insert_ClientAddress.GetLength();
	char* pAddressBuf = new char[address_len+1];

	memset(pAddressBuf, 0, address_len+1);
	memcpy(pAddressBuf, insert_ClientAddress.GetBuffer(), address_len);

	strcat(pNameBuf,pPhoneBuf);
	strcat(pNameBuf,pQQBuf);
	strcat(pNameBuf,pAddressBuf);



	insert_pathName += pNameBuf ;
	insert_pathName += pPhoneBuf;
	insert_pathName += pQQBuf;
	insert_pathName += pAddressBuf;

#endif

	// jason���ݰ�
	std::string postData = "";

// 	Json::Value root;
// 	root["name"]			= std::string(ClientName.GetBuffer());
// 	root["mobilephone"]		= std::string(ClientPhoneNumber.GetBuffer());
// 	root["qq"]				= std::string(ClientQQNumber.GetBuffer());
// 	root["address"]			= std::string(ClientAddress.GetBuffer());
// 
// 	postData = root.toStyledString();


#if (CUR_EDITION == YW_EDITION)



	// ����½����
	if(PostHttpPage(8100, hostName, check_pathName,"check") == false)
		return false;
	// ���ӵ�¼����
	if (PostHttpPage(8100, hostName, insert_pathName,"insert") == false)
		return false;

#elif (CUR_EDITION == HG_EDITION)


	// ����½����
	if(PostHttpPage(8060, hostName, check_pathName,"check") == false)
		return false;
	// ���ӵ�¼����
	if (PostHttpPage(8060, hostName, insert_pathName,"insert") == false)
		return false;

#endif



	return true;
}


int GB2312ToUtf8(const char* gb2312, char* utf8)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return len;
}





/*---------------------------------------*/
//	����˵����
//		����crm���������󣬽�����Ϣ
//
//
//	������
//		int port						--	�˿ں�
//		const std::string & hostName	--	��������
//		const std::string & pathName	--	·����
//		const std::string & postData	--	��������
//
//	����ֵ:
//
//
/*---------------------------------------*/

bool InternetChecking::PostHttpPage(int port, CString  hostName, CString pathName, const std::string & postData)
{
	// ����ػ�
	CInternetSession session(_T("your app agent name")); 
	//session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 30000);

	try 
	{ 
		INTERNET_PORT nPort = (INTERNET_PORT)port; 
		DWORD dwRet = 0;
		CHttpConnection* pServer = session.GetHttpConnection((LPCTSTR)hostName, nPort); 
		
		CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)pathName);   
		
		CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // ����ͷ   

		//��ʼ��������   
		pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size()); 
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet == HTTP_STATUS_OK) 
		{ 
			CString result, newline;   
			while(pFile->ReadString(newline)) 
			{
				//ѭ����ȡÿ������ 
				result = result + newline+"\r\n"; 
			}   

			if (postData == "check")
			{
				CString message_result = result;


				// �����ǰ����

				message_result.Replace("Message","");
				message_result.Replace("Result","");
				message_result.Replace("true","");
				message_result.Replace("false","");
				message_result.Replace("\"","");
				message_result.Replace(",","");
				message_result.Replace(":","");
				message_result.Replace("{","");
				message_result.Replace("}","");
				std::string result_num = message_result.GetBuffer();
				int i_num = atoi(result_num.c_str());

				//AfxMessageBox(CString("��ǰ��ʹ�ô���Ϊ��") + message_result);


// 				if (i_num >= 1000)
// 				{
// 					AfxMessageBox("ʹ�ô���������Ȩ����");
// 					return false;
// 				}
			}
			else
			{


				if (result.Find("true") < 0)
				{
					AfxMessageBox("���º�̨����ʧ��");
					return false;
				}
			}

		} 
		else 
		{ 
			return false; 
		} 
		delete pFile; 
		delete pServer;   
	} 
	catch (CInternetException* pEx) 
	{ 
		TCHAR pszError[200]; 
		pEx->GetErrorMessage(pszError, 200);   
		AfxMessageBox(pszError);
		return false; 
	} 

	session.Close();   
	return true; 
}


