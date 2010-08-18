/************************************************************************/
/*  Extra script of Akaiito (PS2 game)                                  */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// ExtractAkaiito.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ��ȡ ���������� ���ı�

#include "stdafx.h"
#include "ExtractAkaiito.h"
#include "..\ANSI2Unicode\toUnicode.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define JIS0201OFFSET     0xA1
#define JIS0201LENGTH     63
#define JIS0208OFFSET     0x8140

// ɸѡ��Ч�ı��ַ�
/*
ASCII��0x00��0x7F�����޳����Ʒ�����Ч���ı��ַ���ΧΪ��0x20��0x7E
���Ƭ��������0xA1��0xDF
˫�ֽ�����0x8140��0xFFFF������δ�����ַ���jis2u-little-endian.map����ӳ�䵽unicode��0xFFFD
*/
UINT ReplateDumpChar(const char* src, char* dst, UINT length)
{
	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (length==0)
		return 0;

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
		return 0;
	}
	UINT mapLength=loadMap.GetLength();
	char *mapBuffer=new char[mapLength];
	loadMap.Read((void *)mapBuffer,mapLength);
	loadMap.Close();

	unsigned char low=0,high=0;
	WCHAR chr=0,UnknownChr=0xFFFD;
	int ReplaceChr=0x20;
	UINT j;
	for (UINT i=0;i<length;)
	{
		//Shift-JIS���˫�ֽ��ַ����ȸ�λ���λ
		//��λ���ٴ�0x81��
		//jisChr[1]����λ
		//jisChr[0]����λ
		memcpy(&high,src+i,1); //��ȡ��һ��byte
		j=i;
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
			memcpy(&low,src+i,1); //��ȡ��λ
			i++;
		}

		chr=low+high*256;
		if (chr<0x80) // ASCII
		{
			if ((chr>=0x20)&&(chr<=0x7E))
				memcpy(dst+j,src+j,1);
			else
				memset(dst+j,ReplaceChr,1);
		}
		else if (chr<JIS0201OFFSET) // 0x80 - 0xA0 δ����ռ�
		{
			memset(dst+j,ReplaceChr,1);
		}
		else if (chr<(JIS0201OFFSET+JIS0201LENGTH)) // 0xA1 - 0xDF ��Ǽ�����
		{
			//memcpy(dst+j,src+j,1);
			memset(dst+j,ReplaceChr,1); //Ӧ�ò�������Щ�ַ�
		}
		else if (chr<JIS0208OFFSET) // 0xE0 - 0x813F δ����ռ�
		{
			if (i==(j+2))
				memset(dst+j,ReplaceChr,2);
			else
				memset(dst+j,ReplaceChr,1);
		}
		else // 0x8140 - 0xFFFF
		{
			int offset;
			offset=chr-JIS0208OFFSET+JIS0201LENGTH;
			memcpy((void*)&chr,mapBuffer+offset*2,2);
			if (chr==0xFFFD)
				memset(dst+j,ReplaceChr,2);
			else
				memcpy(dst+j,src+j,2);
		}
	}

	delete []mapBuffer;
	mapBuffer=NULL;

	return length;
}

//ɾ�������ַ� $w $s ��
UINT DeleteDollarChar(const char* src, char* dst, UINT length)
{
	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	UINT j=0;
	char chr;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,src+i++,1);
		//��׽$
		if (chr==0x24)
		{
			memcpy(&chr,src+i++,1);
			if ((chr>=0x61)&&(chr<=0x7a))
			{
				//��׽�ɹ�
				memset(dst+j,0x20,1);
				j++;
			}
			else
			{
				memcpy(dst+j,src+i-2,2);
				j=j+2;
			}
		}
		else
		{
			memcpy(dst+j,src+i-1,1);
			j=j+1;
		}
	}

	return j;
}

//ɾ�������ַ�(����ascii�ַ�,0x21~0x7E)
UINT DeleteSingleChar(char* dst, UINT length)
{
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	char chr;
	int state=0;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,dst+i++,1);
		//��׽�ո�,�Զ���ģ��
		if (chr==0x20)
		{
			state=1;
			continue;
		}

		if ((state==1)&&(chr>=0x21)&&(chr<=0x7e))
			state=2;
		else
			state=0;

		if (state==2)
		{
			if (dst[i]==0x20)
			{
				memset(dst+i-1,0x20,1);
				state=1;
			}
			else
				state=0;
		}
	}

	return length;
}

