// MLineChartCtrl.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "MLineChartCtrl.h"


extern HWND hCommWnd;

// MLineChartCtrl

IMPLEMENT_DYNAMIC(MLineChartCtrl, CWnd)

MLineChartCtrl::MLineChartCtrl()
: graph_x(0)
, graph_n(0)
, item_n(0)
, graph_height(0)
, m_bLBtnDbClk(false)
, btn_dbl_clk(false)
, m_iBKColor_i(0)
, m_nViewSplit(0)
, m_bF2Evnt(false)
, view_max(0)
, m_bLClkDown(false)
, m_nBuffer(0)
, m_nViewPeriod(0)
{
	m_hMemBitmap = NULL;
	m_hMemDC = NULL;
}

MLineChartCtrl::~MLineChartCtrl()
{
}


BEGIN_MESSAGE_MAP(MLineChartCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// MLineChartCtrl 메시지 처리기입니다.
void MLineChartCtrl::OnPaint()	// GUN_MARK
{
	CPaintDC dc(this);		// device context for painting
	HDC hDC = dc.m_hDC;

	if (m_bLBtnDbClk == TRUE)	return;

	for (int grph_i = 0; grph_i< graph_n; grph_i++)
	{
		int size = draw_data[grph_i*item_n].lstData.GetSize();
		if (size > view_max)
		{
			for (int item_i = 0; item_i < item_n; item_i++)
			{
				for (int cnt = 0; cnt < size - view_max; cnt++)
				{
					draw_data[grph_i * item_n + item_i].lstData.RemoveAt(0);
				}			
			}
		}
	}

	GetClientRect(rcWindow);
	if (btn_dbl_clk == FALSE)	
	{
		graph_height = rcWindow.Height() / graph_n;
	}
	else						
	{
		graph_height = rcWindow.Height() / ((graph_n / 2) + (graph_n % 2));
	}

	DrawBG(m_hMemDC);
	for (int i = 0; i < graph_n; i++)
	{
		graph_x = i;
		y_axis_max = draw_data[i*item_n].nYMax;
		y_axis_min = draw_data[i*item_n].nYMin;
		DrawChart(m_hMemDC);
	}
	/************************************************
	BOOL BitBlt(HDC hdcDest,int nXDest,int nYDest,int nWidth,int nHeight,HDC hdcSrc,int nXSrc,int nYSrc,DWORD dwRop);

	hdcDest	: 이미지를 출력할 위치의 핸들
	nXDest	: 이미지를 출력할 위치 x 좌표
	nYDest	: 이미지를 출력할 위치 y 좌표
	nWidth	: 이 크기만큼 원본 이미지에서 잘라와 그림
	nHeight	: 이 크기만큼 원본 이미지에서 잘라와 그림
	hdcSrc	: 이미지의 핸들
	nXSrc	: 가져올 이미지의 시작지점(x)
	nYSrc	: 가져올 이미지의 시작지점(y)
	dwRop	: 이미지의 출력 방법(dwRop)


	BOOL StretchBlt(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,DWORD dwRop);

	hdcDest			: 이미지를 출력할 위치의 핸들입니다.
	nXOriginDest	: 이미지를 출력할 위치인 x좌표입니다.
	nYOriginDest	: 이미지를 출력할 위치인 y좌표입니다.
	nWidthDest		: 출력할 이미지의 너비 - 이 크기만큼 hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다.
	nHeightDest		: 출력할 이미지의 높이 - 이 크기만큼 hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다
	hdcSrc			: 이미지의 핸들입니다.
	nXOriginSrc		: 가져올 이미지의 시작지점인 x좌표입니다.- 지정한 위치부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다.
	nYOriginSrc		: 가져올 이미지의 시작지점인 y좌표입니다.- 지정한 위치부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다.
	nWidthSrc		: 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 너비
	nHeightSrc		: 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 높이
	dwRop			: 이미지의 출력 방법입니다.

		************************************************/
//	if (btn_dbl_clk == FALSE)
//	{
//		BitBlt(hDC, 0, graph_height*i, rcWindow.Width(), graph_height, m_hMemDC, 0, graph_height*i, SRCCOPY);
//	}
//	else
//	{
//		BitBlt(hDC, (rcWindow.Width() / 2)*(i % 2), graph_height*(i / 2), rcWindow.Width() / 2, graph_height, m_hMemDC, (rcWindow.Width() / 2)*(i % 2), graph_height*(i / 2), SRCCOPY);
//	}	

#if (1)
	if (m_nViewSplit == 0)
	{
		BitBlt(hDC, 0, 0, rcWindow.Width(), rcWindow.Height(), m_hMemDC, 0, 0, SRCCOPY);
	}
	else
	{
		int x_start = (int)(rcData.left + (rcData.Width()*(double)m_nViewSplit*(1.0f / SPLIT_N)));
		BitBlt(hDC, x_start, 0, rcData.right - x_start, rcWindow.Height(), m_hMemDC, x_start, 0, SRCCOPY);
	}
#endif
	//StretchBlt(hDC, 50, 0, rcWindow.Width()-(stt_n-100), rcWindow.Height(), m_hMemDC, 50, 0, rcWindow.Width()-50, rcWindow.Height(), SRCCOPY);

//	if ((m_Mouse.x >= rcData.left + 5) && (m_Mouse.x <= rcData.right - 5) && (m_Mouse.y >= rcData.top + 5) && (m_Mouse.y <= rcData.bottom - 5))		// 표시영역 설정
//	{
//		CFont font, *pOldFont;
//		font.CreatePointFont(110, _T("Consolas"));	// 10point(x/10), _T("Consolas Bold")
//		pOldFont = (CFont*)dc.SelectObject(&font);
//		if (m_bLBtnDbClk == TRUE)
//		{
//			dc.SetTextColor(RGB(0, 0, 0));
//			dc.SetBkColor(RGB(255, 255, 255));
//		}
//		else
//		{		
//			dc.SetTextColor(RGB(255, 255, 255));	// while
//			dc.SetBkColor(RGB(30, 30, 30));
//		}
//		SelectObject(hDC, pOldFont);
//		DeleteObject(font);
//
//		int y_posTotal	= rcData.top - rcData.bottom;
//		int y_posTemp	= m_Mouse.y - rcData.bottom;
//		int y_datTotal	= (int)(dblYMax - dblYMin);
//		int y_datTemp	= (int)dblYMin + ((y_posTemp*y_datTotal) / y_posTotal);
//
//		CString str;
//		str.Format("%d", (int)y_datTemp);
//		//	str.Format("%d, %d", m_Mouse.x, m_Mouse.y);	// 좌표출력
//		dc.TextOut(m_Mouse.x, m_Mouse.y - 20, str);
//
//		HPEN hDotPen;
//		if (m_bLBtnDbClk == TRUE)
//		{
//			hDotPen = CreatePen(PS_DOT, 1, RGB(255, 96, 208));
//		}
//		else
//		{
//			hDotPen = CreatePen(PS_DOT, 1, RGB(96, 255, 128));
//		}
//		HPEN hOldPen = (HPEN)SelectObject(hDC, hDotPen);
//		MoveToEx(hDC, 50, m_Mouse.y, NULL);
//		LineTo(hDC, rcData.right, m_Mouse.y);
//
//		SelectObject(hDC, hOldPen);
//		DeleteObject(hDotPen);
//	}
}

void MLineChartCtrl::DrawChart(HDC hDC)
{
	static int iCnt = 0;

	// CREATE FONT
	HFONT hFont, hOldFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	strcpy_s(lf.lfFaceName, "Verdana");
	lf.lfHeight = 13;
	hFont = CreateFontIndirect(&lf);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	CalcDataRect();
	CalcAxis();

#if (0)
	DrawBG(hDC);
	DrawAxisX(hDC);
#endif

#if (1)
	CString szText;
	szText.Format("%d", nXDataSize);
	TextOut(hDC, (int)(rcData.right - 30), (int)rcData.bottom, szText, szText.GetLength());
	szText.Format("base:%d", m_iBaseLev[graph_x]);
	TextOut(hDC, (int)(rcData.left + 7), (int)rcData.bottom, szText, szText.GetLength());
	szText.Format("%d/%d", m_nBuffer, m_nViewPeriod);
	TextOut(hDC, (int)(rcData.left + 200), (int)rcData.bottom, szText, szText.GetLength());
#endif

	DrawAxisY(hDC);
	DrawAxisLine(hDC);
	DrawData(hDC);

#if (1)
	if ((m_Mouse.x >= rcData.left + 5) && (m_Mouse.x <= rcData.right - 5) && \
		(m_Mouse.y >= rcData.top + 5) && (m_Mouse.y <= rcData.bottom - 5))		// 표시영역 설정
	{
		if (m_iBKColor_i == 0)
		{
			SetTextColor(hDC, RGB(255, 255, 255));		// dc.SetTextColor(RGB(0, 0, 0)); 와 비교
			SetBkColor(hDC, RGB(30, 30, 30));			// dc.SetBkColor(RGB(255, 255, 255)); 와 비교
		}
		else if (m_iBKColor_i == 1)
		{
			SetTextColor(hDC, RGB(0, 0, 0));
			SetBkColor(hDC, RGB(255, 255, 255));
		}
		else if (m_iBKColor_i == 2)
		{
			SetTextColor(hDC, RGB(255, 255, 255));
			SetBkColor(hDC, RGB(140, 170, 230));
		}

		int y_width = rcData.top - rcData.bottom;
		int y_delta = m_Mouse.y - rcData.bottom;
		int d_width = (int)(dblYMax - dblYMin);
		int d_value = (int)dblYMin + ((y_delta*d_width) / y_width);

		if (m_bLClkDown == TRUE)
		{
			m_iBaseLev[graph_x] = d_value;
			m_bLClkDown = FALSE;
		}

		d_value -= m_iBaseLev[graph_x];

		CString str;
		str.Format("%d", (int)d_value);
		//str.Format("%d, %d", m_Mouse.x, m_Mouse.y);	// 좌표출력
		TextOut(hDC, m_Mouse.x, m_Mouse.y - 20, str, str.GetLength());	// dc.TextOut(m_Mouse.x, m_Mouse.y - 20, str); 와 비교

		HPEN hDotPen;	
		if (m_iBKColor_i == 0)		{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 0)); }
		else if (m_iBKColor_i == 1)	{ hDotPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0)); }
		else if (m_iBKColor_i == 2)	{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255)); }
		else						{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255)); }
		HPEN hOldPen = (HPEN)SelectObject(hDC, hDotPen);
		
		MoveToEx(hDC, 50, m_Mouse.y, NULL);
		LineTo(hDC, rcData.right, m_Mouse.y);

		SelectObject(hDC, hOldPen);
		DeleteObject(hDotPen);
	}
