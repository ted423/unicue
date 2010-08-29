#pragma once

#include "StdAfx.h"
#include "SettingDlg.h"

CSettingDlg::CSettingDlg(void) : CDialog(CSettingDlg::IDD)
{
	CAnsi2UnicodeDlg *pMainWnd = (CAnsi2UnicodeDlg*)::AfxGetApp()->GetMainWnd(); 
	m_Config=pMainWnd->GetConfig();
}

CSettingDlg::~CSettingDlg(void)
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETTING_EDIT, m_Config.TemplateStr);
	DDX_Check(pDX, IDC_SETTING_AUTOFIXCHECK,m_Config.AutoFixCue);
	DDX_Check(pDX, IDC_SETTING_REPLACECHECK, m_Config.AutoFixTTA);
	DDX_Check(pDX, IDC_SETTING_FLACCHECK, m_Config.AcceptDragFLAC);
	DDX_Check(pDX, IDC_SETTING_TAKCHECK, m_Config.AcceptDragTAK);
	DDX_Check(pDX, IDC_SETTING_APECHECK, m_Config.AcceptDragAPE);
	DDX_Check(pDX, IDC_SETTING_CLOSEPROMPTCHECK, m_Config.CloseCuePrompt);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
END_MESSAGE_MAP()