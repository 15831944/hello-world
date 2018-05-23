#include "stdafx.h"  
#include "HardwareInfo.h"  
#pragma  comment(lib, "iphlpapi.lib")


//--------------------------------------------------------------  
//                      CPU���к�  
//--------------------------------------------------------------  
BOOL GetCpuByCmd(char *lpszCpu, int len/*=128*/)  
{     
	const long MAX_COMMAND_SIZE = 10000; // ��������������С     
	//WCHAR szFetCmd[] = L"wmic cpu get processorid"; // ��ȡCPU���к�������    
	const string strEnSearch = "ProcessorId"; // CPU���кŵ�ǰ����Ϣ  

	BOOL   bret = FALSE;  
	HANDLE hReadPipe = NULL; //��ȡ�ܵ�  
	HANDLE hWritePipe = NULL; //д��ܵ�      
	PROCESS_INFORMATION pi;   //������Ϣ      
	STARTUPINFO         si;   //���������д�����Ϣ  
	SECURITY_ATTRIBUTES sa;   //��ȫ����  

	char            szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������  
	string          strBuffer;  
	unsigned long   count = 0;  
	long            ipos = 0;  

	memset(&pi, 0, sizeof(pi));  
	memset(&si, 0, sizeof(si));  
	memset(&sa, 0, sizeof(sa));  

	pi.hProcess = NULL;  
	pi.hThread = NULL;  
	si.cb = sizeof(STARTUPINFO);  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
	sa.lpSecurityDescriptor = NULL;  
	sa.bInheritHandle = TRUE;  

	//1.0 �����ܵ�  
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);  
	if (!bret)  
	{  
		goto END;  
	}  

	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�  
	GetStartupInfo(&si);  
	si.hStdError = hWritePipe;  
	si.hStdOutput = hWritePipe;  
	si.wShowWindow = SW_HIDE; //���������д���  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  

	//3.0 ������ȡ�����еĽ���  
	bret = CreateProcess(NULL,/* szFetCmd*/"wmic cpu get processorid", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);  
	if (!bret)  
	{  
		goto END;  
	}  

	//4.0 ��ȡ���ص�����  
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);  
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);  
	if (!bret)  
	{  
		goto END;  
	}  

	//5.0 ����CPU���к�  
	bret = FALSE;  
	strBuffer = szBuffer;  
	ipos = strBuffer.find(strEnSearch);  

	if (ipos < 0) // û���ҵ�  
	{  
		goto END;  
	}  
	else  
	{  
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());  
	}  

	memset(szBuffer, 0x00, sizeof(szBuffer));  
	strcpy_s(szBuffer, strBuffer.c_str());  

	//ȥ���м�Ŀո� \r \n  
	int j = 0;  
	for (int i = 0; i < strlen(szBuffer); i++)  
	{  
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')  
		{  
			lpszCpu[j] = szBuffer[i];  
			j++;  
		}  
	}  

	bret = TRUE;  

END:  
	//�ر����еľ��  
	CloseHandle(hWritePipe);  
	CloseHandle(hReadPipe);  
	CloseHandle(pi.hProcess);  
	CloseHandle(pi.hThread);  

	return(bret);  
}  



BOOL GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/) 
{	
	const long MAX_COMMAND_SIZE = 10000; 
	// ��������������С	
	// WCHAR szFetCmd[]	= L"wmic BaseBoard get SerialNumber"; 
	// ��ȡ�������к�������	
	const string strEnSearch = "SerialNumber"; 
	// �������кŵ�ǰ����Ϣ 
	BOOL bret	= FALSE; HANDLE hReadPipe = NULL; 
	//��ȡ�ܵ� 
	HANDLE hWritePipe = NULL; 
	//д��ܵ�	
	PROCESS_INFORMATION pi; 
	//������Ϣ	
	STARTUPINFO	si;
	//���������д�����Ϣ
	SECURITY_ATTRIBUTES sa; 
	//��ȫ���� 
	char	szBuffer[MAX_COMMAND_SIZE+1] = {0}; 
	// ���������н�������������
	string	strBuffer; 
	unsigned long	count = 0; 
	long	ipos = 0;
	memset(&pi, 0, sizeof(pi)); 
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa)); 
	pi.hProcess = NULL;
	pi.hThread = NULL; 
	si.cb	= sizeof(STARTUPINFO);
	sa.nLength	= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL; 
	sa.bInheritHandle	= TRUE; 
	//1.0 �����ܵ� 
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0); if(!bret) { goto END; } 
	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ� 
	GetStartupInfo(&si); 
	si.hStdError	= hWritePipe; 
	si.hStdOutput	= hWritePipe;
	si.wShowWindow	= SW_HIDE; 
	//���������д��� 
	si.dwFlags	= STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; 
	//3.0 ������ȡ�����еĽ��� 
	bret = CreateProcess(NULL, /*szFetCmd*/"wmic BaseBoard get SerialNumber", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi );
	if(!bret) 
	{ goto END; }	
	//4.0 ��ȡ���ص����� 
	WaitForSingleObject (pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0); if(!bret) { goto END; } 
	//5.0 �����������к� 
	bret = FALSE; 
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch); 
	if (ipos < 0)  // û���ҵ� 
	{ 
		goto END; 
	}
	else 
	{ 
		strBuffer = strBuffer.substr(ipos+strEnSearch.length()); 
	}	
	memset(szBuffer, 0x00, sizeof(szBuffer)); 
	strcpy_s(szBuffer, strBuffer.c_str()); 

	//ȥ���м�Ŀո� \r \n 
	int j = 0; 
	for (int i = 0; i < strlen(szBuffer); i++) 
	{ 
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r') 
		{ 
			lpszBaseBoard[j] = szBuffer[i]; j++; 
		} 
	} 
	bret = TRUE; 

