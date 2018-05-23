#ifndef		_CommonData_h_
#define		_CommonData_h_


#pragma once
#include <vector>
#include <map>

using namespace std;

// ȫ�ֺ궨��
#define		NEW_ENCRYPT_BASE64		1

#define HG_EDITION 0
#define YW_EDITION 1

#define CUR_EDITION HG_EDITION
















// ����Ľڵ�����
typedef enum NodeType
{
	NodeType_Remainder = 0,			// ����								Ҷ�ӽڵ�
	NodeType_NeededComponent,		// ��Ҫ��С��						Ҷ�ӽڵ�
	NodeType_CombinePanel			// ��ϰ� �����Ϻ���Ҫ��С�����	���ڵ�
}NodeType;

// ������
typedef enum TextureType
{
	TextureType_NO_TEXTURE = 0,			// ������
	TextureType_H_TEXTURE,				// ����
	TextureType_V_TEXTURE				// ����
}TextureType;

// �и��
typedef enum CutDir
{
	CutDir_Horizon = 0,					// ����
	CutDir_Vertical,					// ����
	CutDir_Random,						// ���
	CutDir_Default						// Ĭ��

}CutDir;

// С������ԭ��
typedef enum LayoutOrg
{
	LayoutOrg_LeftBottom = 0,			// ���½�
	LayoutOrg_LeftTop,					// ���Ͻ�
	LayoutOrg_RightBottom,				// ���½�
	LayoutOrg_RightTop					// ���Ͻ�

}LayoutOrg;


// �������  ԭ�ϡ�����

typedef enum PanelType
{
	PanelType_RawMaterial = 0,			// ԭ��
	PanelType_Remainder,				// ����

}PanelType;





// �ļ�����С��
struct ComponentInputItem
{
	ComponentInputItem()
		:m_fLength(0.0)
		,m_fWidth(0.0)
		,m_fThickness(0.0)
		,m_nCount(0)
		,m_eRotateType(0)
		,m_bSlotFlipped(false)
		,m_bVHoleFlipped(false)
		,m_fProductLength(0.0)
		,m_fProductWidth(0.0)
		,m_fProductThickness(0.0)
	{
		m_strTexture = _T("������");
		m_afBanding[0] = 0.0;
		m_afBanding[1] = 0.0;
		m_afBanding[2] = 0.0;
		m_afBanding[3] = 0.0;
	}

	UINT m_uSerialID;                    //���кţ����ڱ�ʶ����
	CString m_strOrderID;               //������
	CString m_strCabinetID;             //���
	CString m_strBarcode;				// ����
	CString m_strCabinetName;			// ������
	CString m_strPanelName;				// �����
	CString m_strMaterial;				// ����
	float m_fLength;					// ���ϳ�
	float m_fWidth;						// ���Ͽ�
	float m_fThickness;					// ���Ϻ��
	int m_nCount;						// ��������
	CString m_strTexture;				// ����
	float m_afBanding[4];               //���
	CString m_strOtherShapeIDAndParam;	//���β���
	int m_eRotateType;					//��ת
	CString	m_strUpperFaceSlotInfo;		// �������Ϣ
	bool m_bSlotFlipped;                //�۷�ת
	CString	m_strDownerFaceSlotInfo;	// �������Ϣ
	CString	m_strUpperFaceHoleInfo;		// �������Ϣ
	bool m_bVHoleFlipped;                //�׷�ת
	CString	m_strDownerFaceHoleInfo;	// �������Ϣ
	CString m_strCustomerInfo;          //�ͻ���Ϣ
	CString m_strJoinedStore;           //���˵�
	CString m_strSlottingFlag;          //���۱�ʶ
	CString m_strReminder;              //���Ǻ�
	CString m_strDrilling;              //���
	CString m_strOrderType;             //��������
	CString m_strReverseSideBarcode;    //��������
	float m_fProductLength;             //��Ʒ����
	float m_fProductWidth;              //��Ʒ���
	float m_fProductThickness;          //��Ʒ���
	CString m_strOtherShapeID;			// ����ID
	CString m_strCustomerAddress;       //�ͻ���ַ
	CString m_strHoleSlotFlag;          //��۱�ʶ
	CString	m_strAbnormalInfo;			// ���ε���Ϣ
	CString m_strPanelClass;            //�������,"Door"Ϊ�Ű壬"Panel"Ϊ��ͨ��
	CString m_strSendingDate;           //��������
};

class CSolution;
class Panel;

struct PanelViewingParam
{
	CSolution* m_pSolution;
	Panel* m_pPanel;
	bool m_bChecked;
};

