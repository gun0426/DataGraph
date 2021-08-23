#pragma once

#include "MLineChartCtrl.h"


// CGraph 대화 상자입니다.

class CGraph : public CDialogEx
{
	DECLARE_DYNAMIC(CGraph)

public:
	CGraph(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraph();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GRAPH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	MLineChartCtrl m_LineChartCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool m_bWindow;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_iViewSize;
};
