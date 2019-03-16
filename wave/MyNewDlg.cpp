#include "stdafx.h"
#include "MyNewDlg.h"
#include"resource.h"
#include <WinSock2.h> 
#include <WS2tcpip.h> 
#include "waveDlg.h"
#include <windows.h>

extern long signal_recvNum[4];
extern double* Analysisdata;//���ڴ洢ʮ���ƴ�������
extern double* SignalOne ;
extern double* SignalTwo;
extern double* SignalThree;
extern double* SignalFour ;//
extern IniInfo* setInfo;//���ڴ洢������Ϣ
bool volatile  ReceiveFlag = TRUE;//�����жϴ��������Ƿ��д
char* TempBuffer;//�洢���յ�������
extern char* RecvBuffer;//�洢���յ�������
char SetBuffer[500];//�洢�·�������Ϣ����
int pSetInfo[100];//�洢�������������Ϣ����
char pSetChar[500];//�洢���յ�������Ϣ�ַ�����
int SetLenth;
BOOL data_ready_flag = TRUE;
BOOL SetFlag = FALSE;
BOOL SendFlag = FALSE;
BOOL UploadFlag = FALSE;
BOOL ResetFlag = FALSE;
extern bool RecvFlag = FALSE;
extern int FileNum = 0;

UINT Wait4Client(LPVOID pParam);
UINT ReceiveMessage(LPVOID pParam);
char Read_Bit(char c, int pos);
int dataEncap(char* pBuff, int recvLen);//�������ݷ�װ
int dataUnEncap(char* pBuff, int recvLen);//�������ݽ���
extern int samplingData(char* pBuff, int recvLen);//�ɼ����ݽ���
extern int ByteToBit(char* pBuff, int recvLen, char *dstBuff);//�ֽ�ת���ɰ�λ�����ַ�

