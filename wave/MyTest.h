#pragma once
#include "afxcmn.h"


// MyTest �Ի���

class MyTest : public CDialogEx
{
	DECLARE_DYNAMIC(MyTest)

public:
	MyTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyTest();

	virtual void OnFinalRelease();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	// �б�ؼ�
	CListCtrl m_FileList;
	virtual BOOL OnInitDialog();
};
