#pragma  once

#include <string>

// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/FileCompressandUnCompressd.lib")
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/FileCompressandUnCompress.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif


class HGTools
{
public:
	//����ָ��Ŀ¼
	static void createDir(CString path);
	//ɾ�����Ŀ¼�µ��ļ�
	static void deleteFile(CString path);
	//ɾ�����Ŀ¼�Լ����������ļ�
	static bool deletePath(CString path);
	//ɾ�����Ŀ¼���������ļ�(��������Ŀ¼)
	static void deletePathFiles(CString path);
	//�ж����·���Ƿ���һ���ļ�
	static bool isFileExists(CString path);
	//�ж����·���Ƿ���һ��Ŀ¼
	static bool isDir(CString path);

	//��ȡ��ָ·������exe·�������·��
	static CString getRelativePath(CString path);
	//��ȡ��ǰ����EXE����·��
	static CString getCurExeFullPath();
	//��ȡ��ǰ����EXE����·����Ŀ¼
	static CString getCurExeContainPath();
	//��ȡ�ļ�����·����Ŀ¼
	static CString getContainPath(CString path);
	//��ȡ�ļ����ƴ���׺��
	static CString getFileNameWithSuffix(CString path);
	//��ȡ�ļ����Ʋ�����׺��
	static CString getFileName(CString path);
	//��ó���ǰ�汾��
	static CString getVersion();
	//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
	static CString getXmlPathFromHgxPath(CString path);
	//�������·����".",".."��·��ת�������ֱ���Ǵ��ַ��
	static CString getRealPathFromReleativePath(CString path);
	//�������·����".",".."��·��ת�������ֱ���Ǵ��ַ��
	//static CString getReleativePathFromRealPath(CString path);



	//����ָ���ļ���ָ��λ��
	static void encryptFile(CString src,CString dst);
	//����ָ���ļ���ָ��λ��
	static void decryptFile(CString src,CString dst);

	//ת��ΪChar*
	static char* Convert2FillChar(CString src,char* dst);
	//ת��ΪChar*
	static const char* Convert2Char(CString src);
	//ת��Ϊstd::string
	static std::string Convert2StdString(int src);
	//ת��Ϊstd::string
	static std::string Convert2StdString(float src);
	//ת��Ϊstd::string
	static std::string Convert2StdString(CString src);
	//ת��Ϊstd::string
	static std::string Convert2StdString(char* src);
	//ת��ΪCString
	static CString Convert2CString(char* src);
	//ת��ΪCString
	static CString Convert2CString(std::string src);



	//�����С�������ű���
	static float getMinScale(CRect& putterRect, CRect& containerRect);

	//�����С�������ű���
	static float getMinScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h);

	//�������������ű���
	static float HGTools::getMaxScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h);

	//���ַ����������ķ��Ž��в��
	static int SplitCString(CString strIn, CStringArray& strAryRe, char division);

	//���ַ����������ķ��Ž��в��
	static int SplitCString(CString &strIn, CStringArray& strAryRe, LPCTSTR str);

	//����෴��
	static std::string GetNagativeValue( std::string value );

	//����ļ�����󱣴�ʱ��
	static long long GetFileLastTime(const CString filePath);
};