/////////д�����ݵ�ָ���ļ�
extern BOOL WriteFile(int wrNum, double* wrData, int wrLenth)
{
	CStdioFile myFile;
	CFileException fileException;
	int LineNum = 0;
	CString strOrder, strValue;
	if (wrData == NULL)
		return FALSE;
	strOrder.Format(_T("E:\\RECV\\myflie%d.txt"), wrNum);
	if (myFile.Open(strOrder, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite), &fileException)
	{
		for (int i = 0; i < wrLenth; i++)
		{
			//if ((i % 10))
			//{
				strValue.Format(_T("%.1f "), wrData[i]);
			//}
			//else
			//{
				//LineNum++;
				////	myFile.SeekToEnd();
				//if (LineNum>1)
				//	strValue.Format(_T("\r\nThe %d Line :\r\n"), LineNum);
				//else
				//	strValue.Format(_T("The %d Line :\r\n"), LineNum);
			//}
			myFile.WriteString(strValue);
		}
		return TRUE;
	}
	else
	{
		TRACE(_T("Can't open file %s,error=%u\n"), strOrder, fileException.m_cause);
		return FALSE;
	}
}
/////////////
extern BOOL WriteChar(int wrNum, char* wrData, int wrLenth)
{
	CStdioFile myFile;
	CFileException fileException;
	int LineNum = 0;
	CString strOrder, strValue;
	if (wrData == NULL)
		return FALSE;
	strOrder.Format(_T("E:\\RECV\\mychar%d.txt"), wrNum);
	if (myFile.Open(strOrder, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite), &fileException)
	{
		for (int i = 0; i < wrLenth; i++)
		{
			//if ((i % 10))
			//{
			if(i%2)
				strValue.Format(_T("%c"), wrData[i]);
			else
				strValue.Format(_T("%c"), wrData[i]);
			//}
			//else
			//{
			//LineNum++;
			////	myFile.SeekToEnd();
			//if (LineNum>1)
			//	strValue.Format(_T("\r\nThe %d Line :\r\n"), LineNum);
			//else
			//	strValue.Format(_T("The %d Line :\r\n"), LineNum);
			//}
			myFile.WriteString(strValue);
		}
		return TRUE;
	}
	else
	{
		TRACE(_T("Can't open file %s,error=%u\n"), strOrder, fileException.m_cause);
		return FALSE;
	}
}
//////////��λ��ȡ����
char Read_Bit(char c, int pos)
{
	int   b_mask = 0x01;
	b_mask = b_mask << (7 - pos);//31
	if ((c&b_mask) == b_mask)             //�ַ�c��b_mask��λ����������ǵ���b_mask,˵����λΪ1 
		return   '1';
	else
		return  '0';
}
//////////ʮ����ת8λ16����
void tran(int num,char * pbuff)
{
	int arr[8], i;
	for (i = 0; i <8; i++)
	{
		arr[i] = num%16;
		num = num / 16;
		if (num == 0)
		{
			num = i;
			break;
		}		
	}
	for (i=num; i >= 0; i--)
	{
		switch (arr[i])
		{
		case 10: pbuff[7-i]='A'; break;
		case 11: pbuff[7-i] = 'B'; break;
		case 12: pbuff[7-i] = 'C'; break;
		case 13: pbuff[7-i] = 'D'; break;
		case 14: pbuff[7-i] = 'E'; break;
		case 15: pbuff[7-i] = 'F'; break;
		default: pbuff[7-i] = arr[i] +48;
		}
	}
	for (i=num; i< 7; i++)
	{
		pbuff[6 - i] = '0';
	}
}
////////////////ʮ����ת2λ16����
void tranChar(int num, char * pbuff, int bits)//
{
	int arr[8], i;
	for (i = 0; i <bits; i++)
	{
		arr[i] = num % 16;
		num = num / 16;
		if (num == 0)
		{
			num = i;
			break;
		}
	}
	for (i = num; i >= 0; i--)
	{
		switch (arr[i])
		{
		case 10: pbuff[1 - i] = 'A'; break;
		case 11: pbuff[1 - i] = 'B'; break;
		case 12: pbuff[1 - i] = 'C'; break;
		case 13: pbuff[1 - i] = 'D'; break;
		case 14: pbuff[1 - i] = 'E'; break;
		case 15: pbuff[1 - i] = 'F'; break;
		default: pbuff[1 - i] = arr[i] + 48;
		}
	}
	for (i = num; i< bits - 1; i++)// 
	{
		pbuff[bits - 1-1] = '0';
	}
}
////////////////
CMyNewDlg::CMyNewDlg(CWnd* pParent /*=NULL*/) 
	:CDialogEx(CMyNewDlg::IDD, pParent),
	connected(false)
	, m_port1(0)
	, m_port2(0)
	, m_port3(0)
	, m_port4(0)
	, m_port5(0)
	, m_port6(0)
	, m_port7(0)
	, m_port8(0)
	, m_port9(0)
	, m_port10(0)
	, m_dstID(_T(""))
	, m_start(_T(""))
	, m_len(_T(""))
	, addrh(_T(""))
	, addrl(_T(""))
{
	TempBuffer = new char[TOTAL_DATA];
	m_chat = _T("Please Click the Button \"Connect\" to Connect to the SocketServer...\r\n");
	m_ip = _T("192.168.0.1");
	m_message = _T("");
	m_port = 6000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int error;
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 1);
	if (error = WSAStartup(wVersionRequested, &wsaData))
	{
		MessageBox(_T("Link Socket Library Failed!"));
		exit(0);
	}
	//start matlab Engine 
	/*pEng = NULL;

	if (!(pEng = engOpen(NULL)))
	{
		MessageBox(_T("Open matlab enging fail!"));
		getchar();
		exit(0);
	}*/
}


CMyNewDlg::~CMyNewDlg()
{
//	if (pEng)
		//engClose(pEng);
	delete[] TempBuffer;
	TempBuffer = NULL;
	delete[] RecvBuffer;
	RecvBuffer = NULL;
	WSACleanup();
}


BOOL CMyNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//matlab enging example
	/*engEvalString(pEng, "x=0:0.02:3.14; y=sin(x); plot(x, y)");
	mxArray *x = engGetVariable(pEng, "x");
	mxArray *y = engGetVariable(pEng, "y");
	double * _x = mxGetPr(x);
	double * _y = mxGetPr(y);*/
	//��ʱ_xΪx���飬_yΪy����
	//

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

									// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}
BEGIN_MESSAGE_MAP(CMyNewDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDDOWNLOAD, &CMyNewDlg::OnBnClickedSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCONNECT, &CMyNewDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDUPLOAD, &CMyNewDlg::OnBnClickedSend3)
	ON_BN_CLICKED(IDRESET, &CMyNewDlg::OnBnClickedReset)
