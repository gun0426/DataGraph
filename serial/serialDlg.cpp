
// serialDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "serial.h"
#include "serialDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND hCommWnd;
DWORD dwBaud[] = { 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 76800, 115200, 230400, 250000, 500000 };
CString strPort[] = { "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15" };
int m_aMsg[100];


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CserialDlg ��ȭ ����



CserialDlg::CserialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserialDlg::IDD, pParent)
	, m_iSPRm_Data(0)
	, m_iSPRm_Stop(0)
	, m_iSPRm_Parity(0)
	, m_bSerOpen(false)
//	, com_port_idx(0)
	, baud_rate_idx(0)
	, com_port_idx(0)
	, m_strSend(_T(""))
	, item_idx(0)
	, graph_idx(0)
//	, m_iYMargin(0)
	, m_STRdatPath(_T(""))
//	, m_bItemChk(FALSE)
, draw_data_max(0)
, m_bAlwaysOnTop(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_BAUD, baud_rate_idx);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, com_port_idx);
	DDX_Text(pDX, IDC_EDIT_TX, m_strSend);
	//	DDX_Control(pDX, IDC_EDIT_RX, m_EditRx);
	DDX_Control(pDX, IDC_RICHEDIT_RX, m_REditRx);
	DDX_CBIndex(pDX, IDC_COMBO1, item_idx);
	DDX_CBIndex(pDX, IDC_COMBO2, graph_idx);
	DDX_Check(pDX, IDC_CHECK1, m_bItemChk[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bItemChk[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bItemChk[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bItemChk[3]);
	DDX_Check(pDX, IDC_CHECK5, m_bItemChk[4]);
	DDX_Check(pDX, IDC_CHECK6, m_bItemChk[5]);
	DDX_Check(pDX, IDC_CHECK7, m_bItemChk[6]);
	DDX_Check(pDX, IDC_CHECK8, m_bItemChk[7]);
	DDX_Text(pDX, IDC_EDIT_MAX_N, draw_data_max);
	DDX_Check(pDX, IDC_CHECK9, m_bAlwaysOnTop);
}

BEGIN_MESSAGE_MAP(CserialDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEROPEN, &CserialDlg::OnBnClickedBtnSeropen)
	ON_BN_CLICKED(IDC_BTN_SEND, &CserialDlg::OnBnClickedBtnSend)
	ON_MESSAGE(RX_EVENT, &CserialDlg::OnRxEvent)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GRAPH, &CserialDlg::OnBnClickedBtnGraph)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CserialDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CserialDlg::OnCbnSelchangeComboPort)
	ON_BN_CLICKED(IDC_BTN_FILEOPEN, &CserialDlg::OnBnClickedBtnFileopen)
//	ON_BN_CLICKED(IDC_BTN_TXSTART, &CserialDlg::OnBnClickedBtnTxstart)
//	ON_BN_CLICKED(IDC_BTN_TXPAUSE, &CserialDlg::OnBnClickedBtnTxpause)
//	ON_BN_CLICKED(IDC_BTN_TXSTOP, &CserialDlg::OnBnClickedBtnTxstop)
//	ON_MESSAGE(TX_THREAD_STOP, &CserialDlg::OnTxThreadStop)
ON_MESSAGE(DAT_SIMUL, &CserialDlg::OnDatSimul)
//ON_MESSAGE(DAT_SIM_STOP, &CserialDlg::OnDatSimStop)
ON_MESSAGE(DAT_SIM_FINISH, &CserialDlg::OnDatSimFinish)
ON_BN_CLICKED(IDC_BTN_GPAUSE, &CserialDlg::OnBnClickedBtnGpause)
ON_BN_CLICKED(IDC_BTN_GCLOSE, &CserialDlg::OnBnClickedBtnGclose)
ON_CBN_SELCHANGE(IDC_COMBO2, &CserialDlg::OnCbnSelchangeCombo2)
ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_CHECK1, &CserialDlg::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK2, &CserialDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CserialDlg::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_CHECK4, &CserialDlg::OnBnClickedCheck4)
ON_BN_CLICKED(IDC_CHECK5, &CserialDlg::OnBnClickedCheck5)
ON_BN_CLICKED(IDC_CHECK6, &CserialDlg::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK7, &CserialDlg::OnBnClickedCheck7)
ON_BN_CLICKED(IDC_CHECK8, &CserialDlg::OnBnClickedCheck8)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_CHECK9, &CserialDlg::OnBnClickedCheck9)
//ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CserialDlg::OnBnClickedButtonClear)
ON_CBN_DROPDOWN(IDC_COMBO1, &CserialDlg::OnCbnDropdownCombo1)
ON_EN_CHANGE(IDC_EDIT_MAX_N, &CserialDlg::OnEnChangeEditMaxN)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CserialDlg �޽��� ó����

