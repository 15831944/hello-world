#ifndef		_KnifeClass_H_
#define		_KnifeClass_H_



#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
using namespace std;

class Panel;
class Component;



class __declspec(dllexport) KnifeClass
{
public:
	KnifeClass(void);
	~KnifeClass(void);

	void setKerfwidth(float kerfwidth){m_kerfwidth = kerfwidth;}
	void setDeburringwidth(float deburringwidth){m_deburringwidth = deburringwidth;}
	void setPanelHeight(float panelheight){m_panelheight = panelheight;}
	void setPanelWidth(float panelwidth){m_panelwidth = panelwidth;}
	void setThickness(float thickness){_thickness = thickness;}
	void setThinMatrial(string ThinMatrial){_thin_matrial = ThinMatrial;}
	void setPanelMatrial(string Matrial){m_material = Matrial;}
	void setOptimizeParam(OptimizeParam optimizeparam){m_optimizeparam = optimizeparam;}

	string getPanelMatrial(){return m_material;}
	float getThickness(){return _thickness;}



	void SetLeftParam(float leftthin,float spanelsize,float leftsp){m_leftthin = leftthin;m_spanelsize = spanelsize;m_leftsp = leftsp;}

	void setKnifeType(std::vector<KnifeType> Knifetype);
	void setvecchangexy(float _x,float _y){m_vecchangex = _x;m_vecchangey = _y;}
	void setvecchangexy(vector<float> cxy);

	// ������֤
	static bool InternetChecking(void);

	// ����http��ҳ
	static bool PostHttpPage(int port,const std::string & hostName, const std::string & pathName, const std::string & postData);

public:
	// ����NC��غ���
	bool OutputdvsGcodem(	Panel* panel,			string file,			string file1,				int cutprint,
							bool doublecut,			bool usecoor,			int coortype,				int PanelNum,
							string str1,			string str2,			vector<P_Info> _pinfo,		string grouptype,
							string file_change,		int dir,				vector<float> _df,			string f_header,
							string f_ender,			int _merge,				string _mainktgcodeb,		string _mainktgcodee,
							string _vecktgcodeb,	string _vecktgcodee,	int total_check_panel_num,	BOOL panel_comments);

	bool OutputGcode(		Panel* panel,			std::string file,		std::string file1,			int cutprint,
							bool doublecut,			bool usecoor,			int coortype,				int PanelNum,
							string str1,			string str2,			vector<P_Info> _pinfo,		string grouptype,
							string file_change,		string _mainktgcodeb,	string _mainktgcodee,		string _vecktgcodeb,
							string _vecktgcodee,	int _vecnochange,		bool cutcore,				string node_dvcut,
							bool LabelInReverseSide, int total_check_panel_num ,  int decimal_place,	BOOL panel_comments);

	


	vector<gcodeCoor> pathoptimize(vector<Component*>& CpnList);

	vector<PointInfo> outdirpoint(vector<PointInfo> oldpoint,float dis,int & _pos,std::string panel_info);



	void OutputLabel(Panel* panel,					string CurCutFileName,			string CurPrintFileName,
					int cutprint,					int PanelNum,					string NextPrintFileName,	
					string NextCutFileName,			vector<P_Info> _pinfo,			ofstream& wfile,
					vector<gcodeCoor>& _newcoor,	string node_dvcut,				bool LabelInReverseSide);



	void GetShortestPath(std::vector<pos> & poses,pos & beginpos,std::vector<pos> & newposes);
	void GetChangePath(std::vector<pos> poses,std::vector<pos> & newposes);



	std::string ChangeGroup(int _x,int _y,std::string _changestr);
	std::string ChangeGroup(std::vector<int> _kn,std::string _changestr);


	std::string replace_allstr(std::string str,const std::string old_value,const std::string new_value);

	vector<gcodeCoor> rollsetoptimize(vector<gcodeCoor> _newcoor);
	bool Line_intersection(float begin_px1,float begin_py1,float end_px1,float end_py1,float begin_px2,float begin_py2,float end_px2,float end_py2);

	void PrintNumber2Array(char* str, int str_len,int decimal_place, float val);


public:
	// ��������

	// �ж�˭�Ƚ�С
	static bool ComponentCompareID(const Component* pfirst, const Component* psecond);


	// ���㹫ʽֵ�� ԭ��������Construction�ģ���������������ⲿ�⣬���ܽ���
	static float  calcFormula(std::string formula);

	// ����ת���������㷨�����ݣ�ƽ��ֱ������ϵ��һ���ޣ�ת����ԭ�е�̻��������ʽ��ƽ��ֱ������ϵ�������ޡ���Ļ����ϵ��
	void TransformMachiningInfoToOld_UpperFace(Panel* pPanel, vector<Component*>& CpnList, vector<gcodeCoor>& NewCoorList, 
												vector<AllPoint>& AllPointList, vector<AllSlot>& AllSlotList, vector<AllVec>& AllVecHoleList,
												map<int,PointFloat>&	LabelPositionMap, vector<CutPath>& CutPathList, vector<CutPath>& PreCutPathList);

	void TransformMachiningInfoToOld_DownerFace(Panel* pPanel, vector<Component*>& CpnList, vector<gcodeCoor>& NewCoorList,
												vector<AllSlot>& DownerAllSlotList, vector<AllVec>& DownerAllVecHoleList);


