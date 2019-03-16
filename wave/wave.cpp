
// wave.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "wave.h"
#include "waveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SkinH.h"
#pragma comment(lib,"SkinHu.lib") //连接皮肤库文件
#include "Thnr.h"
#include "mclmcr.h"   
#include "matrix.h"
#pragma comment(lib,"Thnr.lib") //连接matlab库文件
#pragma comment(lib,"ws2_32.lib")

// CwaveApp

BEGIN_MESSAGE_MAP(CwaveApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CwaveApp 构造

CwaveApp::CwaveApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CwaveApp 对象

CwaveApp theApp;


// CwaveApp 初始化

BOOL CwaveApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//== = SkinH == =
	//SkinH_AttachEx(_T("res/enjoy.she"), NULL); //载入皮肤

	////matlab test
	//////////
	///////////
	//if (!MyAddInitialize())
	//{
	//	AfxMessageBox(_T("初始化失败"));
	//}
	//// 为变量分配内存空间，可以查帮助mwArray
	//mwArray mwX(5, 1, mxDOUBLE_CLASS); // 5，1表示矩阵的大小（所有maltab只有一种变量，就是矩阵，
	//mwArray mwY(5, 1, mxDOUBLE_CLASS);//为了和Cpp变量接轨，设置成1*5的矩阵，mxDOUBLE_CLASS表示变量的精度）
	//mwArray mwM(1, 1, mxDOUBLE_CLASS);
	//mwArray mwXR(50, 1, mxDOUBLE_CLASS);
	//mwArray mwYT(1, 1, mxDOUBLE_CLASS);
	//mwArray mwT(1, 1, mxDOUBLE_CLASS);
	//int x[5] = { 432,435,438,441,443 };    //样本点坐标
	//int y[5] = { 136,138,140,145,147 };    //
	//int xr[50], m = 3, t = 446;           //参数设置
	//for (int i = 400; i<450; i++)
	//{
	//	xr[i - 400] = i;
	//}
	//mwX.SetData(x, 5);      //调用类里面的SetData函数给类赋值
	//mwY.SetData(y, 5);
	//mwM.SetData(&m, 1);
	//mwXR.SetData(xr, 50);
	//mwT.SetData(&t, 1);
	//MyAdd(1, mwYT, mwX, mwY, mwM, mwXR, mwT);
	//double c = mwYT.Get(1, 1); //调用类里面的Get函数获取取函数返回值
	//CString strNumber;
	//strNumber.Format(_T("%lf坐标"), c);
	//AfxMessageBox(strNumber);
	//// 后面是一些终止调用的程序
	//MyAddTerminate();
	//// terminate MCR
	//mclTerminateApplication();
	//////////
	//matlab test

	CwaveDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
int CwaveApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
