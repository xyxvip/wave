
// waveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wave.h"
#include "waveDlg.h"
#include "afxdialogex.h"
#include "ChartCtrl/ChartCtrl.h"//����hight-speed chart�ؼ�
#include "ChartCtrl/ChartLineSerie.h"//����hight-speed chart�ؼ�
#include"MyNewDlg.h"

#include "Thnr.h"
#pragma comment(lib,"Thnr.lib") //����matlab�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.141592654

CString filename;                             // �򿪵��ļ����ַ���
CFile file;
CStdioFile stdfile;// �ļ�����
DWORD len;// ���ڱ����ļ�����
bool FileOpenFlag;
char *pReadData;//���ڴ洢�ļ��ַ�����
//double *TempData; //���ڴ洢��ʱ����
extern long signal_recvNum[4] = {0,0,0,0};//�洢���յ��ĸ�ͨ�����ݳ���
extern char* RecvBuffer=NULL;//�洢���յ�������
double* Analysisdata = NULL;//���ڴ洢��������������
extern double* SignalOne=NULL;
extern double* SignalTwo=NULL;
extern double* SignalThree=NULL;
extern double* SignalFour=NULL;//�����ĸ��ź�Դ���ݴ洢����
extern int ByteToBit(char* pBuff, int recvLen, char *dstBuff);//�ֽ�ת���ɰ�λ�����ַ�
extern int samplingData(char* pBuff, int recvLen);//�ɼ����ݽ���
extern BOOL WriteFile(int wrNum, double* wrData, int wrLenth);
extern BOOL WriteChar(int wrNum, char* wrData, int wrLenth);
IniInfo* setInfo = NULL;
extern bool RecvFlag;
extern int FileNum;
long ReadCount=0;
extern bool volatile  ReceiveFlag ;
extern long ReNum;
int ListNum = 0;//�����б�ؼ�����



int fs = 500 * FM;//����Ƶ��λ1024Mhz
//����matlab����
void CwaveDlg::creatMatlab(int filename)
{	
	//double data[2048];
	//memcpy(data, Analysisdata, 2048);
	double FS = fs;
	double len = filename;
	double THD = 0.0, SFDR = 0.0, SNDR = 0.0, SNR = 0.0, ENOB = 0.0;
	//mwArray mwdata(1, 2048, mxDOUBLE_CLASS);
	//mwdata.SetData(data, 2048);
	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
	mwfs.SetData(&FS, 1);
	mwArray mwlen(1, 1, mxDOUBLE_CLASS);
	mwlen.SetData(&len, 1);
	mwArray mwTHD(1, 1, mxDOUBLE_CLASS);
	mwTHD.SetData(&THD, 1);
	mwArray mwSFDR(1, 1, mxDOUBLE_CLASS);
	mwSFDR.SetData(&SFDR, 1);
	mwArray mwSNDR(1, 1, mxDOUBLE_CLASS);
	mwSNDR.SetData(&SNDR, 1);
	mwArray mwSNR(1, 1, mxDOUBLE_CLASS);
	mwSNR.SetData(&SNR, 1);
	mwArray mwENOB(1, 1, mxDOUBLE_CLASS);
	mwENOB.SetData(&ENOB, 1);
	Thnr(5, mwSNDR, mwSNR, mwTHD, mwSFDR, mwENOB, mwlen);                       //����m����
	//CString str;
	SNDR = mwSNDR.Get(1, 1);
	SNR = mwSNR.Get(1, 1);
	THD = mwTHD.Get(1, 1);
	SFDR = mwSFDR.Get(1, 1);
	ENOB = mwENOB.Get(1, 1);
	m_SNDR.Format(_T("%.4f"), SNDR);
	m_SN.Format(_T("%.4f"), SNR);
	m_THD.Format(_T("%.4f"), THD);
	m_SFDR.Format(_T("%.4f"), SFDR);
	m_ENOB.Format(_T("%.4f"), ENOB);
	//str.Format(_T("%f %f %f %f %f"), SNDR, SNR, THD, SFDR, ENOB);
	//AfxMessageBox(str);
	//////////////////////matlab
}
//�������������ݽṹ
typedef struct
{
	CString Signal_frequency;
	CString Pulse_width;
	CString Repetition_frequency;
	CString Rising_time;
	CString Modulation_type;
	CString Coding_rule;
	CString Element_width;
	CString Element_hight;
	CString Sampling_depth;
	CString Phase;
	CString Amplitude;

}Anylysis_result;
Anylysis_result* sampling_result;//������������������
int Sampling_Anylysis(double *XData, double *YData, int Datalenth, Anylysis_result* sanpling_result,int flag)//�Դ����Ĳ������ݽ��з���
{
	if (Datalenth > 0)
	{
		double sum = 0;
		double maxwave = YData[0];
		int npos = 0;
		double tmp;
		for (int i = 0; i < Datalenth; i++)
		{
			if (YData[i] > maxwave)
			{
				maxwave = YData[i];
				npos = i;
			}
			//sum += YData[i];
		}
		//sum /= Datalenth;
		tmp = XData[npos];
		if (maxwave > 0)
		{
			if(flag==1)
				sanpling_result->Signal_frequency.Format(_T("%.1fMhz"), tmp);//��ȡƵ��ֵ
			else
				sanpling_result->Amplitude.Format(_T("%.2f"), maxwave);//��ȡ����ֵ
		}
		return npos;
	}

}

