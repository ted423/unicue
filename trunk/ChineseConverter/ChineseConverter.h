
// ChineseConverter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChineseConverterApp:
// �йش����ʵ�֣������ ChineseConverter.cpp
//

class CChineseConverterApp : public CWinApp
{
public:
	CChineseConverterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChineseConverterApp theApp;