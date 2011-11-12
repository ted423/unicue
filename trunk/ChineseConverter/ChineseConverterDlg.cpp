
// ChineseConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChineseConverter.h"
#include "ChineseConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChineseConverterDlg �Ի���




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


// CChineseConverterDlg ��Ϣ�������

BOOL CChineseConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ӱ���ѡ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChineseConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		::AfxMessageBox(_T("��ʧ�ܣ�"),MB_OK);
		return FALSE;
	}

	UINT fileLength = OpenFile.GetLength();
	if ((fileLength&1) != 0)
	{
		OpenFile.Close();
		::AfxMessageBox(_T("������ļ���ʽ����֧��Unicode(LE)"),MB_OK);
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

		::AfxMessageBox(_T("������ļ���ʽ����֧��Unicode(LE)"),MB_OK);
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
		::AfxMessageBox(_T(" ֻ��ͬʱ��һ���ļ�"),MB_OK);
	}
	::DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CChineseConverterDlg::OnBnClickedButtonSaveas()
{
	if (!m_UnicodeString)
	{
		::AfxMessageBox(_T("ת������Ϊ�գ�"),MB_OK);
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
		::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
		return;
	}

	SaveFile.Write(CC4Encode::LITTLEENDIAN_BOM, 2);
	SaveFile.Write((void*)m_UnicodeString, m_UnicodeLength*sizeof(wchar_t));
	SaveFile.Close();
}