#endif

	// DELETE FONT
	SelectObject(hDC, hFont);
	DeleteObject(hFont);
}

void MLineChartCtrl::CalcDataRect()
{
	// CALC DATA RECT
	if (btn_dbl_clk == FALSE)
	{
		rcData.left		= rcWindow.left			+ m_ChartConfig.nMarginLeft;
		rcData.right	= rcWindow.right		- m_ChartConfig.nMarginRight;
		rcData.top		= graph_height*graph_x		+ m_ChartConfig.nMarginTop;
		rcData.bottom	= graph_height*(graph_x+1) - m_ChartConfig.nMarginBottom;
	}
	else
	{
		if (graph_x % 2)
		{
			rcData.left = rcWindow.left + rcWindow.Width() / 2;
			rcData.right = rcWindow.right;
		}
		else
		{
			rcData.left = rcWindow.left;
			rcData.right = rcWindow.right / 2;
		}

		rcData.top = graph_height*(graph_x / 2);
		rcData.bottom = rcData.top + graph_height;

		rcData.left		+= m_ChartConfig.nMarginLeft;
//		rcData.right	-= m_ChartConfig.nMarginRight;
		rcData.top		+= m_ChartConfig.nMarginTop;
		rcData.bottom	-= m_ChartConfig.nMarginBottom;
	}
}

