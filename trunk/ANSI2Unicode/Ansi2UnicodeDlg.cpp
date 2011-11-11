/************************************************************************/
/*  Ansi to Unicode 1.1                                                 */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// Ansi2UnicodeDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "HyperLink.h"
#include "Ansi2Unicode.h"
#include "Ansi2UnicodeDlg.h"
#include "SettingDlg.h"

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

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CImage m_png;
	CHyperLink m_link;

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
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

	//用CImage加载：流或文件
	HGLOBAL        hGlobal = NULL;
	HRSRC          hSource = NULL;
	int            nSize   = 0;

	hSource = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_PNG1), _T("PNG"));
	if (hSource == NULL)
		return TRUE;
	hGlobal = LoadResource(AfxGetResourceHandle(), hSource);
	if (hGlobal == NULL)
	{
		FreeResource(hGlobal);
		return TRUE;
	}
	IStream* pStream = NULL;

	nSize = (UINT)SizeofResource(AfxGetResourceHandle(), hSource);
	HGLOBAL hGlobal2 = GlobalAlloc(GMEM_MOVEABLE, nSize);
	if(hGlobal2 == NULL)
	{
		FreeResource(hGlobal);
		return TRUE;
	}

	void* pData = GlobalLock(hGlobal2);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlobal2);
	if(CreateStreamOnHGlobal(hGlobal2, TRUE, &pStream) == S_OK)
	{
		m_png.Load(pStream);
		pStream->Release();
	}
	FreeResource(hGlobal2); 
	FreeResource(hGlobal);

	m_link.Attach(GetDlgItem(IDC_STATIC_KUYUR)->GetSafeHwnd());

	return TRUE;
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_png.IsNull())
		return;

	for(int i = 0; i < m_png.GetWidth(); ++i)
	{
		for(int j = 0; j < m_png.GetHeight(); ++j)
		{
			unsigned char* pucColor = (unsigned char *)m_png.GetPixelAddress(i , j);
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
	m_png.AlphaBlend(dc.GetSafeHdc(),8,8); //透明显示
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAnsi2UnicodeDlg 对话框
CAnsi2UnicodeDlg::CAnsi2UnicodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnsi2UnicodeDlg::IDD, pParent),m_bNeedConvert(TRUE),m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0),
	m_StringCodeType("Local Codepage"),/*m_bConfigLoaded(FALSE),m_bCommandLineOpen(FALSE),*/m_bCueFile(FALSE),m_bTransferString(FALSE)
{
	m_hLittleIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_LITTLE);
	//m_hBigIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_BIG);
	m_RawString=NULL;
	m_String=NULL;
	m_UnicodeString=NULL;
	m_FilePathName="";
	m_CodeStatus="";
	//m_ConfigPath="";
	m_Config.RegNewUniFile=FALSE;

	// 从文件加载配置
	TCHAR szFull[_MAX_PATH];
	GetModuleFileName(NULL,szFull,_MAX_PATH);
	CString processPath(szFull);
	int pos;
	pos=processPath.ReverseFind('\\');
	processPath=processPath.Left(pos+1);
	m_ConfigPath=processPath+_T("Config.xml");

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
			m_Config.AcceptDragAudioFile=TRUE;
			m_Config.AutoCheckCode=TRUE;
			m_Config.AlwaysOnTop=TRUE;
			m_Config.CloseCuePrompt=FALSE;
			m_Config.MapConfName = _T("charmap-anisong.xml");
			//m_Config.RegNewUniFile=FALSE;
		}
	}
	else
	{
		CreateConfigFile();
		m_Config.TemplateStr=_T(".utf-8");
		m_Config.AutoFixCue=TRUE;
		m_Config.AutoFixTTA=FALSE;
		m_Config.AcceptDragAudioFile=TRUE;
		m_Config.AutoCheckCode=TRUE;
		m_Config.AlwaysOnTop=TRUE;
		m_Config.CloseCuePrompt=FALSE;
		m_Config.MapConfName = _T("charmap-anisong.xml");
		//m_Config.RegNewUniFile=FALSE;
	}
	delete doc;
	m_context = new CC4Context((LPCTSTR)m_Config.MapConfName, (LPCTSTR)processPath);
	if (!m_context->init())
		AfxMessageBox(_T("Failed to load charmaps!"));
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
	if (m_context)
	{
		m_context->finalize();
		delete m_context;
		m_context = NULL;
	}
}

