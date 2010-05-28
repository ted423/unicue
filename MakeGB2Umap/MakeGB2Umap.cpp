// MakeGB2Umap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#define APPENDOFFSET 0xFEA1

/* 0xFEA1 - 0xFEFE*/
static const wchar_t AppendGBKChar[]={
       0xE468,0xE469,0xE46A,0xE46B,0xE46C,0xE46D,0xE46E,/* 0xFEA0*/
0xE46F,0xE470,0xE471,0xE472,0xE473,0xE474,0xE475,0xE476,/* 0xFEA8*/
0xE477,0xE478,0xE479,0xE47A,0xE47B,0xE47C,0xE47D,0xE47E,/* 0xFEB0*/
0xE47F,0xE480,0xE481,0xE482,0xE483,0xE484,0xE485,0xE486,/* 0xFEB8*/
0xE487,0xE488,0xE489,0xE48A,0xE48B,0xE48C,0xE48D,0xE48E,/* 0xFEC0*/
0xE48F,0xE490,0xE491,0xE492,0xE493,0xE494,0xE495,0xE496,/* 0xFEC8*/
0xE497,0xE498,0xE499,0xE49A,0xE49B,0xE49C,0xE49D,0xE49E,/* 0xFED0*/
0xE49F,0xE4A0,0xE4A1,0xE4A2,0xE4A3,0xE4A4,0xE4A5,0xE4A6,/* 0xFED8*/
0xE4A7,0xE4A8,0xE4A9,0xE4AA,0xE4AB,0xE4AC,0xE4AD,0xE4AE,/* 0xFEE0*/
0xE4AF,0xE4B0,0xE4B1,0xE4B2,0xE4B3,0xE4B4,0xE4B5,0xE4B6,/* 0xFEE8*/
0xE4B7,0xE4B8,0xE4B9,0xE4BA,0xE4BB,0xE4BC,0xE4BD,0xE4BE,/* 0xFEF0*/
0xE4BF,0xE4C0,0xE4C1,0xE4C2,0xE4C3,0xE4C4,0xE4C5        /* 0xFEF8*/
};

unsigned char CharToHex(char ch) 
{ 
	//0-9 
	if (ch>='0'&&ch<='9') 
		return (ch-'0');         
	//9-15 
	if (ch>='A'&&ch<='F') 
		return (ch-'A'+0xA); 
	//9-15
	if (ch>='a'&&ch<='f') 
		return (ch-'a'+0xA);

	return 0;
}

char SingleHexToChar(int SingleHex)
{
	if ((SingleHex<0)||(SingleHex>=16))
		return ' ';
	
	if (SingleHex<=9)
		return ('0'+SingleHex);
	
	return ('A'+SingleHex-10);
}

//必须预留4个byte以上的空间给dst
void HexToChar(char* dst,unsigned int WideChar)
{
	if (!dst)
		return;
	int D4,D3,D2,D1;
	unsigned int TempChar=WideChar;
	D1=TempChar%16;
	TempChar=TempChar>>4;
	D2=TempChar%16;
	TempChar=TempChar>>4;
	D3=TempChar%16;
	TempChar=TempChar>>4;
	D4=TempChar%16;
	*dst=SingleHexToChar(D4);
	*(dst+1)=SingleHexToChar(D3);
	*(dst+2)=SingleHexToChar(D2);
	*(dst+3)=SingleHexToChar(D1);
}

bool MakeFullCP936()
{
	string inFilename="CP936-raw.txt";
	ifstream infile(inFilename.c_str(),ios::binary);
	if (!infile)
	{
		cerr<<"Unable to open CP936-raw.txt!\n";
		return false;
	}
	ofstream outfile("CP936-full.txt",ios::out);
	if (!outfile)
	{
		cerr<<"Unable to create CP936-full.txt!\n";
		return false;
	}
	int length;
	char *buffer;
	infile.seekg(0,ios::end);
	length=infile.tellg();
	infile.seekg(0,ios::beg);
	buffer=new char[length];
	infile.read(buffer,length);
	infile.close();

	outfile.write(buffer,length);
	delete []buffer;
	buffer=NULL;

	int appendLength=sizeof(AppendGBKChar)/sizeof(AppendGBKChar[0]);
	char dst[5];
	memset(dst,0,5);

	for (int i=0;i<appendLength;i++)
	{
		outfile<<endl;
		HexToChar(dst,APPENDOFFSET+i);
		outfile<<"0x"<<dst;
		HexToChar(dst,AppendGBKChar[i]);
		outfile<<' '<<"0x"<<dst;
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*使用CP936-raw.txt创建完整映射表CP936-full.txt时取消下面的注释*/
	MakeFullCP936();

	string inFilename="CP936-full.txt";
	ifstream infile(inFilename.c_str());
	if (!infile)
	{
		cerr<<"Unable to open CP936-full.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("gb2u-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open gb2u-little-endian.map!\n";
		return -1;
	}

	char LITTLEENDIANBOM[2]={'\xFF','\xFE'};
	//outfile_littleendian.write(LITTLEENDIANBOM,2);

	string str;
	int i=0;
	int offset=0x8140;
	char zero[2]={'\xFD','\xFF'};
	while(getline(infile,str))
	{
		i++;

		//读取GBK码位
		int GBKoffset;
		GBKoffset=CharToHex(str[5])+CharToHex(str[4])*16+CharToHex(str[3])*16*16+CharToHex(str[2])*16*16*16;

		while(offset!=GBKoffset)
		{
			offset++;
			outfile_littleendian.write(zero,2);
		}
		offset++;

		//注意Unicode字符在内存中是先低位后高位
		//写入映射表可以以big_endian或little_endian方式
		//推荐采用little_endian方式

		unsigned char HighByte,LowByte;
		HighByte=CharToHex(str[9])*16 +CharToHex(str[10]);
		LowByte =CharToHex(str[11])*16+CharToHex(str[12]);

		if ((HighByte>255)||(LowByte>255))
			cerr<<"Error occur in Line "<<i<<"!\n";

		outfile_littleendian.write((char*)&LowByte,1);
		outfile_littleendian.write((char*)&HighByte,1);

	}
	cout<<i<<" lines done!\n";
	infile.close();
	outfile_littleendian.close();

	return 0;
}