///////////////////////////����һ���ַ�����ת����ʮ�������ݵĺ���
extern long CharToDecimal(char* srcData, double* dstData, int inlen, int digits, int baseFlag)
{
	if ((srcData == NULL) && (dstData == NULL))
		return 0;
	int doulfag = 0;
	int nCount = 0;
	int sum = 0;
	int temp = 0;
	bool flag = false;
	bool Numflag = false;
	bool floatflag = false;
	int bits = 0;
	switch (baseFlag)
	{
	case 2: {
		for (int i = 0; i < inlen; i++)
		{
			if (srcData[i] == '1')
			{
				doulfag++;
				temp = 1 << (digits - doulfag);
				sum += temp;
			}
			else if (srcData[i] == '0')
			{
				doulfag++;
			}
			else
			{
				doulfag = 0;
				sum = 0;
			}
			if (doulfag == digits)
			{
				doulfag = 0;
				if (sum > pow(2, digits - 1))
					sum = sum - pow(2, digits);
				dstData[nCount] = sum;
				sum = 0;
				nCount++;
			}
		}
		break;
	}
	case 16: {

		for (int i = 0; i < inlen; i++)
		{
			if (isdigit(srcData[i]))
			{
				temp = srcData[i] - 48;
				if ((temp > 7) && (i == 0))//�жϷ���λ��ֵ�Ƿ�Ϊ1
				{
					flag = true;
					//temp -= 8;
				}
				// �ܹ�3λ��һ��16����λ�� 4 bit����  
				// ��һ�Σ�'1'Ϊ���λ������temp���� (len - i -1) * 4 = 2 * 4 = 8 λ  
				// �ڶ��Σ�'d'Ϊ�θ�λ������temp���� (len - i -1) * 4 = 1 * 4 = 4 λ  
				// �����Σ�'e'Ϊ���λ������temp���� (len - i -1) * 4 = 0 * 4 = 0 λ
				doulfag++;
				bits = (4 - doulfag) * 4;
				temp = temp << bits;
				sum += temp;
			}
			else if (srcData[i] < 'A' || (srcData[i] > 'F' && srcData[i] < 'a') || srcData[i] > 'f')
			{
				doulfag = 0;
				sum = 0;
				continue;
			}
			else if (isalpha(srcData[i]))
			{
				if (isupper(srcData[i]))
					temp = srcData[i] - 55;
				else
					temp = srcData[i] - 87;
				doulfag++;
				bits = (digits - doulfag) * digits;
				temp = temp << bits;
				sum += temp;
			}
			else
			{
				doulfag = 0;
				sum = 0;
			}
			if (doulfag == digits)
			{
				doulfag = 0;
				if (flag)
				{
					dstData[nCount] = -sum;
					flag = false;
				}
				else
					dstData[nCount] = sum;
				sum = 0;
				nCount++;
			}
		}
		break;
	}
	case 10:
	{
		for (int i = 0; i < inlen; i++)
		{
			if (isdigit(srcData[i]))
			{
				doulfag++;
				temp = srcData[i] - 48;
				if (floatflag)
				{
					sum += (double)temp / 10;
					floatflag = false;
				}
				else
					sum = sum * pow(10, 1) + temp;//��ʮ��������λ����ȷ�������ÿո������ʱ
												  /*temp = srcData[i] - 48;
												  bits = (digits - doulfag);
												  temp = temp*pow(10, bits);
												  sum += temp;*/
				Numflag = false;
			}
			else if (srcData[i] == '-')
			{
				flag = true;
				Numflag = false;
			}
			else if (srcData[i] == ' ')
			{
				doulfag = 0;
				Numflag = true;
			}
			else if (srcData[i] == '.')
			{
				floatflag = true;
				//sum = 0;
			}
			if (Numflag)
			{
				if (flag)
				{
					dstData[nCount] = -sum;
					flag = false;
				}
				else
					dstData[nCount] = sum;
				doulfag = 0;
				sum = 0;
				nCount++;
			}
		}
		break;
	}
	default:
	{
		for (int i = 0; i < inlen; i++)
		{
			dstData[i] = (double)srcData[i];

		}
		nCount = inlen;
	}
	}
	return nCount;
}


#define DOUBLE_PI  6.2831853
typedef double          FFT_TYPE;

typedef struct complex_st {
	FFT_TYPE real;
	FFT_TYPE img;
} complex;
complex* Fftclc;//���ڴ洢fft��������

// ���ٸ���Ҷ�任  /////////////////////////////////////////


/*
* Bit Reverse
* === Input ===
* x : complex numbers
* n : nodes of FFT. @N should be power of 2, that is 2^(*)
* l : count by bit of binary format, @l=CEIL{log2(n)}
* === Output ===
* r : results after reversed.
* Note: I use a local variable @temp that result @r can be set
* to @x and won't overlap.
*/
void BitReverse(complex *x, complex *r, int n, int l)
{
	int i = 0;
	int j = 0;
	short stk = 0;
	static complex *temp = 0;

	temp = (complex *)malloc(sizeof(complex) * n);
	if (!temp) {
		return;
	}

	for (i = 0; i < n; i++) {
		stk = 0;
		j = 0;
		do {
			stk |= (i >> (j++)) & 0x01;
			if (j < l)
			{
				stk <<= 1;
			}
		} while (j < l);

		if (stk < n) {             /* ���㵹λ����� */
			temp[stk] = x[i];
		}
	}
	/* copy @temp to @r */
	for (i = 0; i < n; i++) {
		r[i] = temp[i];
	}
	free(temp);
}

/*
* FFT Algorithm
* === Inputs ===
* x : double numbers
* N : nodes of FFT. @N should be power of 2, that is 2^(*)
* === Output ===
* the @x contains the result of FFT algorithm, so the original data
* in @x is destroyed, please store them before using FFT.
*/
int FFT2(complex *x, int N)
{
	int i, j, l, ip;
	static int M = 0;
	static int le, le2;
	static FFT_TYPE sR, sI, tR, tI, uR, uI;

	M = (int)(log(N) / log(2));

	/*
	* bit reversal sorting
	*/
	BitReverse(x, x, N, M);

	/*
	* For Loops
	*/
	for (l = 1; l <= M; l++) {   /* loop for ceil{log2(N)} */
		le = (int)pow(2, l);
		le2 = (int)(le / 2);
		uR = 1;
		uI = 0;
		sR = cos(pi / le2);
		sI = -sin(pi / le2);
		for (j = 1; j <= le2; j++) {   /* loop for each sub DFT */
									   //jm1 = j - 1;  
			for (i = j - 1; i <= N - 1; i += le) {  /* loop for each butterfly */
				ip = i + le2;
				tR = x[ip].real * uR - x[ip].img * uI;
				tI = x[ip].real * uI + x[ip].img * uR;
				x[ip].real = x[i].real - tR;
				x[ip].img = x[i].img - tI;
				x[i].real += tR;
				x[i].img += tI;
			}  /* Next i */
			tR = uR;
			uR = tR * sR - uI * sI;
			uI = tR * sI + uI *sR;
		} /* Next j */
	} /* Next l */

	return 0;
}

//welch�������㹦���ܶ�
/*
x-˫����ʵ��һά���飬����Ϊn�������ź�x��i��
n-���α����������źŵĳ���
m-���α������ֶεĳ���
nfft-���α��������ƹ���������FFT�ĳ��ȣ���������2������������nfft>=m
win-���α����������������͡�win=1����ʾ���δ���win=2����ʾ������
fs-˫����ʵ�ͱ���������Ƶ��
r-˫����ʵ��һά���飬����Ϊ��nfft/2+1),���ڴ����غ���r��i����ֵ
freq-˫����ʵ��һά���飬����Ϊ��nfft/2+1),���ڴ���빦�������Ӧ��Ƶ��ֵ
sxx-˫����ʵ��һά���飬����Ϊ��nfft/2+1),���ڴ�Ź����׵�ֵ
sdb-���α��������ڱ�ʾ�����׵����ͣ�sdb=0����ʾ�����ף�sdb=1����ʾ��dBΪ��λ�Ķ�����*/
void pmpse(double x[], int n, int m, int nfft, int win, double fs, double r[], double freq[], double sxx[], int sdb)