typedef struct 
{
// 	RemainderInputItem()
// 		: m_nXLen(0)
// 		, m_nYLen(0)
// 		, m_fThickness(0.0)
// 		, m_nCount(0)
// 	{}

	int m_nXLen;
	int m_nYLen;
	float m_fThickness;
	CString m_strMaterial;
	int m_nCount;
}RemainderInputItem;


// ����NC������ݽṹ

#define  NEW_PROGRAM	(1)		// �³��� �޸������ݽṹ



// ��һ��
struct CutPath
{
	float _x1;
	float _y1;
	float _x2;
	float _y2;
};

// ������
struct P_Info
{
	string _info;	// ���
	string _content;	// ����
};
// 
struct Remove_Line
{
	float begin_x;
	float begin_y;
	float end_x;
	float end_y;
};
// λ��
struct pos
{
	float x,y;	// ��������
	int _kn;	// ����
};
// 
struct changexy
{
	float _size;
	float _x;
	float _y;
};
// ���ڵ�
struct PanelNode
{
	float _x,_y;			// ���Ŀǰʣ������ԭ���ϵ�λ��
	float _height,_width;	// �и�����Ĵ�С
	PanelNode *lchild;		// �и���������
	PanelNode *rchild;		// �и���������
	int _num;				// ����
};
// ������
struct KnifeType
{
	int kt;						// ������ 
	float _diameter;			// ֱ��
	string _tgcodeb;			// ��ͷ����
	string _tgcodee;			// ��β
	float _feedrate;			// �����ٶ�
	float _spindlespeed;		//
	float _endspeed;			// �����ٶ�
	float _overspend;			// �����ٶ�
	float _facespeed;			// �Ӵ������ٶ�
	float _kx;					// ����xλ��
	float _ky;					// ����yλ��
	float _length;				// �ָ���ޣ�����ȵ�����С����ּ��ο��ϣ���_kinfelength = 30 _length = 20 ���һ���е��߶�Ϊ10������20�񣩣��ڶ����е��߶�-0.1(�д�)
	int _kn;					// ����
	string _coorsys;			// ����ϵ
	float _kinfelength;			// ��Ч���ȣ������ܴ����
};

// ��ͨ���
struct p_Component
{
	float _height,_width;		// ������Ĵ�С
	float _x,_y;				// ��������������е�λ��
	string _info;			// �������
	int _dir;					// 0��ʾ������1��ʾ���ƣ�2��ʾ����
	//	float label_x;				// ����x���꣬��԰�����Ͻ�
	//	float label_y;				// ����y���꣬��԰�����Ͻ�
	//	float label_width;			// ������
	//	float label_height;			// ����߶�
};

// ���е����
struct p_ComponentA
{
	float _height,_width;		//	������Ĵ�С
	float _x,_y;				//	��������������е�λ��
	string _info;			//	�������
	int _dir;					//	0��ʾ������1��ʾ���ƣ�2��ʾ����
	int _num;					//	���е�����
};

// ������
struct p_Components
{
	float _height,_width;		// �������ĳ���
	vector<p_Component> _coms;	// ������� �����ж��
};

// �������
struct Coordinate
{
	p_Components _onecoms;		// ������ ���ܴ��ڶ���������ͬһ������
	float _x,_y;				// ��Ӧ�����ԭ�����е�����
};

struct gcodeCoor //����ÿ��С������꣬��С��������Ϣ�����ݣ����ڵ���G����ͻ���ʾ��ͼ
{
	float _x,_y;				// x��y����
	string _info;			// ����
	float _height,_width;		// ������
	int _dir;					// 1������  2������
	int pos;					// �䵶���

	// ����Ψһ����ID
	int m_NumInPanel;

	//	float label_x;				// ����x���꣬��԰�����Ͻ�
	//	float label_y;				// ����y���꣬��԰�����Ͻ�
	//	float label_width;			// ������
	//	float label_height;			// ����߶�
};

// ԭʼ����Ϣ
struct PointInfo
{
	float x;	// x���꣬��������������ΰ�ԭ������
	float y;	// y���꣬��������������ΰ�ԭ������
	float r;	// �뾶��ֱ�ߵ�Ϊ 0��Բ����Ϊ�뾶ֵ
	int sign;	// 0��ֱ�ߵ�  1��Բ����� 2��Բ���յ�
	int dir;	// 0��ֱ�� 1����ʱ�� 2��˳ʱ��
	int cut;	// 0:������ 1���� 2�������䵶

