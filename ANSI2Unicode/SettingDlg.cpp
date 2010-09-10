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
	DDX_Check(pDX, IDC_SETTING_INCUECHECK, m_Config.AcceptDragAudioFile);
	DDX_Check(pDX, IDC_SETTING_CLOSEPROMPTCHECK, m_Config.CloseCuePrompt);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_SETTING_REGISTERBUTTON, &CSettingDlg::OnBnClickedSettingRegisterbutton)
	ON_BN_CLICKED(IDC_SETTING_UNREGISTERBUTTON, &CSettingDlg::OnBnClickedSettingUnregisterbutton)
	ON_BN_CLICKED(IDC_TXTUTF8_BUTTON, &CSettingDlg::OnBnClickedTxtutf8Button)
	ON_BN_CLICKED(IDC_TXTOLDSTYLE_BUTTON, &CSettingDlg::OnBnClickedTxtoldstyleButton)
END_MESSAGE_MAP()

//дע���ֵĬ��������REG_SZ
BOOL CSettingDlg::AddRegKey(HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue,DWORD dwType)
{
	HKEY hAddKey;
	DWORD dwDisp; //����½�����ʱ�ķ�������
	if (RegOpenKeyEx(hKey,lpSubItem,0L,KEY_ALL_ACCESS,&hAddKey))
	{
		//����������½�֮
		if (RegCreateKeyEx(hKey,lpSubItem,0L,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hAddKey,&dwDisp))
		{
			return FALSE;
		}
		RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); //unicode
	}
	else
	{
		RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); //unicode
	}

	RegCloseKey(hAddKey);
	return TRUE;
}

BOOL CSettingDlg::DeleteRegKey()
{
	return TRUE;
}

//ע�ᵽ�����ļ��Ҽ��˵�
void CSettingDlg::OnBnClickedSettingRegisterbutton()
{
	TCHAR AppPathName[MAX_PATH]; //�260
	GetModuleFileName(NULL, AppPathName, MAX_PATH);

	/*
	[HKEY_CLASSES_ROOT\.uni]
	@="UniCue.UNI"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".uni"),_T(""),_T("UniCue.UNI")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\.uni]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI]
	@="UniCue �ļ�"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"),_T(""),_T("UniCue �ļ�")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\shell]
	@="Open"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"),_T(""),_T("Open")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open]
	@="ʹ�� UniCue ��"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"),_T(""),_T("ʹ�� UniCue ��")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\Open]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open\command]
	@="\"AppPathName\" \"%1\""
	*/
	CString PathValue=_T("\"");
	PathValue+=AppPathName;
	PathValue+=_T("\" \"%1\"");
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\Open\\command]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue]
	@="ʹ�� UniCue ת������"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"),_T(""),_T("ʹ�� UniCue ת������")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\unicue]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue\command]
	@="\"AppPathName\" \"%1\""
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\unicue\\command]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\txtfile\shell\unicue]
	@="ʹ�� UniCue ת������"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"),_T(""),_T("ʹ�� UniCue ת������")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\txtfile\\shell\\unicue]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\txtfile\shell\unicue\command]
	@="\"AppPathName\" \"%1\""
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\txtfile\\shell\\unicue\\command]\"ʧ��"));

	//����cue�ļ���ע�����Ϣ���ٶ�cue�Ѿ�������foobar2000.CUE
	/*
	[HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue]
	@="ʹ�� UniCue ת������"

	[HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue\command]
	@="\"AppPathName\" \"%1\""
	*/
	HKEY hCue;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue))
	{
		//������.cue
		/*
		[HKEY_CLASSES_ROOT\.cue]
		@="UniCue.UNI"
		*/
		if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".cue"),_T(""),_T("UniCue.UNI")))
			AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\.cue]\"ʧ��"));
	}
	else
	{
		//����.cue
		DWORD BufferSize=520,ValueType;
		BYTE *Data = new BYTE[520]; //����һ�㲻�ᳬ��260���ַ�
		memset(Data,0,520);
		if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,Data,&BufferSize)==ERROR_SUCCESS)
		{
			TRACE(_T("%d"),BufferSize); //BufferSize=��ֵ�ִ�����*2 +2��+1
			TRACE(_T("%s"),Data);
			if (BufferSize%2==0)
				BufferSize-=2;
			else
				BufferSize-=1;
			if (ValueType!=REG_SZ)
			{
				//��ȡ��ֵ����
				AddRegKey(HKEY_CLASSES_ROOT,_T(".cue"),_T(""),_T("UniCue.UNI"));
			}
			else
			{
				BufferSize=BufferSize>>1;
				WCHAR *CueFileType=new WCHAR[BufferSize+1];
				memcpy((void*)CueFileType,(void*)Data,BufferSize*2);
				CueFileType[BufferSize]='\0';

				//�ٶ�CueFileType����Foobar2000.CUE
				CString CueKeyPath(CueFileType);
				//AfxMessageBox(CueKeyPath);
				CueKeyPath+=_T("\\Shell\\unicue");
				if (!AddRegKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,_T(""),_T("ʹ�� UniCue ת������")))
					AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\foobar2000.CUE\\shell\\unicue\\command]\"ʧ��"));
				CueKeyPath+=_T("\\command");
				if (!AddRegKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,_T(""),(LPCTSTR)PathValue))
					AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\foobar2000.CUE\\shell\\unicue\\command]\"ʧ��"));

				delete []CueFileType;
			}
		}
		delete []Data;
		RegCloseKey(hCue);
	}

	/*
	[HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue]
	@="ʹ�� UniCue ת������"
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"),_T(""),_T("ʹ�� UniCue ת������")))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\Applications\\notepad.exe\\shell\\unicue]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue\command]
	@="\"AppPathName\" \"%1\""
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\Applications\\notepad.exe\\shell\\unicue\\command]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\Applications\ANSI2Unicode.exe\shell\open\command]
	@="\"AppPathName\" \"%1\""
	*/
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\Applications\\ANSI2Unicode.exe\\shell\\open\\command]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\UniCue.UNI\DefaultIcon]
	@="AppFolder\\icons\\file.ico"
	*/
	PathValue=CString(AppPathName);
	int pos=PathValue.ReverseFind('\\');
	PathValue=PathValue.Left(pos);
	PathValue+=_T("\\icons\\uni.ico");
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"),_T(""),(LPCTSTR)PathValue))
		AfxMessageBox(_T("��������\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\DefaultIcon]\"ʧ��"));

	/*
	[HKEY_CLASSES_ROOT\.uni\ShellNew]
	"FileName"="AppFolder\\null.uni"
	*/
	if (m_Config.RegNewUniFile)
	{
		PathValue=CString(AppPathName);
		pos=PathValue.ReverseFind('\\');
		PathValue=PathValue.Left(pos);
		PathValue+=_T("\\null.uni");
		if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"),_T("FileName"),(LPCTSTR)PathValue))
			AfxMessageBox(_T("������ֵ\"[HKEY_CLASSES_ROOT\\.uni\\ShellNew\\FileName]\"ʧ��"));
	}

	//ˢ��shell��ͼ�껺��
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

	//AfxMessageBox(_T("ע�����!"));
}