void CAnsi2UnicodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTOCHECKCODE,  m_Config.AutoCheckCode);
	DDX_Check(pDX, IDC_CHECK_ALWAYSONTOP,    m_Config.AlwaysOnTop);
}

BEGIN_MESSAGE_MAP(CAnsi2UnicodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, &CAnsi2UnicodeDlg::OnFileExit)
	ON_COMMAND(ID_ABOUT, &CAnsi2UnicodeDlg::OnAbout)
	ON_COMMAND(ID_FILE_OPEN, &CAnsi2UnicodeDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAnsi2UnicodeDlg::OnFileSave)
	ON_COMMAND(ID_FILE_OPTION, &CAnsi2UnicodeDlg::OnFileOption)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTCODE, &CAnsi2UnicodeDlg::OnCbnSelchangeComboSelectcode)
	ON_BN_CLICKED(IDC_BUTTON_DO, &CAnsi2UnicodeDlg::OnBnClickedButtonDo)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAnsi2UnicodeDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CAnsi2UnicodeDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_CHECK_AUTOCHECKCODE, &CAnsi2UnicodeDlg::OnBnClickedCheckAutocheckcode)
	ON_BN_CLICKED(IDC_CHECK_ALWAYSONTOP, &CAnsi2UnicodeDlg::OnBnClickedCheckAlwaysontop)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFERSTRING, &CAnsi2UnicodeDlg::OnBnClickedButtonTransferstring)
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
	SetIcon(m_hLittleIcon, TRUE);		// 设置大图标
	SetIcon(m_hLittleIcon, FALSE);		// 设置小图标

	// 添加编码选项
	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
	std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
	std::list<std::wstring>::iterator iter;
	theCombo->InsertString(-1,_T("Local Codepage"));
	for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
		theCombo->InsertString(-1, iter->c_str());
	theCombo->SetCurSel(0);

	//设置或取消最前端
	SetDialogPos();

	// 获取命令行
	if (AfxGetApp()->m_lpCmdLine[0]!=_T('\0'))
	{
		//m_bCommandLineOpen=TRUE;
		m_FilePathName=AfxGetApp()->m_lpCmdLine;
		if (m_FilePathName.GetAt(0)==_T('\"'))
			m_FilePathName.Delete(0,1);
		if (m_FilePathName.GetAt(m_FilePathName.GetLength()-1)==_T('\"'))
			m_FilePathName=m_FilePathName.Left(m_FilePathName.GetLength()-1);
		CString ExtensionName,FileName;
		ExtensionName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('.')-1).MakeLower();
		FileName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('\\')-1);
		if ((ExtensionName==_T("tak"))||(ExtensionName==_T("flac"))||(ExtensionName==_T("ape")))
		{
			if (m_Config.AcceptDragAudioFile)
			{
				if (ExtensionName==_T("flac"))
					ExtractFlacInternalCue(FileName);
				else if ((ExtensionName==_T("tak"))||(ExtensionName==_T("ape")))
					ExtractTakInternalCue(FileName);
			}
			else
			{
				if (TRUE==DealFile())
				{
					if (m_Config.AutoFixTTA) FixTTACue();
					if (m_Config.AutoFixCue) FixCue();
				}
			}
		}
		else
		{
			if (TRUE==DealFile())
			{
				if (m_Config.AutoFixTTA) FixTTACue();
				if (m_Config.AutoFixCue) FixCue();
			}
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
	m_Config.TemplateStr=CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

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

	//AcceptDragAudioFile节点
	pElem=hXmlHandle.FirstChild("AcceptDragAudioFile").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	if (_stricmp(pElem->GetText(),"true")==0)
		m_Config.AcceptDragAudioFile=TRUE;
	else
		m_Config.AcceptDragAudioFile=FALSE;

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

	//CharmapConfPath节点
	pElem=hXmlHandle.FirstChild("CharmapConfPath").Element();
	if (!pElem) return FALSE;
	if (!pElem->GetText()) return FALSE;
	m_Config.MapConfName = CC4EncodeUTF8::convert2unicode(pElem->GetText(), strlen(pElem->GetText())).c_str();

	return TRUE;
}

