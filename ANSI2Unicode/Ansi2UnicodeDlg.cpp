/************************************************************************/
/*  Ansi to Unicode 1.0.2                                               */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// Ansi2UnicodeDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "..\misc\HyperLink.h"
#include "Ansi2Unicode.h"
#include "Ansi2UnicodeDlg.h"
#include "SettingDlg.h"
#include "toUnicode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink m_link;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_link.Attach(GetDlgItem(IDC_STATIC_KUYUR)->GetSafeHwnd());

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAnsi2UnicodeDlg 对话框
CAnsi2UnicodeDlg::CAnsi2UnicodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnsi2UnicodeDlg::IDD, pParent),m_bNeedConvert(TRUE),m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0),
	m_StringCodeType(CODETYPE_SHIFTJIS),/*m_bConfigLoaded(FALSE),*/m_bCommandLineOpen(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RawString=NULL;
	m_String=NULL;
	m_UnicodeString=NULL;
	m_FilePathName="";
	m_CodeStatus="";
	//m_ConfigPath="";

	// 从文件加载配置
	TCHAR szFull[_MAX_PATH];
	GetModuleFileName(NULL,szFull,_MAX_PATH);
	m_ConfigPath=CString(szFull);
	int pos;
	pos=m_ConfigPath.ReverseFind('\\');
	m_ConfigPath=m_ConfigPath.Left(pos);
	m_ConfigPath+=_T("\\Config.xml");

	TiXmlDocument *doc = new TiXmlDocument(CStringA(m_ConfigPath));
	bool loadOK = doc->LoadFile(TIXML_ENCODING_UTF8);
	if (loadOK)
	{
		if (!LoadConfigFile(doc))
		{
			::DeleteFile(m_ConfigPath);
			CreateConfigFile();
			m_Config.TemplateStr=_T(".utf-8");
			m_Config.AutoFixCue=TRUE;
			m_Config.AutoFixTTA=FALSE;
			m_Config.AcceptDragFLAC=TRUE;
			m_Config.AcceptDragAPE=TRUE;
			m_Config.AcceptDragTAK=TRUE;
			m_Config.AutoCheckCode=TRUE;
			m_Config.AlwaysOnTop=TRUE;
			m_Config.CloseCuePrompt=FALSE;
		}
	}
	else
	{
		CreateConfigFile();
		m_Config.TemplateStr=_T(".utf-8");
		m_Config.AutoFixCue=TRUE;
		m_Config.AutoFixTTA=FALSE;
		m_Config.AcceptDragFLAC=TRUE;
		m_Config.AcceptDragAPE=TRUE;
		m_Config.AcceptDragTAK=TRUE;
		m_Config.AutoCheckCode=TRUE;
		m_Config.AlwaysOnTop=TRUE;
		m_Config.CloseCuePrompt=FALSE;
	}
}

CAnsi2UnicodeDlg::~CAnsi2UnicodeDlg()
{
	if (m_RawString)
	{
		delete []m_RawString;
		m_RawString=NULL;
	}
	if (m_UnicodeString)
	{
		delete []m_UnicodeString;
		m_UnicodeString=NULL;
	}
}

void CAnsi2UnicodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTOCHECKCODE, m_Config.AutoCheckCode);
	DDX_Check(pDX, IDC_CHECK_ALWAYSONTOP,   m_Config.AlwaysOnTop);
	//DDX_CBIndex(pDX, IDC_COMBO_SELECTCODE, m_StringCodeType);
}