BOOL CserialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	hCommWnd = m_hWnd;

	/* m_REditRx ��Ʈ ��Ÿ�� ���� */
	CFont fontIt;
	fontIt.CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Consolas"));
	m_REditRx.SetFont(&fontIt);

	chFormat.dwMask = CFM_COLOR;		// | CFM_UNDERLINE | CFM_BOLD;
	chFormat.dwEffects = CFE_ALLCAPS;	// CFE_BOLD | CFE_UNDERLINE;
	chFormat.yHeight = 200;
	chFormat.crTextColor = RGB(255, 0, 0);
	
	com_port_idx = 3;
	baud_rate_idx = 12;
	t_rx.step = 0;
	t_rx.idx_w = 0;
	t_rx.idx_r = 0;
	t_rx.idx_d = 0;
	item_idx = 1;	//1+1
	graph_idx = 3;	//4+1
	for (int i = 0; i < 8; i++)
	{
		if (i <= item_idx)
		{
			m_bItemChk[i] = TRUE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = TRUE;
		}
		else
		{
			m_bItemChk[i] = FALSE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = FALSE;
		}
	}
	m_bAlwaysOnTop = TRUE;
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	CRect rect;
	this->GetWindowRect(rect);	//	GetClientRect(rect); �� ��
	SIZE s;
	ZeroMemory(&s, sizeof(SIZE));
	s.cx = (LONG)::GetSystemMetrics(SM_CXSCREEN);	// �ʺ�
	s.cy = (LONG)::GetSystemMetrics(SM_CYSCREEN);	// ���� 	
	SetWindowPos(NULL, s.cx-rect.Width(), 0, rect.Width(), rect.Height(), SWP_NOREPOSITION);
//	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOSIZE);

	draw_data_max = 40000;
	m_CGraph.m_LineChartCtrl.view_max = draw_data_max;


	UpdateData(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CserialDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CserialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CserialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CserialDlg::OnBnClickedBtnSeropen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/* �ø������ �Ķ���� ���� */
	m_iSPRm_Data = 8;	// ����Ÿ��Ʈ�� = 8 
	m_iSPRm_Stop = 0;	// ��ž��Ʈ�� = 1 
	m_iSPRm_Parity = 0;	// �з�Ƽ : no 

	/* "��Ʈ����" "��Ʈ�ݱ�" ��� */
	if (m_bSerOpen == FALSE){
		m_bSerOpen = TRUE;
	}
	else{
		m_bSerOpen = FALSE;
	}

	UpdateData(TRUE);
	CString CStrPortNum;	// = "Open Port";

	if (m_bSerOpen == TRUE)
	{
		CStrPortNum.Format(_T("Open Port : %s\r\n"), strPort[com_port_idx]);
		if (m_CSerial.m_bPortOpen == FALSE)
		{
			if (m_CSerial.OpenPort(strPort[com_port_idx], dwBaud[baud_rate_idx], m_iSPRm_Data, m_iSPRm_Stop, m_iSPRm_Parity) == TRUE)
			{
				CButton * pButton = (CButton*)GetDlgItem(IDC_BTN_SEROPEN);
				if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
					pButton->SetWindowText(_T("Close"));
				}
			}
		}
		else
		{
			AfxMessageBox(_T("Already Port Opeded !"));
		}
	}
	else
	{
		if (m_CSerial.m_bPortOpen == TRUE)
		{
			m_CSerial.ClosePort();
			CStrPortNum.Format(_T("Close Port : %s \r\n"), strPort[com_port_idx]);
		}
		else
		{
			CStrPortNum.Format(_T("%s Port not yet open \r\n"), strPort[com_port_idx]);
		}
		CButton * pButton = (CButton*)GetDlgItem(IDC_BTN_SEROPEN);
		if (pButton && ::IsWindow(pButton->GetSafeHwnd()))
		{
			pButton->SetWindowText(_T("Open"));
		}
	}
}


BYTE CserialDlg::byIndexData(int xData)
{
	BYTE byData;

	switch (xData)
	{
		case 0:
			byData = 5;
			break;
		case 1:
			byData = 6;
			break;
		case 2:
			byData = 7;
			break;
		case 3:
			byData = 8;
			break;
	}

	return byData;
}


void CserialDlg::OnBnClickedBtnSend()
{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	BYTE aTmp[2048];
//	BYTE aTx[1024]; 
//	WORD txcnt;
//	WORD size;
//	CString strTmp;
//
//	UpdateData(TRUE);
//
//	strTmp = m_strSend;
//	strTmp.Replace(_T(" "), _T(""));	// ���� ���ֱ�
//
//	size = strTmp.GetLength();			// ������ ���� ���ڿ� ���� ��� 
//	strcpy((char*)aTmp, (CStringA)strTmp.GetBuffer(size));	// �������⿡ _CRT_SECURE_NO_WARNINGS �߰�
//
////	// HEX										
//	if (size % 2)
//	{
//		AfxMessageBox(_T("odd input error !"));
//	}
//	txcnt = size / 2;
//	m_CUtils.hexStr2hexBin(aTmp, aTx, txcnt);
//
////	// ASCII
////	{											
////		txcnt = size;
////		strcpy((char*)aTxBuff, (CStringA)strTmp.GetBuffer(strTmp.GetLength()));	// �������⿡ _CRT_SECURE_NO_WARNINGS �߰�
////	}
//
//	m_CSerial.WriteComm((BYTE *)aTx, txcnt);
//
//#if 0
//	strTmp = _T("TX	");
//	strTmp += m_strSend;
//	m_pCLogManager->Log(strTmp);
//#endif


	//m_REditRx.Clear();
	m_REditRx.SetWindowText(_T(""));
	UpdateData(FALSE);
}


