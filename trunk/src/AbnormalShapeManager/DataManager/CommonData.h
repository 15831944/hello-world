/*--------------------------------------------------------------------------------------------------------------------*/
//	CommonData.h     --	 �������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.7.8
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		CommonData_H
#define		CommonData_H

#define		PI							(3.1415926535898)
#define		CLOSE_VALUE					(0.1)						// �ж������ص��ľ���
#define		OUTLINE_SPACE				(3)							// Բ/�������߾���mm
#define		ARC_INTERPOLATE_VALUE		(0.5)						// ���ӻ��ҳ�����0.5mm��岹������
#define		ARC_INTERPOLATE_MIN_VALUE	(0.3)						// ���ӻ��ҳ�����0.3mm�Ż�岹�м�㣬����ͨ��eps�ж�Ϊ�ཻ������Ҫ�岹

#define		Angle2Radian(a)				((a)*PI/180.0)				// �Ƕ�ת����
#define		Radian2Angle(r)				((r)*180.0/PI)				// ����ת�Ƕ�

#define		OBTUSE_ANGLE_VALUE			(100)						// ����Ѷ۽ǵ�����Ϊ�˱���90�Ƚǵ��жϳ�������ֱ�߼��Բ������  2017-8-30 �����ϸ�ĵ�����ʽ�����ཻ�ľ���Բ������
																	// �����н����߶μнǻ������н�Ϊ��ǣ��߶��ཻ��Բ�����ӡ�	�н�Ϊ�۽ǣ�ƽ��ֱ���ཻ
#define		FIGURE_MIN_LEN				(10.0)						// ��Сͼ�γ���


#define		eps							(0.2)
#define		zero(x)						(((x)>0?(x):-(x))<eps)
struct point{double x,y;};





typedef	enum
{
	FigureType_Unknow = 0,
	FigureType_PolyLine,
	FigureType_Arc,
	FigureType_Circle,
	FigureType_Complex,

}FigureType;

// Բ������
enum 
{
	Dir_Parallel,			// ƽ�У�����ֱ��
	Dir_CouterClockwise ,	// ��ʱ��
	Dir_Clockwise ,			// ˳ʱ��
};

// ���ӷ�ʽ
enum
{
	Link_Head2Head,
	Link_Head2Tail,
	Link_Tail2Head,
	Link_Tail2Tail,
};


// ƽ�з���
enum 
{
	Dir_Left,				// ���
	Dir_Right ,				// �ұ�
};



// ��������ö�ٶ���

// 0��ֱ�ߵ� 1:Բ����� 2��Բ���յ� 3������Բ�����ӵ� -1�����账��ĵ�
typedef enum 
{
	Sign_Ignore = -1,
	Sign_Line,
	Sign_ArcStart,
	Sign_ArcEnd,
	Sign_ArcLink

}OutlineSign;

// 0��ֱ�ߵ� 1����ʱ�� 2��˳ʱ��
typedef enum 
{
	Dir_Line = 0,
	Dir_ArcCouterClockWise,
	Dir_ArcClockWise,

}OutlineDir;

// 0������ 1����Ҫ�� 2����ʾ�����µ�
typedef enum 
{
	Cut_KnifeDown = 0,
	Cut_KnifeUp,
	Cut_KnifeRedown,

}OutlineCut;

// 0������ 1��CNC��ϳ��С����ת���ף�
typedef enum 
{
	Type_Regular= 0,
	Type_CncCircle,

}OutlineType;

// 0������(ͼ���еĵ�) 1��ͼ�ο�ʼ  2��ͼ�ν���
typedef enum 
{
	Group_Regular = 0,	// 0������(ͼ���еĵ�)
	Group_Start,		// 1��ͼ�ο�ʼ
	Group_End,			// 2��ͼ�ν���

}OutlineGroup;

// 1������  2������   3��������
typedef enum 
{
	Side_Out = 1,		// 1������
	Side_In,			// 2������ 
	Side_Center,		// 3��������

}OutlineSide;

#endif