END_MESSAGE_MAP()
///////////////////��ͨ��Э���������������������
int dataUnEncap(char* pBuff, int recvLen)
{
	int doulfag = 0;
	int nCount = 0;
	int sum = 0;
	int temp = 0;
	bool flag = false;
	int bits = 0;
	for (int i = 1; i < recvLen; i++)
	{
		if (isdigit(pBuff[i]))
		{
			temp = pBuff[i] - 48;
			if ((temp > 7) && (i == 0))//�жϷ���λ��ֵ�Ƿ�Ϊ1
			{
				flag = true;
				//temp -= 8;
			}
			// �ܹ�8λ��һ��16����λ�� 4 bit����  
			// ��һ�Σ�'1'Ϊ���λ������temp���� (len - i -1) * 4  λ  
			// �ڶ��Σ�'d'Ϊ�θ�λ������temp���� (len - i -1) * 4  λ  
			// �����Σ�'e'Ϊ���λ������temp���� (len - i -1) * 4 λ
			doulfag++;
			bits = (8 - doulfag) * 4;
			if (doulfag > 4)
			{
				temp = temp << bits;
				sum += temp;
			}
		}
		else if (pBuff[i] < 'A' || (pBuff[i] > 'F' && pBuff[i] < 'a') || pBuff[i] > 'f')
		{
			doulfag = 0;
			sum = 0;
			continue;;
		}
		else if (isalpha(pBuff[i]))
		{
			if (isupper(pBuff[i]))
				temp = pBuff[i] - 55;
			else
				temp = pBuff[i] - 87;
			doulfag++;
			bits = (8 - doulfag) * 4;
			if (doulfag > 4)//ȥ��ǰ���ĸ�ʮ������λ�ĵ�ֵַ��ȡ����ֵ
			{
				temp = temp << bits;
				sum += temp;
			}
		}
		else if(pBuff[i]='\0')
		{
			doulfag = 0;
			sum = 0;
			break;
		}
		if (doulfag == 8)
		{
			doulfag = 0;
			pSetInfo[nCount] = sum;
			sum = 0;
			nCount++;
		}
	}
	return nCount;
}
///////////////////��ͨ��Э���װ���ݵ���ͬ�Ĵ�����ַ
int dataEncap(char* pBuff, int recvLen)
{
	int BKAddr, RegAddr;
	int RecData = 0;
	int temp[8];
	//char bitRead[32];
	int ByteCount = 0;
	int SecCount = 0;
	BOOL SecFlag = TRUE;
	int i, j;
	for (i = 0; i<recvLen; i++)
	{
		if (pBuff[i] == '\n')
		{
			SecFlag = FALSE;
			switch (SecCount)
			{
			case 0: BKAddr = 0x01; RegAddr = 0x02; break;
			case 1: BKAddr = 0x02; RegAddr = 0x04; break;
			case 2: BKAddr = 0x03; RegAddr = 0x06; break;
			case 3: BKAddr = 0x04; RegAddr = 0x82; break;
			case 4: BKAddr = 0x04; RegAddr = 0x84; break;
			}

			for (j = 0; j<ByteCount; j++)
			{
				RecData = RecData + temp[j] * (int)pow(10, ByteCount - j - 1);
			}
			pSetInfo[SecCount] = (int)(((BKAddr & 0xFF) << 24)
				| ((RegAddr & 0xFF) << 16)
				| (RecData & 0x0000FFFF));
			/*for (j = 0; j<32; j++)
			{
				bitRead[j] = Read_Bit(pSetInfo[SecCount], j) + 48;//ת����32λ�����Ƹ�ʽ
				printf("%c", bitRead[j]);
			}*/
			tran(pSetInfo[SecCount], pSetChar+ SecCount*8);//ת���ɰ�λʮ�����Ƹ�ʽ
			//memcpy(pSetChar+(SecCount*32), bitRead,32);
			SecCount++;
			ByteCount = 0;
		}
		else
		{
			temp[ByteCount] = pBuff[i] - 48;
			ByteCount++;
			RecData = 0;
		}
		
	}
	return SecCount;
}
/////////////�ֽ�ת���ɰ�λ�����ַ�
int ByteToBit(char* pBuff, int recvLen,char *dstBuff)
{
	for (int i = 0; i < recvLen; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			dstBuff[i * 8 + j] = Read_Bit(pBuff[i],j);
		}
	}
	return 1;
}
//////////////////////�ɼ����ݽ���
int samplingData(char* pBuff, int recvLen)
{
	//int i;
	int nCount = recvLen / 64;
	int nPro = 0;
	int chNum = 0;
	int rCount = 0;
	int nPro0, nPro1, nPro2, nPro3;
	nPro0 = nPro1 = nPro2 = nPro3 = 0;
	while (nPro < nCount)
	{
	
		if (pBuff[nPro * 64] == '1')
			chNum += 2;
		if (pBuff[nPro * 64 +1] == '1')
			chNum += 1;
		switch (chNum)
		{
		case 0:signal_recvNum[0] += CharToDecimal(pBuff + nPro * 64 + 4, SignalOne + nPro0 * 5, 60, 12, 2); nPro0++; break;
		case 1:signal_recvNum[1]+= CharToDecimal(pBuff + nPro * 64 + 4, SignalTwo + nPro1 * 5, 60, 12, 2); nPro1++; break;
		case 2:signal_recvNum[2]+= CharToDecimal(pBuff + nPro * 64 + 4, SignalThree + nPro2 * 5, 60, 12, 2); nPro2++; break;
		case 3:signal_recvNum[3]+= CharToDecimal(pBuff + nPro * 64 + 4, SignalFour + nPro3 * 5, 60, 12, 2); nPro3++; break;
		}
		rCount += signal_recvNum[0];//��ͨ���յ������ݺ�
		chNum = 0;
		nPro++;
	}
	return rCount;
}

