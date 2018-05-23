#include "stdafx.h"
#include "HGTools.h"


#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"

//����ָ��Ŀ¼
void HGTools::createDir(CString path)
{
	if (!HGTools::isDir(path))
	{
		path = "\""+ path +"\"";
		char temp[512] = {0};
		temp[0] = 'm';
		temp[1] = 'd';
		temp[2] = ' ';
		HGTools::Convert2FillChar(path,temp+3);
		system(temp);
	}
}

//�ж����·���Ƿ���һ��Ŀ¼
bool HGTools::isDir(CString path)
{
	return PathIsDirectory(path);
}




//ɾ�����Ŀ¼�µ��ļ�
void HGTools::deleteFile(CString path)
{
	DeleteFile(path);
}


//ɾ�����Ŀ¼�Լ����������ļ�
bool HGTools::deletePath(CString path)
{
	char DirName[MAX_PATH];
	Convert2FillChar(path,DirName);

	char szCurPath[MAX_PATH];       //���ڶ���������ʽ  
    _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�  
    WIN32_FIND_DATAA FindFileData;        
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
    HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);  
    BOOL IsFinded = TRUE;  
    while(IsFinded)  
    {  
        IsFinded = FindNextFileA(hFile, &FindFileData); //�ݹ������������ļ�  
        if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //�������"." ".."Ŀ¼  
        {  
            std::string strFileName = "";  
            strFileName = strFileName + DirName + "//" + FindFileData.cFileName;  
            std::string strTemp;  
            strTemp = strFileName;  
            if( isDir(strFileName.c_str()) ) //�����Ŀ¼����ݹ�ص���  
            {     
                printf("Ŀ¼Ϊ:%s/n", strFileName.c_str());  
                deletePath(strTemp.c_str());  
            }  
            else  
            {  
                DeleteFileA(strTemp.c_str());  
            }  
        }  
    }  
    FindClose(hFile);   
	return TRUE;  
}

//ɾ�����Ŀ¼���������ļ�(��������Ŀ¼)
void HGTools::deletePathFiles(CString path)
{
	if (HGTools::isDir(path))
	{
		path = "\""+ path +"\\*.*\" /Q";
		char temp[512] = {0};
		temp[0] = 'd';
		temp[1] = 'e';
		temp[2] = 'l';
		temp[3] = ' ';
		HGTools::Convert2FillChar(path,temp+4);
		system(temp);

	}
}


//�ж����·���Ƿ���һ���ļ�
bool HGTools::isFileExists(CString path)
{
	return PathFileExists(path);
}



//��ȡ��ǰ����EXE����·���������·��
CString HGTools::getRelativePath(CString path)
{
	return getCurExeContainPath() + "\\" + path ;
}
//��ȡ��ǰ����EXE����·��
CString HGTools::getCurExeFullPath()
{
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;

	return file_path;
}

//��ȡ��ǰ����EXE����·����Ŀ¼
CString HGTools::getCurExeContainPath()
{
	CString file_path = getCurExeFullPath();
	file_path = getContainPath(file_path);

	return file_path;
}

//��ȡ�ļ�����·����Ŀ¼
CString HGTools::getContainPath(CString path)
{
	CString file_path = path;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	return file_path;
}



//��ȡ�ļ����ƴ���׺��
CString HGTools::getFileNameWithSuffix(CString path)
{
	CString file_path = path;
	file_path = file_path.Mid(file_path.ReverseFind('\\')+1);
	return file_path;
}
//��ȡ�ļ����Ʋ�����׺��
CString HGTools::getFileName(CString path)
{
	CString file_path = getFileNameWithSuffix(path);
	file_path = file_path.Left(file_path.ReverseFind('.'));
	return file_path;
}
//��ó���ǰ�汾��
CString HGTools::getVersion()
{
	return " v2.0";
}

//����ָ���ļ���ָ��λ��
// void HGTools::encryptFile(CString src,CString dst)
// {
// 	char encryptPath[1000];
// 	char originPath[1000];
// 	Convert2FillChar(dst,encryptPath);
// 	Convert2FillChar(src,originPath);
// 	File_Encrypt(originPath,encryptPath);
// }
// //����ָ���ļ���ָ��λ��
// void HGTools::decryptFile(CString src,CString dst)
// {
// 	char decryptPath[1000];
// 	char originPath[1000];
// 	Convert2FillChar(src,decryptPath);
// 	Convert2FillChar(dst,originPath);
// 	File_Decrypt(decryptPath,originPath);	
// 
// 
// }


//��CString ת���� char[]����
char* HGTools::Convert2FillChar(CString src,char* dst)
{
	std::string src_string = src.GetBuffer();
	int i = 0;
	for(;i < src_string.size();i++)
	{
		dst[i] = src_string.at(i);
	}
	dst[i] = '\0';
	return dst;
}