	// ����һ���������С���µ���
	 void SetKnifeDownPos(Panel* pPanel);



public:
	float m_kerfwidth;									// �����
	float m_deburringwidth;								// �ޱ߿��
	float _thickness;									// ��ȣ���ʲô�ĺ�ȣ�����
	std::string _thin_matrial;							// ����
	std::string m_material;								// ����
	vector<p_Component> m_com;							// �������
	vector<AllPoint> m_allpoint;						// ���е�ԭʼ��
	vector<DAllPoint> m_disallpoint;					// ���еĵ�����
	vector<AllPoint> m_otherallpoint;					// ������ԭʼ��
	vector<AllSlot> m_allslot;							// ����Ĳ�
	vector<AllVec> m_allvec;							// ����Ŀ�
	vector<AllSlot> m_dallslot;							// �����Ĳ�
	vector<AllVec> m_dallvec;							// ����Ŀ�
	float m_vecchangex;
	float m_vecchangey;
	vector<float> m_cxy;
	float m_panelwidth;									// ���
	float m_panelheight;								// ���


	vector<CutPath> m_PreCutPath;						// ��һ�����������һ����ޱ�
	vector<CutPath> m_cutpath;							// ��һ�������������и�
	map<Panel *,vector<gcodeCoor>> m_handnewcoor;		// �Ż��и�ó���G����
	GcodeParam m_gcodeparam;							// G�������
	std::vector<KnifeType> m_knifetype;					// ������
	OptimizeParam m_optimizeparam;						// �Ż�����
	map<PanelNode *,int> m_orderpanel;					// ������
	int m_max_merge_num;								// �����������
	std::string m_cut_id;								// ����
	bool opstop;										// ֹͣ�Ż�
	//int kc_num;
	float m_leftthin;								
	float m_spanelsize;
	float m_leftsp;


public:
	CString m_strOrderNum;
	static int m_nOutputTBNum; // ��������ļ����
	static CList<CString> m_TBStringList;

public:
	// ��������
	static void ResetOutputTBNum(void);			// ��ʼ����������������
	void SetInputOrderNum(CString str);			// ���ö�����
	CString GetInputOrderNum(void);				// ��ȡ������

	// С��������
	bool m_bSmallPanelCutTwiceEnable;			// �ж�С��ı�׼����
	float m_SmallPanelStandardSize;				// �ж�С��ı�׼����
	float m_SmallPanelSecondSpeed;				// С��ڶ��ε��ٶ�
	float m_SmallPanelReserveHeight;			// С��Ԥ���߶�������ʱʹ��

	void InitSmallPanelCutTwicePara(void);		// ��ʼ��С������������
	void SetSmallPanelCutTwicePara(bool enable, float small_size, float reserve_height, float second_speed);		// ����С������������
	void GetSmallPanelCutTwicePara(bool& enable, float& small_size, float& reserve_height, float& second_speed);	// ��ȡС������������



	// ����G����ʱ���и�һ���������
	void OutputGCode_KnifeOnePanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
		int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
		float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed,  int decimal_place);			

	// ����G����ʱ���и�һ���С�壬��Ҫ�����µ�
	void OutputGCode_KnifeOneSmallPanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
		int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
		float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed,  int decimal_place);	


	// ����ָ���ޣ���忪����
	float m_ThicknessThresholdSize;				// ��Ƚ���
	float m_ThicknessThresholdSecondSpeed;		// �����˺�Ƚ��޵��ٶ�
	float m_ThicknessThresholdReserveHeight;	// �����˺�Ƚ��޵�Ԥ���߶ȣ�������ʱʹ��

	void InitThicknessThresholdPara(void);		// ��ʼ����Ƚ��޲���
	void SetThicknessThresholdPara(float threshol_size, float reserve_height, float second_speed);		// ���ú�Ƚ��޲���
	void GetThicknessThresholdPara(float& threshol_size, float& reserve_height, float& second_speed);	// ��ȡ��Ƚ��޲���


	// ���ε�������ʱʹ��
	float m_AbnormalKnifeWidth;					// ���ε�����ȣ�Ԥ�裬Ĭ�����ڿ���

	void InitAbnormalKnifeWidth(void);			// ��ʼ�����ε���
	void SetAbnormalKnifeWidth(float width);	// �������ε���
	void GetAbnormalKnifeWidth(float& width);	// ��ȡ���ε���

	float getPanelWidth(){return m_panelwidth;}
	float getPanelHeight(){return m_panelheight;}

#if (NEW_PROGRAM == 1)

public:
	// ����λ��
	float m_LabelWidth;	// �����
	float LabelWidth() const { return m_LabelWidth; }
	void LabelWidth(float val) { m_LabelWidth = val; }

	float m_LabelHeight;	// �����
	float LabelHeight() const { return m_LabelHeight; }
	void LabelHeight(float val) { m_LabelHeight = val; }

	map<int,PointFloat>	m_LabelPosition;	// ����λ�ýṹ
	map<int,PointFloat> LabelPosition() const { return m_LabelPosition; }
	void LabelPosition(map<int,PointFloat> val) { m_LabelPosition = val; }
#endif

public:
	// �ٵ��޷���ס����ۣ��ı���ʾ�������  
	bool m_bReportMsg;				// ��Ϣ��ʾʹ��

	void EnaleMsgReport();			// ʹ����Ϣ��ʾ
	void UnaleMsgReport();			// �ر���Ϣ��ʾ

};
#endif