	int group;	//  1����� 2���յ�
	int side;	// �ӹ����� 1�����У��������� 2�����У��������� 3�����ĵ�
	int type;	// ϳ������?0����CNC�ӹ��� 1��CNC�ӹ���

};

// ��������Ϣ
struct DPointInfo
{
	float x;	// x���꣬��������������ΰ�ԭ������
	float y;	// y���꣬��������������ΰ�ԭ������
	float r;	// �뾶��ֱ�ߵ�Ϊ 0��Բ����Ϊ�뾶ֵ
	int sign;	// 0��ֱ�ߵ�	1��Բ����� 2��Բ���յ�
	int dir;	// 0��ֱ��		1����ʱ��	2��˳ʱ��
	int cut;	// 0: ������	1����		2���޵�


#if (NEW_PROGRAM == 1)

	int group;	//  1����� 2���յ�
	int side;	// �ӹ����� 1�����У��������� 2�����У��������� 3�����ĵ�
	int type;	// ϳ������ 0����CNC�ӹ��� 1��CNC�ӹ���
#endif


	int posold;
	int posnew;
};

// ���е�ԭʼ��
typedef struct AllPoint
{
	// ����Ψһ����ID ȡ��������Ϊ�жϱ�׼
	int m_NumInPanel;

	string panel_info;			// ������
	vector<PointInfo> point_info;
}AllPoint;

// ���еĵ�����
typedef struct DAllPoint
{
	string panel_info;
	vector<DPointInfo> point_info;
}DAllPoint;

// ���еĲ�
typedef struct AllSlot
{
	AllSlot():m_bDisabled(false){}
	string panel_info;
	float _x;		// �۵ĳ���������λ��
	float _y;		// �۵ĳ���������λ��
	float _width;	// ��� x���򳤶�
	float _height;	// �߶�	y���򳤶�
	float _depth;	// �۵����
	int _dir;		// �۵ķ��� 0������  1�����

	// ����Ψһ����ID ȡ��������Ϊ�жϱ�׼
	int m_NumInPanel;

	bool m_bDisabled; // �Ƿ񱻽��ã������õĲ����뵼��NC��
}AllSlot;

// ��
typedef struct AllVec
{
	AllVec():m_bDisabled(false){}
	string panel_info;	// ���
	float _x;			// ���С�����ĵ�����
	float _y;			// ���С�����ĵ�����
	float _r;			// �뾶
	float _depth;		// ���

	// ����Ψһ����ID ȡ��������Ϊ�жϱ�׼
	int m_NumInPanel;

	bool m_bDisabled; // �Ƿ񱻽��ã������õĲ����뵼��NC��
}AllVec;

// ���Բ���
struct GcodeParam
{
	string header;
	string ender;
	float prospeech;
	float cutspeech;
	float endspeech;
	float overspeech;
	float upheight;		// ��ȫ�߶�
	float downheight;	// �µ����
	float distance1;	// �µ�ƫ��1����ȫ�߶�-���Ե�ԭ��
	float distance2;	// �µ�ƫ��2���Ե�ԭ��-���µ����
	float org_offset;	// �Ե�ԭ��ƫ�ƣ�Ϊ�˷�ֹ���ߣ������ڽ��Ե�ԭ����������1mm
	string strRinget;
};

// С��ߴ�
struct OptimizeParam
{
	float minwidth;
	float minheight;
	float minsize;	
	float minrate;		// ��С������
	int _knife_type;
};

// ������Ϣ
struct BarString
{
	string bar_code;	// ������
	string print_info;
};

struct Info_dir
{
	int _dir;
	string _info;
};


// ����λ��
typedef struct
{
	float x;
	float y;
}PointFloat;



// ���굶��ֱ��
typedef struct 
{
	std::string panel_info;						// �������
	float _pwidth;								// ���
	float _pheight;								// ���
	float _x;									// x
	float _y;									// y
	float _r;									// r�뾶
	float _depth;								// ���
	float _diameter;							// ֱ��
	float src_x;								// ���ԭ��x
	float src_y;								// ���ԭ��y
	std::string _tgcodeb;						// ������ʼ����
	std::string _tgcodee;						// ���߽�������
	std::string _coorsys;						// ����ϵƫ��
	int _feedrate;								// �ٶ�
	int _spindlespeed;							// �ٶ�
	int _overspeed;								// �ٶ�
	int _endspeed;								// �ٶ�
	int _facespeed;								// �ٶ�

	std::vector<float> _kx;						// ��x����
	std::vector<float> _ky;						// ��y����
	std::vector<int> _kn;						// ��������
	std::vector<int> _samedownkn;				// ˫����
	std::vector<std::string> BeginCodeList;		// ��ʼ�����б�
	std::vector<std::string> EndCodeList;		// ���������б�

	int _thiskn;								// ��ѵ���
	float _thiskx;								// ��ѵ���
	float _thisky;								// ��ѵ���
	bool _kdown;								// ��ѵ���
}KnifeTypeAndVecA;

