// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// FileReadWrite.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


#if (CUR_EDITION == HG_EDITION)
TCHAR* g_szSuffix = ".hge";
#else
TCHAR* g_szSuffix = ".ywe";
#endif
