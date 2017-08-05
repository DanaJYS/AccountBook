#pragma once
#include "wx/wx.h"
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

typedef unsigned char		uchar;
typedef unsigned int		uint;


typedef struct stTime
{
	string		Year;
	string		Month;
	string		Day;
	string		hmsTime;
}MyTime, *pMyTime;

typedef struct GridItem 
{
	uchar		cItemType;			//0:收入；1:支出
	MyTime		stItemTime;
	double		fItemNum;
	GridItem*	pPreItem;
	GridItem*	pNextItem;
}MyGridItem, *pMyGridItem;


MyTime EnTimeToChTime(const string& inTime);
MyTime GetCurTime();

int CompareGridItem(const pMyGridItem pLeft, const pMyGridItem pRight);