// ����;
typedef enum 
{
	KnifeType_Spindle_Hole_Slot_Material = 0,		// ����ײ���		0
	KnifeType_Spindle_Hole_Slot,					// ����ײ�			1
	KnifeType_Spindle_Hole,							// �����			2
	KnifeType_Spindle_Slot,							// �����			3
	KnifeType_Spindle_Material,						// ������			4
	KnifeType_Spindle_Slot_Material,				// �������			5
	KnifeType_Drill_Hole,							// �����			6
	KnifeType_Multi_knife_Hole,						// �൶ͷ���		7

}enumKnifeType;

// ����ֱ�ף�����
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _r;
	float _depth;
	float _diameter;
	std::string _tgcodeb;
	std::string _tgcodee;
	int _feedrate;
	int _spindlespeed;
	float _srcx;
	float _srcy;
}KnifeTypeAndVec;

// ������
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _width;
	float _height;
	float _depth;
	int _dir;
	float _diameter;
	std::string _tgcodeb;
	std::string _tgcdoee;
	int _feedrate;
	int _spindlespeed;
	float _srcx;
	float _srcy;
}KnifeTypeAndSlot;

// ��
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _width;
	float _height;
	float _depth;
	int _dir;
	float _srcx;
	float _srcy;
}AllSlotK;

// ��
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _r;
	float _depth;
	float _srcx;
	float _srcy;
}AllVecK;

class KnifeClass;


typedef  struct Panel_Knife
{
	PanelNode * _panel;
	KnifeClass * _kc;
	vector<Coordinate> _coor;
	CStringW big_file;
}Panel_Knife;

typedef struct print_info
{
	CString panel_no;
	CString com_no;
	CString customer_info;
	CString bar_code;
	CString com_width;
	CString com_height;
	CString com_depth;
	CString com_name;
	CString com_matrial;
	CString cst_no;
	CString cst_name;			//�������ƣ���Ʒ���ƣ�
	CString edge_info;
	CString product_width;
	CString product_height;
	CString other_com;			//����
	CString slot_com;			//����
	CString punching_com;		//���
	CString double_com;			//˫��
	CString franchisee;			//���˵�
	CString ordertype;			//��������
	CString rebar_code;			//��������
	CString customer_address;
	CString zc_identify;
	CString panel_dir;
	CString panel_reversal;
	int     nRotatedAngle;
	bool    bIsDoorPanel;       //�Ƿ��Ű�
	CString m_strSendingDate;   //��������
}print_info;



// �ͻ���Ϣ�ṹ��
class ClientInfo
{
public:
	CString		m_ClientName;
	CString		m_PhoneNumber;
	CString		m_QQNumber;
	CString		m_Address;
	
	CString		m_MacAdress;
	CString		m_BaseBoardSerialNo;
	CString		m_CPUSerialNo;
	CString		m_HDSerialNo;

	int			m_OutputNCTimes;
	long		m_RegisterTime;	


};

struct SourceFilePreProccesParam
{
	SourceFilePreProccesParam()
		:b_upvecImportEnable(true)
		,b_downvecImportEnable(true)
		,b_upsoltImportEnable(true)
		,b_downsoltImportEnable(true)
		,b_othershapeImportEnable(true)
		,i_comnum(1)
		,i_comchange(0)
		,f_changex(0.0)
		,f_changey(0.0)
		,f_width(0.0)
		,f_height(0.0)
		,f_Deburringwidth(0.0)
		,bReserveDeepHole(false)
		,bReserveDeepSlot(false)
	{}

	bool b_upvecImportEnable;//�����
	bool b_downvecImportEnable;//�����
	bool b_upsoltImportEnable;//�����
	bool b_downsoltImportEnable;//�����
	bool b_othershapeImportEnable;//���ι���


	int i_comnum;//�и�����
	int i_comchange;//��ת���ã�����ת=0����������Ϣ��ת=1��������ȷ�ת=2���������ȷ�ת=3��
	float f_changex;//�ײ�ƫ��
	float f_changey;//�ײ�ƫ��

