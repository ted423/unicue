// Big5toUnicode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBig5toUnicodeApp:
// �йش����ʵ�֣������ Big5toUnicode.cpp
//

class CBig5toUnicodeApp : public CWinApp
{
public:
	CBig5toUnicodeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBig5toUnicodeApp theApp;