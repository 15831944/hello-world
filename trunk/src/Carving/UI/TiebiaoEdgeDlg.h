#pragma once

#include "../resource.h"
#include <vector>
#include <string>
#include <map>

// TiebiaoEdgeDlg �Ի���

class TiebiaoEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TiebiaoEdgeDlg)

public:
	TiebiaoEdgeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TiebiaoEdgeDlg();

// �Ի�������
	enum { IDD = IDD_PARAM_TIEBIAO_EDGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	/************************************************************************/
	/* ui                                                                     */
	/************************************************************************/
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedFindImg();

public:
	/************************************************************************/
	/* �߼�                                                                 */
	/************************************************************************/
	//�ڲ����ַ���������������
	void buildEdgeDataByOneEdgeString();
	//����һ�з����Ϣ��
	std::map<std::string,std::string> buildEdgeDataByOneEdgeString(std::string oneEdgeString);
	
	//��������ת�����ڲ������ַ�����
	void saveOneEdgeStringByEdgeData();
	//����һ��������Ϊһ���ַ���
	std::string saveOneEdgeStringByEdgeData(std::map<std::string,std::string> edgeData);

	//��һ������Ϣ���ַ�������Ϊһ������Ϣ
	std::vector<std::map<std::string,std::string>> buildEdgeDataByAllEdgeString(std::string allEdgeString);
	//��һ������Ϣת��Ϊһ��
	std::string saveAllEdgeStringByEdgeData(std::vector<std::map<std::string,std::string>> allEdgeData);

	//ͨ�������ֵ���ҷ�߷���ͼƬ����Ҫ����AllEdgeString��
	std::string findImgPathByEdge(float edge);
	//ͨ�������ֵ���ҷ�߷����ַ�����Ҫ����AllEdgeString��
	std::string findCharByEdge(float edge);

public:
	/************************************************************************/
	/* ����                                                                 */
	/************************************************************************/
	// ��ߺ�ȷ�Χ
	float m_fromThin;
	float FromThin() const { return m_fromThin; }
	void FromThin(float val) { m_fromThin = val; }

	// ��ߺ�ȷ�Χ
	float m_toThin;
	float Thin() const { return m_toThin; }
	void Thin(float val) { m_toThin = val; }

	//��ʾ�ķ�����ַ���
	CString m_DisplayEdgeChar;
	CString DisplayEdgeChar() const { return m_DisplayEdgeChar; }
	void DisplayEdgeChar(CString val) { m_DisplayEdgeChar = val; }

	// ���ʹ��ͼƬ·��
	CString m_EdgeImgPath;
	CString EdgeImgPath() const { return m_EdgeImgPath; }
	void EdgeImgPath(CString val) { m_EdgeImgPath = val; }


	//һ���������
	CString m_oneEdgeString;
	CString OneEdgeString() const { return m_oneEdgeString; }
	void OneEdgeString(CString val) { m_oneEdgeString = val; }

	//ȫ���������
	CString m_allEdgeString;
	CString AllEdgeString() const { return m_allEdgeString; }
	void AllEdgeString(CString val) { m_allEdgeString = val; }
};