BOOL CAnsi2UnicodeDlg::CreateConfigFile()
{
	TiXmlDocument configdoc;
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

	TiXmlElement *AcceptDragAudioFile=new TiXmlElement("AcceptDragAudioFile");
	TiXmlText *AcceptDragAudioFileValue=new TiXmlText("true");
	AcceptDragAudioFile->LinkEndChild(AcceptDragAudioFileValue);
	configure->LinkEndChild(AcceptDragAudioFile);

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

	TiXmlElement *CharmapConfPath = new TiXmlElement("CharmapConfPath");
	TiXmlText *CharmapConfPathValue = new TiXmlText("charmap-anisong.xml");
	CharmapConfPath->LinkEndChild(CharmapConfPathValue);
	configure->LinkEndChild(CharmapConfPath);

	configdoc.LinkEndChild(dec);
	configdoc.LinkEndChild(configure);

	TiXmlPrinter printer;
	configdoc.Accept(&printer);

	CFile theFile;
	theFile.Open(m_ConfigPath,CFile::modeCreate|CFile::modeWrite);
	theFile.Write(CC4Encode::UTF_8_BOM,3);
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
	std::string &UTF8Str = CC4EncodeUTF16::getInstance()->convertWideString((LPCTSTR)m_Config.TemplateStr);
	//std::string &UTF8Str = CC4EncodeUTF16::convert2utf8((LPCTSTR)m_Config.TemplateStr, m_Config.TemplateStr.GetLength());
	//CStringA UTF8Str=CW2A(m_Config.TemplateStr,CP_UTF8);
	TiXmlText *TemplateStrValue=new TiXmlText(UTF8Str.c_str());
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

	TiXmlElement *AcceptDragAudioFile=new TiXmlElement("AcceptDragAudioFile");
	TiXmlText *AcceptDragAudioFileValue;
	if (m_Config.AcceptDragAudioFile)
		AcceptDragAudioFileValue=new TiXmlText("true");
	else
		AcceptDragAudioFileValue=new TiXmlText("false");
	AcceptDragAudioFile->LinkEndChild(AcceptDragAudioFileValue);
	configure->LinkEndChild(AcceptDragAudioFile);

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

	TiXmlElement *CharmapConfPath = new TiXmlElement("CharmapConfPath");
	TiXmlText *CharmapConfPathValue;
	CharmapConfPathValue = new TiXmlText(CC4EncodeUTF16::getInstance()->convertWideString((LPCTSTR)m_Config.MapConfName).c_str());
	//CharmapConfPathValue = new TiXmlText(CW2A(m_Config.MapConfName, CP_UTF8));
	CharmapConfPath->LinkEndChild(CharmapConfPathValue);
	configure->LinkEndChild(CharmapConfPath);

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
		dc.DrawIcon(x, y, m_hLittleIcon);
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
	return static_cast<HCURSOR>(m_hLittleIcon);
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
	if (m_FilePathName==_T(""))
		return FALSE;

	int pos;
	m_bCueFile=FALSE;
	pos=m_FilePathName.ReverseFind('.');
	if (m_FilePathName.Right(m_FilePathName.GetLength()-pos-1).MakeLower()==_T("cue"))
		m_bCueFile=TRUE;

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
		m_StringCodeType=CC4EncodeUTF16::_getName().c_str();
		int nIndex = theCombo->FindStringExact(0, m_StringCodeType);
		theCombo->SetCurSel(nIndex);
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
		m_StringCodeType=CC4EncodeUTF16::_getName().c_str();
		int nIndex = theCombo->FindStringExact(0, m_StringCodeType);
		theCombo->SetCurSel(nIndex);
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
		m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
		int nIndex = theCombo->FindStringExact(0, m_StringCodeType);
		theCombo->SetCurSel(nIndex);
		m_String=m_RawString+3; //真正的起始地址
		m_StringLength=m_RawStringLength-3; //真正的长度
	}

	if (m_bNeedConvert==FALSE)
	{
		theStatic->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);
		if (m_StringCodeType==CC4EncodeUTF16::_getName().c_str())
		{
			CString RightEditText(m_UnicodeString);
			RightEdit->SetWindowText(RightEditText);
			LeftEdit->SetWindowText(_T(""));
		}
		if (m_StringCodeType==CC4EncodeUTF8::_getName().c_str())
		{
			RightEdit->SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
			LeftEdit->SetWindowText(_T(""));
		}
	}
	else
	{
		//检测编码
		if (m_Config.AutoCheckCode==TRUE)
		{
			const CC4Encode *encode = m_context->getMostPossibleEncode(m_String);
			if (encode)
			{
				m_StringCodeType = encode->getName().c_str();
				int nIndex = theCombo->FindStringExact(0, m_StringCodeType);
				theCombo->SetCurSel(nIndex);
				m_CodeStatus = encode->getName().c_str();
			} else {
				theCombo->GetLBText(0,m_StringCodeType);
				theCombo->SetCurSel(0);
				m_CodeStatus = _T("未知编码");
			}
		}
		else
			m_CodeStatus=_T("已经关闭编码自动检测");

		theStatic->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);

		//左
		CString LeftEditText(m_String); //注意：此时LeftEditText的数据类型已经是Unicode字符串，每个字符占用两个字节
		LeftEdit->SetWindowText(LeftEditText);

		//右
		const CC4Encode *encode = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode)
			RightEdit->SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
		else
			RightEdit->SetWindowText(CString(m_String));
	}
	
	return TRUE;
}

