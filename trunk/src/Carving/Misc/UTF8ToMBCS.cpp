#include "stdafx.h"

// UTF8����תΪ���ֽڱ���  
bool UTF8ToMB(const char* pszUTF8, int utf8Len, CString &strMB)  
{ 
    if (NULL == pszUTF8 || utf8Len < 1)
        return true;

    // convert an UTF8 string to widechar   
    int nLen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1/*utf8Len*/, NULL, 0);  

    WCHAR* pszW = new WCHAR[nLen];  
    if (NULL == pszW) return false;

    int nRtn = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1/*utf8Len*/, pszW, nLen);  
    if(nRtn != nLen)  
    {  
        delete[] pszW;  
        return false;  
    }  

    // convert an widechar string to Multibyte   
    int MBLen = WideCharToMultiByte(CP_ACP, 0, pszW, nLen, NULL, 0, NULL, NULL);  
    if (MBLen <= 0)  return false;

    char* pszMB = new char[nLen];  
    if (NULL == pszMB)
    {
        delete[] pszW;
        return false;
    }

    nRtn = WideCharToMultiByte(CP_ACP, 0, pszW, nLen, pszMB, MBLen, NULL, NULL);  
    if(nRtn != MBLen)  
    {  
        delete[] pszW;  
        delete[] pszMB;
        return false;
    }

    strMB = pszMB;

    delete[] pszW;  
    //delete[] pszMB;
    return true;  
}


// ���ֽڱ���תΪUnicode����  
bool MBToUnicode(const char* pmb, int len, CStringW& strDest)  
{
    if (NULL == pmb)
        return true;

    // convert an MBCS string to widechar   
    int wlen = MultiByteToWideChar(CP_ACP, 0, pmb, len, NULL, 0);  
    if (wlen <= 0)
        return false;

    wchar_t* pDest = new wchar_t[wlen];
    if (NULL == pDest)
        return false;

    int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, len, pDest, wlen);  

    if (nRtn != wlen)  
    {  
        delete pDest;
        return false;  
    }

    strDest = pDest;
    delete pDest;

    return true;  
}