//�滻�س��ַ� #cr0 Ϊ����
UINT ReplaceCR(const char* src, char* dst, UINT length)
{
	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	UINT j=0;
	char chr;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,src+i++,1);
		//��׽#cr0
		if (chr==0x23)
		{
			memcpy(&chr,src+i++,1);
			if (chr==0x63)
			{
				memcpy(&chr,src+i++,1);
				if (chr==0x72)
				{
					memcpy(&chr,src+i++,1);
					if (chr==0x30)
					{
						//��׽�ɹ�
						memset(dst+j,0x0D,1);
						j++;
						memset(dst+j,0x0A,1);
						j++;
					}
					else
					{
						memcpy(dst+j,src+i-4,4);
						j=j+4;
					}
				}
				else
				{
					memcpy(dst+j,src+i-3,3);
					j=j+3;
				}
			}
			else
			{
				memcpy(dst+j,src+i-2,2);
				j=j+2;
			}
		}
		else
		{
			memcpy(dst+j,src+i-1,1);
			j=j+1;
		}
	}

	return j;
}

//ɾ���س��ַ� #cr0
UINT DeleteCR(const char* src,char* dst,UINT length)
{
	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	UINT j=0;
	char chr;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,src+i++,1);
		//��׽#cr0
		if (chr==0x23)
		{
			memcpy(&chr,src+i++,1);
			if (chr==0x63)
			{
				memcpy(&chr,src+i++,1);
				if (chr==0x72)
				{
					memcpy(&chr,src+i++,1);
					if (chr==0x30)
					{
						//��׽�ɹ�
						//ʲôҲ���ø�
					}
					else
					{
						memcpy(dst+j,src+i-4,4);
						j=j+4;
					}
				}
				else
				{
					memcpy(dst+j,src+i-3,3);
					j=j+3;
				}
			}
			else
			{
				memcpy(dst+j,src+i-2,2);
				j=j+2;
			}
		}
		else
		{
			memcpy(dst+j,src+i-1,1);
			j=j+1;
		}
	}

	return j;
}

//�滻��������
UINT DeleteNumbers(char* dst, UINT length)
{
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	char chr;
	int state=0;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,dst+i++,1);
		//��׽�ո�,�Զ���ģ��
	}

	return length;
}

//���ո��滻Ϊ���У������ո����ϣ�
UINT ReplaceSpaceChar(const char* src, char* dst, UINT length)
{
	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (length==0)
		return 0;

	UINT j=0;
	char chr;
	int state=0;
	for (UINT i=0;i<length;)
	{
		memcpy(&chr,src+i++,1);
		if (chr!=0x20)
		{
			if (state==1)
			{
				memset(dst+j++,0x20,1);
			}
			if (state==2)
			{
				memset(dst+j++,0x20,1);
				memset(dst+j++,0x20,1);
			}
			if (state==3)
			{
				memset(dst+j++,0x0D,1);
				memset(dst+j++,0x0A,1);
			}
			state=0;
			memcpy(dst+j++,src+i-1,1);
		}
		else
		{
			if (state==0)
			{
				state=1; //һ���ո�
				continue;
			}
			if (state==1)
			{
				state=2; //�����ո�
				continue;
			}
			if (state==2)
			{
				state=3; //�����ո񣬿���׼���滻
				continue;
			}
			if (state==3)
			{
				state=3;
				continue;
			}
		}
	}

	return j;
}

