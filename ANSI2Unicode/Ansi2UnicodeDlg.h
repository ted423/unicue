/************************************************************************/
/*  Ansi to Unicode 1.0.2                                               */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// Ansi2UnicodeDlg.h : ͷ�ļ�
//

#pragma once
#include "..\misc\tinyxml.h"

//����
typedef struct CConfig_tag
{
	CString TemplateStr;                   //����ģ��
	BOOL AutoFixCue;                       //
	BOOL AutoFixTTA;                       //
	BOOL AcceptDragFLAC;                   //
	BOOL AcceptDragTAK;                    //
	BOOL AcceptDragAPE;                    //
}CConfig;

// CAnsi2UnicodeDlg �Ի���
class CAnsi2UnicodeDlg : public CDialog
{
// ����
public:
	CAnsi2UnicodeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CAnsi2UnicodeDlg();

// �Ի�������
	enum { IDD = IDD_ANSI2UNICODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON    m_hIcon;
	CMenu    m_menu;                //�˵�
	BOOL     m_bNeedConvert;        //��Ҫת��
	char*    m_RawString;           //ԭʼ�ַ��������ı���ȡ����BOM��
	UINT     m_RawStringLength;     //ԭʼ�ַ����ĳ��ȣ����ı���ȡ����BOM����)
	char*    m_String;              //�ַ���������BOM��
	UINT     m_StringLength;        //�ַ����ĳ��ȣ�����BOM��
	int      m_StringCodeType;      //�ַ�����������
	wchar_t* m_UnicodeString;       //Unicode�ַ���
	UINT     m_UnicodeLength;       //Unicode�ַ����ĳ���
	BOOL     m_AutoCheckCode;       //�Ƿ��Զ�������
	CString  m_FilePathName;        //�ı��ļ�·��
	CString  m_CodeStatus;          //������״̬
	CConfig  m_Config;              //����
	CString  m_ConfigPath;          //�����ļ�·��
	BOOL     m_bConfigLoaded;

	BOOL LoadConfigFile(TiXmlDocument *xmlfile);
	BOOL CreateConfigFile();
	BOOL SaveConfigFile();
	void FixCue();
	void FixTTACue();

	// ���ɵ���Ϣӳ�亯��
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