void MLineChartCtrl::CalcAxis()
{
	// X축 계산	
//	
	if (draw_data[graph_x * item_n].lstData.GetSize()>m_ChartConfig.nInitWidth)
	{
//		nXDataSize = m_ChartConfig.nInitWidth + (int)(((double)(draw_data[graph_x * item_n].lstData.GetSize() - m_ChartConfig.nInitWidth) / (double)m_ChartConfig.nWidthExtent)*(double)m_ChartConfig.nWidthExtent);
		nXDataSize = (int)draw_data[graph_x * item_n].lstData.GetSize();
	}
	else
	{
		nXDataSize = m_ChartConfig.nInitWidth;
	}
	dblXIntv = rcData.Width()/(double)nXDataSize;


//	// Y축 계산
//	dblYCenter = (draw_data[0].nYMax+draw_data[0].nYMin)/2;
//	if(draw_data[0].nYMax-draw_data[0].nYMin<1)
//	{
//		dblYMin = draw_data[0].nYMin - 1;
//		dblYMax = draw_data[0].nYMax + 1;
//		dblYDataSize = draw_data[0].nYMax-draw_data[0].nYMin + 2;
//	}
//	else
//	{
//		dblYDataSize = draw_data[0].nYMax - draw_data[0].nYMin;
//		dblYMin = draw_data[0].nYMin - dblYDataSize*.2;
//		dblYMax = draw_data[0].nYMax + dblYDataSize*.2;
//		dblYDataSize = dblYMax - dblYMin;
//	}
//	dblYPixelSize = rcData.Height()/dblYDataSize;


	// Y축 계산
	for (int i = 0; i<item_n; i++)
	{
		if (m_bItemEn[i] == TRUE)
		{
			if (draw_data[graph_x*item_n + i].nYMax > y_axis_max)
			{
				y_axis_max = draw_data[graph_x*item_n + i].nYMax;
			}
			if (draw_data[graph_x*item_n + i].nYMin < y_axis_min)
			{
				y_axis_min = draw_data[graph_x*item_n + i].nYMin;
			}
		}
	}
	dblYCenter = (y_axis_max + y_axis_min) / 2;

	if (y_axis_max - y_axis_min<1)
	{
		dblYMin = y_axis_min - 1;
		dblYMax = y_axis_max + 1;
		dblYDataSize = y_axis_max - y_axis_min + 2;
	}
	else
	{
		dblYDataSize = y_axis_max - y_axis_min;
		dblYMin = y_axis_min - dblYDataSize*.1;
		dblYMax = y_axis_max + dblYDataSize*.1;
		dblYDataSize = dblYMax - dblYMin;
	}
	dblYPixelSize = rcData.Height() / dblYDataSize;
}

