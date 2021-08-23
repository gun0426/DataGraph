

#define RX_EVENT	(WM_USER +1)
#define BUFF_SIZE	4096		//4192


//	통신 클래스	CCommThread 

/***********************************************************************
* class CQueue
*
* * Read  - Port를 Open 한 후에 데이타가 들어오면 RX_EVENT 메시지가 
*			Main Window 에 전달된다. ON_MESSAGE 매크로를 이용하여 함수를 
*			연결하고 m_ReadData String에 저장된 데이타를 이용하여 읽는다.
 
* * Write - WriteComm(buff, 30) 과 같이 버퍼와 그 크기를 주면 된다. 
*
***********************************************************************/
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];	// 큐 버퍼 사이즈는 4192로 만듬 
	int m_iHead, m_iTail;	// 큐 버퍼에 데이터를 넣고 빼 올때 사용할 변수 
	CQueue();
	void Clear();			// 버퍼를 초기화 시킴
	int GetSize();			// 현재 버퍼에 들어있는 데이터의 size를 리턴
	BOOL PutByte(BYTE b);	// 큐버퍼에 1바이트를 넣음
	BOOL GetByte(BYTE *pb);	// 큐버퍼에서 1바이트를 빼 옴
};

class	CSerialThread
{
public:
	CSerialThread();
	~CSerialThread();
	//--------- 환경 변수 -----------------------------------------//
	BOOL        check;
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bPortOpen;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// RX_EVENT와 함께 보내는 인수.
	CQueue      m_QueueRead;			// 큐버퍼
	//--------- 외부 사용 함수 ------------------------------------//
	BOOL	OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//포트 열기 
	void	ClosePort();				// 포트 닫기
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//포트에 데이터 쓰기

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//포트에서 데이터 읽어오기
};

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CSerialThread* pComm);