long ReNum = 0;
long ReCount = 0;
bool cleanflag = TRUE;
UINT ReceiveMessage(LPVOID pParam)
{
	CMyNewDlg * c = (CMyNewDlg *)pParam;
	int error;//��¼recv��������ֵ�������յ��ֽ�����Ҳ���쳣����
	int count = 0;
	CString str;
	int repeatNum = 0;
	//char Downflag[2];
	unsigned long total_recv = 0;
	double t1,t2;
	while ((error = recv(c->toServer, TempBuffer, TOTAL_DATA, 0)) != ERROR)//ÿ�������յ�������TOTAL_DATA
	{
		if (error == 0 || error == SOCKET_ERROR)
			break;
		if ((TempBuffer[0] == 'w') && (error<10))
		{
			AfxMessageBox(_T("The doorbell is not ready!"));
		}
		else if ((TempBuffer[0] == 'u') && (error<10))
		{
			dataUnEncap(TempBuffer, error);
			for (int i = 0; i < SetLenth; i++)
			{
				str.Format(_T("The %d section is:%d   "), i, pSetInfo[i]);
				c->m_chat += str;
			}
			RecvFlag = TRUE;
			//c->OnBnClickedUpload();
			//c->PostMessage(WM_UPDATE_MESSAGE, 0, 0);
		}
		else if ((TempBuffer[0] == 'e')  && (error<10))
		{
			for (int i = 0; i < error; i++)
			{
				str.Format(_T("%d "), TempBuffer[i]);
				c->m_chat += str;
			}
		}
		else //if(error>500)
		{
			
			if (total_recv == 0)
				t1 = GetTickCount();
			////////////////
			/*int test;
			for (int i = 0; i < error; i++)
			{
				test = TempBuffer[i];
				tranChar(test, RecvBuffer + 2 * total_recv + 2 * i);
			}*/
			memcpy(RecvBuffer + total_recv, TempBuffer, error);
			total_recv += error;
			if (total_recv >= MAX_NUM)
			{
				t2 = GetTickCount();
				str.Format(_T("The receive time is %f ms!!!!!\r\n"), (t2 - t1));
				c->UpdateData(false);
				//WriteChar(FileNum, RecvBuffer, DATA_M);//д��ʮ�������ַ���ʽ����
				memcpy(TempBuffer,RecvBuffer , DATA_M);
				ByteToBit(TempBuffer, DATA_M, RecvBuffer);//��λת�����ַ�
				count = samplingData(RecvBuffer, DATA_M*4 );//��ͨ���Ž������ݵ�����ͨ��
				WriteFile(FileNum, SignalOne, DATA_32K);
				//ReCount += count;
				//if (ReCount >= DATA_32K)
				//{
					//ReCount = 0;
					ReceiveFlag = FALSE;
				//}

				//c->WriteChar(FileNum, RecvBuffer,total_recv);
				//ByteToBit(RecvBuffer, error, TempBuffer);
				//count = samplingData(TempBuffer, error * 8);
				FileNum++;
				//data_ready_flag = TRUE;
				total_recv = 0;
				c->m_chat += str;
			}	//���ݽ������ˣ��������½�������
			/*else
			{
				char Downflag[2];
				Downflag[0] = 't';
				Downflag[1] = '\0';
				send(c->toServer, Downflag, 1, 0);
			}*///ģ��ѭ����������
			///////////
			//int test;
			//for (int i = 0; i < error; i++)
			//{
			//	test = TempBuffer[i];
			//	tranChar(test, RecvBuffer + 2 * total_recv + 2 * i);
			//}
			////memcpy(RecvBuffer + total_recv, TempBuffer, error);
			//total_recv += error;
			//if (total_recv >= unsigned long(1024 * 1024 * 16))
			//{
			//	
			//	t2 = GetTickCount();
			//	str.Format(_T("The receive time is %f ms!!!!!\r\n"), (t2 - t1));
			//	c->UpdateData(false);
			//	WriteChar(FileNum, RecvBuffer, 2 * total_recv);//д��ʮ�������ַ���ʽ����
			//	count = CharToDecimal(RecvBuffer, Analysisdata, DATA_8K, 1, 8);
			//	WriteFile(FileNum, Analysisdata, count);
			//	ReCount += count;
			//	if (ReCount >= DATA_8K)
			//	{
			//		ReCount = 0;
			//		ReceiveFlag = FALSE;
			//	}
			//	
			//	//c->WriteChar(FileNum, RecvBuffer,total_recv);
			//	//ByteToBit(RecvBuffer, error, TempBuffer);
			//	//count = samplingData(TempBuffer, error * 8);
			//	FileNum++;
			//	//data_ready_flag = TRUE;
			//	total_recv = 0;
			//	c->m_chat += str;
			//}	//���ݽ������ˣ��������½�������
			
		}
		str.Format(_T("The Number of %d times message is:%d\r\n "), ReNum, error);
		c->m_chat += str;
		//c->m_chat += TempBuffer;
		//c->m_chat += "\r\n";
		c->UpdateData(false);
		//ReNum++;
		
		//else if (error>500)
		//{
		//	
		//	repeatNum = error / (MAX_NUM);
		//	if (repeatNum > 0)
		//	{
		//		while (repeatNum>0)
		//		{
		//			if (ReceiveFlag)
		//			{
		//				//ByteToBit(RecvBuffer, MAX_NUM, TempBuffer);
		//				//count = samplingData(TempBuffer, MAX_NUM*8);
		//				count = CharToDecimal(TempBuffer, Analysisdata + ReCount, MAX_NUM, 1, 8);//count��ֵ����Խ��
		//				ReCount += count;
		//				if ((ReCount + count) >= MAX_NUM)//�ж��´ζ�������ת�����ʮ����ֵ�Ƿ�ᳬ����Χ
		//				{
		//					//c->WriteFile(FileNum, Analysisdata, MAX_NUM);
		//					//ReNum = MAX_NUM / NFFT;
		//					ReceiveFlag = FALSE;
		//					ReCount = 0;
		//				}
		//				repeatNum--;
		//			}
		//		}
		//		/*while (ReNum>0)
		//		{
		//		ReceiveFlag = FALSE;
		//		if (SendFlag|| UploadFlag||ResetFlag||ExportFlag)
		//		break;
		//		}*/
		//	}
		//	else
		//	{
		//		
		//		if (ReceiveFlag)
		//		{
		//			//ByteToBit(RecvBuffer, error, TempBuffer);
		//			//count = samplingData(TempBuffer, error * 8);
		//			count = CharToDecimal(TempBuffer, Analysisdata + ReCount, error, 1, 8);
		//			ReCount += count;
		//			if ((ReCount + count) > MAX_NUM)//�ж��´ζ�������ת�����ʮ����ֵ�Ƿ�ᳬ����Χ
		//			{
		//				//c->WriteFile(FileNum, Analysisdata, MAX_NUM);
		//				ReceiveFlag = FALSE;
		//				ReCount = 0;
		//			}
		//		}
		//	}
			if (FileNum > 1000)
				FileNum = 0;
		//}
		//c->m_chat += "Client:";
		//c->m_chat += TempBuffer;
		//c->m_chat += "\r\n";
		//c->UpdateData(false);
		//Sleep(100);
		//if (SendFlag)
		//{
		//	send(c->toServer, pSetChar, SetLenth * 8, 0);//����������Ϣ
		//												 //ReceiveFlag = TRUE;//���������½��ղɼ�����
		//	SendFlag = FALSE;
		//}
		//else if (ExportFlag)
		//{
		//	Downflag[0] = 'e';
		//	Downflag[1] = '\0';
		//	send(c->toServer, Downflag, 1, 0);//��������״̬
		//									  //ReceiveFlag = TRUE;//���������½��ղɼ�����
		//	ExportFlag = FALSE;
		//}
		//else
		//{
		//	Downflag[0] = 't';
		//	Downflag[1] = '\0';
		//	if (data_ready_flag)
		//	{
		//		send(c->toServer, Downflag, 1, 0);
		//		data_ready_flag = FALSE;
		//	}		
		//	//c->m_chat += "send t to server!";
		//}
		c->m_chat.Empty();
	}
	if (!c->connected)return 0;//�ͻ��������رգ�ֱ�ӷ���
	closesocket(c->toServer);
	c->connected = false;
	c->m_chat += "Server Disconnected...\r\n";
	c->m_chat.Empty();
	c->UpdateData(false);
	return 0;
}