//ת��Ϊstd::string
std::string HGTools::Convert2StdString(int src)
{
	CString temp;
	temp.Format("%d",src);
	return temp.GetBuffer();
}

//ת��Ϊstd::string
std::string HGTools::Convert2StdString(float src)
{
	CString temp;
	temp.Format("%f",src);
	return temp.GetBuffer();
}



//ת��ΪChar*
const char* HGTools::Convert2Char(CString src)
{
	std::string temp = src.GetBuffer();
	return temp.c_str();
}
//ת��Ϊstd::string
std::string HGTools::Convert2StdString(CString src)
{
	return src.GetBuffer();
}

//ת��Ϊstd::string
std::string HGTools::Convert2StdString(char* src)
{
	return std::string(src);
}


//ת��ΪCString
CString HGTools::Convert2CString(char* src)
{
	CString temp;
	temp.Format("%s",src);
	return temp;
}
//ת��ΪCString
CString HGTools::Convert2CString(std::string src)
{
	CString temp;
	temp.Format("%s",src.c_str());
	return temp;
}

//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
CString HGTools::getXmlPathFromHgxPath(CString pathName)
{
	if (pathName.Right(4).Left(3) = ".hg")//ƥ�� ".hg*"
	{
		pathName = pathName.Left(pathName.GetLength()-4) + ".xml";
		return pathName;
	}
	return pathName;
}
//�������·����".",".."��·��ת�������ֱ���Ǵ��ַ��
CString HGTools::getRealPathFromReleativePath(CString path)
{
	char tempPath[512] = {0};
	return PathCombine(tempPath,path,NULL);
}


//�����С�������ű���
float HGTools::getMinScale(CRect& putterRect, CRect& containerRect)
{
	float scale_w = putterRect.Width()/((float)(containerRect.Width()));
	float scale_h = putterRect.Height()/((float)(containerRect.Height()));

	float scale = scale_w > scale_h ? scale_w : scale_h;

	return scale;
}

//�����С�������ű���
float HGTools::getMinScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = putterRect_w/containerRect_w;
	float scale_h = putterRect_h/containerRect_h;

	float scale = scale_w > scale_h ? scale_h : scale_w;

	return scale;
}

//�������������ű���
float HGTools::getMaxScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = containerRect_w/putterRect_w;
	float scale_h = containerRect_h/putterRect_h;

	float scale = scale_w < scale_h ? scale_w : scale_h;

	return scale;
}


//���ַ����������ķ��Ž��в��
int HGTools::SplitCString(CString strIn, CStringArray& strAryRe, char division)
{
	strAryRe.RemoveAll();
	if (!strIn.IsEmpty())
	{
		int nCount = 0;
		int nPos = -1;
		nPos = strIn.Find(division);
		CString strTemp = strIn;
		while (nPos != -1)
		{
			CString strSubString = strTemp.Left(nPos);   
			strTemp = strTemp.Right(strTemp.GetLength() - nPos-1);   
			nPos = strTemp.Find(division); 
			nCount++;
			strAryRe.Add(strSubString);
		}

		if (nCount == strAryRe.GetSize())
		{
			CString str;
			int nSize = strIn.ReverseFind(division);
			str = strIn.Right(strIn.GetLength()-nSize-1);
			strAryRe.Add(str);
		}	
	}

	return strAryRe.GetSize();
}

//���ַ����������ķ��Ž��в��
int HGTools::SplitCString(CString &strIn, CStringArray& strAryRe, LPCTSTR str)
{
	if (str == _T(""))
	{
		strAryRe.RemoveAll();
		return 0;
	}

	int nStart = 0;
	int nEnd = 0;
	CString strTmp;
	while ( (nEnd = strIn.Find(str, nStart)) != -1 )
	{
		strTmp = strIn.Mid(nStart, nEnd-nStart);
		strAryRe.Add(strTmp);
		nStart = nEnd+2;
	}
	strTmp = strIn.Mid(nStart, strIn.GetLength());
	strAryRe.Add(strTmp);

	return strAryRe.GetSize();

}

//����෴��
std::string HGTools::GetNagativeValue( std::string value )
{
	float val = atof(value.c_str());
	val = val*-1;
	CString output;
	output.Format("%f",val);
	return output.GetBuffer();
}

//����ļ�����󱣴�ʱ��
long long HGTools::GetFileLastTime(const CString filePath)
{
	WIN32_FIND_DATAA FindFileData;
	::memset(&FindFileData,0,sizeof(FindFileData));
	::FindClose(::FindFirstFileA(filePath,&FindFileData));
	FILETIME& lastWriteTime = FindFileData.ftLastWriteTime;
	return (((long long)lastWriteTime.dwHighDateTime)<<32)+lastWriteTime.dwLowDateTime;
}