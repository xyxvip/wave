// MyTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wave.h"
#include "MyTest.h"
#include "afxdialogex.h"


// MyTest �Ի���

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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IMyTest ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {FB004C5F-F1BA-4E93-99E3-4C5B32F37ADA}
static const IID IID_IMyTest =
{ 0xFB004C5F, 0xF1BA, 0x4E93, { 0x99, 0xE3, 0x4C, 0x5B, 0x32, 0xF3, 0x7A, 0xDA } };

BEGIN_INTERFACE_MAP(MyTest, CDialogEx)
	INTERFACE_PART(MyTest, IID_IMyTest, Dispatch)
END_INTERFACE_MAP()


// MyTest ��Ϣ�������


BOOL MyTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect1;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_FileList.GetClientRect(&rect1);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_FileList.SetExtendedStyle(m_FileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_FileList.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect1.Width() / 6, 0);
	m_FileList.InsertColumn(1, _T("����ʱ��"), LVCFMT_CENTER, rect1.Width() / 6, 1);
	m_FileList.InsertColumn(2, _T("�ź�Ƶ��"), LVCFMT_CENTER, rect1.Width() / 6, 2);
	m_FileList.InsertColumn(3, _T("�źŷ���"), LVCFMT_CENTER, rect1.Width() / 6, 3);
	m_FileList.InsertColumn(4, _T("�ź���λ"), LVCFMT_CENTER, rect1.Width() / 6, 4);
	m_FileList.InsertColumn(5, _T("�ɼ����"), LVCFMT_CENTER, rect1.Width() / 6, 5);
	m_FileList.InsertItem(0, _T("go"));

	m_FileList.InsertItem(0, _T("go"));
	m_FileList.SetItemText(0, 1, _T("go"));
	m_FileList.SetItemText(0, 2, _T("go"));
	m_FileList.SetItemText(0, 3, _T("go"));
	m_FileList.SetItemText(0, 4, _T("go"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