{
	int i, j, k, s;
	int m2, nrd, kmax, ns1, nsectp;
	int nfft21, NumOfSections, NumUsed;
	double u, fl, xsum, norm, twopi, rexmn, imxmn, xmean;
	double *xa, *window;
	xa = (double*)malloc(nfft*sizeof(double));
	window = (double*)malloc(nfft*sizeof(double));
	nfft21 = nfft / 2 + 1;
	NumOfSections = (n - m / 2) / (m / 2);
	NumUsed = NumOfSections*(m / 2) + m / 2;
	s = 0;
	xsum = 0.0;
	ns1 = NumOfSections + 1;//����Ķ���
	m2 = m / 2;

	for (k = 0; k < ns1; k++)
	{
		for (i = 0; i < m2; i++)
		{
			xa[i] = x[s + i];
		}
		for (i = 0; i < m2; i++)
		{
			xsum = xsum + xa[i];
		}
		s += m2;
	}

	xmean = xsum / NumUsed;
	rexmn = xmean;
	imxmn = xmean;
	u = (double)m;//����u��ʹ�ú�����
	if (win == 2)
	{
		u = 0.0;
		twopi = 8.0*atan(1.0);
		fl = m - 1.0;
		for (int i = 0; i < nfft; i++)
		{
			window[i] = 0.54 - 0.46*cos(twopi*i / fl);
			u += window[i] * window[i];
		}
	}
	s = 0;
	for (i = 0; i < nfft21; i++)//������������������
	{
		sxx[i] = 0.0;
	}
	m2 = m / 2;
	for (i = 0; i < m2; i++)
	{
		xa[i + m2] = x[s + i];
	}
	s += m2;
	kmax = (NumOfSections + 1) / 2;
	nsectp = (NumOfSections + 1) / 2;
	nrd = m;

	for (k = 0; k < kmax; k++)//���㹦���׵�ֵ
	{
		for (i = 0; i < m2; i++)
		{
			j = m2 + i;
			Fftclc[i].real = xa[j];
			Fftclc[i].img = 0.0;
		}
		if ((k == (kmax - 1)) && (nsectp != NumOfSections))
		{
			for (i = m2; i < nrd; i++)
			{
				xa[i] = 0.0;
			}
			nrd = m / 2;
		}
		for (i = 0; i < nrd; i++)
		{
			xa[i] = x[s + i];
		}
		for (i = 0; i < m2; i++)
		{
			j = m2 + i;
			Fftclc[j].real = xa[i] - rexmn;
			Fftclc[j].img = xa[j] - imxmn;
			Fftclc[i].real = Fftclc[i].real - rexmn;
			Fftclc[i].img = xa[i] - imxmn;
		}

		if ((k == (kmax - 1)) && (nsectp != NumOfSections))
		{
			for (i = 0; i < m; i++)
			{
				Fftclc[i].img = 0.0;
			}
		}
		s = s + nrd;
		if (win == 2)//��ÿ�μӴ�����
		{
			for (i = 0; i < m; i++)
			{
				Fftclc[i].real *= window[i];
				Fftclc[i].img *= window[i];
			}
		}
		if (m != nfft)//�����Ĳ���
		{
			for (i = m; i < nfft; i++)
			{
				Fftclc[i].real = 0.0;
				Fftclc[i].img = 0.0;
			}
		}
		FFT2(Fftclc, nfft);

		for (i = 1; i < nfft21; i++)
		{
			j = nfft - i;
			sxx[i] += Fftclc[i].real * Fftclc[i].real + Fftclc[i].img * Fftclc[i].img;
			sxx[i] += Fftclc[j].real * Fftclc[j].real + Fftclc[j].img * Fftclc[j].img;
		}
		sxx[0] += Fftclc[0].real * Fftclc[0].real * 2.0;
		sxx[0] += Fftclc[0].img * Fftclc[0].img * 2.0;
	}


	norm = 2.0*u*NumOfSections;
	for (i = 0; i < nfft21; i++)
	{
		sxx[i] = sxx[i] / norm;
		Fftclc[i].real = sxx[i];
		Fftclc[i].img = 0.0;
		j = nfft - i;
		Fftclc[j].real = Fftclc[i].real;
		Fftclc[j].img = Fftclc[i].img;
	}
	FFT2(Fftclc, nfft);
	for (i = 0; i < nfft21; i++)
	{
		r[i] = Fftclc[i].real;
	}
	for (i = 0; i < nfft21; i++)
	{
		freq[i] = i*fs / (double)nfft;
		if (sdb == 1)
		{
			if (sxx[i] == 0.0)
				sxx[i] = 1.0e-15;
			sxx[i] = 20.0*log10(sxx[i]);
		}
	}

	free(xa);
	free(window);

}
////////////////////////////////////////////////////////////////////����Ҷ�任����
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwaveDlg �Ի���



