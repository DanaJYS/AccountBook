#include "GlobalValue.h"

MyTime EnTimeToChTime(const string& inTime)
{
	string strTime[6];
	int k = 0;
	int i = 0, j = 0;

	string strMonth[2][12] = {
		{"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"},
		{"01","02","03","04","05","06","07","08","09","10","11","12"}
	};

	while (inTime[j] != '\0')
	{
		j++;
		if (inTime[j]==' ' && i<j)
		{
			strTime[k++].assign(inTime, i, j-i);
			i = j+1;
		}
		else if (inTime[j]==' ' && i==j)
		{
			i = j+1;
		} 
	}
	if (k>=6)
		k = 5;
	if (i < j)
		strTime[k].assign(inTime, i, j-i-1);

	MyTime outTime;
	outTime.Year = strTime[k];
	for (int m=0; m<12; m++)
	{
		if (strTime[1] == strMonth[0][m])
		{
			outTime.Month = strMonth[1][m];
			break;
		}
	}

	outTime.Day = strTime[2];
	outTime.hmsTime = strTime[3];

	return outTime;
}

MyTime GetCurTime()
{
	char* locTime;
	time_t ltime;
	time(&ltime);
	tm* sltime = localtime(&ltime);
	locTime = asctime(sltime);
	//string curTime(locTime);
	string curTime = locTime;

	return EnTimeToChTime(curTime);
}

//-2:����-1:<��0:=��1:>
int CompareGridItem(const pMyGridItem pLeft, const pMyGridItem pRight)
{
	if (pLeft == NULL || pRight == NULL)
		return -2;

	//�Ƚ���Ŀ����
	if (pLeft->cItemType != pRight->cItemType)
		return pLeft->cItemType-pRight->cItemType;

	//�Ƚ���
	if (pLeft->stItemTime.Year < pRight->stItemTime.Year)
		return -1;
	else if (pLeft->stItemTime.Year > pRight->stItemTime.Year)
		return 1;

	//�Ƚ���
	if (pLeft->stItemTime.Month < pRight->stItemTime.Month)
		return -1;
	else if (pLeft->stItemTime.Month > pRight->stItemTime.Month)
		return 1;

	//�Ƚ���
	if (pLeft->stItemTime.Day < pRight->stItemTime.Day)
		return -1;
	else if (pLeft->stItemTime.Day > pRight->stItemTime.Day)
		return 1;

	//�Ƚ�ʱ����
	if (pLeft->stItemTime.hmsTime < pRight->stItemTime.hmsTime)
		return -1;
	else if (pLeft->stItemTime.hmsTime > pRight->stItemTime.hmsTime)
		return 1;

	//����Ŀ���࣬�����գ�ʱ���붼��ȣ��򷵻�0
	return 0;
}