#pragma once

#include "MLineChartConfig.h"
#include "MLineChartData.h"


#define GRPH_CLEAR	(WM_USER +2)
#define SPLIT_N	4


// MLineChartCtrl

class MLineChartCtrl : public CWnd
{
	DECLARE_DYNAMIC(MLineChartCtrl)

public:
	MLineChartCtrl();
	virtual ~MLineChartCtrl();

public:
	MLineChartConfig m_ChartConfig;
	MLineChartData draw_data[CHART_DATA_N];

	HBITMAP m_hMemBitmap;
	HDC m_hMemDC;
	HDC m_hMemDC_cp;

	CRect rcWindow;
	CRect rcData;
//	HFONT hFont, hOldFont;
	double dblXIntv, dblYIntv;
//	float dblXIntv, dblYIntv;
	int nXDataSize;
	double dblYDataSize, dblYCenter, dblYMin, dblYMax, dblYPixelSize, dblYDataPerP;
//	float dblYDataSize, dblYCenter, dblYMin, dblYMax, dblYPixelSize, dblYDataPerP;

public:
	void DrawChart(HDC hDC);
	void CalcDataRect();
	void CalcAxis();
	void DrawBG(HDC hDC);
	void DrawAxisLine(HDC hDC);
	void DrawAxisX(HDC hDC);
	void DrawAxisY(HDC hDC);
	void DrawData(HDC hDC);
	void DrawDotRect(HDC hDC, int x, int y, int nW, int nH);
	void CreateMemResource();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CPoint m_Mouse;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	int graph_x;		// index
	int graph_n;		// count
	int graph_height;	// height
	int item_n;	
	double y_axis_max;
	double y_axis_min;
	BOOL m_bLBtnDbClk;
	BOOL btn_dbl_clk;
	BOOL m_bItemEn[8];
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	int m_iBKColor_i;
	int m_nViewSplit;
	bool m_bF2Evnt;
	int view_max;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int m_iBaseLev[10];
	bool m_bLClkDown;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int m_nBuffer;
	int m_nViewPeriod;
};
