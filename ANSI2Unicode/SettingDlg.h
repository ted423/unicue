#pragma once
#include "resource.h"
#include "Ansi2UnicodeDlg.h"

class CSettingDlg :
	public CDialog
{
public:
	enum { IDD = IDD_SETTING };
	CConfig m_Config;

public:
	CSettingDlg(void);
	~CSettingDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

protected:
	DECLARE_MESSAGE_MAP()
};