afx_msg LRESULT CserialDlg::OnRxEvent(WPARAM wParam, LPARAM lParam)
{
	CString strHex = _T("");

	int size;
	unsigned char temp;

	size = (m_CSerial.m_QueueRead).GetSize();
	for (int i = 0; i < size; i++)
	{
		(m_CSerial.m_QueueRead).GetByte(&temp);		// ť���� ������ �Ѱ��� �о��
		t_rx.buff[t_rx.idx_w++] = temp;
		t_rx.idx_w %= RX_BUFF_SIZE;
	}

	return 1;
}

int CserialDlg::draw_graph(unsigned char * p_start)
{
	int i = 0;
	int value = 0;
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		for (int grph_i = 0; grph_i < m_CGraph.m_LineChartCtrl.graph_n; grph_i++)
		{
			for (int item_i = 0; item_i <m_CGraph.m_LineChartCtrl.item_n; item_i++)
			{
				value = (int)((int)(*(p_start + i * 2) << 8) + (int)(*(p_start + i * 2 + 1) << 0));
				m_CGraph.m_LineChartCtrl.draw_data[grph_i * m_CGraph.m_LineChartCtrl.item_n + item_i].Add(value, 0, 0);
				i++;
			}
		}
		m_CGraph.m_LineChartCtrl.RedrawWindow();
//m_CGraph.GetDlgItem(IDC_CHECK_GRAPH_1)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
//m_CGraph.GetDlgItem(IDC_CHECK_GRAPH_1)->SetWindowPos(&wndTop, 20, 10, 600, 600, SWP_NOMOVE);
//m_CGraph.GetDlgItem(IDC_CHECK_GRAPH_1)->BringWindowToTop();
//m_CGraph.m_LineChartCtrl.RedrawWindow();
	}

	return 0;
}



