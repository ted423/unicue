// Ansi2Unicode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAnsi2UnicodeApp:
// �йش����ʵ�֣������ Ansi2Unicode.cpp
//

class CAnsi2UnicodeApp : public CWinApp
{
public:
	CAnsi2UnicodeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAnsi2UnicodeApp theApp;