#include "stdafx.h"
#include "Misc.h"
#include <io.h>

CString GetModulePath()
{
	TCHAR cStr[MAX_PATH+1];
	GetModuleFileName(/*GetModuleHandle(0)*/AfxGetModuleState()->m_hCurrentInstanceHandle, cStr, MAX_PATH+1);
	CString str(cStr);
	str = str.Left(str.ReverseFind('\\')+1);
	return str;
}

vector<CString> TraversFile(CString strPath, LPCTSTR szSubfix)
{
	vector<CString> vRet;

	CFileFind find;
	CString strFind = strPath + _T("*.*");//������һ��ȫ����Ŀ¼
	int nResult = find.FindFile(strFind);
	while(nResult)
	{
		nResult = find.FindNextFile();
		if(find.IsDirectory() && !find.IsDots())
		{
			CString strFolder = find.GetFilePath();
			if(strFolder.GetAt(strFolder.GetLength()-1) != _T('\\'))
				strFolder += _T('\\');
			vector<CString> vTmp = TraversFile(strFolder, szSubfix);
			vRet.insert(vRet.end(), vTmp.begin(), vTmp.end());
		}
		else if(!find.IsDirectory())
		{
			CString strFilePath = find.GetFilePath();
			if(strFilePath.Right(3).CompareNoCase(szSubfix) == 0)
				vRet.push_back(strFilePath);
		}

	}
	return vRet;
}

bool CopyTheFileToTarget(CString strSourceFilePath, CString strTargetFilePath)
{
	bool bRet = true;

	CString strTargetDir;
	int nPos = -1;
	nPos = strTargetFilePath.ReverseFind(_T('\\'));
	strTargetDir = strTargetFilePath.Left(nPos);
	if(access(strTargetDir, 0) != 0)
	{
		if(!CreateMultipleDirectory(strTargetDir))
		{
			CString strTmp;
			strTmp.Format(_T("����Ŀ¼%sʧ�ܣ�"), strTargetDir);
			AfxMessageBox(strTmp);
			return false;
		}
	}

	BOOL bRes = TRUE;
	if(access(strTargetFilePath, 0) == 0)
	{
		bRes = DeleteFile(strTargetFilePath);
	}
	if(bRes)
		//bRes = MoveFile(strSourceFilePath, strTargetFilePath);
		bRes = CopyFile(strSourceFilePath, strTargetFilePath, TRUE);

	if(!bRes)
	{
		bRet = false;
	}

	return bRet;
}

BOOL CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���

	if(strDir.IsEmpty())
		return FALSE;

	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	BOOL bSuccess = FALSE;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? TRUE : FALSE;    
	}

	return bSuccess;
}