// Ψһ��Ӧ�ó������
CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		if (argc>=2)
		{
			//��һ����ɸѡ��Ч�ַ�
			if (wcscmp(argv[1],_T("-step1"))==0)
			{
				CFile OpenFile;
				if (!OpenFile.Open(_T("SCRIPT.AFS"),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
				{
					OpenFile.Close();
					::AfxMessageBox(_T("��ʧ�ܣ�����SCRIPT.AFS�Ƿ��ѷ����ڳ���Ŀ¼"),MB_OK);
					return 0;
				}
				UINT length;
				length=OpenFile.GetLength();
				char *akaiito_script; // ע��տ�ʼʱ���ܵ����ַ���������ԭʼ�ļ����д�����ֹ��
				akaiito_script=new char[length];
				OpenFile.Read((void*)akaiito_script,length);
				OpenFile.Close();
				char *akaiito_script_dealing;
				akaiito_script_dealing=new char[length];
				memset((void*)akaiito_script_dealing,0,length);
				ReplateDumpChar(akaiito_script,akaiito_script_dealing,length);

				CFile SaveFile;
				if (!SaveFile.Open(_T("akaiito_script_step1.txt"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
				{
					::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
					if (!akaiito_script)
						delete []akaiito_script;
					akaiito_script=NULL;
					if (!akaiito_script_dealing)
						delete []akaiito_script_dealing;
					akaiito_script_dealing=NULL;
					return 0;
				}
				SaveFile.Write(akaiito_script_dealing,length);
				SaveFile.Close();

				if (!akaiito_script)
					delete []akaiito_script;
				akaiito_script=NULL;
				if (!akaiito_script_dealing)
					delete []akaiito_script_dealing;
				akaiito_script_dealing=NULL;
			}
			//�ڶ�����
			if (wcscmp(argv[1],_T("-step2"))==0)
			{
				CFile OpenFile;
				if (!OpenFile.Open(_T("akaiito_script_step1.txt"),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
				{
					OpenFile.Close();
					::AfxMessageBox(_T("��ʧ�ܣ�����akaiito_script_step1.txt�Ƿ����Ŀ¼���Ѵ���"),MB_OK);
					return 0;
				}
				UINT length;
				length=OpenFile.GetLength();
				char *akaiito_script; // �ڶ�����ʼ�Ѿ����Ե����ַ���������
				akaiito_script=new char[length];
				OpenFile.Read((void*)akaiito_script,length);
				OpenFile.Close();
				char *akaiito_script_dealing;
				akaiito_script_dealing=new char[length];

				memset((void*)akaiito_script_dealing,0,length);
				length=DeleteDollarChar(akaiito_script,akaiito_script_dealing,length);
				length=DeleteSingleChar(akaiito_script_dealing,length);

				CFile SaveFile;
				if (!SaveFile.Open(_T("akaiito_script_step2.txt"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
				{
					::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
					if (!akaiito_script)
						delete []akaiito_script;
					akaiito_script=NULL;
					if (!akaiito_script_dealing)
						delete []akaiito_script_dealing;
					akaiito_script_dealing=NULL;
					return 0;
				}
				SaveFile.Write(akaiito_script_dealing,length);
				SaveFile.Close();

				if (!akaiito_script)
					delete []akaiito_script;
				akaiito_script=NULL;
				if (!akaiito_script_dealing)
					delete []akaiito_script_dealing;
				akaiito_script_dealing=NULL;
			}
			if (wcscmp(argv[1],_T("-step3-1"))==0)
			{
				CFile OpenFile;
				if (!OpenFile.Open(_T("akaiito_script_step2-1.txt"),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
				{
					OpenFile.Close();
					::AfxMessageBox(_T("��ʧ�ܣ�����akaiito_script_step2-b1.txt�Ƿ����Ŀ¼���Ѵ���"),MB_OK);
					return 0;
				}
				UINT length;
				length=OpenFile.GetLength();
				char *akaiito_script; // ���Ե����ַ���������
				akaiito_script=new char[length];
				OpenFile.Read((void*)akaiito_script,length);
				OpenFile.Close();
				char *akaiito_script_dealing;
				akaiito_script_dealing=new char[length];

				memset((void*)akaiito_script_dealing,0,length);
				length=ReplaceCR(akaiito_script,akaiito_script_dealing,length);
				memset((void*)akaiito_script,0,length);
				length=ReplaceSpaceChar(akaiito_script_dealing,akaiito_script,length);

				CFile SaveFile;
				if (!SaveFile.Open(_T("akaiito_script_step3-1.txt"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
				{
					::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
					if (!akaiito_script)
						delete []akaiito_script;
					akaiito_script=NULL;
					if (!akaiito_script_dealing)
						delete []akaiito_script_dealing;
					akaiito_script_dealing=NULL;
					return 0;
				}
				SaveFile.Write(akaiito_script,length);
				SaveFile.Close();

				if (!akaiito_script)
					delete []akaiito_script;
				akaiito_script=NULL;
				if (!akaiito_script_dealing)
					delete []akaiito_script_dealing;
				akaiito_script_dealing=NULL;
			}
			if (wcscmp(argv[1],_T("-step3-2"))==0)
			{
				CFile OpenFile;
				if (!OpenFile.Open(_T("akaiito_script_step2-2.txt"),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
				{
					OpenFile.Close();
					::AfxMessageBox(_T("��ʧ�ܣ�����akaiito_script_step2-b2.txt�Ƿ����Ŀ¼���Ѵ���"),MB_OK);
					return 0;
				}
				UINT length;
				length=OpenFile.GetLength();
				char *akaiito_script; // ���Ե����ַ���������
				akaiito_script=new char[length];
				OpenFile.Read((void*)akaiito_script,length);
				OpenFile.Close();
				char *akaiito_script_dealing;
				akaiito_script_dealing=new char[length];

				memset((void*)akaiito_script_dealing,0,length);
				length=DeleteCR(akaiito_script,akaiito_script_dealing,length);
				memset((void*)akaiito_script,0,length);
				length=ReplaceSpaceChar(akaiito_script_dealing,akaiito_script,length);

				CFile SaveFile;
				if (!SaveFile.Open(_T("akaiito_script_step3-2.txt"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
				{
					::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
					if (!akaiito_script)
						delete []akaiito_script;
					akaiito_script=NULL;
					if (!akaiito_script_dealing)
						delete []akaiito_script_dealing;
					akaiito_script_dealing=NULL;
					return 0;
				}
				SaveFile.Write(akaiito_script,length);
				SaveFile.Close();

				if (!akaiito_script)
					delete []akaiito_script;
				akaiito_script=NULL;
				if (!akaiito_script_dealing)
					delete []akaiito_script_dealing;
				akaiito_script_dealing=NULL;
			}
		}
	}

	return nRetCode;
}
