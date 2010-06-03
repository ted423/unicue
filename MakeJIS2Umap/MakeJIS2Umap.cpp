// MakeJIS2Umap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

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

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream infile1("CP932-1.txt");
	if (!infile1)
	{
		cerr<<"Unable to open CP932-1.txt!\n";
		return -1;
	}

	ifstream infile2("CP932-2.txt");
	if (!infile2)
	{
		cerr<<"Unable to open CP932-2.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("jis2u-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open gb2u-little-endian.map!\n";
		return -1;
	}

	char LITTLEENDIANBOM[2]={'\xFF','\xFE'};
	//outfile_littleendian.write(LITTLEENDIANBOM,2);

	string str;
	int i=0;
	while(getline(infile1,str))
	{
		i++;

		unsigned char HighByte,LowByte;
		HighByte=CharToHex(str[7])*16 +CharToHex(str[8]);
		LowByte =CharToHex(str[9])*16+CharToHex(str[10]);

		if ((HighByte>255)||(LowByte>255))
			cerr<<"Error occur in CP932-1.txt's Line "<<i<<"!\n";

		outfile_littleendian.write((char*)&LowByte,1);
		outfile_littleendian.write((char*)&HighByte,1);
	}
	cout<<i<<" lines in CP932-1.txt done!\n";
	infile1.close();

	i=0;
	int offset=0x8140;
	char zero[2]={'\xFD','\xFF'};
	while(getline(infile2,str))
	{
		i++;

		//读取Shift-JIS码位
		int JISoffset;
		JISoffset=CharToHex(str[5])+CharToHex(str[4])*16+CharToHex(str[3])*16*16+CharToHex(str[2])*16*16*16;

		while(offset!=JISoffset)
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

	cout<<i<<" lines in CP932-2.txt done!\n";
	infile2.close();

	/*
	//补齐至0xFEFE
	while(offset!=0xFEFE)
	{
		offset++;
		outfile_littleendian.write(zero,2);
	}
	outfile_littleendian.write(zero,2);
	*/

	outfile_littleendian.close();

	return 0;
}
