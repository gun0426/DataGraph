
// serialDlg.h : 헤더 파일
//

#define DAT_SIMUL		WM_USER+3
#define DAT_SIM_STOP	WM_USER+4
#define DAT_SIM_FINISH	WM_USER+5


#pragma once
#include "afxwin.h"
#include "serial_thread.h"
#include "Utils.h"
#include "afxcmn.h"
#include "Graph.h"
#include "afx.h"

#define RX_BUFF_SIZE 500
#define RX_DATA_SIZE 255
#define RX_STX	0x02
#define RX_CR	0x0D
#define RX_LF	0x0A

#define SK_STX	0x3A


typedef struct
{
	BYTE step;
	int idx_w;
	int idx_r;
	BYTE buff[RX_BUFF_SIZE];
	BYTE len;
	BYTE bcc;
	WORD cs;
	BYTE idx_d;
	BYTE data[RX_DATA_SIZE];
} str_rx;

enum ThreadWorkingType
{
	THREAD_STOP = 0,
	THREAD_RUNNING = 1,
	THREAD_PAUSE = 2
};

// CserialDlg 대화 상자
class CserialDlg : public CDialogEx
{
// 생성입니다.
public:
	CserialDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CSerialThread m_CSerial;
	CUtils m_CUtils;
	afx_msg void OnBnClickedBtnSeropen();
	int m_iSPRm_Data;
	int m_iSPRm_Stop;
	int m_iSPRm_Parity;
	bool m_bSerOpen;
	BYTE byIndexData(int xData);
	int baud_rate_idx;
	int com_port_idx;
	afx_msg void OnBnClickedBtnSend();
	CString m_strSend;
protected:
	afx_msg LRESULT OnRxEvent(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_EditRx;
	CRichEditCtrl m_REditRx;
	HWND msgHwnd_0;

	CHARFORMAT chFormat;
	CString m_strRxPack = _T("");
	str_rx t_rx;

	int OnRxFrameCheck();
//	int m_aMsg[20];
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGraph();

	CGraph m_CGraph;
	afx_msg void OnCbnSelchangeCombo1();
	int item_idx;
	int graph_idx;
	afx_msg void OnCbnSelchangeComboPort();
//	int m_iYMargin;
	afx_msg void OnBnClickedBtnFileopen();
	CString m_STRdatPath;
	CStdioFile m_SFsim;

//	static UINT Thread_SendFile(LPVOID pParam);
	static UINT Thread_Simul_Draw(LPVOID pParam);

	bool m_bTHRtxStart;
	CWinThread * p_thread_simul_draw;	
	CWinThread * p_thread_data_draw;
	bool is_working_thread;


	ThreadWorkingType m_eTHRtxWork;
	ThreadWorkingType thr_data_draw;
//	afx_msg void OnBnClickedBtnTxstart();
//	afx_msg void OnBnClickedBtnTxpause();
//	afx_msg void OnBnClickedBtnTxstop();
//protected:
//	afx_msg LRESULT OnTxThreadStop(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnDatSimul(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnDatSimStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDatSimFinish(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnGpause();
	afx_msg void OnBnClickedBtnGclose();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL m_bItemChk[8];
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	int draw_data_max;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_bAlwaysOnTop;
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedButtonClear();
	int draw_graph(unsigned char * p_start);
	afx_msg void OnCbnDropdownCombo1();
	static UINT Thread_Data_Draw(LPVOID p_param);
	afx_msg void OnEnChangeEditMaxN();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
