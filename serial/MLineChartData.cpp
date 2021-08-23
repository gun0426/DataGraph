#include "StdAfx.h"
#include "MLineChartData.h"
#include <math.h>

MLineChartData::MLineChartData(void)
{

}

MLineChartData::~MLineChartData(void)
{
	Clear();
}

int MLineChartData::Add(int iOrigin, int iDest, bool bPrint)
{
	__ITEM* item = new __ITEM;
	item->iOrigin = iOrigin;
	item->iDest = iDest;
	item->iIntv = iOrigin - iDest;
	item->bPrint = bPrint;

	lstData.Add(item);

#if (0)
	if(lstData.GetSize() == 1)
	{
		nYMin = nYMax = item->iIntv;
	}
	else if(item->iIntv < nYMin)
	{
		nYMin = item->iIntv;
	}
	else if(item->iIntv > nYMax)
	{
		nYMax = item->iIntv;
	}
	return (int)lstData.GetSize();
#else
	if (lstData.GetSize() == 1)
	{
		nYMin = nYMax = item->iOrigin;
	}
	else if (item->iOrigin < nYMin)
	{
		nYMin = item->iOrigin;
	}
	else if (item->iOrigin > nYMax)
	{
		nYMax = item->iOrigin;
	}
	return (int)lstData.GetSize();
#endif
}

void MLineChartData::Clear()
{
	for(int i=lstData.GetSize()-1; i>=0; i--)
	{
		delete lstData.GetAt(i);
	}

	lstData.RemoveAll();
}