
// ChineseConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChineseConverter.h"
#include "ChineseConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChineseConverterDlg 对话框




CChineseConverterDlg::CChineseConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChineseConverterDlg::IDD, pParent), m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RawString=NULL;
	m_String=NULL;
	m_UnicodeString=NULL;
	m_FilePathName="";
	TCHAR szFull[_MAX_PATH];
	GetModuleFileName(NULL,szFull,_MAX_PATH);
	CString processPath(szFull);
	int pos;
	pos=processPath.ReverseFind('\\');
	processPath=processPath.Left(pos+1);
	m_context = new CC4Context(L"charmap-chinese.xml", (LPCTSTR)processPath);
	if (!m_context->init())
		AfxMessageBox(_T("Failed to load charmaps!"));
}

CChineseConverterDlg::~CChineseConverterDlg()
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

void CChineseConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChineseConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CChineseConverterDlg::OnBnClickedButtonSaveas)
END_MESSAGE_MAP()


// CChineseConverterDlg 消息处理程序

BOOL CChineseConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
	std::list<std::wstring>::iterator iter;
	for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
	{
		if (iter->compare(L"UTF-16") == 0) continue;
		if (iter->compare(L"UTF-8") == 0) continue;
		theCombo->InsertString(-1, iter->c_str());
	}
	theCombo->SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChineseConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChineseConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChineseConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChineseConverterDlg::DealFile()
{
	if (m_FilePathName==_T(""))
		return FALSE;

	CFile OpenFile;
	if (!OpenFile.Open(m_FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
	{
		OpenFile.Close();
		::AfxMessageBox(_T("打开失败！"),MB_OK);
		return FALSE;
	}

	UINT fileLength = OpenFile.GetLength();
	if ((fileLength&1) != 0)
	{
		OpenFile.Close();
		::AfxMessageBox(_T("错误的文件格式。仅支持Unicode(LE)"),MB_OK);
		return FALSE;
	}
	m_RawStringLength=fileLength>>1;
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
	m_RawString=new wchar_t[m_RawStringLength+1];
	OpenFile.Read((void*)m_RawString,m_RawStringLength*sizeof(wchar_t));
	OpenFile.Close();
	m_RawString[m_RawStringLength]=0x0000;

	CEdit *LeftEdit =(CEdit *)GetDlgItem(IDC_EDIT_LEFT);
	CEdit *RightEdit=(CEdit *)GetDlgItem(IDC_EDIT_RIGHT);

	if (m_RawString[0] != CC4Encode::LITTLEENDIAN_MARK)
	{
		if (m_RawString)
		{
			delete []m_RawString;
			m_RawString=NULL;
			m_String=NULL;
		}
		LeftEdit->SetWindowText(_T(""));
		RightEdit->SetWindowText(_T(""));

		::AfxMessageBox(_T("错误的文件格式。仅支持Unicode(LE)"),MB_OK);
		return FALSE;
	}
	m_String=m_RawString+1;
	m_StringLength=m_RawStringLength-1;

	LeftEdit->SetWindowText(m_String);

	CComboBox *theCombo  =(CComboBox*)GetDlgItem(IDC_COMBO_SELECTCODE);
	CString encodeName;
	theCombo->GetWindowText(encodeName);
	const CC4Encode *encode = m_context->getEncode((LPCTSTR)encodeName);
	if (encode)
	{
		m_UnicodeLength = m_StringLength;
		m_UnicodeString = new wchar_t[m_UnicodeLength+1];
		m_UnicodeString[m_UnicodeLength] = 0;
		std::wstring &convertResult = encode->wconvertWideText(m_String, m_StringLength);
		wmemcpy(m_UnicodeString, convertResult.c_str(), m_StringLength);
		RightEdit->SetWindowText(m_UnicodeString);
	}
	else
	{
		RightEdit->SetWindowText(_T(""));
	}

	return TRUE;
}

void CChineseConverterDlg::OnDropFiles(HDROP hDropInfo)
{
	int nFileCount;  
	nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if(nFileCount == 1)
	{
		TCHAR szFileName[MAX_PATH+1];
		::DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		m_FilePathName=CString(szFileName);
		DealFile();
	}
	else
	{
		::AfxMessageBox(_T(" 只能同时打开一个文件"),MB_OK);
	}
	::DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CChineseConverterDlg::OnBnClickedButtonSaveas()
{
	if (!m_UnicodeString)
	{
		::AfxMessageBox(_T("转换内容为空！"),MB_OK);
		return;
	}
	CString FilePath,FileType;
	int position=m_FilePathName.ReverseFind('.');
	FilePath=m_FilePathName.Left(position);
	FileType=m_FilePathName.Right(m_FilePathName.GetLength()-position);
	FilePath+=_T(".convert");
	FilePath+=FileType;

	CFile SaveFile;
	if (!SaveFile.Open(FilePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
	{
		::AfxMessageBox(_T("无法写入文件！"),MB_OK);
		return;
	}

	SaveFile.Write(CC4Encode::LITTLEENDIAN_BOM, 2);
	SaveFile.Write((void*)m_UnicodeString, m_UnicodeLength*sizeof(wchar_t));
	SaveFile.Close();
}
