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
	BOOL AddRegKey(HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue,DWORD dwType=REG_SZ);
	BOOL DeleteRegKey();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSettingRegisterbutton();
	afx_msg void OnBnClickedSettingUnregisterbutton();
	afx_msg void OnBnClickedTxtutf8Button();
	afx_msg void OnBnClickedTxtoldstyleButton();
};
