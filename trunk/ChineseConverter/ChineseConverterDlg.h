/************************************************************************/
/*  简繁转换 1.0                                                        */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// ChineseConverterDlg.h : 头文件
//

#pragma once


// CChineseConverterDlg 对话框
class CChineseConverterDlg : public CDialogEx
{
// 构造
public:
	CChineseConverterDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CChineseConverterDlg();

// 对话框数据
	enum { IDD = IDD_CHINESECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON    m_hIcon;
	wchar_t* m_RawString;           // 原始字符串（从文本读取，含BOM）
	UINT     m_RawStringLength;     // 原始字符串的长度（从文本获取，含BOM长度)
	wchar_t* m_String;              // 字符串（不含BOM）
	UINT     m_StringLength;        // 字符串的长度（不含BOM）
	wchar_t* m_UnicodeString;       // Unicode字符串
	UINT     m_UnicodeLength;       // Unicode字符串的长度
	CString  m_FilePathName;        // 文本文件路径
	CC4Context* m_context;          // converting context
	BOOL DealFile();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonSaveas();
};
