#pragma once
#include "afxcmn.h"


// MyTest 对话框

class MyTest : public CDialogEx
{
	DECLARE_DYNAMIC(MyTest)

public:
	MyTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyTest();

	virtual void OnFinalRelease();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	// 列表控件
	CListCtrl m_FileList;
	virtual BOOL OnInitDialog();
};
