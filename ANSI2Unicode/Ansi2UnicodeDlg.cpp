// Ansi2UnicodeDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "Ansi2Unicode.h"
#include "Ansi2UnicodeDlg.h"
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

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAnsi2UnicodeDlg 对话框




CAnsi2UnicodeDlg::CAnsi2UnicodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnsi2UnicodeDlg::IDD, pParent),m_bNeedConvert(TRUE),m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0),
	m_StringCodeType(CODETYPE_DEFAULT),m_AutoCheckCode(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RawString=NULL;
	m_String=NULL;
	m_UnicodeString=NULL;
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
	DDX_Check(pDX, IDC_CHECK_AUTOCHECKCODE, m_AutoCheckCode);
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
	theCombo->InsertString(-1,_T("Default"));
	theCombo->InsertString(-1,_T("GBK"));
	theCombo->InsertString(-1,_T("Big5"));
	theCombo->InsertString(-1,_T("Shift-JIS"));
	theCombo->InsertString(-1,_T("UTF-8"));
	theCombo->InsertString(-1,_T("Unicode(UTF-16)"));
	theCombo->SetCurSel(m_StringCodeType);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

void CAnsi2UnicodeDlg::OnFileOpen()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("txt文本文件(*.txt)|*.txt|cue文件(*.cue)|*.cue|log文件(*.log)|*.log||"));
	if (openFile.DoModal() == IDOK)
	{
		CFile OpenFile;
		if (!OpenFile.Open(openFile.GetPathName(),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("打开失败！"),MB_OK);
			return;
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

		CComboBox *theCombo;
		theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);

		// Unicode(little-endian)
		if (((unsigned char)m_RawString[0]==0xFF)&&((unsigned char)m_RawString[1]==0xFE))
		{
			m_bNeedConvert=FALSE;
			m_StringCodeType=CODETYPE_UNICODE;
			theCombo->SetCurSel(m_StringCodeType);
			m_String=m_RawString+2; //真正的起始地址
			m_StringLength=m_RawStringLength-2; //真正的长度
			if ((m_RawStringLength%2)!=0)
			{
				MessageBox(_T("文本错误！"));
				return;
			}
			m_UnicodeLength=m_StringLength>>1;
			m_UnicodeString=new wchar_t[m_UnicodeLength+1];
			memcpy((void*)m_UnicodeString,m_String,m_StringLength);
			m_UnicodeString[m_UnicodeLength]='\0';
		}
		// Unicode(big-endian)
		if (((unsigned char)m_RawString[0]==0xFE)&&((unsigned char)m_RawString[1]==0xFF))
		{
			m_bNeedConvert=FALSE;
			m_StringCodeType=CODETYPE_UNICODE;
			theCombo->SetCurSel(m_StringCodeType);
			m_String=m_RawString+2; //真正的起始地址
			m_StringLength=m_RawStringLength-2; //真正的长度
			if ((m_RawStringLength%2)!=0)
			{
				MessageBox(_T("文本错误！"));
				return;
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
			m_bNeedConvert=FALSE;
			m_StringCodeType=CODETYPE_UTF8;
			theCombo->SetCurSel(m_StringCodeType);
			m_String=m_RawString+3; //真正的起始地址
			m_StringLength=m_RawStringLength-3; //真正的长度
		}

		if (m_bNeedConvert==FALSE)
		{
			CEdit *LeftEdit=(CEdit *)GetDlgItem(IDC_EDIT_ANSI);
			if (m_StringCodeType==CODETYPE_UNICODE)
			{
				CString LeftEditText(m_UnicodeString);
				LeftEdit->SetWindowText(LeftEditText);
			}
			if (m_StringCodeType==CODETYPE_UTF8)
			{
				LeftEdit->SetWindowText(UTF8toUnicde(m_String,m_StringLength));
			}
		}
		else
		{
			//检测编码
			if (m_AutoCheckCode==TRUE)
			{
				m_StringCodeType=CheckCodeType(m_String,m_StringLength);
				theCombo->SetCurSel(m_StringCodeType);
			}

			CString LeftEditText(m_String); //注意：此时LeftEditText的数据类型已经是Unicode字符串，每个字符占用两个字节
			CEdit *LeftEdit=(CEdit *)GetDlgItem(IDC_EDIT_ANSI);
			LeftEdit->SetWindowText(LeftEditText);
		}
	}
}