CwaveDlg::CwaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WAVE_DIALOG, pParent)
	, m_radio1(0)
	, m_check1(TRUE)
	, m_check2(FALSE)
	, m_check3(FALSE)
	, m_check4(FALSE)
	, m_voltage(3.3)
	, m_radio2(0)
	, m_THD(_T("0.0"))
	, m_SFDR(_T("0.0"))
	, m_SN(_T("0.0"))
	, m_ENOB(_T("0.0"))
	, m_SNDR(_T("0.0"))
	, total_cap(0)
	, current_cap(0.0)
{
	//////////////////call matlab
	if (!ThnrInitialize())
	{
		AfxMessageBox(_T("Could not initialize the library"));
		return;
	}
	sampling_result = new Anylysis_result;
	/*sampling_result->Amplitude = "";
	sampling_result->Coding_rule = "";
	sampling_result->Element_hight = "";
	sampling_result->Element_width = "";
	sampling_result->Modulation_type = "";
	sampling_result->Phase = "";
	sampling_result->Pulse_width = "";
	sampling_result->Repetition_frequency = "";
	sampling_result->Rising_time = "";
	sampling_result->Sampling_depth = "";
	sampling_result->Signal_frequency = "";*/
	//memset(sampling_result, 0, sizeof(Anylysis_result));
	setInfo = new IniInfo;
	//memset(setInfo, 0, sizeof(IniInfo));//
	pReadData = new char[MAX_NUM];// ��ʼ�������ڴ�
	//TempData = new double[MAX_NUM];
	Analysisdata = new double[DATA_M];
	SignalOne=new double[DATA_M];
	SignalTwo = new double[DATA_M];
	SignalThree = new double[DATA_M];
	SignalFour = new double[DATA_M];//�����ĸ��ź�Դ���ݴ洢����
	Fftclc = new complex[DATA_M];
	RecvBuffer = new char[MAX_NUM];
	//memset(pReadData, 0, sizeof(char)*MAX_NUM);
	//memset(TempData, 0, sizeof(double)*MAX_NUM);
	//memset(Analysisdata, 0, sizeof(double)*MAX_NUM);
	//memset(Fftclc, 0, sizeof(complex)*MAX_NUM);//���ó�ֵΪ��
	pDlg = NULL;
	pTest = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_list);
	DDX_Control(pDX, IDC_COMBO2, Sampling_depth);
	DDX_Control(pDX, IDC_COMBO3, Sampling_frequency);
	DDX_Control(pDX, IDC_COMBO4, Trigger_setting);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
	DDX_Check(pDX, IDC_CHECK2, m_check2);
	DDX_Check(pDX, IDC_CHECK3, m_check3);
	DDX_Check(pDX, IDC_CHECK4, m_check4);
	DDX_Text(pDX, IDC_EDIT1, m_voltage);
	DDX_Radio(pDX, IDC_RADIO4, m_radio2);
	DDX_Text(pDX, IDC_EDIT14, m_THD);
	DDX_Text(pDX, IDC_EDIT5, m_SFDR);
	DDX_Text(pDX, IDC_EDIT7, m_SN);
	DDX_Text(pDX, IDC_EDIT8, m_ENOB);
	DDX_Text(pDX, IDC_EDIT6, m_SNDR);
	DDX_Text(pDX, IDC_EDIT2, total_cap);
	DDX_Text(pDX, IDC_EDIT3, current_cap);
}

BEGIN_MESSAGE_MAP(CwaveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDREFRESH, &CwaveDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDOPEN, &CwaveDlg::OnBnClickedOpen)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDANALYSIS, &CwaveDlg::OnBnClickedAnalysis)
	ON_BN_CLICKED(IDIMPORT, &CwaveDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_RADIO1, &CwaveDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CwaveDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CwaveDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_CHECK1, &CwaveDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CwaveDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CwaveDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CwaveDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_RADIO5, &CwaveDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO4, &CwaveDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO6, &CwaveDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CwaveDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDIMPORT2, &CwaveDlg::OnBnClickedImport2)
	ON_BN_CLICKED(IDSET, &CwaveDlg::OnBnClickedSet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CwaveDlg ��Ϣ�������

BOOL CwaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��Ӳ���������ѡ��ؼ���ֵ
	Sampling_depth.AddString(_T("100ms"));
	Sampling_depth.AddString(_T("150ms"));
	Sampling_depth.SetCurSel(0);
	Sampling_frequency.AddString(_T("1Ghz"));//0
	Sampling_frequency.AddString(_T("2Ghz"));//1
	Sampling_frequency.AddString(_T("500Mhz"));//2
	Sampling_frequency.SetCurSel(2);
	Trigger_setting.AddString(_T("�Զ�����"));
	Trigger_setting.AddString(_T("�ֶ�����"));
	Trigger_setting.AddString(_T("�ⲿ����"));
	Trigger_setting.SetCurSel(0);
	//��Ӳ���������ѡ��ؼ���ֵ
	//chartctrl
	//	m_HSChartCtrl.ShowWindow(SW_HIDE);
	CRect rect;
	GetDlgItem(IDC_STATIC_Tab)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_Tab)->ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_tab.Create(CMFCTabCtrl::STYLE_3D_ONENOTE,//�ؼ���ʽ��������������ʾ 
		rect,//�ؼ�����
		this,//�ؼ��ĸ�����ָ��
		1,//�ؼ�ID
		CMFCTabCtrl::LOCATION_TOP);//��ǩλ��	
	m_tab.AutoSizeWindow(TRUE);//�����öԻ�����tab��ʾ��������ţ�ͬʱ�����Ի����OnSize��Ϣ


	m_ChartCtrl1.Create(&m_tab, rect, 2);
	m_ChartCtrl1.SetBackColor(RGB(0, 120, 0));
	m_ChartCtrl1.SetBorderColor(RGB(0, 0, 200));//���ñ�������������ɫ
	CChartAxis *pAxis = NULL;
	//��ʼ������
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetMinMax(0, NFFT);
	//pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	m_pLineSerie1 = m_ChartCtrl1.CreateLineSerie();
	UpdateData(FALSE);
	m_tab.AddTab(&m_ChartCtrl1, _T("ʱ����"), 0, FALSE);
	m_tab.SetActiveTab(0);//������ʾ��һҳ��
	//
	m_ChartCtrl2.Create(&m_tab, rect, 3);
	m_ChartCtrl2.SetBackColor(RGB(0, 120, 0));
	m_ChartCtrl2.SetBorderColor(RGB(0, 0, 200));
	pAxis = m_ChartCtrl2.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl2.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	m_pLineSerie2 = m_ChartCtrl2.CreateLineSerie();
	m_tab.AddTab(&m_ChartCtrl2, _T("�����ײ���"), 1, FALSE);
	//
	m_ChartCtrl3.Create(&m_tab, rect, 4);
	m_ChartCtrl3.SetBackColor(RGB(0, 120, 0));
	m_ChartCtrl3.SetBorderColor(RGB(0, 0, 200));
	pAxis = m_ChartCtrl3.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl3.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetMinMax(-360, 360);
	m_pLineSerie3 = m_ChartCtrl3.CreateLineSerie();
	m_tab.AddTab(&m_ChartCtrl3, _T("��λ�ײ���"), 2, FALSE);
	//
	m_ChartCtrl4.Create(&m_tab, rect, 5);
	m_ChartCtrl4.SetBackColor(RGB(0, 120, 0));
	m_ChartCtrl4.SetBorderColor(RGB(0, 0, 200));
	pAxis = m_ChartCtrl4.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl4.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	m_pLineSerie4 = m_ChartCtrl4.CreateLineSerie();
	m_tab.AddTab(&m_ChartCtrl4, _T("�����ײ���"), 3, FALSE);

	m_tab.ShowWindow(SW_SHOWNORMAL);
	/*CRect TabBRect, TabTRect;
	m_tab.GetWindowRect(rect);
	ScreenToClient(rect);
	m_tab.GetTabArea(TabTRect, TabBRect);
	rect.top += TabTRect.Height();*/

	//�༭�����ݳ�ʼ����
	SetDlgItemText(IDC_EDIT1, _T("3.3"));
	SetDlgItemText(IDC_EDIT4, _T("100Mhz"));
	SetDlgItemText(IDC_EDIT12, _T("100ms"));
	SetDlgItemText(IDC_EDIT13, _T("10"));
	SetDlgItemText(IDC_EDIT14, _T("0.0"));

	//
	CRect rect1;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_list.GetClientRect(&rect1);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect1.Width() / 6, 0);
	m_list.InsertColumn(1, _T("����ʱ��"), LVCFMT_CENTER, rect1.Width() / 6, 1);
	m_list.InsertColumn(2, _T("�ź�Ƶ��"), LVCFMT_CENTER, rect1.Width() / 6, 2);
	m_list.InsertColumn(3, _T("�źŷ���"), LVCFMT_CENTER, rect1.Width() / 6, 3);
	m_list.InsertColumn(4, _T("�ź���λ"), LVCFMT_CENTER, rect1.Width() / 6, 4);
	m_list.InsertColumn(5, _T("�ɼ����"), LVCFMT_CENTER, rect1.Width() / 6, 5);

	//���û�������Χ
	m_slider.SetRange(0, 10);//���û�����Χ
	m_slider.SetTicFreq(10);//ÿ10����λ��һ�̶�
	m_slider.SetPos(10);
	////////////////
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP1);
	m_brbk.CreatePatternBrush(&bmp);
	bmp.DeleteObject();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwaveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CwaveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
		//

	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwaveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL reflag = TRUE;