//ж���Ҽ��˵�����
void CSettingDlg::OnBnClickedSettingUnregisterbutton()
{
	RegDeleteKey(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"));  //32bit����
	RegDeleteKey(HKEY_CLASSES_ROOT,_T(".uni"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe"));
	//ж��cue�ļ����Ҽ��˵�
	HKEY hCue;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue)==ERROR_SUCCESS)  //����.cue
	{
		DWORD BufferSize=520,ValueType;
		BYTE *Data = new BYTE[520]; //����һ�㲻�ᳬ��260���ַ�
		memset(Data,0,520);
		if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,Data,&BufferSize)==ERROR_SUCCESS)
		{
			if (BufferSize%2==0)
				BufferSize-=2;
			else
				BufferSize-=1;
			if (ValueType==REG_SZ)
			{
				BufferSize=BufferSize>>1;
				WCHAR *CueFileType=new WCHAR[BufferSize+1];
				memcpy((void*)CueFileType,(void*)Data,BufferSize*2);
				CueFileType[BufferSize]='\0';
				CString CueKeyPath(CueFileType);
				CueKeyPath+=_T("\\Shell\\unicue\\command");
				RegDeleteKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath);
				CueKeyPath=CueKeyPath.Left(CueKeyPath.GetLength()-8);
				RegDeleteKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath);

				delete []CueFileType;
			}
		}
		delete []Data;
		RegCloseKey(hCue);
	}

	/*
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"),KEY_WOW64_32KEY,0L);  //32bit����
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T(".uni"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell"),KEY_WOW64_32KEY,0L);
	RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe"),KEY_WOW64_32KEY,0L);
	//ж��cue�ļ����Ҽ��˵�
	HKEY hCue;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue)==ERROR_SUCCESS)  //����.cue
	{
		DWORD BufferSize=520,ValueType;
		BYTE *Data = new BYTE[520]; //����һ�㲻�ᳬ��260���ַ�
		memset(Data,0,520);
		if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,Data,&BufferSize)==ERROR_SUCCESS)
		{
			if (BufferSize%2==0)
				BufferSize-=2;
			else
				BufferSize-=1;
			if (ValueType==REG_SZ)
			{
				BufferSize=BufferSize>>1;
				WCHAR *CueFileType=new WCHAR[BufferSize+1];
				memcpy((void*)CueFileType,(void*)Data,BufferSize*2);
				CueFileType[BufferSize]='\0';
				CString CueKeyPath(CueFileType);
				CueKeyPath+=_T("\\Shell\\unicue\\command");
				RegDeleteKeyEx(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,KEY_WOW64_32KEY,0L);
				CueKeyPath=CueKeyPath.Left(CueKeyPath.GetLength()-8);
				RegDeleteKeyEx(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,KEY_WOW64_32KEY,0L);

				delete []CueFileType;
			}
		}
		delete []Data;
		RegCloseKey(hCue);
	}
	*/

	//ˢ��shell��ͼ�껺��
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
}

void CSettingDlg::OnBnClickedTxtutf8Button()
{
	// ɾ��[HKEY_CLASSES_ROOT\.txt\ShellNew] �µļ�ֵ
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
		RegDeleteValue(hKey,_T("NullFile"));
	/*
	[HKEY_CLASSES_ROOT\.txt\ShellNew]
	"FileName"="AppFolder\\null.uni"
	*/
	TCHAR AppPathName[MAX_PATH]; //�260
	GetModuleFileName(NULL, AppPathName, MAX_PATH);

	CString PathValue(AppPathName);
	int pos=PathValue.ReverseFind('\\');
	PathValue=PathValue.Left(pos);
	PathValue+=_T("\\null.uni");
	if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("FileName"),(LPCTSTR)PathValue))
		AfxMessageBox(_T("������ֵ\"[HKEY_CLASSES_ROOT\\.txt\\ShellNew\\FileName]\"ʧ��"));
}

void CSettingDlg::OnBnClickedTxtoldstyleButton()
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
		RegDeleteValue(hKey,_T("FileName"));

	//AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("ItemName"),_T("@%SystemRoot%\\system32\\notepad.exe,-470"),REG_EXPAND_SZ);
	AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("NullFile"),_T(""));
}
