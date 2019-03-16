
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
////////////////定义需要使用的全局变量
#define DATA_K 1024
#define DATA_8K 1024*8
#define DATA_32K 1024*32
#define DATA_64K 1024*64
#define DATA_M 1024*1024
#define DATA_4M DATA_M*4
#define MAX_NUM DATA_M*16
#define TOTAL_DATA DATA_4M*5
#define PI 3.141592654
#define FM  1048576//定义采样频率单位Mhz
#define NFFT  8192// FFT所用的点数NFFT
extern long signal_recvNum[4];//存储接收到的各通道数据长度
extern char* RecvBuffer;//存储接收到的数据
extern double* Analysisdata;//用于存储十进制处理数据
extern double* SignalOne;
extern double* SignalTwo;
extern double* SignalThree;
extern double* SignalFour;//定义四个信号源数据存储变量
extern long CharToDecimal(char* srcData,double* dstData,int inlen,int digits, int baseFlag);
extern long ReadCount;//定义读入数据的次数
extern long ReNum; //定义循环接收数据次数
//extern bool  ReceiveFlag;//定义读写数据标志
extern int FileNum;//定义写入的文件数
//////定义配置信息变量
typedef struct {
	int Sampling_depth;
	int Sampling_frequency;
	double Trigger_Level;
	int Trigger_Setting;
	int	Acquisition_Mode;
	bool Source_Channel[4];
	int port[10];
}IniInfo;
extern IniInfo* setInfo;//声明配置信息变量
extern bool RecvFlag;//定义加载配置状态变量
///////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


