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
	WCHAR chr=0,UnknownChr=0xFFFD;
	for (UINT i=0;i<length;)
	{
		//GBK����ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//gbkChr[1]����λ
		//gbkChr[0]����λ
		memcpy(&high,gbkStr+i,1); //��ȡ��һ��byte
		i++;
		if (high>0x7F) //��һ��byte�Ǹ�λ
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
		if (chr<0x80) // ASCII��
			unicodeStr.AppendChar(chr);
		else if (chr<GBKOFFSET) // 0x80 - 0x813F δ����ռ�
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
	WCHAR chr=0,UnknownChr=0xFFFD;
	for (UINT i=0;i<length;)
	{
		//big5����ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//big5Chr[1]����λ
		//big5Chr[0]����λ
		memcpy(&high,big5Str+i,1); //��ȡ��һ��byte
		i++;
		if (high>0x7F) //��һ��byte�Ǹ�λ
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
		if (chr<0x80) // ASCII��
			unicodeStr.AppendChar(chr);
		else if (chr<BIG5OFFSET) // 0x80 - 0x813F δ����ռ�
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
	WCHAR chr=0,UnknownChr=0xFFFD;
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
		if (chr<0x80) // ASCII
			unicodeStr.AppendChar(chr);
		else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 δ����ռ�
			unicodeStr.AppendChar(UnknownChr);
		else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF ��Ǽ�����
		{
			int offset;
			offset=chr-JIS0201OFFSET;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			unicodeStr.AppendChar(chr);
		}
		else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F δ����ռ�
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

	//ת��
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

	//�ж��Ƿ���UTF-8����
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
		else // ����UTF-8�ַ���
		{
			isUTF8=FALSE;
			break;
		}
	}

	if (isUTF8)
		return CODETYPE_UTF8;

	UINT JISMapLength,GBKMapLength,BIG5MapLength;
	char *JISMapBuffer,*GBKMapBuffer,*BIG5MapBuffer;
	//����ӳ���
	TCHAR path[MAX_PATH]; //�260
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

	//�����룬˳����Shift-JIS>GBK>BIG5
	//���ͨ��ǰ������⣬�������������м��
	//����Ӧ��Unicode�ַ��Ƿ�Ϊ0xFFFD���ı�Խ����׼ȷ��Խ��
	//�������֮�����������
	CodeType strCodeType=CODETYPE_SHIFTJIS;
	unsigned char low=0,high=0;
	WCHAR chr=0;
	for (i=0;i<length;)
	{
		memcpy(&high,AnsiStr+i,1); //��ȡ��һ��byte
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
			memcpy(&low,AnsiStr+i,1); //��ȡ��λ
			i++;
		}

		chr=low+high*256;
		if (chr<0x80) // ASCII
		{}
		else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 δ����ռ�
		{
			strCodeType=CODETYPE_DEFAULT; // δ֪����
			break;
		}
		else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF ��Ǽ�����
		{}
		else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F δ����ռ�
		{
			strCodeType=CODETYPE_DEFAULT;  // δ֪����
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
			memcpy(&high,AnsiStr+i,1); //��ȡ��һ��byte
			i++;
			if (high>0x7F) //��һ��byte�Ǹ�λ
			{
				memcpy(&low,AnsiStr+i,1); //��ȡ��λ
				i++;
			}
			else
			{
				low=high;
				high=0;
			}

			chr=low+high*256;
			if (chr<0x80) // ASCII��
			{}
			else if (chr<GBKOFFSET) // 0x80 - 0x813F δ����ռ�
			{
				strCodeType=CODETYPE_DEFAULT;   // δ֪����
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
			memcpy(&high,AnsiStr+i,1); //��ȡ��һ��byte
			i++;
			if (high>0x7F) //��һ��byte�Ǹ�λ
			{
				memcpy(&low,AnsiStr+i,1); //��ȡ��λ
				i++;
			}
			else
			{
				low=high;
				high=0;
			}

			chr=low+high*256;
			if (chr<0x80) // ASCII��
			{}
			else if (chr<BIG5OFFSET) // 0x80 - 0x813F δ����ռ�
			{
				strCodeType=CODETYPE_DEFAULT;   // δ֪����
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

//һ���޳���ת������
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