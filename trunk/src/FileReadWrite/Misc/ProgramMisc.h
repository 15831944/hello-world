#pragma once
#include <string>
#include <map>
#include <vector>
#include "../Work/WorkDef.h"

using namespace std;

//����ת
string ExchangeSlotXY(string str_slot/*, float fXLen*/, float fYLen);

//�׷�ת
string ExchangeVerticalXY(string str_vertical/*, float fXLen*/, float fYLen);

//�����ת
//�������۷�ת���׷�ת�����η�ת����߷�ת�������ַ�����ת
void TurnOverVecSlot(std::map<std::string,std::string>& map_one_matrial);

//�����µ�����
CString newBarCode(int seed);
//���Ψһ����
CString uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck);

LineType GetLineType(int nLineSign);

int GetLineLinkingTypeByLineType(LineType eFrontLineType, LineType eBackLineType);