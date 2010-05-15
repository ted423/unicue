// Big5toUnicodeDlg.h : 头文件
//

#pragma once


// CBig5toUnicodeDlg 对话框
class CBig5toUnicodeDlg : public CDialog
{
// 构造
public:
	CBig5toUnicodeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CBig5toUnicodeDlg();

// 对话框数据
	enum { IDD = IDD_BIG5TOUNICODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu m_menu;                //菜单
	UINT  m_big5Length;          //big5字符串的长度
	int   m_unicodeLength;       //unicode字符串的长度
	char* m_big5Str;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileExit();
	afx_msg void OnAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnBnClickedButtonDo();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
