#include "StdAfx.h"
#include "KnifeClass.h"


#include <algorithm>
#include <iostream>
#include <fstream>
#include <afxinet.h>
#include <iphlpapi.h >
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Windows.h"
#include <map>
#include <algorithm>
#include "math.h"
#include <WinSock2.h>


// muParserDLL
#include "../../include/muParser/muParserDLL.h"


// DataManager
#include "../../include/DataManager/Algorithm/Algorithm.h"
#include "../../include/DataManager/BaseDataType/Panel/Panel.h"



// AbnormalShapeManager
#include "../../include/AbnormalShapeManager/DataManager/CommonData.h"
#include "../../include/AbnormalShapeManager/DataManager/CSingleton.h"
#include "../../include/AbnormalShapeManager/DataManager/Layer/Layer.h"
#include "../../include/AbnormalShapeManager/DataManager/Figure/Complex.h"
#include "../../include/AbnormalShapeManager/DataManager/Figure/Arc.h"
#include "../../include/AbnormalShapeManager/DataManager/Point/ShapePoint.h"
#include "../../include/AbnormalShapeManager/GeneralInterface/GeneralInterface.h"


using namespace std;
#pragma comment(lib,"Iphlpapi.lib")

// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManagerd.lib")
#pragma comment(lib, "../../lib/muParserd.lib")
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManager.lib")
#pragma comment(lib, "../../lib/muParser.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif





int KnifeClass::m_nOutputTBNum = 1;
CList<CString> KnifeClass::m_TBStringList;








// �ڲ�ʹ�ú���


















// �ж�˭�Ƚ�С
bool KnifeClass::ComponentCompareID(const Component* pfirst, const Component* psecond) 
{       
	bool flag = false;
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);

	if (p1->m_NumberInPanel < p2->m_NumberInPanel)
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	return flag;	
}




KnifeClass::KnifeClass()
{

	// ��ʼ��GCodePara
	m_gcodeparam.cutspeech = 8000;
	m_gcodeparam.distance1 = 30;
	m_gcodeparam.distance2 = 20;
	m_gcodeparam.org_offset = 0;
	m_gcodeparam.downheight = 0;
	m_gcodeparam.ender = "";
	m_gcodeparam.endspeech = 8000;
	m_gcodeparam.header = "";
	m_gcodeparam.overspeech = 8000;
	m_gcodeparam.prospeech = 8000;
	m_gcodeparam.upheight = 45;
	m_gcodeparam.strRinget = "D01";

	// ��ʼ��OptimizeParam

	m_optimizeparam.minheight = 300;
	m_optimizeparam.minwidth = 300;
	m_optimizeparam.minsize = 90000;


}

KnifeClass::~KnifeClass()
{

}

void KnifeClass::SetInputOrderNum(CString str)
{
	m_strOrderNum = str;
}


// ��ʼ����������������
void KnifeClass::ResetOutputTBNum(void)
{
	m_nOutputTBNum = 1;
}


// ���õ���
void KnifeClass::setKnifeType(std::vector<KnifeType> vecKnifeType)
{
	int nCount;

	m_knifetype.clear();
	KnifeType ktype;	// ��δ��������⣬�������ĵ���Ϊ�գ������һ��δ��ʼ���ĵ��� 

	nCount = vecKnifeType.size();
	if (nCount > 0)
	{
		for (int i = 0;i < vecKnifeType.size();i++)
		{ 
			KnifeType onetype = vecKnifeType.at(i);
			//onetype._facespeed = onetype._feedrate;
			if (onetype.kt == 0 || onetype.kt == 4)
			{
				ktype = onetype;
			}
			else
			{
				m_knifetype.push_back(onetype);
			}
		}
		m_knifetype.push_back(ktype);
	}
}

void KnifeClass::setvecchangexy(vector<float> cxy)
{ 
	m_cxy.clear();
	for (int i = 0;i < cxy.size();i++)
	{
		m_cxy.push_back(cxy.at(i));
	}
}

// ����С������������
void KnifeClass::SetSmallPanelCutTwicePara(bool enable, float small_size, float reserve_height, float second_speed)
{								
	m_bSmallPanelCutTwiceEnable = enable;
	m_SmallPanelStandardSize = small_size;	
	m_SmallPanelSecondSpeed = second_speed;	
	m_SmallPanelReserveHeight = reserve_height;
}

// ���ú�Ƚ��޲���
void KnifeClass::SetThicknessThresholdPara(float threshol_size, float reserve_height, float second_speed)
{
	m_ThicknessThresholdSize = threshol_size;			
	m_ThicknessThresholdReserveHeight = reserve_height;
	m_ThicknessThresholdSecondSpeed = second_speed;	
}

bool KnifeClass::OutputdvsGcodem(	Panel* pPanel,		string file,	string file1,			int cutprint,	
									bool doublecut,		bool usecoor,	int coortype,			int PanelNum,
									string str1,		string str2,	vector<P_Info> _pinfo,	string grouptype,		
									string file_change,	int dir,		vector<float> _df,		string f_header,		
									string f_ender,		int _merge,		string _mainktgcodeb,	string _mainktgcodee,	
									string _vecktgcodeb, string _vecktgcodee, int total_check_panel_num,	BOOL panel_comments)
{
	//vector<Coordinate> coor = m_OpKnifeCoor[panel];
	vector<gcodeCoor> _newcoor;
	//vector<gcodeCoor> hand_newcoor = m_handnewcoor[panel];

	// ��ʼ������
#if 1
	

	GcodeParam gcodeparam = m_gcodeparam;//����Gcodeʱ�����Ż�ǰ���õģ��������Ż������õ�

// 	if (hand_newcoor.size() == 0)
// 	{

	// �������������
 //		vector<Component*> CpnList;
// 		pPanel->GetAllNeedComponent(CpnList);
// 		sort(CpnList.begin(), CpnList.end(), ComponentCompareID);
// 
// 		_newcoor = pathoptimize(CpnList);
// 		_newcoor = rollsetoptimize(_newcoor);
// 	}
// 	else
// 	{
// 		_newcoor = hand_newcoor;
// 	}



		// �������������
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);
		sort(CpnList.begin(), CpnList.end(), ComponentCompareID);
		_newcoor = pathoptimize(CpnList);
		TransformMachiningInfoToOld_DownerFace(pPanel, CpnList, _newcoor, m_dallslot, m_dallvec);






	bool _havesv = false;
	for (int i = 0;i < _newcoor.size();i++)
	{
		// ����Ψһ����ID
		int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

		std::string info = _newcoor.at(i)._info;
		for (int j = 0;j < m_dallvec.size();j++)
		{
			// ����Ψһ����ID
			int one_vec_id_in_panel = m_dallvec.at(j).m_NumInPanel;

			std::string panel_info = m_dallvec.at(j).panel_info;

			//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
			if (one_coor_id_in_panel == one_vec_id_in_panel)
			{
				_havesv = true;
				break;
			}
		}

		if (_havesv == true)
		{
			break;
		}


		for (int j = 0;j < m_dallslot.size();j++)
		{
			// ����Ψһ����ID
			int one_slot_id_in_panel = m_dallslot.at(j).m_NumInPanel;

			std::string panel_info = m_dallslot.at(j).panel_info;

			//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
			if (one_coor_id_in_panel == one_slot_id_in_panel)
			{
				_havesv = true;
				break;
			}
		}
		if (_havesv == true)
		{
			break;
		}
	}
	if (_havesv == false)
	{
		return false;
	}

	DeleteFile(file.c_str());
	std::ofstream wfile(file.c_str(),std::ios::out);
	int key = GetLastError();


#endif

	CString vs_info = "";
	if (wfile)
	{

		// �ϲ���ʽ == 1
#if 1

		if (_merge == 1)
		{
			CString s_header;
			s_header.Format("%s",f_header.c_str());
			s_header.Replace(_T("\r\n"),_T("@"));
			std::string tmp_header = s_header.GetBuffer();

			for (int i = 0;i < tmp_header.size();i++)
			{
				if (tmp_header.at(i) == '@')
				{
					wfile<<endl;
				}
				else if (tmp_header.at(i) == '!')
				{
					wfile<<"@";
				}
				else
				{
					wfile<<tmp_header.at(i);
				}
			}
			wfile<<endl;
		}
		wfile<<endl;

#endif

		// ����ҳ�룬����鿴
#if 1

		if (panel_comments == TRUE)
		{
			CString strPageInfo;

			strPageInfo.Format("//(No.%d/%d, �ߴ�:%0.1f*%0.1f, ���:%0.1f, ����:%s)", PanelNum, total_check_panel_num, pPanel->m_OrgLen, pPanel->m_OrgWidth, pPanel->m_Thickness, pPanel->m_Material);

			wfile << strPageInfo << endl;
		}


#endif





		// �ռ���
#if 1


		vector<AllVec> _leftvec;


		std::vector<KnifeTypeAndVecA> pvecA;

		for (int i = 0;i < _newcoor.size();i++)
		{
			// ����Ψһ����ID
			int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

			std::string info = _newcoor.at(i)._info;

			float src_x = _newcoor.at(i)._x;
			float src_y = _newcoor.at(i)._y;

			float p_width = _newcoor.at(i)._width;
			float p_height = _newcoor.at(i)._height;

			if (dir == 1)
			{
				src_x = /*m_panelwidth*/pPanel->m_OrgLen - src_x - p_width;
			}
			else if (dir == 2)
			{
				src_y = /*m_panelheight*/pPanel->m_OrgWidth - src_y - p_height;
			}


			for (int v = 0;v < m_dallvec.size();v++)
			{
				// ����Ψһ����ID
				int one_vec_id_in_panel = m_dallvec.at(v).m_NumInPanel;

				std::string panel_info = m_dallvec.at(v).panel_info;
				float one_d = m_dallvec.at(v)._r;

				//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_vec_id_in_panel)
				{
					KnifeTypeAndVecA onevec;

					onevec.panel_info = panel_info;
					onevec._x = m_dallvec.at(v)._x;
					onevec._y = m_dallvec.at(v)._y;
					if (dir == 2)
					{
						onevec._x = p_width - onevec._x;
						onevec._y = p_height - onevec._y;
					}
					onevec._r = m_dallvec.at(v)._r;
					onevec._depth = m_dallvec.at(v)._depth;
					onevec.src_x = src_x;
					onevec.src_y = src_y;
					onevec._pheight = p_height;
					onevec._pwidth = p_width;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (_newcoor.at(i)._dir == 2)
// 					{
// 						onevec._x = _newcoor.at(i)._width - m_dallvec.at(v)._y;
// 						onevec._y = m_dallvec.at(v)._x;
// 					}

					float min_dis = 100000;
					int v_key = -1;
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{
						if (m_knifetype.at(v_j).kt != 6)
						{
							continue;
						}

						if (m_dallvec.at(v)._r - m_knifetype.at(v_j)._diameter > -0.01 && m_dallvec.at(v)._r - m_knifetype.at(v_j)._diameter < 0.01)
						{
							v_key = v_j;
							break;
						}
					}

					if (v_key == -1)
					{
						_leftvec.push_back(m_dallvec.at(v));
						continue;
					}

					onevec._diameter = m_knifetype.at(v_key)._diameter;
					onevec._tgcodeb = m_knifetype.at(v_key)._tgcodeb;
					onevec._tgcodee = m_knifetype.at(v_key)._tgcodee;
					onevec._feedrate = m_knifetype.at(v_key)._feedrate;
					onevec._spindlespeed = m_knifetype.at(v_key)._spindlespeed;
					onevec._overspeed = m_knifetype.at(v_key)._overspend;
					onevec._endspeed = m_knifetype.at(v_key)._endspeed;
					onevec._r = onevec._r - onevec._diameter;

					onevec._kx.clear();
					onevec._ky.clear();
					onevec._kn.clear();
					onevec._samedownkn.clear();

					// ��յ��߿�ʼ��������
					onevec.BeginCodeList.clear();
					onevec.EndCodeList.clear();

					onevec._kdown = true;
					onevec._coorsys = m_knifetype.at(v_key)._coorsys;

					// �ж��Ƿ�Ϊ��ͬ�Ŀף������ظ����
					bool IsThesameVec = false;
					for (int pvecA_i = 0;pvecA_i < pvecA.size();pvecA_i++)
					{
						float detection_x		= abs(pvecA.at(pvecA_i)._x		- onevec._x + pvecA.at(pvecA_i).src_x - onevec.src_x);
						float detection_y		= abs(pvecA.at(pvecA_i)._y		- onevec._y + pvecA.at(pvecA_i).src_y - onevec.src_y);
						float detection_r		= abs(pvecA.at(pvecA_i)._r		- onevec._r);
						float detection_depth	= abs(pvecA.at(pvecA_i)._depth	- onevec._depth);

						if (detection_x == 0 && 
							detection_y == 0 &&
							detection_r == 0 &&
							detection_depth == 0)
						{
							IsThesameVec = true;
							break;
						}
					}

					if (IsThesameVec == false)
					{
						pvecA.push_back(onevec);
					}
				}
			}
		}

#endif

	// �ж��Ƿ�����ϵ�
#if 1


		for (int p_i = 0;p_i < pvecA.size();p_i++)
		{
			float _d = pvecA.at(p_i)._diameter;
			KnifeTypeAndVecA&  knife_vec =  pvecA.at(p_i);

			for (int p_j = 0;p_j < m_knifetype.size();p_j++)
			{
				const KnifeType& one_knife_type = m_knifetype.at(p_j);

				if (m_knifetype.at(p_j).kt != 6)
				{
					continue;
				}

				if (one_knife_type._diameter - _d > -0.01 && one_knife_type._diameter - _d < 0.01)
				{
					knife_vec._kx.push_back(knife_vec.src_x + knife_vec._x - one_knife_type._kx);
					knife_vec._ky.push_back(knife_vec.src_y + knife_vec._y - one_knife_type._ky);
					knife_vec._kn.push_back(one_knife_type._kn);


					// ��¼����
					knife_vec.BeginCodeList.push_back(one_knife_type._tgcodeb);
					knife_vec.EndCodeList.push_back(one_knife_type._tgcodee);

				}
			}
		}

		for (int p_i = 0;p_i < pvecA.size();p_i++)
		{
			KnifeTypeAndVecA onevec;
			onevec = pvecA.at(p_i);
			int key = 0;
		}


		//�ж��Ƿ�����ϵ������û�У������ϲ�����

		bool twodownknife = false;
		for (int k_i = 0;k_i < m_knifetype.size();k_i++)
		{
			if (m_knifetype.at(k_i).kt == 7)
			{
				twodownknife = true;
				break;
			}
		}

#endif

		// �������
#if 1 


		vector<int> posselect;
		posselect.clear();
		for (int p_i = 0;p_i < pvecA.size();p_i++)
		{
			if (pvecA.at(p_i)._kdown == false)
			{
				continue;
			}

			int max_same_num = 0;
			int max_same_key = 0;
			std::vector<int> same_p;
			std::vector<int> down_kn;
			int this_kn = 0;
			float this_kx = 0.0;
			float this_ky = 0.0;

			string this_begin_code;
			string this_end_code;


			for (int p_m = 0;p_m < pvecA.at(p_i)._kn.size();p_m++)
			{
				int m_same_n = 0;
				std::vector<int> n_same_p;
				std::vector<int> n_down_kn;
				bool haveselect = false;
				for (int p_j = 0;p_j < pvecA.size();p_j++)
				{
					if (p_i == p_j || pvecA.at(p_j)._kdown == false || find( posselect.begin(), posselect.end(), p_j ) != posselect.end())
					{
						continue;
					}

					for (int p_n = 0;p_n < pvecA.at(p_j)._kn.size();p_n++)
					{
						float dis_x = pvecA.at(p_i)._kx.at(p_m) - pvecA.at(p_j)._kx.at(p_n);
						float dis_y = pvecA.at(p_i)._ky.at(p_m) - pvecA.at(p_j)._ky.at(p_n);
						float dis_d = pvecA.at(p_i)._depth - pvecA.at(p_j)._depth;
						if (dis_x > -0.01 && dis_x < 0.01 && dis_y > -0.01 && dis_y < 0.01 && dis_d > -0.01 && dis_d < 0.01)
						{
							m_same_n = m_same_n + 1;
							n_same_p.push_back(p_j);
							n_down_kn.push_back(p_n);
							haveselect = true;
							break;
						}
					}

					if (haveselect == true)
					{
						break;
					}
				}

				if (m_same_n > max_same_num)
				{
					max_same_num = m_same_n;
					max_same_key = p_m;
					same_p = n_same_p;
					down_kn = n_down_kn;
					this_kn = pvecA.at(p_i)._kn.at(p_m);
					this_kx = pvecA.at(p_i)._kx.at(p_m);
					this_ky = pvecA.at(p_i)._ky.at(p_m);


					// ��ֵ�����ĵ���
					this_begin_code = pvecA.at(p_i).BeginCodeList.at(p_m);
					this_end_code = pvecA.at(p_i).EndCodeList.at(p_m);



					posselect.push_back(p_i);
				}

				if (haveselect == true)
				{
					break;
				}
			}

			if (max_same_num > 0)
			{
				for (int p_k = 0;p_k < same_p.size();p_k++)
				{
					pvecA.at(p_i)._samedownkn.push_back(pvecA.at(same_p.at(p_k))._kn.at(down_kn.at(p_k)));
					if (twodownknife == true)
					{
						pvecA.at(same_p.at(p_k))._kdown = false;
					}
				}

				pvecA.at(p_i)._thiskn = this_kn;
				pvecA.at(p_i)._thiskx = this_kx;
				pvecA.at(p_i)._thisky = this_ky;

				pvecA.at(p_i)._tgcodeb = this_begin_code;
				pvecA.at(p_i)._tgcodee = this_end_code;


				for (int k_i = 0;k_i < m_knifetype.size();k_i++) 
				{
					if (m_knifetype.at(k_i).kt == 7)
					{
						pvecA.at(p_i)._tgcodeb = m_knifetype.at(k_i)._tgcodeb;
						pvecA.at(p_i)._tgcodee = m_knifetype.at(k_i)._tgcodee;
						break;
					}
				}
			}
			else
			{
				pvecA.at(p_i)._thiskn = pvecA.at(p_i)._kn.at(0);
				pvecA.at(p_i)._thiskx = pvecA.at(p_i)._kx.at(0);
				pvecA.at(p_i)._thisky = pvecA.at(p_i)._ky.at(0);
			}
		}




		std::vector<pos> poses;
		std::vector<pos> newposes;

		int key = pvecA.size();

		for (int p_i = 0;p_i < pvecA.size();p_i++)
		{
			if (pvecA.at(p_i)._kdown == false)
			{
				continue;
			}

			pos one_pos;
			one_pos.x = pvecA.at(p_i)._thiskx;
			one_pos.y = pvecA.at(p_i)._thisky;
			poses.push_back(one_pos);
		}

		pos begin_pos;
		begin_pos.x = 0;
		begin_pos.y = 0;
		if (coortype == 1)
		{
			begin_pos.x = 0;
			begin_pos.y = m_panelheight;
		}
		else if (coortype == 2)
		{
			begin_pos.x = 0;
			begin_pos.y = -1.0*m_panelheight;
		}

		GetShortestPath(poses,begin_pos,newposes);
		poses.clear();
		for (int p_i = 0;p_i < newposes.size();p_i++)
		{
			poses.push_back(newposes.at(p_i));
		}

		newposes.clear();
		GetChangePath(poses,newposes);

		// 2017-3-30 yuanzb �˴����ܳ����ظ��Ŀ���ӣ���ԭ����ע��
		//			����ͬ��x��y�����Ž����жϣ���δȷ�����޸��Ƿ�������Ӱ��

		std::vector<KnifeTypeAndVecA> tmppvec;
		for (int p_j = 0;p_j < newposes.size();p_j++)
		{
			pos cur_pos = newposes.at(p_j);

			for (int p_i = 0;p_i < pvecA.size();p_i++)
			{
				KnifeTypeAndVecA cur_kife_vec = pvecA.at(p_i);

				if (cur_kife_vec._kdown == false)
				{
					continue;
				}

				if (cur_pos.x - (cur_kife_vec._thiskx) > -0.01	&&
					cur_pos.x - (cur_kife_vec._thiskx) < 0.01	&& 
					cur_pos.y - (cur_kife_vec._thisky) > -0.01	&& 
					cur_pos.y - (cur_kife_vec._thisky) < 0.01	)
				{
					// ����Ƿ����ظ�����
					BOOL have_insert_flag = FALSE;

					for (int i = 0; i < tmppvec.size(); i++)
					{
						KnifeTypeAndVecA inser_kife_vec = tmppvec.at(i);

						if (inser_kife_vec._thiskx == cur_kife_vec._thiskx &&
							inser_kife_vec._thisky == cur_kife_vec._thisky &&
							inser_kife_vec._thiskn == cur_kife_vec._thiskn)
						{
							have_insert_flag = TRUE;
						}
					}


					if (have_insert_flag == FALSE)
					{
						tmppvec.push_back(cur_kife_vec);
					}

				}
			}
		}

#endif

		// �������
#if 1



		pvecA.clear();
		for (int p_i = 0;p_i < tmppvec.size();p_i++)
		{
			pvecA.push_back(tmppvec.at(p_i));
		}

		if (pvecA.size() > 0)
		{
			wfile<<_vecktgcodeb.c_str()<<endl;
		}

		std::string old_tgcodeb = "";
		std::string old_tgcodee = "";
		for (int v_i = 0;v_i < pvecA.size();v_i++)
		{
			float _d = pvecA.at(v_i)._diameter;
			float _thx = pvecA.at(v_i)._thiskx;
			float _thy = pvecA.at(v_i)._thisky;
			KnifeTypeAndVecA tmpon = pvecA.at(v_i);

			std::string _tgcodeb = pvecA.at(v_i)._tgcodeb;
			std::string _tgcodee = pvecA.at(v_i)._tgcodee;
			if (pvecA.at(v_i)._samedownkn.size() > 0)
			{
				int _sx,_sy;
				_sx = pvecA.at(v_i)._thiskn;
				_sy = pvecA.at(v_i)._samedownkn.at(0);	// ��ϵ�
				std::string s_samedownkn = ChangeGroup(_sx,_sy,grouptype);

				_tgcodeb = replace_allstr(_tgcodeb,"*",s_samedownkn);
				_tgcodee = replace_allstr(_tgcodee,"*",s_samedownkn);

				std::string _coorsys = "";
				for (int v_k = 0;v_k < m_knifetype.size();v_k++)
				{
					if (m_knifetype.at(v_k)._kn == pvecA.at(v_i)._thiskn)
					{
						_coorsys = m_knifetype.at(v_k)._coorsys;
						if (usecoor == true)
						{
							pvecA.at(v_i)._thiskx = pvecA.at(v_i)._thiskx + m_knifetype.at(v_k)._kx;
							pvecA.at(v_i)._thisky = pvecA.at(v_i)._thisky + m_knifetype.at(v_k)._ky;
						}

						// �滻$����
						_coorsys = m_knifetype.at(v_k)._coorsys;
						_tgcodeb = replace_allstr(_tgcodeb,"$",_coorsys);
						_tgcodee = replace_allstr(_tgcodee,"$",_coorsys);
					}
				}
			}
			else
			{
				for (int v_k = 0;v_k < m_knifetype.size();v_k++)
				{
					if (m_knifetype.at(v_k)._kn == pvecA.at(v_i)._thiskn)
					{
						if (usecoor == true)
						{
							float kx = m_knifetype.at(v_k)._kx;
							float ky = m_knifetype.at(v_k)._ky;

							float tkx = pvecA.at(v_i)._thiskx;
							float tky = pvecA.at(v_i)._thisky;

							pvecA.at(v_i)._thiskx = pvecA.at(v_i)._thiskx + m_knifetype.at(v_k)._kx;
							pvecA.at(v_i)._thisky = pvecA.at(v_i)._thisky + m_knifetype.at(v_k)._ky;
						}
					}
				}
			}

			// �Ա����ϵ��߿�ʼ����
			if (strcmp(_tgcodeb.c_str(),old_tgcodeb.c_str()) != 0)
			{
				for (int _g_i = 0;_g_i < _tgcodeb.size();_g_i++)
				{
					if (_tgcodeb.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_tgcodeb.at(_g_i);
					}
				}
			}

			old_tgcodeb = _tgcodeb;
			wfile<<endl;

			float v_x = pvecA.at(v_i)._thiskx;
			float v_y = pvecA.at(v_i)._thisky;
			if (coortype == 1)
			{
				v_y = (v_y - pvecA.at(v_i).src_y)*(-1.0) + pvecA.at(v_i).src_y;
			}
			else if (coortype == 2)
			{
				v_y = (v_y - pvecA.at(v_i).src_y)*(-1.0) + pvecA.at(v_i).src_y;
			}

			float v_depth;

			if (gcodeparam.downheight > 1000)
			{
				v_depth = -1* pvecA.at(v_i)._depth;
			}
			else
			{
				v_depth = _thickness -  pvecA.at(v_i)._depth;
				if (gcodeparam.downheight > 0.5)
				{
					v_depth = v_depth - gcodeparam.downheight;
				}
			}



			char chheight1[12],chheight2[12],chheight3[12];
			sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
			sprintf_s(chheight2,"%0.4f",v_depth);
			if (gcodeparam.downheight > 1000)
			{
				sprintf_s(chheight3,"%0.4f",0.0000);
			}
			else
			{
				sprintf_s(chheight3,"%0.4f",_thickness);
				if (gcodeparam.downheight > 0.5)
				{
					sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
				}
			}


			char chrx1[12],chry1[12];
			sprintf_s(chrx1,"%0.4f",v_x);
			sprintf_s(chry1,"%0.4f",v_y);

			if (coortype == 0)
			{
				wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1<<endl;
				wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3<<" F "<<pvecA.at(v_i)._spindlespeed<<endl;
				wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<pvecA.at(v_i)._feedrate<<endl;
				wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
			}
			else if (coortype == 1)
			{
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<pvecA.at(v_i)._spindlespeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<pvecA.at(v_i)._feedrate<<endl;
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
			}
			else if (coortype == 2)
			{
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<pvecA.at(v_i)._spindlespeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<pvecA.at(v_i)._feedrate<<endl;
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
			}

			// 2016-11-1 yuanzb ��������NC ��ͬ�ĵ�֮���д�˽�������	
			int nPrevASize = pvecA.size();				// ������
			string strCurKinefBeginCode = _tgcodeb;		// ��ǰ������ʼ����
			string strNextKinefBeginCode = "";			// ��һ��������ʼ����

			if (v_i != pvecA.size() -1)
			{
				KnifeTypeAndVecA NextVecA =  pvecA.at(v_i+1);
				strNextKinefBeginCode = NextVecA._tgcodeb;

				if (NextVecA._samedownkn.size() > 0)
				{
					// �滻��ϵ�
					int _sx,_sy;
					_sx = NextVecA._thiskn;
					_sy = NextVecA._samedownkn.at(0);	// ��ϵ�
					std::string s_samedownkn = ChangeGroup(_sx,_sy,grouptype);

					strNextKinefBeginCode = replace_allstr(strNextKinefBeginCode,"*",s_samedownkn);


					// 2017-4-19 yuanzb �滻$��
					for (int v_k = 0;v_k < m_knifetype.size();v_k++)
					{
						if (m_knifetype.at(v_k)._kn == pvecA.at(v_i+1)._thiskn)
						{	
							KnifeType tm_type = m_knifetype.at(v_k);
							std::string _coorsys = m_knifetype.at(v_k)._coorsys;

							if (usecoor == true)
							{
								_coorsys = m_knifetype.at(v_k)._coorsys;

								strNextKinefBeginCode = replace_allstr(strNextKinefBeginCode,"$",_coorsys);
							}
						}
					}
				}
			}
			
			// ����ǰ����һ��������ʼ������ͬ������Ҫд���߽�������
			if (strCurKinefBeginCode.compare(strNextKinefBeginCode) == 0)
			{
				wfile<<endl;
			}
			else
			{
				for (int _g_i = 0;_g_i < _tgcodee.size();_g_i++)
				{
					if (_tgcodee.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_tgcodee.at(_g_i);
					}
				}
				wfile<<endl;
			}
			wfile<<endl;

		}

		if (pvecA.size() > 0)
		{
			wfile<<_vecktgcodee.c_str()<<endl;
		}

		wfile<<_mainktgcodeb.c_str()<<endl;
		


#endif

		//���ᵶ�������
#if 1


		std::vector<KnifeTypeAndVec> pvec;
		std::vector<KnifeTypeAndSlot> p_solt;
		for (int i = 0;i < _newcoor.size();i++)
		{
			// ����Ψһ����ID
			int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

			std::string info = _newcoor.at(i)._info;
			float p_width = _newcoor.at(i)._width;
			float p_height = _newcoor.at(i)._height;

			float src_x = _newcoor.at(i)._x;
			float src_y = _newcoor.at(i)._y;

			if (dir == 1)
			{
				src_x = /*m_panelwidth*/pPanel->m_OrgLen - src_x - p_width;
			}
			else if (dir == 2)
			{
				src_y = /*m_panelheight*/pPanel->m_OrgWidth - src_y - p_height;
			}

			for (int v = 0;v < _leftvec.size();v++)
			{

				// ����Ψһ����ID
				int one_vec_id_in_panel = _leftvec.at(v).m_NumInPanel;

				std::string panel_info = _leftvec.at(v).panel_info;
				float one_d = _leftvec.at(v)._r;

				//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_vec_id_in_panel)
				{
					KnifeTypeAndVec onevec;

					onevec.panel_info = panel_info;
					onevec._pwidth = p_width;
					onevec._pheight = p_height;
					onevec._x = _leftvec.at(v)._x;
					onevec._y = _leftvec.at(v)._y;
					if (dir == 2)
					{
						onevec._x = p_width - onevec._x;
						onevec._y = p_height - onevec._y;
					}
					onevec._r = _leftvec.at(v)._r;
					onevec._depth = _leftvec.at(v)._depth;
					onevec._srcx = src_x;
					onevec._srcy = src_y;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (_newcoor.at(i)._dir == 2)
// 					{
// 						onevec._x = _newcoor.at(i)._width - _leftvec.at(v)._y;
// 						onevec._y = _leftvec.at(v)._x;
// 					}

					float min_dis = 100000;
					int v_key = -1;
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{
						if (m_knifetype.at(v_j).kt != 0 && m_knifetype.at(v_j).kt != 1 && m_knifetype.at(v_j).kt != 2)
						{
							continue;
						}

						if (_leftvec.at(v)._r - m_knifetype.at(v_j)._diameter >= 0.0 && min_dis > (_leftvec.at(v)._r - m_knifetype.at(v_j)._diameter))
						{
							min_dis = _leftvec.at(v)._r - m_knifetype.at(v_j)._diameter;
							v_key = v_j;
						}
					}

					if (v_key == -1)
					{
						CString info;
						info.Format("�����%s��Ӧ��λֱ����%0.1f�޷����\r\n",panel_info.c_str(),one_d);
						//AfxMessageBox(info);
						vs_info = vs_info + info;
						continue;
					}

					onevec._diameter = m_knifetype.at(v_key)._diameter;
					onevec._tgcodeb = m_knifetype.at(v_key)._tgcodeb;
					onevec._tgcodee = m_knifetype.at(v_key)._tgcodee;
					onevec._feedrate = m_knifetype.at(v_key)._feedrate;
					onevec._spindlespeed = m_knifetype.at(v_key)._spindlespeed;
					onevec._r = onevec._r - onevec._diameter;


					// �����ظ���
					bool IsThesameVec = false;
					for (int pvec_i = 0; pvec_i < pvec.size(); pvec_i++)
					{
						float detection_x		= abs(pvec.at(pvec_i)._x - onevec._x + pvec.at(pvec_i)._srcx - onevec._srcx);
						float detection_y		= abs(pvec.at(pvec_i)._y - onevec._y + pvec.at(pvec_i)._srcy - onevec._srcy);
						float detection_r		= abs(pvec.at(pvec_i)._r - onevec._r);
						float detection_depth	= abs(pvec.at(pvec_i)._depth - onevec._depth);

						if (detection_x == 0 && 
							detection_y == 0 &&
							detection_r == 0 &&
							detection_depth == 0)
						{
							IsThesameVec = true;
							break;
						}
					}

					if (IsThesameVec == false)
					{
						pvec.push_back(onevec);
					}

					//pvec.push_back(onevec);
				}
			}

		


			for (int s = 0;s < m_dallslot.size();s++)
			{
				// ����Ψһ����ID
				int one_slot_id_in_panel = m_dallslot.at(s).m_NumInPanel;

				std::string panel_info = m_dallslot.at(s).panel_info;

				//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_slot_id_in_panel)
				{
					KnifeTypeAndSlot oneks;
					oneks.panel_info = panel_info;
					oneks._pwidth = p_width;
					oneks._pheight = p_height;
					oneks._depth = m_dallslot.at(s)._depth;
					oneks._dir = m_dallslot.at(s)._dir;
					oneks._width = m_dallslot.at(s)._width;
					oneks._height = m_dallslot.at(s)._height;
					oneks._x = m_dallslot.at(s)._x;
					oneks._y = m_dallslot.at(s)._y;
					if (dir == 2)
					{
						oneks._x = p_width - oneks._x;
						oneks._y = p_height - oneks._y;
					}

					oneks._srcx = src_x;
					oneks._srcy = src_y;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (_newcoor.at(i)._dir == 2)
// 					{
// 						oneks._x = _newcoor.at(i)._width - m_dallslot.at(s)._y;
// 						oneks._y = m_dallslot.at(s)._x;
// 						oneks._dir = 1 - oneks._dir;
// 					}

					float min_dis = 100000;
					int v_key = -1;
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{
						if (m_knifetype.at(v_j).kt != 0 && m_knifetype.at(v_j).kt != 1 && m_knifetype.at(v_j).kt != 3 && m_knifetype.at(v_j).kt != 5)
						{
							continue;
						}
						if (m_dallslot.at(s)._height - m_knifetype.at(v_j)._diameter >= 0.0 && min_dis > (m_dallslot.at(s)._height - m_knifetype.at(v_j)._diameter))
						{
							min_dis = m_dallslot.at(s)._height - m_knifetype.at(v_j)._diameter;
							v_key = v_j;
						}
					}

					if (v_key == -1)
					{
						CString info;
						info.Format("�����%s��Ӧ�ۿ�%0.1f�޷�����\r\n",panel_info.c_str(),oneks._height);
						//AfxMessageBox(info);
						vs_info = vs_info + info;
						continue;
					}

					oneks._diameter = m_knifetype.at(v_key)._diameter;
					oneks._tgcodeb = m_knifetype.at(v_key)._tgcodeb;
					oneks._tgcdoee = m_knifetype.at(v_key)._tgcodee;
					oneks._feedrate = m_knifetype.at(v_key)._feedrate;
					oneks._spindlespeed = m_knifetype.at(v_key)._spindlespeed;

					bool IsThesameVec = false;
					for (int solt_i = 0; solt_i < p_solt.size(); solt_i++)
					{
						KnifeTypeAndSlot ExistSolt = p_solt.at(solt_i);

						int ExistSoltDir		= ExistSolt._dir;
						float detection_x		= abs(ExistSolt._x - oneks._x + ExistSolt._srcx - oneks._srcx);
						float detection_y		= abs(ExistSolt._y - oneks._y + ExistSolt._srcy - oneks._srcy);
						float detection_width	= abs(ExistSolt._width	- oneks._width);
						float detection_height	= abs(ExistSolt._height	- oneks._height);
						float detection_depth	= abs(ExistSolt._depth	- oneks._depth);

						if (detection_x == 0			&&
							detection_y == 0			&&
							ExistSoltDir == oneks._dir	&&
							detection_width == 0		&&
							detection_height == 0		&&
							detection_depth == 0  )
						{
							IsThesameVec = true;
							break;
						}
					}
					if (IsThesameVec == false)
					{
						p_solt.push_back(oneks);
					}


					//p_solt.push_back(oneks);
				}
			}
		}


#endif

#if 1

		// ��ʼ�������
		float _k_d = -1.0;
		KnifeType last_knife;

		// ��ʼ��
		last_knife.kt = -1;
		last_knife._diameter =  -1;
		last_knife._kn = -1;


		for (int v_i = 0;v_i < m_knifetype.size();v_i++)
		{
			if (m_knifetype.at(v_i).kt == 6 || m_knifetype.at(v_i).kt == 7)
			{
				continue;
			}

			float _d = m_knifetype.at(v_i)._diameter;
			std::string _tgcodeb = m_knifetype.at(v_i)._tgcodeb;
			std::string _tgcodee = m_knifetype.at(v_i)._tgcodee;
			std::vector<AllVecK> _vec;
			for (int v_j = 0;v_j < pvec.size();v_j++)
			{
				if (pvec.at(v_j)._diameter - _d > -0.01 && pvec.at(v_j)._diameter - _d < 0.01 && (m_knifetype.at(v_i).kt == 0 || m_knifetype.at(v_i).kt == 1 || m_knifetype.at(v_i).kt == 2))
				{
					AllVecK onevec;
					onevec.panel_info = pvec.at(v_j).panel_info;
					onevec._pheight = pvec.at(v_j)._pheight;
					onevec._pwidth = pvec.at(v_j)._pwidth;
					onevec._depth = pvec.at(v_j)._depth;
					onevec._r = pvec.at(v_j)._r;
					onevec._x = pvec.at(v_j)._x;
					onevec._y = pvec.at(v_j)._y;
					onevec._srcx = pvec.at(v_j)._srcx;
					onevec._srcy = pvec.at(v_j)._srcy;

					_vec.push_back(onevec);
				}
			}

			std::vector<AllSlotK> _slot;
			for (int s_j = 0;s_j < p_solt.size();s_j++)
			{
				if (p_solt.at(s_j)._diameter - _d > -0.01 && p_solt.at(s_j)._diameter - _d < 0.01 && (m_knifetype.at(v_i).kt == 0 || m_knifetype.at(v_i).kt == 1 || m_knifetype.at(v_i).kt == 3 ||  m_knifetype.at(v_i).kt == 5))
				{
					AllSlotK onesolt;
					onesolt.panel_info = p_solt.at(s_j).panel_info;
					onesolt._pheight = p_solt.at(s_j)._pheight;
					onesolt._pwidth = p_solt.at(s_j)._pwidth;
					onesolt._depth = p_solt.at(s_j)._depth;
					onesolt._dir = p_solt.at(s_j)._dir;
					onesolt._height = p_solt.at(s_j)._height;
					onesolt._width = p_solt.at(s_j)._width;
					onesolt._x = p_solt.at(s_j)._x;
					onesolt._y = p_solt.at(s_j)._y;
					onesolt._srcx = p_solt.at(s_j)._srcx;
					onesolt._srcy = p_solt.at(s_j)._srcy;

					_slot.push_back(onesolt);
				}
			}

			if (_vec.size() > 0 || _slot.size() > 0)
			{
				for (int _g_i = 0;_g_i < _tgcodeb.size();_g_i++)
				{
					if (_tgcodeb.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_tgcodeb.at(_g_i);
					}
				}
				_k_d = m_knifetype.at(v_i)._diameter;

				wfile<<endl;
			}


			// ���
			if (_vec.size() > 0)
			{
				std::vector<pos> poses;
				std::vector<pos> newposes;
				for (int p_i = 0;p_i < _vec.size();p_i++)
				{
					pos one_pos;
					one_pos.x = _vec.at(p_i)._x + _vec.at(p_i)._srcx;
					one_pos.y = _vec.at(p_i)._y + _vec.at(p_i)._srcy;
					poses.push_back(one_pos);
				}

				pos begin_pos;
				begin_pos.x = 0;
				begin_pos.y = 0;
				GetShortestPath(poses,begin_pos,newposes);
				poses.clear();
				for (int p_i = 0;p_i < newposes.size();p_i++)
				{
					poses.push_back(newposes.at(p_i));
				}
				newposes.clear();
				GetChangePath(poses,newposes);
				std::vector<AllVecK> tmppvec;
				for (int p_j = 0;p_j < newposes.size();p_j++)
				{
					for (int p_i = 0;p_i < _vec.size();p_i++)
					{
						if (newposes.at(p_j).x - (_vec.at(p_i)._x + _vec.at(p_i)._srcx) > -0.01 && newposes.at(p_j).x - (_vec.at(p_i)._x + _vec.at(p_i)._srcx) < 0.01 && newposes.at(p_j).y - (_vec.at(p_i)._y + _vec.at(p_i)._srcy) > -0.01 && newposes.at(p_j).y - (_vec.at(p_i)._y + _vec.at(p_i)._srcy) < 0.01 )
						{
							tmppvec.push_back(_vec.at(p_i));
						}
					}
				}

				_vec.clear();
				for (int p_i = 0;p_i < tmppvec.size();p_i++)
				{
					_vec.push_back(tmppvec.at(p_i));
				}

				for (int v_o = 0;v_o < _vec.size();v_o++)
				{
					float v_x = _vec.at(v_o)._x + _vec.at(v_o)._srcx;
					float v_y = _vec.at(v_o)._y + _vec.at(v_o)._srcy;
					if (coortype == 1)
					{
						v_y = -1*_vec.at(v_o)._y + _vec.at(v_o)._srcy;
					}
					else if (coortype == 2)
					{
						v_y = -1*_vec.at(v_o)._y + _vec.at(v_o)._srcy;
					}
					float v_r = _vec.at(v_o)._r;
					float v_depth;

					if (gcodeparam.downheight > 1000)
					{
						v_depth = -1* _vec.at(v_o)._depth;
						
					}
					else
					{
						v_depth = _thickness -  _vec.at(v_o)._depth;
						if (gcodeparam.downheight > 0.5)
						{
							v_depth = v_depth - gcodeparam.downheight;
						}
					}



					char chheight1[12],chheight2[12],chheight3[12];

					sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
					sprintf_s(chheight2,"%0.4f",v_depth);



					if (gcodeparam.downheight > 1000)
					{
						sprintf_s(chheight3,"%0.4f",0.0000);
					}
					else
					{
						sprintf_s(chheight3,"%0.4f",_thickness);
						if (gcodeparam.downheight > 0.5)
						{
							sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
						}
					}

					if (_vec.at(v_o)._r > -0.01 && _vec.at(v_o)._r < 0.01)
					{
						char chrx1[12],chry1[12];
						sprintf_s(chrx1,"%0.4f",v_x);
						sprintf_s(chry1,"%0.4f",v_y);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}

					}
					else if ((_vec.at(v_o)._r - 0.6)/_d <= 1.0 || (_vec.at(v_o)._depth - _thickness > -0.01 &&  _vec.at(v_o)._depth - _thickness < 0.01))
					{
						char chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];

						sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry1,"%0.4f",v_y);
						sprintf_s(chrx2,"%0.4f",v_x);
						sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						sprintf_s(chry3,"%0.4f",v_y);
						sprintf_s(chrx4,"%0.4f",v_x);
						sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry5,"%0.4f",v_y);
						sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else if ((_vec.at(v_o)._r - 0.6)/_d > 1.0 && (_vec.at(v_o)._r - 0.6)/_d <= 2)
					{
						char chrx0[12],chry0[12],chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];

						sprintf_s(chrx0,"%0.4f",v_x);
						sprintf_s(chry0,"%0.4f",v_y);
						sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry1,"%0.4f",v_y);
						sprintf_s(chrx2,"%0.4f",v_x);
						sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						sprintf_s(chry3,"%0.4f",v_y);
						sprintf_s(chrx4,"%0.4f",v_x);
						sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry5,"%0.4f",v_y);
						sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else if ((_vec.at(v_o)._r - 0.6)/_d > 2 && (_vec.at(v_o)._r - 0.6)/_d <= 3)
					{
						char chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];
						char chrx12[12],chry12[12],chrx22[12],chry22[12],chrx32[12],chry32[12],chrx42[12],chry42[12],chrx52[12],chry52[12],chr2[12];

						sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry1,"%0.4f",v_y);
						sprintf_s(chrx2,"%0.4f",v_x);
						sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						sprintf_s(chry3,"%0.4f",v_y);
						sprintf_s(chrx4,"%0.4f",v_x);
						sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						sprintf_s(chry5,"%0.4f",v_y);
						sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

						sprintf_s(chrx12,"%0.4f",v_x - _d/2.0);
						sprintf_s(chry12,"%0.4f",v_y);
						sprintf_s(chrx22,"%0.4f",v_x);
						sprintf_s(chry22,"%0.4f",v_y + _d/2.0);
						sprintf_s(chrx32,"%0.4f",v_x + _d/2.0);
						sprintf_s(chry32,"%0.4f",v_y);
						sprintf_s(chrx42,"%0.4f",v_x);
						sprintf_s(chry42,"%0.4f",v_y - _d/2.0);
						sprintf_s(chrx52,"%0.4f",v_x - _d/2.0);
						sprintf_s(chry52,"%0.4f",v_y);
						sprintf_s(chr2,"%0.4f",_d/2.0);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry22<<" Y "<<chrx22<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry32<<" Y "<<chrx32<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry42<<" Y "<<chrx42<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry52<<" Y "<<chrx52<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx22<<" Y "<<chry22<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx32<<" Y "<<chry32<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx42<<" Y "<<chry42<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx52<<" Y "<<chry52<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx22<<" Y "<<chry22<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx32<<" Y "<<chry32<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx42<<" Y "<<chry42<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx52<<" Y "<<chry52<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else
					{
						char chrx[12],chry[12];
						sprintf_s(chrx,"%0.4f",v_x);
						sprintf_s(chry,"%0.4f",v_y);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight1<<endl;
							wfile<<"G01"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx<<" Y "<<chry<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
						}

						float begin_x,begin_y,end_x,end_y,last_x,last_y;
						char chbegin_x1[12],chbegin_y1[12],chend_x1[12],chend_y1[12],chr[12],chbegin_x2[12],chbegin_y2[12],chend_x2[12],chend_y2[12];
						begin_x = v_x;
						begin_y = v_y;

						float dis_r = 0.75*_d/2.0;

						for (int r_i = 1;r_i < 100;r_i++)
						{
							if (r_i%2 == 1)
							{
								end_x = begin_x + dis_r * r_i * 2.0;
								end_y = begin_y;
								last_x = v_x + dis_r;
								last_y = v_y;
								begin_x = end_x;
								begin_y = end_y;

								if (end_x > v_x + _vec.at(v_o)._r/2.0 || end_x < v_x - _vec.at(v_o)._r/2.0)
								{
									float intersection_x,intersection_y;
									intersection_x = (((dis_r * r_i)*(dis_r * r_i) - (_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0))/(v_x - last_x) + v_x + last_x)/2.0;
									if (r_i%2 == 1)
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x)) + v_y;
									}
									else
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x))*(-1) + v_y;
									}
									sprintf_s(chend_x1,"%0.4f",intersection_x);
									sprintf_s(chend_y1,"%0.4f",intersection_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									sprintf_s(chend_x1,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",intersection_x);
									sprintf_s(chend_y1,"%0.4f",intersection_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else
								{
									sprintf_s(chend_x1,"%0.4f",last_x);
									sprintf_s(chend_y1,"%0.4f",last_y + dis_r * r_i);
									sprintf_s(chend_x2,"%0.4f",last_x + dis_r * r_i);
									sprintf_s(chend_y2,"%0.4f",last_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
								}


							}
							else
							{
								end_x = begin_x - dis_r * r_i * 2.0;
								end_y = begin_y;
								last_x = v_x;
								last_y = v_y;
								begin_x = end_x;
								begin_y = end_y;

								if (_vec.at(v_o)._r/2.0 - dis_r * r_i > -0.01 && _vec.at(v_o)._r/2.0 - dis_r * r_i < 0.01)
								{
									sprintf_s(chend_x1,"%0.4f",last_x);
									sprintf_s(chend_y1,"%0.4f",last_y - dis_r * r_i);
									sprintf_s(chend_x2,"%0.4f",last_x - dis_r * r_i);
									sprintf_s(chend_y2,"%0.4f",last_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",last_x);
									sprintf_s(chend_y1,"%0.4f",last_y + dis_r * r_i);
									sprintf_s(chend_x2,"%0.4f",last_x + dis_r * r_i);
									sprintf_s(chend_y2,"%0.4f",last_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else if (end_x > v_x + _vec.at(v_o)._r/2.0 || end_x < v_x - _vec.at(v_o)._r/2.0)
								{
									float intersection_x,intersection_y;
									intersection_x = (((dis_r * r_i)*(dis_r * r_i) - (_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0))/(v_x - last_x) + v_x + last_x)/2.0;
									if (r_i%2 == 1)
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x)) + v_y;
									}
									else
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x))*(-1) + v_y;
									}
									sprintf_s(chend_x1,"%0.4f",intersection_x);
									sprintf_s(chend_y1,"%0.4f",intersection_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",intersection_x);
									sprintf_s(chend_y1,"%0.4f",intersection_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									sprintf_s(chend_x1,"%0.4f",v_x);
									sprintf_s(chend_y1,"%0.4f",v_y);
									sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else
								{
									sprintf_s(chend_x1,"%0.4f",last_x);
									sprintf_s(chend_y1,"%0.4f",last_y - dis_r * r_i);
									sprintf_s(chend_x2,"%0.4f",last_x - dis_r * r_i);
									sprintf_s(chend_y2,"%0.4f",last_y);
									sprintf_s(chr,"%0.4f",dis_r * r_i);
									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
								}
							}

						}
					}

				}
			}


			// ����
			if (_slot.size() > 0)
			{
				for (int p_s = 0;p_s < _slot.size();p_s++)
				{
					float s_x = _slot.at(p_s)._x;
					float s_y = _slot.at(p_s)._y;
					if (coortype != 0)
					{
						s_y = -1*s_y;
					}
					float s_width = _slot.at(p_s)._width;
					float s_height = _slot.at(p_s)._height;
					float s_depth;
					if (gcodeparam.downheight > 1000)
					{
						s_depth = -1*_slot.at(p_s)._depth;
					}
					else
					{
						s_depth = _thickness - _slot.at(p_s)._depth;
						if (gcodeparam.downheight > 0.5)
						{
							s_depth = s_depth - gcodeparam.downheight;
						}
					}

					int s_dir = _slot.at(p_s)._dir;

					char chheight1[12],chheight2[12],chheight3[12];
					sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
					sprintf_s(chheight2,"%0.4f",s_depth);
					if (gcodeparam.downheight > 1000)
					{
						sprintf_s(chheight3,"%0.4f",0.0000);
					}
					else
					{
						sprintf_s(chheight3,"%0.4f",_thickness);
						if (gcodeparam.downheight > 0.5)
						{
							sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
						}
					}

					if (s_dir == 1)		//���򿪲�
					{
						//ֻ��һ�²ۣ���ֱ���Ͳۿ�һ��
						if(_d - s_height > -0.01 && _d - s_height < 0.01)
						{
							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float begin_x,begin_y,end_x,end_y;
							begin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 + _d/2.0;
							begin_y = s_y + _slot.at(p_s)._srcy;
							end_x = s_x + _slot.at(p_s)._srcx + s_width/2.0 - _d/2.0;
							end_y = s_y + _slot.at(p_s)._srcy;

							sprintf_s(chbegin_x,"%0.4f",begin_y);
							sprintf_s(chbegin_y,"%0.4f",begin_x);
							sprintf_s(chend_x,"%0.4f",end_y);
							sprintf_s(chend_y,"%0.4f",end_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G00"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							}
							else if (coortype == 1)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G00"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							}
							else if (coortype == 2)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G00"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							}
						}
						else
						{
							int big_kerfwidth = (int)(_d * 10 + 0.5);
							int big_sheight = (int)(s_height * 10 + 0.5);

							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float sbegin_x,sbegin_y,send_x,send_y;

							float begin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 + _d/2.0;
							float begin_y = s_y - s_height/2.0 + _d/2.0 + _slot.at(p_s)._srcy;

							sprintf_s(chbegin_x,"%0.4f",begin_y);
							sprintf_s(chbegin_y,"%0.4f",begin_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							else if (coortype == 1)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							else if (coortype == 2)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							int s_key = 0;
							for (int s_k = 0;s_k < big_sheight/big_kerfwidth + (big_sheight%big_kerfwidth != 0 ? 1 : 0);s_k++)
							{
								s_key = s_k;
								char schbegin_x[12],schbegin_y[12],schend_x[12],schend_y[12];

								sbegin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 + _d/2.0;
								sbegin_y = s_y - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2.0 : _d/2.0 + s_k*_d) + _slot.at(p_s)._srcy;
								send_x = s_x + s_width/2.0 + _slot.at(p_s)._srcx - _d/2.0;
								send_y = s_y - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2.0 : _d/2.0 + s_k*_d) + _slot.at(p_s)._srcy;

								sprintf_s(chbegin_x,"%0.4f",sbegin_y);
								sprintf_s(chbegin_y,"%0.4f",sbegin_x);
								sprintf_s(chend_x,"%0.4f",send_y);
								sprintf_s(chend_y,"%0.4f",send_x);

								if (s_k%2 == 0)
								{
									if (s_k != 0)
									{
										float pre_y = s_y - s_height/2 + _d/2.0 + s_k*_d + _slot.at(p_s)._srcy - _d*2.0;
										char pre_sx[12];
										sprintf_s(pre_sx,"%0.4f",pre_y);
										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 1)
										{
											wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 2)
										{
											wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
									}
									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										sprintf_s(chend_x,"%0.4f",send_y);
										sprintf_s(chend_y,"%0.4f",send_x);
									}
								}
								else
								{
									if (s_k != 1)
									{
										float pre_y = s_y - s_height/2 + _d/2.0 + s_k*_d + _slot.at(p_s)._srcy - _d*2.0;
										char pre_sx[12];
										sprintf_s(pre_sx,"%0.4f",pre_y);
										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 1)
										{
											wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 2)
										{
											wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
									}
									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										sprintf_s(chend_x,"%0.4f",sbegin_y);
										sprintf_s(chend_y,"%0.4f",sbegin_x);
									}
								}

							}
							if (s_key%2 == 0 && s_key != 0)
							{
								float pre_y = s_y - s_height/2 + _d/2.0 + s_key*_d + _slot.at(p_s)._srcy - _d*2.0;
								char pre_sx[12];
								sprintf_s(pre_sx,"%0.4f",pre_y);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}
							if (s_key%2 == 1 && s_key != 1)
							{
								float pre_y = s_y - s_height/2 + _d/2.0 + s_key*_d + _slot.at(p_s)._srcy - _d*2.0;
								char pre_sx[12];
								sprintf_s(pre_sx,"%0.4f",pre_y);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}
							if (s_key == 1)
							{
								float pre_y = s_y - s_height/2 + ((0 + 1)*_d > s_height ? s_height - _d/2 : _d/2) + _slot.at(p_s)._srcy;
								char pre_sx[12];
								sprintf_s(pre_sx,"%0.4f",pre_y);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}
						}
					}
					else				//���򿪲�
					{
						if(_d - s_height > -0.01 && _d - s_height < 0.01)
						{
							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float begin_x,begin_y,end_x,end_y;
							begin_x = s_x + _slot.at(p_s)._srcx;
							begin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 + _d/2.0;
							end_x = s_x + _slot.at(p_s)._srcx;
							end_y = s_y + _slot.at(p_s)._srcy + s_width/2.0 - _d/2.0;

							sprintf_s(chbegin_x,"%0.4f",begin_y);
							sprintf_s(chbegin_y,"%0.4f",begin_x);
							sprintf_s(chend_x,"%0.4f",end_y);
							sprintf_s(chend_y,"%0.4f",end_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
								wfile<<"G00"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							}
							else if (coortype == 1)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
								wfile<<"G00"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							}
							else if (coortype == 2)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
								wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
								wfile<<"G00"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							}

						}
						else
						{
							int big_kerfwidth = (int)(_d * 10 + 0.5);
							int big_sheight = (int)(s_height * 10 + 0.5);

							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float sbegin_x,sbegin_y,send_x,send_y;

							float begin_x = s_x - s_height/2 + _d/2.0 + _slot.at(p_s)._srcx;
							float begin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 + _d/2.0;

							sprintf_s(chbegin_x,"%0.4f",begin_y);
							sprintf_s(chbegin_y,"%0.4f",begin_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							else if (coortype == 1)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							else if (coortype == 2)
							{
								wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight1<<endl;
								wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							}
							int s_key = 0;
							for (int s_k = 0;s_k < big_sheight/big_kerfwidth + (big_sheight%big_kerfwidth != 0 ? 1 : 0);s_k++)
							{
								s_key = s_k;
								char schbegin_x[12],schbegin_y[12],schend_x[12],schend_y[12];

								sbegin_x = s_x - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2 : _d/2 + s_k*_d) + _slot.at(p_s)._srcx;
								sbegin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 + _d/2.0;
								send_x = s_x - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2 : _d/2 + s_k*_d) + _slot.at(p_s)._srcx;
								send_y = s_y + s_width/2.0 + _slot.at(p_s)._srcy - _d/2.0;

								sprintf_s(chbegin_x,"%0.4f",sbegin_y);
								sprintf_s(chbegin_y,"%0.4f",sbegin_x);
								sprintf_s(chend_x,"%0.4f",send_y);
								sprintf_s(chend_y,"%0.4f",send_x);

								if (s_k%2 == 0)
								{
									if (s_k != 0)
									{
										float pre_x = s_x - s_height/2 + _d/2 + s_k*_d + _slot.at(p_s)._srcx - _d * 2.0;
										char pre_sy[12];
										sprintf_s(pre_sy,"%0.4f",pre_x);
										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 1)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 2)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
									}
									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										sprintf_s(chend_x,"%0.4f",send_y);
										sprintf_s(chend_y,"%0.4f",send_x);
									}
								}
								else
								{
									if (s_k != 1)
									{
										float pre_x = s_x - s_height/2 + _d/2 + s_k*_d + _slot.at(p_s)._srcx - _d * 2.0;
										char pre_sy[12];
										sprintf_s(pre_sy,"%0.4f",pre_x);
										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 1)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 2)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
									}
									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										sprintf_s(chend_x,"%0.4f",sbegin_y);
										sprintf_s(chend_y,"%0.4f",sbegin_x);
									}
								}

							}
							if (s_key%2 == 0 && s_key != 0)
							{
								float pre_x = s_x - s_height/2 + _d/2 + s_key*_d + _slot.at(p_s)._srcx - _d * 2.0;
								char pre_sy[12];
								sprintf_s(pre_sy,"%0.4f",pre_x);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}
							if (s_key%2 == 1 && s_key != 1)
							{
								float pre_x = s_x - s_height/2 + _d/2 + s_key*_d + _slot.at(p_s)._srcx - _d * 2.0;
								char pre_sy[12];
								sprintf_s(pre_sy,"%0.4f",pre_x);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}
							if (s_key == 1)
							{
								float pre_x = s_x - s_height/2 + ((0 + 1)*_d > s_height ? s_height - _d/2 : _d/2) + _slot.at(p_s)._srcx;
								char pre_sy[12];
								sprintf_s(pre_sy,"%0.4f",pre_x);
								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}

						}
					}
				}
			}

			// �ж��Ƿ񻻵�
			// 2017-5-6 yuanzb ÿ�ѵ��������п׻��߲ۣ�����Ҫ��������
			// yuanzb 2017-5-8 ÿһ�ѵ�������ײۺ󶼱���д��������
			// yuanzb 2017-5-24 ͬһ�ѵ�������ײۺ���Ҫд�������룬ֱ�ӿ���


#if 0

			bool _needchangeT = true;
			for (int _ktype = 0;_ktype <m_knifetype.size();_ktype++)
			{

				KnifeType cur_knife = m_knifetype.at(_ktype);

				// ��Ϊ�ϣ�����ס��൶��
				cur_knife = m_knifetype.at(_ktype);
				if (	m_knifetype.at(_ktype).kt != KnifeType_Spindle_Material	
					&&  m_knifetype.at(_ktype).kt != KnifeType_Drill_Hole	
					&&  m_knifetype.at(_ktype).kt != KnifeType_Multi_knife_Hole	
					&&	m_knifetype.at(_ktype)._diameter == _k_d )
				{
					//_needchangeT = false;


					// ��ͬ�ĵ����ۺ󲻻�����ֱ�ӿ���
					if (_vec.size() > 0 || _slot.size() > 0)
					{
						last_knife = cur_knife;
					}


					break;
				}
			}




#else
			// 2017-6-23 yuanzb ÿ�ѵ������󣬷��ǵ�ǰ���ĵ�����Ϊ�ײ��ϻ��߲��ϣ���¼Ϊ���һ�ѵ�,���濪�ϻ���ʹ��ͬһ�ѵ�
			//					ע�⣬���������У�ֻ��������Ψһһ�ѿ��Կ��ϵĵ�

			bool _needchangeT = true;
			KnifeType cur_knife = m_knifetype.at(v_i);


			// ��ͬ�ĵ����ۺ󲻻�����ֱ�ӿ���
			if (_vec.size() > 0 || _slot.size() > 0)
			{
				last_knife = cur_knife;

				if (	cur_knife.kt == KnifeType_Spindle_Hole_Slot_Material	
					||  cur_knife.kt == KnifeType_Spindle_Slot_Material	)
				{
					_needchangeT = false;
				}
			}

#endif










			if (_needchangeT == true)
			{
				if (_vec.size() > 0 || _slot.size() > 0)
				{
					for (int _g_i = 0;_g_i < _tgcodee.size();_g_i++)
					{
						if (_tgcodee.at(_g_i) == '@')
						{
							wfile<<endl;
						}
						else
						{
							wfile<<_tgcodee.at(_g_i);
						}
					}
					wfile<<endl;
				}
			}
		}


#endif

		// �ޱ�
#if 1

		if(_df.size() > 0)
		{
			bool _needchangeT = true;
			float knift_lenght = 0.0;
			int knife_num = 0;
			int knife_spindlespeed = 8000;
			int knife_feedrate = 8000;
			int knife_endspeed = 8000;
			int knife_overspend = 8000;
			std::string _kgcodeb,_kgcodee;
			float _lastd = 0.0;

			KnifeType  cur_knife;

			for (int ktype = 0;ktype < m_knifetype.size();ktype++)
			{
				if (m_knifetype.at(ktype).kt == 0 || m_knifetype.at(ktype).kt == 4 || m_knifetype.at(ktype).kt == 5)
				{
					cur_knife = m_knifetype.at(ktype);

					_kgcodeb			= cur_knife._tgcodeb;
					_kgcodee			= cur_knife._tgcodee;
					knift_lenght		= cur_knife._length;
					knife_spindlespeed	= cur_knife._spindlespeed;
					knife_feedrate		= cur_knife._feedrate;
					knife_endspeed		= cur_knife._endspeed;
					knife_overspend		= cur_knife._overspend;
					_lastd				= cur_knife._diameter;
					break;
				}
			}

			// ��֮ǰ�Ŀײ۵���ͬ ����д��ʼ��������
			if (last_knife.kt == cur_knife.kt &&
				last_knife._diameter == cur_knife._diameter &&
				last_knife._kn == cur_knife._kn )
			{
				// ��֮ǰ�Ŀײ۵���ͬ ����д��ʼ��������
			}
			else
			{
				// ��֮ǰ�Ŀײ۵�����ͬ д��ʼ��������

				for (int _g_i = 0;_g_i < _kgcodeb.size();_g_i++)
				{
					if (_kgcodeb.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_kgcodeb.at(_g_i);
					}
				}
				last_knife = cur_knife;
			}



			wfile<<endl;

			char chdefsrcx[12],chdefsrcy[12],chdefx[12],chdefy[12],chh1[12],chh2[12],chh3[12];
			if (gcodeparam.downheight > 1000)
			{
				sprintf_s(chh2,"%0.4f",0.0);
				sprintf_s(chh3,"%0.4f",-1.0 * _thickness - 0.1);
			}
			else
			{
				sprintf_s(chh2,"%0.4f",_thickness - gcodeparam.downheight);
				sprintf_s(chh3,"%0.4f",-1.0 * m_gcodeparam.downheight);
			}

			sprintf_s(chh1,"%0.4f",gcodeparam.upheight);	

			float kdx1, kdx2, kdx3, kdx4, kdy1, kdy2, kdy3, kdy4;

			kdx1 = -1 * _lastd/2.0 + abs(_df.at(0));
			kdy1 = -1 * _lastd/2.0 + abs(_df.at(3));

			kdx2 = kdx1;
			kdy2 = /*m_panelheight*/pPanel->m_OrgWidth + _lastd/2.0 - abs(_df.at(1));

			kdx3 = /*m_panelwidth*/pPanel->m_OrgLen + _lastd/2.0 - abs(_df.at(2));
			kdy3 = kdy2;

			kdx4 = kdx3;
			kdy4 = kdy1;

			if (_merge != 2)
			{
				char ckdx1[12],ckdx2[12],ckdx3[12],ckdx4[12],ckdy1[12],ckdy2[12],ckdy3[12],ckdy4[12];

				sprintf_s(ckdx1,"%0.4f",kdx1);
				sprintf_s(ckdx2,"%0.4f",kdx2);
				sprintf_s(ckdx3,"%0.4f",kdx3);
				sprintf_s(ckdx4,"%0.4f",kdx4);

				sprintf_s(ckdy1,"%0.4f",kdy1);
				sprintf_s(ckdy2,"%0.4f",kdy2);
				sprintf_s(ckdy3,"%0.4f",kdy3);
				sprintf_s(ckdy4,"%0.4f",kdy4);

				float fix_1 =_df.at(0);
				float fix_2 =_df.at(1);
				float fix_3 =_df.at(2);
				float fix_4 =_df.at(3);

				enum
				{
					knife_up = 0,
					knife_down
				};

				int knife_pos = knife_up;

				// ��һ���ޱ�
				if (fix_1 != 0)
				{
					// ��Ҫ��


					if (knife_pos == knife_up)
					{

						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;

						// ��һ�����
						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh3<<endl;
					}


				}
				else
				{
					// ����Ҫ��
				}

				// �ڶ���
				if (fix_2 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh3<<endl;
				}
				else
				{
					// ����Ҫ��

					if (knife_pos == knife_down &&  fix_1 !=  0)
					{
						wfile<<"G00"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh1;
						// 					if (knife_feedrate > 0.01)
						// 						wfile<<" F "<<knife_feedrate;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}

				}


				// ������
				if (fix_3 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh3<<endl;
				}
				else
				{
					// ����Ҫ��

					if ((knife_pos == knife_down) &&  (fix_2  != 0))
					{
						wfile<<"G00"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh1;
						//if (knife_feedrate > 0.01)
						//	wfile<<" F "<<knife_feedrate;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}
				}


				// ���ı�
				if (fix_4 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile << "G01" << " X " << kdy1 << " Y " << kdx1 << " Z " << chh3 << endl;
					wfile << "G00" << " X " << kdy1 << " Y " << kdx1 << " Z " << chh1 << endl;
				}
				else
				{
					// ����Ҫ��

					if (knife_pos == knife_down  && fix_3  != 0)
					{
						wfile<<"G00"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh1;
						// 					if (knife_feedrate > 0.01)
						// 						wfile<<" F "<<knife_feedrate;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}
				}

				if (knife_pos == knife_down)
					wfile << "G00" << " Z " << chh1;


				// ���ص���һ����
				// 			wfile<<"G00"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh1;
				// 			if (knife_overspend > 0.01)
				// 				wfile<<" F "<<knife_overspend;
				// 			wfile<<endl;



				// ��һ����
				// 			wfile<<"G00"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh1;
				// 			if (knife_overspend > 0.01)
				// 				wfile<<" F "<<knife_overspend;
				// 			wfile<<endl;
				// 
				// 			wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh2;
				// 			if (knife_spindlespeed > 0.01)
				// 				wfile<<" F "<<knife_spindlespeed;
				// 			wfile<<endl;
				// 
				// 			wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh3;
				// 			if (knife_feedrate > 0.01)
				// 				wfile<<" F "<<knife_feedrate;
				// 			wfile<<endl;
				// 
				// 			wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh3<<endl;
				// 			wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh3<<endl;
				// 			wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh3<<endl;
				// 			wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh3<<endl;
				// 
				// 			// ���ص���һ����
				// 			wfile<<"G00"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh1;
				// 			if (knife_overspend > 0.01)
				// 				wfile<<" F "<<knife_overspend;
				// 			wfile<<endl;


			}
		}
	

#endif



		wfile<<_mainktgcodee.c_str()<<endl;
		std::string strToolEndCode = last_knife._tgcodee;
		for (int _g_i = 0;_g_i < strToolEndCode.size();_g_i++)
		{
			if (strToolEndCode.at(_g_i) == '@')
			{
				wfile<<endl;
			}
			else
			{
				wfile<<strToolEndCode.at(_g_i);
			}
		}
		wfile<<endl;

		if (_merge == 1)
		{
			CString s_tmpend;
			CString str_filechange = "";
			s_tmpend.Format("%s",f_ender.c_str());

			if (file_change.size() > 0)
			{
				str_filechange.Format("@1800:=\"%s\"",file_change.c_str());
			}
			else
			{
				str_filechange = "@1800:=0";
			}
			std::string tmpender = s_tmpend.GetBuffer();
			for (int end_i = 0;end_i < tmpender.size();end_i++)
			{
				if (tmpender.at(end_i) == '@')
				{
					wfile<<endl;
				}
				else if (tmpender.at(end_i) == '*')
				{
					wfile<<str_filechange<<endl;
				}
				else
				{
					wfile<<tmpender.at(end_i);
				}
			}
		}
	}
	wfile.close();
	if (vs_info.GetLength() > 0)
	{
		AfxMessageBox(vs_info);
	}
	return true;
}









bool KnifeClass::OutputGcode(	Panel* pPanel,				string CurCutFileName,		string CurPrintFileName,	int cutprint,				
								bool duplex,				bool sys_offset,			int coortype,				int PanelNum, 
								string NextPrintFileName,	string NextCutFileName,		vector<P_Info> _pinfo,		string grouptype,
								string file_change,			string _mainktgcodeb,		string _mainktgcodee,		string _vecktgcodeb,		
								string _vecktgcodee,		int _vecnochange,			bool KnifeOffset,			string node_dvcut,
								bool LabelInReverseSide,	int total_check_panel_num,  int decimal_place,			BOOL panel_comments)
{
	vector<gcodeCoor> _newcoor;
	GcodeParam gcodeparam = m_gcodeparam;//����Gcodeʱ�����Ż�ǰ���õģ��������Ż������õ�
	vector<gcodeCoor> hand_newcoor = m_handnewcoor[pPanel];
	vector<Component*> CpnList;

	// �������������
	pPanel->GetAllNeededComponent(CpnList);
	sort(CpnList.begin(), CpnList.end(), ComponentCompareID);
	_newcoor = pathoptimize(CpnList);
	TransformMachiningInfoToOld_UpperFace(pPanel, CpnList, _newcoor, m_allpoint, m_allslot, m_allvec, m_LabelPosition, m_cutpath, m_PreCutPath);



	// ��ʼд�ļ�
	DeleteFile(CurCutFileName.c_str());
	DeleteFile(CurPrintFileName.c_str());

	// ��֪������
	if (coortype == 1)
	{
		for (int i = 0;i < _newcoor.size();i++)
		{
			_newcoor.at(i)._y = m_panelheight - _newcoor.at(i)._y;
		}
	}
	else if (coortype == 2)
	{
		for (int i = 0;i < _newcoor.size();i++)
		{
			_newcoor.at(i)._y = -1.0 * _newcoor.at(i)._y;
		}
	}


	std::ofstream wfile(CurCutFileName.c_str(),std::ios::out);
	int key = GetLastError();


	CString vs_info = "";
	if (wfile)
	{

		// ��������ļ�
		OutputLabel(pPanel, CurCutFileName, CurPrintFileName, cutprint, PanelNum, NextPrintFileName, NextCutFileName, _pinfo, wfile, _newcoor, node_dvcut, LabelInReverseSide);


		// д�ļ�ͷ
#if 1

		CString s_header;
		s_header.Format("%s",gcodeparam.header.c_str());
		s_header.Replace(_T("\r\n"),_T("@"));
		std::string tmp_header = s_header.GetBuffer();

		for (int i = 0;i < tmp_header.size();i++)
		{
			if (tmp_header.at(i) == '@')
			{
				wfile<<endl;
			}
			else if (tmp_header.at(i) == '!')
			{
				wfile<<"@";
			}
			else
			{
				wfile<<tmp_header.at(i);
			}
		}
#endif

		// д�ȴ��ļ�ͷ
#if 1
		if (cutprint == 17)
		{

			wfile << "R1=2440"										<< endl;
			wfile << "R2=1200"										<< endl;
			wfile << "R3=18"										<< endl;
			wfile << "R4=0"											<< endl;
			wfile << "FCONST"										<< endl;
			wfile << "R99=2 (Flag KL)"								<< endl;
			wfile << "M50 (Init Check)"								<< endl;
			wfile << "@402 K5003 K5 K1"								<< endl;
			wfile << "G71 (Metric data input)"						<< endl;
			wfile << "G17 (X-Y Plane)"								<< endl;
			wfile << "G53"											<< endl;
			wfile << "G0 Z=R202	"									<< endl;
			wfile << "G0 X=R200 Y=R201"								<< endl;
			wfile << "T0 M06 D0"									<< endl;
			wfile << "G53"											<< endl;
			wfile << "G0 Z=R202"									<< endl;
			wfile << "G0 X=R203 Y=R201"								<< endl;
			wfile << "M05"											<< endl;
			wfile << "M58"											<< endl;
			wfile << "@716 (Refresh)"								<< endl;
			wfile << "@714"											<< endl;
			wfile << "R10=10"										<< endl;
			wfile << "#set paramZeroShift( G56; R215; R216; 0)#"	<< endl;
			wfile << "G56 (activates zero offest shift)"			<< endl;
			wfile << "T0 M06 D0"									<< endl;
			wfile << "G0 Z=R202"									<< endl;
			wfile << "T01 M06 D1"									<< endl;


			wfile << "G04 F1"										<< endl;
			wfile << "M03 S=R262"									<< endl;
			wfile << "G04 F=R263"									<< endl;


		}


#endif


		// ����ҳ�룬����鿴
#if 1
		if (panel_comments == TRUE)
		{

			CString strPageInfo;

			strPageInfo.Format("//(No.%d/%d, �ߴ�:%0.1f*%0.1f, ���:%0.1f, ����:%s)", PanelNum, total_check_panel_num, /*m_panelwidth*/pPanel->m_OrgLen, /*m_panelheight*/pPanel->m_OrgWidth, _thickness, m_material.c_str());

			wfile << strPageInfo << endl;

		}


#endif


		// ������
#if 1

		//wfile<<gcodeparam.header.c_str()<<endl;
		vector<AllVec> _leftvec;

		// ����������
		std::vector<KnifeTypeAndVecA> pvecA;
		for (int i = 0;i < _newcoor.size();i++)
		{
			// ����Ψһ����ID
			int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

			// С������
			gcodeCoor panel_coor = _newcoor.at(i);

			std::string info	= panel_coor._info;
			float src_x			= panel_coor._x;
			float src_y			= panel_coor._y;
			float p_width		= panel_coor._width;
			float p_height		= panel_coor._height;

			for (int v = 0; v < m_allvec.size(); v++)
			{

				// ԭʼ������
				AllVec src_vec = m_allvec.at(v);

				// ����Ψһ����ID
				int one_vec_id_in_panel = m_allvec.at(v).m_NumInPanel;
				std::string panel_info	= src_vec.panel_info;
				float one_d				= src_vec._r;

				//if (strcmp(info.c_str(), panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_vec_id_in_panel)
				{
					// ���������
					KnifeTypeAndVecA onevec;	

					onevec.panel_info	= panel_info;
					onevec._x			= src_vec._x;
					onevec._y			= src_vec._y;
					onevec._r			= src_vec._r;
					onevec._depth		= src_vec._depth;
					onevec.src_x		= src_x;
					onevec.src_y		= src_y;
					onevec._pheight		= p_height;
					onevec._pwidth		= p_width;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (panel_coor._dir == 2)
// 					{
// 						onevec._x = panel_coor._width - src_vec._y;
// 						onevec._y = src_vec._x;
// 					}

					float min_dis = 100000;
					int v_key = -1; 
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{	
						// �����еĵ�
						KnifeType one_knife = m_knifetype.at(v_j);

						if (one_knife.kt != KnifeType_Drill_Hole)	// one_knife.kt != 6
						{
							continue;
						}
						// ѡ��ƥ��ĵ�
						if (src_vec._r - one_knife._diameter > -0.01 
							&& src_vec._r - one_knife._diameter < 0.01)
						{
							v_key = v_j;	// �ҵ���������
							break;
						}
					}

					if (v_key == -1)
					{
						_leftvec.push_back(m_allvec.at(v));
						continue;
					}

					const KnifeType& select_knife	= m_knifetype.at(v_key);

					onevec._diameter				= select_knife._diameter;
					onevec._tgcodeb					= select_knife._tgcodeb;
					onevec._tgcodee					= select_knife._tgcodee;

					
					onevec._feedrate				= select_knife._feedrate;
					onevec._spindlespeed			= select_knife._spindlespeed;
					onevec._overspeed				= select_knife._overspend;
					onevec._endspeed				= select_knife._endspeed;
					onevec._facespeed				= select_knife._facespeed;
					onevec._r						= onevec._r - onevec._diameter;		// ����������
					onevec._coorsys					= select_knife._coorsys;
					onevec._kdown					= true;

					onevec._kx.clear();
					onevec._ky.clear();
					onevec._kn.clear();
					onevec._samedownkn.clear();

					// ��յ��߿�ʼ��������
					onevec.BeginCodeList.clear();
					onevec.EndCodeList.clear();

					// �ж��Ƿ�Ϊ��ͬ�Ŀף������ظ����
					bool IsThesameVec = false;
					for (int pvecA_i = 0;pvecA_i < pvecA.size();pvecA_i++)
					{
						float detection_x		= abs(pvecA.at(pvecA_i)._x		- onevec._x + pvecA.at(pvecA_i).src_x - onevec.src_x);
						float detection_y		= abs(pvecA.at(pvecA_i)._y		- onevec._y + pvecA.at(pvecA_i).src_y - onevec.src_y);
						float detection_r		= abs(pvecA.at(pvecA_i)._r		- onevec._r);
						float detection_depth	= abs(pvecA.at(pvecA_i)._depth	- onevec._depth);

						if (detection_x == 0 && 
							detection_y == 0 &&
							detection_r == 0 &&
							detection_depth == 0)
						{
							IsThesameVec = true;
							break;
						}
					}

					if (IsThesameVec == false)
					{
						pvecA.push_back(onevec);
					}
				}
			}
		}

#endif


		// ��ʼ������
#if 1
		// ѭ����������ף����Ƿ����˫���µ���Ͽ�
		for (int p_i = 0;p_i < pvecA.size();p_i++)
		{
			KnifeTypeAndVecA&  knife_vec =  pvecA.at(p_i);
			float _d = pvecA.at(p_i)._diameter;
			// ��������
			for (int p_j = 0;p_j < m_knifetype.size();p_j++)
			{
				const KnifeType& one_knife_type = m_knifetype.at(p_j);
				// �ҵ����굶
				if (one_knife_type.kt != KnifeType_Drill_Hole)	// m_knifetype.at(p_j).kt != 6
				{
					continue;
				}
				// ���굶ֱ����ͬ
				if (one_knife_type._diameter - _d < 0.01 
					&& one_knife_type._diameter - _d > -0.01)
				{
					knife_vec._kx.push_back(knife_vec.src_x + knife_vec._x - one_knife_type._kx);		// ԭʼ���� + �ޱ� - ƫ��
					knife_vec._ky.push_back(knife_vec.src_y + knife_vec._y - one_knife_type._ky);		// ԭʼ���� + �ޱ� - ƫ��
					knife_vec._kn.push_back(one_knife_type._kn);

					// ��¼����
					knife_vec.BeginCodeList.push_back(one_knife_type._tgcodeb);
					knife_vec.EndCodeList.push_back(one_knife_type._tgcodee);



				//	break;				// 2016-12-6 yuanzb ���޸Ĵ���ƫ�������У���������ֱ����ͬ�ĵ������ţ������¼�ˣ�
					// ��������ʼ����ֻ��¼��һ���������õڶ��ѵ�ʱ�����˵�һ�ѵ��Ŀ�ʼ���룬����ֻ�ü�¼һ�ѵ�����ֹ����
				}
			}
		}

#endif 


		//�ж��Ƿ�����ϵ������û�У������ϲ�����
#if 1

		bool twodownknife = false;
		for (int k_i = 0;k_i < m_knifetype.size();k_i++)
		{
			if (m_knifetype.at(k_i).kt == 7)
			{
				twodownknife = true;
				break;
			}
		}

		//@@@@@ ȷ��ʹ����һ�ѵ���ȷ����������
		for (int vecsame_i = 0;vecsame_i < 1;vecsame_i++)
		{
			vector<int> posselect;
			posselect.clear();

			// ������

			// �ױ���
			int vec_num = pvecA.size();

			for (int i_vec = 0;i_vec < vec_num; i_vec++)
			{
				KnifeTypeAndVecA& first_vec = pvecA.at(i_vec);

				if (first_vec._kdown == false)
				{
					continue;
				}

				int max_same_num = 0;
				int max_same_key = 0;
				std::vector<int> same_p;
				std::vector<int> down_kn;

				int this_kn = 0;
				float this_kx = 0.0;
				float this_ky = 0.0;
				string this_begin_code;
				string this_end_code;



				// ���õĵ�����
				int cur_knife_num = first_vec._kn.size();
				for (int i_cur_knife = 0; i_cur_knife < cur_knife_num; i_cur_knife++)	// �ж൶��ѡ
				{
					int m_same_n = 0;
					std::vector<int> n_same_p;
					std::vector<int> n_down_kn;
					bool haveselect = false;

					// �ٱ�����
					for (int j_vec = 0; j_vec < vec_num; j_vec++)
					{
						KnifeTypeAndVecA& next_vec = pvecA.at(j_vec);

						if (i_vec == j_vec 
							|| next_vec._kdown == false 
							|| find( posselect.begin( ), posselect.end( ), j_vec ) != posselect.end())
						{
							continue;
						}

						int next_knife_num =  next_vec._kn.size();
						for (int i_next_knife = 0;i_next_knife < next_knife_num;i_next_knife++)
						{
							float dis_x = first_vec._kx.at(i_cur_knife) - next_vec._kx.at(i_next_knife);
							float dis_y = first_vec._ky.at(i_cur_knife) - next_vec._ky.at(i_next_knife);
							float dis_d = first_vec._depth - next_vec._depth;

							if (dis_x == 0 && dis_y == 0 && dis_d == 0)
							{
								m_same_n = m_same_n + 1;
								n_same_p.push_back(j_vec);
								n_down_kn.push_back(i_next_knife);
								haveselect = true;
								break;
							}
						}

						if (haveselect == true)
						{
							break;
						}
					}

					if (m_same_n > max_same_num)
					{
						max_same_num = m_same_n;
						max_same_key = i_cur_knife;
						same_p = n_same_p;
						down_kn = n_down_kn;
						this_kn = first_vec._kn.at(i_cur_knife);		// ȷ�ϵ������� ��������⣬�����ѵ�ʱû�аѵ��߿�ʼ�����������
						this_kx = first_vec._kx.at(i_cur_knife);
						this_ky = first_vec._ky.at(i_cur_knife);

						// ��ֵ�����ĵ���
						this_begin_code = first_vec.BeginCodeList.at(i_cur_knife);
						this_end_code	= first_vec.EndCodeList.at(i_cur_knife);


						posselect.push_back(i_vec);
					}

					if (haveselect == true)
					{
						break;
					}
				}


				// �Ƿ��ж൶����
				if (max_same_num > 0)
				{
					for (int p_k = 0;p_k < same_p.size();p_k++)
					{
						first_vec._samedownkn.push_back(pvecA.at(same_p.at(p_k))._kn.at(down_kn.at(p_k)));
						if (twodownknife == true)
						{
							pvecA.at(same_p.at(p_k))._kdown = false;
						}
					}

					first_vec._thiskn = this_kn;			// ȷ�ϵ�������
					first_vec._thiskx = this_kx;
					first_vec._thisky = this_ky;
					first_vec._tgcodeb = this_begin_code;
					first_vec._tgcodee = this_end_code;


					for (int k_i = 0;k_i < m_knifetype.size();k_i++)
					{
						if (m_knifetype.at(k_i).kt == 7)
						{
							first_vec._tgcodeb = m_knifetype.at(k_i)._tgcodeb;
							first_vec._tgcodee = m_knifetype.at(k_i)._tgcodee;
							break;
						}
					}
				}
				else
				{
					vector<int> tmpn = pvecA.at(i_vec)._kn;
					int num = tmpn.size();
					first_vec._thiskn = first_vec._kn.at(0);	// ֻ��һ�ѵ����õ�ֱ�Ӹ�ֵ
					first_vec._thiskx = first_vec._kx.at(0);
					first_vec._thisky = first_vec._ky.at(0);
				}
			}
		}
		//@@@@@

		std::vector<pos> poses;
		std::vector<pos> newposes;

		int key = pvecA.size();

		// �����������
		if (_vecnochange == true)
		{

			vector<vector<KnifeTypeAndVecA>> diff_pvecA;

#if 0
			// ��ΰѿװ�ֱ������Ĵ����д��󣬵���©�ף���д

			int p_key = 1;
			for (int p_i = 0;p_i < pvecA.size();p_i = p_i + p_key)
			{
				for (int p_j = p_i+1;p_j < pvecA.size();p_j++)
				{
					std::string one_begin = pvecA.at(p_i)._tgcodeb;
					std::string one_end = pvecA.at(p_i)._tgcodee;

					std::string two_begin = pvecA.at(p_j)._tgcodeb;
					std::string two_end = pvecA.at(p_j)._tgcodee;

					if (strcmp(one_begin.c_str(),two_begin.c_str()) == 0 && strcmp(one_end.c_str(),two_end.c_str()) == 0)
					{
						swap(pvecA[p_j],pvecA[p_key]);
						p_key++;
					}
				}

				vector<KnifeTypeAndVecA> d_pvecA;
				for (int d_i = p_i;d_i < p_key;d_i++)
				{
					d_pvecA.push_back(pvecA.at(d_i));
				}

				diff_pvecA.push_back(d_pvecA);

			}

#else

			vector<vector<KnifeTypeAndVecA>> DifferentDiameterVecGroup;

			for (int i_vec = 0; i_vec < pvecA.size(); i_vec++)
			{
				KnifeTypeAndVecA curVec = pvecA.at(i_vec);
				int  nGroupCount = DifferentDiameterVecGroup.size();

				// ˫����ʱ��ȡ���Ŀײ��ռ�
				if (curVec._kdown == false)
				{
					continue;
				}


				// ����ֱ���½�����
				if (nGroupCount == 0)
				{
					vector<KnifeTypeAndVecA> new_vec_list;

					new_vec_list.push_back(curVec);

					DifferentDiameterVecGroup.push_back(new_vec_list);
				}
				else
				{
					// Ѱ��������ֱͬ�����飬������룬�����½�
					bool exist_flag = false;
					for (int i_group = 0; i_group < nGroupCount; i_group++)
					{
						vector<KnifeTypeAndVecA>& tmp_vec_list = DifferentDiameterVecGroup.at(i_group);
						KnifeTypeAndVecA tmp_vec = tmp_vec_list.at(0);

						// ���÷���
						if (tmp_vec_list.size() == 0)
						{
							exist_flag = false;
							continue;
						}

						// ֱ�������š�˫������ͬ��Ϊһ��
						if ( (abs(curVec._diameter - tmp_vec._diameter) < 0.01)  
							 && (curVec._thiskn == tmp_vec._thiskn))
						{



							// ����˫����
							if (curVec._samedownkn.size() == 0 && tmp_vec._samedownkn.size() == 0)
							{
								exist_flag = true;
								tmp_vec_list.push_back(curVec);
								break;
							} // ͬΪ˫���� �ҵ�����ͬ
							else if (curVec._samedownkn.size() == 1 && tmp_vec._samedownkn.size() == 1)
							{
								if (curVec._samedownkn[0] == tmp_vec._samedownkn[0])
								{
									exist_flag = true;
									tmp_vec_list.push_back(curVec);
									break;
								}
								else
								{
									exist_flag = false;
								}
							}
							else
							{
								exist_flag = false;
							}

						}
						else
						{
							exist_flag = false;
						}
					}

					if (exist_flag == false)
					{
						vector<KnifeTypeAndVecA> new_vec_list;

						new_vec_list.push_back(curVec);

						DifferentDiameterVecGroup.push_back(new_vec_list);
					}

				}
			}

			diff_pvecA = DifferentDiameterVecGroup;

#endif

			





		//	swap(diff_pvecA[0], diff_pvecA[1]);

			

			// �����������

			pvecA.clear();

			for (int i = 0;i < diff_pvecA.size();i++)
			{
				std::vector<KnifeTypeAndVecA> one_pvecA = diff_pvecA.at(i);
				
				// �����ʱλ������
				poses.clear();
				newposes.clear();

				int one_pvecA_size = one_pvecA.size();
				for (int i_vec = 0;i_vec < one_pvecA_size; i_vec++)
				{
					KnifeTypeAndVecA tmp_hole = one_pvecA.at(i_vec);

					if (tmp_hole._kdown == false)
					{
						continue;
					}

					pos one_pos;
					one_pos.x	= tmp_hole._thiskx;
					one_pos.y	= tmp_hole._thisky;
					one_pos._kn = tmp_hole._thiskn;

					poses.push_back(one_pos);
				}

				pos begin_pos;
				begin_pos.x = 0;
				begin_pos.y = 0;

				if (coortype == 1)
				{
					begin_pos.x = 0;
					begin_pos.y = m_panelheight;
				}
				else if (coortype == 2)
				{
					begin_pos.x = 0;
					begin_pos.y = -1.0*m_panelheight;
				}

				GetShortestPath(poses,begin_pos,newposes);
				poses.clear();

				for (int i_vec = 0;i_vec < newposes.size();i_vec++)
				{
					poses.push_back(newposes.at(i_vec));
				}

				newposes.clear();
				GetChangePath(poses,newposes);
				// ����Ҫ��poses���

				std::vector<KnifeTypeAndVecA> tmppvec;

				for (int p_j = 0;p_j < newposes.size();p_j++)
				{
					for (int p_i = 0;p_i < one_pvecA.size();p_i++)
					{
						if (one_pvecA.at(p_i)._kdown == false)
						{
							continue;
						}
						if (	newposes.at(p_j).x - (one_pvecA.at(p_i)._thiskx) > -0.01 
							&&	newposes.at(p_j).x - (one_pvecA.at(p_i)._thiskx) < 0.01 
							&&	newposes.at(p_j).y - (one_pvecA.at(p_i)._thisky) > -0.01 
							&&	newposes.at(p_j).y - (one_pvecA.at(p_i)._thisky) < 0.01 
							&&	newposes.at(p_j)._kn ==  one_pvecA.at(p_i)._thiskn)
						{
							tmppvec.push_back(one_pvecA.at(p_i));
						}
					}
				}

				for (int i_vec = 0;i_vec < tmppvec.size();i_vec++)
				{
					pvecA.push_back(tmppvec.at(i_vec));
				}
			}
		}
		else
		{
			for (int p_i = 0;p_i < pvecA.size();p_i++)
			{
				if (pvecA.at(p_i)._kdown == false)
				{
					continue;
				}

				pos one_pos;
				one_pos.x = pvecA.at(p_i)._thiskx;			// ��¼����+xy
				one_pos.y = pvecA.at(p_i)._thisky;
				one_pos._kn = pvecA.at(p_i)._thiskn;

				poses.push_back(one_pos);
			}

			pos begin_pos;
			begin_pos.x = 0;
			begin_pos.y = 0;

			if (coortype == 1)
			{
				begin_pos.x = 0;
				begin_pos.y = m_panelheight;
			}
			else if (coortype == 2)
			{
				begin_pos.x = 0;
				begin_pos.y = -1.0*m_panelheight;
			}

			GetShortestPath(poses,begin_pos,newposes);
			poses.clear();

			for (int p_i = 0;p_i < newposes.size();p_i++)
			{
				poses.push_back(newposes.at(p_i));
			}

			newposes.clear();
			GetChangePath(poses,newposes);
			std::vector<KnifeTypeAndVecA> tmppvec;

			for (int p_j = 0;p_j < newposes.size();p_j++)
			{
				for (int p_i = 0;p_i < pvecA.size();p_i++)
				{
					if (pvecA.at(p_i)._kdown == false)
					{
						continue;
					}

					if (newposes.at(p_j).x - (pvecA.at(p_i)._thiskx) > -0.01 
						&& newposes.at(p_j).x - (pvecA.at(p_i)._thiskx) < 0.01 
						&& newposes.at(p_j).y - (pvecA.at(p_i)._thisky) > -0.01 
						&& newposes.at(p_j).y - (pvecA.at(p_i)._thisky) < 0.01 
						&& newposes.at(p_j)._kn ==  pvecA.at(p_i)._thiskn)
					{
						tmppvec.push_back(pvecA.at(p_i));
					}
				}
			}

			pvecA.clear();
			for (int p_i = 0;p_i < tmppvec.size();p_i++)
			{
				pvecA.push_back(tmppvec.at(p_i));
			}
		}

		std::string old_tgcodeb = "";
		std::string old_tgcodee = "";

		if (pvecA.size() > 0)
		{
			wfile<<_vecktgcodeb.c_str()<<endl;
		}


		// ������
		std::vector<KnifeTypeAndVecA> TmpKnifeTypeAndVecA = pvecA;

		// һ��һ��������
		for (int v_i = 0;v_i < pvecA.size();v_i++)
		{
			KnifeTypeAndVecA tmp_knife = pvecA.at(v_i);
			float _d = pvecA.at(v_i)._diameter;

			std::string _tgcodeb = pvecA.at(v_i)._tgcodeb;
			std::string _tgcodee = pvecA.at(v_i)._tgcodee;

			if (pvecA.at(v_i)._samedownkn.size() > 0)
			{
				int _sx,_sy;
				_sx = pvecA.at(v_i)._thiskn;
				_sy = pvecA.at(v_i)._samedownkn.at(0);
				std::vector<int> same_down_kn;
				same_down_kn.push_back(pvecA.at(v_i)._thiskn);

				for (int str_same_i = 0;str_same_i < pvecA.at(v_i)._samedownkn.size();str_same_i++)
				{
					same_down_kn.push_back(pvecA.at(v_i)._samedownkn.at(str_same_i));
				}

				//std::string s_samedownkn = ChangeGroup(same_down_kn,grouptype);
				std::string s_samedownkn = ChangeGroup(_sx,_sy,grouptype);

				_tgcodeb = replace_allstr(_tgcodeb,"*",s_samedownkn);
				_tgcodee = replace_allstr(_tgcodee,"*",s_samedownkn);

				std::string _coorsys = "";
				for (int v_k = 0;v_k < m_knifetype.size();v_k++)
				{
					if (m_knifetype.at(v_k)._kn == pvecA.at(v_i)._thiskn)
					{	
						KnifeType tm_type = m_knifetype.at(v_k);
						_coorsys = m_knifetype.at(v_k)._coorsys;

						if (sys_offset == true)
						{
							pvecA.at(v_i)._thiskx = pvecA.at(v_i)._thiskx + m_knifetype.at(v_k)._kx;
							pvecA.at(v_i)._thisky = pvecA.at(v_i)._thisky + m_knifetype.at(v_k)._ky;
							
						}
						//  
						_coorsys = m_knifetype.at(v_k)._coorsys;
						_tgcodeb = replace_allstr(_tgcodeb,"$",_coorsys);
						_tgcodee = replace_allstr(_tgcodee,"$",_coorsys);
					}
				}
			}
			else
			{
				for (int v_k = 0;v_k < m_knifetype.size();v_k++)
				{
					if (m_knifetype.at(v_k)._kn == pvecA.at(v_i)._thiskn)
					{
						if (sys_offset == true)
						{
							CString str_kx,str_ky,str_kn;
							str_kx.Format("%f",pvecA.at(v_i)._thiskx + m_knifetype.at(v_k)._kx);
							str_ky.Format("%f",pvecA.at(v_i)._thisky + m_knifetype.at(v_k)._ky);
							str_kn.Format("%d",pvecA.at(v_i)._thiskn);

							str_kx.Format("%f",pvecA.at(v_i)._thiskx);
							str_ky.Format("%f",pvecA.at(v_i)._thisky);
							str_kn.Format("%d",pvecA.at(v_i)._thiskn);

							pvecA.at(v_i)._thiskx = pvecA.at(v_i)._thiskx + m_knifetype.at(v_k)._kx;
							pvecA.at(v_i)._thisky = pvecA.at(v_i)._thisky + m_knifetype.at(v_k)._ky;
						}
					}
				}
			}

			// 			if (v_i != 0 && strcmp(_tgcodeb.c_str(),pvecA.at(v_i-1)._tgcodeb.c_str()) == 0)
			// 			{
			// 				wfile<<endl;
			// 			}
			// 			else
			// 			{
			// 				for (int _g_i = 0;_g_i < _tgcodeb.size();_g_i++)
			// 				{
			// 					if (_tgcodeb.at(_g_i) == '@')
			// 					{
			// 						wfile<<endl;
			// 					}
			// 					else
			// 					{
			// 						wfile<<_tgcodeb.at(_g_i);
			// 					}
			// 				}
			// 			}


			// �Ա����ϵ��߿�ʼ����
			if (strcmp(_tgcodeb.c_str(), old_tgcodeb.c_str()) != 0)
			{
				for (int _g_i = 0;_g_i < _tgcodeb.size();_g_i++)
				{
					if (_tgcodeb.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_tgcodeb.at(_g_i);
					}
				}
			}

			old_tgcodeb = _tgcodeb;
			wfile<<endl;

			float v_x = pvecA.at(v_i)._thiskx;
			float v_y = pvecA.at(v_i)._thisky;
			if (coortype == 1)
			{
				v_y = (v_y - pvecA.at(v_i).src_y)*(-1.0) + pvecA.at(v_i).src_y;
			}
			else if (coortype == 2)
			{
				v_y = (v_y - pvecA.at(v_i).src_y)*(-1.0) + pvecA.at(v_i).src_y;
			}

			float v_depth;

			if (gcodeparam.downheight > 1000)
			{
				v_depth = -1* pvecA.at(v_i)._depth;
			}
			else
			{

				v_depth = _thickness -  pvecA.at(v_i)._depth;
				if (gcodeparam.downheight > 0.5)
				{
					v_depth = v_depth - gcodeparam.downheight;
				}
			}



			char chheight1[12],chheight2[12],chheight3[12];
			//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
			//sprintf_s(chheight2,"%0.4f",v_depth);


			PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);
			PrintNumber2Array(chheight2, 12, decimal_place, v_depth);

			if (gcodeparam.downheight > 1000)
			{
				//sprintf_s(chheight3,"%0.4f",0.0000);
				PrintNumber2Array(chheight3, 12, decimal_place,0.0000);
			}
			else
			{
				//sprintf_s(chheight3,"%0.4f",_thickness);
				PrintNumber2Array(chheight3, 12, decimal_place, _thickness);

				if (gcodeparam.downheight > 0.5)
				{
					//sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
					PrintNumber2Array(chheight3, 12, decimal_place, _thickness - gcodeparam.downheight);
				}
			}


			char chrx1[12],chry1[12];
			//sprintf_s(chrx1,"%0.4f",v_x);
			//sprintf_s(chry1,"%0.4f",v_y);
			PrintNumber2Array(chrx1, 12, decimal_place, v_x);
			PrintNumber2Array(chry1, 12, decimal_place, v_y);

			if (coortype == 0)
			{
				wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1;
				if (pvecA.at(v_i)._overspeed > 0.01)
				{
					wfile<<" F "<<pvecA.at(v_i)._overspeed;
				}
				wfile<<endl;
				wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3;
				if (pvecA.at(v_i)._facespeed > 0.01)
				{
					wfile<<" F "<<pvecA.at(v_i)._facespeed;
				}
				wfile<<endl;
				wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
				if (pvecA.at(v_i)._feedrate > 0.01)
				{
					wfile<<" F "<<pvecA.at(v_i)._feedrate;
				}
				wfile<<endl;
				wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1;
				if (pvecA.at(v_i)._overspeed > 0.01)
				{
					wfile<<" F "<<pvecA.at(v_i)._overspeed<<endl;
				}
				wfile<<endl;
			}
			else if (coortype == 1)
			{
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<pvecA.at(v_i)._spindlespeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<pvecA.at(v_i)._feedrate<<endl;
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
			}
			else if (coortype == 2)
			{
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<pvecA.at(v_i)._spindlespeed<<endl;
				wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<pvecA.at(v_i)._feedrate<<endl;
				wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<pvecA.at(v_i)._overspeed<<endl;
			}

			// 2016-11-1 yuanzb ��������NC ��ͬ�ĵ�֮���д�˽�������	
			int nPrevASize = pvecA.size();				// ������
			string strCurKinefBeginCode = _tgcodeb;		// ��ǰ������ʼ����
			string strNextKinefBeginCode = "";			// ��һ��������ʼ����

			if (v_i != pvecA.size() -1)
			{
				KnifeTypeAndVecA NextVecA =  pvecA.at(v_i+1);
				strNextKinefBeginCode = NextVecA._tgcodeb;

				if (NextVecA._samedownkn.size() > 0)
				{
					// �滻��ϵ�
					int _sx,_sy;
					_sx = NextVecA._thiskn;
					_sy = NextVecA._samedownkn.at(0);	// ��ϵ�
					std::string s_samedownkn = ChangeGroup(_sx,_sy,grouptype);

					// �滻*��
					strNextKinefBeginCode = replace_allstr(strNextKinefBeginCode,"*",s_samedownkn);


					// 2017-4-19 yuanzb �滻$��
					for (int v_k = 0;v_k < m_knifetype.size();v_k++)
					{
						if (m_knifetype.at(v_k)._kn == pvecA.at(v_i+1)._thiskn)
						{	
							KnifeType tm_type = m_knifetype.at(v_k);
							std::string _coorsys = m_knifetype.at(v_k)._coorsys;

							if (sys_offset == true)
							{
								_coorsys = m_knifetype.at(v_k)._coorsys;

								strNextKinefBeginCode = replace_allstr(strNextKinefBeginCode,"$",_coorsys);
							}
						}
					}
				}
			}


			if (strCurKinefBeginCode.compare(strNextKinefBeginCode) == 0)
			{
				wfile<<endl;
			}
			else
			{
				for (int _g_i = 0;_g_i < _tgcodee.size();_g_i++)
				{
					if (_tgcodee.at(_g_i) == '@')
					{
						wfile << endl;
					}
					else
					{
						wfile << _tgcodee.at(_g_i);
					}
				}
				wfile << endl;
			}
		}



		if (pvecA.size() > 0)
			wfile<<_vecktgcodee.c_str()<<endl;

		wfile<<_mainktgcodeb.c_str()<<endl;


#endif		



		// �ײ�

#if 1
		


		// �ײ�
		std::vector<KnifeTypeAndVec> pvec;
		std::vector<KnifeTypeAndSlot> p_solt;
		for (int i = 0;i < _newcoor.size();i++)
		{
			// ����Ψһ����ID
			int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

			std::string info = _newcoor.at(i)._info;
			float p_width = _newcoor.at(i)._width;
			float p_height = _newcoor.at(i)._height;

			float src_x = _newcoor.at(i)._x;
			float src_y = _newcoor.at(i)._y;

			for (int v = 0;v < _leftvec.size();v++)
			{
				// ����Ψһ����ID
				int one_vec_id_in_panel = _leftvec.at(v).m_NumInPanel;

				std::string panel_info = _leftvec.at(v).panel_info;
				float one_d = _leftvec.at(v)._r;

				//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_vec_id_in_panel)
				{
					KnifeTypeAndVec onevec;

					onevec.panel_info = panel_info;
					onevec._pwidth = p_width;
					onevec._pheight = p_height;
					onevec._x = _leftvec.at(v)._x;
					onevec._y = _leftvec.at(v)._y;
					onevec._r = _leftvec.at(v)._r;
					onevec._depth = _leftvec.at(v)._depth;
					onevec._srcx = src_x;
					onevec._srcy = src_y;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (_newcoor.at(i)._dir == 2)
// 					{
// 						onevec._x = _newcoor.at(i)._width - _leftvec.at(v)._y;
// 						onevec._y = _leftvec.at(v)._x;
// 					}

					float min_dis = 100000;
					int v_key = -1;
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{
						if (m_knifetype.at(v_j).kt != 0 && m_knifetype.at(v_j).kt != 1 && m_knifetype.at(v_j).kt != 2)
						{
							continue;
						}

						if (_leftvec.at(v)._r - m_knifetype.at(v_j)._diameter >= 0.0 && min_dis > (_leftvec.at(v)._r - m_knifetype.at(v_j)._diameter))
						{
							min_dis = _leftvec.at(v)._r - m_knifetype.at(v_j)._diameter;
							v_key = v_j;
						}
					}

					if (v_key == -1)
					{
						CString info;
						info.Format("�����%s��Ӧ��λֱ����%0.1f�޷����\r\n",panel_info.c_str(),one_d);
						//AfxMessageBox(info);
						vs_info = vs_info + info;
						continue;
					}

					onevec._diameter = m_knifetype.at(v_key)._diameter;
					onevec._tgcodeb = m_knifetype.at(v_key)._tgcodeb;
					onevec._tgcodee = m_knifetype.at(v_key)._tgcodee;
					onevec._feedrate = m_knifetype.at(v_key)._feedrate;
					onevec._spindlespeed = m_knifetype.at(v_key)._spindlespeed;
					onevec._r = onevec._r - onevec._diameter;
					
					// �����ظ���
					bool IsThesameVec = false;
					for (int pvec_i = 0; pvec_i < pvec.size(); pvec_i++)
					{
						float detection_x		= abs(pvec.at(pvec_i)._x - onevec._x + pvec.at(pvec_i)._srcx - onevec._srcx);
						float detection_y		= abs(pvec.at(pvec_i)._y - onevec._y + pvec.at(pvec_i)._srcy - onevec._srcy);
						float detection_r		= abs(pvec.at(pvec_i)._r - onevec._r);
						float detection_depth	= abs(pvec.at(pvec_i)._depth - onevec._depth);

						if (detection_x == 0 && 
							detection_y == 0 &&
							detection_r == 0 &&
							detection_depth == 0)
						{
							IsThesameVec = true;
							break;
						}
					}
					if (IsThesameVec == false)
					{
						pvec.push_back(onevec);
					}
				}
			}


			for (int s = 0;s < m_allslot.size();s++)
			{
				// ����Ψһ����ID
				int one_slot_id_in_panel = m_allslot.at(s).m_NumInPanel;

				std::string panel_info = m_allslot.at(s).panel_info;

				//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
				if (one_coor_id_in_panel == one_slot_id_in_panel)
				{
					KnifeTypeAndSlot oneks;
					oneks.panel_info = panel_info;
					oneks._pwidth = p_width;
					oneks._pheight = p_height;
					oneks._depth = m_allslot.at(s)._depth;
					oneks._dir = m_allslot.at(s)._dir;
					oneks._width = m_allslot.at(s)._width;
					oneks._height = m_allslot.at(s)._height;
					oneks._x = m_allslot.at(s)._x;
					oneks._y = m_allslot.at(s)._y;
					oneks._srcx = src_x;
					oneks._srcy = src_y;

					// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 					if (_newcoor.at(i)._dir == 2)
// 					{
// 						oneks._x = _newcoor.at(i)._width - m_allslot.at(s)._y;
// 						oneks._y = m_allslot.at(s)._x;
// 						oneks._dir = 1 - oneks._dir;
// 					}
					// �ҵ�����ʵ��ǰѵ�
					float min_dis = 100000;
					int v_key = -1;
					for (int v_j = 0;v_j < m_knifetype.size();v_j++)
					{
						if (m_knifetype.at(v_j).kt != 0 && m_knifetype.at(v_j).kt != 1 && m_knifetype.at(v_j).kt != 3 && m_knifetype.at(v_j).kt != 5)
						{
							continue;
						}
						if (m_allslot.at(s)._height - m_knifetype.at(v_j)._diameter >= 0.0 && min_dis > (m_allslot.at(s)._height - m_knifetype.at(v_j)._diameter))
						{
							min_dis = m_allslot.at(s)._height - m_knifetype.at(v_j)._diameter;
							v_key = v_j;
						}
					}

					if (v_key == -1)
					{
						CString info;
						info.Format("�����%s��Ӧ�ۿ�%0.1f�޷�����\r\n",panel_info.c_str(),oneks._height);
						//AfxMessageBox(info);
						vs_info = vs_info + info;
						continue;
					}

					oneks._diameter = m_knifetype.at(v_key)._diameter;
					oneks._tgcodeb = m_knifetype.at(v_key)._tgcodeb;
					oneks._tgcdoee = m_knifetype.at(v_key)._tgcodee;
					oneks._feedrate = m_knifetype.at(v_key)._feedrate;
					oneks._spindlespeed = m_knifetype.at(v_key)._spindlespeed;

					// �����ظ���
					bool IsTheSameSlot = false;
					for (int solt_i = 0;solt_i < p_solt.size();solt_i++)
					{
						KnifeTypeAndSlot ExistSolt = p_solt.at(solt_i);
						
						int ExistSoltDir		= ExistSolt._dir;
						float detection_x		= abs(ExistSolt._x - oneks._x + ExistSolt._srcx - oneks._srcx);
						float detection_y		= abs(ExistSolt._y - oneks._y + ExistSolt._srcy - oneks._srcy);
						float detection_width	= abs(ExistSolt._width	- oneks._width);
						float detection_height	= abs(ExistSolt._height	- oneks._height);
						float detection_depth	= abs(ExistSolt._depth	- oneks._depth);

						if (detection_x == 0			&&
							detection_y == 0			&&
							ExistSoltDir == oneks._dir	&&
							detection_width == 0		&&
							detection_height == 0		&&
							detection_depth == 0  )
						{
							IsTheSameSlot = true;
							break;
						}
					}
					if (IsTheSameSlot == false)
					{
						p_solt.push_back(oneks);
					}
				}
			}
		}

		float _k_d = -1.0;
		KnifeType last_knife;	// ������ۿ���ͬһ�ѵ���������

		// ��ʼ��
		last_knife.kt = -1;
		last_knife._diameter =  -1;
		last_knife._kn = -1;


		// ���ݵ�����׿���
		for (int v_i = 0;v_i < m_knifetype.size();v_i++)	// ÿ�ѵ�������׿���
		{
			if (m_knifetype.at(v_i).kt == 6 || m_knifetype.at(v_i).kt == 7)
			{
				continue;
			}

			float _d = m_knifetype.at(v_i)._diameter;
			std::string _tgcodeb = m_knifetype.at(v_i)._tgcodeb;
			std::string _tgcodee = m_knifetype.at(v_i)._tgcodee;
			std::vector<AllVecK> _vec;

			for (int v_j = 0;v_j < pvec.size();v_j++)	// ѡȡ���ʵĿ�
			{
				if (pvec.at(v_j)._diameter - _d > -0.01 && pvec.at(v_j)._diameter - _d < 0.01 && (m_knifetype.at(v_i).kt == 0 || m_knifetype.at(v_i).kt == 1 || m_knifetype.at(v_i).kt == 2))
				{
					AllVecK onevec;
					onevec.panel_info = pvec.at(v_j).panel_info;
					onevec._pheight = pvec.at(v_j)._pheight;
					onevec._pwidth = pvec.at(v_j)._pwidth;
					onevec._depth = pvec.at(v_j)._depth;
					onevec._r = pvec.at(v_j)._r;
					onevec._x = pvec.at(v_j)._x;
					onevec._y = pvec.at(v_j)._y;
					onevec._srcx = pvec.at(v_j)._srcx;
					onevec._srcy = pvec.at(v_j)._srcy;

					_vec.push_back(onevec);
				}
			}

			std::vector<AllSlotK> _slot;	// ѡȡ���ʵĲ�
			for (int s_j = 0;s_j < p_solt.size();s_j++)
			{
				if (p_solt.at(s_j)._diameter - _d > -0.01 && p_solt.at(s_j)._diameter - _d < 0.01 && (m_knifetype.at(v_i).kt == 0 || m_knifetype.at(v_i).kt == 1 || m_knifetype.at(v_i).kt == 3 ||  m_knifetype.at(v_i).kt == 5))
				{
					AllSlotK onesolt;
					onesolt.panel_info = p_solt.at(s_j).panel_info;
					onesolt._pheight = p_solt.at(s_j)._pheight;
					onesolt._pwidth = p_solt.at(s_j)._pwidth;
					onesolt._depth = p_solt.at(s_j)._depth;
					onesolt._dir = p_solt.at(s_j)._dir;
					onesolt._height = p_solt.at(s_j)._height;
					onesolt._width = p_solt.at(s_j)._width;
					onesolt._x = p_solt.at(s_j)._x;
					onesolt._y = p_solt.at(s_j)._y;
					onesolt._srcx = p_solt.at(s_j)._srcx;
					onesolt._srcy = p_solt.at(s_j)._srcy;

					_slot.push_back(onesolt);
				}
			}

			if (_vec.size() > 0 || _slot.size() > 0)	// д��ʼ����
			{
				for (int _g_i = 0;_g_i < _tgcodeb.size();_g_i++)
				{
					if (_tgcodeb.at(_g_i) == '@')
					{
						wfile<<endl;
					}
					else
					{
						wfile<<_tgcodeb.at(_g_i);
					}
				}
				_k_d = m_knifetype.at(v_i)._diameter;

				wfile<<endl;
			}


			// ��׿�ʼ
			if (_vec.size() > 0)
			{
				std::vector<pos> poses;
				std::vector<pos> newposes;
				for (int p_i = 0;p_i < _vec.size();p_i++)
				{
					pos one_pos;
					one_pos.x = _vec.at(p_i)._x + _vec.at(p_i)._srcx;
					one_pos.y = _vec.at(p_i)._y + _vec.at(p_i)._srcy;
					poses.push_back(one_pos);
				}

				pos begin_pos;
				begin_pos.x = 0;
				begin_pos.y = 0;
				GetShortestPath(poses,begin_pos,newposes);
				poses.clear();

				for (int p_i = 0;p_i < newposes.size();p_i++)
				{
					poses.push_back(newposes.at(p_i));
				}

				newposes.clear();
				GetChangePath(poses,newposes);
				std::vector<AllVecK> tmppvec;

				// 2017-11-14 yuanzb ����ͬһ��λ�ô������ף������ظ�������⣬���newposes����������_vec��������ͬ
				//					 ֱ�Ӳ���_vec��ֵ,�����ͬ����ԭ������
				if (newposes.size() == _vec.size())
				{
					tmppvec.insert(tmppvec.end(), _vec.begin(), _vec.end());
				}
				else
				{
					for (int p_j = 0;p_j < newposes.size();p_j++)
					{
						pos cur_pos = newposes.at(p_j);

						for (int p_i = 0;p_i < _vec.size();p_i++)
						{
							AllVecK cur_vec = _vec.at(p_i);

							float detect_x = abs(cur_pos.x - (cur_vec._x + cur_vec._srcx));
							float detect_y = abs(cur_pos.y - (cur_vec._y + cur_vec._srcy));

							if (detect_x == 0 &&
								detect_y == 0 )
							{
								tmppvec.push_back(_vec.at(p_i));
							}
						}
					}
				}
				

				_vec.clear();
				for (int p_i = 0;p_i < tmppvec.size();p_i++)
				{
					_vec.push_back(tmppvec.at(p_i));
				}

				// ���
				for (int v_o = 0;v_o < _vec.size();v_o++)
				{
					float v_x = _vec.at(v_o)._x + _vec.at(v_o)._srcx;
					float v_y = _vec.at(v_o)._y + _vec.at(v_o)._srcy;
					if (coortype == 1)
					{
						v_y = -1*_vec.at(v_o)._y + _vec.at(v_o)._srcy;
					}
					else if (coortype == 2)
					{
						v_y = -1*_vec.at(v_o)._y + _vec.at(v_o)._srcy;
					}
					float v_r = _vec.at(v_o)._r;
					float v_depth;

					if (gcodeparam.downheight > 1000)
					{
						v_depth = -1* _vec.at(v_o)._depth;

					}
					else
					{
						v_depth = _thickness -  _vec.at(v_o)._depth;
						if (gcodeparam.downheight > 0.5)
						{
							v_depth = v_depth - gcodeparam.downheight;
						}
					}



					char chheight1[12],chheight2[12],chheight3[12];
					//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
					//sprintf_s(chheight2,"%0.4f",v_depth);

					PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);
					PrintNumber2Array(chheight2, 12, decimal_place, v_depth);

					if (gcodeparam.downheight > 1000)
					{
						//sprintf_s(chheight3,"%0.4f",0.0000);
						PrintNumber2Array(chheight3,  12, decimal_place, 0.0000);
					}
					else
					{
						//sprintf_s(chheight3,"%0.4f",_thickness);
						PrintNumber2Array(chheight3, 12, decimal_place, _thickness);
						if (gcodeparam.downheight > 0.5)
						{
							//sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
							PrintNumber2Array(chheight3,  12, decimal_place,_thickness - gcodeparam.downheight);
						}
					}

					if (_vec.at(v_o)._r > -0.01 && _vec.at(v_o)._r < 0.01)
					{
						char chrx1[12],chry1[12];
						//sprintf_s(chrx1,"%0.4f",v_x);
						//sprintf_s(chry1,"%0.4f",v_y);
						PrintNumber2Array(chrx1, 12, decimal_place, v_x);
						PrintNumber2Array(chry1, 12, decimal_place, v_y);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3;
							if (m_knifetype.at(v_i)._facespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}

					}
					else if ((_vec.at(v_o)._r - 0.6)/_d <= 1.0 || (_vec.at(v_o)._depth - _thickness > -0.01 &&  _vec.at(v_o)._depth - _thickness < 0.01))
					{
						char chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];

						//sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry1,"%0.4f",v_y);
						//sprintf_s(chrx2,"%0.4f",v_x);
						//sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						//sprintf_s(chry3,"%0.4f",v_y);
						//sprintf_s(chrx4,"%0.4f",v_x);
						//sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry5,"%0.4f",v_y);
						//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

						PrintNumber2Array(chrx1, 12, decimal_place,	v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry1, 12, decimal_place,	v_y);
						PrintNumber2Array(chrx2, 12, decimal_place,	v_x);
						PrintNumber2Array(chry2, 12, decimal_place,	v_y + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx3, 12, decimal_place,	v_x + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry3, 12, decimal_place,	v_y);
						PrintNumber2Array(chrx4, 12, decimal_place,	v_x);
						PrintNumber2Array(chry4, 12, decimal_place,	v_y - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx5, 12, decimal_place,	v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry5, 12, decimal_place,	v_y);
						PrintNumber2Array(chr,	 12, decimal_place,	_vec.at(v_o)._r/2.0);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight3;
							if (m_knifetype.at(v_i)._facespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._spindlespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else if ((_vec.at(v_o)._r - 0.6)/_d > 1.0 && (_vec.at(v_o)._r - 0.6)/_d <= 2)
					{
						char chrx0[12],chry0[12],chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];

						//sprintf_s(chrx0,"%0.4f",v_x);
						//sprintf_s(chry0,"%0.4f",v_y);
						//sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry1,"%0.4f",v_y);
						//sprintf_s(chrx2,"%0.4f",v_x);
						//sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						//sprintf_s(chry3,"%0.4f",v_y);
						//sprintf_s(chrx4,"%0.4f",v_x);
						//sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry5,"%0.4f",v_y);
						//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);


						PrintNumber2Array(chrx0, 12, decimal_place, v_x);
						PrintNumber2Array(chry0, 12, decimal_place, v_y);
						PrintNumber2Array(chrx1, 12, decimal_place, v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry1, 12, decimal_place, v_y);
						PrintNumber2Array(chrx2, 12, decimal_place, v_x);
						PrintNumber2Array(chry2, 12, decimal_place, v_y + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx3, 12, decimal_place, v_x + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry3, 12, decimal_place, v_y);
						PrintNumber2Array(chrx4, 12, decimal_place, v_x);
						PrintNumber2Array(chry4, 12, decimal_place, v_y - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx5, 12, decimal_place, v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry5, 12, decimal_place, v_y);
						PrintNumber2Array(chr,	 12, decimal_place, _vec.at(v_o)._r/2.0);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight3;
							if (m_knifetype.at(v_i)._facespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry0<<" Y "<<chrx0<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._spindlespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx0<<" Y "<<chry0<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else if ((_vec.at(v_o)._r - 0.6)/_d > 2 && (_vec.at(v_o)._r - 0.6)/_d <= 3)
					{
						char chrx1[12],chry1[12],chrx2[12],chry2[12],chrx3[12],chry3[12],chrx4[12],chry4[12],chrx5[12],chry5[12],chr[12];
						char chrx12[12],chry12[12],chrx22[12],chry22[12],chrx32[12],chry32[12],chrx42[12],chry42[12],chrx52[12],chry52[12],chr2[12];

						//sprintf_s(chrx1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry1,"%0.4f",v_y);
						//sprintf_s(chrx2,"%0.4f",v_x);
						//sprintf_s(chry2,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx3,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
						//sprintf_s(chry3,"%0.4f",v_y);
						//sprintf_s(chrx4,"%0.4f",v_x);
						//sprintf_s(chry4,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
						//sprintf_s(chrx5,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
						//sprintf_s(chry5,"%0.4f",v_y);
						//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
						//
						//sprintf_s(chrx12,"%0.4f",v_x - _d/2.0);
						//sprintf_s(chry12,"%0.4f",v_y);
						//sprintf_s(chrx22,"%0.4f",v_x);
						//sprintf_s(chry22,"%0.4f",v_y + _d/2.0);
						//sprintf_s(chrx32,"%0.4f",v_x + _d/2.0);
						//sprintf_s(chry32,"%0.4f",v_y);
						//sprintf_s(chrx42,"%0.4f",v_x);
						//sprintf_s(chry42,"%0.4f",v_y - _d/2.0);
						//sprintf_s(chrx52,"%0.4f",v_x - _d/2.0);
						//sprintf_s(chry52,"%0.4f",v_y);
						//sprintf_s(chr2,"%0.4f",_d/2.0);

						PrintNumber2Array(chrx1, 12, decimal_place,v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry1, 12, decimal_place,v_y);
						PrintNumber2Array(chrx2, 12, decimal_place,v_x);
						PrintNumber2Array(chry2, 12, decimal_place,v_y + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx3, 12, decimal_place,v_x + _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry3, 12, decimal_place,v_y);
						PrintNumber2Array(chrx4, 12, decimal_place,v_x);
						PrintNumber2Array(chry4, 12, decimal_place,v_y - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chrx5, 12, decimal_place,v_x - _vec.at(v_o)._r/2.0);
						PrintNumber2Array(chry5, 12, decimal_place,v_y);
						PrintNumber2Array(chr,	 12, decimal_place,_vec.at(v_o)._r/2.0);

						PrintNumber2Array(chrx12,	 12, decimal_place,v_x - _d/2.0);
						PrintNumber2Array(chry12,	 12, decimal_place,v_y);
						PrintNumber2Array(chrx22,	 12, decimal_place,v_x);
						PrintNumber2Array(chry22,	 12, decimal_place,v_y + _d/2.0);
						PrintNumber2Array(chrx32,	 12, decimal_place,v_x + _d/2.0);
						PrintNumber2Array(chry32,	 12, decimal_place,v_y);
						PrintNumber2Array(chrx42,	 12, decimal_place,v_x);
						PrintNumber2Array(chry42,	 12, decimal_place,v_y - _d/2.0);
						PrintNumber2Array(chrx52,	 12, decimal_place,v_x - _d/2.0);
						PrintNumber2Array(chry52,	 12, decimal_place,v_y);
						PrintNumber2Array(chr2,		 12, decimal_place,_d/2.0);


						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight3;
							if (m_knifetype.at(v_i)._facespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry12<<" Y "<<chrx12<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._spindlespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry22<<" Y "<<chrx22<<" R "<<chr2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry32<<" Y "<<chrx32<<" R "<<chr2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry42<<" Y "<<chrx42<<" R "<<chr2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry52<<" Y "<<chrx52<<" R "<<chr2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._spindlespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry1<<" Y "<<chrx1<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry2<<" Y "<<chrx2<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry3<<" Y "<<chrx3<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry4<<" Y "<<chrx4<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G90 G03"<<" X "<<chry5<<" Y "<<chrx5<<" R "<<chr;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
							wfile<<"G00"<<" X "<<chry5<<" Y "<<chrx5<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
						}
						else if (coortype == 1)
						{
							wfile<<"G00"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx22<<" Y "<<chry22<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx32<<" Y "<<chry32<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx42<<" Y "<<chry42<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx52<<" Y "<<chry52<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
						else if (coortype == 2)
						{
							wfile<<"G00"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx12<<" Y "<<chry12<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx22<<" Y "<<chry22<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx32<<" Y "<<chry32<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx42<<" Y "<<chry42<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx52<<" Y "<<chry52<<" R "<<chr2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._spindlespeed<<endl;
							wfile<<"G01"<<" X "<<chrx1<<" Y "<<chry1<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx2<<" Y "<<chry2<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx3<<" Y "<<chry3<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx4<<" Y "<<chry4<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G90 G02"<<" X "<<chrx5<<" Y "<<chry5<<" R "<<chr<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
							wfile<<"G00"<<" X "<<chrx5<<" Y "<<chry5<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
						}
					}
					else
					{
						char chrx[12],chry[12];
						//sprintf_s(chrx,"%0.4f",v_x);
						//sprintf_s(chry,"%0.4f",v_y);
						PrintNumber2Array(chrx, 12, decimal_place,v_x);
						PrintNumber2Array(chry, 12, decimal_place,v_y);

						if (coortype == 0)
						{
							wfile<<"G00"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight1;
							if (m_knifetype.at(v_i)._overspend > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._overspend;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight3;
							if (m_knifetype.at(v_i)._facespeed > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
							wfile<<endl;
							wfile<<"G01"<<" X "<<chry<<" Y "<<chrx<<" Z "<<chheight2;
							if (m_knifetype.at(v_i)._feedrate > 0.01)
								wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
							wfile<<endl;
						}

						float begin_x,begin_y,end_x,end_y,last_x,last_y;
						char chbegin_x1[12],chbegin_y1[12],chend_x1[12],chend_y1[12],chr[12],chbegin_x2[12],chbegin_y2[12],chend_x2[12],chend_y2[12];
						begin_x = v_x;
						begin_y = v_y;

						float dis_r = 0.75*_d/2.0;

						for (int r_i = 1;r_i < 100;r_i++)
						{
							if (r_i%2 == 1)
							{
								end_x = begin_x + dis_r * r_i * 2.0;
								end_y = begin_y;
								last_x = v_x + dis_r;
								last_y = v_y;
								begin_x = end_x;
								begin_y = end_y;

								if (end_x > v_x + _vec.at(v_o)._r/2.0 || end_x < v_x - _vec.at(v_o)._r/2.0)
								{
									float intersection_x,intersection_y;
									intersection_x = (((dis_r * r_i)*(dis_r * r_i) - (_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0))/(v_x - last_x) + v_x + last_x)/2.0;
									if (r_i%2 == 1)
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x)) + v_y;
									}
									else
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x))*(-1) + v_y;
									}

									//sprintf_s(chend_x1,"%0.4f",intersection_x);
									//sprintf_s(chend_y1,"%0.4f",intersection_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);


									PrintNumber2Array(chend_x1,	12, decimal_place,intersection_x);
									PrintNumber2Array(chend_y1,	12, decimal_place,intersection_y);
									PrintNumber2Array(chr,		12, decimal_place,dis_r * r_i);


									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

									PrintNumber2Array(chend_x1,	12, decimal_place,v_x + _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chend_y1,	12, decimal_place,v_y);
									PrintNumber2Array(chr,		12, decimal_place,_vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);


									PrintNumber2Array(chend_x1,	12, decimal_place,v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place,v_y - _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chr,		12, decimal_place,_vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}


									//sprintf_s(chend_x1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);


									PrintNumber2Array(chend_x1,		12, decimal_place, v_x - _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chend_y1,		12, decimal_place, v_y);
									PrintNumber2Array(chr,			12, decimal_place, _vec.at(v_o)._r/2.0);


									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,	"%0.4f",v_x);
									//sprintf_s(chend_y1,	"%0.4f",v_y + _vec.at(v_o)._r/2.0);
									//sprintf_s(chr,		"%0.4f",_vec.at(v_o)._r/2.0);


									PrintNumber2Array(chend_x1,	12, decimal_place, v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, v_y + _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}


									//sprintf_s(chend_x1,"%0.4f",intersection_x);
									//sprintf_s(chend_y1,"%0.4f",intersection_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

									PrintNumber2Array(chend_x1,	12, decimal_place, intersection_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, intersection_y);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);

									PrintNumber2Array(chend_x1,	12, decimal_place, v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, v_y);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/4.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1;
										if (m_knifetype.at(v_i)._overspend > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._overspend;
										wfile<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else
								{
									//sprintf_s(chend_x1,"%0.4f",last_x);
									//sprintf_s(chend_y1,"%0.4f",last_y + dis_r * r_i);
									//sprintf_s(chend_x2,"%0.4f",last_x + dis_r * r_i);
									//sprintf_s(chend_y2,"%0.4f",last_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);

									PrintNumber2Array(chend_x1,		12, decimal_place,	last_x);
									PrintNumber2Array(chend_y1,		12, decimal_place,	last_y + dis_r * r_i);
									PrintNumber2Array(chend_x2,		12, decimal_place,	last_x + dis_r * r_i);
									PrintNumber2Array(chend_y2,		12, decimal_place,	last_y);
									PrintNumber2Array(chr,			12, decimal_place,	dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
								}


							}
							else
							{
								end_x = begin_x - dis_r * r_i * 2.0;
								end_y = begin_y;
								last_x = v_x;
								last_y = v_y;
								begin_x = end_x;
								begin_y = end_y;

								if (_vec.at(v_o)._r/2.0 - dis_r * r_i > -0.01 && _vec.at(v_o)._r/2.0 - dis_r * r_i < 0.01)
								{
									//sprintf_s(chend_x1,"%0.4f",last_x);
									//sprintf_s(chend_y1,"%0.4f",last_y - dis_r * r_i);
									//sprintf_s(chend_x2,"%0.4f",last_x - dis_r * r_i);
									//sprintf_s(chend_y2,"%0.4f",last_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);

									PrintNumber2Array(chend_x1, 	12, decimal_place, last_x);
									PrintNumber2Array(chend_y1, 	12, decimal_place, last_y - dis_r * r_i);
									PrintNumber2Array(chend_x2, 	12, decimal_place, last_x - dis_r * r_i);
									PrintNumber2Array(chend_y2, 	12, decimal_place, last_y);
									PrintNumber2Array(chr,			12, decimal_place, dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",last_x);
									//sprintf_s(chend_y1,"%0.4f",last_y + dis_r * r_i);
									//sprintf_s(chend_x2,"%0.4f",last_x + dis_r * r_i);
									//sprintf_s(chend_y2,"%0.4f",last_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);

									PrintNumber2Array(chend_x1,	12, decimal_place, last_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, last_y + dis_r * r_i);
									PrintNumber2Array(chend_x2,	12, decimal_place, last_x + dis_r * r_i);
									PrintNumber2Array(chend_y2,	12, decimal_place, last_y);
									PrintNumber2Array(chr,		12, decimal_place, dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);

									PrintNumber2Array(chend_x1,	12, decimal_place,	v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place,	v_y);
									PrintNumber2Array(chr,		12, decimal_place,	_vec.at(v_o)._r/4.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1;
										if (m_knifetype.at(v_i)._overspend > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._overspend;
										wfile<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_x1<<" Y "<<chend_y1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else if (end_x > v_x + _vec.at(v_o)._r/2.0 || end_x < v_x - _vec.at(v_o)._r/2.0)
								{
									float intersection_x,intersection_y;
									intersection_x = (((dis_r * r_i)*(dis_r * r_i) - (_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0))/(v_x - last_x) + v_x + last_x)/2.0;
									if (r_i%2 == 1)
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x)) + v_y;
									}
									else
									{
										intersection_y = sqrt((_vec.at(v_o)._r/2.0)*(_vec.at(v_o)._r/2.0) - (intersection_x - v_x)*(intersection_x - v_x))*(-1) + v_y;
									}

									//sprintf_s(chend_x1,"%0.4f",intersection_x);
									//sprintf_s(chend_y1,"%0.4f",intersection_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);
									
									PrintNumber2Array(chend_x1,		12, decimal_place,	intersection_x);
									PrintNumber2Array(chend_y1,		12, decimal_place,	intersection_y);
									PrintNumber2Array(chr,			12, decimal_place,	dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x - _vec.at(v_o)._r/2.0);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

									PrintNumber2Array(chend_x1,		12, decimal_place, v_x - _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chend_y1,		12, decimal_place, v_y);
									PrintNumber2Array(chr,			12, decimal_place, _vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y + _vec.at(v_o)._r/2.0);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

									PrintNumber2Array(chend_x1,		12, decimal_place,	v_x);
									PrintNumber2Array(chend_y1,		12, decimal_place,	v_y + _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chr,			12, decimal_place,	_vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x + _vec.at(v_o)._r/2.0);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);
									
									PrintNumber2Array(chend_x1,	12, decimal_place, v_x + _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chend_y1,	12, decimal_place, v_y);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y - _vec.at(v_o)._r/2.0);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/2.0);

									PrintNumber2Array(chend_x1,	12, decimal_place, v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, v_y - _vec.at(v_o)._r/2.0);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/2.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",intersection_x);
									//sprintf_s(chend_y1,"%0.4f",intersection_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);

									PrintNumber2Array(chend_x1,	12, decimal_place, intersection_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, intersection_y);
									PrintNumber2Array(chr,		12, decimal_place, dis_r * r_i);


									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
									}

									//sprintf_s(chend_x1,"%0.4f",v_x);
									//sprintf_s(chend_y1,"%0.4f",v_y);
									//sprintf_s(chr,"%0.4f",_vec.at(v_o)._r/4.0);


									PrintNumber2Array(chend_x1,	12, decimal_place, v_x);
									PrintNumber2Array(chend_y1,	12, decimal_place, v_y);
									PrintNumber2Array(chr,		12, decimal_place, _vec.at(v_o)._r/4.0);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1;
										if (m_knifetype.at(v_i)._overspend < -0.01 && m_knifetype.at(v_i)._overspend > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._overspend;
										wfile<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G00"<<" X "<<chend_y1<<" Y "<<chend_x1<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
									}
									break;
								}
								else
								{
									//sprintf_s(chend_x1,"%0.4f",last_x);
									//sprintf_s(chend_y1,"%0.4f",last_y - dis_r * r_i);
									//sprintf_s(chend_x2,"%0.4f",last_x - dis_r * r_i);
									//sprintf_s(chend_y2,"%0.4f",last_y);
									//sprintf_s(chr,"%0.4f",dis_r * r_i);


									PrintNumber2Array(chend_x1,	12, decimal_place,	last_x);
									PrintNumber2Array(chend_y1,	12, decimal_place,	last_y - dis_r * r_i);
									PrintNumber2Array(chend_x2,	12, decimal_place,	last_x - dis_r * r_i);
									PrintNumber2Array(chend_y2,	12, decimal_place,	last_y);
									PrintNumber2Array(chr,		12, decimal_place,	dis_r * r_i);

									if (coortype == 0)
									{
										wfile<<"G90 G03"<<" X "<<chend_y1<<" Y "<<chend_x1<<" R "<<chr<<endl;
										wfile<<"G90 G03"<<" X "<<chend_y2<<" Y "<<chend_x2<<" R "<<chr<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G90 G02"<<" X "<<chend_x1<<" Y "<<chend_y1<<" R "<<chr<<endl;
										wfile<<"G90 G02"<<" X "<<chend_x2<<" Y "<<chend_y2<<" R "<<chr<<endl;
									}
								}
							}

						}
					}

				}
			}


			// ��ʼ����
			if (_slot.size() > 0)	
			{
				for (int p_s = 0;p_s < _slot.size();p_s++)
				{
					float s_x = _slot.at(p_s)._x;
					float s_y = _slot.at(p_s)._y;
					if (coortype != 0)
					{
						s_y = -1*s_y;
					}
					float s_width = _slot.at(p_s)._width;
					float s_height = _slot.at(p_s)._height;
					float s_depth;
					if (gcodeparam.downheight > 1000)
					{
						s_depth = -1*_slot.at(p_s)._depth;
					}
					else
					{
						s_depth = _thickness - _slot.at(p_s)._depth;
						if (gcodeparam.downheight > 0.5)
						{
							s_depth = s_depth - gcodeparam.downheight;
						}
					}

					int s_dir = _slot.at(p_s)._dir;

					char chheight1[12],chheight2[12],chheight3[12];
					//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
					//sprintf_s(chheight2,"%0.4f",s_depth);


					PrintNumber2Array(chheight1,	12, decimal_place, gcodeparam.upheight);
					PrintNumber2Array(chheight2,	12, decimal_place, s_depth);

					if (gcodeparam.downheight > 1000)
					{
						//sprintf_s(chheight3,"%0.4f",0.0000);	
						PrintNumber2Array(chheight3, 12, decimal_place, 0.0000);	
					}
					else
					{
						//sprintf_s(chheight3,"%0.4f",_thickness);
						PrintNumber2Array(chheight3, 12, decimal_place,_thickness);

						if (gcodeparam.downheight > 0.5)
						{
							//sprintf_s(chheight3,"%0.4f",_thickness - gcodeparam.downheight);
							PrintNumber2Array(chheight3, 12, decimal_place,_thickness - gcodeparam.downheight);

						}
					}

					if (s_dir == 1)		//���򿪲�
					{
						//ֻ��һ�²ۣ���ֱ���Ͳۿ�һ��
						if(_d - s_height > -0.01 && _d - s_height < 0.01)
						{
							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float begin_x,begin_y,end_x,end_y;
							begin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 /*+ _d/2.0*/;		// 2016-7-28 yuanzb ����ʱ���Ӿ�죬����ʱ��������
							begin_y = s_y + _slot.at(p_s)._srcy;
							end_x = s_x + _slot.at(p_s)._srcx + s_width/2.0 /*- _d/2.0*/;
							end_y = s_y + _slot.at(p_s)._srcy;

							//sprintf_s(chbegin_x,"%0.4f",begin_y);
							//sprintf_s(chbegin_y,"%0.4f",begin_x);
							//sprintf_s(chend_x,"%0.4f",end_y);
							//sprintf_s(chend_y,"%0.4f",end_x);


							PrintNumber2Array(chbegin_x, 12, decimal_place, begin_y);
							PrintNumber2Array(chbegin_y, 12, decimal_place, begin_x);
							PrintNumber2Array(chend_x,	 12, decimal_place, end_y);
							PrintNumber2Array(chend_y,	 12, decimal_place, end_x);


							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._facespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
								if (m_knifetype.at(v_i)._spindlespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
								if (m_knifetype.at(v_i)._feedrate > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._feedrate > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
								wfile<<endl;
								wfile<<"G00"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
							}
						}
						else	// ���죿������ �����ֲۣ��ȵ�Ҫ��ʱ��Ҫ����������
						{
							int big_kerfwidth = (int)(_d * 10 + 0.5);
							int big_sheight = (int)(s_height * 10 + 0.5);

							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float sbegin_x,sbegin_y,send_x,send_y;

							float begin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 /*+ _d/2.0*/;		// 2016-8-23 yuanzb ����ʱ���Ӿ�죬����ʱ��������
							float begin_y = s_y - s_height/2.0 + _d/2.0 + _slot.at(p_s)._srcy;

							//sprintf_s(chbegin_x,"%0.4f",begin_y);
							//sprintf_s(chbegin_y,"%0.4f",begin_x);


							PrintNumber2Array(chbegin_x, 12, decimal_place, begin_y);
							PrintNumber2Array(chbegin_y, 12, decimal_place, begin_x);

							// ��ʼ�µ�
							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._facespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
								wfile<<endl;
							}


							int s_key = 0;

							int add_flag = big_sheight%big_kerfwidth != 0 ? 1 : 0;
							int total_time =  big_sheight/big_kerfwidth + add_flag;

							// 2017-5-16 yuanzb ���۵���С�ڵ��Ǻܽӽ��ۿ���Ҫ����һ��
							if (total_time == 1)
							{
								total_time = 2;
							}

							for (int s_k = 0; s_k < total_time ; s_k++)
							{
								s_key = s_k;
								char schbegin_x[12],schbegin_y[12],schend_x[12],schend_y[12];

								sbegin_x = s_x + _slot.at(p_s)._srcx - s_width/2.0 /*+ _d/2.0*/;		// 2016-8-23 yuanzb ����ʱ���Ӿ�죬����ʱ��������
								sbegin_y = s_y - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2.0 : _d/2.0 + s_k*_d) + _slot.at(p_s)._srcy;
								send_x = s_x + s_width/2.0 + _slot.at(p_s)._srcx /*- _d/2.0*/;
								send_y = s_y - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2.0 : _d/2.0 + s_k*_d) + _slot.at(p_s)._srcy;

								//sprintf_s(chbegin_x,"%0.4f",sbegin_y);
								//sprintf_s(chbegin_y,"%0.4f",sbegin_x);
								//sprintf_s(chend_x,"%0.4f",send_y);
								//sprintf_s(chend_y,"%0.4f",send_x);


								PrintNumber2Array(chbegin_x, 12, decimal_place, sbegin_y);
								PrintNumber2Array(chbegin_y, 12, decimal_place, sbegin_x);
								PrintNumber2Array(chend_x,	 12, decimal_place, send_y);
								PrintNumber2Array(chend_y,	 12, decimal_place, send_x);

								if (s_k%2 == 0)
								{
									if (s_k != 0)
									{
										float pre_y = s_y - s_height/2 + _d/2.0 + s_k*_d + _slot.at(p_s)._srcy - _d*2.0;
										char pre_sx[12];

										//sprintf_s(pre_sx,"%0.4f",pre_y);
										PrintNumber2Array(pre_sx, 12, decimal_place, pre_y);

										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2;
											if (m_knifetype.at(v_i)._feedrate > 0.01)
												wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
											wfile<<endl;
										}
									}

									if (coortype == 0)
									{	
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._spindlespeed > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
										wfile<<endl;
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
									}

									if (s_k == total_time/*big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0)*/)
									{
										//sprintf_s(chend_x,"%0.4f",send_y);
										//sprintf_s(chend_y,"%0.4f",send_x);

										PrintNumber2Array(chend_x, 12, decimal_place, send_y);
										PrintNumber2Array(chend_y, 12, decimal_place, send_x);
									}
								}
								else
								{
									if (s_k != 1)
									{
										float pre_y = s_y - s_height/2 + _d/2.0 + s_k*_d + _slot.at(p_s)._srcy - _d*2.0;
										char pre_sx[12];
										//sprintf_s(pre_sx,"%0.4f",pre_y);
										PrintNumber2Array(pre_sx, 12, decimal_place, pre_y);


										if (s_k == 0)
										{
											wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight2;
											if (m_knifetype.at(v_i)._spindlespeed > 0.01)
												wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
											wfile<<endl;
										}
										else
										{
											wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight2;
											if (m_knifetype.at(v_i)._feedrate > 0.01)
												wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
											wfile<<endl;
										}
									}
									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
									}
									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										//sprintf_s(chend_x,"%0.4f",sbegin_y);
										//sprintf_s(chend_y,"%0.4f",sbegin_x);
										PrintNumber2Array(chend_x, 12, decimal_place, sbegin_y);
										PrintNumber2Array(chend_y, 12, decimal_place, sbegin_x);
									}
								}

							}

						



							if (s_key%2 == 0 && s_key != 0)
							{
								float pre_y = s_y - s_height/2 + _d/2.0 + s_key*_d + _slot.at(p_s)._srcy - _d*2.0;
								char pre_sx[12];
								//sprintf_s(pre_sx,"%0.4f",pre_y);
								PrintNumber2Array(pre_sx, 12, decimal_place, pre_y);

								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chend_y<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chend_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}

							if (s_key%2 == 1 && s_key != 1)
							{
								float pre_y = s_y - s_height/2 + _d/2.0 + s_key*_d + _slot.at(p_s)._srcy - _d*2.0;
								char pre_sx[12];
								//sprintf_s(pre_sx,"%0.4f",pre_y);
								PrintNumber2Array(pre_sx, 12, decimal_place, pre_y);


								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}

							if (s_key == 1)
							{
								float pre_y = s_y - s_height/2 + ((0 + 1)*_d > s_height ? s_height - _d/2 : _d/2) + _slot.at(p_s)._srcy;
								char pre_sx[12];
								//sprintf_s(pre_sx,"%0.4f",pre_y);
								PrintNumber2Array(pre_sx, 12, decimal_place , pre_y);


								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<pre_sx<<" Y "<<chbegin_y<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<chbegin_y<<" Y "<<pre_sx<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}
						}
					}
					else				//���򿪲�
					{
						if(_d - s_height > -0.01 && _d - s_height < 0.01)
						{
							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float begin_x,begin_y,end_x,end_y;
							begin_x = s_x + _slot.at(p_s)._srcx;
							begin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 /*+ _d/2.0*/;	// 2016-7-28 yuanzb ����ʱ���Ӿ�죬����ʱ��������
							end_x = s_x + _slot.at(p_s)._srcx;
							end_y = s_y + _slot.at(p_s)._srcy + s_width/2.0 /*- _d/2.0*/;

							//sprintf_s(chbegin_x,"%0.4f",begin_y);
							//sprintf_s(chbegin_y,"%0.4f",begin_x);
							//sprintf_s(chend_x,"%0.4f",end_y);
							//sprintf_s(chend_y,"%0.4f",end_x);

							PrintNumber2Array(chbegin_x, 12, decimal_place, begin_y);
							PrintNumber2Array(chbegin_y, 12, decimal_place, begin_x);
							PrintNumber2Array(chend_x,	 12, decimal_place, end_y);
							PrintNumber2Array(chend_y,	 12, decimal_place, end_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._facespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
								if (m_knifetype.at(v_i)._spindlespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
								if (m_knifetype.at(v_i)._feedrate > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._endspeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
								wfile<<endl;
								wfile<<"G00"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
							}
						}
						else // ���죿������ �����ֲۣ��ȵ�Ҫ��ʱ��Ҫ���������� ͬ��
						{
							int big_kerfwidth = (int)(_d * 10 + 0.5);
							int big_sheight = (int)(s_height * 10 + 0.5);

							char chbegin_x[12],chbegin_y[12],chend_x[12],chend_y[12];
							float sbegin_x,sbegin_y,send_x,send_y;

							float begin_x = s_x - s_height/2 + _d/2.0 + _slot.at(p_s)._srcx;
							float begin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 /*+ _d/2.0*/;	// 2016-8-23 yuanzb ����ʱ���Ӿ�죬����ʱ��������

							//sprintf_s(chbegin_x,"%0.4f",begin_y);
							//sprintf_s(chbegin_y,"%0.4f",begin_x);

							PrintNumber2Array(chbegin_x, 12, decimal_place, begin_y);
							PrintNumber2Array(chbegin_y, 12, decimal_place, begin_x);

							if (coortype == 0)
							{
								wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight1;
								if (m_knifetype.at(v_i)._overspend > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._overspend;
								wfile<<endl;
								wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight3;
								if (m_knifetype.at(v_i)._facespeed > 0.01)
									wfile<<" F "<<m_knifetype.at(v_i)._facespeed;
								wfile<<endl;
							}

							int s_key = 0;


							int add_flag = big_sheight%big_kerfwidth != 0 ? 1 : 0;
							int total_time =  big_sheight/big_kerfwidth + add_flag;

							// 2017-5-16 yuanzb ���۵���С�ڵ��Ǻܽӽ��ۿ���Ҫ����һ��
							if (total_time == 1)
							{
								total_time = 2;
							}


							for (int s_k = 0; s_k < total_time/*big_sheight/big_kerfwidth + (big_sheight%big_kerfwidth != 0 ? 1 : 0)*/; s_k++)
							{
								s_key = s_k;
								char schbegin_x[12],schbegin_y[12],schend_x[12],schend_y[12];

								sbegin_x = s_x - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2 : _d/2 + s_k*_d) + _slot.at(p_s)._srcx;
								sbegin_y = s_y + _slot.at(p_s)._srcy - s_width/2.0 /*+ _d/2.0*/;	// 2016-8-23 yuanzb ����ʱ���Ӿ�죬����ʱ��������
								send_x = s_x - s_height/2 + ((s_k + 1)*_d > s_height ? s_height - _d/2 : _d/2 + s_k*_d) + _slot.at(p_s)._srcx;
								send_y = s_y + s_width/2.0 + _slot.at(p_s)._srcy /*- _d/2.0*/;

								//sprintf_s(chbegin_x,"%0.4f",sbegin_y);
								//sprintf_s(chbegin_y,"%0.4f",sbegin_x);
								//sprintf_s(chend_x,"%0.4f",send_y);
								//sprintf_s(chend_y,"%0.4f",send_x);


								PrintNumber2Array(chbegin_x, 12, decimal_place, sbegin_y);
								PrintNumber2Array(chbegin_y, 12, decimal_place, sbegin_x);
								PrintNumber2Array(chend_x,	 12, decimal_place, send_y);
								PrintNumber2Array(chend_y,	 12, decimal_place, send_x);

								if (s_k%2 == 0)
								{
									if (s_k != 0)
									{
										float pre_x = s_x - s_height/2 + _d/2 + s_k*_d + _slot.at(p_s)._srcx - _d * 2.0;
										char pre_sy[12];
										//sprintf_s(pre_sy,"%0.4f",pre_x);
										PrintNumber2Array(pre_sy, 12, decimal_place, pre_x);

										if (coortype == 0)
										{
											wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 1)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
										else if (coortype == 2)
										{
											wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										}
									}

									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._spindlespeed > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
										wfile<<endl;
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}

									if (s_k == total_time/*big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0)*/)
									{
										//sprintf_s(chend_x,"%0.4f",send_y);
										//sprintf_s(chend_y,"%0.4f",send_x);

										PrintNumber2Array(chend_x, 12, decimal_place, send_y);
										PrintNumber2Array(chend_y, 12, decimal_place, send_x);
									}
								}
								else
								{
									if (s_k != 1)
									{
										float pre_x = s_x - s_height/2 + _d/2 + s_k*_d + _slot.at(p_s)._srcx - _d * 2.0;
										char pre_sy[12];
										//sprintf_s(pre_sy,"%0.4f",pre_x);
										PrintNumber2Array(pre_sy, 12, decimal_place, pre_x);

										if (s_k == 0)
										{
											wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight2;
											if (m_knifetype.at(v_i)._spindlespeed > 0.01)
												wfile<<" F "<<m_knifetype.at(v_i)._spindlespeed;
											wfile<<endl;
										}
										else
										{
											wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight2;
											if (m_knifetype.at(v_i)._feedrate > 0.01)
												wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
											wfile<<endl;
										}
									}

									if (coortype == 0)
									{
										wfile<<"G01"<<" X "<<chend_x<<" Y "<<chend_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
										wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<chbegin_y<<" Z "<<chheight2;
										if (m_knifetype.at(v_i)._feedrate > 0.01)
											wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
										wfile<<endl;
									}
									else if (coortype == 1)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}
									else if (coortype == 2)
									{
										wfile<<"G01"<<" X "<<chend_y<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
										wfile<<"G01"<<" X "<<chbegin_y<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									}

									if (s_k == big_sheight/big_kerfwidth - (big_sheight%big_kerfwidth == 0 ? 1 : 0))
									{
										//sprintf_s(chend_x,"%0.4f",sbegin_y);
										//sprintf_s(chend_y,"%0.4f",sbegin_x);
										PrintNumber2Array(chend_x, 12, decimal_place, sbegin_y);
										PrintNumber2Array(chend_y, 12, decimal_place, sbegin_x);
									}
								}

							}
							if (s_key%2 == 0 && s_key != 0)
							{
								float pre_x = s_x - s_height/2 + _d/2 + s_key*_d + _slot.at(p_s)._srcx - _d * 2.0;
								char pre_sy[12];
								//sprintf_s(pre_sy,"%0.4f",pre_x);
								PrintNumber2Array(pre_sy, 12, decimal_place, pre_x);

								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<chend_x<<" Y "<<pre_sy<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chend_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}
							if (s_key%2 == 1 && s_key != 1)
							{
								float pre_x = s_x - s_height/2 + _d/2 + s_key*_d + _slot.at(p_s)._srcx - _d * 2.0;
								char pre_sy[12];
							//	sprintf_s(pre_sy,"%0.4f",pre_x);
								PrintNumber2Array(pre_sy, 12, decimal_place , pre_x);

								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}

							}
							if (s_key == 1)
							{
								float pre_x = s_x - s_height/2 + ((0 + 1)*_d > s_height ? s_height - _d/2 : _d/2) + _slot.at(p_s)._srcx;
								char pre_sy[12];

								//sprintf_s(pre_sy,"%0.4f",pre_x);
								PrintNumber2Array(pre_sy, 12, decimal_place, pre_x);

								if (coortype == 0)
								{
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight2;
									if (m_knifetype.at(v_i)._feedrate > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._feedrate;
									wfile<<endl;
									wfile<<"G01"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight3;
									if (m_knifetype.at(v_i)._endspeed > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._endspeed;
									wfile<<endl;
									wfile<<"G00"<<" X "<<chbegin_x<<" Y "<<pre_sy<<" Z "<<chheight1;
									if (m_knifetype.at(v_i)._overspend > 0.01)
										wfile<<" F "<<m_knifetype.at(v_i)._overspend;
									wfile<<endl;
								}
								else if (coortype == 1)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
								else if (coortype == 2)
								{
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight2<<" F "<<m_knifetype.at(v_i)._feedrate<<endl;
									wfile<<"G01"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight3<<" F "<<m_knifetype.at(v_i)._endspeed<<endl;
									wfile<<"G00"<<" X "<<pre_sy<<" Y "<<chbegin_x<<" Z "<<chheight1<<" F "<<m_knifetype.at(v_i)._overspend<<endl;
								}
							}

						}
					}
				}
			}


			// �ж��Ƿ񻻵�

#if 0

			// 2017-5-6 yuanzb ÿ�ѵ��������п׻��߲ۣ�����Ҫ��������
			KnifeType cur_knife;
			bool _needchangeT = true;
			for (int _ktype = 0;_ktype <m_knifetype.size();_ktype++)
			{
				// ��Ϊ�ϣ�����ס��൶��

				// �����ǰʹ�ÿײ��ϡ�����ʱ�п��ܣ�����Ҳʹ��ͬһ�ѵ�������Ҫ�������뿪�ϵ�����¼
				cur_knife = m_knifetype.at(_ktype);

				if (	cur_knife.kt = KnifeType_Spindle_Hole_Slot_Material	
					&&  cur_knife.kt = KnifeType_Spindle_Slot_Material	
					&&  cur_knife.kt = KnifeType_Multi_knife_Hole	
					&&	m_knifetype.at(_ktype)._diameter == _k_d )
				{
					_needchangeT = false;

					// ��ͬ�ĵ����ۺ󲻻�����ֱ�ӿ���
					if (_vec.size() > 0 || _slot.size() > 0)
					{
						last_knife = cur_knife;
					}



					break;
				}
			}

#else

			// 2017-6-23 yuanzb ÿ�ѵ������󣬷��ǵ�ǰ���ĵ�����Ϊ�ײ��ϻ��߲��ϣ���¼Ϊ���һ�ѵ�,���濪�ϻ���ʹ��ͬһ�ѵ�
			//					ע�⣬���������У�ֻ��������Ψһһ�ѿ��Կ��ϵĵ�

			bool _needchangeT = true;
			KnifeType cur_knife = m_knifetype.at(v_i);


			// ��ͬ�ĵ����ۺ󲻻�����ֱ�ӿ���
			if (_vec.size() > 0 || _slot.size() > 0)
			{
				last_knife = cur_knife;

				if (	cur_knife.kt == KnifeType_Spindle_Hole_Slot_Material	
					||  cur_knife.kt == KnifeType_Spindle_Slot_Material	)
				{
					_needchangeT = false;
				}
			}


#endif
			

			













			if (_needchangeT == true)
			{
				if (_vec.size() > 0 || _slot.size() > 0)
				{
					for (int _g_i = 0;_g_i < _tgcodee.size();_g_i++)
					{
						if (_tgcodee.at(_g_i) == '@')
						{
							wfile<<endl;
						}
						else
						{
							wfile<<_tgcodee.at(_g_i);
						}
					}
					wfile<<endl;
				}
			}
		}

#endif



		// ׼������

#if 1
		bool _needchangeT = true;
		float knift_lenght = 0.0;
		int knife_num = 0;
		float knife_diameter = m_kerfwidth;						// ����Ӧ���õ��ǵ��İ뾶��֮ǰ�õ��Ǿ��Ŀ��
		float knife_spindlespeed = 8000;
		float oldknife_spindlespeed = knife_spindlespeed;
		float knife_feedrate = 8000;
		float knife_endspeed = 8000;
		float knife_overspend = 8000;
		float knife_facespeed = 8000;
		std::string _kgcodeb,_kgcodee;


		// Ѱ�ҵ�
		for (int ktype = 0;ktype <m_knifetype.size();ktype++)
		{
			KnifeType  cur_knife = m_knifetype.at(ktype);

			if (cur_knife.kt == 0 || cur_knife.kt == 4 || cur_knife.kt == 5)
			{
				// �ҵ����ʵĵ�

				knift_lenght = m_knifetype.at(ktype)._length;		// ȡ���ָ���޲�������
				//	knift_lenght = m_knifetype.at(ktype)._kinfelength;
				knife_diameter = m_knifetype.at(ktype)._diameter;	// ��ֱ��

				_kgcodeb = m_knifetype.at(ktype)._tgcodeb;
				_kgcodee = m_knifetype.at(ktype)._tgcodee;
				knife_spindlespeed = m_knifetype.at(ktype)._spindlespeed;
				knife_feedrate = m_knifetype.at(ktype)._feedrate;
				knife_endspeed = m_knifetype.at(ktype)._endspeed;
				knife_overspend = m_knifetype.at(ktype)._overspend;
				knife_facespeed = m_knifetype.at(ktype)._facespeed;
				oldknife_spindlespeed = knife_spindlespeed;
				_needchangeT = false;


				// ���ݰ����и�����ж��µ�����
				if (_thickness <= knift_lenght)
				{
					knife_num = 1;
				}
				else
				{
					knife_num = _thickness/knift_lenght + 1;
				}

				// ��֮ǰ�Ŀײ۵���ͬ ����д��ʼ��������
				if (last_knife.kt == cur_knife.kt &&
					last_knife._diameter == cur_knife._diameter &&
					last_knife._kn == cur_knife._kn )
				{

				}
				else
				{
					// ����ʼ����
					for (int _g_i = 0;_g_i < _kgcodeb.size();_g_i++)
					{
						if (_kgcodeb.at(_g_i) == '@')
						{
							wfile<<endl;
						}
						else
						{
							wfile<<_kgcodeb.at(_g_i);
						}
					}
				}

				wfile<<endl;

				break;
			}
		}

#endif

		// �������

		if (knife_num == 0)
		{
			CString info;
			info.Format("û�к��ʵĿ��ϵ���");

			vs_info = vs_info + info;

		}


		// ���������ı�
#if 0
	
		if (knife_num > 0)
		{

			char chh1[12],chh2[12],chh3[12];
			if (gcodeparam.downheight > 1000)
			{
				sprintf_s(chh2,"%0.4f",0.0);
				sprintf_s(chh3,"%0.4f",-1.0 * _thickness - 0.1);
			}
			else
			{
				sprintf_s(chh2,"%0.4f",_thickness - gcodeparam.downheight);
				sprintf_s(chh3,"%0.4f",-1.0 * m_gcodeparam.downheight);
			}

			sprintf_s(chh1,"%0.4f",gcodeparam.upheight);	



			vector<float> _df;
			float kdx1, kdx2, kdx3, kdx4, kdy1, kdy2, kdy3, kdy4;

			_df.push_back(3);
			_df.push_back(3);
			_df.push_back(3);
			_df.push_back(3);


			kdx1 = -1 * knife_diameter/2.0 + abs(_df.at(0));
			kdy1 = -1 * knife_diameter/2.0 + abs(_df.at(3));

			kdx2 = kdx1;
			kdy2 = m_panelheight + knife_diameter/2.0 - abs(_df.at(1));

			kdx3 = m_panelwidth + knife_diameter/2.0 - abs(_df.at(2));
			kdy3 = kdy2;

			kdx4 = kdx3;
			kdy4 = kdy1;

			if (1)
			{
				char ckdx1[12],ckdx2[12],ckdx3[12],ckdx4[12],ckdy1[12],ckdy2[12],ckdy3[12],ckdy4[12];

				sprintf_s(ckdx1,"%0.4f",kdx1);
				sprintf_s(ckdx2,"%0.4f",kdx2);
				sprintf_s(ckdx3,"%0.4f",kdx3);
				sprintf_s(ckdx4,"%0.4f",kdx4);

				sprintf_s(ckdy1,"%0.4f",kdy1);
				sprintf_s(ckdy2,"%0.4f",kdy2);
				sprintf_s(ckdy3,"%0.4f",kdy3);
				sprintf_s(ckdy4,"%0.4f",kdy4);

				float fix_1 =_df.at(0);
				float fix_2 =_df.at(1);
				float fix_3 =_df.at(2);
				float fix_4 =_df.at(3);

				enum
				{
					knife_up = 0,
					knife_down
				};

				int knife_pos = knife_up;

				// ��һ���ޱ�
				if (fix_1 != 0)
				{
					// ��Ҫ��


					if (knife_pos == knife_up)
					{

						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;

						// ��һ�����
						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh3<<endl;
					}


				}
				else
				{
					// ����Ҫ��
				}

				// �ڶ���
				if (fix_2 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh3<<endl;
				}
				else
				{
					// ����Ҫ��

					if (knife_pos == knife_down &&  fix_1 >  0)
					{
						wfile<<"G00"<<" X "<<kdy2<<" Y "<<kdx2<<" Z "<<chh1;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}

				}


				// ������
				if (fix_3 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh3<<endl;
				}
				else
				{
					// ����Ҫ��

					if (knife_pos == knife_down &&  fix_2 > 0)
					{
						wfile<<"G00"<<" X "<<kdy3<<" Y "<<kdx3<<" Z "<<chh1;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}
				}


				// ���ı�
				if (fix_4 != 0)
				{
					// δ�䵶�����䵶
					if (knife_pos == knife_up)
					{
						// ������
						knife_pos = knife_down;

						// ��һ���߿�ʼ
						wfile<<"G00"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh1;
						if (knife_overspend > 0.01)
							wfile<<" F "<<knife_overspend;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh2;
						if (knife_spindlespeed > 0.01)
							wfile<<" F "<<knife_spindlespeed;
						wfile<<endl;

						wfile<<"G01"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh3;
						if (knife_feedrate > 0.01)
							wfile<<" F "<<knife_feedrate;
						wfile<<endl;
					}

					// ֱ����һ��
					wfile<<"G01"<<" X "<<kdy1<<" Y "<<kdx1<<" Z "<<chh3<<endl;
				}
				else
				{
					// ����Ҫ��

					if (knife_pos == knife_down  && fix_3 > 0)
					{
						wfile<<"G00"<<" X "<<kdy4<<" Y "<<kdx4<<" Z "<<chh1;
						wfile<<endl;


						// ����������
						knife_pos = knife_up;
					}
				}
			}
		}

#endif

	
		// ��һ��
#if 1
		
		std::vector<CutPath> one_PreCutPath = m_PreCutPath;
		for (int _c_i = 0;_c_i < one_PreCutPath.size();_c_i++)
		{
			for (int k_l = 0;k_l < knife_num;k_l++)
			{
				char chheight1[12],chx_0[12],chx_1[12],chy_0[12],chy_1[12],chheight3[12];

				//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
				PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);

				if (gcodeparam.downheight > 1000)
				{
					float up_knife = -1.0 * k_l * knift_lenght;
					//sprintf_s(chheight3,"%0.4f",up_knife);
					PrintNumber2Array(chheight3, 12, decimal_place, up_knife);
				}
				else
				{
					float up_knife = _thickness - (k_l+1)*knift_lenght > 0 ? _thickness - (k_l+1)*knift_lenght : -0.1;
					//sprintf_s(chheight3,"%0.4f",up_knife);
					PrintNumber2Array(chheight3, 12, decimal_place, up_knife);

					if (gcodeparam.downheight > 0.5)
					{
						//sprintf_s(chheight3,"%0.4f",up_knife - gcodeparam.downheight);
						PrintNumber2Array(chheight3, 12, decimal_place, up_knife - gcodeparam.downheight);
					}
				}

				//sprintf_s(chx_0,"%0.4f",one_PreCutPath.at(_c_i)._y1);
				//sprintf_s(chx_1,"%0.4f",one_PreCutPath.at(_c_i)._y2);
				//sprintf_s(chy_0,"%0.4f",one_PreCutPath.at(_c_i)._x1);
				//sprintf_s(chy_1,"%0.4f",one_PreCutPath.at(_c_i)._x2);


				PrintNumber2Array(chx_0, 12, decimal_place, one_PreCutPath.at(_c_i)._y1);
				PrintNumber2Array(chx_1, 12, decimal_place, one_PreCutPath.at(_c_i)._y2);
				PrintNumber2Array(chy_0, 12, decimal_place, one_PreCutPath.at(_c_i)._x1);
				PrintNumber2Array(chy_1, 12, decimal_place, one_PreCutPath.at(_c_i)._x2);

				wfile<<"G00"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight1;
				if (knife_overspend > 0.01)
					wfile<<" F "<<knife_overspend;
				wfile<<endl;
				wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight3;
				if (knife_facespeed > 0.01)
					wfile<<" F "<<knife_facespeed;
				wfile<<endl;
				wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight3;
				if (knife_facespeed > 0.01)
					wfile<<" F "<<knife_facespeed;
				wfile<<endl;
				wfile<<"G00"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight1;
				if (knife_overspend > 0.01)
					wfile<<" F "<<knife_overspend;
				wfile<<endl;
			}
		}

#endif









#if 1
		// 2016-7-22 yuanzb Ӧ����Ҫ��ȥ���ָ���߲��������ã�����С���������Ĺ���
		// ԭ�еĹ�����һ����ּ����иÿ����һ�Σ�����һ�㣬������,�̶�ʹÿ����ֻ��һ��
		// ����ô��ѵ������/���� +1 �����������ߵķָ���ޣ�һ�����ּ�����
		for (int k_l = 0;k_l < knife_num;k_l++)			
		{
			for (int i = 0;i < _newcoor.size();i++)		// ����ô���������
			{
				// ����Ψһ����ID
				int one_coor_id_in_panel = _newcoor.at(i).m_NumInPanel;

				gcodeCoor CurCoor = _newcoor.at(i);
				int pos = _newcoor.at(i).pos;			// ������19
				bool no_cutpoint = false;

				std::string info = _newcoor.at(i)._info;

				if(info.at(0) == '!' && info.at(1) == '!')
				{
					knife_spindlespeed = m_leftsp;
				}
				else
				{
					knife_spindlespeed = oldknife_spindlespeed;
				}

				vector<PointInfo> points;

				// ��ȡ��ǰ�����������
				// 2017-4-7 yuanzb ����ԭ��ͨ���������������㣬���������ʱ������ȱ�ݣ��ְ�˳����������
				//points = m_allpoint.at(i).point_info;
				for (int p = 0;p < m_allpoint.size();p++)
				{
					// ����Ψһ����ID
					int one_outline_id_in_panel = m_allpoint.at(p).m_NumInPanel;

					//std::string panel_info = m_allpoint.at(p).panel_info;
					//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
					if (one_coor_id_in_panel == one_outline_id_in_panel)
					{
						points = m_allpoint.at(p).point_info;
						break;
					}
				}

				if (coortype != 0)
				{
					for (int q = 0;q < points.size();q++)
					{
						float _x = points.at(q).x;
						float _y = -1.0*points.at(q).y;
						points.at(q).x = _x;
						points.at(q).y = _y;
					}
				}

				//����û��points�İ���������������,�����һ��points
				if (points.size() == 0)
				{
					if (coortype == 0)
					{
						PointInfo p1,p2,p3,p4;
						p1.x = 0;p1.y = 0;p1.r = 0;p1.sign = 0;p1.dir = 0;p1.cut = 0;
						p2.x = 0;p2.y = _newcoor.at(i)._height;p2.r = 0;p2.sign = 0;p2.dir = 0;p2.cut = 0;
						p3.x = _newcoor.at(i)._width;p3.y = _newcoor.at(i)._height;p3.r = 0;p3.sign = 0;p3.dir = 0;p3.cut = 0;
						p4.x = _newcoor.at(i)._width;p4.y = 0;p4.r = 0;p4.sign = 0;p4.dir = 0;p4.cut = 0;

						points.push_back(p1);
						points.push_back(p2);
						points.push_back(p3);
						points.push_back(p4);
					}
					else
					{
						PointInfo p1,p2,p3,p4;
						p1.x = 0;p1.y = 0;p1.r = 0;p1.sign = 0;p1.dir = 0;p1.cut = 0;
						p2.x = 0;p2.y = -1*_newcoor.at(i)._height;p2.r = 0;p2.sign = 0;p2.dir = 0;p2.cut = 0;
						p3.x = _newcoor.at(i)._width;p3.y = -1* _newcoor.at(i)._height;p3.r = 0;p3.sign = 0;p3.dir = 0;p3.cut = 0;
						p4.x = _newcoor.at(i)._width;p4.y = 0;p4.r = 0;p4.sign = 0;p4.dir = 0;p4.cut = 0;

						points.push_back(p1);
						points.push_back(p2);
						points.push_back(p3);
						points.push_back(p4);
					}

				}


				// 2017-4-14 yuanzb �½ṹ������Ҫ���ݲ�����ת�����㣬�����������һ����׼ȷ�ģ���ʾ������ô��������ô�����ɣ���Ӧ�õ��������޸������������
// 				else if (points.size() > 0 && _newcoor.at(i)._dir == 2)
// 				{
// 					vector<PointInfo> otherpoints;
// 					for (int _p = 0;_p < points.size();_p++)
// 					{
// 						PointInfo one_p;
// 						one_p.cut = points.at(_p).cut;
// 						one_p.dir = points.at(_p).dir;
// 						one_p.r = points.at(_p).r;
// 						one_p.sign = points.at(_p).sign;
// 						one_p.x = _newcoor.at(i)._width - points.at(_p).y;
// 						one_p.y = points.at(_p).x;
// 						otherpoints.push_back(one_p);
// 					}
// 					points.clear();
// 					for (int _p = 0;_p < otherpoints.size();_p++)
// 					{
// 						points.push_back(otherpoints.at(_p));
// 					}
// 				}

				//TODO: ��ʼ�����Ӧ�����꣬��ѡ�����ʼ��
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;

				// �����εĵ�ֶ�
				vector<vector<PointInfo>> _pNewpoint;
				vector<PointInfo> _tmppoint;
				for (int n_p = 0;n_p < points.size();n_p++)
				{
					int _cut = points.at(n_p).cut;
					if (_cut == 1)
					{
						_tmppoint.push_back(points.at(n_p));
						_pNewpoint.push_back(_tmppoint);
						_tmppoint.clear();
					}
					else if (_cut == 2)
					{
						_tmppoint.clear();
						_tmppoint.push_back(points.at(n_p));
					}
					else
					{
						_tmppoint.push_back(points.at(n_p));
					}
				}

				// ����ô��ĵ�
				if (_tmppoint.size() > 0)
					_pNewpoint.push_back(_tmppoint);

				for (int _pn = 0;_pn < _pNewpoint.size();_pn++)
				{
					int nCount;
					vector<PointInfo> newpoint;
					vector<PointInfo> tmpnewpoint;

					// ��һ�ε�������
					tmpnewpoint = _pNewpoint.at(_pn);

					// �����ݿ��ܳ���
					if(tmpnewpoint.size() < 3)	// ������С��3
					{
						//AfxMessageBox("��⵽����" + info.c_str() + "�������ݳ����������ο��ļ���");
					}

					//Ŀǰ���õ��߰뾶����
					if (coortype == 0)
					{

#if 1	// �µĵ����������
						// ����ָ�Ϊƽ��ֱ������ϵ
						nCount = tmpnewpoint.size();
						for (int index = 0; index < nCount; index++)
						{
							tmpnewpoint[index].y *= -1;
						}

						// �����غϵ�
						tmpnewpoint = ALGORITHM_API::FiltrateCoincidentPoints2(tmpnewpoint);

						// ���ݵ���֯������Ҫ�����ݽṹ
						Cad_Complex* pComplex = new Cad_Complex;

						// ���»�ȡ��״�����
						nCount = tmpnewpoint.size();
						for (int index = 0; index < nCount; index++)
						{
							PointInfo cur_point = tmpnewpoint[index];

							if (index == 0)
							{
								if (cur_point.sign == 0)	// ֱ��ֱ�����
								{
									ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

									pPnt->m_FigureType = FigureType_PolyLine;
									pComplex->AddShapePoint(pPnt);
								}
								else if (cur_point.sign == 1 || cur_point.sign == 3)	// Բ������Բ�����ӵ�(�������ݴ�����ʱ�ȼ���) ��Բ���յ�һ���㣬���һ��Բ��
								{
									float center_x, center_y, r,sign ;
									float mid_x, mid_y, vec_x, vec_y;
									PointInfo end_pnt = tmpnewpoint[index+1];

									// ��Բ��
									r = end_pnt.r;
									sign = end_pnt.dir;
									ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x,cur_point.y, end_pnt.y, r, sign, center_x, center_y);

									// �����е��������
									mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
									mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

									vec_x = mid_x - center_x;
									vec_y = mid_y - center_y;

									ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
									vec_x *= r;
									vec_y *= r;

									// ��Բ���е��������
									mid_x = center_x + vec_x;
									mid_y = center_y + vec_y;

									// ���Բ��������
									ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
									ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
									ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

									pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;


									pComplex->AddShapePoint(pStart);
									pComplex->AddShapePoint(pMid);
									pComplex->AddShapePoint(pEnd);

									//index++;
								}
							}
							else
							{
								PointInfo prev_point = tmpnewpoint[index-1];

								if (cur_point.sign == 0)	// ֱ��ֱ�����
								{
									ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

									pPnt->m_FigureType = FigureType_PolyLine;
									pComplex->AddShapePoint(pPnt);
								}
								else if (cur_point.sign == 1 || cur_point.sign == 3)	// Բ����� ��Բ���յ�һ���㣬���һ��Բ��
								{
									float center_x, center_y, r,sign ;
									float mid_x, mid_y, vec_x, vec_y;
									PointInfo end_pnt = tmpnewpoint[index+1];

									// ��Բ��
									r = end_pnt.r;
									sign = end_pnt.dir;	// Բ������
									ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x, cur_point.y, end_pnt.y, r, sign, center_x, center_y);

									// �����е��������
									mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
									mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

									vec_x = mid_x - center_x;
									vec_y = mid_y - center_y;

									ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
									vec_x *= r;
									vec_y *= r;

									// ��Բ���е��������
									mid_x = center_x + vec_x;
									mid_y = center_y + vec_y;

									// ���Բ��������
									ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
									ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
									ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

									if (prev_point.sign == 0 || prev_point.sign == 2)	// ǰһ��Ϊֱ�߻���Բ���յ㣬˵��ǰһ����ֱ�ߣ���ǰ���ͼ��ͼ����Ϊֱ��
									{
										pStart->m_FigureType = FigureType_PolyLine;
										pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
									}
									else
									{
										pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
									}

									if (pStart->IsCloseTo(pComplex->GetTailShapePoint()) == FALSE)	// �������һ���غϣ�����ӵ�һ��
									{
										pComplex->AddShapePoint(pStart);
									}

									pComplex->AddShapePoint(pMid);
									pComplex->AddShapePoint(pEnd);
								}
							}

						}

						// �������ƽ�м���
						Figure* pTmpFig = pComplex->CalParallelFigure(1, knife_diameter/2.0);	// ��ƫ�������
						Cad_Complex* pParallel = static_cast<Cad_Complex*>(pTmpFig);			// ��̬ת��


						//Cad_Complex* pParallel = static_cast<Cad_Complex*>(pComplex->CalParallelFigure(1, knife_diameter/2.0));


						// ��ȡƽ�к�Ľṹ
						pParallel->CalShapePointOutlineInfo();
						int nShpPntNum = pParallel->GetShapePointNum();

						for (int index = 0; index < nShpPntNum; index++)
						{
							PointInfo new_point;
							ShapePoint* pPnt = pParallel->GetShapePoint(index);

							if (pPnt->m_sign != -1)
							{
								new_point.x = pPnt->m_x;
								new_point.y = pPnt->m_y * -1;
								new_point.r = pPnt->m_radius;
								new_point.dir = pPnt->m_dir;
								new_point.group = pPnt->m_group;
								new_point.side = pPnt->m_side;
								new_point.type = pPnt->m_type;
								new_point.sign = pPnt->m_sign;
								new_point.cut = pPnt->m_cut;

								newpoint.push_back(new_point);
							}
						}

#else	// ԭ���ĵ����������

						newpoint = outdirpoint(tmpnewpoint,knife_diameter/2.0,pos,_newcoor.at(i)._info);

#endif
					}
					else
					{
						//newpoint = outotherdirpoint(tmpnewpoint,_kerfwidth/2.0);
						vector<PointInfo> othertmpnewpoint;
						vector<PointInfo> othernewpoint;
						for (int other_i = tmpnewpoint.size();other_i > 0;other_i--)
						{
							PointInfo one_other;
							one_other.cut = tmpnewpoint.at(other_i - 1).cut;
							one_other.dir = tmpnewpoint.at(other_i - 1).dir == 0 ? 0 : (3 - tmpnewpoint.at(other_i - 1).dir);
							one_other.r = tmpnewpoint.at(other_i - 1).r;
							one_other.sign = (tmpnewpoint.at(other_i - 1).sign == 0 || tmpnewpoint.at(other_i - 1).sign == 3) ? tmpnewpoint.at(other_i - 1).sign : (3 - tmpnewpoint.at(other_i - 1).sign);
							one_other.x = tmpnewpoint.at(other_i - 1).x;
							one_other.y = tmpnewpoint.at(other_i - 1).y;
							othertmpnewpoint.push_back(one_other);
						}

						othernewpoint = outdirpoint(othertmpnewpoint,knife_diameter/2.0,pos,_newcoor.at(i)._info);

						for (int other_i = othernewpoint.size();other_i > 0;other_i--)
						{
							PointInfo one_other;
							one_other.cut = othernewpoint.at(other_i - 1).cut;
							one_other.dir = othernewpoint.at(other_i - 1).dir == 0 ? 0 : (3 - othernewpoint.at(other_i - 1).dir);
							one_other.r = othernewpoint.at(other_i - 1).r;
							one_other.sign = (othernewpoint.at(other_i - 1).sign == 0 || othernewpoint.at(other_i - 1).sign == 3) ? othernewpoint.at(other_i - 1).sign : (3 - othernewpoint.at(other_i - 1).sign);
							one_other.x = othernewpoint.at(other_i - 1).x;
							one_other.y = othernewpoint.at(other_i - 1).y;
							newpoint.push_back(one_other);
						}
					}

					// ����Ҫ���䵶����ж��⴦��

// 					if (_pn > 0)
// 					{
// 						bool havesline = false;
// 						for (int t_i = 0;t_i < tmpnewpoint.size();t_i++)
// 						{
// 							if (tmpnewpoint.at(t_i).sign == 0)
// 							{
// 								havesline = true;
// 								break;
// 							}
// 						}
// 						if (havesline == false)
// 						{
// 							pos = -1;
// 						}
// 					}
//
// 					if (pos == -1)
// 					{
// 						pos = 0;
// 						no_cutpoint = true;
// 					}
// 					else if(_pNewpoint.at(_pn).size()-1 < pos)
// 					{
// 						pos = pos - _pNewpoint.at(_pn).size();
// 						no_cutpoint = true;
// 					}
// 					else
// 					{
// 						no_cutpoint = false;
// 					}


					// �����ﴦ��ԭʼ�䵽���뵶������䵶�㣬�ҵ�������ӽ���pos

#if 1

					int old_pnt_size = tmpnewpoint.size();	// ԭʼ��
					int new_pnt_size = newpoint.size();	// ������
					int new_pos = pos;						// �䵶��


					vector<PointInfo> OldPointList;

					for (int i = 0; i < old_pnt_size; i++)
					{
						PointInfo tmp_pnt = tmpnewpoint.at(i);
						
						tmp_pnt.y *= -1;

						OldPointList.push_back(tmp_pnt);


					}


					if (pos >= old_pnt_size)
					{

					}
					else
					{
						int close_pos = new_pos;
						float close_dist;
						PointInfo old_pnt = OldPointList[pos%old_pnt_size];
						FPoint old_pos_pnt(old_pnt.x, old_pnt.y);

						for (int i_pos = 0; i_pos < new_pnt_size; i_pos++)
						{
							PointInfo new_pnt = newpoint[i_pos];
							FPoint new_pos_pnt(new_pnt.x, new_pnt.y);

							float cur_dist = old_pos_pnt.DistanceTo(new_pos_pnt);

							if (i_pos == 0)
							{
								close_dist = cur_dist;
								close_pos = i_pos;
							}
							else
							{
								if (cur_dist < close_dist)
								{
									close_dist = cur_dist;
									close_pos = i_pos;
								}
							}


						}

						// ȡ����ӽ���pos
						new_pos = close_pos;
					}

					// ��ȡ�����䵽��
					pos = new_pos;


#endif



					// �ж������������С�壬С��Ҫ�����������ķ�ʽ
					if (m_bSmallPanelCutTwiceEnable == true 
						&& (CurCoor._width < m_SmallPanelStandardSize || CurCoor._height < m_SmallPanelStandardSize)
						&& knife_num  == 1)
					{
						OutputGCode_KnifeOneSmallPanel(newpoint, pos, gcodeparam ,points, wfile, k_l, src_x, src_y, info, knift_lenght, coortype, no_cutpoint, KnifeOffset,
							knife_spindlespeed , oldknife_spindlespeed, knife_feedrate, knife_endspeed, knife_overspend , knife_facespeed, decimal_place);
					}
					else
					{
						OutputGCode_KnifeOnePanel(newpoint, pos, gcodeparam ,points, wfile, k_l, src_x, src_y, info, knift_lenght, coortype, no_cutpoint, KnifeOffset,
							knife_spindlespeed , oldknife_spindlespeed, knife_feedrate, knife_endspeed, knife_overspend , knife_facespeed, decimal_place);
					}
				}
			}
		}
#endif

		// ��һ��
#if 1

		std::vector<CutPath> one_cutpath = m_cutpath;
		for (int _c_i = 0;_c_i < one_cutpath.size();_c_i++)
		{
			for (int k_l = 0;k_l < knife_num;k_l++)
			{
				char chheight1[12],chx_0[12],chx_1[12],chy_0[12],chy_1[12],chheight3[12];

				//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
				PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);

				if (gcodeparam.downheight > 1000)
				{
					float up_knife = -1.0 * k_l * knift_lenght;
					//sprintf_s(chheight3,"%0.4f",up_knife);
					PrintNumber2Array(chheight3, 12, decimal_place, up_knife);
				}
				else
				{
					float up_knife = _thickness - (k_l+1)*knift_lenght > 0 ? _thickness - (k_l+1)*knift_lenght : -0.1;
					//sprintf_s(chheight3,"%0.4f",up_knife);
					PrintNumber2Array(chheight3, 12, decimal_place,up_knife);

					if (gcodeparam.downheight > 0.5)
					{
						//sprintf_s(chheight3,"%0.4f",up_knife - gcodeparam.downheight);
						PrintNumber2Array(chheight3, 12, decimal_place, up_knife - gcodeparam.downheight);
					}
				}
				//sprintf_s(chx_0,"%0.4f",one_cutpath.at(_c_i)._y1);
				//sprintf_s(chx_1,"%0.4f",one_cutpath.at(_c_i)._y2);
				//sprintf_s(chy_0,"%0.4f",one_cutpath.at(_c_i)._x1);
				//sprintf_s(chy_1,"%0.4f",one_cutpath.at(_c_i)._x2);


				PrintNumber2Array(chx_0, 12, decimal_place, one_cutpath.at(_c_i)._y1);
				PrintNumber2Array(chx_1, 12, decimal_place, one_cutpath.at(_c_i)._y2);
				PrintNumber2Array(chy_0, 12, decimal_place, one_cutpath.at(_c_i)._x1);
				PrintNumber2Array(chy_1, 12, decimal_place, one_cutpath.at(_c_i)._x2);

				wfile<<"G00"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight1;
				if (knife_overspend > 0.01)
					wfile<<" F "<<knife_overspend;
				wfile<<endl;
				wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight3;
				if (knife_facespeed > 0.01)
					wfile<<" F "<<knife_facespeed;
				wfile<<endl;
				wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight3;
				if (knife_facespeed > 0.01)
					wfile<<" F "<<knife_facespeed;
				wfile<<endl;
				wfile<<"G00"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight1;
				if (knife_overspend > 0.01)
					wfile<<" F "<<knife_overspend;
				wfile<<endl;
			}
		}

#endif

		/**/
		// ���߽�������

#if 1
		for (int _g_i = 0;_g_i < _kgcodee.size();_g_i++)
		{
			if (_kgcodee.at(_g_i) == '@')
			{
				wfile<<endl;
			}
			else
			{
				wfile<<_kgcodee.at(_g_i);
			}
		}
		wfile<<endl;

		CString s_tmpend;
		CString str_filechange = "";
		s_tmpend.Format("%s",gcodeparam.ender.c_str());

		CString other_filechange = "";
		if (file_change.size() > 0)
		{
			other_filechange.Format("%s",file_change.c_str());
		}

		if (file_change.size() > 0)
		{
			str_filechange.Format("@1800:=\"%s\"",file_change.c_str());
		}
		else
		{
			str_filechange = "@1800:=0";
		}
		//s_tmpend.Replace(_T("*"),str_filechange);
		std::string tmpender = s_tmpend.GetBuffer();
		for (int end_i = 0;end_i < tmpender.size();end_i++)
		{
			if (tmpender.at(end_i) == '@')
			{
				wfile<<endl;
			}
			else if (tmpender.at(end_i) == '*')
			{
				wfile<<str_filechange<<endl;
			}
			else if (tmpender.at(end_i) == '#')
			{
				if (cutprint == 1)	// �ǻ��ļ�β��Ҫ�õ�#�������滻
				{
					wfile<<tmpender.at(end_i);
				}
				else
				{
					wfile<<other_filechange<<endl;
				}
			}
			else if (tmpender.at(end_i) == '!' && cutprint == 1)	// �ǻ��ļ�β��Ҫ�õ�@������ͷ����ԭ���ã��滻Ϊ@
			{
				wfile<<"@";
			}
			else
			{
				wfile<<tmpender.at(end_i);
			}
		}
		wfile<<_mainktgcodee.c_str()<<endl;
		//wfile<<gcodeparam.ender.c_str()<<endl;

#endif

		// ���⴦��
#if 1

		// �ص��´�ϵͳ���һ�������ļ����һ��Ҫ��m29
		if( cutprint == 9)
		{
			//int panel_num = m_OpKnifeCoor.size();

			// 			if(PanelNum == panel_num)
			// 			{
			// 				wfile << "M29" << endl;
			// 			}

			if(NextCutFileName.size() == 0)	// ���һ�������ļ�
			{
				wfile << "M29" << endl;
			}
		}
		else if( cutprint == 12)
		{
			// ��ï-�´�ϵͳ����Ҫ��ÿ�������ļ���ָ����һ�������ļ����������һ�������ļ���������M��


			if(NextCutFileName.size() == 0)	// ���һ�������ļ�
			{
				wfile << "M81" << endl;
			}
			else	// �������һ��
			{
				CString str_file;
				str_file.Format("%s", NextCutFileName.c_str());
				str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

				wfile << "G500 F\"" << str_file << "\"" << endl;

			}

			wfile << "M999" << endl;
			wfile << "M99" << endl;
		}
		else if (cutprint == 13)	// ��ŵK1  
		{
			if(NextCutFileName.size() != 0)	// �������һ�������ļ�
			{

				wfile << "M801 MSG\"{|K|" << NextCutFileName << "}\"" << endl;
				wfile << "M30" << endl;
			}
		}
		else if (cutprint == 3)	// ����-�´� ��Ҫ����
		{

			wfile << "M5"			<< endl;
			wfile << "G53Z0"		<< endl;
			wfile << "G49"			<< endl;
			wfile << "G90G0G55X0."	<< endl;
			wfile << "M23"			<< endl;
			wfile << "M21"			<< endl;
			wfile << "M105"		<< endl;
			wfile << "M101"		<< endl;
			wfile << "M24"			<< endl;
			wfile << "M300"		<< endl;
			wfile << "WAIT()"		<< endl;

			if(NextCutFileName.size() != 0)	// �������һ�������ļ�
			{
				wfile << "@1800:=" << "\"" << NextCutFileName << "\"" << endl;
			}
			else
			{

				wfile << "@1800:=" << "\"" << ".nc" << "\"" << endl;
			}



			wfile << "M19"			<< endl;
			wfile << "M99"			<< endl;
		}
		else if (cutprint == 1)		// �ǻ��ļ�β��Ҫ�����Զ����ϴ���
		{
			wfile << "M05"		<< endl;
			wfile << "M52"		<< endl;
			wfile << "G79 Z0"	<< endl;
			wfile << "M405"		<< endl;
			wfile << "M30"		<< endl;
			wfile << "%"		<< endl;	
		}
		else if (cutprint == 17)	// �ȴ�
		{
			wfile << "@714"					<< endl;
			wfile << "R10=R10+1"			<< endl;
			wfile << "R4=R4+1"				<< endl;
			wfile << "R5=R5+1"				<< endl;
			wfile << "T0 M06 D0"			<< endl;
			wfile << "G53"					<< endl;
			wfile << "G0 Z=R202"			<< endl;
			wfile << "G0 X=R203 Y=R201"		<< endl;
			wfile << "M05"					<< endl;
			wfile << "M59"					<< endl;
			wfile << "@716 (Refresh)"		<< endl;
			wfile << "T00 D0"				<< endl;
			wfile << "M05"					<< endl;
			wfile << "G53"					<< endl;
			wfile << "G0 Z=R202"			<< endl;
			wfile << "G0 X=R200 Y=R201"		<< endl;
			wfile << "M70 (Vacuum All Off)"	<< endl;
			wfile << "M30"					<< endl;
		}
		else if (cutprint == 18)	// ��ŵK1-������  
		{
			if(NextCutFileName.size() != 0)	// �������һ�������ļ�
			{

				wfile << "M801 MSG\"{|K|" << NextCutFileName << "}\"" << endl;
				wfile << "M30" << endl;
				wfile << "M30" << endl;
			}
		}

#endif


	} // endof if(wfile)
	else
	{
		CString info = CurCutFileName.c_str();
		info += "����ʧ�ܣ�";

		AfxMessageBox( info);
		return false;
	}
	wfile.close();

	// ֻ��ʾһ��
	if (m_bReportMsg == true)
	{
		if (vs_info.GetLength() > 0)
		{
			AfxMessageBox(vs_info);

			// ��ʾ�꣬�ر�
			UnaleMsgReport();		
		}
	}




	return true;
}







// ����·��
vector<gcodeCoor> KnifeClass::pathoptimize(vector<Component*>& CpnList)
{
	vector<gcodeCoor> gcoor;
	vector<gcodeCoor> newgcoor;

	// �������������
// 	vector<Component*> CpnList;
// 	pPanel->GetAllNeedComponent(CpnList);
// 
// 	sort(CpnList.begin(), CpnList.end(), ComponentCompareID);

	int nCpnCount = CpnList.size();

	for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
	{
		Component* pCpn = CpnList.at(i_cpn);

		gcodeCoor gcr;
		gcr._height = pCpn->m_RealWidth;
		gcr._width = pCpn->m_RealLength;
		gcr._info = pCpn->m_BarCode;
		gcr._x = pCpn->m_x;
		gcr._y = pCpn->m_y;
		gcr._dir = pCpn->m_Texture;
		gcr.pos = 0;

		// ����Ψһ��ʶȡ�������ж�
		gcr.m_NumInPanel = pCpn->m_NumberInPanel;

		gcoor.push_back(gcr);
	}



	// 	for (int i = 0;i < coor.size();i++)
	// 	{
	// 		for (int j = 0;j < coor.at(i)._onecoms._coms.size();j++)
	// 		{
	// 			gcodeCoor gcr;
	// 			gcr._height = com_height;
	// 			gcr._width = com_width;
	// 			gcr._info = com_info;
	// 			gcr._x = com_x;
	// 			gcr._y = com_y;
	// 			gcr._dir = com_dir;
	// 			gcr.pos = 0;
	// 			gcoor.push_back(gcr);
	// 		}
	// 	}


	return gcoor;

}



//��һ��Ҫ�ָ���ַ���

//�ڶ���Ҫ��ŵ�����

//�������ָ��ʵ���������������±� ���û��Ԫ������ �������ú��       Ϊ-1.ע��c�ں������ú�ֵ�����仯������һ����ַ���ò��� 

//���ĸ�����Ϊ�ָ��������Ȳ���
void Split(CString a,CString *b,int &c,CString d)       
{
	int d_len = d.GetLength();
	int j=0,n=0,m_pos;

	while(1)
	{
		m_pos = a.Find(d,j);
		if(m_pos == -1 && j == 0)
		{
			c = -1;

			b[0] = a;

			break;

		}

		if(m_pos == -1 && j!=0)

		{

			b[n] = a.Mid(j,a.GetLength()-j);

			c = n;

			break;

		}

		if(j == 0)

		{

			b[n] = a.Mid(0,m_pos);

			j = m_pos + d_len;

		}

		else

		{

			b[n] = a.Mid(j,m_pos-j);

			j = m_pos + d_len;

		}

		n++;
	}
}









/*---------------------------------------*/
//	����˵����
//		��������ļ�
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
void KnifeClass::OutputLabel(	Panel* pPanel,					string CurCutFileName,			string CurPrintFileName,
								int cutprint,					int PanelNum,					string NextPrintFileName,	
								string NextCutFileName,			vector<P_Info> _pinfo,			ofstream& wfile,
								vector<gcodeCoor>& _newcoor,	string node_dvcut,				bool LabelInReverseSide)
{
	float _thickness		= pPanel->m_Thickness;
	CString m_material		= pPanel->m_Material;
	float m_panelwidth		= pPanel->m_OrgLen;
	float m_panelheight		= pPanel->m_OrgWidth;




#if 1
	if (cutprint == 1)			// �ǻ�
	{
		// 2016-9-12 yuanzb ���Ӵ����Ϣ�����ʡ���ȡ��ߴ�
	
		wfile<<";��ģ�" << _thickness << "mm" << m_material << " �ߴ磺" << m_panelwidth << "*" <<  m_panelheight << "*" << _thickness <<endl;
		wfile<<";SELECT PROCESS"<<endl;
		wfile<<"(GTO,PRO1,!PROC(0)=1) ;JUMP TO MAIN PROGRAM"<<endl;
		wfile<<"(GTO,PRO2,!PROC(0)=2) ;JUMP TO LABEL PASTING"<<endl;
		wfile<<endl;
		wfile<<"\"PRO2\""<<endl;

#if 1

		// 2016-9-12 yuanzb ������ȡ������Ҫ������
		wfile<<"#@PSS=0"<<endl;
		wfile<<"#@PEE=0"<<endl;

		wfile<<"G90 G10"<<endl;
		wfile<<"M700"<<endl;
		wfile<<"G90 G10"<<endl;
		wfile<<"M703"<<endl;
		wfile<<";(GTO,END)"<<endl;
		wfile<<";\"BG\"";
		wfile<<endl;
		wfile<<"(UAO,1)"<<endl;
		wfile<<"G90 G0"<<endl;
		wfile<<endl;


		for (int i = 0;i < _newcoor.size();i++)
		{
			std::string info = _newcoor.at(i)._info;
			float src_x = _newcoor.at(i)._x;
			float src_y = _newcoor.at(i)._y;
			float _width = _newcoor.at(i)._width;
			float _height = _newcoor.at(i)._height;

			wfile<<"\"ST";
			if (i+1 < 10)
			{
				wfile<<"0";
			}
			wfile<<(i+1)<<"\""<<endl;

			CString str_PanelNum;
			str_PanelNum.Format("%d-%d",PanelNum,i+1);
			int str_len = 8 - str_PanelNum.GetLength();
			for (int p = 0;p < str_len;p++)
			{
				str_PanelNum = "0" + str_PanelNum;
			}

			wfile<<"(DIS,\""<<str_PanelNum<<"\")"<<endl;
			wfile<<"LS11=\'"<<str_PanelNum<<"\'"<<endl;
			wfile<<"M701"<<endl;
			wfile<<"(GTO,ST02,E41=0)"<<endl;


			char ch_x[10],ch_y[10];
			float _px;
			float _py;


			// ������λ��
#if (NEW_PROGRAM == 1)

			// ���ұ�ǩλ��
			map<int,PointFloat>::iterator it_label;

			it_label = m_LabelPosition.find(i);

			if(it_label == m_LabelPosition.end())	// δ�ҵ�
			{
				_px = src_x + _width/2;
				_py = src_y + _height/2;
			}
			else	// �ҵ� 
			{
				PointFloat label_pos = it_label->second;

				_px = src_x + label_pos.x;
				_py = src_y + label_pos.y;

			}
#else

			_px = src_x + _width/2;
			_py = src_y + _height/2;

#endif


			// �������� ˫��2
			if (LabelInReverseSide == true)
			{
				if (node_dvcut == "1")
				{
					_px = m_panelwidth - _px;
				}
				else if (node_dvcut == "2")		
				{
					_py = m_panelheight - _py;
				}
			}

			sprintf_s(ch_x,"%0.4f",_px);
			sprintf_s(ch_y,"%0.4f",_py);


			wfile<<"G90 G0 V"<<_py<<" U"<<_px<<endl;
			wfile<<"M702"<<endl;
			wfile<<"(GTO,ST";
			if (i+1 < 10)
			{
				wfile<<"0";
			}
			wfile<<(i+1);
			wfile<<",E42=0)"<<endl;
			wfile<<"(DIS,\"\")"<<endl;
			wfile<<endl;
		}

		wfile<<";(GTO,BG)"<<endl;
		wfile<<"\"END\""<<endl;
		wfile<<"G90 G00"<<endl;
		wfile<<"M704"<<endl;

		// 2016-9-12 yuanzb ������ȡ������Ҫ������
		wfile<<"#@PSS=1"<<endl;
		wfile<<"#@PEE=1"<<endl;


		wfile<<"M02"<<endl;
		wfile<<"M30"<<endl;

		// 2017-5-15 yuanzb ԭ���ǻԶ�ȡknife.ini�ļ��е�header��ender,���ڲ���ȡ�������ֶΣ���Ϊ���������
		wfile <<	"\"PRO1\""	<< endl;
		wfile <<	"#@PS=1"	<< endl;
		wfile <<	"#@PE=1"	<< endl;
		wfile <<	"G90"		<< endl;
		wfile <<	"G40"		<< endl;
		wfile <<	"G80"		<< endl;
		wfile <<	"M52"		<< endl;
		wfile <<	"(UAO,1)"	<< endl;
		wfile <<	"G80"		<< endl;
				

		wfile<<endl;

#endif

	}
	else if (cutprint == 2)		// ��ŵ-ά��ϵͳ
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			wpfile<<"G54G90"<<endl;
			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;


				wpfile<<"G801 M\"";
				std::string get_info = _newcoor.at(i)._info;
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(get_info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile<<_pinfo.at(q)._content.c_str();
						break;
					}
				}
				wpfile<<"\""<<endl;

				float _px;
				float _py;

				// ������λ��
#if (NEW_PROGRAM == 1)
				// ���ұ�ǩλ��
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else

				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 A"<<str_py<<" B"<<str_px<<" F15000.000"<<endl;
				wpfile<<"M200"<<endl;
			}
			wpfile<<"G53 X0.000 Y0.000"<<endl;
			wpfile<<"M101"<<endl;
			wpfile<<"G801 P\""<<NextPrintFileName.c_str()<<"\""<<endl;
			if (NextCutFileName.size() > 0)
			{
				wpfile<<"G801 Q\""<<NextCutFileName.c_str()<<"\""<<endl;
			}

			wpfile<<"G4X2"<<endl;
			wpfile<<"M400"<<endl;
			wpfile<<"M99"<<endl;
		}


#endif


	}
	else if (cutprint == 3)		 // ����-�´�
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			wpfile<<"%@MACRO;"<<endl;
			for (int i = 0;i < _newcoor.size();i++)
			{
				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}


				wpfile<<"G500;"<<endl;
				wpfile<<"G700A"<<"\""<<str_PanelNum<<".bmp"<<"\";"<<endl;
				wpfile<<"G502;"<<endl;

				float _px;
				float _py;
				CString p_x, p_y;




#if (NEW_PROGRAM == 1)

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);

				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = _newcoor.at(i)._x;
					_py = -1.0*_newcoor.at(i)._y;	// �����ĸ߶�

					_px = _px + ((_newcoor.at(i)._width/2.0) > 0 ? (_newcoor.at(i)._width/2.0) : 0);
					_py = _py - ((_newcoor.at(i)._height/2.0) > 0 ? (_newcoor.at(i)._height/2.0) : 0);

					_px = _px > 2400.0 ? 2399 : _px;
					_py = _py < -1190.0 ? -1189 : _py;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = _newcoor.at(i)._x + label_pos.x;
					_py = -1 *(_newcoor.at(i)._y + label_pos.y);

				}
#else
				_px = _newcoor.at(i)._x;
				_py = -1.0*_newcoor.at(i)._y;	// �����ĸ߶�

				_px = _px + ((_newcoor.at(i)._width/2.0) > 0 ? (_newcoor.at(i)._width/2.0) : 0);
				_py = _py - ((_newcoor.at(i)._height/2.0) > 0 ? (_newcoor.at(i)._height/2.0) : 0);

				_px = _px > 2400.0 ? 2399 : _px;
				_py = _py < -1190.0 ? -1189 : _py;

#endif


				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}


				p_x.Format("%0.4f",_px);
				p_y.Format("%0.4f",_py);
				wpfile<<"G01 "<<"X "<<p_x<<" Y "<<p_y<<" F 45000;"<<endl;
				wpfile<<"G501;"<<endl;
			}
			wpfile<<"WAIT();"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"@1600:=\""<<NextPrintFileName.c_str()<<"\";"<<endl;
			}
			else
			{
				wpfile<<"@1600:="<<"0"<<";"<<endl;
			}
			wpfile<<"M99;"<<endl;
		}

#endif

	}
	else if (cutprint == 4)		// ����
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			char s_panelheight[12],s_panelwidth[12],s_thickness[12];
			sprintf_s(s_panelheight,"%0.4f",m_panelheight);
			sprintf_s(s_panelwidth,"%0.4f",m_panelwidth);
			sprintf_s(s_thickness,"%0.4f",_thickness);
			wpfile<<"N0000,"<<PanelNum<<","<<s_panelheight<<","<<s_panelwidth<<","<<s_thickness<<",1,"<<m_material<<endl;
			wpfile<<"N0000,LBX,LBY,X,Y,PATX,PATY,����,��ǩ"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				CString p_x,p_y;

				float _px;
				float _py;

				// ������λ��
#if (NEW_PROGRAM == 1)
				

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else

				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				char s_num[12];
				char s_x[12], s_y[12], s_p_width[12], s_p_height[12];
				sprintf_s(s_x,"%0.4f",_px);
				sprintf_s(s_y,"%0.4f",_py);
				sprintf_s(s_p_width,"%0.4f",_width);
				sprintf_s(s_p_height,"%0.4f",_height);

				sprintf_s(s_num,"N%04d",i+1);

				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}

				wpfile<<s_num<<","<<_py<<","<<_px<<","<<s_y<<","<<s_x<<","<<_height<<","<<_width<<","<<m_material<<","<<str_PanelNum<<".bmp"<<endl;

			}	
			wpfile<<"N9999"<<endl;
		}

#endif

	}
	else if (cutprint == 5)		// ����-�´�
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			wpfile<<"G54G90"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				wpfile<<"G801 U\"";
				std::string get_info = _newcoor.at(i)._info;
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(get_info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile<<_pinfo.at(q)._content.c_str();
						break;
					}
				}
				wpfile<<"\""<<endl;


				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				float _px;
				float _py;

				// ������λ��
#if (NEW_PROGRAM == 1)
				

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);

				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else

				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 A"<<str_py<<" B"<<str_px<<" F12000.000"<<endl;
				wpfile<<"M200"<<endl;
			}
			wpfile<<"M101"<<endl;
			std::string cut_str = "";
			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);
			wpfile<<"G801 P\""<<str_file<<"\""<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"G801 Q\""<<NextPrintFileName.c_str()<<"\""<<endl;
			}

			wpfile<<"M400"<<endl;
			if (NextPrintFileName.size() == 0)
			{
				wpfile<<"M29"<<endl;
			}
			wpfile<<"M99"<<endl;
		}


#endif

	}
	else if (cutprint == 6)	// ����-ά��
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			wpfile<<"M300"<<endl;
			wpfile<<"G0 Z15.000"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{

				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				wpfile<<"M801 MSG\"{|Y|";
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile<<_pinfo.at(q)._content.c_str();
						break;
					}
				}
				wpfile<<"}\""<<endl;



				float _px;
				float _py;

				// ������λ��
#if (NEW_PROGRAM == 1)

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else

				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif


				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 X"<<str_py<<" Y"<<str_px<<" Z-1.0000 F24"<<endl;
				wpfile<<"M200"<<endl;
			}
			wpfile<<"G53 X0.000 Y0.000"<<endl;
			wpfile<<"M100"<<endl;
			if (NextCutFileName.size() > 0)
			{
				std::string tmp_last_str;
				for (int i = CurCutFileName.size()-1;i >= 0;i--)
				{
					if(CurCutFileName.at(i) == '\\')
						break;
					tmp_last_str.push_back(CurCutFileName.at(i));
				}
				std::string last_str;
				for (int i = tmp_last_str.size() - 1;i >= 0;i--)
				{
					last_str.push_back(tmp_last_str.at(i));
				}
				wpfile<<"M801 MSG\"{|K|"<<last_str.c_str()<<"}\"M400"<<endl;
				wpfile<<"M801 MSG\"{|T|"<<NextPrintFileName.c_str()<<"}\""<<endl;
			}
			else
			{
				std::string tmp_last_str;
				for (int i = CurCutFileName.size()-1;i >= 0;i--)
				{
					if(CurCutFileName.at(i) == '\\')
						break;
					tmp_last_str.push_back(CurCutFileName.at(i));
				}
				std::string last_str;
				for (int i = tmp_last_str.size() - 1;i >= 0;i--)
				{
					last_str.push_back(tmp_last_str.at(i));
				}
				wpfile<<"M801 MSG\"{|K|"<<last_str.c_str()<<"}\"M400"<<endl;
			}

			wpfile<<"M30"<<endl;
		}


#endif

	}
	else if (cutprint == 7)	// ��ŵ-�´�
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			// ���� Z��̧�ߵ�15
			wpfile<<"M300"<<endl;
			wpfile<<"G0 Z15.000"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;


				wpfile<<"M801 MSG\"{|Y|";
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile<<_pinfo.at(q)._content.c_str();
						break;
					}
				}
				wpfile<<"}\""<<endl;

				float _px, _py;

#if (NEW_PROGRAM == 1)
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 X"<<str_py<<" Y"<<str_px<<" Z-1.000 F24"<<endl;
				wpfile<<"M200"<<endl;
			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			wpfile<<"G53 X0.000 Y0.000"<<endl;
			wpfile<<"M100"<<endl;
			wpfile<<"M801 MSG\"{|K|"<<str_file<<"}\"M400"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"M801 MSG\"{|T|"<<NextPrintFileName.c_str()<<"}\""<<endl;
			}

			wpfile<<"M30"<<endl;
		}
		wpfile.close();


#endif

	}
	else if (cutprint == 8)	// �ص�-ά��
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		CString strCP, strNP, strCC, strNC;

		strCP = CurPrintFileName.c_str();
		strNP = NextPrintFileName.c_str();
		strCC = CurCutFileName.c_str();
		strNC = NextCutFileName.c_str();

		if (wpfile)
		{
			// ���� Z��̧�ߵ�15

			if(PanelNum != 1)			// 2016-7-5 yuanzb	ά���һ�������ļ�������ҪM300����������Ҫ
				wpfile<<"M300"<<endl;

			// 2016-11-14 yuanzb	ά�������ļ�����ҪM100
			wpfile<<"M100"<<endl;	

			wpfile<<"G0 Z15.000"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{

				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				wpfile<<"M801 MSG\"{|Y|";

				// bmp�ļ���
				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}

				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile << str_PanelNum << ".bmp";
						break;
					}
				}

				wpfile<<"}\""<<endl;

				// ��������λ��
				float _px, _py;

#if (NEW_PROGRAM == 1)

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);

				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G1 X"<<str_py<<" Y"<<str_px<<" Z-1.000 F24"<<endl;
				wpfile<<"M200"<<endl;
			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			// 2016-7-5 �ص�Ҫ��ɾ������
			//wpfile<<"G53 X150.000 Y2400.000"<<endl;		

			// 2016-11-14 �ص�Ҫ��ɾ��M100���滻ΪG53 X100.00000 Y2400.00000 M500 ������ TYB16102101
			//wpfile<<"M100"<<endl;
			wpfile << "G53 X100.00000 Y2400.00000 M500" << endl;

			wpfile<<"M801 MSG\"{|K|"<<str_file<<"}\"M400"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"M801 MSG\"{|T|"<<NextPrintFileName.c_str()<<"}\""<<endl;
			}

			wpfile<<"M30"<<endl;
		}

		wpfile.close();



#endif

	}
	else if (cutprint == 9)	// �������� ����CSV�ļ� �� lab�ļ� ���µ�����ϵY�Ǹ��ģ�
	{
		// ����CSV�ļ� 
		// ÿһ��С��zeng

#if 1



		CString CsvFileName = CurPrintFileName.c_str();
		CString LabFileName = CurPrintFileName.c_str();
		CString TxtFileName = CurCutFileName.c_str();

		LabFileName = CsvFileName.Left(CsvFileName.ReverseFind(_T('.')) + 1) + "lab";
		CsvFileName = CsvFileName.Left(CsvFileName.ReverseFind(_T('\\')) + 1) + "TB.csv";
		TxtFileName = TxtFileName.Left(CsvFileName.ReverseFind(_T('\\')) + 1) + m_strOrderNum +".txt";

		std::ofstream csv_file(CsvFileName, std::ios::app);
		std::ofstream lab_file(LabFileName, std::ios::out);


		// ����
		CString material = m_material; 
		CString thickness;
		thickness.Format("%d", (int)(_thickness));
		material = material + '_' + thickness;

		// �ж�txt�ļ��Ƿ���ڣ�����׷�ӣ�������д�ļ�ͷ
		CFileFind find_file;
		if (find_file.FindFile(TxtFileName) == TRUE)
		{
			std::ofstream txt_file(TxtFileName, std::ios::app);

			txt_file << CurCutFileName << ',' << '1' << ',' << '1' << ',' << material << endl;
			txt_file.close();
		}
		else
		{
			// txt�ļ�ͷ
			CString txt_head = _T("OnlyCutting = False");
			std::ofstream txt_file(TxtFileName, std::ios::out);


			txt_file << txt_head << endl;
			txt_file << CurCutFileName << ',' << '1' << ',' << '1' << ',' << material << endl;
			txt_file.close();
		}


		if (csv_file)
		{
			for (int i = 0;i < _newcoor.size();i++)
			{
				std::string info = _newcoor.at(i)._info;

				// ����bmp�ļ���
				CString str_PanelNum;

				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}

				EnDeCsvInfo CsvInfo;

				// ����Ҫ����Ϣ�����ܸı�
				//CsvInfo.strComponentInfo = _newcoor.at(p)._info.c_str();
				CsvInfo.strBmpInfo_Y = str_PanelNum + _T(".bmp");
				CsvInfo.strDirInfo_V = '1';
				CsvInfo.strLabNum_Z.Format("%d", m_nOutputTBNum);
				m_nOutputTBNum++;

				// ������Ϣ�����Ըı�
				for (int q = 0;q < _pinfo.size();q++)
				{
					CString src_info, dst_info; 

					src_info = info.c_str();
					dst_info = _pinfo[q]._info.c_str();
					if (src_info.Compare(dst_info) == 0)
					{
						int num;
						PrintInfo print_info;
						CString pArray[50];
						CString strContent = _pinfo.at(q)._content.c_str();

						// �ָ��ַ���
						Split(strContent, pArray, num, CString(','));

						print_info.noProcessSmallPanelOne1		= pArray[0];
						print_info.noProcessSmallPanelOne2		= pArray[1];
						print_info.noProcessSmallPanelOne3		= pArray[2];
						print_info.noProcessSmallPanelOne4		= pArray[3];
						print_info.noProcessSmallPanelOne5		= pArray[4];

						print_info.noProcessSmallPanelOne6		= pArray[5];
						print_info.noProcessSmallPanelOne7		= pArray[6];
						print_info.noProcessSmallPanelOne8		= pArray[7];
						print_info.noProcessSmallPanelOne9		= pArray[8];
						print_info.noProcessSmallPanelOne10		= pArray[9];

						print_info.noProcessSmallPanelOne11		= pArray[10];
						print_info.noProcessSmallPanelOne12		= pArray[11];
						print_info.noProcessSmallPanelOne13		= pArray[12];
						print_info.noProcessSmallPanelOne14		= pArray[13];
						print_info.noProcessSmallPanelOne15		= pArray[14];

						print_info.noProcessSmallPanelOne16		= pArray[15];
						print_info.noProcessSmallPanelOne17		= pArray[16];
						print_info.noProcessSmallPanelOne18		= pArray[17];
						print_info.noProcessSmallPanelOne19		= pArray[18];
						print_info.noProcessSmallPanelOne20		= pArray[19];

						print_info.noProcessSmallPanelOne21		= pArray[20];
						print_info.noProcessSmallPanelOne22		= pArray[21];
						print_info.noProcessSmallPanelOne23		= pArray[22];		
						print_info.noProcessSmallPanelOne24		= pArray[23];		
						print_info.noProcessSmallPanelOne25		= pArray[24];

						print_info.noProcessSmallPanelOne26		= pArray[25];
						print_info.noProcessSmallPanelOne27		= pArray[26];
						print_info.noProcessSmallPanelOne28		= pArray[27];
						print_info.noProcessSmallPanelOne29		= pArray[28];
						print_info.noProcessSmallPanelOne30		= pArray[29];

						print_info.noProcessSmallPanelOne31		= pArray[30];

						CsvInfo.strInfo_A = print_info.noProcessSmallPanelOne1;
						CsvInfo.strInfo_B = print_info.noProcessSmallPanelOne2;
						CsvInfo.strInfo_C = print_info.noProcessSmallPanelOne3;
						CsvInfo.strInfo_D = print_info.noProcessSmallPanelOne4;
						CsvInfo.strInfo_E = print_info.noProcessSmallPanelOne5;

						CsvInfo.strInfo_F = print_info.noProcessSmallPanelOne6;	
						CsvInfo.strInfo_G = print_info.noProcessSmallPanelOne7;	
						CsvInfo.strInfo_H = print_info.noProcessSmallPanelOne8;	
						CsvInfo.strInfo_I = print_info.noProcessSmallPanelOne9;	
						CsvInfo.strInfo_J = print_info.noProcessSmallPanelOne10;

						CsvInfo.strInfo_K = print_info.noProcessSmallPanelOne11;
						CsvInfo.strInfo_L = print_info.noProcessSmallPanelOne12;
						CsvInfo.strInfo_M = print_info.noProcessSmallPanelOne13;
						CsvInfo.strInfo_N = print_info.noProcessSmallPanelOne14;
						CsvInfo.strInfo_O = print_info.noProcessSmallPanelOne15;

						CsvInfo.strInfo_P = print_info.noProcessSmallPanelOne16;
						CsvInfo.strInfo_Q = print_info.noProcessSmallPanelOne17;
						CsvInfo.strInfo_R = print_info.noProcessSmallPanelOne18;
						CsvInfo.strInfo_S = print_info.noProcessSmallPanelOne19;
						CsvInfo.strInfo_T = print_info.noProcessSmallPanelOne20;


						CsvInfo.strInfo_U = print_info.noProcessSmallPanelOne21;	


						CsvInfo.strInfo_AA = print_info.noProcessSmallPanelOne22;	
						CsvInfo.strInfo_AB = print_info.noProcessSmallPanelOne23;	
						CsvInfo.strInfo_AC = print_info.noProcessSmallPanelOne24;	
						CsvInfo.strInfo_AD = print_info.noProcessSmallPanelOne25;	
						CsvInfo.strInfo_AE = print_info.noProcessSmallPanelOne26;

						CsvInfo.strInfo_AF = print_info.noProcessSmallPanelOne27;	
						CsvInfo.strInfo_AG = print_info.noProcessSmallPanelOne28;	
						CsvInfo.strInfo_AH = print_info.noProcessSmallPanelOne29;	
						CsvInfo.strInfo_AI = print_info.noProcessSmallPanelOne30;	
						CsvInfo.strInfo_AJ = print_info.noProcessSmallPanelOne31;	







						break;
					}
				}







				// дCSV�ļ�
				csv_file	<< CsvInfo.strInfo_A		<< ',' 
					<< CsvInfo.strInfo_B		<< ',' 
					<< CsvInfo.strInfo_C		<< ','
					<< CsvInfo.strInfo_D		<< ','
					<< CsvInfo.strInfo_E		<< ','

					<< CsvInfo.strInfo_F		<< ',' 
					<< CsvInfo.strInfo_G		<< ','
					<< CsvInfo.strInfo_H		<< ','
					<< CsvInfo.strInfo_I		<< ','
					<< CsvInfo.strInfo_J		<< ','

					<< CsvInfo.strInfo_K		<< ','
					<< CsvInfo.strInfo_L		<< ','
					<< CsvInfo.strInfo_M		<< ','
					<< CsvInfo.strInfo_N		<< ','
					<< CsvInfo.strInfo_O		<< ','

					<< CsvInfo.strInfo_P		<< ','
					<< CsvInfo.strInfo_Q		<< ','
					<< CsvInfo.strInfo_R		<< ','
					<< CsvInfo.strInfo_S		<< ','
					<< CsvInfo.strInfo_T		<< ','

					<< CsvInfo.strInfo_U		<< ','

					<< CsvInfo.strDirInfo_V		<< ','
					<< CsvInfo.strZplInfo_W		<< ','
					<< CsvInfo.strZplInfo2_X	<< ','
					<< CsvInfo.strBmpInfo_Y		<< ','
					<< CsvInfo.strLabNum_Z		<< ','

					<< CsvInfo.strInfo_AA		<< ',' 
					<< CsvInfo.strInfo_AB		<< ',' 
					<< CsvInfo.strInfo_AC		<< ','
					<< CsvInfo.strInfo_AD		<< ','
					<< CsvInfo.strInfo_AE		<< ','

					<< CsvInfo.strInfo_AF		<< ',' 
					<< CsvInfo.strInfo_AG		<< ','
					<< CsvInfo.strInfo_AH		<< ','
					<< CsvInfo.strInfo_AI		<< ','
					<< CsvInfo.strInfo_AJ		<< endl;

				// дLab�ļ� X�� Y������������
				CString p_x,p_y;
				float _px, _py;

#if (NEW_PROGRAM == 1)
				
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);

				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = _newcoor.at(i)._x;
					_py = -1.0*_newcoor.at(i)._y;	// �����ĸ߶�

					_px = _px + ((_newcoor.at(i)._width/2.0) > 0 ? (_newcoor.at(i)._width/2.0) : 0);
					_py = _py - ((_newcoor.at(i)._height/2.0) > 0 ? (_newcoor.at(i)._height/2.0) : 0);

					_px = _px > 2400.0 ? 2399 : _px;
					_py = _py < -1190.0 ? -1189 : _py;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = _newcoor.at(i)._x + label_pos.x;
					_py = -1 *(_newcoor.at(i)._y + label_pos.y);

				}
#else
				_px = _newcoor.at(i)._x;
				_py = -1.0*_newcoor.at(i)._y;	// �����ĸ߶�

				_px = _px + ((_newcoor.at(i)._width/2.0) > 0 ? (_newcoor.at(i)._width/2.0) : 0);
				_py = _py - ((_newcoor.at(i)._height/2.0) > 0 ? (_newcoor.at(i)._height/2.0) : 0);

				_px = _px > 2400.0 ? 2399 : _px;
				_py = _py < -1190.0 ? -1189 : _py;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}


				p_x.Format("%0.4f",_px);
				p_y.Format("%0.4f",_py);

				lab_file << p_x << ',' << p_y  << ',' << CsvInfo.strLabNum_Z << ',' << CsvInfo.strDirInfo_V << endl;
			}

			csv_file.close();
			lab_file.close();
		}


#endif

	}
	else if (cutprint == 10)  // �ص�-�´�ϵͳ
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(), std::ios::out);
		if (wpfile)
		{

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;


				// ��������λ��
				float _px, _py;

#if (NEW_PROGRAM == 1)

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.2f",_px);
				str_py.Format("%0.2f",_py);
				wpfile<<"G300 X"<<str_py<<" Y"<<str_px;

				if (1)
				{
					CString str_PanelNum;
					str_PanelNum.Format("%d-%d",PanelNum,i+1);
					int str_len = 8 - str_PanelNum.GetLength();
					for (int q = 0;q < str_len;q++)
					{
						str_PanelNum = "0" + str_PanelNum;
					}

					wpfile<< " F"<< "\"" << str_PanelNum << ".bmp" << "\"" << endl;
				}

				wpfile<<"M200"<<endl;

			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			wpfile << "M101" << endl;	// ����
			wpfile << "M400" << endl;
			wpfile << "G801 P\"" << str_file << "\"" <<endl;

			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"G801 Q\""<<NextPrintFileName.c_str()<<"\""<<endl;
			}

			wpfile<<"M99"<<endl;
		}
		wpfile.close();



#endif

	}
	else if (cutprint == 11)	// �л������Զ��������
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			// �����
			wpfile<<"%"<<endl;
			wpfile<<"O0028"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x			= _newcoor.at(i)._x;
				float src_y			= _newcoor.at(i)._y;
				std::string info	= _newcoor.at(i)._info;
				float _width		= _newcoor.at(i)._width;
				float _height		= _newcoor.at(i)._height;


				wpfile<<"M801 MSG\"|L|";
				std::string get_info = _newcoor.at(i)._info;
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(get_info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile<<_pinfo.at(q)._content.c_str();
						break;
					}
				}
				wpfile<<"\" ;"<<endl;
				wpfile<<"M400 ;"<<endl;


				// ��������λ��
				float _px, _py;

#if (NEW_PROGRAM == 1)

				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}


				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);

				wpfile<<	"G01 X"	<<	str_py	<<	" Y"	<<	str_px	<<	" ;"<<endl;
				wpfile<<	"M401 ;"	<<endl;
			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			wpfile<<"M801 MSG\"|K|"<<str_file<<"\" ;"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"M801 MSG\"|T|"<<NextPrintFileName.c_str()<<"\"  ;"<<endl;
			}

			wpfile<<"%"<<endl;
		}
		wpfile.close();


#endif

	}
	else if (cutprint == 12)  // ������ï-�´�ϵͳ
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(), std::ios::out);
		if (wpfile)
		{

			// ���꿪ͷ
			wpfile << "G54" << endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;


				// ��������λ��
				float _px, _py;

#if (NEW_PROGRAM == 1)
				
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);

				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}




				// ����ͼƬ��
				CString str_px,str_py;
				str_px.Format("%0.3f",_px);
				str_py.Format("%0.3f",_py);

				if (1)
				{
					CString str_PanelNum;
					str_PanelNum.Format("%d-%d",PanelNum,i+1);
					int str_len = 8 - str_PanelNum.GetLength();
					for (int q = 0;q < str_len;q++)
					{
						str_PanelNum = "0" + str_PanelNum;
					}

					wpfile<<"/G300"<< " F"<< "\"" << str_PanelNum << ".bmp" << "\"" << endl;
				}

				wpfile << "G90 G0 " << "A" << str_py << " B" << str_px << endl;

				wpfile<<"M100"<<endl;

			}


			// �������һ������
			if (NextPrintFileName.size() > 0)
			{
				wpfile << "G600 F\"" << NextPrintFileName.c_str() << "\"" << endl;
			}
			else
			{
				// ���һ������
				wpfile << "M82" << endl;
			}

			wpfile<<"M99"<<endl;
		}
		wpfile.close();


#endif

	}
	else if (cutprint == 13)	// ��ŵK1
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			// ���� Z��̧�ߵ�15
			wpfile<<"M300"<<endl;
			wpfile<<"M601"<<endl;
			wpfile<<"G0 Z15.000"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				wpfile<<"M801 MSG\"{|Y|";

				// bmp�ļ���
				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}

				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile << str_PanelNum << ".bmp";
						break;
					}
				}

				wpfile<<"}\""<<endl;

				float _px, _py;

#if (NEW_PROGRAM == 1)
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}

				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 X"<<str_py<<" Y"<<str_px<<" Z-1.000 F24"<<endl;
				wpfile<<"M200"<<endl;
			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			wpfile<<"G53 X0.000 Y0.000"<<endl;
			wpfile<<"M100"<<endl;
			wpfile<<"M801 MSG\"{|K|"<<str_file<<"}\"M400"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"M801 MSG\"{|T|"<<NextPrintFileName.c_str()<<"}\""<<endl;
			}

			wpfile<<"M30"<<endl;
		}
		wpfile.close();


#endif
	}
	else if (cutprint == 14)	// WorkList-�´�
	{

		// �����������������ļ�
#if 0

	std::ofstream wpfile(CurPrintFileName.c_str(), std::ios::out);
	if (wpfile)
	{
	
		for (int i = 0;i < _newcoor.size();i++)
		{
			float src_x = _newcoor.at(i)._x;
			float src_y = _newcoor.at(i)._y;
			std::string info = _newcoor.at(i)._info;
			float _width = _newcoor.at(i)._width;
			float _height = _newcoor.at(i)._height;
	
	
			// ��������λ��
			float _px, _py;
	
	#if (NEW_PROGRAM == 1)

		// ���ұ�ǩλ��
		map<int,PointFloat>::iterator it_label;

		it_label = m_LabelPosition.find(i);


		if(it_label == m_LabelPosition.end())	// δ�ҵ�
		{
			_px = src_x + _width/2;
			_py = src_y + _height/2;
		}
		else	// �ҵ� 
		{
			PointFloat label_pos = it_label->second;

			_px = src_x + label_pos.x;
			_py = src_y + label_pos.y;

		}
	#else
		_px = src_x + _width/2;
		_py = src_y + _height/2;

	#endif
	
			// �������� ˫��2
			if (LabelInReverseSide == true)
			{
				if (node_dvcut == "1")
				{
					_px = m_panelwidth - _px;
				}
				else if (node_dvcut == "2")		
				{
					_py = m_panelheight - _py;
				}
			}
	
	
	
			CString str_px,str_py;
			str_px.Format("%0.2f",_px);
			str_py.Format("%0.2f",_py);
			wpfile<<"G300 X"<<str_py<<" Y"<<str_px;
	
			if (1)
			{
				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}
	
				wpfile<< " F"<< "\"" << str_PanelNum << ".bmp" << "\"" << endl;
			}
	
			wpfile<<"M200"<<endl;
	
		}
	
		CString str_file;
		str_file.Format("%s",CurCutFileName.c_str());
		str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);
	
		wpfile << "M101" << endl;	// ����
		wpfile << "M400" << endl;
		wpfile << "G801 P\"" << str_file << "\"" <<endl;
	
		if (NextPrintFileName.size() > 0)
		{
			wpfile<<"G801 Q\""<<NextPrintFileName.c_str()<<"\""<<endl;
		}
	
		wpfile<<"M99"<<endl;
	}
	wpfile.close();



#endif

	}
	else if (cutprint == 15)  // ��ŵk2
	{

#if 1

		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			// ���� Z��̧�ߵ�15
			wpfile<<"M601"<<endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;

				// ��Ҫ���С��ʹ��������Ϣ
				CString str_PanelNum;
				str_PanelNum.Format("%d-%d",PanelNum,i+1);
				int str_len = 8 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}





				wpfile<<"M801 MSG\"{|Y|";
				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile  <<  _pinfo.at(q)._content.c_str() << "," << str_PanelNum ;
						break;
					}
				}
				wpfile<<"}\""<<endl;

				wpfile<<"M200"<<endl;


				float _px, _py;

#if (NEW_PROGRAM == 1)
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}



				CString str_px,str_py;
				str_px.Format("%0.4f",_px);
				str_py.Format("%0.4f",_py);
				wpfile<<"G01 X"<<str_py<<" Y"<<str_px<<" Z-1.000 F15000"<<endl;
				wpfile<<"M205"<<endl;
			}

			CString str_file;
			str_file.Format("%s",CurCutFileName.c_str());
			str_file = str_file.Right(str_file.GetLength() - str_file.ReverseFind('\\') - 1);

			wpfile<<"G53 X0.000 Y0.000"<<endl;
			wpfile<<"M100"<<endl;
			wpfile<<"M801 MSG\"{|K|"<<str_file<<"}\"M400"<<endl;
			if (NextPrintFileName.size() > 0)
			{
				wpfile<<"M801 MSG\"{|T|"<<NextPrintFileName.c_str()<<"}\""<<endl;
			}

			wpfile<<"M30"<<endl;
		}
		wpfile.close();


#endif
	}
	else if (cutprint == 16)	// ����
	{

#if 1


		std::ofstream wpfile(CurPrintFileName.c_str(),std::ios::out);
		if (wpfile)
		{
			int nCpnCount = _newcoor.size();

			// ��ʼ����
			wpfile << "N0000" << endl;
			wpfile << nCpnCount << endl;

			for (int i = 0;i < _newcoor.size();i++)
			{
				float src_x = _newcoor.at(i)._x;
				float src_y = _newcoor.at(i)._y;
				std::string info = _newcoor.at(i)._info;
				float _width = _newcoor.at(i)._width;
				float _height = _newcoor.at(i)._height;


				// jpg�ļ���
				CString str_PanelNum;
				str_PanelNum.Format("%d", i+1);
				int str_len = 4 - str_PanelNum.GetLength();
				for (int q = 0;q < str_len;q++)
				{
					str_PanelNum = "0" + str_PanelNum;
				}
				str_PanelNum = "N" + str_PanelNum;

				for (int q = 0;q < _pinfo.size();q++)
				{
					if (strcmp(info.c_str(),_pinfo.at(q)._info.c_str()) == 0)
					{
						wpfile << str_PanelNum << endl;
						break;
					}
				}

				

				float _px, _py;

#if (NEW_PROGRAM == 1)
				// ���ұ�ǩλ��
				map<int,PointFloat>::iterator it_label;

				it_label = m_LabelPosition.find(i);


				if(it_label == m_LabelPosition.end())	// δ�ҵ�
				{
					_px = src_x + _width/2;
					_py = src_y + _height/2;
				}
				else	// �ҵ� 
				{
					PointFloat label_pos = it_label->second;

					_px = src_x + label_pos.x;
					_py = src_y + label_pos.y;

				}
#else
				_px = src_x + _width/2;
				_py = src_y + _height/2;

#endif

				// �������� ˫��2
				if (LabelInReverseSide == true)
				{
					if (node_dvcut == "1")
					{
						_px = m_panelwidth - _px;
					}
					else if (node_dvcut == "2")		
					{
						_py = m_panelheight - _py;
					}
				}

				CString str_px,str_py;
				str_px.Format("%0.4f", _px);
				str_py.Format("%0.4f", _py);
				wpfile << "X" << str_py << ",Y" << str_px << endl;
				
			}

			wpfile << "N9999" << endl;
		}
		wpfile.close();


#endif
	}
	else if (cutprint == 17)	// �ȴ�SKL-2613E
	{
		;// 	�˻���û���Զ����ֻ꣬��Ϊ�˼��ļ�ͷβ
	}


#endif

}






vector<PointInfo> KnifeClass::outdirpoint(vector<PointInfo> oldpoint,float dis,int & _pos,std::string panel_info)
{

	// ���е�����
	vector<PointInfo> newpoint;
	for (int i = 0;i < m_disallpoint.size();i++)
	{
		std::string _dpanel_info = m_disallpoint.at(i).panel_info;
		if (strcmp(panel_info.c_str(),_dpanel_info.c_str()) == 0)
		{
			if (oldpoint.at(oldpoint.size() - 1).cut == 1)
			{
				vector<DPointInfo> dnewpoint = m_disallpoint.at(i).point_info;
				int old_pos = _pos;
				for (int j = 0;j < dnewpoint.size();j++)
				{
					if (dnewpoint.at(j).cut == 2)
					{
						break;
					}
					PointInfo one_newpoint;
					one_newpoint.x = dnewpoint.at(j).x;
					one_newpoint.y = dnewpoint.at(j).y;
					one_newpoint.r = dnewpoint.at(j).r;
					one_newpoint.sign = dnewpoint.at(j).sign;
					one_newpoint.dir = dnewpoint.at(j).dir;
					one_newpoint.cut = dnewpoint.at(j).cut;
					if (dnewpoint.at(j).posold == old_pos)
					{
						_pos = dnewpoint.at(j).posnew;
					}
					newpoint.push_back(one_newpoint);
				}
			}
			else if (oldpoint.at(0).cut == 2)
			{
				vector<DPointInfo> dnewpoint = m_disallpoint.at(i).point_info;
				int old_pos = _pos;
				bool begin_insert = false;
				for (int j = 0;j < dnewpoint.size();j++)
				{
					if (dnewpoint.at(j).cut == 2)
					{
						begin_insert = true;
					}
					if (begin_insert == false)
					{
						continue;
					}
					PointInfo one_newpoint;
					one_newpoint.x = dnewpoint.at(j).x;
					one_newpoint.y = dnewpoint.at(j).y;
					one_newpoint.r = dnewpoint.at(j).r;
					one_newpoint.sign = dnewpoint.at(j).sign;
					one_newpoint.dir = dnewpoint.at(j).dir;
					one_newpoint.cut = dnewpoint.at(j).cut;
					if (dnewpoint.at(j).posold == old_pos)
					{
						_pos = dnewpoint.at(j).posnew;
					}
					newpoint.push_back(one_newpoint);
				}
			}
			else
			{
				vector<DPointInfo> dnewpoint = m_disallpoint.at(i).point_info;
				int old_pos = _pos;
				for (int j = 0;j < dnewpoint.size();j++)
				{
					PointInfo one_newpoint;
					one_newpoint.x = dnewpoint.at(j).x;
					one_newpoint.y = dnewpoint.at(j).y;
					one_newpoint.r = dnewpoint.at(j).r;
					one_newpoint.sign = dnewpoint.at(j).sign;
					one_newpoint.dir = dnewpoint.at(j).dir;
					one_newpoint.cut = dnewpoint.at(j).cut;
					if (dnewpoint.at(j).posold == old_pos)
					{
						_pos = dnewpoint.at(j).posnew;
					}
					newpoint.push_back(one_newpoint);
				}
			}
			return newpoint;
		}
	}

	// �����غϵ�
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

#if 1

	filtrationpoint = ALGORITHM_API::FiltrateCoincidentPoints(oldpoint);

#else

	for (int i = 0; i < old_point_size; i++)
	{
		PointInfo one_point,two_point,pre_point;
		one_point = oldpoint.at(i);
		two_point = oldpoint.at((i+1)%oldpoint.size());
		pre_point = oldpoint.at((i+oldpoint.size()-1)%oldpoint.size());
		
		// ��ǰ������һ���غ�
		float dis_prepoint = sqrt((pre_point.x - one_point.x)*(pre_point.x - one_point.x) + (pre_point.y - one_point.y)*(pre_point.y - one_point.y));
		if (dis_prepoint < 0.01 && dis_prepoint > -0.01 && i != 0)
		{
			continue;
		}

		float one_x,one_y,two_x,two_y;
		one_x = one_point.x;
		one_y = one_point.y;
		two_x = two_point.x;
		two_y = two_point.y;

		float dis_point = sqrt((one_x - two_x)*(one_x - two_x) + (one_y - two_y)*(one_y - two_y));

		// ��ǰ������һ�����غϵ�
		if (dis_point < 0.01 && dis_point > -0.01)
		{
			//2016 yuanzb  ���һ���غ��Ҳ���Բ���յ㣬ɾ��
			if (i == old_point_size-1 && (one_point.sign != 2))
			{
				continue;
			}


			PointInfo new_one;
			new_one.x = one_point.x;
			new_one.y = one_point.y;
			if (one_point.sign > 0 && two_point.sign > 0)
			{
				new_one.sign = 3;
				new_one.dir = one_point.dir;
				new_one.r = one_point.r;
			}
			else if (one_point.sign > 0 && two_point.sign == 0)
			{
				new_one.sign = one_point.sign;
				new_one.dir = one_point.dir;
				new_one.r = one_point.r;
			}
			else if (one_point.sign == 0 && two_point.sign > 0)
			{
				new_one.sign = two_point.sign;
				new_one.dir = two_point.dir;
				new_one.r = two_point.r;
			}
			else
			{
				new_one.sign = 0;
				new_one.dir = 0;
				new_one.r = 0.0;
			}
			new_one.cut = one_point.cut;

			filtrationpoint.push_back(new_one);
			if (_pos > i)
			{
				_pos--;
			}
			i++;
		}
		else
		{
			filtrationpoint.push_back(one_point);
		}
	}

	// ��Բ������д���
	for (int i = 0;i < filtrationpoint.size();i++)
	{
		if (filtrationpoint.at(i).r < -0.1 && filtrationpoint.at(i).sign == 1)
		{
			filtrationpoint.at(i).r = filtrationpoint.at(i+1).r;
		}
		if (filtrationpoint.at(i).r < -0.1 && filtrationpoint.at(i).sign == 2)
		{
			filtrationpoint.at(i).r = filtrationpoint.at(i-1).r;
		}
	}


#endif


	// ��ʼƫ��
	for (int i = 0;i < filtrationpoint.size();i++)
	{
		float x1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).x;
		float x2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).x;
		float x3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).x;
		float y1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).y;
		float y2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).y;
		float y3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).y;

		int sign1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).sign;
		int sign2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).sign;
		int sign3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).sign;

		float arcr1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).r;
		float arcr2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).r;
		float arcr3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).r;

		int arcdir1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).dir;
		int arcdir2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).dir;
		int arcdir3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).dir;

		int cut1 = filtrationpoint.at((filtrationpoint.size()+i-1)%filtrationpoint.size()).cut;
		int cut2 = filtrationpoint.at((filtrationpoint.size()+i)%filtrationpoint.size()).cut;
		int cut3 = filtrationpoint.at((filtrationpoint.size()+i+1)%filtrationpoint.size()).cut;

		if (sign2 == 0)
		{
			// �Զ�������Բ��β��������������غ�
			float d = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
			if (sign1 == 2 && i == 0 && d < 1)
			{
				 x1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).x;
				 y1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).y;
				 sign1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).sign;
				 arcr1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).r;
				 arcdir1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).dir;
				 cut1 = filtrationpoint.at((filtrationpoint.size()+i-2)%filtrationpoint.size()).cut;
			}


			float vec_x1 = x2 - x1;
			float vec_x2 = x3 - x2;
			float vec_y1 = y2 - y1;
			float vec_y2 = y3 - y2;

			float m_vec1 = sqrt(vec_x1*vec_x1 + vec_y1*vec_y1);
			float m_vec2 = sqrt(vec_x2*vec_x2 + vec_y2*vec_y2);

			float c_arc = (vec_x1*vec_x2 + vec_y1*vec_y2)/(m_vec1 * m_vec2);


			float dirx1 = x2;
			float diry1 = y2;
			float dirx2 = x3;
			float diry2 = y3;

			ALGORITHM_API::rotatepoint(x1,y1,dirx1,diry1,3.1415926/2.0);
			ALGORITHM_API::rotatepoint(x2,y2,dirx2,diry2,3.1415926/2.0);

			float dis_x1 = (dirx1 - x1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;
			float dis_y1 = (diry1 - y1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;

			float dis_x2 = (dirx2 - x2)/(sqrt((dirx2 - x2)*(dirx2 - x2) + (diry2 - y2)*(diry2 - y2))) * dis;
			float dis_y2 = (diry2 - y2)/(sqrt((dirx2 - x2)*(dirx2 - x2) + (diry2 - y2)*(diry2 - y2))) * dis;

			float new_x1 = x1 + dis_x1;
			float new_x2 = x2 + dis_x1;
			float new_x3 = x2 + dis_x2;
			float new_x4 = x3 + dis_x2;
			float new_y1 = y1 + dis_y1;
			float new_y2 = y2 + dis_y1;
			float new_y3 = y2 + dis_y2;
			float new_y4 = y3 + dis_y2;

			float x_0,y_0,a1,b1,a2,b2,x_1,y_1,x_2,y_2;

			float new_vecx1 = new_x2 - new_x1;
			float new_vecy1 = new_y2 - new_y1;
			float new_vecx2 = new_x4 - new_x3;
			float new_vecy2 = new_y4 - new_y3;

			if (new_vecx1 * new_vecx2 + new_vecy1 * new_vecy2 + 0.1 < 0)
			{
				if ((new_vecx1 > -0.01 && new_vecx1 < 0.01) || (new_vecy1 > -0.01 && new_vecy1 < 0.01))
				{
					x_0 = ((sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1) + dis)/(sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1)))*new_vecx1 + new_x1;
					y_0 = ((sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1) + dis)/(sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1)))*new_vecy1 + new_y1;

					x_1 = ((sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1) + dis)/(sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1)))*new_vecx1 + x1;
					y_1 = ((sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1) + dis)/(sqrt(new_vecx1 * new_vecx1 + new_vecy1 * new_vecy1)))*new_vecy1 + y1;

					x_2 = new_x3;
					y_2 = new_y3;

					PointInfo pos1;
					pos1.x = x_0;
					pos1.y = y_0;
					pos1.r = 0;
					pos1.sign = 0;
					pos1.dir = 0;
					pos1.cut = 0;

					PointInfo pos2;
					pos2.x = x_1;
					pos2.y = y_1;
					pos2.r = 0;
					pos2.sign = 0;
					pos2.dir = 0;
					pos2.cut = 0;

					PointInfo pos3;
					pos3.x = x_2;
					pos3.y = y_2;
					pos3.r = filtrationpoint.at(i).r;
					pos3.sign = filtrationpoint.at(i).sign;
					pos3.dir = filtrationpoint.at(i).dir;
					pos3.cut = filtrationpoint.at(i).cut;

					newpoint.push_back(pos1);
					newpoint.push_back(pos2);
					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
				else
				{
					x_0 = new_x2;
					y_0 = new_y2;

					x_1 = ((sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2) + dis)/(sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2)))*(-1*new_vecx2) + x3;
					y_1 = ((sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2) + dis)/(sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2)))*(-1*new_vecy2) + y3;

					x_2 = ((sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2) + dis)/(sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2)))*(-1*new_vecx2) + new_x4;
					y_2 = ((sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2) + dis)/(sqrt(new_vecx2 * new_vecx2 + new_vecy2 * new_vecy2)))*(-1*new_vecy2) + new_y4;

					PointInfo pos1;
					pos1.x = x_0;
					pos1.y = y_0;
					pos1.r = filtrationpoint.at(i).r;
					pos1.sign = filtrationpoint.at(i).sign;
					pos1.dir = filtrationpoint.at(i).dir;
					pos1.cut = filtrationpoint.at(i).cut;

					PointInfo pos2;
					pos2.x = x_1;
					pos2.y = y_1;
					pos2.r = 0;
					pos2.sign = 0;
					pos2.dir = 0;
					pos2.cut = 0;

					PointInfo pos3;
					pos3.x = x_2;
					pos3.y = y_2;
					pos3.r = 0;
					pos3.sign = 0;
					pos3.dir = 0;
					pos3.cut = 0;

					newpoint.push_back(pos1);
					newpoint.push_back(pos2);
					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
			}
			else
			{
				if (new_x1 - new_x2 > -0.01 && new_x1 - new_x2 < 0.01 && (new_x3 - new_x4 > 0.01 || new_x3 - new_x4 < -0.01))
				{
					a2 = (new_y3 - new_y4)/(new_x3 - new_x4);
					b2 = (new_x3*new_y4 - new_y3*new_x4)/(new_x3 - new_x4);

					x_0 = new_x1;
					y_0 = a2*x_0 + b2;
				}
				else if (new_x3 - new_x4 > -0.01 && new_x3 - new_x4 < 0.01 && (new_x1 - new_x2 > 0.01 || new_x1 - new_x2 < -0.01))
				{
					a1 = (new_y1 - new_y2)/(new_x1 - new_x2);
					b1 = (new_x1*new_y2 - new_y1*new_x2)/(new_x1 - new_x2);

					x_0 = new_x3;
					y_0 = a1*x_0 + b1;
				}
				else
				{
					a1 = (new_y1 - new_y2)/(new_x1 - new_x2);
					b1 = (new_x1*new_y2 - new_y1*new_x2)/(new_x1 - new_x2);

					a2 = (new_y3 - new_y4)/(new_x3 - new_x4);
					b2 = (new_x3*new_y4 - new_y3*new_x4)/(new_x3 - new_x4);

					if (a1 - a2 > -0.01 && a1 - a2 < 0.01)
					{
						x_0 = new_x3;
						y_0 = new_y3;
					}
					else
					{
						x_0 = (b2 - b1)/(a1 - a2);
						y_0 = (a1*b2 - a2*b1)/(a1 - a2);
					}
				}

				PointInfo pos;
				pos.x = x_0;
				pos.y = y_0;
				pos.r = 0.0;
				pos.sign = filtrationpoint.at(i).sign;
				pos.dir = filtrationpoint.at(i).dir;
				pos.cut = filtrationpoint.at(i).cut;

				newpoint.push_back(pos);
			}
		}
		else if (sign2 == 1)	// Բ�����
		{
			if (arcdir2 == 1)
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x2;
				begin_y = y2;
				end_x = x3;
				end_y = y3;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr2,0,x_0,y_0);

				float vec_linex = x2 - x1;
				float vec_liney = y2 - y1;

				float vec_centerx1 = x2 - x_0;
				float vec_centerx2 = x3 - x_0;
				float vec_centery1 = y2 - y_0;
				float vec_centery2 = y3 - y_0;
				float vec_centerz = vec_centerx1*vec_centery2 - vec_centerx2*vec_centery1;
				float vec_tanx = vec_centerx1;
				float vec_tany = vec_centerx2;
				if (vec_centerz > 0)
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,3.1415926/2.0);
				}
				else
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,-1.0*3.1415926/2.0);
				}
				if ((vec_linex*vec_tanx + vec_liney*vec_tany) < 0.0)
				{
					float dirx1 = x2;
					float diry1 = y2;

					ALGORITHM_API::rotatepoint(x1,y1,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;
					float dis_y1 = (diry1 - y1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;

					float new_x1 = x1 + dis_x1;
					float new_x2 = x2 + dis_x1;
					float new_y1 = y1 + dis_y1;
					float new_y2 = y2 + dis_y1;

					float new_disx = (new_x2 - new_x1)/(sqrt((new_x1 - new_x2)*(new_x1 - new_x2) + (new_y1 - new_y2)*(new_y1 - new_y2))) * dis;
					float new_disy = (new_y2 - new_y1)/(sqrt((new_x1 - new_x2)*(new_x1 - new_x2) + (new_y1 - new_y2)*(new_y1 - new_y2))) * dis;

					float new_x = new_x2 + new_disx;
					float new_y = new_y2 + new_disy;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = 0.0;
					pos1.sign = 0;
					pos1.dir = 0;
					pos1.cut = cut2;

					newpoint.push_back(pos1);

					float center_disx = (vec_centerx1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;
					float center_disy = (vec_centery1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_linex1 = vec_linex;
					float vec_liney1 = vec_liney;
					float vec_linex2 = -1.0*dis_x1;
					float vec_liney2 = -1.0*dis_y1;

					if (vec_linex1 > -0.01 && vec_linex1 < 0.01)
					{
						float vec_a = vec_liney2/vec_linex2;
						float vec_b = new_centery - vec_a*new_centerx;

						float intersect_x = new_x;
						float intersect_y = vec_a*new_x + vec_b;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}
					else if (vec_linex2 > -0.01 && vec_linex2 < 0.01)
					{
						float vec_a = vec_liney1/vec_linex1;
						float vec_b = new_y - vec_a*new_x;

						float intersect_x = new_centerx;
						float intersect_y = vec_a*new_centerx + vec_b;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}
					else
					{
						float vec_a1 = vec_liney1/vec_linex1;
						float vec_b1 = new_y - vec_a1*new_x;

						float vec_a2 = vec_liney2/vec_linex2;
						float vec_b2 = new_centery - vec_a2*new_centerx;

						float intersect_x = (vec_b2 - vec_b1)/(vec_a1 - vec_a2);
						float intersect_y = vec_a1 * intersect_x + vec_b1;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}

					PointInfo pos3;
					pos3.x = new_centerx;
					pos3.y = new_centery;
					pos3.r = arcr2 + dis;
					pos3.sign = sign2;
					pos3.dir = arcdir2;
					pos3.cut = cut2;

					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
				else
				{
					float dirx1 = x2;
					float diry1 = y2;

					ALGORITHM_API::rotatepoint(x1,y1,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;
					float dis_y1 = (diry1 - y1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;

					float new_x1 = x1 + dis_x1;
					float new_x2 = x2 + dis_x1;
					float new_y1 = y1 + dis_y1;
					float new_y2 = y2 + dis_y1;

					float center_disx = (vec_centerx1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;
					float center_disy = (vec_centery1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_newx1 = new_x2 - new_x1;
					float vec_newy1 = new_y2 - new_y1;
					float vec_newx2 = new_x1 - x1;
					float vec_newy2 = new_y1 - y1;

					float intersection_x,intersection_y;
					if (vec_newx1 > -0.01 && vec_newx1 < 0.01)
					{
						float line_a2 = vec_newy2/vec_newx2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = new_x1;
						intersection_y = line_a2*intersection_x + line_b2;
						
					}
					else if (vec_newx2 > -0.01 && vec_newx2 < 0.01)
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_b1 = new_y1 - line_a1*new_x1;

						intersection_x = new_centerx;
						intersection_y = line_a1*intersection_x + line_b1;
					}
					else
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_a2 = vec_newy2/vec_newx2;
						float line_b1 = new_y1 - line_a1*new_x1;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = (line_b2 - line_b1)/(line_a1 - line_a2);
						intersection_y = line_a1*intersection_x + line_b1;
					}
					

					float dis_intersection_center = sqrt((intersection_x-new_centerx)*(intersection_x-new_centerx)+(intersection_y-new_centery)*(intersection_y-new_centery));
					if (dis_intersection_center > 0.01 || dis_intersection_center < -0.01)
					{
						PointInfo pos1;
						pos1.x = intersection_x;
						pos1.y = intersection_y;
						pos1.r = 0.0;
						pos1.sign = 0;
						pos1.dir = 0;
						pos1.cut = cut2;

						newpoint.push_back(pos1);

						if (_pos > i)
						{
							_pos = _pos + 1;
						}
					}

					PointInfo pos3;
					pos3.x = new_centerx;
					pos3.y = new_centery;
					pos3.r = arcr2 + dis;
					pos3.sign = sign2;
					pos3.dir = arcdir2;
					pos3.cut = cut2;

					newpoint.push_back(pos3);
				}
			}
			else
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x3;
				begin_y = y3;
				end_x = x2;
				end_y = y2;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr2,0,x_0,y_0);

				float vec_linex = x2 - x1;
				float vec_liney = y2 - y1;

				float vec_centerx1 = x2 - x_0;
				float vec_centerx2 = x3 - x_0;
				float vec_centery1 = y2 - y_0;
				float vec_centery2 = y3 - y_0;
				float vec_centerz = vec_centerx1*vec_centery2 - vec_centerx2*vec_centery1;
				float vec_tanx = vec_centerx1;
				float vec_tany = vec_centerx2;
				if (vec_centerz > 0)
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,3.1415926/2.0);
				}
				else
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,-1.0*3.1415926/2.0);
				}
				if ((vec_linex*vec_tanx + vec_liney*vec_tany) < 0.0)
				{
					float dirx1 = x2;
					float diry1 = y2;

					ALGORITHM_API::rotatepoint(x1,y1,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;
					float dis_y1 = (diry1 - y1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;

					float new_x1 = x1 + dis_x1;
					float new_x2 = x2 + dis_x1;
					float new_y1 = y1 + dis_y1;
					float new_y2 = y2 + dis_y1;

					float new_disx = (new_x2 - new_x1)/(sqrt((new_x1 - new_x2)*(new_x1 - new_x2) + (new_y1 - new_y2)*(new_y1 - new_y2))) * dis;
					float new_disy = (new_y2 - new_y1)/(sqrt((new_x1 - new_x2)*(new_x1 - new_x2) + (new_y1 - new_y2)*(new_y1 - new_y2))) * dis;

					float new_x = new_x2 + new_disx;
					float new_y = new_y2 + new_disy;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = 0.0;
					pos1.sign = 0;
					pos1.dir = 0;
					pos1.cut = cut2;

					newpoint.push_back(pos1);

					float center_disx = -1.0*(vec_centerx1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;
					float center_disy = -1.0*(vec_centery1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_linex1 = vec_linex;
					float vec_liney1 = vec_liney;
					float vec_linex2 = -1.0*dis_x1;
					float vec_liney2 = -1.0*dis_y1;

					if (vec_linex1 > -0.01 && vec_linex1 < 0.01)
					{
						float vec_a = vec_liney2/vec_linex2;
						float vec_b = new_centery - vec_a*new_centerx;

						float intersect_x = new_x;
						float intersect_y = vec_a*new_x + vec_b;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}
					else if (vec_linex2 > -0.01 && vec_linex2 < 0.01)
					{
						float vec_a = vec_liney1/vec_linex1;
						float vec_b = new_y - vec_a*new_x;

						float intersect_x = new_centerx;
						float intersect_y = vec_a*new_centerx + vec_b;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}
					else
					{
						float vec_a1 = vec_liney1/vec_linex1;
						float vec_b1 = new_y - vec_a1*new_x;

						float vec_a2 = vec_liney2/vec_linex2;
						float vec_b2 = new_centery - vec_a2*new_centerx;

						float intersect_x = (vec_b2 - vec_b1)/(vec_a1 - vec_a2);
						float intersect_y = vec_a1 * intersect_x + vec_b1;

						PointInfo pos2;
						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;

						newpoint.push_back(pos2);
					}

					PointInfo pos3;
					pos3.x = new_centerx;
					pos3.y = new_centery;
					pos3.r = arcr2 + dis;
					pos3.sign = sign2;
					pos3.dir = arcdir2;
					pos3.cut = cut2;

					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
				else
				{
					float dirx1 = x2;
					float diry1 = y2;

					ALGORITHM_API::rotatepoint(x1,y1,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;
					float dis_y1 = (diry1 - y1)/(sqrt((dirx1 - x1)*(dirx1 - x1) + (diry1 - y1)*(diry1 - y1))) * dis;

					float new_x1 = x1 + dis_x1;
					float new_x2 = x2 + dis_x1;
					float new_y1 = y1 + dis_y1;
					float new_y2 = y2 + dis_y1;

					float center_disx = -1.0*(vec_centerx1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;
					float center_disy = -1.0*(vec_centery1/(sqrt(vec_centerx1*vec_centerx1 + vec_centery1*vec_centery1))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_newx1 = new_x2 - new_x1;
					float vec_newy1 = new_y2 - new_y1;
					float vec_newx2 = new_x1 - x1;
					float vec_newy2 = new_y1 - y1;

					float intersection_x,intersection_y;
					if (vec_newx1 > -0.01 && vec_newx1 < 0.01)
					{
						float line_a2 = vec_newy2/vec_newx2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = new_x1;
						intersection_y = line_a2*intersection_x + line_b2;

					}
					else if (vec_newx2 > -0.01 && vec_newx2 < 0.01)
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_b1 = new_y1 - line_a1*new_x1;

						intersection_x = new_centerx;
						intersection_y = line_a1*intersection_x + line_b1;
					}
					else
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_a2 = vec_newy2/vec_newx2;
						float line_b1 = new_y1 - line_a1*new_x1;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = (line_b2 - line_b1)/(line_a1 - line_a2);
						intersection_y = line_a1*intersection_x + line_b1;
					}


					float dis_intersection_center = sqrt((intersection_x-new_centerx)*(intersection_x-new_centerx)+(intersection_y-new_centery)*(intersection_y-new_centery));
					if (dis_intersection_center > 0.01 || dis_intersection_center < -0.01)
					{
						PointInfo pos1;
						pos1.x = intersection_x;
						pos1.y = intersection_y;
						pos1.r = 0.0;
						pos1.sign = 0;
						pos1.dir = 0;
						pos1.cut = cut2;

						newpoint.push_back(pos1);

						if (_pos > i)
						{
							_pos = _pos + 1;
						}
					}


					PointInfo pos3;
					pos3.x = new_centerx;
					pos3.y = new_centery;
					pos3.r = arcr2 + dis;
					pos3.sign = sign2;
					pos3.dir = arcdir2;
					pos3.cut = cut2;

					newpoint.push_back(pos3);
				}
			}
		}
		else if (sign2 == 2)	// Բ���յ�
		{
			if (arcdir2 == 1)
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x1;
				begin_y = y1;
				end_x = x2;
				end_y = y2;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr2,0,x_0,y_0);

				float vec_linex = x3 - x2;
				float vec_liney = y3 - y2;

				float vec_centerx1 = x1 - x_0;
				float vec_centerx2 = x2 - x_0;
				float vec_centery1 = y1 - y_0;
				float vec_centery2 = y2 - y_0;
				float vec_centerz = vec_centerx1*vec_centery2 - vec_centerx2*vec_centery1;
				float vec_tanx = vec_centerx2;
				float vec_tany = vec_centery2;

				if (vec_centerz > 0)
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,3.1415926/2.0);
				}
				else
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,-1.0*3.1415926/2.0);
				}
				if ((vec_linex*vec_tanx + vec_liney*vec_tany) < 0.0)
				{
					float dirx1 = x3;
					float diry1 = y3;

					ALGORITHM_API::rotatepoint(x2,y2,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;
					float dis_y1 = (diry1 - y2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;

					float new_x2 = x2 + dis_x1;
					float new_x3 = x3 + dis_x1;
					float new_y2 = y2 + dis_y1;
					float new_y3 = y3 + dis_y1;

					float new_disx = (new_x3 - new_x2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;
					float new_disy = (new_y3 - new_y2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;

					float new_x = new_x2 - new_disx;
					float new_y = new_y2 - new_disy;

					PointInfo pos3;
					pos3.x = new_x;
					pos3.y = new_y;
					pos3.r = 0.0;
					pos3.sign = 0;
					pos3.dir = 0;
					pos3.cut = cut2;

					float center_disx = (vec_centerx2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;
					float center_disy = (vec_centery2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_linex1 = vec_linex;
					float vec_liney1 = vec_liney;
					float vec_linex2 = dis_x1;
					float vec_liney2 = dis_y1;

					PointInfo pos2;

					if (vec_linex1 > -0.01 && vec_linex1 < 0.01)
					{
						float vec_a = vec_liney2/vec_linex2;
						float vec_b = new_centery - vec_a*new_centerx;

						float intersect_x = new_x;
						float intersect_y = vec_a*new_x + vec_b;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}
					else if (vec_linex2 > -0.01 && vec_linex2 < 0.01)
					{
						float vec_a = vec_liney1/vec_linex1;
						float vec_b = new_y - vec_a*new_x;

						float intersect_x = new_centerx;
						float intersect_y = vec_a*new_centerx + vec_b;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}
					else
					{
						float vec_a1 = vec_liney1/vec_linex1;
						float vec_b1 = new_y - vec_a1*new_x;

						float vec_a2 = vec_liney2/vec_linex2;
						float vec_b2 = new_centery - vec_a2*new_centerx;

						float intersect_x = (vec_b2 - vec_b1)/(vec_a1 - vec_a2);
						float intersect_y = vec_a1 * intersect_x + vec_b1;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}

					PointInfo pos1;
					pos1.x = new_centerx;
					pos1.y = new_centery;
					pos1.r = arcr2 + dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
					newpoint.push_back(pos2);
					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
				else
				{
					float dirx1 = x3;
					float diry1 = y3;

					ALGORITHM_API::rotatepoint(x2,y2,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;
					float dis_y1 = (diry1 - y2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;

					float new_x2 = x2 + dis_x1;
					float new_x3 = x3 + dis_x1;
					float new_y2 = y2 + dis_y1;
					float new_y3 = y3 + dis_y1;

					/*float new_disx = (new_x3 - new_x2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;
					float new_disy = (new_y3 - new_y2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;

					float new_x = new_x2 - new_disx;
					float new_y = new_y2 - new_disy;

					PointInfo pos3;
					pos3.x = new_x;
					pos3.y = new_y;
					pos3.r = 0.0;
					pos3.sign = 0;
					pos3.dir = 0;
					pos3.cut = cut2;*/

					float center_disx = (vec_centerx2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;
					float center_disy = (vec_centery2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					PointInfo pos1;
					pos1.x = new_centerx;
					pos1.y = new_centery;
					pos1.r = arcr2 + dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);

					float vec_newx1 = new_x3 - new_x2;
					float vec_newy1 = new_y3 - new_y2;
					float vec_newx2 = new_x2 - x2;
					float vec_newy2 = new_y2 - y2;

					float intersection_x,intersection_y;
					if (vec_newx1 > -0.01 && vec_newx1 < 0.01)
					{
						float line_a2 = vec_newy2/vec_newx2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = new_x2;
						intersection_y = line_a2*intersection_x + line_b2;

					}
					else if (vec_newx2 > -0.01 && vec_newx2 < 0.01)
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_b1 = new_y2 - line_a1*new_x2;

						intersection_x = new_centerx;
						intersection_y = line_a1*intersection_x + line_b1;
					}
					else
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_a2 = vec_newy2/vec_newx2;
						float line_b1 = new_y2 - line_a1*new_x2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = (line_b2 - line_b1)/(line_a1 - line_a2);
						intersection_y = line_a1*intersection_x + line_b1;
					}


					float dis_intersection_center = sqrt((intersection_x-new_centerx)*(intersection_x-new_centerx)+(intersection_y-new_centery)*(intersection_y-new_centery));
					if (dis_intersection_center > 0.01 || dis_intersection_center < -0.01)
					{
						PointInfo pos1;
						pos1.x = intersection_x;
						pos1.y = intersection_y;
						pos1.r = 0.0;
						pos1.sign = 0;
						pos1.dir = 0;
						pos1.cut = cut2;

						newpoint.push_back(pos1);

						if (_pos > i)
						{
							_pos = _pos + 1;
						}
					}
					/*newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 1;
					}*/
				}
			}
			else
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x2;
				begin_y = y2;
				end_x = x1;
				end_y = y1;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr2,0,x_0,y_0);

				float vec_linex = x3 - x2;
				float vec_liney = y3 - y2;

				float vec_centerx1 = x1 - x_0;
				float vec_centerx2 = x2 - x_0;
				float vec_centery1 = y1 - y_0;
				float vec_centery2 = y2 - y_0;
				float vec_centerz = vec_centerx1*vec_centery2 - vec_centerx2*vec_centery1;
				float vec_tanx = vec_centerx2;
				float vec_tany = vec_centery2;

				if (vec_centerz > 0)
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,3.1415926/2.0);
				}
				else
				{
					ALGORITHM_API::rotatepoint(0.0,0.0,vec_tanx,vec_tany,-1.0*3.1415926/2.0);
				}
				if ((vec_linex*vec_tanx + vec_liney*vec_tany) < 0.0)
				{
					float dirx1 = x3;
					float diry1 = y3;

					ALGORITHM_API::rotatepoint(x2,y2,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;
					float dis_y1 = (diry1 - y2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;

					float new_x2 = x2 + dis_x1;
					float new_x3 = x3 + dis_x1;
					float new_y2 = y2 + dis_y1;
					float new_y3 = y3 + dis_y1;

					float new_disx = (new_x3 - new_x2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;
					float new_disy = (new_y3 - new_y2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;

					float new_x = new_x2 - new_disx;
					float new_y = new_y2 - new_disy;

					PointInfo pos3;
					pos3.x = new_x;
					pos3.y = new_y;
					pos3.r = 0.0;
					pos3.sign = 0;
					pos3.dir = 0;
					pos3.cut = cut2;

					float center_disx = -1.0*(vec_centerx2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;
					float center_disy = -1.0*(vec_centery2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					float vec_linex1 = vec_linex;
					float vec_liney1 = vec_liney;
					float vec_linex2 = dis_x1;
					float vec_liney2 = dis_y1;

					PointInfo pos2;

					if (vec_linex1 > -0.01 && vec_linex1 < 0.01)
					{
						float vec_a = vec_liney2/vec_linex2;
						float vec_b = new_centery - vec_a*new_centerx;

						float intersect_x = new_x;
						float intersect_y = vec_a*new_x + vec_b;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}
					else if (vec_linex2 > -0.01 && vec_linex2 < 0.01)
					{
						float vec_a = vec_liney1/vec_linex1;
						float vec_b = new_y - vec_a*new_x;

						float intersect_x = new_centerx;
						float intersect_y = vec_a*new_centerx + vec_b;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}
					else
					{
						float vec_a1 = vec_liney1/vec_linex1;
						float vec_b1 = new_y - vec_a1*new_x;

						float vec_a2 = vec_liney2/vec_linex2;
						float vec_b2 = new_centery - vec_a2*new_centerx;

						float intersect_x = (vec_b2 - vec_b1)/(vec_a1 - vec_a2);
						float intersect_y = vec_a1 * intersect_x + vec_b1;

						pos2.x = intersect_x;
						pos2.y = intersect_y;
						pos2.r = 0;
						pos2.sign = 0;
						pos2.dir = 0;
						pos2.cut = cut2;
					}

					PointInfo pos1;
					pos1.x = new_centerx;
					pos1.y = new_centery;
					pos1.r = arcr2 - dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
					newpoint.push_back(pos2);
					newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 2;
					}
				}
				else
				{
					float dirx1 = x3;
					float diry1 = y3;

					ALGORITHM_API::rotatepoint(x2,y2,dirx1,diry1,3.1415926/2.0);
					float dis_x1 = (dirx1 - x2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;
					float dis_y1 = (diry1 - y2)/(sqrt((dirx1 - x2)*(dirx1 - x2) + (diry1 - y2)*(diry1 - y2))) * dis;

					float new_x2 = x2 + dis_x1;
					float new_x3 = x3 + dis_x1;
					float new_y2 = y2 + dis_y1;
					float new_y3 = y3 + dis_y1;

					float new_disx = (new_x3 - new_x2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;
					float new_disy = (new_y3 - new_y2)/(sqrt((new_x2 - new_x3)*(new_x2 - new_x3) + (new_y2 - new_y3)*(new_y2 - new_y3))) * dis;

					float new_x = new_x2 - new_disx;
					float new_y = new_y2 - new_disy;

					/*PointInfo pos3;
					pos3.x = new_x;
					pos3.y = new_y;
					pos3.r = 0.0;
					pos3.sign = 0;
					pos3.dir = 0;
					pos3.cut = cut2;*/

					float center_disx = -1.0*(vec_centerx2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;
					float center_disy = -1.0*(vec_centery2/(sqrt(vec_centerx2*vec_centerx2 + vec_centery2*vec_centery2))) * dis;

					float new_centerx = x2 + center_disx;
					float new_centery = y2 + center_disy;

					PointInfo pos1;
					pos1.x = new_centerx;
					pos1.y = new_centery;
					pos1.r = arcr2 - dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
					/*newpoint.push_back(pos3);

					if (_pos > i)
					{
						_pos = _pos + 1;
					}*/

					float vec_newx1 = new_x3 - new_x2;
					float vec_newy1 = new_y3 - new_y2;
					float vec_newx2 = new_x2 - x2;
					float vec_newy2 = new_y2 - y2;

					float intersection_x,intersection_y;
					if (vec_newx1 > -0.01 && vec_newx1 < 0.01)
					{
						float line_a2 = vec_newy2/vec_newx2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = new_x2;
						intersection_y = line_a2*intersection_x + line_b2;

					}
					else if (vec_newx2 > -0.01 && vec_newx2 < 0.01)
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_b1 = new_y2 - line_a1*new_x2;

						intersection_x = new_centerx;
						intersection_y = line_a1*intersection_x + line_b1;
					}
					else
					{
						float line_a1 = vec_newy1/vec_newx1;
						float line_a2 = vec_newy2/vec_newx2;
						float line_b1 = new_y2 - line_a1*new_x2;
						float line_b2 = new_centery - line_a2*new_centerx;

						intersection_x = (line_b2 - line_b1)/(line_a1 - line_a2);
						intersection_y = line_a1*intersection_x + line_b1;
					}


					float dis_intersection_center = sqrt((intersection_x-new_centerx)*(intersection_x-new_centerx)+(intersection_y-new_centery)*(intersection_y-new_centery));
					if (dis_intersection_center > 0.01 || dis_intersection_center < -0.01)
					{
						PointInfo pos1;
						pos1.x = intersection_x;
						pos1.y = intersection_y;
						pos1.r = 0.0;
						pos1.sign = 0;
						pos1.dir = 0;
						pos1.cut = cut2;

						newpoint.push_back(pos1);

						if (_pos > i)
						{
							_pos = _pos + 1;
						}
					}
				}
			}
		}
		else if (sign2 == 3)	// ���ӵ�
		{
			float pre_center_x,cur_center_x,pre_center_y,cur_center_y;
			if (arcdir1 == 1)
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x1;
				begin_y = y1;
				end_x = x2;
				end_y = y2;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr1,0,pre_center_x,pre_center_y);
			}
			else
			{
				float x_0,y_0;
				float begin_x,begin_y,end_x,end_y;

				begin_x = x2;
				begin_y = y2;
				end_x = x1;
				end_y = y1;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr1,0,pre_center_x,pre_center_y);
			}
			if (arcdir3 == 1)
			{
				float begin_x,begin_y,end_x,end_y;

				begin_x = x2;
				begin_y = y2;
				end_x = x3;
				end_y = y3;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr1,0,cur_center_x,cur_center_y);		// ������Բ�������

// 				float begin_x2,begin_y2,end_x2,end_y2;
// 
// 				begin_x2 = x3;
// 				begin_y2 = y3;
// 				end_x2 = x2;
// 				end_y2 = y2;
// 
// 				getCircleCenter(begin_x2,end_x2,begin_y2,end_y2,arcr1,0,cur_center_x,cur_center_y);
			}
			else
			{
				float begin_x,begin_y,end_x,end_y;

				begin_x = x3;
				begin_y = y3;
				end_x = x2;
				end_y = y2;

				ALGORITHM_API::getCircleCenter(begin_x,end_x,begin_y,end_y,arcr1,0,cur_center_x,cur_center_y);
			}

			float vec_linex1 = pre_center_x - x2;
			float vec_liney1 = pre_center_y - y2;
			float vec_linex2 = cur_center_x - x2;
			float vec_liney2 = cur_center_y - y2;
			float vec_linex3 = cur_center_x - x3;
			float vec_liney3 = cur_center_y - y3;

			float vec_disx1 = vec_linex1/(sqrt(vec_linex1*vec_linex1 + vec_liney1*vec_liney1)) * dis;
			float vec_disy1 = vec_liney1/(sqrt(vec_linex1*vec_linex1 + vec_liney1*vec_liney1)) * dis;
			float vec_disx2 = vec_linex2/(sqrt(vec_linex2*vec_linex2 + vec_liney2*vec_liney2)) * dis;
			float vec_disy2 = vec_liney2/(sqrt(vec_linex2*vec_linex2 + vec_liney2*vec_liney2)) * dis;
			float vec_disx3 = vec_linex3/(sqrt(vec_linex3*vec_linex3 + vec_liney3*vec_liney3)) * dis;
			float vec_disy3 = vec_liney3/(sqrt(vec_linex3*vec_linex3 + vec_liney3*vec_liney3)) * dis;

			float point_dis = sqrt((vec_disx2-vec_disx1)*(vec_disx2-vec_disx1) + (vec_disy2-vec_disy1)*(vec_disy2-vec_disy1));
			if (point_dis > -0.1 && point_dis < 0.1)	// Բ����ͬ����Բ��ֵ�СԲ��
			{
				if (arcdir1 == 1)
				{
					float new_x = x2 - vec_disx1;
					float new_y = y2 - vec_disy1;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = arcr2 + dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
				}
				else
				{
					float new_x = x2 + vec_disx1;
					float new_y = y2 + vec_disy1;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = arcr2 - dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
				}
			}
			else	// Բ�Ĳ�ͬ��������Բ��
			{
				// ����Բ������δ����
				if (arcdir3 == 1)	// ��ʱ��
				{
					float new_x = x2 - vec_disx1;
					float new_y = y2 - vec_disy1;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = arcr2 + dis;	// �뾶����
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
				}
				else	// ˳ʱ��
				{
					float new_x = x2 + vec_disx1;
					float new_y = y2 + vec_disy1;

					PointInfo pos1;
					pos1.x = new_x;
					pos1.y = new_y;
					pos1.r = arcr2 - dis;
					pos1.sign = sign2;
					pos1.dir = arcdir2;
					pos1.cut = cut2;

					newpoint.push_back(pos1);
				}


			}
		}
		else
		{
			//AfxMessageBox("���Եĵ㣬sign = -1");
		}
	}
	/*std::ofstream wfile1("D:\\log1.txt",std::ios::out | std::ios::app);
	for (int i = 0;i < newpoint.size();i++)
	{
		wfile1<<newpoint.at(i).x<<" "<<newpoint.at(i).y<<" "<<newpoint.at(i).r<<" "<<newpoint.at(i).sign<<" "<<newpoint.at(i).dir<<endl;
	}
	wfile1.close();*/
	return newpoint;
}









void KnifeClass::OutputGCode_KnifeOnePanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
											int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
											float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed, int decimal_place)
{

	// ���￪ʼ
	int nDPointNum = newpoint.size();
	int pos2 = (pos+1)%nDPointNum;

	// ���䵽����м�⣬��ֹ����Խ�������
	if (pos >= nDPointNum)
	{
		pos = 0;
		pos2 = 1;

		//AfxMessageBox(_T("�䵶�㳬���������������䵽����Ϊ0��"));
	}


	// �������꣬������ļ�
	PointInfo c_p1, c_p2;
	c_p1 = newpoint.at(pos);
	c_p2 = newpoint.at(pos2);
	//c_p2 = newpoint.at((pos+1)%points.size());

	float L, dx, dy, dist1, dist2;

	dx = c_p1.x - c_p2.x;
	dy = c_p1.y - c_p2.y;
	L = sqrt(dx*dx + dy*dy);
	dist1 = gcodeparam.distance1;
	dist2 = gcodeparam.distance2;

	float x_dis0,y_dis0,x_dis1,y_dis1,x_end,y_end;

	// �µ���ʽ�������	
	if ((c_p1.sign == 1 || c_p1.sign == 3) && (c_p2.sign == 2 || c_p2.sign == 3))
	{
		// Բ��ֱ���µ�
		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}
	else if (L < dist1 + dist2)	
	{
		// ��ֱ��ֱ���µ�
		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}
	else
	{
		// ����50mm��б�µ�
		float m_disx1 = ((c_p1.x - c_p2.x)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*gcodeparam.distance1;
		float m_disx2 = ((c_p1.x - c_p2.x)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*(gcodeparam.distance2 + m_kerfwidth/2.0);
		float m_disy1 = ((c_p1.y - c_p2.y)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*gcodeparam.distance1;
		float m_disy2 = ((c_p1.y - c_p2.y)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*(gcodeparam.distance2 + m_kerfwidth/2.0);


		float m_disx1_1 = (dx/L) * dist1;
		float m_disx2_1 = (dx/L) * (dist2 + m_kerfwidth/2.0);
		float m_disy1_1 = (dy/L) * dist1;
		float m_disy2_1 = (dy/L) * (dist2 + m_kerfwidth/2.0);




		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x + m_disx2;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y + m_disy2;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x + m_disx1 + m_disx2;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y + m_disy1 + m_disy2;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}


	char chheight1[12], cheight2[12];
	char chx_0[12];
	char chx_1[12];
	char chy_0[12],chy_1[12], chheight3[12];
	char chx_2[12];
	char chy_2[12];

	// ��ȫ�߶�
	//sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
	PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);



	if (gcodeparam.downheight > 1000)
	{
		float up_knife = -1.0 * k_l * knift_lenght;
		//sprintf_s(chheight3,"%0.4f",up_knife);
		PrintNumber2Array(chheight3, 12, decimal_place, up_knife);
	}
	else
	{	// �ϱ���߶� 2018-4-21 ���Ҫ�����ӶԵ�ԭ��ƫ�ƣ�����һ������߶ȣ���ֹ����
		float up_knife = _thickness - k_l*knift_lenght + gcodeparam.org_offset;
		//sprintf_s(chheight3,"%0.4f",up_knife);
		PrintNumber2Array(chheight3, 12, decimal_place, up_knife);

		if (gcodeparam.downheight > 0.5)
		{
			//sprintf_s(chheight3,"%0.4f",up_knife - gcodeparam.downheight);
			PrintNumber2Array(chheight3, 12, decimal_place, up_knife - gcodeparam.downheight);
		}
	}

	if (gcodeparam.downheight > 1000)
	{
		if (info.at(0) == '!' && info.at(1) == '!')
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness - m_leftthin) ? (_thickness - m_leftthin) : knift_lenght * (k_l+1);
			//sprintf_s(cheight2,"%0.4f",(down_knife+0.1)*(-1));
			PrintNumber2Array(cheight2, 12, decimal_place,(down_knife+0.1)*(-1));
		}
		else
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness) ? (_thickness) : knift_lenght * (k_l+1);
			//sprintf_s(cheight2,"%0.4f",(down_knife+0.1)*(-1));
			PrintNumber2Array(cheight2, 12, decimal_place, (down_knife+0.1)*(-1));
		}

	}
	else
	{
		if (info.at(0) == '!' && info.at(1) == '!')
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness - m_leftthin) ? ((-1.0) * (gcodeparam.downheight) + m_leftthin) : (_thickness - m_leftthin) - knift_lenght * (k_l+1);
			//sprintf_s(cheight2,"%0.4f",down_knife);
			PrintNumber2Array(cheight2, 12, decimal_place, down_knife);
		}
		else
		{	// ����ʱ�µ��߶�
			float down_knife = knift_lenght * (k_l+1) > _thickness ? (-1.0) * (gcodeparam.downheight) : _thickness - knift_lenght * (k_l+1);
			//sprintf_s(cheight2,"%0.4f",down_knife);
			PrintNumber2Array(cheight2, 12, decimal_place , down_knife);
		}

	}

	// �䵶 ����ָ��λ��
	//sprintf_s(chy_1,"%0.4f",x_dis1);		// ��㣬׼���䵶 |
	//sprintf_s(chx_1,"%0.4f",y_dis1);
	//sprintf_s(chx_0,"%0.4f",y_dis0);		// �䵶�������		
	//sprintf_s(chy_0,"%0.4f",x_dis0);
	//sprintf_s(chx_2,"%0.4f",x_end);		// �䵶��͵����ƽ��һ�ε�������	-
	//sprintf_s(chy_2,"%0.4f",y_end);


	PrintNumber2Array(chy_1,  12, decimal_place, x_dis1);		// ��㣬׼���䵶 |
	PrintNumber2Array(chx_1,  12, decimal_place, y_dis1);
	PrintNumber2Array(chx_0,  12, decimal_place, y_dis0);		// �䵶�������		
	PrintNumber2Array(chy_0,  12, decimal_place, x_dis0);
	PrintNumber2Array(chx_2,  12, decimal_place, x_end);		// �䵶��͵����ƽ��һ�ε�������	-
	PrintNumber2Array(chy_2,  12, decimal_place, y_end);

	if (coortype == 0)
	{
		wfile<<"G00"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight1;	// ��ȫ�߶�
		if (knife_overspend > 0.01)
			wfile<<" F "<<knife_overspend;
		wfile<<endl;
		wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight3;	// ���ϱ���߶�
		if (knife_facespeed > 0.01)
			wfile<<" F "<<knife_facespeed;
		wfile<<endl;
		wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;	// ������͸߶ȣ�ֻ��һ�ξ���-0.1���д�
		if (knife_spindlespeed > 0.01)
			wfile<<" F "<<knife_spindlespeed;
		wfile<<endl;
	}
	else
	{
		wfile<<"G00"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<chheight1<<" F "<<knife_overspend<<endl;
		wfile<<"G01"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<chheight3<<" F "<<knife_facespeed<<endl;
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<knife_spindlespeed<<endl;
	}


	// ʣ������е�
	for (int n = 0;n < newpoint.size();n++)
	{
		int key,prekey;

		key = (pos + 1 + n)%newpoint.size();
		prekey = (pos + n)%newpoint.size();

		PointInfo newpos = newpoint.at(key);
		float pos_x = newpos.x + src_x;
		float pos_y = newpos.y + src_y;
		float pos_r = newpos.r;
		int pos_sign = newpos.sign;
		int pos_dir = newpos.dir;
		int pos_cut = newpos.cut;

		PointInfo prenewpos = newpoint.at(prekey);
		float prepos_x = prenewpos.x + src_x;
		float prepos_y = prenewpos.y + src_y;
		float prepos_r = prenewpos.r;
		int prepos_sign = prenewpos.sign;
		int prepos_dir = prenewpos.dir;
		int prepos_cut = prenewpos.cut;				

		char chx[12],chy[12],chr[12],chpr[12];
		//sprintf_s(chx,"%0.4f",pos_y);
		//sprintf_s(chy,"%0.4f",pos_x);
		//sprintf_s(chr,"%0.4f",pos_r);
		//sprintf_s(chpr,"%0.4f",prepos_r);

		PrintNumber2Array(chx,	  12, decimal_place, pos_y);
		PrintNumber2Array(chy,	  12, decimal_place, pos_x);
		PrintNumber2Array(chr,	  12, decimal_place, pos_r);
		PrintNumber2Array(chpr,	  12, decimal_place, prepos_r);

		if (pos_sign == 0 || pos_sign == 1)		// ֱ�ߣ�Բ�����
		{
			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (knife_feedrate > 0.01)
					wfile<<" F "<<knife_feedrate;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<" F "<<knife_feedrate<<endl;
			}
		}
		else if (pos_sign == 2)	// ��ǰ��ΪԲ���յ�
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else if (pos_sign == 3)	// Բ���յ㣬��Ҫ��Բ��
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else
		{
			if (key != 1)
			{
				if (pos_dir == 1)
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
				else
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
			}

			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (knife_feedrate > 0.01)
					wfile<<"F "<<knife_feedrate;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<"F "<<knife_feedrate<<endl;
			}
		}
	}

	// ���һ�ߣ���̧��
	// �����һ���㵽�ڶ�������Բ����Բ�������ڶ�����
	if (c_p1.sign == 1 || c_p1.sign == 3)
	{
		float beginpos_x = c_p2.x + src_x;
		float beginpos_y = c_p2.y + src_y;
		float beginpos_r = c_p2.r;
		int beginpos_sign = c_p2.sign;
		int beginpos_dir = c_p2.dir;
		int beginpos_cut = c_p2.cut;

		char beginchx[12],beginchy[12],beginchr[12];
		//sprintf_s(beginchx,"%0.4f",beginpos_y);
		//sprintf_s(beginchy,"%0.4f",beginpos_x); 
		//sprintf_s(beginchr,"%0.4f",beginpos_r);

		PrintNumber2Array(beginchx,  12, decimal_place, beginpos_y);
		PrintNumber2Array(beginchy,  12, decimal_place, beginpos_x); 
		PrintNumber2Array(beginchr,  12, decimal_place, beginpos_r);

		if (beginpos_dir == 1)
		{
			if (coortype == 0)
			{
				wfile<<"G90 G03"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G03"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}
		}
		else
		{
			if (coortype == 0)
			{
				wfile<<"G90 G02"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G02"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}

		}
	}

	// �ص�ԭʼ�߶�
	if (coortype == 0)
	{
		wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<cheight2<<endl;	// �ص��ʼ�µ�λ��
		wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;		// �ص����굶��λ��
		if (knife_endspeed > 0.01)
			wfile<<" F "<<knife_endspeed;
		wfile<<endl;

		if (KnifeOffset == true)
		{
			wfile<<"G01"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<cheight2;
			if (knife_endspeed > 0.01)
				wfile<<" F "<<knife_endspeed;
			wfile<<endl;
			wfile<<"G00"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<chheight1;
			if (knife_overspend > 0.01)
				wfile<<" F "<<knife_overspend;
			wfile<<endl;
		}
		else
		{
			wfile<<"G00"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight1;		// ̧������ȫ�߶�
			if (knife_overspend > 0.01)
				wfile<<" F "<<knife_overspend;
			wfile<<endl;
		}
	}
	else
	{
		wfile<<"G01"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<cheight2<<endl;
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<knife_endspeed<<endl;
		wfile<<"G00"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<chheight1<<" F "<<knife_overspend<<endl;
	}

	// �������
}



// ����G����ʱ���и�һ���С�壬��Ҫ�����µ�
void KnifeClass::OutputGCode_KnifeOneSmallPanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
	int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
	float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed, int decimal_place)
{

	// ���￪ʼ
	int nDPointNum = newpoint.size();
	int pos2 = (pos+1)%nDPointNum;

	// ���䵽����м�⣬��ֹ����Խ�������
	if (pos >= nDPointNum)
	{
		pos = 0;
		pos2 = 1;

		//AfxMessageBox(_T("�䵶�㳬���������������䵽����Ϊ0��"));
	}


	// �������꣬������ļ�
	float knife_down_pos = 0;
	float first_height = 0;
	PointInfo c_p1,c_p2;
	c_p1 = newpoint.at(pos);
	c_p2 = newpoint.at(pos2);	
	float L, dx, dy, dist1, dist2;

	dx = c_p1.x - c_p2.x;
	dy = c_p1.y - c_p2.y;
	L = sqrt(dx*dx + dy*dy);
	dist1 = gcodeparam.distance1;
	dist2 = gcodeparam.distance2;

	float x_dis0,y_dis0,x_dis1,y_dis1,x_end,y_end;

	if ((c_p1.sign == 1 || c_p1.sign == 3) && (c_p2.sign == 2 || c_p2.sign == 3))
	{
		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}
	else if (L < dist1 + dist2)	
	{
		// ��ֱ��ֱ���µ�
		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}
	else
	{
#if 1

		float m_disx1 = ((c_p1.x - c_p2.x)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*gcodeparam.distance1;
		float m_disx2 = ((c_p1.x - c_p2.x)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*(gcodeparam.distance2 + m_kerfwidth/2.0);
		float m_disy1 = ((c_p1.y - c_p2.y)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*gcodeparam.distance1;
		float m_disy2 = ((c_p1.y - c_p2.y)/(sqrt((c_p2.x - c_p1.x)*(c_p2.x - c_p1.x)+(c_p2.y - c_p1.y)*(c_p2.y - c_p1.y))))*(gcodeparam.distance2 + m_kerfwidth/2.0);

		/*#else*/

		float m_disx1_1 = (dx/L) * dist1;
		float m_disx2_1 = (dx/L) * (dist2 + m_kerfwidth/2.0);
		float m_disy1_1 = (dy/L) * dist1;
		float m_disy2_1 = (dy/L) * (dist2 + m_kerfwidth/2.0);

#endif



		x_dis0 = src_x + newpoint.at((pos+1)%newpoint.size()).x + m_disx2;
		y_dis0 = src_y + newpoint.at((pos+1)%newpoint.size()).y + m_disy2;
		x_dis1 = src_x + newpoint.at((pos+1)%newpoint.size()).x + m_disx1 + m_disx2;
		y_dis1 = src_y + newpoint.at((pos+1)%newpoint.size()).y + m_disy1 + m_disy2;
		x_end = src_x + newpoint.at((pos+1)%newpoint.size()).x;
		y_end = src_y + newpoint.at((pos+1)%newpoint.size()).y;
	}


	char chheight1[12],cheight2[12],chx_0[12],chx_1[12],chy_0[12],chy_1[12],chheight3[12],chx_2[12],chy_2[12];

	// ��ȫ�߶�
//	sprintf_s(chheight1,"%0.4f",gcodeparam.upheight);
	PrintNumber2Array(chheight1, 12, decimal_place, gcodeparam.upheight);


	if (gcodeparam.downheight > 1000)
	{
		float up_knife = -1.0 * k_l * knift_lenght;
		//sprintf_s(chheight3,"%0.4f",up_knife);
		PrintNumber2Array(chheight3, 12, decimal_place, up_knife);
	}
	else
	{	// �ϱ���߶�
		float up_knife = _thickness - k_l*knift_lenght;
		//sprintf_s(chheight3,"%0.4f",up_knife);
		PrintNumber2Array(chheight3, 12, decimal_place, up_knife);

		if (gcodeparam.downheight > 0.5)
		{
			//sprintf_s(chheight3,"%0.4f",up_knife - gcodeparam.downheight);
			PrintNumber2Array(chheight3, 12, decimal_place, up_knife - gcodeparam.downheight);
		}
	}

	if (gcodeparam.downheight > 1000)
	{
		if (info.at(0) == '!' && info.at(1) == '!')
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness - m_leftthin) ? (_thickness - m_leftthin) : knift_lenght * (k_l+1);
			knife_down_pos = (down_knife+0.1)*(-1);
		}
		else
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness) ? (_thickness) : knift_lenght * (k_l+1);
			knife_down_pos = (down_knife+0.1)*(-1) ;
		}

	}
	else
	{
		if (info.at(0) == '!' && info.at(1) == '!')
		{
			float down_knife = knift_lenght * (k_l+1) > (_thickness - m_leftthin) ? ((-1.0) * (gcodeparam.downheight) + m_leftthin) : (_thickness - m_leftthin) - knift_lenght * (k_l+1);
			knife_down_pos = down_knife;
		}
		else
		{
			float down_knife = knift_lenght * (k_l+1) > _thickness ? (-1.0) * (gcodeparam.downheight) : _thickness - knift_lenght * (k_l+1);
			knife_down_pos = down_knife;
		}

	}

	// ���õ�һ���е��µ��߶�
	first_height = knife_down_pos + m_SmallPanelReserveHeight;
	//sprintf_s(cheight2,"%0.4f", first_height);
	PrintNumber2Array(cheight2, 12, decimal_place, first_height);

	// �䵶 ����ָ��λ��
	//sprintf_s(chy_1,"%0.4f",x_dis1);		// ��㣬׼���䵶 |
	//sprintf_s(chx_1,"%0.4f",y_dis1);
	//sprintf_s(chx_0,"%0.4f",y_dis0);		// �䵶�������		
	//sprintf_s(chy_0,"%0.4f",x_dis0);
	//sprintf_s(chx_2,"%0.4f",x_end);		// �䵶��͵����ƽ��һ�ε�������	-
	//sprintf_s(chy_2,"%0.4f",y_end);


	PrintNumber2Array(chy_1, 12, decimal_place, x_dis1);		// ��㣬׼���䵶 |
	PrintNumber2Array(chx_1, 12, decimal_place, y_dis1);
	PrintNumber2Array(chx_0, 12, decimal_place, y_dis0);		// �䵶�������		
	PrintNumber2Array(chy_0, 12, decimal_place, x_dis0);
	PrintNumber2Array(chx_2, 12, decimal_place, x_end);		// �䵶��͵����ƽ��һ�ε�������	-
	PrintNumber2Array(chy_2, 12, decimal_place, y_end);

	if (coortype == 0)
	{
		wfile<<"G00"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight1;	// ��ȫ�߶�
		if (knife_overspend > 0.01)
			wfile<<" F "<<knife_overspend;
		wfile<<endl;
		wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<chheight3;	// ���ϱ���߶�
		if (knife_facespeed > 0.01)
			wfile<<" F "<<knife_facespeed;
		wfile<<endl;
		wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;	// ������͸߶ȣ�ֻ��һ�ξ���-0.1���д�
		if (knife_spindlespeed > 0.01)
			wfile<<" F "<<knife_spindlespeed;
		wfile<<endl;
	}
	else
	{
		wfile<<"G00"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<chheight1<<" F "<<knife_overspend<<endl;
		wfile<<"G01"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<chheight3<<" F "<<knife_facespeed<<endl;
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<knife_spindlespeed<<endl;
	}





	// ʣ������е�
	for (int n = 0;n < newpoint.size();n++)
	{
		int key,prekey;

		key = (pos + 1 + n)%newpoint.size();
		prekey = (pos + n)%newpoint.size();

		PointInfo newpos = newpoint.at(key);
		float pos_x = newpos.x + src_x;
		float pos_y = newpos.y + src_y;
		float pos_r = newpos.r;
		int pos_sign = newpos.sign;
		int pos_dir = newpos.dir;
		int pos_cut = newpos.cut;

		PointInfo prenewpos = newpoint.at(prekey);
		float prepos_x = prenewpos.x + src_x;
		float prepos_y = prenewpos.y + src_y;
		float prepos_r = prenewpos.r;
		int prepos_sign = prenewpos.sign;
		int prepos_dir = prenewpos.dir;
		int prepos_cut = prenewpos.cut;				

		char chx[12],chy[12],chr[12],chpr[12];
		//sprintf_s(chx,"%0.4f",pos_y);
		//sprintf_s(chy,"%0.4f",pos_x);
		//sprintf_s(chr,"%0.4f",pos_r);
		//sprintf_s(chpr,"%0.4f",prepos_r);


		PrintNumber2Array(chx,	 12, decimal_place, pos_y);
		PrintNumber2Array(chy,	 12, decimal_place, pos_x);
		PrintNumber2Array(chr,	 12, decimal_place, pos_r);
		PrintNumber2Array(chpr,	 12, decimal_place, prepos_r);

		if (pos_sign == 0 || pos_sign == 1)
		{
			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (knife_feedrate > 0.01)
					wfile<<" F "<<knife_feedrate;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<" F "<<knife_feedrate<<endl;
			}
		}
		else if (pos_sign == 2)	// ��ǰ��ΪԲ���յ�
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else if (pos_sign == 3)	// Բ���յ㣬��Ҫ��Բ��
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else
		{
			if (key != 1)
			{
				if (pos_dir == 1)
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
				else
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
			}

			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (knife_feedrate > 0.01)
					wfile<<"F "<<knife_feedrate;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<"F "<<knife_feedrate<<endl;
			}
		}
	}


	// ���һ�ߣ���̧��
	// �����һ���㵽�ڶ�������Բ����Բ�������ڶ�����
	if (c_p1.sign == 1 || c_p1.sign == 3)
	{
		float beginpos_x = c_p2.x + src_x;
		float beginpos_y = c_p2.y + src_y;
		float beginpos_r = c_p2.r;
		int beginpos_sign = c_p2.sign;
		int beginpos_dir = c_p2.dir;
		int beginpos_cut = c_p2.cut;

		char beginchx[12],beginchy[12],beginchr[12];
		//sprintf_s(beginchx,"%0.4f",beginpos_y);
		//sprintf_s(beginchy,"%0.4f",beginpos_x); 
		//sprintf_s(beginchr,"%0.4f",beginpos_r);

		PrintNumber2Array(beginchx, 12, decimal_place, beginpos_y);
		PrintNumber2Array(beginchy, 12, decimal_place, beginpos_x); 
		PrintNumber2Array(beginchr, 12, decimal_place, beginpos_r);

		if (beginpos_dir == 1)
		{
			if (coortype == 0)
			{
				wfile<<"G90 G03"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G03"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}
		}
		else
		{
			if (coortype == 0)
			{
				wfile<<"G90 G02"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G02"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}

		}
	}

	// ���һ�ߣ���̧��
	if (coortype == 0)
	{
		wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<cheight2<<endl;
		wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;
		if (knife_endspeed > 0.01)
			wfile<<" F "<<knife_endspeed;
		wfile<<endl;
		if (KnifeOffset == true)
		{
			wfile<<"G01"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<cheight2;

			if (knife_endspeed > 0.01)
				wfile<<" F "<<knife_endspeed;

			wfile<<endl;
		}
	}
	else
	{
		wfile<<"G01"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<cheight2<<endl;
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<knife_endspeed<<endl;
	}

	/************************************************************************/
	/*             ��Χ����һȦ����ȥ���ٿ�һȦ                             */
	/************************************************************************/


	//sprintf_s(cheight2,"%0.4f", knife_down_pos);
	PrintNumber2Array(cheight2, 12, decimal_place, knife_down_pos);

	// С��������
	if (coortype == 0)
	{
		if (KnifeOffset == true)
		{
			wfile<<"G01"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<cheight2;
			if (m_SmallPanelSecondSpeed > 0.01)
				wfile<<" F "<<m_SmallPanelSecondSpeed;
			wfile<<endl;
		}
		else
		{
			wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;
			if (m_SmallPanelSecondSpeed > 0.01)
				wfile<<" F "<<m_SmallPanelSecondSpeed;
			wfile<<endl;
		}
	}
	else
	{
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<m_SmallPanelSecondSpeed<<endl;
	}



	// ��һ��ʣ������е�
	for (int n = 0;n < newpoint.size();n++)
	{
		int key,prekey;

		key = (pos + 1 + n)%newpoint.size();
		prekey = (pos + n)%newpoint.size();

		PointInfo newpos = newpoint.at(key);
		float pos_x = newpos.x + src_x;
		float pos_y = newpos.y + src_y;
		float pos_r = newpos.r;
		int pos_sign = newpos.sign;
		int pos_dir = newpos.dir;
		int pos_cut = newpos.cut;

		PointInfo prenewpos = newpoint.at(prekey);
		float prepos_x = prenewpos.x + src_x;
		float prepos_y = prenewpos.y + src_y;
		float prepos_r = prenewpos.r;
		int prepos_sign = prenewpos.sign;
		int prepos_dir = prenewpos.dir;
		int prepos_cut = prenewpos.cut;				

		char chx[12],chy[12],chr[12],chpr[12];
		//sprintf_s(chx,"%0.4f",pos_y);
		//sprintf_s(chy,"%0.4f",pos_x);
		//sprintf_s(chr,"%0.4f",pos_r);
		//sprintf_s(chpr,"%0.4f",prepos_r);

		PrintNumber2Array(chx,	 12, decimal_place, pos_y);
		PrintNumber2Array(chy,	 12, decimal_place, pos_x);
		PrintNumber2Array(chr,	 12, decimal_place, pos_r);
		PrintNumber2Array(chpr,	 12, decimal_place, prepos_r);

		if (pos_sign == 0 || pos_sign == 1)
		{
			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (m_SmallPanelSecondSpeed > 0.01)
					wfile<<" F "<<m_SmallPanelSecondSpeed;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<" F "<< m_SmallPanelSecondSpeed <<endl;
			}
		}
		else if (pos_sign == 2)	// ��ǰ��ΪԲ���յ�
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else if (pos_sign == 3)	// Բ���յ㣬��Ҫ��Բ��
		{
			if (n > 0)			// ��һ����ΪԲ���յ㣬����Բ��
			{
				if (pos_dir == 1)	// ��ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}

				}
				else				// ˳ʱ��
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chr<<endl;
					}
				}
			}
		}
		else
		{
			if (key != 1)
			{
				if (pos_dir == 1)
				{
					if (coortype == 0)
					{
						wfile<<"G90 G03"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G03"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
				else
				{
					if (coortype == 0)
					{
						wfile<<"G90 G02"<<" X "<<chx<<" Y "<<chy<<" R "<<chpr<<endl;
					}
					else
					{
						wfile<<"G90 G02"<<" X "<<chy<<" Y "<<chx<<" R "<<chpr<<endl;
					}
				}
			}
			if (coortype == 0)
			{
				wfile<<"G01"<<" X "<<chx<<" Y "<<chy<<" Z "<<cheight2;
				if (m_SmallPanelSecondSpeed > 0.01)
					wfile<<"F "<< m_SmallPanelSecondSpeed;
				wfile<<endl;
			}
			else
			{
				wfile<<"G01"<<" X "<<chy<<" Y "<<chx<<" Z "<<cheight2<<"F "<<m_SmallPanelSecondSpeed<<endl;
			}
		}
	}


	// ���һ�ߣ���̧��
	// �����һ���㵽�ڶ�������Բ����Բ�������ڶ�����
	if (c_p1.sign == 1 || c_p1.sign == 3)
	{
		float beginpos_x = c_p2.x + src_x;
		float beginpos_y = c_p2.y + src_y;
		float beginpos_r = c_p2.r;
		int beginpos_sign = c_p2.sign;
		int beginpos_dir = c_p2.dir;
		int beginpos_cut = c_p2.cut;

		char beginchx[12],beginchy[12],beginchr[12];
		//sprintf_s(beginchx,"%0.4f",beginpos_y);
		//sprintf_s(beginchy,"%0.4f",beginpos_x); 
		//sprintf_s(beginchr,"%0.4f",beginpos_r);

		PrintNumber2Array(beginchx, 12, decimal_place,beginpos_y);
		PrintNumber2Array(beginchy, 12, decimal_place,beginpos_x); 
		PrintNumber2Array(beginchr, 12, decimal_place,beginpos_r);


		if (beginpos_dir == 1)
		{
			if (coortype == 0)
			{
				wfile<<"G90 G03"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G03"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}
		}
		else
		{
			if (coortype == 0)
			{
				wfile<<"G90 G02"<<" X "<<beginchx<<" Y "<<beginchy<<" R "<<beginchr<<endl;
			}
			else
			{
				wfile<<"G90 G02"<<" X "<<beginchy<<" Y "<<beginchx<<" R "<<beginchr<<endl;
			}

		}
	}

	// �ص�ԭʼ�߶�
	// ��Χ����һ�Σ������жϸ߶�
	// ���һ�ߣ���̧��
	if (coortype == 0)
	{
		wfile<<"G01"<<" X "<<chx_1<<" Y "<<chy_1<<" Z "<<cheight2<<endl;	// �ص��ʼ�µ�λ��
		wfile<<"G01"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<cheight2;		// �ص����굶��λ��
		if (m_SmallPanelSecondSpeed > 0.01)
			wfile<<" F "<<m_SmallPanelSecondSpeed;
		wfile<<endl;
		if (KnifeOffset == true)
		{
			wfile<<"G01"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<cheight2;
			if (m_SmallPanelSecondSpeed > 0.01)
				wfile<<" F "<<m_SmallPanelSecondSpeed;
			wfile<<endl;
			wfile<<"G00"<<" X "<<chy_2<<" Y "<<chx_2<<" Z "<<chheight1;
			if (m_SmallPanelSecondSpeed > 0.01)
				wfile<<" F "<<m_SmallPanelSecondSpeed;
			wfile<<endl;
		}
		else
		{
			wfile<<"G00"<<" X "<<chx_0<<" Y "<<chy_0<<" Z "<<chheight1;
			if (m_SmallPanelSecondSpeed > 0.01)
				wfile<<" F "<<m_SmallPanelSecondSpeed;
			wfile<<endl;
		}
	}
	else
	{
		wfile<<"G01"<<" X "<<chy_1<<" Y "<<chx_1<<" Z "<<cheight2<<endl;
		wfile<<"G01"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<cheight2<<" F "<<m_SmallPanelSecondSpeed<<endl;
		wfile<<"G00"<<" X "<<chy_0<<" Y "<<chx_0<<" Z "<<chheight1<<" F "<<m_SmallPanelSecondSpeed<<endl;
	}

	// �������
}




// ��ȡ���·��
void KnifeClass::GetShortestPath(std::vector<pos> & poses,pos & beginpos,std::vector<pos> & newposes)
{
	if (poses.size() == 0)
	{
		return;
	}
	else if (poses.size() == 1)
	{
		newposes.push_back(poses.at(0));
		poses.clear();
		return;
	}
	else
	{
		float min_path = 10000000.0;
		int min_num = 0;
		for (int i = 0;i < poses.size();i++)
		{
			float now_path = sqrt((poses.at(i).x - beginpos.x)*(poses.at(i).x - beginpos.x) + (poses.at(i).y - beginpos.y)*(poses.at(i).y - beginpos.y));
			if (now_path < min_path)
			{
				min_path = now_path;
				min_num = i;
			}
		}
		newposes.push_back(poses.at(min_num));
		pos newbegin;
		newbegin.x = poses.at(min_num).x;
		newbegin.y = poses.at(min_num).y;
		newbegin._kn = poses.at(min_num)._kn;
		poses.erase(poses.begin()+min_num);
		GetShortestPath(poses,newbegin,newposes);
	}
}


// ��ȡ�ı���·��
void KnifeClass::GetChangePath(std::vector<pos> poses,std::vector<pos> & newposes)
{
	float different_dis = 0.01;
	int different_num = 0;
	float min_num = 0;
	for (int i = 1;i < poses.size();i++)
	{
		float now_dis = sqrt((poses.at(i).x - poses.at(i-1).x)*(poses.at(i).x - poses.at(i-1).x) + (poses.at(i).y - poses.at(i-1).y)*(poses.at(i).y - poses.at(i-1).y));
		float min_dis = 10000000;
		for (int j = 0;j < poses.size();j++)
		{
			if (i == j)
			{
				continue;
			}
			else
			{
				float one_dis = sqrt((poses.at(i).x - poses.at(j).x)*(poses.at(i).x - poses.at(j).x) + (poses.at(i).y - poses.at(j).y)*(poses.at(i).y - poses.at(j).y));
				if (one_dis < min_dis)
				{
					min_num = j;
					min_dis = one_dis;
				}
			}
		}
		if (now_dis - min_dis > different_dis)
		{
			different_dis = now_dis - min_dis;
			different_num = i;
		}
	}
	if (different_num == 0)
	{
		newposes.clear();
		for (int i = 0;i < poses.size();i++)
		{
			newposes.push_back(poses.at(i));
		}
		return;
	}
	else
	{
		std::vector<pos> tmpposes1,tmpposes2;
		for (int i = 0;i < min_num;i++)
		{
			if (i == different_num)
			{
				continue;
			}
			tmpposes1.push_back(poses.at(i));
			tmpposes2.push_back(poses.at(i));
		}
		tmpposes1.push_back(poses.at(different_num));
		tmpposes1.push_back(poses.at(min_num));
		tmpposes2.push_back(poses.at(min_num));
		tmpposes2.push_back(poses.at(different_num));
		for (int i = min_num+1;i < poses.size();i++)
		{
			if (i == different_num)
			{
				continue;
			}
			tmpposes1.push_back(poses.at(i));
			tmpposes2.push_back(poses.at(i));
		}

		float all_disold = sqrt((poses.at(0).x)*(poses.at(0).x) + (poses.at(0).y)*(poses.at(0).y));
		float all_disnew1 = sqrt((tmpposes1.at(0).x)*(tmpposes1.at(0).x) + (tmpposes1.at(0).y)*(tmpposes1.at(0).y));
		float all_disnew2 = sqrt((tmpposes2.at(0).x)*(tmpposes2.at(0).x) + (tmpposes2.at(0).y)*(tmpposes2.at(0).y));
		for (int i = 1;i < poses.size();i++)
		{
			all_disold = all_disold + sqrt((poses.at(i).x - poses.at(i-1).x)*(poses.at(i).x - poses.at(i-1).x) + (poses.at(i).y - poses.at(i-1).y)*(poses.at(i).y - poses.at(i-1).y));
		}
		for (int i = 1;i < tmpposes1.size();i++)
		{
			all_disnew1 = all_disnew1 + sqrt((tmpposes1.at(i).x - tmpposes1.at(i-1).x)*(tmpposes1.at(i).x - tmpposes1.at(i-1).x) + (tmpposes1.at(i).y - tmpposes1.at(i-1).y)*(tmpposes1.at(i).y - tmpposes1.at(i-1).y));
		}
		for (int i = 1;i < tmpposes2.size();i++)
		{
			all_disnew2 = all_disnew2 + sqrt((tmpposes2.at(i).x - tmpposes2.at(i-1).x)*(tmpposes2.at(i).x - tmpposes2.at(i-1).x) + (tmpposes2.at(i).y - tmpposes2.at(i-1).y)*(tmpposes2.at(i).y - tmpposes2.at(i-1).y));
		}

		if (all_disnew1 < all_disold)
		{
			if (all_disnew1 < all_disnew2)
			{
				poses.clear();
				for (int i = 0;i < tmpposes1.size();i++)
				{
					poses.push_back(tmpposes1.at(i));
				}
				GetChangePath(poses,newposes);
			}
			else
			{
				poses.clear();
				for (int i = 0;i < tmpposes2.size();i++)
				{
					poses.push_back(tmpposes2.at(i));
				}
				GetChangePath(poses,newposes);
			}
		}
		else if (all_disnew2 < all_disold)
		{
			if (all_disnew2 < all_disnew1)
			{
				poses.clear();
				for (int i = 0;i < tmpposes2.size();i++)
				{
					poses.push_back(tmpposes2.at(i));
				}
				GetChangePath(poses,newposes);
			}
			else
			{
				poses.clear();
				for (int i = 0;i < tmpposes1.size();i++)
				{
					poses.push_back(tmpposes1.at(i));
				}
				GetChangePath(poses,newposes);
			}
		}
		else
		{
			newposes.clear();
			for (int i = 0;i < poses.size();i++)
			{
				newposes.push_back(poses.at(i));
			}
			return;
		}

	}
}

std::string KnifeClass::ChangeGroup(std::vector<int> _kn,std::string _changestr)
{
	std::string _cstr1 = "";
	std::string _cstr2 = "";
	std::string _cstr3 = "";
	std::string _cstr4 = "";
	std::string _cstr5 = "";
	std::string _cstr6 = "";
	std::string _cstr7 = "";
	int key = 0;
	bool getstr = false;
	bool calstr = false;
	std::string str_cal = "";
	for (int i = 0;i < _changestr.size();i++)
	{
		if (_changestr.at(i) == '{')
		{
			getstr = true;
			continue;
		}
		else if (_changestr.at(i) == '}')
		{
			getstr = false;
			key++;
			continue;
		}
		else if (_changestr.at(i) == '<')
		{
			calstr = true;
			continue;
		}
		else if (_changestr.at(i) == '>')
		{
			break;
		}
		else if (calstr == true)
		{
			str_cal.push_back(_changestr.at(i));
		}
		else
		{
			switch(key)
			{
			case 0:
				_cstr1 = _cstr1 + _changestr.at(i);
				break;
			case 1:
				_cstr2 = _cstr2 + _changestr.at(i);
				break;
			case 2:
				_cstr3 = _cstr3 + _changestr.at(i);
				break;
			case 3:
				_cstr4 = _cstr4 + _changestr.at(i);
				break;
			case 4:
				_cstr5 = _cstr5 + _changestr.at(i);
				break;
			case 5:
				_cstr6 = _cstr6 + _changestr.at(i);
				break;
			case 6:
				_cstr7 = _cstr7 + _changestr.at(i);
				break;
			default:
				break;
			}
		}
	}
	std::vector<std::string> _cstr;
	_cstr.push_back(_cstr1);
	_cstr.push_back(_cstr2);
	_cstr.push_back(_cstr3);
	_cstr.push_back(_cstr4);
	_cstr.push_back(_cstr5);
	_cstr.push_back(_cstr6);
	_cstr.push_back(_cstr7);

	std::string return_str_result = "";
	if (str_cal == "+")
	{
		return_str_result = "0";
	}
	if (str_cal == "*")
	{
		return_str_result = "1";
	}

	for (int i = 0;i < _kn.size();i++)
	{
		CString str_x;
		str_x.Format("%d",_kn.at(i));
		std::string s_x = str_x.GetBuffer();
		std::string _changestr_one = _cstr.at(i+1);
		if (i == 0)
		{
			_changestr_one = replace_allstr(_changestr_one,"y",s_x);
		}
		else
		{
			_changestr_one = replace_allstr(_changestr_one,"x",s_x);
		}
		bool begin_calculate = false;
		std::string str_calculate = "";
		std::string str_result = "";
		for (int i = 0;i < _changestr_one.size();i++)
		{
			if (_changestr_one.at(i) == '<')
			{
				begin_calculate = true;
				continue;
			}
			else if (_changestr_one.at(i) == '>')
			{
				begin_calculate = false;

				// ��д��ʽ���㺯��

#if 0	
				hg3d::Construction cst;
				int i_result = (int)(cst.calcFormula(str_calculate));
#else
				int i_result = (int)(calcFormula(str_calculate));
#endif

				CString s_re;
				s_re.Format("%d",i_result);
				str_result = str_result + s_re.GetBuffer();

				str_calculate = "";

				continue;
			}
			else
			{
				if (begin_calculate == false)
				{
					str_result.push_back(_changestr_one.at(i));
				}
				else
				{
					str_calculate.push_back(_changestr_one.at(i));
				}
			}
		}
		return_str_result = return_str_result + str_result;
	}

	if(str_cal != "")
	{
		// ��д��ʽ���㺯��

#if 0	
		hg3d::Construction cst;
		int i_result = (int)(cst.calcFormula(return_str_result));
#else
		int i_result = (int)(calcFormula(return_str_result));
#endif



		CString s_re;
		s_re.Format("%d",i_result);
		return_str_result = s_re.GetBuffer();
	}
	return_str_result = _cstr.at(0) +  return_str_result;
	return return_str_result;
}

std::string KnifeClass::ChangeGroup(int _x,int _y,std::string _changestr)
{
	CString str_x,str_y;
	str_x.Format("%d",_x);
	str_y.Format("%d",_y);
	std::string s_x = str_x.GetBuffer();
	std::string s_y = str_y.GetBuffer();

	_changestr = replace_allstr(_changestr,"x",s_x);
	_changestr = replace_allstr(_changestr,"y",s_y);

	bool begin_calculate = false;
	std::string str_calculate = "";
	std::string str_result = "";
	for (int i = 0;i < _changestr.size();i++)
	{
		if (_changestr.at(i) == '<')
		{
			begin_calculate = true;
			continue;
		}
		else if (_changestr.at(i) == '>')
		{
			begin_calculate = false;



			// ��д��ʽ���㺯��

#if 0	
			hg3d::Construction cst;
			int i_result = (int)(cst.calcFormula(str_calculate));
#else
			int i_result = (int)(calcFormula(str_calculate));
#endif


			CString s_re;
			s_re.Format("%d",i_result);
			str_result = str_result + s_re.GetBuffer();

			str_calculate = "";

			continue;
		}
		else
		{
			if (begin_calculate == false)
			{
				str_result.push_back(_changestr.at(i));
			}
			else
			{
				str_calculate.push_back(_changestr.at(i));
			}
		}
	}
	return str_result;
}


std::string KnifeClass::replace_allstr(std::string str,const std::string old_value,const std::string new_value)
{
	while(true)   
	{                                                                                                                                                              
		std::string::size_type pos(0);     
		if((pos=str.find(old_value))!=string::npos)
			str.replace(pos,old_value.length(),new_value);
		else
			break;
	}
	return str; 
}


vector<gcodeCoor> KnifeClass::rollsetoptimize(vector<gcodeCoor> coor)
{
	vector<gcodeCoor> newcoor;

	vector<Remove_Line> Lines;

	//����ı�Ϊ�����䵶��
	for (int i = 0;i < 20;i++)
	{
		Remove_Line one_line1,one_line2,one_line3,one_line4;

		one_line1.begin_x = m_deburringwidth + i;
		one_line1.begin_y = m_deburringwidth + i;
		one_line1.end_x = m_deburringwidth + i;
		one_line1.end_y = m_panelheight - m_deburringwidth - i;

		one_line2.begin_x = m_deburringwidth + i;
		one_line2.begin_y = m_panelheight - m_deburringwidth - i;
		one_line2.end_x = m_panelwidth - m_deburringwidth - i;
		one_line2.end_y = m_panelheight - m_deburringwidth - i;

		one_line3.begin_x = m_panelwidth - m_deburringwidth - i;
		one_line3.begin_y = m_panelheight - m_deburringwidth - i;
		one_line3.end_x = m_panelwidth - m_deburringwidth - i;
		one_line3.end_y = m_deburringwidth + i;

		one_line4.begin_x = m_panelwidth - m_deburringwidth - i;
		one_line4.begin_y = m_deburringwidth + i;
		one_line4.end_x = m_deburringwidth + i;
		one_line4.end_y = m_deburringwidth + i;

		Lines.push_back(one_line1);
		Lines.push_back(one_line2);
		Lines.push_back(one_line3);
		Lines.push_back(one_line4);
	}

	//�洢��һ���䵶���λ��
	float pre_x = -1;
	float pre_y = -1;

	for (int i = 0;i < coor.size();i++)
	{
		float x0 = coor.at(i)._x;
		float y0 = coor.at(i)._y;
		float coor_width = coor.at(i)._width;
		float coor_height = coor.at(i)._height;

		int CurPanelID = coor.at(i).m_NumInPanel;
		std::string info = coor.at(i)._info;

		vector<PointInfo> points;

		// �����㰴���ID˳�򼴿ɻ�ȡ
		//points = m_allpoint.at(i).point_info;

		for (int p = 0;p < m_allpoint.size();p++)
		{
			int panel_id = m_allpoint.at(p).m_NumInPanel;

			std::string panel_info = m_allpoint.at(p).panel_info;


			//if (strcmp(info.c_str(),panel_info.c_str()) == 0)
			if (panel_id == CurPanelID)
			{
				points = m_allpoint.at(p).point_info;
				break;
			}
		}

		//����û��points�İ���������������,�����һ��points
// 		if (points.size() == 0)
// 		{
// 			PointInfo p1,p2,p3,p4;
// 			p1.x = 0;p1.y = 0;p1.r = 0;p1.sign = 0;p1.dir = 0;p1.cut = 0;
// 			p2.x = 0;p2.y = coor.at(i)._height;p2.r = 0;p2.sign = 0;p2.dir = 0;p2.cut = 0;
// 			p3.x = coor.at(i)._width;p3.y = coor.at(i)._height;p3.r = 0;p3.sign = 0;p3.dir = 0;p3.cut = 0;
// 			p4.x = coor.at(i)._width;p4.y = 0;p4.r = 0;p4.sign = 0;p4.dir = 0;p4.cut = 0;
// 			points.push_back(p1);
// 			points.push_back(p2);
// 			points.push_back(p3);
// 			points.push_back(p4);
// 		}

		// 2017-4-20 yuanzb ����NCǰ�Ѿ��������㣬�ײ۽����˴�������NC���ٶ��⴦��
// 		else if (points.size() > 0 && coor.at(i)._dir == 2)
// 		{
// 			vector<PointInfo> otherpoints;
// 			for (int _p = 0;_p < points.size();_p++)
// 			{
// 				PointInfo one_p;
// 				one_p.cut = points.at(_p).cut;
// 				one_p.dir = points.at(_p).dir;
// 				one_p.r = points.at(_p).r;
// 				one_p.sign = points.at(_p).sign;
// 				one_p.x = coor.at(i)._width - points.at(_p).y;
// 				one_p.y = points.at(_p).x;
// 				otherpoints.push_back(one_p);
// 			}
// 			points.clear();
// 			for (int _p = 0;_p < otherpoints.size();_p++)
// 			{
// 				points.push_back(otherpoints.at(_p));
// 			}
// 		}

		int pos = 0;
		//�������䵶��
		float _distance = m_gcodeparam.distance1+m_gcodeparam.distance2+m_deburringwidth/2.0 + 30;

		if (m_optimizeparam.minheight < coor_height && m_optimizeparam.minwidth < coor_width && m_optimizeparam.minsize < coor_height * coor_width)
			//if (500 < coor_height && 1000 < coor_width && _optimizeparam.minsize < coor_height * coor_width)
		{
			vector<int> poses;//�洢�����䵶�ĵ㼯
			//��ȡ�䵶�㼯
			for (int k = 0;k < points.size();k++)
			{
				PointInfo p1,p2;
				int p_one,p_two;
				p_one = k%points.size();
				p_two = (k+1)%points.size();
				p1 = points.at(p_one);
				p2 = points.at(p_two);
				if(p1.x != p2.x && p1.y != p2.y)
					continue;
				float pdistance = sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
				if (p2.sign != 2 && p2.sign != 3 && pdistance > _distance)
				{
					poses.push_back(k);
				}
				else
					continue;

			}
			//�ӵ㼯�л�ȡ���ŵ��䵶��
			float min_dis = 100000;
			if (pre_x < 0 && pre_y < 0)
			{
				pos = 0;
			}
			else
			{
				for (int n = 0;n < poses.size();n++)
				{
					PointInfo np;
					np = points.at((poses.at(n)+1)%points.size());

					float p_x = np.x + x0;
					float p_y = np.y + y0;

					float np_dis = sqrt((p_x - pre_x)*(p_x - pre_x) + (p_y - pre_y)*(p_y - pre_y));
					if (np_dis < min_dis)
					{
						pos = poses.at(n);
						min_dis = np_dis;
					}
				}
			}

		}
		else
		{
			vector<int> poses;//�洢�����䵶�ĵ㼯

			for (int k = 0;k < points.size();k++)
			{
				PointInfo p1,p2,p3;
				p1 = points.at(k%points.size());
				p2 = points.at((k+1)%points.size());

				bool no_intersection = true;
				int Lines_num = Lines.size();
				int for_num = 0;
				for (int l = 0;l < Lines_num;l++)
				{
					float begin_x,begin_y,end_x,end_y;
					begin_x = Lines.at(l).begin_x;
					begin_y = Lines.at(l).begin_y;
					end_x = Lines.at(l).end_x;
					end_y = Lines.at(l).end_y;

					for_num++;
					if (Line_intersection(begin_x,begin_y,end_x,end_y,p1.x + x0,p1.y + y0,p2.x + x0,p2.y + y0) == false)
					{
						no_intersection = false;
						break;
					}
					else
					{
						continue;
					}
				}

				float pdistance = sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
				if (no_intersection == true)
				{
					if (p2.sign != 2 && p2.sign != 3 && pdistance > _distance)
					{
						poses.push_back(k);
					}
					else
						continue;
				}
				else
					continue;

			}

			//��������ŵ��䵶��
			float max_dis = 0;

			for (int i = 0;i < poses.size();i++)
			{
				PointInfo p_1,p_2;
				p_1 = points.at(poses.at(i)%points.size());
				p_2 = points.at((poses.at(i)+1)%points.size());
				float p_dis = sqrt((p_1.x-p_2.x)*(p_1.x-p_2.x) + (p_1.y-p_2.y)*(p_1.y-p_2.y));
				if (p_dis > max_dis && (p_dis - max_dis < -0.01 || p_dis - max_dis > 0.01)) //�����еľ�������Ҳ�����������ʱ
				{
					max_dis = p_dis;
					pos = poses.at(i);
				}
				if (p_dis - max_dis > -0.01 && p_dis - max_dis < 0.01)//����Ӧ�ߵľ������ʱ��ѡ���м����ı���Ϊ�䵶��
				{

					PointInfo pos_dis1,pos_dis2;
					pos_dis1 = points.at(pos%points.size());
					pos_dis2 = points.at((pos+1)%points.size());

					float get_maxdis = 0;
					float get_nowdis = 0;

					if (p_1.x - p_2.x > -0.01 && p_1.x - p_2.x < 0.01)
					{
						float get_nowdis1 = p_1.x + x0;
						float get_nowdis2 = m_panelwidth - (p_1.x + x0);
						get_nowdis = get_nowdis1 < get_nowdis2 ? get_nowdis1 : get_nowdis2;
					}
					if (p_1.y - p_2.y > -0.01 && p_1.y - p_2.y < 0.01)
					{
						float get_nowdis1 = p_1.y + y0;
						float get_nowdis2 = m_panelheight - (p_1.y + y0);
						get_nowdis = get_nowdis1 < get_nowdis2 ? get_nowdis1 : get_nowdis2;
					}

					if (pos_dis1.x - pos_dis2.x > -0.01 && pos_dis1.x - pos_dis2.x < 0.01)
					{
						float get_maxdis1 = pos_dis1.x + x0;
						float get_maxdis2 = m_panelwidth - (pos_dis1.x + x0);
						get_maxdis = get_maxdis1 < get_maxdis2 ? get_maxdis1 : get_maxdis2;
					}
					if (pos_dis1.y - pos_dis2.y > -0.01 && pos_dis1.y - pos_dis2.y < 0.01)
					{
						float get_maxdis1 = pos_dis1.y + y0;
						float get_maxdis2 = m_panelheight - (pos_dis1.y + y0);
						get_maxdis = get_maxdis1 < get_maxdis2 ? get_maxdis1 : get_maxdis2;
					}

					if (get_nowdis > get_maxdis)
					{
						pos = poses.at(i);
					}

				}
			}
		}

		//�ж��䵶���Ƿ���ϱ�׼
		if (pos == 0)
		{
			if(points.at(0).sign == 2 || points.at(0).sign == 3)
			{
				pos = -1;
			}
		}

		gcodeCoor gc;
		gc.pos = pos;
		gc._height = coor.at(i)._height;
		gc._info = coor.at(i)._info;
		gc._width = coor.at(i)._width;
		gc._x = coor.at(i)._x;
		gc._y = coor.at(i)._y;
		gc._dir = coor.at(i)._dir;
		newcoor.push_back(gc);

		//��ȡ�õ������
		PointInfo pre_pos = points.at((pos+1)%points.size());
		pre_x = pre_pos.x + coor.at(i)._x;
		pre_y = pre_pos.y + coor.at(i)._y;
		//��Ӳ����䵶����
		Remove_Line line1,line2,line3,line4;

		line1.begin_x = coor.at(i)._x - m_kerfwidth;
		line1.begin_y = coor.at(i)._y - m_kerfwidth;
		line1.end_x = coor.at(i)._x - m_kerfwidth;
		line1.end_y = coor.at(i)._y + coor.at(i)._height + m_kerfwidth;

		line2.begin_x = coor.at(i)._x - m_kerfwidth;
		line2.begin_y =coor.at(i)._y + coor.at(i)._height + m_kerfwidth;
		line2.end_x = coor.at(i)._x + coor.at(i)._width + m_kerfwidth;
		line2.end_y = coor.at(i)._y + coor.at(i)._height + m_kerfwidth;

		line3.begin_x = coor.at(i)._x + coor.at(i)._width + m_kerfwidth;
		line3.begin_y = coor.at(i)._y + coor.at(i)._height + m_kerfwidth;
		line3.end_x = coor.at(i)._x + coor.at(i)._width + m_kerfwidth;
		line3.end_y = coor.at(i)._y - m_kerfwidth;

		line4.begin_x = coor.at(i)._x + coor.at(i)._width + m_kerfwidth;
		line4.begin_y = coor.at(i)._y - m_kerfwidth;
		line4.end_x = coor.at(i)._x - m_kerfwidth;
		line4.end_y = coor.at(i)._y - m_kerfwidth;

		Lines.push_back(line1);
		Lines.push_back(line2);
		Lines.push_back(line3);
		Lines.push_back(line4);
	}
	return newcoor;
}


bool KnifeClass::Line_intersection(float begin_px1,float begin_py1,float end_px1,float end_py1,float begin_px2,float begin_py2,float end_px2,float end_py2)
{
	if (((begin_px1 - begin_px2) > -0.01 && (begin_px1 - begin_px2) < 0.01) && ((begin_px1 - end_px1) > -0.01 && (begin_px1 - end_px1) < 0.01) && ((begin_px2 - end_px2) > -0.01 && (begin_px2 - end_px2) < 0.01))
	{
		float disy1,disy2,disy3,disy4;
		disy1 = (begin_py2 - begin_py1)*(begin_py2 - end_py1);  
		disy2 = (end_py2 - begin_py1)*(end_py2 - end_py1);
		disy3 = (begin_py1 - begin_py2)*(begin_py1 - end_py2);
		disy4 = (end_py1 - begin_py2)*(end_py1 - end_py2);

		if (disy1 < 0.01 || disy2 < 0.01 || disy3 < 0.01 || disy4 < 0.01)
		{
			return false;
		}
		else
			return true;
	}
	if (((begin_py1 - begin_py2) > -0.01 && (begin_py1 - begin_py2) < 0.01) && ((begin_py1 - end_py1) > -0.01 && (begin_py1 - end_py1) < 0.01) && ((begin_py2 - end_py2) > -0.01 && (begin_py2 - end_py2) < 0.01))
	{
		float disx1,disx2,disx3,disx4;
		disx1 = (begin_px2 - begin_px1)*(begin_px2 - end_px1);  
		disx2 = (end_px2 - begin_px1)*(end_px2 - end_px1);
		disx3 = (begin_px1 - begin_px2)*(begin_px1 - end_px2);
		disx4 = (end_px1 - begin_px2)*(end_px1 - end_px2);

		if (disx1 < 0.01 || disx2 < 0.01 || disx3 < 0.01 || disx4 < 0.01)
		{
			return false;
		}
		else
			return true;
	}
	else
		return true;
}


/*---------------------------------------*/
//	����˵����
//		���㹫ʽֵ
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
float KnifeClass::calcFormula(std::string formula)
{

	muParserHandle_t hParser;
	hParser = mupCreate(muBASETYPE_FLOAT); // initialize the parser
	mupSetExpr(hParser, formula.c_str());



	float val = mupEval(hParser);
	mupRelease(hParser); // finalle free the parser ressources
	return val;

}

/*---------------------------------------*/
//	����˵����
//		����ת���������㷨�����ݣ�ƽ��ֱ������ϵ��һ���ޣ�ת����
//		ԭ�е�̻��������ʽ��ƽ��ֱ������ϵ�������ޡ���Ļ����ϵ��
//		
//		newcoor��xyҪ�����½Ǳ�Ϊ���Ͻǣ����εĵ�Ҫ�����ƶ�һ����ȣ���ȡ��
//
//	������
//		Panel* pPanel							--	���ָ��		
//		vector<Component*>& CpnList				--	С������
//		vector<gcodeCoor>& NewCoorList			--	NC��������
//		vector<AllPoint>& AllPointList			--	����������
//		vector<AllSlot>& AllUpperFaceSlotList	--  ���������
//		vector<AllVec>& AllUpperFaceHoleList	--	���������
//		map<int,PointFloat>& LabelPositionMap	--	����λ��map
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void KnifeClass::TransformMachiningInfoToOld_UpperFace(	Panel* pPanel, vector<Component*>& CpnList,vector<gcodeCoor>& NewCoorList ,
													vector<AllPoint>& AllPointList, vector<AllSlot>& AllUpperFaceSlotList, vector<AllVec>& AllUpperFaceHoleList, 
													map<int,PointFloat>&	LabelPositionMap, vector<CutPath>& CutPathList, vector<CutPath>& PreCutPathList)
{
	int nCpnCount = CpnList.size();
	int nCoorCount = NewCoorList.size();
	float panel_width = pPanel->m_OrgWidth;	// ����ת��һ��Ҫ��ԭʼ��������мǣ�

	// ���
	AllPointList.clear();
	AllUpperFaceSlotList.clear();
	AllUpperFaceHoleList.clear();
	LabelPositionMap.clear();
	CutPathList.clear();
	PreCutPathList.clear();

	// С�����ض�Ҫ����������ͬ������϶����ݳ���
	if (nCpnCount != nCoorCount	)
	{
		AfxMessageBox("�������������������ͬ��");
		return;
	}



	for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
	{
		AllPoint OneCpnOutline;
		Component* pCpn = CpnList.at(i_cpn);
		gcodeCoor coor = NewCoorList.at(i_cpn);
		float cpn_width = pCpn->m_RealWidth;

		// �����䵽��
		coor.pos = pCpn->m_nKnifeDownPos;

		// �޸Ķ�������
		coor._y = panel_width - coor._y - cpn_width;
		NewCoorList.at(i_cpn) = coor;

		// ƫ��������
		vector<PointInfo> NewPointList;
		for (int i_pnt = 0; i_pnt < pCpn->m_vOutlinePoint.size(); i_pnt++)
		{
			PointInfo point = pCpn->m_vOutlinePoint.at(i_pnt);

			point.y -= cpn_width;		// �����ƶ�һ�����
			point.y = -point.y;			// ��ȡ��

			NewPointList.push_back(point);
		}

		// ����
		OneCpnOutline.panel_info = pCpn->m_BarCode.GetBuffer(0);
		OneCpnOutline.point_info = NewPointList;

		// ����Ψһ��ʶȡ�������ж�
		OneCpnOutline.m_NumInPanel = pCpn->m_NumberInPanel;

		// ���һ�������������
		AllPointList.push_back(OneCpnOutline);


		// �����

		// ���������
		int nUpperFaceSlotCount = pCpn->m_vUpperFaceSlot.size();
		for(int i_upper_face_slot = 0; i_upper_face_slot < nUpperFaceSlotCount; i_upper_face_slot++)
		{
			if(pCpn->m_vUpperFaceSlot.at(i_upper_face_slot).m_bDisabled)
				continue;

			AllSlot	OneSlot;

			OneSlot = pCpn->m_vUpperFaceSlot.at(i_upper_face_slot);

			// ���������ԭ������½Ǳ�Ϊ���Ͻǣ������� - �۵�y����
			OneSlot._y = cpn_width - OneSlot._y;
			

			// ����Ψһ��ʶȡ�������ж�
			OneSlot.m_NumInPanel = pCpn->m_NumberInPanel;


			AllUpperFaceSlotList.push_back(OneSlot);

		}


		// �����
		int nUpperFaceHoleCount = pCpn->m_vUpperFaceHole.size();
		for(int i_upper_face_hole = 0; i_upper_face_hole < nUpperFaceHoleCount; i_upper_face_hole++)
		{
			if( pCpn->m_vUpperFaceHole.at(i_upper_face_hole).m_bDisabled)
				continue;

			AllVec	OneHole;

			OneHole = pCpn->m_vUpperFaceHole.at(i_upper_face_hole);

			// ���������ԭ������½Ǳ�Ϊ���Ͻǣ������� - �۵�y����
			OneHole._y = cpn_width - OneHole._y;


			// ����Ψһ��ʶȡ�������ж�
			OneHole.m_NumInPanel = pCpn->m_NumberInPanel;


			AllUpperFaceHoleList.push_back(OneHole);

		}

		// ����λ��
		PointFloat label_pos;

		label_pos.x = pCpn->m_fXLabelCenter;
		label_pos.y = cpn_width - pCpn->m_fYLabelCenter;

		LabelPositionMap[i_cpn] = label_pos;


	}

	// �����и�
	int nCutCount = pPanel->m_vCutPath.size();
	for(int i_cut = 0; i_cut < nCutCount; i_cut++)
	{
		CutPath tmp_cut = pPanel->m_vCutPath.at(i_cut);

		tmp_cut._y1 = panel_width - tmp_cut._y1;
		tmp_cut._y2 = panel_width - tmp_cut._y2;
		
		CutPathList.push_back(tmp_cut);
	}

	// Ԥ�и�
	int nPreCutCount = pPanel->m_vPreCutPath.size();
	for(int i_pre_cut = 0; i_pre_cut < nPreCutCount; i_pre_cut++)
	{
		CutPath tmp_cut = pPanel->m_vPreCutPath.at(i_pre_cut);

		tmp_cut._y1 = panel_width - tmp_cut._y1;
		tmp_cut._y2 = panel_width - tmp_cut._y2;

		PreCutPathList.push_back(tmp_cut);
	}

}

/*---------------------------------------*/
//	����˵����
//		��������ת���������㷨�����ݣ�ƽ��ֱ������ϵ��һ���ޣ�ת����
//		ԭ�е�̻��������ʽ��ƽ��ֱ������ϵ�������ޡ���Ļ����ϵ��
//
//	����
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void KnifeClass::TransformMachiningInfoToOld_DownerFace(Panel* pPanel, vector<Component*>& CpnList, vector<gcodeCoor>& NewCoorList,
															vector<AllSlot>& DownerAllSlotList, vector<AllVec>& DownerAllVecHoleList)
{
	int nCpnCount = CpnList.size();
	int nCoorCount = NewCoorList.size();
	float panel_width = pPanel->m_OrgWidth;		// ����ת��һ��Ҫ��ԭʼ��������мǣ�

	// ���
	DownerAllSlotList.clear();
	DownerAllVecHoleList.clear();

	// С�����ض�Ҫ����������ͬ������϶����ݳ���
	if (nCpnCount != nCoorCount	)
	{
		AfxMessageBox("�������������������ͬ��");
		return;
	}


	for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
	{
		AllPoint OneCpnOutline;
		Component* pCpn = CpnList.at(i_cpn);
		gcodeCoor coor = NewCoorList.at(i_cpn);
		float cpn_width = pCpn->m_RealWidth;

		

		// �����䵽��
		coor.pos = pCpn->m_nKnifeDownPos;

		// �޸Ķ�������
		coor._y = panel_width - coor._y - cpn_width;
		NewCoorList.at(i_cpn) = coor;


		// �����

		// ���ڷ����
		int nDownerFaceSlotCount = pCpn->m_vDownerFaceSlot.size();
		for(int i_downer_face_slot = 0; i_downer_face_slot < nDownerFaceSlotCount; i_downer_face_slot++)
		{
			if(pCpn->m_vDownerFaceSlot.at(i_downer_face_slot).m_bDisabled)
				continue;


			AllSlot	OneSlot;

			OneSlot = pCpn->m_vDownerFaceSlot.at(i_downer_face_slot);

			// ���������ԭ������½Ǳ�Ϊ���Ͻǣ������� - �۵�y����
			OneSlot._y = cpn_width - OneSlot._y;

			// ��ɵ�̻�����һ�£�����ǰ����۶����һ����죬����ʱ���ȥ��  �Ժ����ʵ�������������
			OneSlot._width = OneSlot._width + m_kerfwidth;

			// ����Ψһ��ʶȡ�������ж�
			OneSlot.m_NumInPanel = pCpn->m_NumberInPanel;

			DownerAllSlotList.push_back(OneSlot);

		}


		// �����
		int nDownerFaceHoleCount = pCpn->m_vDownerFaceHole.size();
		for(int i_downer_face_hole = 0; i_downer_face_hole < nDownerFaceHoleCount; i_downer_face_hole++)
		{
			if(pCpn->m_vDownerFaceHole.at(i_downer_face_hole).m_bDisabled)
				continue;

			AllVec	OneHole;

			OneHole = pCpn->m_vDownerFaceHole.at(i_downer_face_hole);

			// ���������ԭ������½Ǳ�Ϊ���Ͻǣ������� - �۵�y����
			OneHole._y = cpn_width - OneHole._y;

			// ����Ψһ��ʶȡ�������ж�
			OneHole.m_NumInPanel = pCpn->m_NumberInPanel;


			DownerAllVecHoleList.push_back(OneHole);

		}
	}

}






/*---------------------------------------*/
//	����˵����
//		�����䵽��
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
void KnifeClass::SetKnifeDownPos(Panel* pPanel)
{
	vector<Component*> CpnList;
	vector<gcodeCoor> NewCoorList;
	vector<AllPoint> AllPointList;
	float panel_width = pPanel->m_OrgWidth;		// ����ת��һ��Ҫ��ԭʼ��������мǣ�


	// �������������
	pPanel->GetAllNeededComponent(CpnList);
	sort(CpnList.begin(), CpnList.end(), ComponentCompareID);
	NewCoorList = pathoptimize(CpnList);


	// �Ը������������
	int nCpnCount = CpnList.size();
	for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
	{
		AllPoint OneCpnOutline;
		Component* pCpn = CpnList.at(i_cpn);
		gcodeCoor coor = NewCoorList.at(i_cpn);
		float cpn_width = pCpn->m_RealWidth;


		// �޸Ķ�������
		coor._y = panel_width - coor._y - cpn_width;
		NewCoorList.at(i_cpn) = coor;

		// ƫ��������
		vector<PointInfo> NewPointList;
		for (int i_pnt = 0; i_pnt < pCpn->m_vOutlinePoint.size(); i_pnt++)
		{
			PointInfo point = pCpn->m_vOutlinePoint.at(i_pnt);

			point.y -= cpn_width;		// �����ƶ�һ�����
			point.y = -point.y;			// ��ȡ��

			NewPointList.push_back(point);
		}

		// ����
		OneCpnOutline.panel_info = pCpn->m_BarCode.GetBuffer(0);
		OneCpnOutline.point_info = NewPointList;
		OneCpnOutline.m_NumInPanel = pCpn->m_NumberInPanel;

		// ���һ�������������
		AllPointList.push_back(OneCpnOutline);
	}

	// ����������
	m_allpoint.clear();
	m_allpoint = AllPointList;


	// �����µ���
	NewCoorList = rollsetoptimize(NewCoorList);	// �µ�������



	// �����µ���
	for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
	{
		Component* pCpn = CpnList.at(i_cpn);
		gcodeCoor coor = NewCoorList.at(i_cpn);


		pCpn->m_nKnifeDownPos = coor.pos;
	}
}






// ������֤
bool KnifeClass::InternetChecking(void)
{
	//������֤
	CString str_mac;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// ��ȡmac��ַ
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	if (ERROR_SUCCESS==nRel)
	{
		for (int i = 0;i < 6;i++)
		{
			CString tmp_mac; 
			tmp_mac.Format(_T("%x "),pIpAdapterInfo->Address[i]);
			str_mac = str_mac + tmp_mac;
		}
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	std::string hostName = "www.techtimesun.net";
	std::string check_pathName = "API/LockAPI.aspx?action=check&mac=";
	check_pathName = check_pathName + str_mac.GetBuffer();

	std::string insert_pathName = "API/LockAPI.aspx?action=insert&mac=";
	insert_pathName = insert_pathName+str_mac.GetBuffer();

	std::string postData = "";

	if(PostHttpPage(8060, hostName, check_pathName,"check") == false)
		return false;

	if (PostHttpPage(8060, hostName, insert_pathName,"insert") == false)
		return false;

	return true;
}


/*---------------------------------------*/
//	����˵����
//
//
//
//	������
//		int port						--	�˿ں�
//		const std::string & hostName	--	��������
//		const std::string & pathName	--	·����
//		const std::string & postData	--	��������
//
//	����ֵ:
//
//
/*---------------------------------------*/

bool KnifeClass::PostHttpPage(int port,const std::string & hostName, const std::string & pathName, const std::string & postData)
{
	// ����ػ�
	CInternetSession session(_T("your app agent name")); 

	try 
	{ 
		INTERNET_PORT nPort = (INTERNET_PORT)port; 
		DWORD dwRet = 0;
		CHttpConnection* pServer = session.GetHttpConnection((LPCTSTR)hostName.c_str(), nPort); 
		CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)pathName.c_str());   
		CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // ����ͷ   

		//��ʼ��������   
		pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size()); 
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet == HTTP_STATUS_OK) 
		{ 
			CString result, newline;   
			while(pFile->ReadString(newline)) 
			{
				//ѭ����ȡÿ������ 
				result = result + newline+"\r\n"; 
			}   

			if (postData == "check")
			{
				CString message_result = result;


				// �����ǰ����

				message_result.Replace("Message","");
				message_result.Replace("Result","");
				message_result.Replace("true","");
				message_result.Replace("false","");
				message_result.Replace("\"","");
				message_result.Replace(",","");
				message_result.Replace(":","");
				message_result.Replace("{","");
				message_result.Replace("}","");
				std::string result_num = message_result.GetBuffer();
				int i_num = atoi(result_num.c_str());

				AfxMessageBox(CString("��ǰ��ʹ�ô���Ϊ��") + message_result);


				if (i_num >= 1000)
				{
					AfxMessageBox("ʹ�ô���������Ȩ����");
					return false;
				}
			}
			else
			{


				if (result.Find("true") < 0)
				{
					AfxMessageBox("���º�̨����ʧ��");
					return false;
				}
			}

		} 
		else 
		{ 
			return false; 
		} 
		delete pFile; 
		delete pServer;   
	} 
	catch (CInternetException* pEx) 
	{ 
		TCHAR pszError[200]; 
		pEx->GetErrorMessage(pszError, 200);   
		AfxMessageBox(pszError);
		return false; 
	} 

	session.Close();   
	return true; 
}


/*---------------------------------------*/
//	����˵����
//		����ʾ����
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
void KnifeClass::EnaleMsgReport()
{
	m_bReportMsg = true;
}

/*---------------------------------------*/
//	����˵����
//		�ر���Ϣ��ʾ
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
void  KnifeClass::UnaleMsgReport()
{
	m_bReportMsg = false;
}



/*---------------------------------------*/
//	����˵����
//		��ӡ���ֵ�������
//
//
//	������
//		char* string		--	�����ַ
//		int decimal_place	--	С��λ��
//		float val			--	ֵ
//
//	����ֵ:
//
//
/*---------------------------------------*/
void KnifeClass::PrintNumber2Array(char* str, int str_len, int decimal_place, float val)
{

	switch(decimal_place)
	{
	case 0:
		sprintf_s(str, str_len,"%0.1f",val);
		break;
	case 1:
		sprintf_s(str, str_len,"%0.2f",val);
		break;
	case 2:
		sprintf_s(str, str_len,"%0.3f",val);
		break;
	default:
		sprintf_s(str, str_len, "%0.4f",val);

		break;
	}

	
}