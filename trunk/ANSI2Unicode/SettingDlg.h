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
	BOOL AddRegKey(HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue);
	BOOL DeleteRegKey();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSettingRegisterbutton();
	afx_msg void OnBnClickedSettingUnregisterbutton();
	afx_msg void OnBnClickedTxtutf8Button();
	afx_msg void OnBnClickedTxtoldstyleButton();
};
