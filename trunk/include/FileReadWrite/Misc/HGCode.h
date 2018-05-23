#pragma  once
#include <string>
class AFX_EXT_CLASS HGCode
{
public:
	// ��UTF-8ת����Unicode
	static void UTF8_To_Unicode(wchar_t* pOut,char *pText);  
	//UTF-8 תΪ GB2312
	static void UTF_8To_GB2312(std::string &pOut, char *pText, int pLen);

	//Unicode ת����UTF-8
	static void Unicode_To_UTF8(char* pOut,wchar_t* pText);  
	//��Unicode ת���� GB2312
	static void Unicode_To_GB2312(char* pOut,wchar_t uData);  

	//GB2312 ת���ɡ�Unicode
	static void Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer);
	//GB2312 תΪ UTF-8
	static void GB2312_To_UTF8(std::string& pOut,char *pText, int pLen);

	static std::wstring UTF8_To_ws( const std::string& src );
	static std::string HGCode::UTF8_To_s( const std::string& src );


private:
	// ��UTF-8ת����Unicode
	static void one_UTF8_To_Unicode(wchar_t* pOut,char *pText);  
	//UTF-8 תΪ GB2312
	static void one_UTF_8To_GB2312(std::string &pOut, char *pText, int pLen);

	//Unicode ת����UTF-8
	static void one_Unicode_To_UTF8(char* pOut,wchar_t* pText);  
	//��Unicode ת���� GB2312
	static void one_Unicode_To_GB2312(char* pOut,wchar_t uData);  

	//GB2312 ת���ɡ�Unicode
	static void one_Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer);
	//GB2312 תΪ UTF-8
	static void one_GB2312_To_UTF8(std::string& pOut,char *pText, int pLen);

private:
	static char* szResult;
	static wchar_t* wszResult;

public:
	// wchar ת char
	static const char* wchar_t_Unicode_To_GB2312(wchar_t* str);
	// char ת wchar
	static const wchar_t* char_Gb2312_To_Unicode(const char* szTestNew);
	// ����ʹ��
	static const char* convert(CString& str)
	{
		USES_CONVERSION;
		return wchar_t_Unicode_To_GB2312(A2W(str.GetBuffer()));
	}
	// ����ʹ��
	static const char* convert(wchar_t* wszTestNew)
	{
		return wchar_t_Unicode_To_GB2312(wszTestNew);
	}
	// ����ʹ��
	static const wchar_t* convert(const char* szTestNew)
	{
		return char_Gb2312_To_Unicode(szTestNew);
	}
	// ����ʹ��
	static const wchar_t* convert(std::string& str)
	{
		return char_Gb2312_To_Unicode(str.c_str());
	}


public:
	static void CString_Unicode_To_GB2312(CString& cstr)
	{
		std::string str;
		one_GB2312_To_UTF8(str,cstr.GetBuffer(),cstr.GetLength());
		cstr = str.c_str();
	}
	static void CString_GB2312_To_Unicode(CString& cstr)
	{
		std::string str;
		one_GB2312_To_UTF8(str,cstr.GetBuffer(),cstr.GetLength());
		cstr = str.c_str();
	}



public:
	static std::string UrlGB2312(char * str);                           //urlgb2312����
	static std::string UrlUTF8(char * str);                             //urlutf8 ����
	static std::string UrlUTF8Decode(std::string str);                  //urlutf8����
	static std::string UrlGB2312Decode(std::string str);                //urlgb2312����
private:
	static char StrToBin(char *str);
	static char HGCode:: CharToInt(char ch);


};