//�����Ļ�������16��������Ա�����ʾ
void CMyNewDlg::PrintData(char* title, unsigned char* buffer, int length)
{
	int i;
	CString temp("");
	m_chat += "(";
	m_chat += title;
	m_chat += ":";
	CString str;
	for (i = 0; i<length; i++)
	{
		str = *(buffer + i)>15 ? "" : "0";
		temp.Format(_T("%s%X "),str, *(buffer + i));
		m_chat += temp;
	}
	m_chat += ")\r\n";
}

void CMyNewDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	DDX_Text(pDX, IDC_EDITCONNECT, m_chat);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	//DDX_Text(pDX, IDC_EDITMESSAGE, m_message);
	//DDV_MaxChars(pDX, m_message, 1000);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 1024, 65535);
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_port1);
	DDX_Text(pDX, IDC_EDIT16, m_port2);
	DDX_Text(pDX, IDC_EDIT15, m_port3);
	DDX_Text(pDX, IDC_EDIT17, m_port4);
	DDX_Text(pDX, IDC_EDIT18, m_port5);
	DDX_Text(pDX, IDC_EDIT19, m_port6);
	DDX_Text(pDX, IDC_EDIT20, m_port7);
	DDX_Text(pDX, IDC_EDIT21, m_port8);
	DDX_Text(pDX, IDC_EDIT22, m_port9);
	DDX_Text(pDX, IDC_EDIT23, m_port10);
	DDX_Text(pDX, IDC_EDIT24, m_dstID);
	DDX_Text(pDX, IDC_EDIT27, m_start);
	DDX_Text(pDX, IDC_EDIT26, m_len);
	DDX_Text(pDX, IDC_EDIT25, addrh);
	DDX_Text(pDX, IDC_EDIT28, addrl);
}

