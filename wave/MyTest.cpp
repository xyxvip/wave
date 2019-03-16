// MyTest.cpp : 实现文件
//

#include "stdafx.h"
#include "wave.h"
#include "MyTest.h"
#include "afxdialogex.h"


// MyTest 对话框

IMPLEMENT_DYNAMIC(MyTest, CDialogEx)

MyTest::MyTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST, pParent)
{

	EnableAutomation();

}

MyTest::~MyTest()
{
}

void MyTest::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void MyTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FileList);
}


BEGIN_MESSAGE_MAP(MyTest, CDialogEx)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(MyTest, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IMyTest 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {FB004C5F-F1BA-4E93-99E3-4C5B32F37ADA}
static const IID IID_IMyTest =
{ 0xFB004C5F, 0xF1BA, 0x4E93, { 0x99, 0xE3, 0x4C, 0x5B, 0x32, 0xF3, 0x7A, 0xDA } };

BEGIN_INTERFACE_MAP(MyTest, CDialogEx)
	INTERFACE_PART(MyTest, IID_IMyTest, Dispatch)
END_INTERFACE_MAP()


// MyTest 消息处理程序


BOOL MyTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect1;

	// 获取编程语言列表视图控件的位置和大小   
	m_FileList.GetClientRect(&rect1);

	// 为列表视图控件添加全行选中和栅格风格   
	m_FileList.SetExtendedStyle(m_FileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_FileList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rect1.Width() / 6, 0);
	m_FileList.InsertColumn(1, _T("分析时间"), LVCFMT_CENTER, rect1.Width() / 6, 1);
	m_FileList.InsertColumn(2, _T("信号频率"), LVCFMT_CENTER, rect1.Width() / 6, 2);
	m_FileList.InsertColumn(3, _T("信号幅度"), LVCFMT_CENTER, rect1.Width() / 6, 3);
	m_FileList.InsertColumn(4, _T("信号相位"), LVCFMT_CENTER, rect1.Width() / 6, 4);
	m_FileList.InsertColumn(5, _T("采集深度"), LVCFMT_CENTER, rect1.Width() / 6, 5);
	m_FileList.InsertItem(0, _T("go"));

	m_FileList.InsertItem(0, _T("go"));
	m_FileList.SetItemText(0, 1, _T("go"));
	m_FileList.SetItemText(0, 2, _T("go"));
	m_FileList.SetItemText(0, 3, _T("go"));
	m_FileList.SetItemText(0, 4, _T("go"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
