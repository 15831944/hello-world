#pragma once

#define PLAIN_FILE_OPEN_ERROR -1  
#define KEY_FILE_OPEN_ERROR -2  
#define CIPHER_FILE_OPEN_ERROR -3  
#define OK 0   
typedef char ElemType; 

////��ʼ�û���IP  
//extern int IP_Table[64];   
////���ʼ�û���IP^-1  
//extern int IP_1_Table[64];  
//
////�����û���E  
//extern int E_Table[48];  
//
////�û�����P  
//extern int P_Table[32]};  
//
////S��  
//extern int S[8][4][16];  
////�û�ѡ��1  
//extern int PC_1[56];  
//
////�û�ѡ��2  
//extern int PC_2[48];  
//
////�����ƴ����Ĺ涨  
////ģ�黯
//extern int MOVE_TIMES[16]; 

extern "C" __declspec(dllexport) int func_drawing0(char* src_path, char* dst_path); 
extern "C" __declspec(dllexport) int func_drawing1(char* src_path, char* dst_path); 

 