	//��λƫ��ҵ�ָ����ֱ���Ŀף���ֱ����һ��ֵ��
	std::map<float,float> vecAdder;

	//��λ���ˣ��ҵ�ָ����ֱ���Ŀף����Ƴ���
	vector<float> _cxy;

	float f_width;//ԭ�ϳ���
	float f_height;//ԭ�Ͽ��
	float f_Deburringwidth; //�ޱ߾���

	bool bReserveDeepHole;
	bool bReserveDeepSlot;
};


// ����CSV�ļ���Ϣ
typedef struct 
{
	CString strInfo_A;					//	A		//������
	CString strInfo_B;					//	B		//���
	CString strInfo_C;					//	C		//����
	CString strInfo_D;					//	D		//��Ʒ����
	CString strInfo_E;					//	E		//��Ʒ����
	CString strInfo_F;					//	F		//����
	CString strInfo_G;					//	G		//��
	CString strInfo_H;					//	H		//��
	CString strInfo_I;					//	I		//���
	CString strInfo_J;					//	J		//�и�����
	CString strInfo_K;					//	K		//������
	CString strInfo_L;					//	L		//�ⳤ1
	CString strInfo_M;					//	M		//���1
	CString strInfo_N;					//	N		//�ⳤ2
	CString strInfo_O;					//	O		//���2
	CString strInfo_P;					//	P		//�Ƿ���ת
	CString strInfo_Q;					//	Q		//�۷�ת
	CString strInfo_R;					//	R		//�׷�ת
	CString strInfo_S;					//	S		//�ͻ���Ϣ
	CString strInfo_T;					//	T		//���˵�
	CString strInfo_U;					//	U		//���۱�ʶ

	// ���룬���ܸ�
	CString strDirInfo_V;				//	V
	CString strZplInfo_W;				//	W
	CString strZplInfo2_X;				//	X
	CString strBmpInfo_Y;				//	Y
	CString strLabNum_Z;				//	Z

	CString strInfo_AA;				//	AA			//���Ǻ�
	CString strInfo_AB;				//	AB			//���
	CString strInfo_AC;				//	AC			//��������
	CString strInfo_AD;				//	AD			//��������
	CString strInfo_AE;				//	AE			//��Ʒ����
	CString strInfo_AF;				//	AF			//��Ʒ���
	CString strInfo_AG;				//	AG			//��Ʒ���
	CString strInfo_AH;				//	AH			//����ID
	CString strInfo_AI;				//	AI			//�ͻ���ַ
	CString strInfo_AJ;				//	AJ			//��۱�ʶ







}EnDeCsvInfo;	





typedef struct 
{
	CString noProcessSmallPanelOne1;	//������
	CString noProcessSmallPanelOne2;	//���
	CString noProcessSmallPanelOne3;	//����
	CString noProcessSmallPanelOne4;	//��Ʒ����
	CString noProcessSmallPanelOne5;	//��Ʒ����
	CString noProcessSmallPanelOne6;	//����
	CString noProcessSmallPanelOne7;	//��
	CString noProcessSmallPanelOne8;	//��
	CString noProcessSmallPanelOne9;	//���
	CString noProcessSmallPanelOne10;	//�и�����
	CString noProcessSmallPanelOne11;	//������
	CString noProcessSmallPanelOne12;	//�ⳤ1
	CString noProcessSmallPanelOne13;	//���1
	CString noProcessSmallPanelOne14;	//�ⳤ2
	CString noProcessSmallPanelOne15;	//���2
	CString noProcessSmallPanelOne16;	//�Ƿ���ת
	CString noProcessSmallPanelOne17;	//�۷�ת
	CString noProcessSmallPanelOne18;	//�׷�ת
	CString noProcessSmallPanelOne19;	//�ͻ���Ϣ
	CString noProcessSmallPanelOne20;	//���˵�
	CString noProcessSmallPanelOne21;	//���۱�ʶ
	CString noProcessSmallPanelOne22;	//���Ǻ�
	CString noProcessSmallPanelOne23;	//���
	CString noProcessSmallPanelOne24;	//��������
	CString noProcessSmallPanelOne25;	//��������
	CString noProcessSmallPanelOne26;	//��Ʒ����
	CString noProcessSmallPanelOne27;	//��Ʒ���
	CString noProcessSmallPanelOne28;	//��Ʒ���
	CString noProcessSmallPanelOne29;	//����ID
	CString noProcessSmallPanelOne30;	//�ͻ���ַ
	CString noProcessSmallPanelOne31;	//��۱�ʶ


}PrintInfo;


#endif