BOOL CAnsi2UnicodeDlg::ExtractTakInternalCue(CString AudioFileName)
{
	m_CodeStatus=_T("UTF-8 (Internal Cue File)");
	m_bNeedConvert=FALSE;
	m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE))->FindStringExact(0, m_StringCodeType);
	((CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE))->SetCurSel(nIndex);
	GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);
	GetDlgItem(IDC_EDIT_ANSI)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(_T(""));

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

	if (m_FilePathName==_T(""))
		return FALSE;

	CFile OpenFile;
	if (!OpenFile.Open(m_FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
	{
		OpenFile.Close();
		::AfxMessageBox(_T("打开失败！"),MB_OK);
		return FALSE;
	}

	m_FilePathName+=_T(".cue");
	GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);

	if (OpenFile.GetLength()<20480) // 小于20K，文档太小了
	{
		OpenFile.Close();
		return FALSE;
	}

	OpenFile.Seek(-20480,CFile::end);
	unsigned char Buffer[20480]; //20k的缓冲区
	memset(Buffer,0,20480);
	OpenFile.Read(Buffer,20480);
	OpenFile.Close();

	//查找 Cuesheet 标记,自动机模型,大小写不敏感
	int state=0,BeginPos=0,EndPos=0,Length=0;
	for (int i=0;i<20480;++i)
	{
		if ((Buffer[i]>=0x41)&&(Buffer[i]<=0x5A))
			Buffer[i]=Buffer[i]+0x20;

		switch (Buffer[i])
		{
		case 'c':
			state=1;      //C
			break;
		case 'u':
			if (state==1)
				state=2;  //Cu
			else
				state=0;
			break;
		case 'e':
			switch (state)
			{
			case 2:
				state=3;  //Cue
				break;
			case 5:
				state=6;  //Cueshe
				break;
			case 6:
				state=7;  //Cueshee
				break;
			default:
				state=0;
			}
			break;
		case 's':
			if (state==3)
				state=4;  //Cues
			else
				state=0;
			break;
		case 'h':
			if (state==4)
				state=5;  //Cuesh
			else
				state=0;
			break;
		case 't':
			if (state==7)
			{
				state=8;  //Cuesheet
			}
			else
				state=0;
			break;
		default:
			state=0;
		}
		if (state==8)
		{
			BeginPos=i+2;
			break;
		}
	}
	//AfxMessageBox(_T("1"));
	if (BeginPos==0)
		return FALSE;
	//AfxMessageBox(_T("2"));
	//查找终止符 0D 0A ? 00 00 00 00 00 00 （连续六个终止符以上）
	state=0;
	for (int i=BeginPos;i<20480;++i)
	{
		switch (Buffer[i])
		{
		case '\0':
			state++;
			break;
		default:
			state=0;
		}
		if (state==6)
		{
			EndPos=i-6; //指向0D 0A后的第一个字符
			break;
		}
	}

	if (EndPos<=1)
		return FALSE;

	if ((Buffer[EndPos-2]=='\x0D')&&(Buffer[EndPos-1]=='\x0A'))
		EndPos--;

	Length=EndPos-BeginPos+1;
	if (Length<=10) //too short
		return FALSE;

	m_RawStringLength=Length;
	m_RawString=new char[m_RawStringLength+1];
	memcpy(m_RawString,Buffer+BeginPos,m_RawStringLength);
	m_RawString[m_RawStringLength]='\0';
	m_String=m_RawString;
	m_StringLength=m_RawStringLength;

	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());

	FixInternalCue(AudioFileName);

	return TRUE;
}

