#include "stdafx.h"

#include "base64.h"

// �����ṩ����Կ		   TECHTIMESUNTECHTIMESUNtechtimesuntechtimesuntechtimesunSUNSUNsun
// ��Կ
const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
						   

char * base64_encode( const unsigned char * bindata, char * base64, int binlength )
{
	int i, j;
	unsigned char current;

	for ( i = 0, j = 0 ; i < binlength ; i += 3 )
	{
		current = (bindata[i] >> 2) ;
		current &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
		if ( i + 1 >= binlength )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}
		current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
		if ( i + 2 >= binlength )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			break;
		}
		current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
		base64[j++] = base64char[(int)current];
	}
	base64[j] = '\0';
	return base64;
}

int base64_decode( const char * base64, unsigned char * bindata )
{
	int i, j;
	unsigned char k;
	unsigned char temp[4];
	for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
	{
		memset( temp, 0xFF, sizeof(temp) );
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i] )
				temp[0]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+1] )
				temp[1]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+2] )
				temp[2]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+3] )
				temp[3]= k;
		}

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
			((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
		if ( base64[i+2] == '=' )
			break;

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
			((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
		if ( base64[i+3] == '=' )
			break;

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
			((unsigned char)(temp[3]&0x3F));
	}
	return j;
}



/*---------------------------------------*/
//	����˵����
//		�����ļ�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
int encrypt_base64(char src_path[], char dst_path[]) 
{
	int src_file_len;
	CFile src_file, dst_file;
	char* pSrcBuf = NULL;
	char* pDstBuf = NULL;

	try
	{
		// ��Դ�ļ�����������
		if (src_file.Open((LPCTSTR)src_path, CFile::modeRead) == TRUE)
		{
			src_file_len = src_file.GetLength();
			pSrcBuf = new char[src_file_len];
			src_file.Read(pSrcBuf, src_file_len);
			src_file.Close();

			// ����������������base64����
			pDstBuf = new char[src_file_len*2];	// ���ܻ��ɣ������Բ��ᳬ��2��
			base64_encode((const unsigned char*)pSrcBuf, pDstBuf, src_file_len);

			// д�����ļ��ļ�
			DeleteFile((LPCTSTR)dst_path);
			
			if (dst_file.Open((LPCTSTR)dst_path, CFile::modeReadWrite | CFile::modeCreate) == TRUE)
			{
				CString str(pDstBuf);
				dst_file.Write(pDstBuf, str.GetLength());
				dst_file.Close();

				// ����ڴ�
				delete[]	pSrcBuf;
				delete[]	pDstBuf;
			}
			
		}
		
		

	}
	catch (CMemoryException* e)
	{

	}
	catch (CFileException* e)
	{
		int cause = e->m_cause;

		AfxMessageBox("�ӽ��ܳ���CFileException::%d", cause);

	}
	catch (CException* e)
	{
	}


	return 0;
}


/*---------------------------------------*/
//	����˵����
//		�����ļ�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
int decrypt_base64(char src_path[], char dst_path[])
{
	int src_file_len;
	CFile src_file, dst_file;
	char* pSrcBuf = NULL;
	char* pDstBuf = NULL;

	try
	{
		// ��Դ�ļ�����������
		if (src_file.Open((LPCTSTR)src_path, CFile::modeRead) == TRUE)
		{

			src_file_len = src_file.GetLength();
			pSrcBuf = new char[src_file_len];
			src_file.Read(pSrcBuf, src_file_len);
			src_file.Close();

			// �������������ݽ���
			pDstBuf = new char[src_file_len];	// ���ܳ���һ���ȼ��ܺ�Ķ�
			int dst_file_len = base64_decode(pSrcBuf, (unsigned char *)pDstBuf);

			// д�����ļ��ļ�
			DeleteFile((LPCTSTR)dst_path);

			if (dst_file.Open((LPCTSTR)dst_path, CFile::modeReadWrite | CFile::modeCreate) == TRUE)
			{
				dst_file.Write(pDstBuf, dst_file_len);
				dst_file.Close();

				// ����ڴ�
				delete[]	pSrcBuf;
				delete[]	pDstBuf;
			}
		}
		else
		{
			AfxMessageBox("�ļ���ʧ��");
		}
	}
	catch (CMemoryException* e)
	{
	}
	catch (CFileException* e)
	{

		int cause = e->m_cause;

		AfxMessageBox("�ӽ��ܳ���CFileException::%d", cause);

	}
	catch (CException* e)
	{
	}


	return 0;
}

int func_drawing0(char* src_path, char* dst_path)
{
	return encrypt_base64(src_path, dst_path);
}

int func_drawing1(char* src_path, char* dst_path)
{
	return decrypt_base64(src_path, dst_path);
}