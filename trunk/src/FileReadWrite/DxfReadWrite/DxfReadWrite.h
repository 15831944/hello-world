/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfReadWrite.h      -- dxf�ļ���д�������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.7.21
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_DxfReadWrite_H_
#define		_DxfReadWrite_H_


class Panel;

class AFX_EXT_CLASS DxfReadWrite
{
public:
	static bool OutputUpperFaceDxf(Panel* pPanel, CString strDxfFilePath);	
	static bool OutputDownerFaceDxf(Panel* pPanel, CString strDxfFilePath);

};



#endif