void CMyNewDlg::OnBnClickedSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!connected)return;
	memset(SetBuffer,0,500);
	m_chat.Empty();
	m_message.Empty();
	UpdateData(true);
	////////////////�·�������Ϣ
	CString str;
	str.Format(_T("%d "), setInfo->Sampling_depth);
	m_message += str;
	str.Format(_T("%d "), setInfo->Sampling_frequency);
	m_message += str;
	str.Format(_T("%d "), setInfo->Trigger_Level*100);
	m_message += str;
	str.Format(_T("%d "), setInfo->Trigger_Setting);
	m_message += str;
	str.Format(_T("%d "), setInfo->Acquisition_Mode);
	m_message += str;
	
	for (int i = 0; i < 4; i++)
	{
		str.Format(_T("%d "),setInfo->Source_Channel[i]);
		m_message += str;
	}
	
	///////////////�·�������Ϣ
	///////////////�·��˿ں�
	m_message += 'p';
	setInfo->port[0] = m_port1;
	setInfo->port[1] = m_port2;
	setInfo->port[2] = m_port3;
	setInfo->port[3] = m_port4;
	setInfo->port[4] = m_port5;
	setInfo->port[5] = m_port6;
	setInfo->port[6] = m_port7;
	setInfo->port[7] = m_port8;
	setInfo->port[8] = m_port9;
	setInfo->port[9] = m_port10;
	for (int i = 0; i < 10; i++)
	{
		tranChar(setInfo->port[i], pSetChar + 2 * i,2);
		//str.Format(_T("%d"), setInfo->port[i]);
		//m_message += str;
	}
	for (int i = 0; i < 20; i++)
	{
		str.Format(_T("%c"), pSetChar[i]);
		m_message += str;
	}
	///////////////�·��ɼ���ַ
	m_message += "u";
	m_message += m_dstID;
	m_message += m_start;
	m_message += m_len;
	m_message += addrh;
	m_message += addrl;
	//char buffer[1024];
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	SetLenth = WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), NULL, 0, NULL, NULL);
	if (SetLenth >= 1024)
		SetLenth = 1024;
	//lenth = lenth % 16 ? lenth + 16 - lenth % 16 : lenth;
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), SetBuffer, SetLenth, NULL, NULL);
	m_chat = m_message;
	//SetLenth = dataEncap(SetBuffer, SetLenth);//��Э���װ����
	m_message.ReleaseBuffer();
	UpdateData(false);
	/*if (SetFlag)
	{
		SendFlag = TRUE;
		return;
	}*/
	//PrintData("Input Data", (unsigned char*)buffer, lenth);
	//char setflag[2];
	//setflag[0] = 's';
	//setflag[1] = '\0';
	//if (send(toServer, setflag, 1, 0) == SOCKET_ERROR)//��������״̬
	//{
	//	m_chat += "SetInfo DownLoad Failed!(Socket Exception?)\r\n";
	//	UpdateData(false);
	//	return;
	//}
	//if (send(toServer, pSetChar, SetLenth*8, 0) == SOCKET_ERROR)//����������Ϣ
	if (send(toServer, SetBuffer, SetLenth , 0) == SOCKET_ERROR)//����������Ϣ
	{//�쳣����
		m_chat += "Send Failed!(Socket Exception?)\r\n";
		UpdateData(false);
		return;
	}
	//m_chat += _T("The Message Of Client Received:\r\n");
	m_message = "";
	SetFlag = TRUE;
	UpdateData(false);
}