void CAnsi2UnicodeDlg::OnFileSave()
{
	CFileDialog saveFile(FALSE,_T("*.txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,_T("txt文本文件(*.txt)|*.txt|cue文件(*.cue)|*.cue|log文件(*.log)|*.log||"));
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
	if (m_bNeedConvert)
		GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(AnsiToUnicode(m_String,m_StringLength,m_StringCodeType));
	else
		MessageBox(_T("Unicode或UTF-8文本，无需转换!"));
}

void CAnsi2UnicodeDlg::OnDropFiles(HDROP hDropInfo)
{
	int nFileCount;  
	nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if(nFileCount == 1)
	{
		TCHAR szFileName[MAX_PATH+1];
		::DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		CFile OpenFile;
		if (!OpenFile.Open(szFileName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("打开失败！"),MB_OK);
		}
		else
		{
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

			// Unicode(little-endian)
			if (((unsigned char)m_RawString[0]==0xFF)&&((unsigned char)m_RawString[1]==0xFE))
			{
				m_bNeedConvert=FALSE;
				m_StringCodeType=CODETYPE_UNICODE;
				CComboBox *theCombo;
				theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
				theCombo->SetCurSel(m_StringCodeType);
				m_String=m_RawString+2; //真正的起始地址
				m_StringLength=m_RawStringLength-2; //真正的长度
				if ((m_RawStringLength%2)!=0)
				{
					MessageBox(_T("文本错误！"));
					return;
				}
				m_UnicodeLength=m_StringLength>>1;
				m_UnicodeString=new wchar_t[m_UnicodeLength+1];
				memcpy((void*)m_UnicodeString,m_String,m_StringLength);
				m_UnicodeString[m_UnicodeLength]='\0';
			}
			// Unicode(big-endian)
			if (((unsigned char)m_RawString[0]==0xFE)&&((unsigned char)m_RawString[1]==0xFF))
			{
				m_bNeedConvert=FALSE;
				m_StringCodeType=CODETYPE_UNICODE;
				CComboBox *theCombo;
				theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
				theCombo->SetCurSel(m_StringCodeType);
				m_String=m_RawString+2; //真正的起始地址
				m_StringLength=m_RawStringLength-2; //真正的长度
				if ((m_RawStringLength%2)!=0)
				{
					MessageBox(_T("文本错误！"));
					return;
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
				m_bNeedConvert=FALSE;
				m_StringCodeType=CODETYPE_UTF8;
				CComboBox *theCombo;
				theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
				theCombo->SetCurSel(m_StringCodeType);
				m_String=m_RawString+3; //真正的起始地址
				m_StringLength=m_RawStringLength-3; //真正的长度
			}

			if (m_bNeedConvert==FALSE)
			{
				CEdit *LeftEdit=(CEdit *)GetDlgItem(IDC_EDIT_ANSI);
				if (m_StringCodeType==CODETYPE_UNICODE)
				{
					CString LeftEditText(m_UnicodeString);
					LeftEdit->SetWindowText(LeftEditText);
				}
				if (m_StringCodeType==CODETYPE_UTF8)
				{
					LeftEdit->SetWindowText(UTF8toUnicde(m_String,m_StringLength));
				}
			}
			else
			{
				//检测编码
				if (m_AutoCheckCode==TRUE)
				{
					m_StringCodeType=CheckCodeType(m_String,m_StringLength);
					CComboBox *theCombo;
					theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
					theCombo->SetCurSel(m_StringCodeType);
				}

				CString LeftEditText(m_String); //注意：此时LeftEditText的数据类型已经是Unicode字符串，每个字符占用两个字节
				CEdit *LeftEdit=(CEdit *)GetDlgItem(IDC_EDIT_ANSI);
				LeftEdit->SetWindowText(LeftEditText);
			}
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
	}
}
