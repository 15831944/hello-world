/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfFile.h    -- dxf�ļ��������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	��ȡdxf�ļ���ͼԪ��Ϣ
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		DxfFile_H
#define		DxfFile_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Figure;

class DxfFile
{
public:
	DxfFile();
	~DxfFile();

	int Load(CString file_name);
public:

	CStdioFile m_File;					// �ļ����
	vector<Figure*> m_FigureList;		// ��ȡ��ͼԪ����

public:
	static CString SectionStar;			// �ο�ʼ
	static CString SectionEnd;			// �ν���
	static CString EntitySectionStar;	// ʵ��ο�ʼ

	void ReadFigureData();
	void ReadEntitySection();
	void ReadPolyline();
	void ReadCircle();
	void ReadArc();
};

#endif