#ifndef		_HgmReadWrite_h_
#define		_HgmReadWrite_h_
#pragma  once

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>
#include <map>


using namespace std;

class AFX_EXT_CLASS  HgmReadWrite
{
public:

	// ��ȡ����������ĵ�̻��Խ�hgm��ʽ�ļ�
	static bool  ReadPanelInfo(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);
	static vector<map<string, string>> translatexml(std::string filepath, SourceFilePreProccesParam& param);

	// ��ȡ�����ļ�
	static void LoadSettings_KnifeType();
	static void SaveSettings_KnifeType();

public:
	// ��erpʹ�õĶ�ȡ�����ݽӿ�
	static bool  ReadPanelInfoByStream(char* pszXml, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);
		static vector<map<string, string>> translatexmlByStream(char* pszXml, SourceFilePreProccesParam& param);





public:

	// Ϊ��debug���ԣ������ܣ�ֱ�Ӷ�ȡxml
	static bool  ReadPanelInfo_Debug(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);


};

#endif