// flac文件结构
// http://flac.sourceforge.net/format.html
BOOL CAnsi2UnicodeDlg::ExtractFlacInternalCue(CString AudioFileName)
{
	m_CodeStatus=_T("UTF-8 (Internal Cue File)");
	m_bNeedConvert=FALSE;
	m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE))->FindStringExact(0, m_StringCodeType);
	((CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE))->SetCurSel(nIndex);
	GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);
	GetDlgItem(IDC_EDIT_ANSI)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(_T(""));

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

	if (m_FilePathName==_T(""))
		return FALSE;

	CFile OpenFile;
	if (!OpenFile.Open(m_FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
	{
		OpenFile.Close();
		::AfxMessageBox(_T("打开失败！"),MB_OK);
		return FALSE;
	}

	m_FilePathName+=_T(".cue");
	GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("文档编码检测结果：")+m_CodeStatus+_T("\n\n文档路径：")+m_FilePathName);
	if (OpenFile.GetLength()<1048576) // 小于1M，文档太小了
	{
		OpenFile.Close();
		return FALSE;
	}

	unsigned char Header[5];
	memset(Header,0,5);
	ULONGLONG position=0;
	//4个字节的头部
	OpenFile.Read((void*)Header,4);
	if (strcmp((char*)Header,"fLaC")!=0)
	{
		//AfxMessageBox(_T("Not real flac file!"));
		return FALSE;
	}

	unsigned char chr;
	unsigned char *Buffer=NULL;
	UINT Length;
	//4个字节的METADATA_BLOCK_HEADER
	do 
	{
		OpenFile.Read((void*)Header,4);
		//解析
		memcpy(&chr,Header,1);
		//检查最高位是否为1
		if ((chr&0x80)==0x80)
		{
			//最后一个METADATA_BLOCK
			if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
			{
				//读取BLOCK长度
				Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
				//申请空间
				Buffer=new unsigned char[Length+1];
				//读取BLOCK DATA
				OpenFile.Read((void*)Buffer,Length);
				Buffer[Length]='\0';
			}
			break;
		}
		else
		{
			//不是最后一个METADATA_BLOCK
			if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
			{
				//读取BLOCK长度
				Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
				//申请空间
				Buffer=new unsigned char[Length+1];
				//读取BLOCK DATA
				OpenFile.Read((void*)Buffer,Length);
				Buffer[Length]='\0';
				break;
			}
			else //不是VORBIS_COMMENT
			{
				//读取BLOCK长度
				Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
				//移动文件指针
				OpenFile.Seek(Length,CFile::current);
				position=OpenFile.GetPosition();
			}
		}
	} while(position<=1048576);

	OpenFile.Close();
	if (!Buffer)
		return FALSE;

	//查找 Cuesheet 标记,自动机模型,大小写不敏感
	int state=0,BeginPos=0,EndPos=0;
	for (UINT i=0;i<Length;++i)
	{
		if ((Buffer[i]>=0x41)&&(Buffer[i]<=0x5A))
			Buffer[i]=Buffer[i]+0x20;

		switch (Buffer[i])
		{
		case 'c':
			state=1;      //C
			break;
		case 'u':
			if (state==1)
				state=2;  //Cu
			else
				state=0;
			break;
		case 'e':
			switch (state)
			{
			case 2:
				state=3;  //Cue
				break;
			case 5:
				state=6;  //Cueshe
				break;
			case 6:
				state=7;  //Cueshee
				break;
			default:
				state=0;
			}
			break;
		case 's':
			if (state==3)
				state=4;  //Cues
			else
				state=0;
			break;
		case 'h':
			if (state==4)
				state=5;  //Cuesh
			else
				state=0;
			break;
		case 't':
			if (state==7)
			{
				state=8;  //Cuesheet
			}
			else
				state=0;
			break;
		default:
			state=0;
		}
		if (state==8)
		{
			BeginPos=i+2;
			break;
		}
	}
	if (BeginPos==0)
	{
		delete []Buffer;
		return FALSE;
	}
	//查找终止符 0D 0A ? 00 00 00（连续3个终止符以上）
	state=0;
	for (int i=BeginPos;i<20480;++i)
	{
		switch (Buffer[i])
		{
		case '\0':
			state++;
			break;
		default:
			state=0;
		}
		if (state==3)
		{
			EndPos=i-3; //指向0D 0A后的第一个字符
			break;
		}
	}

	if (EndPos<=1)
	{
		delete []Buffer;
		return FALSE;
	}

	if ((Buffer[EndPos-2]=='\x0D')&&(Buffer[EndPos-1]=='\x0A'))
		EndPos--;

	int CueLength=EndPos-BeginPos+1;
	if (CueLength<=10) //too short
	{
		delete []Buffer;
		return FALSE;
	}

	m_RawStringLength=CueLength;
	m_RawString=new char[m_RawStringLength+1];
	memcpy(m_RawString,Buffer+BeginPos,m_RawStringLength);
	m_RawString[m_RawStringLength]='\0';
	m_String=m_RawString;
	m_StringLength=m_RawStringLength;
	delete []Buffer;

	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
	FixInternalCue(AudioFileName);

	return TRUE;
}

