// Big5toUnicodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Big5toUnicode.h"
#include "Big5toUnicodeDlg.h"
#include "b2u-CString.h"

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


// CBig5toUnicodeDlg 对话框




CBig5toUnicodeDlg::CBig5toUnicodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBig5toUnicodeDlg::IDD, pParent),m_big5Length(0),m_unicodeLength(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_big5Str=NULL;
}

CBig5toUnicodeDlg::~CBig5toUnicodeDlg()
{
	if (m_big5Str)
	{
		delete []m_big5Str;
		m_big5Str=NULL;
	}
}

void CBig5toUnicodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBig5toUnicodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, &CBig5toUnicodeDlg::OnFileExit)
	ON_COMMAND(ID_ABOUT, &CBig5toUnicodeDlg::OnAbout)
	ON_COMMAND(ID_FILE_OPEN, &CBig5toUnicodeDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CBig5toUnicodeDlg::OnFileSave)
	ON_BN_CLICKED(IDC_BUTTON_DO, &CBig5toUnicodeDlg::OnBnClickedButtonDo)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CBig5toUnicodeDlg 消息处理程序

BOOL CBig5toUnicodeDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBig5toUnicodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBig5toUnicodeDlg::OnPaint()
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
HCURSOR CBig5toUnicodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBig5toUnicodeDlg::OnFileExit()
{
	SendMessage(WM_CLOSE);
}

void CBig5toUnicodeDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CBig5toUnicodeDlg::OnFileOpen()
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
		m_big5Length=OpenFile.GetLength();
		if (m_big5Str)
		{
			delete []m_big5Str;
			m_big5Str=NULL;
		}
		m_big5Str=new char[m_big5Length+1];
		OpenFile.Read(m_big5Str,m_big5Length);
		OpenFile.Close();

		m_big5Str[m_big5Length]='\0';
		CString Big5Str(m_big5Str); //注意：此时Big5Str的数据类型已经是Unicode字符串，每个字符占用两个字节
		CEdit *Big5Edit=(CEdit *)GetDlgItem(IDC_EDIT_BIG5);
		Big5Edit->SetWindowText(Big5Str);
	}
}

void CBig5toUnicodeDlg::OnFileSave()
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

void CBig5toUnicodeDlg::OnBnClickedButtonDo()
{
	GetDlgItem(IDC_EDIT_UNICODE)->SetWindowText(Big5ToUnicode(m_big5Str,m_big5Length));
}

void CBig5toUnicodeDlg::OnDropFiles(HDROP hDropInfo)
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
			m_big5Length=OpenFile.GetLength();
			if (m_big5Str)
			{
				delete []m_big5Str;
				m_big5Str=NULL;
			}
			m_big5Str=new char[m_big5Length+1];
			OpenFile.Read(m_big5Str,m_big5Length);
			OpenFile.Close();

			m_big5Str[m_big5Length]='\0';
			CString Big5Str(m_big5Str);
			CEdit *Big5Edit=(CEdit *)GetDlgItem(IDC_EDIT_BIG5);
			Big5Edit->SetWindowText(Big5Str);
		}
	}
	else
	{
		::AfxMessageBox(_T(" 只能同时打开一个文件"),MB_OK);
	}
	::DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}
