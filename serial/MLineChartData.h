#pragma once

class MLineChartData
{
public:
	typedef struct __ITEM
	{
		int iOrigin;
		int iDest;
		int iIntv;
		bool bPrint;
	} __ITEM;

public:
	MLineChartData(void);
public:
	~MLineChartData(void);

public:
	CArray<__ITEM*, __ITEM*> lstData;
	double nYMin;
	double nYMax;
public:
	int Add(int iOrigin, int iDest, bool bPrint);	//*//
	void Clear();
};
