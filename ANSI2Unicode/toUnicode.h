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

//gbkStr���һ���ַ�Ϊ��ֹ��,length��������ֹ��
CString GBKtoUnicode(const char* gbkStr,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!gbkStr)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //�260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\gb2u-little-endian.map");

	//����ӳ���
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

	//ת��
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//GBK����ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//gbkChr[1]����λ
		//gbkChr[0]����λ
		memcpy(&high,gbkStr+i,1); //��ȡ��һ��byte
		i++;
		if (high>0x80) //��һ��byte�Ǹ�λ
		{
			memcpy(&low,gbkStr+i,1); //��ȡ��λ
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

//big5Str���һ���ַ�Ϊ��ֹ��,length��������ֹ��
CString Big5toUnicode(const char* big5Str,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!big5Str)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //�260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\b2u-little-endian.map");

	//����ӳ���
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

	//ת��
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//big5����ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//big5Chr[1]����λ
		//big5Chr[0]����λ
		memcpy(&high,big5Str+i,1); //��ȡ��һ��byte
		i++;
		if (high>0x80) //��һ��byte�Ǹ�λ
		{
			memcpy(&low,big5Str+i,1); //��ȡ��λ
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

//jisStr���һ���ַ�Ϊ��ֹ��,length��������ֹ��
CString JIStoUnicode(const char* jisStr,UINT length)
{
	CString unicodeStr;
	unicodeStr=_T("");

	if (!jisStr)
		return unicodeStr;

	if (length==0)
		return unicodeStr;

	TCHAR path[MAX_PATH]; //�260
	GetModuleFileName(NULL, path, MAX_PATH);
	CString mapPath=CString(path);

	int position=mapPath.ReverseFind('\\');
	mapPath=mapPath.Left(position);
	mapPath+=_T("\\jis2u-little-endian.map");

	//����ӳ���
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

	//ת��
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (UINT i=0;i<length;)
	{
		//Shift-JIS���˫�ֽ��ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//jisChr[1]����λ
		//jisChr[0]����λ
		memcpy(&high,jisStr+i,1); //��ȡ��һ��byte
		i++;
		if (high<=0x7F)  //ASCII����
		{
			low=high;
			high=0;
		}
		else if ((high>=0xA1)&&(high<=0xDF))  //���Ƭ������
		{
			low=high;
			high=0;
		}
		else  //˫�ֽ���
		{
			memcpy(&low,jisStr+i,1); //��ȡ��λ
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

	//ת��
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
		else // ����UTF-8�ַ���
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