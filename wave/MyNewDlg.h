#pragma once
#include "afxdialogex.h"
#include "afxcmn.h"
#include "afxwin.h"
#include"resource.h"
extern bool volatile  ReceiveFlag;//定义读写数据标志
//#include "engine.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeng.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")//start matlab

class CMyNewDlg :
	public CDialogEx
{
public:
	CMyNewDlg(CWnd* pParent = NULL);
	~CMyNewDlg();
	//客户端：
	SOCKET toServer;//连接至服务器端的套接字
	bool connected;//指示连接状态
	void PrintData(char* title, unsigned char* buffer, int length);

	enum { IDD = IDD_SOCKETCLIENT};
	CString	m_chat;
	CString	m_ip;
	CString	m_message;
	UINT	m_port;
	HICON m_hIcon;
	//Engine* pEng;//matlab enging
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnBnClickedSend();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend3();
	afx_msg void OnBnClickedReset();
	int m_port1;
	int m_port2;
	int m_port3;
	int m_port4;
	int m_port5;
	int m_port6;
	int m_port7;
	int m_port8;
	int m_port9;
	int m_port10;
	CString m_dstID;
	CString m_start;
	CString m_len;
	CString addrh;
	CString addrl;
};

