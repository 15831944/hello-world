#ifndef		__InternetChecking_H__
#define		__InternetChecking_H__

#include <string>

using namespace std;






class AFX_EXT_CLASS InternetChecking
{
public:
	// ������֤
	static bool AccessCRM(CString ClientName, CString ClientPhoneNumber, CString ClientQQNumber, CString ClientAddress);

	// ����http��ҳ
	static bool PostHttpPage(int port, CString  hostName, CString pathName, const std::string & postData);

	static bool LoginStatisticsDaily(void);
};


#endif