void CMyNewDlg::OnDestroy()
{
	/*if (RecvBuffer != NULL)
	{
		delete[] RecvBuffer;
		RecvBuffer = NULL;
	}*/
	CDialogEx::OnDestroy();
	delete this;
	// TODO: �ڴ˴������Ϣ����������
}


void CMyNewDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_message.Empty();

	////////
	//char test;
	//for (int i = 0; i < 256; i++)
	//{
	//	tranChar(i, RecvBuffer+2*i);
	//}

	//m_chat += RecvBuffer;
	////RecvBuffer[0] = 127;
	//UpdateData(false);
	//ByteToBit(RecvBuffer,128,TempBuffer);
	//samplingData(TempBuffer, 128 * 8);
	//////////
	char Downflag[2];
	Downflag[0] = 't';
	Downflag[1] = '\0';
	if (connected)//��������ӣ���Ͽ�
	{
		send(toServer, Downflag, 1, 0);
		//data_ready_flag = FALSE;
		/*connected = false;
		closesocket(toServer);
		m_chat += "Disconnect to Server!\r\n";
		UpdateData(false);*/
		return;
	}

	UpdateData(true);
	//����Socket
	struct protoent *ppe;
	ppe = getprotobyname("tcp");
	if ((toServer = socket(PF_INET, SOCK_STREAM, ppe->p_proto)) == INVALID_SOCKET)
	{
		m_chat += "Initialize Socket Listener Failed!\r\n";
		UpdateData(false);
		return;
	}

	//�������ӷ�����
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(m_port);
	char sendBuf[20] = "127.0.0.1";
	int length = m_ip.GetLength();
	int lenth = WideCharToMultiByte(CP_ACP, 0, m_ip, m_ip.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, m_ip, m_ip.GetLength(), sendBuf, lenth, NULL, NULL);//�������ip����sendBuf��
	//sendBuf[lenth + 1] = '/0';   //���ֽ��ַ���'/0'����
	inet_pton(AF_INET, sendBuf, &saddr.sin_addr);
	//saddr.sin_addr.s_addr = inet_pton(AF_INET,sendBuf, &saddr.sin_addr);
	m_chat += inet_ntop(AF_INET, (void*)&saddr.sin_addr, sendBuf, 16);
	if (connect(toServer, (struct sockaddr *)&saddr, sizeof(saddr)))
	{
		int net = WSAGetLastError();
		m_chat += "Connect Failed!\r\n";
		UpdateData(false);
		return;
	}
	m_chat += "Server: ";
	m_chat += inet_ntop(AF_INET, (void*)&saddr.sin_addr, sendBuf, 16);
	//m_chat += inet_ntoa(saddr.sin_addr);
	m_chat += " Connected!\r\n";
	connected = true;
	UpdateData(false);
	send(toServer, Downflag, 1, 0);
	AfxBeginThread(ReceiveMessage, this);//���ӽ����������߳����ڽ�����Ϣ
}


