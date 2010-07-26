#pragma once
#include "resource.h"

class CSettingDlg :
	public CDialog
{
public:
	enum { IDD = IDD_SETTING };

public:
	CSettingDlg(void);
	~CSettingDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

protected:
	DECLARE_MESSAGE_MAP()
};
