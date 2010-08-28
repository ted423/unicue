/************************************************************************/
/*  Ansi to Unicode 1.0.2                                               */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// Ansi2UnicodeDlg.h : 头文件
//

#pragma once
#include "..\misc\tinyxml.h"

//配置
typedef struct CConfig_tag
{
	CString TemplateStr;                   //命名模板
	BOOL AutoFixCue;                       //
	BOOL AutoFixTTA;                       //
	BOOL AcceptDragFLAC;                   //
	BOOL AcceptDragTAK;                    //
	BOOL AcceptDragAPE;                    //
}CConfig;

// CAnsi2UnicodeDlg 对话框
class CAnsi2UnicodeDlg : public CDialog
{
// 构造
public:
	CAnsi2UnicodeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CAnsi2UnicodeDlg();

// 对话框数据
	enum { IDD = IDD_ANSI2UNICODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON    m_hIcon;
	CMenu    m_menu;                //菜单
	BOOL     m_bNeedConvert;        //需要转换
	char*    m_RawString;           //原始字符串（从文本读取，含BOM）
	UINT     m_RawStringLength;     //原始字符串的长度（从文本获取，含BOM长度)
	char*    m_String;              //字符串（不含BOM）
	UINT     m_StringLength;        //字符串的长度（不含BOM）
	int      m_StringCodeType;      //字符串编码类型
	wchar_t* m_UnicodeString;       //Unicode字符串
	UINT     m_UnicodeLength;       //Unicode字符串的长度
	BOOL     m_AutoCheckCode;       //是否自动检查编码
	CString  m_FilePathName;        //文本文件路径
	CString  m_CodeStatus;          //编码检测状态
	CConfig  m_Config;              //配置
	CString  m_ConfigPath;          //配置文件路径
	BOOL     m_bConfigLoaded;

	BOOL LoadConfigFile(TiXmlDocument *xmlfile);
	BOOL CreateConfigFile();
	BOOL SaveConfigFile();
	void FixCue();
	void FixTTACue();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CConfig GetConfig();
	afx_msg void OnFileExit();
	afx_msg void OnAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnBnClickedButtonDo();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnCbnSelchangeComboSelectcode();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSaveas();
	afx_msg void OnBnClickedCheckAutocheckcode();
	afx_msg void OnFileOption();
};
