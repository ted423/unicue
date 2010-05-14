//big5 to Unicode转换函数
#pragma once
#include "stdafx.h"

#define BIG5OFFSET 0x8140

//big5Str最后一个字符为终止符
CString Big5ToUnicode(const char* big5Str,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!big5Str)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //最长260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\b2u-little-endian.map");

	//加载映射表
	CFile loadMap;
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("Big5toUnicode map loading error!"));
		return unicodeStr;
	}

	UINT mapLength=loadMap.GetLength();
	char *mapBuffer=new char[mapLength];
	loadMap.Read((void *)mapBuffer,mapLength);
	loadMap.Close();

	//转换
	unsigned char low=0,high=0;
	TCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//big5码的字符是先高位后低位
		//高位至少从0x81起
		//big5Chr[1]：高位
		//big5Chr[0]：低位
		memcpy(&high,big5Str+i,1); //读取第一个byte
		i++;
		if (high>0x80) //第一个byte是高位
		{
			memcpy(&low,big5Str+i,1); //读取低位
			i++;
		}
		else
		{
			low=high;
			high=0;
		}

		chr=low+high*256;
		if (chr<BIG5OFFSET)
		{
			unicodeStr.AppendChar(chr);
		}
		else
		{
			int offset;
			offset=chr-BIG5OFFSET;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
	}

	delete []mapBuffer;
	mapBuffer=NULL;

	return unicodeStr;
}