void MLineChartCtrl::DrawBG(HDC hDC)
{
	HBRUSH hBrush = CreateSolidBrush(m_ChartConfig.crBG);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	FillRect(hDC, rcWindow, hBrush);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

void MLineChartCtrl::DrawAxisLine(HDC hDC)
{
	// DRAW AXIS LINE	
	HPEN hAxisLine = CreatePen(PS_SOLID, 2, m_ChartConfig.crAxis);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);

	MoveToEx(hDC, rcData.left, rcData.top, NULL);
	LineTo(hDC, rcData.left, rcData.bottom);
	LineTo(hDC, rcData.right, rcData.bottom);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hAxisLine);
}

void MLineChartCtrl::DrawAxisX(HDC hDC)
{
	// Text Color
	SetTextColor(hDC, m_ChartConfig.crText);
	SetBkColor(hDC, m_ChartConfig.crTextBG);
	
	// X축 그리기
	HPEN hAxisLine = CreatePen(PS_SOLID, 1, m_ChartConfig.crGuideLine);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);

	int nAxisRight = rcData.left;
	CString szText;
	for(int i=0; i<nXDataSize; i++)
	{
		MoveToEx(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom, NULL);
		LineTo(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom+5);

		szText.Format("%d", i+1);

		if(i == 0 || nAxisRight+5 < rcData.left + dblXIntv*i)
		{
			TextOut(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom +5, szText, szText.GetLength());

			SIZE size;
			GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);

			nAxisRight = (int)(rcData.left + dblXIntv*i + size.cx);
		}
	}

	SIZE sizeText;
	GetTextExtentPoint32(hDC, m_ChartConfig.szAxisXNM, m_ChartConfig.szAxisXNM.GetLength(), &sizeText);

	TextOut(hDC, rcWindow.right - sizeText.cx - 5, rcWindow.bottom - sizeText.cy - 5, m_ChartConfig.szAxisXNM, m_ChartConfig.szAxisXNM.GetLength());

	SelectObject(hDC, hOldPen);
	DeleteObject(hAxisLine);
}