void CwaveDlg::OnBnClickedRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CwaveDlg::OnBnClickedAnalysis();
	
	if (reflag)
	{
		SetTimer(1, 400, NULL);//������ʱ��1
		reflag = FALSE;
	}
	else
	{
		KillTimer(1);
		reflag = TRUE;
	}
		
}

void CwaveDlg::OnBnClickedOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CFileDialog dlg(TRUE, _T(""), _T("*.*"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.*|*.*|"));

	if (IDOK == dlg.DoModal()) {

		filename = dlg.GetPathName();
	}
	//else if (IDCANCEL == dlg.DoModal()) {
		//return;
	//}
	else
		return;

	//// ��ȡ����
	file.Open(filename, CFile::typeBinary | CFile::modeRead | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeCreate);
	len = file.GetLength();
	if (!len)
		return;
	FileOpenFlag = true;
	//if (pReadData != NULL)
	//{
	//	delete pReadData;
	//	pReadData = NULL;
	//	pReadData = new char[len];// �����ڴ�
	//	//delete TempData;
	//	//TempData = NULL;
	//	//TempData = new double[len];
	//	delete Analysisdata;
	//	Analysisdata = NULL;
	//	Analysisdata = new double[len];
	//	delete Fftclc;
	//	Fftclc = NULL;
	//	Fftclc = new complex[len];
	//}
	//else
	//{
	//	pReadData = new char[len];// �����ڴ�
	////	TempData = new double[len];
	//	Analysisdata = new double[len];
	//	Fftclc = new complex[len];
	//}
	file.Read(pReadData, len);//  ���ﱣ��Ķ�ȡ�ļ�������Sampling_Anylysis
	if (len > DATA_M)
		len = DATA_M;
	/*for (int i = 0; i < len/64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (j == 2)
				pReadData[i * 64 + j] = '1';
			else if (j == 3)
				pReadData[i * 64 + j] = '1';
			else if(i%2)
				pReadData[i * 64 + j] = j % 2+48;
			else
				pReadData[i * 64 + j] = 49;

		}
	}*/
	//ByteToBit(pReadData, len, RecvBuffer);
	//len=samplingData(RecvBuffer, len*8);
	//for (int i = 0; i < len; i++)
		//Analysisdata[i] = SignalOne[i];
	//BOOL write=WriteFile(111, Analysisdata, DATA_32K);

	len=CharToDecimal(pReadData, Analysisdata, len, 2, 10);//��ʾ��������ݵ���12λ2������ʽ,���ַ�����תΪʮ������ֵ,����ʮ�������ݳ���
	//for (int i = 0; i < (len  - 4); i++)
	//{
	//	Analysisdata[i] = (3 * TempData[i] + 2 * TempData[i + 1] + TempData[i + 2] - TempData[i + 4])*0.2;
	//	Analysisdata[i + 1] = (4 * TempData[i] + 3 * TempData[i + 1] + 2 * TempData[i + 2] + TempData[i + 3])*0.1;
	//	Analysisdata[i + 2] = (TempData[i] + TempData[i + 1] + TempData[i + 2] + TempData[i + 3] + TempData[i + 4])*0.2;
	//	Analysisdata[i + 3] = (TempData[i + 1] + 2 * TempData[i + 2] + 3 * TempData[i + 3] + 4 * TempData[i + 4])*0.1;
	//	Analysisdata[i + 4] = (-TempData[i] + TempData[i + 2] + 2 * TempData[i + 3] + 3 * TempData[i + 4])*0.2;
	//}//filter������㻬��ƽ�����˲�
	//double t;
	//for (int i = 0; i < len ; i++)
	//{
	//	t =  double(i*(double(FM) / fs));//��������Ҫ���ݲ���Ƶ�ʽ�������任
	//	//t = i*(double(1024 * 1024) / (fs));
	//	Analysisdata[i] = 125*sin(2 * pi * 15 *t) + 2248 * cos(200 * pi * t);
	//	//Fftclc[i].real =  cos(2 * pi * 40 * t) + 3 * cos(2 * pi * 100 * t); //0.5*sin(2 * pi * 15 * t) + 2 * sin(2 * pi * 40 * t); 
	//	//Fftclc[i].img = 0.00;
	////	TRACE("%f", TempData[i]);
	//}
	//len = len/4 ;
	//char a[20];
	//file.SeekToBegin();
	//file.Read(a, 20);
	//a[19] = '\0';
	//CString s(a);
	//MessageBox(s);//��ӡ�������ݣ����е���
	file.Close();

	//CString str;
	//stdfile.Open(filename, CFile::modeRead, NULL);
	//stdfile.ReadString(str);
	//MessageBox(str);//��һ��
	//stdfile.ReadString(str);
	//MessageBox(str);//�ڶ���

}

void CwaveDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
	{
		CString str;
		int pos = ((CSliderCtrl*)pScrollBar)->GetPos();
		float num;
		if (m_voltage)
			num = (float)pos*m_voltage / 10;//������һ�������ѹ����
		else
			num = (float)pos*3.3 / 10;
		str.Format(_T("%.1f"), num);
		GetDlgItem(IDC_EDIT1)->SetWindowText(str);//��ȡ��������ֵ����ʾ
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CwaveDlg::LeftMoveArray(double* ptr, size_t length, double data)
{
	for (size_t i = 1; i<length; ++i)
	{
		ptr[i - 1] = ptr[i];
	}
	ptr[length - 1] = data;
}

void CwaveDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (ReadCount < len / NFFT)
	//{

	//	LeftMoveArray(Analysisdata, NFFT, Analysisdata[ReadCount+NFFT]);
	//	ReadCount++;
	//}
	//else
	//{
	//	ReadCount = 0;
	//	KillTimer(1);
	//}
	//if (ReNum - ReadCount)
		//ReadCount = ReNum;
	//else
		//return;
	CwaveDlg::OnBnClickedAnalysis();
	CDialogEx::OnTimer(nIDEvent);
}


void CwaveDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	delete pDlg;
	ThnrTerminate();                    //����matlab DLL��
	mclTerminateApplication();//����matlab DLL��
	CDialogEx::OnClose();
	/////////////////////////�Գ����ж�̬���ٵ��ڴ�����ͷ�
	/*if (RecvBuffer != NULL)
	{
		delete[] RecvBuffer;
		RecvBuffer = NULL;
	}*/
	if (pReadData != NULL)
	{
		delete pReadData;
		pReadData = NULL;
	}
	/*if (TempData != NULL)
	{
		delete TempData;
		TempData = NULL;
	}*/
	if (Analysisdata != NULL)
	{
		delete[] Analysisdata;
		Analysisdata = NULL;
	}
	if (SignalOne != NULL)
	{
		delete[] SignalOne;
		SignalOne = NULL;
	}
	if (SignalTwo != NULL)
	{
		delete[] SignalTwo;
		SignalTwo = NULL;
	}
	if (SignalThree != NULL)
	{
		delete[] SignalThree;
		SignalThree = NULL;
	}
	if (SignalFour != NULL)
	{
		delete[] SignalFour;
		SignalFour = NULL;
	}
	if (Fftclc != NULL)
	{
		delete[] Fftclc;
		Fftclc = NULL;
	}
	if (sampling_result != NULL)
	{
		delete[] sampling_result;
		sampling_result = NULL;
	}
	if (setInfo != NULL)
	{
		delete[] setInfo;
		setInfo = NULL;
	}
}