void CAnsi2UnicodeDlg::OnFileOpen()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("文本文件(*.txt;*.cue;*.log)|*.txt;*.cue;*.log|txt文本文件(*.txt)|*.txt|cue文件(*.cue)|*.cue|log文件(*.log)|*.log|All Files (*.*)|*.*||"));
	if (openFile.DoModal() == IDOK)
	{
		m_FilePathName=openFile.GetPathName();
		CString ExtensionName,FileName;
		ExtensionName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('.')-1).MakeLower();
		FileName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('\\')-1);
		if ((ExtensionName==_T("tak"))||(ExtensionName==_T("flac"))||(ExtensionName==_T("ape")))
		{
			if (m_Config.AcceptDragAudioFile)
			{
				if (ExtensionName==_T("flac"))
					ExtractFlacInternalCue(FileName);
				else if ((ExtensionName==_T("tak"))||(ExtensionName==_T("ape")))
					ExtractTakInternalCue(FileName);
			}
			else
			{
				if (TRUE==DealFile())
				{
					if (m_Config.AutoFixTTA) FixTTACue();
					if (m_Config.AutoFixCue) FixCue();
				}
			}
		}
		else
		{
			if (TRUE==DealFile())
			{
				if (m_Config.AutoFixTTA) FixTTACue();
				if (m_Config.AutoFixCue) FixCue();
			}
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
		std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
		SaveFile.Write(CC4Encode::UTF_8_BOM, 3);
		SaveFile.Write(utf8str.c_str(), utf8str.length());
		SaveFile.Close();
	}
}