BEGIN_MESSAGE_MAP(CAnsi2UnicodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, &CAnsi2UnicodeDlg::OnFileExit)
	ON_COMMAND(ID_ABOUT, &CAnsi2UnicodeDlg::OnAbout)
	ON_COMMAND(ID_FILE_OPEN, &CAnsi2UnicodeDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAnsi2UnicodeDlg::OnFileSave)
	ON_BN_CLICKED(IDC_BUTTON_DO, &CAnsi2UnicodeDlg::OnBnClickedButtonDo)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTCODE, &CAnsi2UnicodeDlg::OnCbnSelchangeComboSelectcode)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAnsi2UnicodeDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CAnsi2UnicodeDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_CHECK_AUTOCHECKCODE, &CAnsi2UnicodeDlg::OnBnClickedCheckAutocheckcode)
	ON_COMMAND(ID_FILE_OPTION, &CAnsi2UnicodeDlg::OnFileOption)
	ON_BN_CLICKED(IDC_CHECK_ALWAYSONTOP, &CAnsi2UnicodeDlg::OnBnClickedCheckAlwaysontop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAnsi2UnicodeDlg 消息处理程序

BOOL CAnsi2UnicodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	m_menu.LoadMenu(IDR_MENU1);
	CDialog::SetMenu(&m_menu);

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 添加编码选项
	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
	theCombo->InsertString(-1,_T("Local Codepage"));
	theCombo->InsertString(-1,_T("GBK"));
	theCombo->InsertString(-1,_T("Big5"));
	theCombo->InsertString(-1,_T("Shift-JIS"));
	theCombo->InsertString(-1,_T("UTF-8"));
	theCombo->InsertString(-1,_T("Unicode(UTF-16)"));
	theCombo->SetCurSel(m_StringCodeType);

	//设置或取消最前端
	SetDialogPos();

	// 获取命令行
	if (AfxGetApp()->m_lpCmdLine[0]!=_T('\0'))
	{
		m_bCommandLineOpen=TRUE;
		m_FilePathName=AfxGetApp()->m_lpCmdLine;
		if (m_FilePathName.GetAt(0)==_T('\"'))
			m_FilePathName.Delete(0,1);
		if (m_FilePathName.GetAt(m_FilePathName.GetLength()-1)==_T('\"'))
			m_FilePathName=m_FilePathName.Left(m_FilePathName.GetLength()-1);
		if (TRUE==DealFile())
		{
			if (m_Config.AutoFixTTA) FixTTACue();
			if (m_Config.AutoFixCue) FixCue();
		}
	}

	/*
	LPWSTR *szArglist;
	int nArgs;
	int i;

	szArglist=CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if(NULL!=szArglist )
	{
		for( i=0; i<nArgs; i++) 
			AfxMessageBox(szArglist[i]);
	}

	LocalFree(szArglist);
	*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CAnsi2UnicodeDlg::SetDialogPos()
{
	CRect rc;
	GetWindowRect(&rc);

	if (m_Config.AlwaysOnTop)
		return SetWindowPos(&wndTopMost,rc.left,rc.top,rc.bottom,rc.right,SWP_NOMOVE|SWP_NOSIZE);
	else
		return SetWindowPos(&wndNoTopMost,rc.left,rc.top,rc.bottom,rc.right,SWP_NOMOVE|SWP_NOSIZE);
}

BOOL CAnsi2UnicodeDlg::LoadConfigFile(TiXmlDocument *xmlfile)
{
	TiXmlHandle hRoot(xmlfile);
	TiXmlElement *pElem;
	TiXmlHandle hXmlHandle(0);

	//config节点
	pElem=hRoot.FirstChildElement().Element();
	if (!pElem) return FALSE;
	if (strcmp(pElem->Value(),"config")!=0)
		return FALSE;

	//TemplateStr节点
	hXmlHandle=TiXmlHandle(pElem);
	pElem=hXmlHandle.FirstChild("TemplateStr").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	m_Config.TemplateStr=UTF8toUnicode(pElem->GetText());

	//AutoFixCue节点
	pElem=hXmlHandle.FirstChild("AutoFixCue").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AutoFixCue=TRUE;
	else
		m_Config.AutoFixCue=FALSE;

	//AutoFixTTA节点
	pElem=hXmlHandle.FirstChild("AutoFixTTA").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AutoFixTTA=TRUE;
	else
		m_Config.AutoFixTTA=FALSE;

	//AcceptDragFLAC节点
	pElem=hXmlHandle.FirstChild("AcceptDragFLAC").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AcceptDragFLAC=TRUE;
	else
		m_Config.AcceptDragFLAC=FALSE;

	//AcceptDragTAK节点
	pElem=hXmlHandle.FirstChild("AcceptDragTAK").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AcceptDragTAK=TRUE;
	else
		m_Config.AcceptDragTAK=FALSE;

	//AcceptDragAPE节点
	pElem=hXmlHandle.FirstChild("AcceptDragAPE").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AcceptDragAPE=TRUE;
	else
		m_Config.AcceptDragAPE=FALSE;

	//CloseCuePrompt节点
	pElem=hXmlHandle.FirstChild("CloseCuePrompt").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.CloseCuePrompt=TRUE;
	else
		m_Config.CloseCuePrompt=FALSE;

	//AutoCheckCode节点
	pElem=hXmlHandle.FirstChild("AutoCheckCode").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AutoCheckCode=TRUE;
	else
		m_Config.AutoCheckCode=FALSE;

	//AlwaysOnTop节点
	pElem=hXmlHandle.FirstChild("AlwaysOnTop").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AlwaysOnTop=TRUE;
	else
		m_Config.AlwaysOnTop=FALSE;

	return TRUE;
}

BOOL CAnsi2UnicodeDlg::CreateConfigFile()
{
	TiXmlDocument configdoc;
	//TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","gb2312","");
	TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
	TiXmlElement *configure=new TiXmlElement("config");

	TiXmlElement *TemplateStr=new TiXmlElement("TemplateStr");
	TiXmlText *TemplateStrValue=new TiXmlText(".utf-8");
	TemplateStr->LinkEndChild(TemplateStrValue);
	configure->LinkEndChild(TemplateStr);

	TiXmlElement *AutoFixCue=new TiXmlElement("AutoFixCue");
	TiXmlText *AutoFixCueValue=new TiXmlText("true");
	AutoFixCue->LinkEndChild(AutoFixCueValue);
	configure->LinkEndChild(AutoFixCue);

	TiXmlElement *AutoFixTTA=new TiXmlElement("AutoFixTTA");
	TiXmlText *AutoFixTTAValue=new TiXmlText("false");
	AutoFixTTA->LinkEndChild(AutoFixTTAValue);
	configure->LinkEndChild(AutoFixTTA);

	TiXmlElement *AcceptDragFLAC=new TiXmlElement("AcceptDragFLAC");
	TiXmlText *AcceptDragFLACValue=new TiXmlText("true");
	AcceptDragFLAC->LinkEndChild(AcceptDragFLACValue);
	configure->LinkEndChild(AcceptDragFLAC);

	TiXmlElement *AcceptDragTAK=new TiXmlElement("AcceptDragTAK");
	TiXmlText *AcceptDragTAKValue=new TiXmlText("true");
	AcceptDragTAK->LinkEndChild(AcceptDragTAKValue);
	configure->LinkEndChild(AcceptDragTAK);

	TiXmlElement *AcceptDragAPE=new TiXmlElement("AcceptDragAPE");
	TiXmlText *AcceptDragAPEValue=new TiXmlText("true");
	AcceptDragAPE->LinkEndChild(AcceptDragAPEValue);
	configure->LinkEndChild(AcceptDragAPE);

	TiXmlElement *CloseCuePrompt=new TiXmlElement("CloseCuePrompt");
	TiXmlText *CloseCuePromptValue=new TiXmlText("false");
	CloseCuePrompt->LinkEndChild(CloseCuePromptValue);
	configure->LinkEndChild(CloseCuePrompt);

	TiXmlElement *AutoCheckCode=new TiXmlElement("AutoCheckCode");
	TiXmlText *AutoCheckCodeValue=new TiXmlText("true");
	AutoCheckCode->LinkEndChild(AutoCheckCodeValue);
	configure->LinkEndChild(AutoCheckCode);

	TiXmlElement *AlwaysOnTop=new TiXmlElement("AlwaysOnTop");
	TiXmlText *AlwaysOnTopValue=new TiXmlText("true");
	AlwaysOnTop->LinkEndChild(AlwaysOnTopValue);
	configure->LinkEndChild(AlwaysOnTop);

	configdoc.LinkEndChild(dec);
	configdoc.LinkEndChild(configure);
	//configdoc.SaveFile(CStringA(m_ConfigPath));

	TiXmlPrinter printer;
	configdoc.Accept(&printer);

	//const CStringW UnicodeStr(printer.CStr());
	//const CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};

	CFile theFile;
	theFile.Open(m_ConfigPath,CFile::modeCreate|CFile::modeWrite);
	theFile.Write(UTF8BOM,3);
	theFile.Write(printer.CStr(),strlen(printer.CStr()));
	theFile.Close();

	return TRUE;
}

BOOL CAnsi2UnicodeDlg::SaveConfigFile()
{
	TiXmlDocument configdoc;
	TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
	TiXmlElement *configure=new TiXmlElement("config");

	TiXmlElement *TemplateStr=new TiXmlElement("TemplateStr");
	CStringA UTF8Str=CW2A(m_Config.TemplateStr,CP_UTF8);
	TiXmlText *TemplateStrValue=new TiXmlText(UTF8Str);
	TemplateStr->LinkEndChild(TemplateStrValue);
	configure->LinkEndChild(TemplateStr);

	TiXmlElement *AutoFixCue=new TiXmlElement("AutoFixCue");
	TiXmlText *AutoFixCueValue;
	if (m_Config.AutoFixCue)
		AutoFixCueValue=new TiXmlText("true");
	else
		AutoFixCueValue=new TiXmlText("false");
	AutoFixCue->LinkEndChild(AutoFixCueValue);
	configure->LinkEndChild(AutoFixCue);

	TiXmlElement *AutoFixTTA=new TiXmlElement("AutoFixTTA");
	TiXmlText *AutoFixTTAValue;
	if (m_Config.AutoFixTTA)
		AutoFixTTAValue=new TiXmlText("true");
	else
		AutoFixTTAValue=new TiXmlText("false");
	AutoFixTTA->LinkEndChild(AutoFixTTAValue);
	configure->LinkEndChild(AutoFixTTA);

	TiXmlElement *AcceptDragFLAC=new TiXmlElement("AcceptDragFLAC");
	TiXmlText *AcceptDragFLACValue;
	if (m_Config.AcceptDragFLAC)
		AcceptDragFLACValue=new TiXmlText("true");
	else
		AcceptDragFLACValue=new TiXmlText("false");
	AcceptDragFLAC->LinkEndChild(AcceptDragFLACValue);
	configure->LinkEndChild(AcceptDragFLAC);

	TiXmlElement *AcceptDragTAK=new TiXmlElement("AcceptDragTAK");
	TiXmlText *AcceptDragTAKValue;
	if (m_Config.AcceptDragTAK)
		AcceptDragTAKValue=new TiXmlText("true");
	else
		AcceptDragTAKValue=new TiXmlText("false");
	AcceptDragTAK->LinkEndChild(AcceptDragTAKValue);
	configure->LinkEndChild(AcceptDragTAK);

	TiXmlElement *AcceptDragAPE=new TiXmlElement("AcceptDragAPE");
	TiXmlText *AcceptDragAPEValue;
	if (m_Config.AcceptDragAPE)
		AcceptDragAPEValue=new TiXmlText("true");
	else
		AcceptDragAPEValue=new TiXmlText("false");
	AcceptDragAPE->LinkEndChild(AcceptDragAPEValue);
	configure->LinkEndChild(AcceptDragAPE);

	TiXmlElement *CloseCuePrompt=new TiXmlElement("CloseCuePrompt");
	TiXmlText *CloseCuePromptValue;
	if (m_Config.CloseCuePrompt)
		CloseCuePromptValue=new TiXmlText("true");
	else
		CloseCuePromptValue=new TiXmlText("false");
	CloseCuePrompt->LinkEndChild(CloseCuePromptValue);
	configure->LinkEndChild(CloseCuePrompt);

	TiXmlElement *AutoCheckCode=new TiXmlElement("AutoCheckCode");
	TiXmlText *AutoCheckCodeValue;
	if (m_Config.AutoCheckCode)
		AutoCheckCodeValue=new TiXmlText("true");
	else
		AutoCheckCodeValue=new TiXmlText("false");
	AutoCheckCode->LinkEndChild(AutoCheckCodeValue);
	configure->LinkEndChild(AutoCheckCode);

	TiXmlElement *AlwaysOnTop=new TiXmlElement("AlwaysOnTop");
	TiXmlText *AlwaysOnTopValue;
	if (m_Config.AlwaysOnTop)
		AlwaysOnTopValue=new TiXmlText("true");
	else
		AlwaysOnTopValue=new TiXmlText("false");
	AlwaysOnTop->LinkEndChild(AlwaysOnTopValue);
	configure->LinkEndChild(AlwaysOnTop);

	configdoc.LinkEndChild(dec);
	configdoc.LinkEndChild(configure);

	TiXmlPrinter printer;
	configdoc.Accept(&printer);

	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};

	CFile theFile;
	theFile.Open(m_ConfigPath,CFile::modeCreate|CFile::modeWrite);
	theFile.Write(UTF8BOM,3);
	theFile.Write(printer.CStr(),strlen(printer.CStr()));
	theFile.Close();

	return TRUE;
}

void CAnsi2UnicodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAnsi2UnicodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CAnsi2UnicodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAnsi2UnicodeDlg::OnFileExit()
{
	SendMessage(WM_CLOSE);
}

void CAnsi2UnicodeDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

BOOL CAnsi2UnicodeDlg::DealFile()
{
	CFile OpenFile;
	if (!OpenFile.Open(m_FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
	{
		OpenFile.Close();
		::AfxMessageBox(_T("打开失败！"),MB_OK);
		return FALSE;
	}
	m_bNeedConvert=TRUE;
	m_RawStringLength=OpenFile.GetLength();
	if (m_RawString)
	{
		delete []m_RawString;
		m_RawString=NULL;
		m_String=NULL;
	}
	if (m_UnicodeString)
	{
		delete []m_UnicodeString;
		m_UnicodeString=NULL;
	}
	m_RawString=new char[m_RawStringLength+1];
	OpenFile.Read(m_RawString,m_RawStringLength);
	OpenFile.Close();
	m_RawString[m_RawStringLength]='\0';
	m_String=m_RawString;
	m_StringLength=m_RawStringLength;

	CComboBox *theCombo  =(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
	CStatic   *theStatic =(CStatic*)GetDlgItem(IDC_STATIC_STAT);
	m_CodeStatus=_T("未知编码");

	CEdit *LeftEdit =(CEdit *)GetDlgItem(IDC_EDIT_ANSI);
	CEdit *RightEdit=(CEdit *)GetDlgItem(IDC_EDIT_UNICODE);

	// Unicode(little-endian)
	if (((unsigned char)m_RawString[0]==0xFF)&&((unsigned char)m_RawString[1]==0xFE))
	{
		m_CodeStatus=_T("Unicode (little endian)");
		m_bNeedConvert=FALSE;
		m_StringCodeType=CODETYPE_UNICODE;
		theCombo->SetCurSel(m_StringCodeType);
		m_String=m_RawString+2; //真正的起始地址
		m_StringLength=m_RawStringLength-2; //真正的长度
		if ((m_RawStringLength%2)!=0)
		{
			MessageBox(_T("文本错误！"));
			return FALSE;
		}
		m_UnicodeLength=m_StringLength>>1;
		m_UnicodeString=new wchar_t[m_UnicodeLength+1];
		memcpy((void*)m_UnicodeString,m_String,m_StringLength);
		m_UnicodeString[m_UnicodeLength]='\0';
	}
	// Unicode(big-endian)
	if (((unsigned char)m_RawString[0]==0xFE)&&((unsigned char)m_RawString[1]==0xFF))
	{
		m_CodeStatus=_T("Unicode (big endian)");
		m_bNeedConvert=FALSE;
		m_StringCodeType=CODETYPE_UNICODE;
		theCombo->SetCurSel(m_StringCodeType);
		m_String=m_RawString+2; //真正的起始地址
		m_StringLength=m_RawStringLength-2; //真正的长度
		if ((m_RawStringLength%2)!=0)
		{
			MessageBox(_T("文本错误！"));
			return FALSE;
		}
		m_UnicodeLength=m_StringLength>>1;
		m_UnicodeString=new wchar_t[m_UnicodeLength+1];
		memcpy((void*)m_UnicodeString,m_String,m_StringLength);
		m_UnicodeString[m_UnicodeLength]='\0';
		//调整高低位顺序
		for (UINT i=0;i<m_UnicodeLength;i++)
		{
			unsigned char chars[2];
			memcpy(chars,(void*)(m_UnicodeString+i),2);
			wchar_t theChr=chars[0]*256+chars[1];
			m_UnicodeString[i]=theChr;
		}
	}
	// UTF-8(with BOM)
	if (((unsigned char)m_RawString[0]==0xEF)&&((unsigned char)m_RawString[1]==0xBB)&&((unsigned char)m_RawString[2]==0xBF))
	{
		m_CodeStatus=_T("UTF-8 (with BOM)");
		m_bNeedConvert=FALSE;
		m_StringCodeType=CODETYPE_UTF8;
		theCombo->SetCurSel(m_StringCodeType);
		m_String=m_RawString+3; //真正的起始地址
		m_StringLength=m_RawStringLength-3; //真正的长度
	}

	if (m_bNeedConvert==FALSE)
	{
		theStatic->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);
		if (m_StringCodeType==CODETYPE_UNICODE)
		{
			CString RightEditText(m_UnicodeString);
			RightEdit->SetWindowText(RightEditText);
			LeftEdit->SetWindowText(_T(""));
		}
		if (m_StringCodeType==CODETYPE_UTF8)
		{
			RightEdit->SetWindowText(UTF8toUnicode(m_String,m_StringLength));
			LeftEdit->SetWindowText(_T(""));
		}
	}
	else
	{
		//检测编码
		if (m_Config.AutoCheckCode==TRUE)
		{
			m_StringCodeType=CheckCodeType(m_String,m_StringLength,m_StringCodeType);
			theCombo->SetCurSel(m_StringCodeType);
			switch (m_StringCodeType)
			{
			case CODETYPE_DEFAULT:
				m_CodeStatus=_T("未知编码");
				break;
			case CODETYPE_SHIFTJIS:
				m_CodeStatus=_T("Shift-JIS");
				break;
			case CODETYPE_GBK:
				m_CodeStatus=_T("GBK");
				break;
			case CODETYPE_BIG5:
				m_CodeStatus=_T("Big5");
				break;
			case CODETYPE_UTF8:
				m_CodeStatus=_T("UTF-8 (without BOM)");
				break;
			default:
				m_CodeStatus=_T("未知编码");
			}
		}
		else
			m_CodeStatus=_T("已经关闭编码自动检测");

		theStatic->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);

		//左
		CString LeftEditText(m_String); //注意：此时LeftEditText的数据类型已经是Unicode字符串，每个字符占用两个字节
		LeftEdit->SetWindowText(LeftEditText);

		//右
		RightEdit->SetWindowText(AnsiToUnicode(m_String,m_StringLength,m_StringCodeType));
	}
	
	return TRUE;
}

void CAnsi2UnicodeDlg::OnFileOpen()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("文本文件(*.txt;*.cue;*.log)|*.txt;*.cue;*.log|txt文本文件(*.txt)|*.txt|cue文件(*.cue)|*.cue|log文件(*.log)|*.log|All Files (*.*)|*.*||"));
	if (openFile.DoModal() == IDOK)
	{
		m_FilePathName=openFile.GetPathName();
		if (TRUE==DealFile())
		{
			if (m_Config.AutoFixTTA) FixTTACue();
			if (m_Config.AutoFixCue) FixCue();
		}
	}
}

void CAnsi2UnicodeDlg::OnFileSave()
{
	CFileDialog saveFile(FALSE,_T("*.txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,_T("txt文本文件(*.txt)|*.txt|cue文件(*.cue)|*.cue|log文件(*.log)|*.log|All Files (*.*)|*.*||"));
	if (saveFile.DoModal() == IDOK)
	{
		CFile SaveFile;
		if (!SaveFile.Open(saveFile.GetFileName(),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
		{
			::AfxMessageBox(_T("无法写入文件！"),MB_OK);
			return;
		}
		CString UnicodeStr;
		GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(UnicodeStr);
		CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
		char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
		SaveFile.Write(&UTF8BOM,3);
		SaveFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
		SaveFile.Close();
	}
}

void CAnsi2UnicodeDlg::OnBnClickedButtonDo()
{
	/*
	if (m_bNeedConvert)
		GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(AnsiToUnicode(m_String,m_StringLength,m_StringCodeType));
	else
		MessageBox(_T("Unicode或UTF-8文本，无需转换!"));
	*/
}