int readNum = 0;
void CwaveDlg::OnBnClickedAnalysis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (len&&FileOpenFlag)//���ݴ������ݳ��Ƚ���ͼ�ε��ػ�
	{

		double* pDataX = NULL;
		double* pDataY = NULL;
		int proNum = 0;
		//�����ڴ�

		if (len < NFFT)
		{
			proNum = len;	
		}
		else
		{
			proNum = NFFT;
		}
		int maxNum = max(NFFT, len);
		pDataX = new double[maxNum];
		pDataY = new double[maxNum];
		for (int i = 0; i < len; i++)
		{
			pDataX[i] = i;// double(i*fs) / len;
			pDataY[i] = Analysisdata[i];
		}
		m_pLineSerie1->ClearSerie();
		m_pLineSerie1->AddPoints(pDataX, pDataY, len);//�������ݣ�����ʱ����
		//////////////////////////welch���㹦����
		int m, win, sdb, nfft, nfft21;
		double freq[NFFT], sxx[NFFT], rfft[NFFT], xfft[NFFT];
		m = 256;
		win = 2;
		nfft = 2048;
		sdb = 1;
		for (int i = 0; i < proNum; i++)
			xfft[i] = Analysisdata[i];
		pmpse(xfft, proNum, m, nfft, win, fs/ FM, rfft, freq, sxx, sdb);
		/////////////////////////
		for (int i = 0; i < proNum; i++)
		{
			pDataX[i] = freq[i];
			pDataY[i] = sxx[i];
			//pDataY[i] = 10 * log10(Fftclc[i].real*Fftclc[i].real + Fftclc[i].img*Fftclc[i].img);
		}
		//BOOL write = WriteFile(112, pDataY, DATA_K);
		m_pLineSerie2->ClearSerie();
		m_pLineSerie2->AddPoints(pDataX, pDataY, proNum / 2);//����fft�任֮�󣬻��ƹ����ײ���
		int nPos = Sampling_Anylysis(pDataX, pDataY, proNum / 2, sampling_result,1);//ͨ�������׵ó��ź�Ƶ��

		for (int i = 0; i < proNum; i++)
		{
			Fftclc[i].real = Analysisdata[i];
			Fftclc[i].img = 0.0;
		}
		FFT2(Fftclc, proNum);
		//DFT(Analysisdata, log2(NFFT), 1);
		for (int i = 0; i < proNum; i++)
		{
			pDataX[i] = i*(fs / FM)/ proNum;
			//pDataY[i] = 2*Analysisdata[i];
			pDataY[i] = sqrt(Fftclc[i].real*Fftclc[i].real + Fftclc[i].img*Fftclc[i].img) / (proNum / 2);
		}
		m_pLineSerie4->ClearSerie();
		m_pLineSerie4->AddPoints(pDataX, pDataY, proNum / 2);//����fft�任֮�󣬻��Ʒ����ײ���
		nPos = Sampling_Anylysis(pDataX, pDataY, proNum / 2, sampling_result, 2);//��ȡ����ֵ
		//sampling_result->Amplitude.Format(_T("%.2f"), pDataY[nPos]);
		

		for (int i = 0; i < proNum; i++)
		{
			//pDataY[i] =atan(Analysisdata[i])/pi*180 ;
			pDataY[i] = atan2(Fftclc[i].real, Fftclc[i].img) / pi * 180;
		}
		m_pLineSerie3->ClearSerie();
		m_pLineSerie3->AddPoints(pDataX, pDataY, proNum / 2);//����fft�任֮�󣬻�����λ�ײ���
		sampling_result->Phase.Format(_T("%.2f"), pDataY[nPos]);//�õ���λֵ

		if (pDataX)
		{
			delete[] pDataX;
		}
		if (pDataY)
		{
			delete[] pDataY;
		}
	}
	else if (Analysisdata != NULL&&(!ReceiveFlag))
	{
		double* pDataX = NULL;
		double* pDataY = NULL;
		//�����ڴ�

		pDataX = new double[DATA_32K];
		pDataY = new double[DATA_32K];
		switch (m_radio2)
		{
		case 0:memcpy(Analysisdata, SignalOne, DATA_32K*sizeof(double)); break;
		case 1:memcpy(Analysisdata, SignalTwo, DATA_32K*sizeof(double)); break;
		case 2:memcpy(Analysisdata, SignalThree, DATA_32K*sizeof(double)); break;
		case 3:memcpy(Analysisdata, SignalFour, DATA_32K*sizeof(double)); break;
		}
		
		for (int i = 0; i < DATA_32K; i++)
		{
			pDataX[i] = i;// double(i*fs) / MAX_NUM;
			pDataY[i] = Analysisdata[i];//SignalOne[i] 
		}

		m_pLineSerie1->ClearSerie();
		m_pLineSerie1->AddPoints(pDataX, pDataY, DATA_32K);//�������ݣ�����ʱ����

													   //////////////////////////welch���㹦����
		int m, win, sdb, nfft, nfft21;
		double freq[NFFT], sxx[NFFT], r[NFFT], x[NFFT];
		m = 256;
		win = 2;
		nfft = 1024;
		sdb = 1;
		for (int i = 0; i < NFFT; i++)
			x[i] = Analysisdata[i];
		pmpse(x, NFFT, m, nfft, win, fs / FM, r, freq, sxx, sdb);
		/////////////////////////
		for (int i = 0; i < NFFT; i++)
		{
			pDataX[i] = freq[i];
			pDataY[i] = sxx[i];
			//pDataY[i] = 10 * log10(Fftclc[i].real*Fftclc[i].real + Fftclc[i].img*Fftclc[i].img);
		}

		m_pLineSerie2->ClearSerie();
		m_pLineSerie2->AddPoints(pDataX, pDataY, 512);//����fft�任֮�󣬻��ƹ����ײ���
		int nPos = Sampling_Anylysis(pDataX, pDataY, NFFT / 2, sampling_result,1);

		for (int i = 0; i < NFFT; i++)
		{
			Fftclc[i].real = Analysisdata[i];
			Fftclc[i].img = 0.0;
		}
		FFT2(Fftclc, NFFT);
		//DFT(Analysisdata, log2(NFFT), 1);
		for (int i = 0; i < NFFT; i++)
		{
			pDataX[i] = i*(fs / FM) / NFFT;
			//pDataX[i] = double(i*fs) / NFFT;
			pDataY[i] = sqrt(Fftclc[i].real*Fftclc[i].real + Fftclc[i].img*Fftclc[i].img) / (NFFT / 2);
		}
		m_pLineSerie4->ClearSerie();
		m_pLineSerie4->AddPoints(pDataX, pDataY, NFFT / 2);//����fft�任֮�󣬻��Ʒ����ײ���
		nPos = Sampling_Anylysis(pDataX, pDataY, NFFT / 2, sampling_result, 2);//��ȡ����ֵ
		//sampling_result->Amplitude.Format(_T("%.2f"), pDataY[nPos]);//��ȡ����ֵ

		for (int i = 0; i < NFFT; i++)
		{
			//pDataY[i] =atan(Analysisdata[i])/pi*180 ;
			pDataY[i] = atan2(Fftclc[i].real, Fftclc[i].img) / pi * 180;
		}
		m_pLineSerie3->ClearSerie();
		m_pLineSerie3->AddPoints(pDataX, pDataY, NFFT / 2);//����fft�任֮�󣬻�����λ�ײ���
		sampling_result->Phase.Format(_T("%.2f"), pDataY[nPos]);//�õ���λֵ

		if (pDataX)
		{
			delete[] pDataX;
		}
		if (pDataY)
		{
			delete[] pDataY;
		}
		if (readNum < FileNum)
		{
			creatMatlab(readNum);
			readNum++;
		}
		else
			creatMatlab(FileNum);

		ReceiveFlag = TRUE;
	}
	else
		return;
	
	// ���б���ͼ�ؼ��в����б���������б������ı� 
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%2d:%2d:%2d"), st.wHour, st.wMinute, st.wSecond);//��ȡϵͳʱ��
	if (ListNum > 6)
	{
		ListNum = 0;
		m_list.DeleteAllItems();
	}
	int listcount = ListNum + 1;
	CString liststr;
	liststr.Format(_T("%d"), listcount);
	m_list.InsertItem(ListNum, liststr);
	m_list.SetItemText(ListNum, 1, strTime);
	m_list.SetItemText(ListNum, 2, sampling_result->Signal_frequency);
	m_list.SetItemText(ListNum, 3, sampling_result->Amplitude);
	m_list.SetItemText(ListNum, 4, sampling_result->Phase);
	//int     status = Sampling_depth.GetCurSel();   //��ȡ���λ��
	CString strstatus;     // ����һ���ַ�������
	Sampling_depth.GetLBText(Sampling_depth.GetCurSel(), strstatus);
	m_list.SetItemText(ListNum, 5, strstatus);
	ListNum++;

	SetDlgItemText(IDC_EDIT4, sampling_result->Signal_frequency);
	SetDlgItemText(IDC_EDIT12,strstatus);
	SetDlgItemText(IDC_EDIT13, sampling_result->Amplitude);
	//SetDlgItemText(IDC_EDIT14, sampling_result->Phase);
	UpdateData(FALSE);//��������ֵ��ʾ��������
	//��÷������ݲ���ʾ
