// MakeB2Umap.cpp : 定义控制台应用程序的入口点。
//
#pragma once

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

unsigned char CharToHex(char ch) 
{ 
	//0-9 
	if (ch >= '0' && ch <= '9') 
		return (ch - '0');         
	//9-15 
	if (ch >= 'A' && ch <= 'F') 
		return (ch - 'A' + 0xA); 
	//9-15
	if (ch >= 'a' && ch <= 'f') 
		return (ch - 'a' + 0xA);

	return(0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	string inFilename="uao250-b2u.txt";
	ifstream infile(inFilename.c_str());
	if (!infile)
	{
		cerr<<"Unable to open b2u.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("b2u-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open b2u-little-endian.map!\n";
		return -1;
	}

	char LITTLEENDIANBOM[2]={'\xFF','\xFE'};
	//outfile_littleendian.write(LITTLEENDIANBOM,2);

	/*
	ofstream outfile_bigendian("b2u-big-endian.map",ios_base::binary);
	if (!outfile_bigendian)
	{
		cerr<<"Can not open b2u-big-endian.map!\n";
		return -1;
	}
	char BIGENDIANBOM[2]={'\xFE','\xFF'};
	outfile_bigendian.write(BIGENDIANBOM,2);
	*/

	string str;
	int i=0;
	int offset=0x8140;
	char zero[2]={'\xFD','\xFF'};
	while(getline(infile,str))
	{
		i++;

		//读取Big5码位
		int Big5offset;
		Big5offset=CharToHex(str[5])+CharToHex(str[4])*16+CharToHex(str[3])*16*16+CharToHex(str[2])*16*16*16;

		while(offset!=Big5offset)
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

		/*
		unsigned char bigEndian[2];
		bigEndian[0]=CharToHex(str[9])*16 +CharToHex(str[10]);
		bigEndian[1]=CharToHex(str[11])*16+CharToHex(str[12]);
		outfile_bigendian.write(bigEndian,2);
		*/
	}
	cout<<i<<" lines done!\n";
	infile.close();
	outfile_littleendian.close();
	//outfile_bigendian.close();

	return 0;
}