void CAnsi2UnicodeDlg::OnDropFiles(HDROP hDropInfo)
{
	int nFileCount;  
	nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if(nFileCount == 1)
	{
		TCHAR szFileName[MAX_PATH+1];
		::DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		m_FilePathName=CString(szFileName);
		if (TRUE==DealFile())
		{
			if (m_Config.AutoFixTTA) FixTTACue();
			if (m_Config.AutoFixCue) FixCue();
		}
	}
	else
	{
		::AfxMessageBox(_T(" 只能同时打开一个文件"),MB_OK);
	}
	::DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CAnsi2UnicodeDlg::OnCbnSelchangeComboSelectcode()
{
	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
	if (m_bNeedConvert)
	{
		m_StringCodeType=theCombo->GetCurSel();
		GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(AnsiToUnicode(m_String,m_StringLength,m_StringCodeType));
		if (m_Config.AutoFixTTA) FixTTACue();
		if (m_Config.AutoFixCue) FixCue();
	}
}

void CAnsi2UnicodeDlg::OnBnClickedButtonSave()
{
	CFile SaveFile;
	if (!SaveFile.Open(m_FilePathName,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
	{
		::AfxMessageBox(_T("无法写入文件！"),MB_OK);
		return;
	}
	CString UnicodeStr;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(UnicodeStr);
	CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
	SaveFile.Write(&UTF8BOM,3);
	SaveFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
	SaveFile.Close();
}

void CAnsi2UnicodeDlg::OnBnClickedButtonSaveas()
{
	CString FilePath,FileType;
	int position=m_FilePathName.ReverseFind('.');
	FilePath=m_FilePathName.Left(position);
	FileType=m_FilePathName.Right(m_FilePathName.GetLength()-position);
	FilePath+=m_Config.TemplateStr;
	FilePath+=FileType;

	CFile SaveFile;
	if (!SaveFile.Open(FilePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
	{
		::AfxMessageBox(_T("无法写入文件！"),MB_OK);
		return;
	}
	CString UnicodeStr;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(UnicodeStr);
	CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
	SaveFile.Write(&UTF8BOM,3);
	SaveFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
	SaveFile.Close();
}

void CAnsi2UnicodeDlg::OnBnClickedCheckAutocheckcode()
{
	m_Config.AutoCheckCode=!m_Config.AutoCheckCode;
}

void CAnsi2UnicodeDlg::OnBnClickedCheckAlwaysontop()
{
	m_Config.AlwaysOnTop=!m_Config.AlwaysOnTop;
	//设置或取消最前端
	SetDialogPos();
}

void CAnsi2UnicodeDlg::OnFileOption()
{
	CSettingDlg SettingDlg;
	if (SettingDlg.DoModal()==IDOK)
	{
		m_Config=SettingDlg.m_Config;
		//SaveConfigFile();
	}
}

CConfig CAnsi2UnicodeDlg::GetConfig()
{
	return m_Config;
}

void CAnsi2UnicodeDlg::FixCue()
{
	FixTTACue();

	int pos;
	BOOL isCue=FALSE;
	pos=m_FilePathName.ReverseFind('.');
	if (m_FilePathName.Right(m_FilePathName.GetLength()-pos-1).MakeLower()==_T("cue"))
		isCue=TRUE;

	CString CueString;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(CueString);
	int BeginPos=CueString.Find(_T("FILE \""));
	if (BeginPos==-1)
	{
		if (isCue&&(!m_Config.CloseCuePrompt)) MessageBox(_T("cue文件异常"));
		return;
	}
	int EndPos=CueString.Find(_T("\" WAVE"));
	if (EndPos==-1)
	{
		if (isCue&&(!m_Config.CloseCuePrompt)) MessageBox(_T("cue文件异常"));
		return;
	}
	BeginPos+=6;
	if (BeginPos>=EndPos)
	{
		if (isCue&&(!m_Config.CloseCuePrompt)) MessageBox(_T("cue文件异常"));
		return;
	}

	CString MusicFileName,MusicFilePath; //音频文件名，路径
	MusicFileName=CueString.Mid(BeginPos,EndPos-BeginPos);

	//依据文档路径：m_FilePathName查找音频文件
	pos=m_FilePathName.ReverseFind('\\');
	MusicFilePath=m_FilePathName.Left(pos);
	MusicFilePath+=_T("\\");
	MusicFilePath+=MusicFileName;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind=FindFirstFile(MusicFilePath, &FindFileData);

	if (hFind==INVALID_HANDLE_VALUE) //没找到cue中音频文件
	{
		pos=MusicFilePath.ReverseFind('.');
		MusicFilePath=MusicFilePath.Left(pos);
		//替换扩展名查找
		CString FindFilePath;

		FindFilePath=MusicFilePath+_T(".ape"); //ape
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".mac"); //ape
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".flac"); //flac
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".fla"); //flac
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".tta"); //tta
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".tak"); //tak
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".wv"); //wv
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".m4a"); //apple lossless
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".wma"); //wma
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".wav"); //wav
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".wave"); //wav
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		FindFilePath=MusicFilePath+_T(".mp3"); //mp3
		hFind=FindFirstFile(FindFilePath, &FindFileData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			CueString.Replace(MusicFileName,FindFileData.cFileName);
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
			FindClose(hFind);
			return;
		}

		//最后还是没找到
		FindClose(hFind);
		return;
	}
	else
	{
		FindClose(hFind);
		return;
	}
}

void CAnsi2UnicodeDlg::FixTTACue()
{
	CString OldCueString;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(OldCueString);
	OldCueString.MakeLower(); //转换为小写
	int Pos=OldCueString.Find(_T("the true audio"));
	if (Pos<=0)
		return;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(OldCueString);
	CString NewCueString;
	NewCueString=OldCueString.Left(Pos)+_T("WAVE")+OldCueString.Right(OldCueString.GetLength()-Pos-14);
	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(NewCueString);
}

void CAnsi2UnicodeDlg::OnDestroy()
{
	CDialog::OnDestroy();
	SaveConfigFile();
}
