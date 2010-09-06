/************************************************************************/
/*  Ansi to Unicode 1.0.3                                               */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

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
	WCHAR chr=0,UnknownChr=0xFFFD;
	for (UINT i=0;i<length;)
	{
		//GBK码的字符是先高位后低位
		//高位至少从0x81起
		//gbkChr[1]：高位
		//gbkChr[0]：低位
		memcpy(&high,gbkStr+i,1); //读取第一个byte
		i++;
		if (high>0x7F) //第一个byte是高位
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
		if (chr<0x80) // ASCII码
			unicodeStr.AppendChar(chr);
		else if (chr<GBKOFFSET) // 0x80 - 0x813F 未定义空间
			unicodeStr.AppendChar(UnknownChr);
		else // 0x8140 - 0xFFFF
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
	WCHAR chr=0,UnknownChr=0xFFFD;
	for (UINT i=0;i<length;)
	{
		//big5码的字符是先高位后低位
		//高位至少从0x81起
		//big5Chr[1]：高位
		//big5Chr[0]：低位
		memcpy(&high,big5Str+i,1); //读取第一个byte
		i++;
		if (high>0x7F) //第一个byte是高位
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
		if (chr<0x80) // ASCII码
			unicodeStr.AppendChar(chr);
		else if (chr<BIG5OFFSET) // 0x80 - 0x813F 未定义空间
			unicodeStr.AppendChar(UnknownChr);
		else // 0x8140 - 0xFFFF
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
	WCHAR chr=0,UnknownChr=0xFFFD;
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
		if (chr<0x80) // ASCII
			unicodeStr.AppendChar(chr);
		else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 未定义空间
			unicodeStr.AppendChar(UnknownChr);
		else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF 半角假名区
		{
			int offset;
			offset=chr-JIS0201OFFSET;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
		else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F 未定义空间
			unicodeStr.AppendChar(UnknownChr);
		else // 0x8140 - 0xFFFF
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

CString UTF8toUnicode(const char* utf8Str,UINT length)
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
		else if((0xE0&utf8Str[i])==0xC0) // 110xxxxx 10xxxxxx
		{
			chr =(utf8Str[i+0]&0x3F)<<6;
			chr|=(utf8Str[i+1]&0x3F);
			i+=2;
		}
		else if((0xF0&utf8Str[i])==0xE0) // 1110xxxx 10xxxxxx 10xxxxxx
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
		return UTF8toUnicode(AnsiStr,length);
	default:
		return CString(AnsiStr);
	}
}

CodeType CheckCodeType(const char* AnsiStr,UINT length,CodeType OldCodeType)
{
	if (!AnsiStr)
		return CODETYPE_DEFAULT;
	if (length==0)
		return CODETYPE_DEFAULT;

	//判断是否是UTF-8编码
	UINT i=0;
	BOOL isUTF8=TRUE;
	while (i<length)
	{
		if ((0x80&AnsiStr[i])==0) // ASCII
		{
			i++;
			continue;
		}
		else if((0xE0&AnsiStr[i])==0xC0) // 110xxxxx
		{
			if (AnsiStr[i+1]=='\0')
			{
				isUTF8=FALSE;
				break;
			}
			if ((0xC0&AnsiStr[i+1])==0x80) // 10xxxxxx
			{
				i+=2;
				continue;
			}
			else
			{
				isUTF8=FALSE;
				break;
			}
		}
		else if((0xF0&AnsiStr[i])==0xE0) // 1110xxxx
		{
			if (AnsiStr[i+1]=='\0')
			{
				isUTF8=FALSE;
				break;
			}
			if (AnsiStr[i+2]=='\0')
			{
				isUTF8=FALSE;
				break;
			}
			if (((0xC0&AnsiStr[i+1])==0x80)&&((0xC0&AnsiStr[i+2])==0x80)) // 10xxxxxx 10xxxxxx
			{
				i+=3;
				continue;
			}
			else
			{
				isUTF8=FALSE;
				break;
			}
		}
		else // 不是UTF-8字符串
		{
			isUTF8=FALSE;
			break;
		}
	}

	if (isUTF8)
		return CODETYPE_UTF8;

	UINT JISMapLength,GBKMapLength,BIG5MapLength;
	char *JISMapBuffer,*GBKMapBuffer,*BIG5MapBuffer;
	//加载映射表
	TCHAR path[MAX_PATH]; //最长260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);
	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	CString mapFolder=mapPath;
	CFile loadMap;

	mapPath=mapFolder+_T("\\jis2u-little-endian.map");
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("JIStoUnicode map loading error!"));
		return CODETYPE_DEFAULT;
	}
	JISMapLength=loadMap.GetLength();
	JISMapBuffer=new char[JISMapLength];
	loadMap.Read((void *)JISMapBuffer,JISMapLength);
	loadMap.Close();

	mapPath=mapFolder+_T("\\gb2u-little-endian.map");
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("GBKtoUnicode map loading error!"));
		if (JISMapBuffer)
			delete []JISMapBuffer;
		return CODETYPE_DEFAULT;
	}
	GBKMapLength=loadMap.GetLength();
	GBKMapBuffer=new char[GBKMapLength];
	loadMap.Read((void *)GBKMapBuffer,GBKMapLength);
	loadMap.Close();

	mapPath=mapFolder+_T("\\b2u-little-endian.map");
	if (!loadMap.Open(mapPath,CFile::modeRead))
	{
		loadMap.Close();
		::AfxMessageBox(_T("Big5toUnicode map loading error!"));
		if (JISMapBuffer)
			delete []JISMapBuffer;
		if (GBKMapBuffer)
			delete []GBKMapBuffer;
		return CODETYPE_DEFAULT;
	}
	BIG5MapLength=loadMap.GetLength();
	BIG5MapBuffer=new char[BIG5MapLength];
	loadMap.Read((void *)BIG5MapBuffer,BIG5MapLength);
	loadMap.Close();

	//检测编码，顺序是Shift-JIS>GBK>BIG5
	//如果通过前面编码检测，则跳过后面所有检测
	//检查对应的Unicode字符是否为0xFFFD，文本越长，准确度越高
	//语义分析之类是无理的了
	CodeType strCodeType=CODETYPE_SHIFTJIS;
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (i=0;i<length;)
	{
		memcpy(&high,AnsiStr+i,1); //读取第一个byte
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
			memcpy(&low,AnsiStr+i,1); //读取低位
			i++;
		}

		chr=low+high*256;
		if (chr<0x80) // ASCII
		{}
		else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 未定义空间
		{
			strCodeType=CODETYPE_DEFAULT; // 未知编码
			break;
		}
		else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF 半角假名区
		{}
		else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F 未定义空间
		{
			strCodeType=CODETYPE_DEFAULT;  // 未知编码
			break;
		}
		else // 0x8140 - 0xFFFF
		{
			int offset;
			offset=chr-JIS0208OFFSET+JIS0201LENGTH;
			memcpy((void*)&chr,JISMapBuffer+offset*2,2);
			if (chr==0xFFFD)
			{
				strCodeType=CODETYPE_GBK;
				break;
			}
		}
	}

	if (strCodeType==CODETYPE_GBK)
	{
		for (i=0;i<length;)
		{
			memcpy(&high,AnsiStr+i,1); //读取第一个byte
			i++;
			if (high>0x7F) //第一个byte是高位
			{
				memcpy(&low,AnsiStr+i,1); //读取低位
				i++;
			}
			else
			{
				low=high;
				high=0;
			}

			chr=low+high*256;
			if (chr<0x80) // ASCII码
			{}
			else if (chr<GBKOFFSET) // 0x80 - 0x813F 未定义空间
			{
				strCodeType=CODETYPE_DEFAULT;   // 未知编码
				break;
			}
			else
			{
				int offset;
				offset=chr-GBKOFFSET;
				memcpy((void*)&chr,GBKMapBuffer+offset*2,2);
				if (chr==0xFFFD)
				{
					strCodeType=CODETYPE_BIG5;
					break;
				}
			}
		}
	}

	if (strCodeType==CODETYPE_BIG5)
	{
		for (i=0;i<length;)
		{
			memcpy(&high,AnsiStr+i,1); //读取第一个byte
			i++;
			if (high>0x7F) //第一个byte是高位
			{
				memcpy(&low,AnsiStr+i,1); //读取低位
				i++;
			}
			else
			{
				low=high;
				high=0;
			}

			chr=low+high*256;
			if (chr<0x80) // ASCII码
			{}
			else if (chr<BIG5OFFSET) // 0x80 - 0x813F 未定义空间
			{
				strCodeType=CODETYPE_DEFAULT;   // 未知编码
				break;
			}
			else
			{
				int offset;
				offset=chr-BIG5OFFSET;
				memcpy((void*)&chr,BIG5MapBuffer+offset*2,2);
				if (chr==0xFFFD)
				{
					strCodeType=CODETYPE_DEFAULT;
					break;
				}
			}
		}
	}

	if (JISMapBuffer)
		delete []JISMapBuffer;
	JISMapBuffer=NULL;
	if (GBKMapBuffer)
		delete []GBKMapBuffer;
	GBKMapBuffer=NULL;
	if (BIG5MapBuffer)
		delete []BIG5MapBuffer;
	BIG5MapBuffer=NULL;

	return strCodeType;
}

