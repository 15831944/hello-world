#pragma once
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../Misc/Misc.h"
#include "../UIData/UIData.h"
#include <map>

using namespace std;

void ConvertInputInfoToComponentList(vector<ComponentInputItem>& vComponentInputItem, ComponentList& componentList);

void ConvertInputInfoToComponentList(vector<ComponentInputItem>& vComponentInputItem, vector<PreCombineItem>& vPreCombineItem, ComponentList& componentList/*, map<Component*, PreCombineItem>& mapComponentToPreCombine*/);

Component* ConvertOneInputInfoToOneComponent(ComponentInputItem& theInputItem);

void SplitComponentList(ComponentList& componentList, vector<ComponentList>& vComponentList);

void ProcessOptimize(/*CSingleon& data*/);

void PreProcessOfExportGCode();

void PostProcessOfExportGCode();

PointFloat createTiebiaoPosition(Component& theComponent,float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap = 0, float fYGap = 0, bool bLabelInUpperSide = true);//Ϊ�������������Ϣ��������������������������ǩ��ߣ��õ�����Ӧ�����ĸ���Ϊ��������

//��������ϵ�е������ڳ���
std::vector<PointFloat> createPointMatrixExtend(float panelWidth,float panelHeight, float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap, float fYGap);

//�ж���������Ƿ�Ϸ�
bool isTieBiaoPositionUsable(Component& theComponent, PointFloat point, float tiebiao_width , float tiebiao_height, bool bLabelInUpperSide = true);

PointFloat GetDefaultLabelPos(Component& theComponent, float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap = 0, float fYGap = 0, bool bForce = false);

PointFloat GetAttachPos_Component(Panel* pPanel, PointFloat ptMouseInPanel, float fComponentXLen, float fComponentYLen, float fKerf, int nAttachDistance);

PointFloat GetAttachPos_RemainderCutting_Start(Panel* pPanel, PointFloat ptMouseInPanel, float fKerf, int nAttachDistance);

PointFloat GetAttachPos_RemainderCutting_End(Panel* pPanel, PointFloat ptMouseInPanel, float fKerf, int nAttachDistance, PointFloat ptStartInPanel);

PointFloat GetAttachPos_PreCombineComponent(Panel* pPanel,  vector<PreCombineComponent> vPreCombineComponent, int nSelectedItem, PointFloat ptfMovingPosInPanel, float fKerf, int nAttachDistance);

void CalcKnifeDownPosInPanel(Panel* pPanel);

void CalcKnifeDowPos();