void MLineChartCtrl::DrawAxisY(HDC hDC)
{
	static int sttCnt = 0;

	SetTextColor(hDC, m_ChartConfig.crText);
	SetBkColor(hDC, m_ChartConfig.crTextBG);

	// Y축 그리기
	HPEN hAxisLine = CreatePen(PS_SOLID, 1, m_ChartConfig.crGuideLine);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);
	CString szText;
	SIZE size;
	
	// MIN, CENTER, MAX
	MoveToEx(hDC, rcData.left - 4, rcData.top, NULL);
	LineTo(hDC, rcData.right, rcData.top);
	szText.Format(m_ChartConfig.szFormat, (int)dblYMax);
	GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	TextOut(hDC, rcData.left - 6 - size.cx, rcData.top - size.cy / 2, szText, szText.GetLength());	// y축에서 6pixel 떨어져서 숫자표시
	
	MoveToEx(hDC, rcData.left - 4, rcData.CenterPoint().y, NULL);
	LineTo(hDC, rcData.right, rcData.CenterPoint().y);
	szText.Format(m_ChartConfig.szFormat, (int)dblYCenter);
	GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	TextOut(hDC, rcData.left - 6 - size.cx, rcData.CenterPoint().y - size.cy / 2, szText, szText.GetLength());
	
	MoveToEx(hDC, rcData.left - 4, rcData.bottom, NULL);
	LineTo(hDC, rcData.right, rcData.bottom);
	szText.Format(m_ChartConfig.szFormat, (int)dblYMin);
	GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	TextOut(hDC, rcData.left - 6 - size.cx, rcData.bottom - size.cy / 2, szText, szText.GetLength());
	
	if (rcData.Height() > 200)	// MIN-CENTER중간, CENTER-MAX중간
	{
		MoveToEx(hDC, rcData.left - 4, (rcData.CenterPoint().y + rcData.top) / 2, NULL);
		LineTo(hDC, rcData.right, (rcData.CenterPoint().y + rcData.top) / 2);
	
		szText.Format(m_ChartConfig.szFormat, (int)(dblYMax + dblYCenter) / 2);
		GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
		TextOut(hDC, rcData.left - 6 - size.cx, (rcData.top + rcData.CenterPoint().y) / 2 - size.cy / 2, szText, szText.GetLength());
	
		MoveToEx(hDC, rcData.left - 4, (rcData.CenterPoint().y + rcData.bottom) / 2, NULL);
		LineTo(hDC, rcData.right, (rcData.CenterPoint().y + rcData.bottom) / 2);
	
		szText.Format(m_ChartConfig.szFormat, (int)(dblYMin + dblYCenter) / 2);
		GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
		TextOut(hDC, rcData.left - 6 - size.cx, (rcData.bottom + rcData.CenterPoint().y) / 2 - size.cy / 2, szText, szText.GetLength());
	}
	
	TextOut(hDC, 5, 5, m_ChartConfig.szAxisYNM, m_ChartConfig.szAxisYNM.GetLength());
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hAxisLine);
}
#if (0)
void MLineChartCtrl::DrawData(HDC hDC)
{
	// DRAW DATA
	int x, y;
	int preX = 0;
	int maxY = 0;

	HPEN hLinePen;
	HPEN hOldPen;
	static int itemIndx = 0;

	itemIndx++;
	itemIndx %= item_n;

//	if(m_ChartConfig.bShowDataLine)
	{
		for (int itemIndx = 0; itemIndx < item_n; itemIndx++)
		{
			if (m_bItemEn[itemIndx] == TRUE)
			{
				if ((itemIndx == 2) || (itemIndx == 3))
				{
					hLinePen = CreatePen(PS_DOT, 1, m_ChartConfig.crDataLine[itemIndx]);
				}
				else
				{
					hLinePen = CreatePen(PS_SOLID, 1, m_ChartConfig.crDataLine[itemIndx]);
				}
				hOldPen = (HPEN)SelectObject(hDC, hLinePen);
				SelectObject(hDC, hLinePen);
				int size = draw_data[graph_x * item_n + itemIndx].lstData.GetSize();
				//int intv = 1 + (size / (1800));
				int intv = 1;
		
				int trggr_i = 0;

#if (1)
				if (m_nViewSplit == 0)
				{
					trggr_i = 0;
				}
				else
				{
					trggr_i = (int)((double)size*(1.0f/SPLIT_N)*(double)m_nViewSplit);
				}			
#endif
				for (int i = trggr_i; i < size; i += intv)
				{
					x = (int)(rcData.left + dblXIntv*(i + 1));
					y = rcData.CenterPoint().y + (int)((dblYCenter - draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->iOrigin)*dblYPixelSize);

					if (i == trggr_i)
					{
						if (m_nViewSplit != 0)
						{
							MoveToEx(hDC, (int)(rcData.left + (rcData.Width()*(1.0f / SPLIT_N)*m_nViewSplit)), rcData.top, NULL);
							LineTo(hDC, (int)(rcData.left + (rcData.Width()*(1.0f / SPLIT_N)*m_nViewSplit)), rcData.bottom);
							MoveToEx(hDC, x, y, NULL);
						}
						else
						{
							MoveToEx(hDC, x, y, NULL);
						}
					}
					else
					{
						LineTo(hDC, x, y);
					}
					
					if (intv == 1)
					{
						if (draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->bPrint == TRUE)
						{
							CString strData;
							strData.Format("%d", draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->iIntv);
							TextOut(hDC, x, y - 20, strData, lstrlen(strData));
							DrawDotRect(hDC, x, y, 3, 3);
						}
					}
				}
				SelectObject(hDC, hOldPen);
				DeleteObject(hLinePen);
			}
		}
	}
}
#else
void MLineChartCtrl::DrawData(HDC hDC)
{
	// DRAW DATA
	int x, y;
	int preX = 0;
	int maxY = 0;
	int minY = 0;

	HPEN hLinePen;
	HPEN hOldPen;
	static int itemIndx = 0;

	itemIndx++;
	itemIndx %= item_n;

	//	if(m_ChartConfig.bShowDataLine)
	{
		for (int itemIndx = 0; itemIndx < item_n; itemIndx++)
		{
			if (m_bItemEn[itemIndx] == TRUE)
			{
				if ((itemIndx == 2) || (itemIndx == 3))
				{
					hLinePen = CreatePen(PS_DOT, 1, m_ChartConfig.crDataLine[itemIndx]);
				}
				else
				{
					hLinePen = CreatePen(PS_SOLID, 1, m_ChartConfig.crDataLine[itemIndx]);
				}
				hOldPen = (HPEN)SelectObject(hDC, hLinePen);
				SelectObject(hDC, hLinePen);
				int size = draw_data[graph_x * item_n + itemIndx].lstData.GetSize();


#if (0)
if (size > 1980)
{
	return;
}
#endif

int pixel = rcData.right - rcData.left;
int add = 1;
if (size > pixel)
{
	add = size / pixel;
}

				int trggr_i = 0;
				if (m_nViewSplit == 0)
				{
					trggr_i = 0;
				}
				else
				{
					trggr_i = (int)((double)size*(1.0f / SPLIT_N)*(double)m_nViewSplit);
				}

//				for (int i = trggr_i; i < size; i++)
				for (int i = trggr_i; i < size; i += add)
				{
					x = (int)(rcData.left + dblXIntv*(i + 1));
					y = rcData.CenterPoint().y + (int)((dblYCenter - draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->iOrigin)*dblYPixelSize);

					if (i == trggr_i)
					{
						if (m_nViewSplit != 0)
						{
							MoveToEx(hDC, (int)(rcData.left + (rcData.Width()*(1.0f / SPLIT_N)*m_nViewSplit)), rcData.top, NULL);
							LineTo(hDC, (int)(rcData.left + (rcData.Width()*(1.0f / SPLIT_N)*m_nViewSplit)), rcData.bottom);
							MoveToEx(hDC, x, y, NULL);
						}
						else
						{
							MoveToEx(hDC, x, y, NULL);
						}
						preX = x;
						maxY = y;
						minY = y;
					}
					else
					{
						if (x != preX)
						{
							if (maxY == minY)
							{
								LineTo(hDC, preX, maxY);
							}
							else
							{
								LineTo(hDC, preX, maxY);
								LineTo(hDC, preX, minY);
							}
							preX = x;
							maxY = y;
							minY = y;
						}
						else
						{
							if (y < maxY)	maxY = y;
							else			minY = y;
							continue;
						}
					}

					if (draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->bPrint == TRUE)	/* GUN_MARK */
					{
						CString strData;
						strData.Format("%d", draw_data[graph_x * item_n + itemIndx].lstData.GetAt(i)->iIntv);
						TextOut(hDC, x, y - 20, strData, lstrlen(strData));
						DrawDotRect(hDC, x, y, 2, 2);
					}
				}
				SelectObject(hDC, hOldPen);
				DeleteObject(hLinePen);
			}
		}
	}
}
#endif
void MLineChartCtrl::DrawDotRect(HDC hDC, int x, int y, int nW, int nH)
{
	Rectangle(hDC, x-nW, y-nH, x+nW, y+nH);
}

