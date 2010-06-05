#pragma once

#define CODETYPE_DEFAULT  0
#define CODETYPE_GBK      1
#define CODETYPE_BIG5     2
#define CODETYPE_SHIFTJIS 3
#define CODETYPE_UTF8     4
#define CODETYPE_UNICODE  5

#define GBKOFFSET         0x8140
#define BIG5OFFSET        0x8140
#define JIS0201OFFSET     0xA1
#define JIS0201LENGTH     63
#define JIS0208OFFSET     0x8140

typedef int CodeType;

//gbkStr最后一个字符为终止符,length不包含终止符
CString GBKtoUnicode(const char* gbkStr,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!gbkStr)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //最长260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\gb2u-little-endian.map");

	//加载映射表
	CFile loadMap;
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("GBKtoUnicode map loading error!"));
		return unicodeStr;
	}

	UINT mapLength=loadMap.GetLength();
	char *mapBuffer=new char[mapLength];
	loadMap.Read((void *)mapBuffer,mapLength);
	loadMap.Close();

	//转换
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//GBK码的字符是先高位后低位
		//高位至少从0x81起
		//gbkChr[1]：高位
		//gbkChr[0]：低位
		memcpy(&high,gbkStr+i,1); //读取第一个byte
		i++;
		if (high>0x80) //第一个byte是高位
		{
			memcpy(&low,gbkStr+i,1); //读取低位
			i++;
		}
		else
		{
			low=high;
			high=0;
		}

		chr=low+high*256;
		if (chr<GBKOFFSET)
		{
			unicodeStr.AppendChar(chr);
		}
		else
		{
			int offset;
			offset=chr-GBKOFFSET;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
	}

	delete []mapBuffer;
	mapBuffer=NULL;

	return unicodeStr;
}

//big5Str最后一个字符为终止符,length不包含终止符
CString Big5toUnicode(const char* big5Str,UINT length)
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
	WCHAR chr=0;
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

//jisStr最后一个字符为终止符,length不包含终止符
CString JIStoUnicode(const char* jisStr,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!jisStr)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //最长260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\jis2u-little-endian.map");

	//加载映射表
	CFile loadMap;
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("JIStoUnicode map loading error!"));
		return unicodeStr;
	}

	UINT mapLength=loadMap.GetLength();
	char *mapBuffer=new char[mapLength];
	loadMap.Read((void *)mapBuffer,mapLength);
	loadMap.Close();

	//转换
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//Shift-JIS码的双字节字符是先高位后低位
		//高位至少从0x81起
		//jisChr[1]：高位
		//jisChr[0]：低位
		memcpy(&high,jisStr+i,1); //读取第一个byte
		i++;
		if (high<=0x7F)  //ASCII码区
		{
			low=high;
			high=0;
		}
		else if ((high>=0xA1)&&(high<=0xDF))  //半角片假名区
		{
			low=high;
			high=0;
		}
		else  //双字节区
		{
			memcpy(&low,jisStr+i,1); //读取低位
			i++;
		}

		chr=low+high*256;
		if (chr<JIS0201OFFSET)
		{
			unicodeStr.AppendChar(chr);
		}
		else if (chr<JIS0208OFFSET)
		{
			int offset;
			offset=chr-JIS0201OFFSET;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
		else
		{
			int offset;
			offset=chr-JIS0208OFFSET+JIS0201LENGTH;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
	}

	delete []mapBuffer;
	mapBuffer=NULL;

	return unicodeStr;
}

CString UTF8toUnicde(const char* utf8Str,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!utf8Str)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	//转换
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		if ((0x80&utf8Str[i])==0) // ASCII
		{
			chr=utf8Str[i];
			i++;
		}
		else if((0xE0&utf8Str[i])==0xC0) // 110xxxxx 10xxxxx
		{
			chr =(utf8Str[i+0]&0x3F)<<6;
			chr|=(utf8Str[i+1]&0x3F);
			i+=2;
		}
		else if((0xF0&utf8Str[i])==0xE0) // 1110xxxx 10xxxxx 10xxxxx
		{
			chr =(utf8Str[i+0]&0x1F)<<12;
			chr|=(utf8Str[i+1]&0x3F)<<6;
			chr|=(utf8Str[i+2]&0x3F);
			i+=3;
		}
		/*
		else if() // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{}
		else if() // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx  10xxxxxx
		{}
		else if() // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx  10xxxxxx 10xxxxxx 
		{}
		*/
		else // 不是UTF-8字符串
		{
			return unicodeStr;
		}
		unicodeStr.AppendChar(chr);
	}

	return unicodeStr;
}

CString AnsiToUnicode(const char* AnsiStr,UINT length, CodeType codetype)
{
	switch(codetype)
	{
	case CODETYPE_GBK:
		return GBKtoUnicode(AnsiStr,length);
	case CODETYPE_BIG5:
		return Big5toUnicode(AnsiStr,length);
	case CODETYPE_SHIFTJIS:
		return JIStoUnicode(AnsiStr,length);
	case CODETYPE_UTF8:
		return UTF8toUnicde(AnsiStr,length);
	default:
		return CString(AnsiStr);
	}
}

CodeType CheckCodeType(const char* AnsiStr,UINT length,CodeType OldCodeType)
{
	return OldCodeType;
}