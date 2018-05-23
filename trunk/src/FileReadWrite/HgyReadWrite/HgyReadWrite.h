#ifndef		_HgyReadWrite_h_
#define		_HgyReadWrite_h_
#pragma  once

#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>

class Component;

using namespace std;
using namespace TinyXml;

class AFX_EXT_CLASS  HgyReadWrite
{
public:
	//	��׼��д����
	static bool ReadHgy(CString szFileDir);										// ����hgy
	static bool WriteHgy(CString szFileDir);									// ����Ϊhgy
	static bool SaveOneTree2Xml(TiXmlElement* pCurNode, Component* pCurCpn);	// ���������νṹ��XML�ļ�
	static bool LoadOneTreeFromXml(TiXmlElement* pCurNode, Component* pCurCpn);	// ��XML�ļ���ȡ������ͽṹ

	// ���������Ρ��ײ�����

	static void SaveOutline(TiXmlElement* pCpnElem, Component* pCpn);			// ��������������
	static void SaveUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn);		// ���������������
	static void SaveUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn);		// ���������������
	static void SaveDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn);	// ���������������
	static void SaveDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn);	// ���������������

	// ��ȡ������Ρ��ײ�����
	static void ReadOutlineHoleSlotInfo(TiXmlElement* pCurCpnNode, Component* pNewCpn);// ��ȡ������Ρ��ײ�����
	static void ReadOutline(TiXmlElement* pCpnElem, Component* pCpn);			// ��ȡ�����������
	static void ReadUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn);		// ��ȡ������������
	static void ReadUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn);		// ��ȡ������������
	static void ReadDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn);	// ��ȡ������������
	static void ReadDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn);	// ��ȡ������������



public:
	// �ض���CSharp���õ���ʱ����
	static bool WriteHgy_CSharp(CString szFileDir);									// ����Ϊhgy



};

#endif