void CAnsi2UnicodeDlg::OnDropFiles(HDROP hDropInfo)
{
	if (!m_bTransferString)
	{
		int nFileCount;  
		nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
		if(nFileCount == 1)
		{
			TCHAR szFileName[MAX_PATH+1];
			::DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
			m_FilePathName=CString(szFileName);
			CString ExtensionName,FileName;
			ExtensionName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('.')-1).MakeLower();
			FileName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('\\')-1);
			if ((ExtensionName==_T("tak"))||(ExtensionName==_T("flac"))||(ExtensionName==_T("ape")))
			{
				if (m_Config.AcceptDragAudioFile)
				{
					if (ExtensionName==_T("flac"))
						ExtractFlacInternalCue(FileName);
					else if ((ExtensionName==_T("tak"))||(ExtensionName==_T("ape")))
						ExtractTakInternalCue(FileName);
				}
				else
				{
					if (TRUE==DealFile())
					{
						if (m_Config.AutoFixTTA) FixTTACue();
						if (m_Config.AutoFixCue) FixCue();
					}
				}
			}
			else
			{
				if (TRUE==DealFile())
				{
					if (m_Config.AutoFixTTA) FixTTACue();
					if (m_Config.AutoFixCue) FixCue();
				}
			}
		}
		else
		{
			::AfxMessageBox(_T(" 只能同时打开一个文件"),MB_OK);
		}
		::DragFinish(hDropInfo);
	}
	else
	{
		//抓取文件名
		int nFileCount;  
		nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
		CString LeftStr;
		for (int i=0;i<nFileCount;i++)
		{
			TCHAR szFileName[MAX_PATH+1];
			::DragQueryFile(hDropInfo,i,szFileName,MAX_PATH);
			TCHAR *pdest=wcsrchr(szFileName,'\\');
			if (pdest)
			{
				pdest++;
				LeftStr+=pdest;
				LeftStr+=_T("\x0D\x0A");
			}
		}

		::DragFinish(hDropInfo);
		GetDlgItem(IDC_EDIT_ANSI)->SetWindowText(LeftStr);
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CAnsi2UnicodeDlg::OnCbnSelchangeComboSelectcode()
{
	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);

	if (m_bTransferString)
	{
		theCombo->GetWindowText(m_StringCodeType);
		//左
		CString LeftStr;
		GetDlgItem(IDC_EDIT_ANSI)->GetWindowText(LeftStr);
		CStringA LeftAnsiStr(LeftStr);
		//右
		const CC4Encode *encode = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode)
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(encode->wconvertText((LPCSTR)LeftAnsiStr,LeftAnsiStr.GetLength()).c_str());
		else
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CString(LeftAnsiStr));
		return;
	}

	if (m_bNeedConvert)
	{
		theCombo->GetWindowText(m_StringCodeType);
		const CC4Encode *encode = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode)
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
		else
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CString(m_String));
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
	std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
	SaveFile.Write(CC4Encode::UTF_8_BOM, 3);
	SaveFile.Write(utf8str.c_str(), utf8str.length());
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
	std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
	SaveFile.Write(CC4Encode::UTF_8_BOM, 3);
	SaveFile.Write(utf8str.c_str(), utf8str.length());
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