void CMyNewDlg::OnBnClickedSend3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//char Downflag[2];
	//UploadFlag = TRUE;

	//Downflag[0] = 'u';
	//Downflag[1] = '\0';
	//send(toServer, Downflag, 1, 0);//��������״̬
	//UploadFlag = FALSE;

	m_message.Empty();
	UpdateData(true);
	///////////////�·��˿ں�
	m_message = "u";
	m_message += m_dstID;
	m_message += m_start;
	m_message += m_len;
	m_message += addrh;
	m_message += addrl;

	///////////////�·�������Ϣ
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	SetLenth = WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), NULL, 0, NULL, NULL);
	if (SetLenth >= 1024)
		SetLenth = 1024;
	//lenth = lenth % 16 ? lenth + 16 - lenth % 16 : lenth;
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), SetBuffer, SetLenth, NULL, NULL);
	//SetLenth = dataEncap(SetBuffer, SetLenth);//��Э���װ����
	m_message.ReleaseBuffer();

	if (send(toServer, SetBuffer, SetLenth, 0) == SOCKET_ERROR)//����������Ϣ
	{//�쳣����
		m_chat += "Send Failed!(Socket Exception?)\r\n";
		UpdateData(false);
		return;
	}

}


void CMyNewDlg::OnBnClickedReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_message.Empty();
	UpdateData(true);
	///////////////�·��˿ں�
	pSetChar[0] = 'p';
	CString str;
	setInfo->port[0] = m_port1;
	setInfo->port[1] = m_port2;
	setInfo->port[2] = m_port3;
	setInfo->port[3] = m_port4;
	setInfo->port[4] = m_port5;
	setInfo->port[5] = m_port6;
	setInfo->port[6] = m_port7;
	setInfo->port[7] = m_port8;
	setInfo->port[8] = m_port9;
	setInfo->port[9] = m_port10;
	for (int i = 0; i < 10; i++)
	{
		tranChar(setInfo->port[i], pSetChar +2*i+1,2);
		//str.Format(_T("%d\n"), setInfo->port[i]);
		//m_message += str;
	}
	SetLenth = 21;
	///////////////�·�������Ϣ
	//char buffer[1024];
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	//SetLenth = WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), NULL, 0, NULL, NULL);
	//if (SetLenth >= 1024)
	//	SetLenth = 1024;
	////lenth = lenth % 16 ? lenth + 16 - lenth % 16 : lenth;
	////Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	////���ֽڱ���ת���ɶ��ֽڱ���
	//WideCharToMultiByte(CP_ACP, 0, m_message, m_message.GetLength(), SetBuffer, SetLenth, NULL, NULL);
	//SetLenth = dataEncap(SetBuffer, SetLenth);//��Э���װ����
	//m_message.ReleaseBuffer();
	
	if (send(toServer, pSetChar, SetLenth, 0) == SOCKET_ERROR)//����������Ϣ
	{//�쳣����
		m_chat += "Send Failed!(Socket Exception?)\r\n";
		UpdateData(false);
		return;
	}
	//ResetFlag = TRUE;
	//char Downflag[2];
	//Downflag[0] = 'r';
	//Downflag[1] = '\0';
	//send(toServer, Downflag, 1, 0);//��������״̬
	//ResetFlag = FALSE;
	//
}
