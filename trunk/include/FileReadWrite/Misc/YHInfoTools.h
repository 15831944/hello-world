#pragma once

//#include <vector>
#include <string>
//#include <map>
//#include "KnifeClass.h"

using namespace std;
 
#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	string _name;
	string _value;
};
#endif // PARAM_POINT


//struct p_show
//{
//	float _x;
//	float _y;
//};

class YHInfoTools
{
public:
	YHInfoTools(void);

	//////////////////////////////////////////////////////////////////////////
	//���ߺ���
public:
	//(�������滻��ĸ)���磺PW+PD-PK
	static string replace_allstr(string str,const string old_value,const string new_value);

	////��ת��ĵ�λ��Ϣ
	//static void getOtherAllPoint(std::vector<AllPoint>& otherallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width);

	////�õ������λ��Ϣ
	//static void getAllDPoint(std::vector<DAllPoint>& rdallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width,float _OtherKerfWidth);

	////�õ���λ��Ϣ
	//static void getAllPoint(std::vector<AllPoint>& rallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width);

	////�õ��������Ϣ����
	//static void getdAllSlot(std::vector<AllSlot>& rdallslot,CString slotinfo,CString rotateinfo,CString panelinfo);

	////�õ��������Ϣ
	//static void getdAllVec(std::vector<AllVec>& rdallvec,CString vecinfo,CString panelinfo);

	////�õ��������Ϣ
	//static void getAllSlot(std::vector<AllSlot>& rallslot,CString slotinfo,CString rotateinfo,CString panelinfo);

	////�õ��������Ϣ
	//static void getAllVec(std::vector<AllVec>& rallvec,CString vecinfo,CString panelinfo);

	////��ò��ϣ����-���ʣ��еĺ��
	//static float GetThinFromThinMatrial(std::string thin_matrial);

	////��ò��ϣ����-���ʣ��еĲ���
	//static std::string GetMatrialFromThinMatrial(std::string thin_matrial);

	////����Ż�����
	//static OptimizeParam YHInfoTools::GetOptimizParamFromFile();


	//////���ݳ������������������
	//static bool sort_baseStrLen(const param_point & m1, const param_point & m2) ;

	//////���ݳ������������������, �������ճ��ȣ�������
	//static void sortParam_pointByStrLen(std::vector<param_point> & pa_p) ;

	////���ݴ������飬�������������д��ڵ�# %������
	//static void BarCodeFixer(std::vector<KnifeClass *> &_knifeclasses);


};