void MLineChartCtrl::OnSize(UINT nType, int cx, int cy)
{
	CreateMemResource();

	CWnd::OnSize(nType, cx, cy);
}

void MLineChartCtrl::CreateMemResource()
{
	CRect r;
	CDC * pDC = GetDC();

	GetClientRect(r);

	if(m_hMemDC == NULL)
		m_hMemDC = CreateCompatibleDC(pDC->m_hDC);

	if(m_hMemBitmap != NULL)
		DeleteObject(m_hMemBitmap);

	m_hMemBitmap = CreateCompatibleBitmap(pDC->m_hDC, r.Width(), r.Height());
	SelectObject(m_hMemDC, m_hMemBitmap);

	ReleaseDC(pDC);
}


void MLineChartCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_Mouse = point;

	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}


void MLineChartCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (int i = 0; i < graph_n; i++)
	{
		for (int j = 0; j < item_n; j++)
		{
			draw_data[i * item_n + j].Clear();
		}
	}

	CWnd::OnRButtonDblClk(nFlags, point);

}


void MLineChartCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_bLBtnDbClk == TRUE)
	{
		m_bLBtnDbClk = FALSE;
	}
	else
	{
		m_bLBtnDbClk = TRUE;
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void MLineChartCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (btn_dbl_clk == TRUE)
	{
		btn_dbl_clk = FALSE;
	}
	else
	{
		btn_dbl_clk = TRUE;
	}
//	for (int i = 0; i < graph_n; i++)
//	{
//		for (int j = 0; j < item_n; j++)
//		{
//			draw_data[i * item_idx + j].Clear();
//		}
//	}
//	y_axis_max = 0;
//	y_axis_min = 0;

	CWnd::OnMButtonDblClk(nFlags, point);
}


BOOL MLineChartCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}


void MLineChartCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLClkDown = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void MLineChartCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//::PostMessage(hCommWnd, GRPH_CLEAR, 0, 0);
	m_nViewPeriod = 0;

	CWnd::OnRButtonDown(nFlags, point);
}