#if (0)
//#define pi	3.14159265359
//
afx_msg LRESULT CserialDlg::OnRxEvent(WPARAM wParam, LPARAM lParam)
{
	CString strHex = _T("");
	BYTE readByte;
	int size;

	size = (m_CSerial.m_QueueRead).GetSize();		// ��Ʈ�� ���� ������ ����
	m_CGraph.m_LineChartCtrl.m_nBuffer = size;
	if (size > 150)
	{
		m_CGraph.m_LineChartCtrl.m_nViewPeriod++;
	}

	for (int i = 0; i < size; i++)
	{
		(m_CSerial.m_QueueRead).GetByte(&readByte);
		t_rx.buff[t_rx.idx_w++] = readByte;
		t_rx.idx_w %= RX_BUFF_SIZE;
	}
	OnRxFrameCheck();

	return 1;

	m_strRxPack = _T("");
	for (int i = 0; i < size; i++)
	{
		(m_CSerial.m_QueueRead).GetByte(&readByte);		// ť���� ������ �Ѱ��� �о��
		/* HEX */
		t_rx.buff[t_rx.idx_w++] = readByte;
		t_rx.idx_w %= RX_BUFF_SIZE;

		//if (readByte == RX_ETX)
		{
			OnRxFrameCheck();
			if (m_REditRx.GetLineCount() > 50)
			{
				m_REditRx.SetWindowText(_T(""));
				UpdateData(FALSE);
			}
		}
		strHex.Format(_T("%02X "), readByte);
		m_strRxPack += strHex;
		// ASCII
		//strTmp.Format(_T("%c"), aByte);
		//CStrRst += strTmp;
	}

	m_REditRx.SetSel(0, -1);
	m_REditRx.SetSel(-1, 0);
	m_REditRx.SetSelectionCharFormat(chFormat);
	m_REditRx.ReplaceSel(m_strRxPack);

	return 1;
}
#endif
#if (1)
typedef enum
{
	STEP_STX = 0,
	STEP_LEN,
	STEP_CMD,
	STEP_DATA,
	STEP_BCC,
	STEP_CR,
	STEP_LF
}enumRx;
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx s_Step = STEP_STX;
	static int stt_n = 0;
	static int sttDly[20] = { 0 };
	static BOOL sttPress[20] = { FALSE };
	static BYTE s_PreByte = 0;

	while (t_rx.idx_w != t_rx.idx_r)
	{
		temp = t_rx.buff[t_rx.idx_r++];
		t_rx.idx_r %= RX_BUFF_SIZE;

		switch (s_Step)
		{
		case STEP_STX:
			if (temp == RX_STX)
			{
				t_rx.bcc = 0;
				s_Step = STEP_LEN;
			}
			break;
		case STEP_LEN:
			t_rx.bcc ^= temp;
			t_rx.len = temp;
			t_rx.idx_d = 0;
			s_Step = STEP_CMD;
			break;
		case STEP_CMD:
			t_rx.data[t_rx.idx_d++] = temp;
			t_rx.bcc ^= temp;
			s_Step = STEP_DATA;
			break;
		case STEP_DATA:
			t_rx.data[t_rx.idx_d++] = temp;
			t_rx.bcc ^= temp;
			if (t_rx.idx_d == t_rx.len)
			{
				s_Step = STEP_BCC;
			}
			break;
		case STEP_BCC:
			if (t_rx.bcc == temp)
			{
				s_Step = STEP_CR;
			}
			break;
		case STEP_CR:
			if (temp == RX_CR)
			{
				s_Step = STEP_LF;
			}
			break;
		case STEP_LF:
			if (temp == RX_LF)
			{
				int i = 0;
				int dat_1 = 0;
				int dat_2 = 0;
				bool bPrint = FALSE;
				if (m_CGraph.m_LineChartCtrl.m_hWnd)
				{
					for (int GRPH_i = 0; GRPH_i < graph_idx+1; GRPH_i++)
					{
						for (int ITEM_i = 0; ITEM_i < item_idx + 1; ITEM_i++)
						{
							bPrint = FALSE;
							dat_1 = (int)((int)(t_rx.data[i * 2 + 0] << 8) + (int)(t_rx.data[i * 2 + 1] << 0));		
							if (ITEM_i == 0)	// ù��° ����Ÿ�� RAW, �ι�° ����Ÿ�� BASE, ����°���ʹ� �������. 
							{
								dat_2 = (int)((int)(t_rx.data[i * 2 + 2] << 8) + (int)(t_rx.data[i * 2 + 3] << 0));	// BASE ����Ÿ
							}else
							{
								dat_2 = dat_1;
							}

							if (ITEM_i == 0)
							{
								if (sttPress[GRPH_i] == FALSE)
								{
									if (dat_1 - dat_2 > 50)
									{
										sttPress[GRPH_i] = TRUE;
										sttDly[GRPH_i] = 0;
									}
								}
								else
								{
									sttDly[GRPH_i]++;
									if (sttDly[GRPH_i] == 7)
									{
										bPrint = TRUE;
									}
								}
								if (dat_1 - dat_2 < 100)
								{
									sttPress[GRPH_i] = FALSE;
								}
							}
							m_CGraph.m_LineChartCtrl.draw_data[GRPH_i * (item_idx + 1) + ITEM_i].Add(dat_1, dat_2, bPrint);
							i++;
						}
					}

					if (stt_n >= m_CGraph.m_LineChartCtrl.m_nViewPeriod)
					{
						m_CGraph.m_LineChartCtrl.RedrawWindow();
						stt_n = 0;
					}
					else
					{
						stt_n++;
					}
				}
			}
			s_Step = STEP_STX;
			break;
		default:
			s_Step = STEP_STX;
			break;
		}

		if ((temp == RX_LF) && (s_PreByte == RX_CR))
		{
			s_Step = STEP_STX;
		}

		s_PreByte = temp;
	}

	return 0;
}
#endif
#if (0)
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx s_Step = STEP_STX;
	static int stt_n = 0;
	static int sttDly[20] = { 0 };
	static BOOL sttPress[20] = { FALSE };
	static BYTE s_PreByte = 0;
	//static int stt_print_n = 0;

	static int status = 0;

	while (t_rx.idx_w != t_rx.idx_r)
	{
		temp = t_rx.buff[t_rx.idx_r++];
		t_rx.idx_r %= RX_BUFF_SIZE;


		switch (status)
		{
		case 0:
			if (temp == RX_STX)
			{
				status++;
				t_rx.idx_d = 0;
			}
			break;
		case 1:
			t_rx.data[t_rx.idx_d++] = temp;
			if (temp == RX_CR)
			{
				status++;
			}
			break;
		case 2:
			if (temp == RX_LF)
			{
				status = 0;
				int i = 0;
				int dat_1 = 0;
				int dat_2 = 0;
				bool bPrint = FALSE;
				if (m_CGraph.m_LineChartCtrl.m_hWnd)
				{
					for (int grph_i = 0; grph_i < m_CGraph.m_LineChartCtrl.graph_n; grph_i++)
					{
						for (int item_i = 0; item_i < m_CGraph.m_LineChartCtrl.item_n; item_i++)
						{
							bPrint = FALSE;
							dat_1 = (int)((int)(t_rx.data[i * 2 + 1] << 8) + (int)(t_rx.data[i * 2 + 2] << 0));
							if (item_i == 0)	// ù��° ����Ÿ�� RAW, �ι�° ����Ÿ�� BASE, ����°���ʹ� �������. 
							{
								dat_2 = (int)((int)(t_rx.data[i * 2 + 3] << 8) + (int)(t_rx.data[i * 2 + 4] << 0));	// BASE ����Ÿ
							}
							else
							{
								dat_2 = dat_1;
							}

							if (item_i == 0)
							{
								if (sttPress[grph_i] == FALSE)
								{
									if (dat_1 - dat_2 > 50)
									{
										sttPress[grph_i] = TRUE;
										sttDly[grph_i] = 0;
									}
								}
								else
								{
									sttDly[grph_i]++;
									if (sttDly[grph_i] == 7)
									{
										bPrint = TRUE;
									}
								}
								if (dat_1 - dat_2 < 100)
								{
									sttPress[grph_i] = FALSE;
								}
							}
							m_CGraph.m_LineChartCtrl.draw_data[grph_i * m_CGraph.m_LineChartCtrl.item_n + item_i].Add(dat_1, dat_2, bPrint);
							i++;
						}
					}

					if (stt_n >= m_nDrawPeriod)
					{
						m_CGraph.m_LineChartCtrl.RedrawWindow();
						stt_n = 0;
					}
					else
					{
						stt_n++;
					}
				}
			}
			else
			{
				status--;
			}
			break;
		}
	}
		
	return 0;


}
#endif
#if (0)
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx step = STEP_STX;
	static int sttCnt = 0;
	static int sttIndx = 0;
	static int sttData = 0;
	static bool sttEven = FALSE;
	static WORD sttCS;
	static BYTE sttPre = 0;

	while (t_rx.idx_w != t_rx.idx_r)
	{
		temp = t_rx.buff[t_rx.idx_r++];
		t_rx.idx_r %= RX_BUFF_SIZE;

		if (temp == 0x0A)
		{
			if (sttPre == 0x0D)
			{
				if (step == STEP_ETX)
				{
					int countG = m_CGraph.m_LineChartCtrl.graph_idx;
					int countI = m_CGraph.m_LineChartCtrl.item_idx;
					int i = 0;
					int iData = 0;
					if (m_CGraph.m_LineChartCtrl.m_hWnd)
					{
						for (int grph_i = 0; grph_i < countG; grph_i++)
						{
							for (int item_i = 0; item_i < countI; item_i++)
							{
								iData = (int)((int)(t_rx.data[i * 2 + 4] << 8) + (int)(t_rx.data[i * 2 + 5] << 0));
								m_CGraph.m_LineChartCtrl.draw_data[grph_i * countI + item_i].Add(iData, 0, 0);
								i++;
							}
						}
						sttCnt++;
						sttCnt %= 10000000;

						if (m_CGraph.m_LineChartCtrl.draw_data[0].lstData.GetSize() < 600)
						{
							//m_CGraph.m_LineChartCtrl.RedrawWindow();
							m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
						}
						else if (m_CGraph.m_LineChartCtrl.draw_data[0].lstData.GetSize() < 1200)
						{
							if (!(sttCnt % 2))
							{
								//m_CGraph.m_LineChartCtrl.RedrawWindow();
								m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
							}
						}
						else
						{
							if (!(sttCnt % 3))
							{
								//m_CGraph.m_LineChartCtrl.RedrawWindow();
								m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
							}
						}
						//m_CGraph.m_LineChartCtrl.RedrawWindow();
					}
				}
				step = STEP_STX;
			}
		}

		switch (step)
		{
			case STEP_STX:
				if (temp == SK_STX)
				{
					t_rx.cs = 0;
					step = STEP_LEN;
				}
				break;
			case STEP_LEN:
				t_rx.cs += temp;
				t_rx.len = temp&0x7f;
				t_rx.idx_d = 0;
				step = STEP_CMD;
				break;
			case STEP_CMD:
				t_rx.data[t_rx.idx_d++] = temp;
				t_rx.cs += temp;
				step = STEP_DATA;
				break;
			case STEP_DATA:
				t_rx.data[t_rx.idx_d++] = temp;
				t_rx.cs += temp;
				if (t_rx.idx_d == t_rx.len)
				{
					sttEven = FALSE;
					sttCS = 0;
					step = STEP_BCC;
				}
				break;
			case STEP_BCC:
				if (sttEven == FALSE)
				{
//					sttCS = temp;
//					sttCS <<= 8;
					sttEven = TRUE;
				}
				else
				{
//					sttCS += temp;
//					if (t_rx.cs == sttCS)
//					{
//						sttEven = FALSE;
//						step = STEP_ETX;
//					}
					step = STEP_ETX;
				}
				break;
			case STEP_ETX:
				break;
			default:
				step = STEP_STX;
				break;
		}
		sttPre = temp;
	}

	return 0;
}
#endif

void CserialDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	CWnd* pCtl = GetDlgItem(IDC_RICHEDIT_RX);
//
//	if (!pCtl){ return; }
//
//	CRect rectCtl;
//	pCtl->GetWindowRect(&rectCtl);
//	ScreenToClient(&rectCtl);
//
//	pCtl->MoveWindow(rectCtl.left, rectCtl.top, cx - 2 * rectCtl.left, cy - rectCtl.top - rectCtl.left, TRUE);

	return;
}


void CserialDlg::OnBnClickedBtnGraph()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_CGraph.DestroyWindow();
	m_CGraph.Create(IDD_GRAPH_DIALOG, this);
	m_CGraph.ShowWindow(SW_SHOW);

	m_CGraph.m_LineChartCtrl.graph_n = graph_idx + 1;
	m_CGraph.m_LineChartCtrl.item_n = item_idx + 1;


if (is_working_thread == false)
{
	is_working_thread = true;
	p_thread_data_draw = AfxBeginThread(Thread_Data_Draw, this);
}

}


void CserialDlg::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.graph_n = graph_idx + 1;
	m_CGraph.m_LineChartCtrl.item_n = item_idx + 1;

	for (int i = 0; i < 8; i++)
	{
		if (i <= item_idx)
		{
			m_bItemChk[i] = TRUE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = TRUE;
		}
		else
		{
			m_bItemChk[i] = FALSE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = FALSE;
		}			
	}
	UpdateData(FALSE);
}


void CserialDlg::OnCbnSelchangeComboPort()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CserialDlg::OnBnClickedBtnFileopen()
{
	static bool fileOpen = FALSE;

	if (p_thread_simul_draw != NULL)
	{
		p_thread_simul_draw->SuspendThread();
		m_eTHRtxWork = THREAD_PAUSE;
	}

	if (fileOpen == TRUE)
	{
		m_SFsim.Close();
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	/* �� File Path ��� */
	char szFilters[] = "Data(*.dat,*.txt) | *.dat;*.txt; | All Files(*.*)|*.*||";
	//	char szFilters[] = "All Files(*.*)|*.*||";	
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);
	if (fileDlg.DoModal() == IDOK)					// �����ϰ� Ȯ���ϸ� IDOK �� �Ѿ�ɴϴ�
	{
		m_STRdatPath = fileDlg.GetPathName();		// ���丮+�����̸�+Ȯ����
	}
	else
	{
		return;
	}

	if (m_SFsim.Open(m_STRdatPath, CFile::modeRead | CFile::typeText))
	{
		fileOpen = TRUE;
	}
	else
	{
		Sleep(0);
	}

	if (p_thread_simul_draw == NULL)
	{
		p_thread_simul_draw = AfxBeginThread(Thread_Simul_Draw, this);
		if (p_thread_simul_draw == NULL)
		{
			AfxMessageBox(_T("Thread_SendFile Error"));
		}
		p_thread_simul_draw->m_bAutoDelete = FALSE;
		m_eTHRtxWork = THREAD_RUNNING;
	}
	else
	{
		if (m_eTHRtxWork == THREAD_PAUSE)
		{
			p_thread_simul_draw->ResumeThread();
			m_eTHRtxWork = THREAD_RUNNING;
		}
	}
}

