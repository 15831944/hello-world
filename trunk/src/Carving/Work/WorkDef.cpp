#include "stdafx.h"
#include "WorkDef.h"

char* g_aRequirementListTitle[] = {"������", "���", "������", "��Ʒ����", "��Ʒ����", "��������", "���ϳ���", "���Ͽ��", "���Ϻ��", "��������", "��·����", "�ⳤ1", \
	"���1", "�ⳤ2", "���2", "����", "��ת", "�����", "�۷�ת", "�����", "�����", "�׷�ת", "�����", "�ͻ���Ϣ", "���˵�", "���۱�ʶ", "���Ǻ�", \
	"���", "��������", "��������", "��Ʒ����", "��Ʒ���", "��Ʒ���", "����ID", "�ͻ���ַ", "��۱�ʶ", "���ε���Ϣ"};

char* g_aRequirementListTitle_ENU[] = {"order", "cabinet", "info", "PorductName", "porduct", "matrial", "width", "height", "thin", "num", "dir", "edgeLen1", \
	"edgeWidth1", "edgeLen2", "edgeWidth2", "pointInfo", "rotate", "slotInfo", "slotRotate", "dslotInfo", "vecInfo", "vecRotate", "dvecInfo", "customeInfo", "store", "SawMark", "HelpSign", \
	"Vertical", "OrderType", "BackCode", "ProductWidth", "ProductHeight", "ProductThin", "otherID", "customeAddr", "DrillingLogo", "OtherPoints"};

char* g_aLabelMachineName[] = {"��", "�ǻ�", "��ŵ-ά��ϵͳ", "����-�´�ϵͳ", "����", "����-�´�ϵͳ", "����-ά��ϵͳ", "��ŵ-�´�ϵͳ", "�ص�-ά��ϵͳ", "����", "�ص�-�´�ϵͳ", \
	"�л�����", "��ï-�´�ϵͳ", "��ŵK1", "WorkList-�´�ϵͳ", "��ŵK2", "����", "�ȴ�SKL-2613E", "��ŵK1-������"};

int g_aMaterialListItem[] = {LIST_COLUMN_ORDER_ID, LIST_COLUMN_CABINET_ID, LIST_COLUMN_BARCODE, LIST_COLUMN_REVERSE_SIDE_BARCODE, LIST_COLUMN_CABINET_NAME, LIST_COLUMN_PANEL_NAME, LIST_COLUMN_MATERIAL, \
	LIST_COLUMN_PRODUCT_LEN, LIST_COLUMN_PRODUCT_WIDTH, LIST_COLUMN_PRODUCT_THICKNESS, LIST_COLUMN_COUNT, LIST_COLUMN_BANDING_FRONT, LIST_COLUMN_BANDING_RIGHT, LIST_COLUMN_BANDING_BACK, LIST_COLUMN_BANDING_LEFT, \
	LIST_COLUMN_ORDER_TYPE, LIST_COLUMN_CUSTOMER_INFO, LIST_COLUMN_CARVING_LEN, LIST_COLUMN_CARVING_WIDTH, LIST_COLUMN_CARVING_THICKNESS, LIST_COLUMN_TEXTURE, LIST_COLUMN_OTHER_SHAPE_ID, LIST_COLUMN_CUSTOMER_ADDRESS/*, LIST_COLUMN_HOLE_SLOT_FLAG*/};