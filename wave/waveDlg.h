
// waveDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ChartCtrl/ChartCtrl.h"//引用hight-speed chart控件
#include "ChartCtrl/ChartLineSerie.h"//引用hight-speed chart控件
#include "afxwin.h"
#include ".\ChartCtrl\ChartCtrl.h"
#include"MyNewDlg.h"
#include"MyTest.h"
				   //
// CwaveDlg 对话框
class CwaveDlg : public CDialogEx
{
// 构造
public:
	CwaveDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CwaveDlg::LeftMoveArray(double* ptr, size_t length, double data);//数据左移
	CMyNewDlg *pDlg;
	MyTest *pTest;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 添加列表控件变量
	CListCtrl m_list;
	CComboBox Sampling_depth;
	CComboBox Sampling_frequency;
	// 触发设置
	CComboBox Trigger_setting;
	
	void creatMatlab(int filename);//
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOpen();
	//
	CChartCtrl m_ChartCtrl1;
	CChartCtrl m_ChartCtrl2;
	CChartCtrl m_ChartCtrl3;
	CChartCtrl m_ChartCtrl4;//定义四个绘制不同波形的图像控件
	CMFCTabCtrl m_tab;
	CChartLineSerie* m_pLineSerie1;
	CChartLineSerie* m_pLineSerie2;
	CChartLineSerie* m_pLineSerie3;
	CChartLineSerie* m_pLineSerie4;
	// 滑动条变量
	CSliderCtrl m_slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedAnalysis();
	afx_msg void OnBnClickedImport();
	// 定义采集模式设置变量
	int m_radio1;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	// 定义复选框变量
	BOOL m_check1;
	BOOL m_check2;
	BOOL m_check3;
	BOOL m_check4;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	double m_voltage;
	// 定义显示源选项
	int m_radio2;
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedImport2();
	int m_Sampling_depth;
	// 总谐波失真
	CString m_THD;
	// 无杂散动态范围
	CString m_SFDR;
	CString m_SN;
	// 有效位
	CString m_ENOB;
	// 信号-失真噪声比
	CString m_SNDR;
	long total_cap;
	double current_cap;
	afx_msg void OnBnClickedSet();
	CBrush m_brbk;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