UINT CserialDlg::Thread_Simul_Draw(LPVOID pParam)
{
	CserialDlg *aa = (CserialDlg*)pParam;
	CserialDlg *pDlg = (CserialDlg*)AfxGetApp()->m_pMainWnd;


	CString strLine = _T("");
	CString strData = _T("");
	BYTE aHexStr[2048];
	BYTE aHexBin[1024];

	while (1)
	{
		Sleep(0);

		if (pDlg->m_CGraph.m_bWindow == TRUE)
		{
			if (pDlg->m_SFsim.ReadString(strLine) == FALSE)	
			{
//				pDlg->SendMessage(DAT_SIM_STOP, 0, 0);
				pDlg->SendMessage(DAT_SIM_FINISH, 0, 0);
			}
			else
			{
				int pos = strLine.Find(_T("#OK"));	/* $0010000018a118a618ab18a818fa18fa18fa0000187a187c187f187f18cb18cc18cb000018d618d818e018e2192b192f192b0000197119741987198719cf19cf19cf0000#OK\n */
				strData = strLine.Mid(9, pos - 9);	/* 18a118a618ab18a818fa18fa18fa0000187a187c187f187f18cb18cc18cb000018d618d818e018e2192b192f192b0000197119741987198719cf19cf19cf0000 */

				strcpy((char*)aHexStr, (CStringA)strData.GetBuffer(strData.GetLength()));	// �������⿡ _CRT_SECURE_NO_WARNINGS �߰�
				pDlg->m_CUtils.hexStr2hexBin(aHexStr, aHexBin, strData.GetLength() / 2);

				pDlg->SendMessage(DAT_SIMUL, 0, (LPARAM)aHexBin);
			}
		}
	}
	return 0;
}

afx_msg LRESULT CserialDlg::OnDatSimul(WPARAM wParam, LPARAM lParam)
{
	BYTE* ptr;

	ptr = (BYTE*)lParam;

	int i = 0;
	int iData = 0;
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		for (int grph_i = 0; grph_i < m_CGraph.m_LineChartCtrl.graph_n; grph_i++)
		{
			for (int item_i = 0; item_i <m_CGraph.m_LineChartCtrl.item_n; item_i++)
			{
				iData = (int)((int)(*(ptr + i * 2) << 8) + (int)(*(ptr + i * 2 + 1) << 0));
				m_CGraph.m_LineChartCtrl.draw_data[grph_i * m_CGraph.m_LineChartCtrl.item_n + item_i].Add(iData, 0, 0);
				i++;
			}
		}
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}
	return 0;
}

afx_msg LRESULT CserialDlg::OnDatSimFinish(WPARAM wParam, LPARAM lParam)
{
	if (p_thread_simul_draw != NULL)
	{
		m_SFsim.Close();
		p_thread_simul_draw->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(p_thread_simul_draw->m_hThread, &dwResult);

		delete p_thread_simul_draw;
		p_thread_simul_draw = NULL;

		m_eTHRtxWork = THREAD_STOP;
	}
	return 0;
}

void CserialDlg::OnBnClickedBtnGpause()
{
	static bool bPausePlay = FALSE;

	if (m_CGraph.m_bWindow == FALSE)
	{
		bPausePlay = FALSE;
	}

	if (bPausePlay == FALSE)
	{
		if (p_thread_simul_draw == NULL)
		{
			AfxMessageBox(_T("Thread Not Started !"));
		}
		else
		{
			p_thread_simul_draw->SuspendThread();
			m_eTHRtxWork = THREAD_PAUSE;
		}
		bPausePlay = TRUE;
	}
	else
	{
		if (p_thread_simul_draw == NULL)
		{
			//p_thread_simul_draw = AfxBeginThread(Thread_Simul_Draw, this);
			//if (p_thread_simul_draw == NULL)
			//{
			//	AfxMessageBox(_T("Thread_SendFile Error"));
			//}
			//p_thread_simul_draw->m_bAutoDelete = FALSE;
			//m_eTHRtxWork = THREAD_RUNNING;
		}
		else
		{
			if (m_eTHRtxWork == THREAD_PAUSE)
			{
				p_thread_simul_draw->ResumeThread();
				m_eTHRtxWork = THREAD_RUNNING;
			}
		}
		bPausePlay = FALSE;
	}

}