void CAnsi2UnicodeDlg::OnBnClickedButtonDo()
{
	// 只有转换字符串时才有效
	if (m_bTransferString)
	{
		//左
		CString LeftStr;
		GetDlgItem(IDC_EDIT_ANSI)->GetWindowText(LeftStr);
		CStringA LeftAnsiStr(LeftStr);

		CComboBox *theCombo  =(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
		CStatic   *theStatic =(CStatic*)GetDlgItem(IDC_STATIC_STAT);
		theCombo->GetWindowText(m_StringCodeType);
		m_CodeStatus=_T("未知编码");

		//检测编码
		if (m_Config.AutoCheckCode==TRUE)
		{
			const CC4Encode *encode = m_context->getMostPossibleEncode(m_String);
			if (encode)
			{
				m_StringCodeType = encode->getName().c_str();
				int nIndex = theCombo->FindStringExact(0, m_StringCodeType);
				theCombo->SetCurSel(nIndex);
				m_CodeStatus = encode->getName().c_str();
			} else {
				theCombo->GetLBText(0,m_StringCodeType);
				theCombo->SetCurSel(0);
				m_CodeStatus = _T("未知编码");
			}
		}
		else
			m_CodeStatus=_T("已经关闭编码自动检测");

		theStatic->SetWindowText(_T("编码检测结果：")+m_CodeStatus);

		//右
		const CC4Encode *encode = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode)
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
		else
			GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CString(m_String));
	}
}

void CAnsi2UnicodeDlg::OnBnClickedButtonTransferstring()
{
	m_bTransferString=!m_bTransferString;
	if (m_bTransferString)
	{
		GetDlgItem(IDC_BUTTON_TRANSFERSTRING)->SetWindowText(_T("切换到转换文档"));
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVEAS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DO)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("编码检测结果："));
		CMenu *pM=m_menu.GetSubMenu(0);
		pM->EnableMenuItem(ID_FILE_OPEN,MF_GRAYED);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TRANSFERSTRING)->SetWindowText(_T("切换到转换字符串"));
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVEAS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DO)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STAT)->SetWindowText(_T("文档编码检测结果：\n\n文档路径："));
		//恢复
		CMenu *pM=m_menu.GetSubMenu(0);
		pM->EnableMenuItem(ID_FILE_OPEN,MF_ENABLED);
		GetDlgItem(IDC_EDIT_ANSI)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(_T(""));
		m_FilePathName=_T("");
		m_bNeedConvert=FALSE;
		/*
		if (TRUE==DealFile())
		{
			if (m_Config.AutoFixTTA) FixTTACue();
			if (m_Config.AutoFixCue) FixCue();
		}
		*/
	}
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
	if (!m_bCueFile)
		return;

	FixTTACue();

	CString CueString;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(CueString);
	int BeginPos=CueString.Find(_T("FILE \""));
	if (BeginPos==-1)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}
	int EndPos=CueString.Find(_T("\" WAVE"));
	if (EndPos==-1)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}
	BeginPos+=6;
	if (BeginPos>=EndPos)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}

	CString MusicFileName,MusicFilePath; //音频文件名，路径
	MusicFileName=CueString.Mid(BeginPos,EndPos-BeginPos);

	//依据文档路径：m_FilePathName查找音频文件
	int pos=m_FilePathName.ReverseFind('\\');
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

void CAnsi2UnicodeDlg::FixInternalCue(CString AudioFileName)
{
	CString CueString;
	GetDlgItem(IDC_EDIT_UNICODE)->GetWindowText(CueString);
	int BeginPos=CueString.Find(_T("FILE \""));
	if (BeginPos==-1)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}
	int EndPos=CueString.Find(_T("\" WAVE"));
	if (EndPos==-1)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}
	BeginPos+=6;
	if (BeginPos>=EndPos)
	{
		if (!m_Config.CloseCuePrompt) MessageBox(_T("cue文件异常"));
		return;
	}

	CString OldFileName; //音频文件名
	OldFileName=CueString.Mid(BeginPos,EndPos-BeginPos);
	CueString.Replace(OldFileName,AudioFileName);
	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(CueString);
}

void CAnsi2UnicodeDlg::FixTTACue()
{
	if (!m_bCueFile)
		return;

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