//一组无长度转换函数
CString UTF8toUnicode(const char* utf8Str)
{
	UINT theLength=strlen(utf8Str);
	return UTF8toUnicode(utf8Str,theLength);
}

CString GBKtoUnicode(const char* gbkStr)
{
	UINT theLength=strlen(gbkStr);
	return GBKtoUnicode(gbkStr,theLength);
}
CString Big5toUnicode(const char* big5Str)
{
	UINT theLength=strlen(big5Str);
	return Big5toUnicode(big5Str,theLength);
}

CString JIStoUnicode(const char* jisStr)
{
	UINT theLength=strlen(jisStr);
	return JIStoUnicode(jisStr,theLength);
}

CodeType CheckCodeType(const char* AnsiStr,CodeType OldCodeType)
{
	UINT theLength=strlen(AnsiStr);
	return CheckCodeType(AnsiStr,theLength,OldCodeType);
}

CString AnsiToUnicode(const char* AnsiStr,CodeType codetype)
{
	switch(codetype)
	{
	case CODETYPE_GBK:
		return GBKtoUnicode(AnsiStr);
	case CODETYPE_BIG5:
		return Big5toUnicode(AnsiStr);
	case CODETYPE_SHIFTJIS:
		return JIStoUnicode(AnsiStr);
	case CODETYPE_UTF8:
		return UTF8toUnicode(AnsiStr);
	default:
		return CString(AnsiStr);
	}
}