void CserialDlg::OnBnClickedBtnGclose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (p_thread_simul_draw == NULL)
	{
		AfxMessageBox(_T("Thread Not Started !"));
	}
	else
	{
		m_SFsim.Close();
		m_CGraph.m_bWindow = FALSE;
		m_CGraph.DestroyWindow();
		p_thread_simul_draw->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(p_thread_simul_draw->m_hThread, &dwResult);

		delete p_thread_simul_draw;
		p_thread_simul_draw = NULL;

		m_eTHRtxWork = THREAD_STOP;
	}
}

void CserialDlg::OnCbnSelchangeCombo2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.graph_n = graph_idx + 1;
	m_CGraph.m_LineChartCtrl.item_n = item_idx + 1;
}

//
//UINT CserialDlg::Thread_SendFile(LPVOID pParam)
//{
//	CserialDlg *aa = (CserialDlg*)pParam;
//	CserialDlg *pDlg = (CserialDlg*)AfxGetApp()->m_pMainWnd;
//
//	CString strLine = _T("");
//	CString strData = _T("");
//	BYTE aHexStr[2048];
//	BYTE aHexBin[1024];
//
//	while (pDlg->m_eTHRtxWork == THREAD_RUNNING)
//	{
//		Sleep(100);
//
//		if (pDlg->m_SFsim.ReadString(strLine) == FALSE)
//		{
//			pDlg->PostMessage(TX_THREAD_STOP, 0, 0);
//		}
//		else
//		{
//			int pos = strLine.Find(_T("#OK"));
//			strData = strLine.Mid(9, pos - 9);
//
//			strcpy((char*)aHexStr, (CStringA)strData.GetBuffer(strData.GetLength()));	// �������⿡ _CRT_SECURE_NO_WARNINGS �߰�
//			aHexBin[0] = 0x02;
//			aHexBin[1] = strData.GetLength() / 2;
//			pDlg->m_CUtils.hexStr2hexBin(aHexStr, &aHexBin[2], strData.GetLength() / 2);
//
//			aHexBin[2 + strData.GetLength() / 2] = 0;
//			for (int i = 0; i < strData.GetLength() / 2 + 1; i++)
//			{
//				aHexBin[2 + strData.GetLength() / 2] ^= aHexBin[1 + i];
//			}
//			aHexBin[2 + strData.GetLength() / 2 + 1] = 0x03;
//
//			pDlg->m_CSerial.WriteComm((BYTE *)aHexBin, strData.GetLength() / 2 + 4);
//		}
//	}
//	
//	return 0;
//}
//
//void CserialDlg::OnBnClickedBtnTxstart()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	if (p_thread_simul_draw == NULL)
//	{
//		p_thread_simul_draw = AfxBeginThread(Thread_SendFile, this);
//		if (p_thread_simul_draw == NULL)
//		{
//			AfxMessageBox(_T("Thread_SendFile Error"));
//		}
//		p_thread_simul_draw->m_bAutoDelete = FALSE;
//		m_eTHRtxWork = THREAD_RUNNING;
//	}
//	else
//	{
//		if (m_eTHRtxWork == THREAD_PAUSE)
//		{
//			p_thread_simul_draw->ResumeThread();
//			m_eTHRtxWork = THREAD_RUNNING;
//		}
//	}
//}
//
//
//void CserialDlg::OnBnClickedBtnTxpause()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	if (p_thread_simul_draw == NULL)
//	{
//		AfxMessageBox(_T("Thread Not Started !"));
//	}
//	else
//	{
//		p_thread_simul_draw->SuspendThread();
//		m_eTHRtxWork = THREAD_PAUSE;
//	}
//}
//
//
//void CserialDlg::OnBnClickedBtnTxstop()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	if (p_thread_simul_draw == NULL)
//	{
//		AfxMessageBox(_T("Thread Not Started !"));
//	}
//	else
//	{
//		m_SFsim.Close();
//		p_thread_simul_draw->SuspendThread();
//		DWORD dwResult;
//		::GetExitCodeThread(p_thread_simul_draw->m_hThread, &dwResult);
//
//		delete p_thread_simul_draw;
//		p_thread_simul_draw = NULL;
//
//		m_eTHRtxWork = THREAD_STOP;
//	}
//}
//
//
//afx_msg LRESULT CserialDlg::OnTxThreadStop(WPARAM wParam, LPARAM lParam)
//{
//	if (p_thread_simul_draw != NULL)
//	{
//		m_SFsim.Close();
//		p_thread_simul_draw->SuspendThread();
//		DWORD dwResult;
//		::GetExitCodeThread(p_thread_simul_draw->m_hThread, &dwResult);
//
//		delete p_thread_simul_draw;
//		p_thread_simul_draw = NULL;
//
//		m_eTHRtxWork = THREAD_STOP;
//	}
//	return 0;
//}


BOOL CserialDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(140, 170, 230));
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CserialDlg::OnBnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[0] = m_bItemChk[0];
}


void CserialDlg::OnBnClickedCheck2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[1] = m_bItemChk[1];
}


void CserialDlg::OnBnClickedCheck3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[2] = m_bItemChk[2];
}


void CserialDlg::OnBnClickedCheck4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[3] = m_bItemChk[3];
}


void CserialDlg::OnBnClickedCheck5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[4] = m_bItemChk[4];
}


void CserialDlg::OnBnClickedCheck6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[5] = m_bItemChk[5];
}


void CserialDlg::OnBnClickedCheck7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[6] = m_bItemChk[6];
}


void CserialDlg::OnBnClickedCheck8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[7] = m_bItemChk[7];
}


