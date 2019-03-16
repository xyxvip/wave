
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once
////////////////������Ҫʹ�õ�ȫ�ֱ���
#define DATA_K 1024
#define DATA_8K 1024*8
#define DATA_32K 1024*32
#define DATA_64K 1024*64
#define DATA_M 1024*1024
#define DATA_4M DATA_M*4
#define MAX_NUM DATA_M*16
#define TOTAL_DATA DATA_4M*5
#define PI 3.141592654
#define FM  1048576//�������Ƶ�ʵ�λMhz
#define NFFT  8192// FFT���õĵ���NFFT
extern long signal_recvNum[4];//�洢���յ��ĸ�ͨ�����ݳ���
extern char* RecvBuffer;//�洢���յ�������
extern double* Analysisdata;//���ڴ洢ʮ���ƴ�������
extern double* SignalOne;
extern double* SignalTwo;
extern double* SignalThree;
extern double* SignalFour;//�����ĸ��ź�Դ���ݴ洢����
extern long CharToDecimal(char* srcData,double* dstData,int inlen,int digits, int baseFlag);
extern long ReadCount;//����������ݵĴ���
extern long ReNum; //����ѭ���������ݴ���
//extern bool  ReceiveFlag;//�����д���ݱ�־
extern int FileNum;//����д����ļ���
//////����������Ϣ����
typedef struct {
	int Sampling_depth;
	int Sampling_frequency;
	double Trigger_Level;
	int Trigger_Setting;
	int	Acquisition_Mode;
	bool Source_Channel[4];
	int port[10];
}IniInfo;
extern IniInfo* setInfo;//����������Ϣ����
extern bool RecvFlag;//�����������״̬����
///////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