END: //�ر����еľ�� 
	CloseHandle(hWritePipe); 
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess); 
	CloseHandle(pi.hThread);

	return(bret);
} 



void ChangeByteOrder(PCHAR szString, USHORT uscStrSize)   
{   
	USHORT  i   = 0;   
	CHAR    temp= '\0';   

	for (i = 0; i < uscStrSize; i+=2)   
	{   
		temp            = szString[i];   
		szString[i]     = szString[i+1];   
		szString[i+1]   = temp;   
	}   
}  

//--------------------------------------------------------------  
//                      Ӳ�����к�  
//--------------------------------------------------------------  
BOOL GetHDSerialByCmd(char *lpszHD, int len/*=128*/)  
{   
	BOOL        bRtn     = FALSE;  
	DWORD       bytesRtn = 0;  
	char        szhd[80] = {0};   
	PIDSECTOR   phdinfo;   
	HANDLE      hDrive   = NULL;   
	GETVERSIONOUTPARAMS vers;   
	SENDCMDINPARAMS     in;   
	SENDCMDOUTPARAMS    out;   

	ZeroMemory(&vers, sizeof(vers));   
	ZeroMemory(&in  , sizeof(in));   
	ZeroMemory(&out , sizeof(out));   

	//�����ĸ�����Ӳ�̣�ȡ��һ�������ݵ�����Ӳ��  
	for (int j=0; j<4; j++)  
	{   
		sprintf(szhd,   "\\\\.\\PhysicalDrive%d",   j);   
		hDrive = CreateFileA(szhd,   
			GENERIC_READ|GENERIC_WRITE,   
			FILE_SHARE_READ|FILE_SHARE_WRITE,  
			0,  
			OPEN_EXISTING,  
			0,  
			0);   
		if (NULL == hDrive)  
		{   
			continue;   
		}   

		if (!DeviceIoControl(hDrive, DFP_GET_VERSION, 0, 0, &vers, sizeof(vers), &bytesRtn,0))  
		{   
			goto FOREND;  
		}   

		//If IDE identify command not supported, fails   
		if (!(vers.fCapabilities&1))  
		{   
			goto FOREND;  
		}   

		//Identify the IDE drives   
		if (j&1)  
		{   
			in.irDriveRegs.bDriveHeadReg = 0xb0;   
		}  
		else  
		{   
			in.irDriveRegs.bDriveHeadReg = 0xa0;   
		}   

		if (vers.fCapabilities&(16>>j))  
		{   
			//We don't detect a ATAPI device.   
			goto FOREND;  
		}  
		else  
		{   
			in.irDriveRegs.bCommandReg = 0xec;   
		}   
		in.bDriveNumber = j;   
		in.irDriveRegs.bSectorCountReg  = 1;   
		in.irDriveRegs.bSectorNumberReg = 1;   
		in.cBufferSize = 512;   

		if (!DeviceIoControl(hDrive, DFP_RECEIVE_DRIVE_DATA, &in, sizeof(in), &out, sizeof(out), &bytesRtn,0))  
		{   
			//"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl;   
			goto FOREND;              
		}   
		phdinfo=(PIDSECTOR)out.bBuffer;   

		char    s[21] = {0};          
		memcpy(s, phdinfo->sSerialNumber, 20);         
		s[20] = 0;   
		ChangeByteOrder(s, 20);   

		//ɾ���ո��ַ�  
		int ix = 0;  
		for (ix=0; ix<20; ix++)  
		{  
			if (s[ix] == ' ')  
			{  
				continue;  
			}  
			break;  
		}  
		memcpy(lpszHD, s+ix, 20);  

		bRtn = TRUE;  
		break;  
FOREND:  
		CloseHandle(hDrive);   
		hDrive = NULL;   
	}  

	CloseHandle(hDrive);   
	hDrive = NULL;   
	return(bRtn);  
}  


// ��ȡmac��ַ
void GetMacAdress(CString& str_mac)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// ֻҪ��һ��û�л�ȡ�ɹ�����˵�����类�����ˣ����ٻ�ȡ�ڶ���
	// ��ȡmac��ַ
// 	if (ERROR_BUFFER_OVERFLOW==nRel)
// 	{
// 		delete pIpAdapterInfo;
// 		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
// 		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
// 	}

	// ��ȡ�ɹ�
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
}



void GetHDSerial(CString& str_hd)
{

	char lpszHD[128] = {0};  
	GetHDSerialByCmd(lpszHD); 
	str_hd = lpszHD;
}
void GetCpu(CString& str_cpu)
{

	char lpszCPU[128] = {0};  
	GetCpuByCmd(lpszCPU);
	str_cpu = lpszCPU;
}


void GetBaseboard(CString& str_baseboard)
{
	char lpszBaseBoard[128] = {0};  
	GetBaseBoardByCmd(lpszBaseBoard);
	str_baseboard = lpszBaseBoard;
}