BOOL CserialDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
			case VK_RETURN:
				int temp;
				temp = draw_data_max;
				UpdateData(TRUE);		
				if (temp > draw_data_max)
				{
					for (int i = 0; i < m_CGraph.m_LineChartCtrl.graph_n; i++)
					{
						for (int j = 0; j < m_CGraph.m_LineChartCtrl.item_n; j++)
						{
							m_CGraph.m_LineChartCtrl.draw_data[i * m_CGraph.m_LineChartCtrl.item_n + j].Clear();
						}
					}
				}
				m_CGraph.m_LineChartCtrl.view_max = draw_data_max;
				return TRUE;
	//		case VK_F2:
	//		case VK_F3:
	//			return TRUE;
			default:
				break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CserialDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	pDC->SetBkMode(TRANSPARENT); // ���ڻ��� ������ ��ġ�ϰ� ����

	pDC->SetTextColor(RGB(0, 0, 0));	// ���ڻ�

	if (nCtlColor == CTLCOLOR_STATIC)	// ���� ����ƽ
	{
		return CreateSolidBrush(RGB(140, 170, 230));
	}

//	if (nCtlColor == CTLCOLOR_BTN)		// ��ư(üũ��ư/������ư)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_EDIT)		// ��ư(üũ��ư/������ư)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_LISTBOX)		// ��ư(üũ��ư/������ư)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_DLG)		// ���̾�α� �ڽ� ������
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}


	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CserialDlg::OnBnClickedCheck9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_bAlwaysOnTop == TRUE)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}



void CserialDlg::OnBnClickedButtonClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
#if (1)
	m_REditRx.SetSel(0, -1);
	m_REditRx.Clear();
#else
	m_REditRx.SetWindowText(_T(""));
	UpdateData(FALSE);
#endif
}

void CserialDlg::OnCbnDropdownCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(FALSE);
}


UINT CserialDlg::Thread_Data_Draw(LPVOID p_param)
{
	CserialDlg *pMain = (CserialDlg*)p_param;
	static int check_sum = 0;

	while (pMain->is_working_thread)
	{
		Sleep(0);

		if (pMain->t_rx.idx_r != pMain->t_rx.idx_w)
		{
			switch (pMain->t_rx.step)
			{
				case 0:	/* header */
					if (pMain->t_rx.buff[pMain->t_rx.idx_r] == 0x3A)
					{
						check_sum = 0;
						pMain->SetTimer(1, 100, NULL);
						pMain->t_rx.step++;
					}
					break;
				case 1:	/* length */
					pMain->t_rx.len = pMain->t_rx.buff[pMain->t_rx.idx_r];
					pMain->t_rx.idx_d = 0;
					check_sum = pMain->t_rx.buff[pMain->t_rx.idx_r] & 0x000000ff;
					pMain->t_rx.step++;
					break;
				case 2:	/* data */
					pMain->t_rx.data[pMain->t_rx.idx_d++] = pMain->t_rx.buff[pMain->t_rx.idx_r];
					check_sum += pMain->t_rx.buff[pMain->t_rx.idx_r] & 0x000000ff;
					if (pMain->t_rx.idx_d == (pMain->t_rx.len - 0x80) * 2)
					{
						pMain->t_rx.step++;
					}
					break;
				case 3:	/* check-sum */
					pMain->t_rx.cs = pMain->t_rx.buff[pMain->t_rx.idx_r] & 0x000000ff;
					pMain->t_rx.cs <<= 8;
					pMain->t_rx.step++;
					break;
				case 4:
					pMain->t_rx.cs += pMain->t_rx.buff[pMain->t_rx.idx_r] & 0x000000ff;
					pMain->t_rx.cs = ((~pMain->t_rx.cs) & 0x0000ffff) - 1;
					if (pMain->t_rx.cs == check_sum)
					{
						pMain->t_rx.step++;
					}
					else
					{
						pMain->t_rx.step = 0;
					}
					break;
				case 5:
					if (pMain->t_rx.buff[pMain->t_rx.idx_r] == 0x0D)
					{
						pMain->t_rx.step++;
					}
					else
					{
						pMain->t_rx.step = 0;
					}
					break;
				case 6:
					if (pMain->t_rx.buff[pMain->t_rx.idx_r] == 0x0A)
					{
						pMain->KillTimer(1);
						pMain->draw_graph(&pMain->t_rx.data[4]);
					}
					pMain->t_rx.step = 0;
			}

			pMain->t_rx.idx_r++;
			pMain->t_rx.idx_r %= RX_BUFF_SIZE;

		}

#if (0)
pMain->t_rx.data[pMain->t_rx.idx_d++] = pMain->t_rx.buff[pMain->t_rx.idx_r];

if (pMain->t_rx.buff[pMain->t_rx.idx_r] == 0x0A)
{
	if (pMain->t_rx.buff[(pMain->t_rx.idx_r + RX_BUFF_SIZE - 1) % RX_BUFF_SIZE] == 0x0D)
	{
		pMain->t_rx.idx_d = 0;
		pMain->draw_graph(&pMain->t_rx.data[7]);
	}
}
pMain->t_rx.idx_r++;
pMain->t_rx.idx_r %= RX_BUFF_SIZE;
#endif


	}

	return 0;
}


void CserialDlg::OnEnChangeEditMaxN()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CserialDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (nIDEvent == 1)
	{
		t_rx.step = 0;
	}
	CDialogEx::OnTimer(nIDEvent);
}
