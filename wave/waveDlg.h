
// waveDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ChartCtrl/ChartCtrl.h"//����hight-speed chart�ؼ�
#include "ChartCtrl/ChartLineSerie.h"//����hight-speed chart�ؼ�
#include "afxwin.h"
#include ".\ChartCtrl\ChartCtrl.h"
#include"MyNewDlg.h"
#include"MyTest.h"
				   //
// CwaveDlg �Ի���
class CwaveDlg : public CDialogEx
{
// ����
public:
	CwaveDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void CwaveDlg::LeftMoveArray(double* ptr, size_t length, double data);//��������
	CMyNewDlg *pDlg;
	MyTest *pTest;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ����б�ؼ�����
	CListCtrl m_list;
	CComboBox Sampling_depth;
	CComboBox Sampling_frequency;
	// ��������
	CComboBox Trigger_setting;
	
	void creatMatlab(int filename);//
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOpen();
	//
	CChartCtrl m_ChartCtrl1;
	CChartCtrl m_ChartCtrl2;
	CChartCtrl m_ChartCtrl3;
	CChartCtrl m_ChartCtrl4;//�����ĸ����Ʋ�ͬ���ε�ͼ��ؼ�
	CMFCTabCtrl m_tab;
	CChartLineSerie* m_pLineSerie1;
	CChartLineSerie* m_pLineSerie2;
	CChartLineSerie* m_pLineSerie3;
	CChartLineSerie* m_pLineSerie4;
	// ����������
	CSliderCtrl m_slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedAnalysis();
	afx_msg void OnBnClickedImport();
	// ����ɼ�ģʽ���ñ���
	int m_radio1;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	// ���帴ѡ�����
	BOOL m_check1;
	BOOL m_check2;
	BOOL m_check3;
	BOOL m_check4;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	double m_voltage;
	// ������ʾԴѡ��
	int m_radio2;
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedImport2();
	int m_Sampling_depth;
	// ��г��ʧ��
	CString m_THD;
	// ����ɢ��̬��Χ
	CString m_SFDR;
	CString m_SN;
	// ��Чλ
	CString m_ENOB;
	// �ź�-ʧ��������
	CString m_SNDR;
	long total_cap;
	double current_cap;
	afx_msg void OnBnClickedSet();
	CBrush m_brbk;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