//	RedrawWindow();
}


void CwaveDlg::OnBnClickedImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pDlg == NULL)
	{
		pDlg=new CMyNewDlg;
		pDlg->Create(IDD_SOCKETCLIENT, GetDesktopWindow());
	}
	pDlg->ShowWindow(SW_SHOW);
	pDlg->SetFocus();// �õ�����
}


void CwaveDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 0;
	m_check1 = TRUE;
	m_check2 = FALSE;
	m_check3 = FALSE;
	m_check4 = FALSE;
	GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
	UpdateData(false);
}


void CwaveDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 1;
	m_check1 = TRUE;
	m_check2 = FALSE;
	m_check3 = FALSE;
	m_check4 = FALSE;
	UpdateData(false);
	GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
}


void CwaveDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 2;
	GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
	m_check1 = TRUE;
	m_check2 = TRUE;
	m_check3 = TRUE;
	m_check4 = TRUE;
	UpdateData(false);//���û����ݸ��µ�����
}


void CwaveDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_radio1 ==2)
	{
		m_check1 = TRUE;
		m_check2 = TRUE;
		m_check3 = TRUE;
		m_check4 = TRUE;
		
	}
	else
	{
		m_check1 = TRUE;
		m_check2 = FALSE;
		m_check3 = FALSE;
		m_check4 = FALSE;
	}
	m_radio2 = 0;
	CwaveDlg::OnBnClickedRadio4();
	UpdateData(false);
}


void CwaveDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_radio1 == 2)
	{
		m_check1 = TRUE;
		m_check2 = TRUE;
		m_check3 = TRUE;
		m_check4 = TRUE;

	}
	else if (m_radio1 == 0)
	{
		m_check1 = FALSE;
		m_check2 = TRUE;
		m_check3 = FALSE;
		m_check4 = FALSE;
	}
	else
		return;
	m_radio2 = 1;
	CwaveDlg::OnBnClickedRadio5();
	UpdateData(false);
}


void CwaveDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_radio1 == 2)
	{
		m_check1 = TRUE;
		m_check2 = TRUE;
		m_check3 = TRUE;
		m_check4 = TRUE;

	}
	else
	{
		m_check1 = FALSE;
		m_check2 = FALSE;
		m_check3 = TRUE;
		m_check4 = FALSE;
	}
	m_radio2 = 2;
	CwaveDlg::OnBnClickedRadio6();
	UpdateData(false);
}


void CwaveDlg::OnBnClickedCheck4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_radio1 == 2)
	{
		m_check1 = TRUE;
		m_check2 = TRUE;
		m_check3 = TRUE;
		m_check4 = TRUE;

	}
	else if (m_radio1 == 0)
	{
		m_check1 = FALSE;
		m_check2 = FALSE;
		m_check3 = FALSE;
		m_check4 = TRUE;
	}
	else
		return;
	m_radio2 = 3;
	CwaveDlg::OnBnClickedRadio7();
	UpdateData(false);
}


void CwaveDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CwaveDlg::OnBnClickedRadio5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (Analysisdata != NULL)
	//{
	//	double t = 0;
	//	for (int i = 0; i < NFFT; i++)
	//	{
	//		t = double(i) / (fs);//��������Ҫ���ݲ���Ƶ�ʽ�������任
	//		Analysisdata[i] = cos(2 * pi * 40 * t) + 3 * cos(2 * pi * 100 * t);
	//	}
	////	CwaveDlg::OnBnClickedAnalysis();
	//}
	m_radio2 = 1;
	ReceiveFlag = FALSE;
}

void CwaveDlg::OnBnClickedRadio6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (Analysisdata != NULL)
	{
		double t = 0;
		for (int i = 0; i < NFFT; i++)
		{
			t = double(i) / (fs);//��������Ҫ���ݲ���Ƶ�ʽ�������任
			Analysisdata[i] = 0.5*sin(2 * pi * 15 * t) + 2 * sin(2 * pi * 40 * t);
		}
	//	CwaveDlg::OnBnClickedAnalysis();
	}
}


void CwaveDlg::OnBnClickedRadio7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (Analysisdata != NULL)
	{
		double t = 0;
		for (int i = 0; i < NFFT; i++)
		{
			t = double(i) / (fs);//��������Ҫ���ݲ���Ƶ�ʽ�������任
			Analysisdata[i] = 5*cos(2 * pi * 30 * t) + 2 * sin(2 * pi * 60 * t);
		}
		//CwaveDlg::OnBnClickedAnalysis();
	}
}

//�ɼ�����
void CwaveDlg::OnBnClickedImport2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*char a = '1';
	int b = a;*/
	if (RecvFlag)
	{
		
		CString str;     // ����һ���ַ�������
		int nIndex ;
		switch (setInfo->Sampling_frequency)
		{
		case 1:
			nIndex = 0;
			Sampling_frequency.SetCurSel(nIndex);
			break;
		case 2:
			nIndex = 1;
			Sampling_frequency.SetCurSel(nIndex);
			break;
		case 500:
			nIndex = 2;
			Sampling_frequency.SetCurSel(nIndex);
			break;
		}
		switch (setInfo->Sampling_depth)
		{
		case 100:
			nIndex = 0;
			Sampling_depth.SetCurSel(nIndex);
			break;
		case 150:
			nIndex = 1;
			Sampling_depth.SetCurSel(nIndex);
			break;
		}
		m_voltage=setInfo->Trigger_Level;//��ȡ������ƽ
		Trigger_setting.SetCurSel(setInfo->Trigger_Setting);//��ȡ��������ģʽ
		m_radio1=setInfo->Acquisition_Mode;//��ȡ�ɼ�ģʽ����
		m_check1=setInfo->Source_Channel[0] ;//��ȡ��ͨ��״̬
		m_check2=setInfo->Source_Channel[1] ;
		m_check3=setInfo->Source_Channel[2] ;
		m_check4=setInfo->Source_Channel[3] ;
		RecvFlag = FALSE;
		UpdateData(FALSE);
	}
	else
	{
		UpdateData();
		CString str;     // ����һ���ַ�������
		int nIndex = Sampling_frequency.GetCurSel();
		Sampling_frequency.GetLBText(nIndex, str);
		setInfo->Sampling_frequency = _ttol(str);//��ȡ����Ƶ��
		switch (nIndex)
		{
		case 0:
			fs = 1024* FM;
			setInfo->Sampling_frequency = 1;
			break;
		case 1:
			fs = 2048* FM;
			setInfo->Sampling_frequency = 2;
			break;
		case 2:
			fs = 500 * FM;
			setInfo->Sampling_frequency = 500;
			break;
		}
		Sampling_depth.GetLBText(Sampling_depth.GetCurSel(), str);
		setInfo->Sampling_depth = _ttol(str);//��ȡ�ɼ����
		setInfo->Trigger_Level = m_voltage;//��ȡ������ƽ
		setInfo->Trigger_Setting = Trigger_setting.GetCurSel();//��ȡ��������ģʽ
		setInfo->Acquisition_Mode = m_radio1;//��ȡ�ɼ�ģʽ����
		setInfo->Source_Channel[0] = m_check1;//��ȡ��ͨ��״̬
		setInfo->Source_Channel[1] = m_check2;
		setInfo->Source_Channel[2] = m_check3;
		setInfo->Source_Channel[3] = m_check4;
	}
	
}


void CwaveDlg::OnBnClickedSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (pTest == NULL)
	//{
	//	pTest = new MyTest;
	//	pTest->Create(IDD_TEST, GetDesktopWindow());
	//}
	//pTest->ShowWindow(SW_SHOW);
	//pTest->SetFocus();// �õ�����
	if (pTest == NULL)
	{
		pTest = new MyTest;
		pTest->DoModal();
	}
	
	//pTest->UpdateData(false);
}


HBRUSH CwaveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
		return m_brbk;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
