/************************************************************************/
/*  Ansi to Unicode 1.0.3                                               */
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
	BOOL AutoFixCue;                       //�Զ�����cue�е���Ƶ�ļ���չ��
	BOOL AutoFixTTA;                       //�Զ�������ʽTTA��ǩ
	BOOL AcceptDragAudioFile;              //������ҷ��Ƶ�ĵ���ȡ��Ƕcue
	BOOL AutoCheckCode;                    //�Ƿ��Զ�������
	BOOL AlwaysOnTop;                      //�Ƿ�������ǰ
	BOOL CloseCuePrompt;                   //�Ƿ�ر�cue�ļ��д������ʾ
	BOOL RegNewUniFile;                    //ע���½�uni�ļ�
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
	HICON    m_hLittleIcon;
	HICON    m_hBigIcon;
	CMenu    m_menu;                //�˵�
	BOOL     m_bNeedConvert;        //��Ҫת��
	char*    m_RawString;           //ԭʼ�ַ��������ı���ȡ����BOM��
	UINT     m_RawStringLength;     //ԭʼ�ַ����ĳ��ȣ����ı���ȡ����BOM����)
	char*    m_String;              //�ַ���������BOM��
	UINT     m_StringLength;        //�ַ����ĳ��ȣ�����BOM��
	int      m_StringCodeType;      //�ַ�����������
	wchar_t* m_UnicodeString;       //Unicode�ַ���
	UINT     m_UnicodeLength;       //Unicode�ַ����ĳ���
	CString  m_FilePathName;        //�ı��ļ�·��
	CString  m_CodeStatus;          //������״̬
	CConfig  m_Config;              //����
	CString  m_ConfigPath;          //�����ļ�·��
	//BOOL     m_bConfigLoaded;     //���óɹ����ر��
	//BOOL     m_bCommandLineOpen;  //�����в������
	BOOL     m_bCueFile;            //�ı���cue�ļ�
	BOOL     m_bTransferString;    //�л���ת���ַ���״̬

	BOOL LoadConfigFile(TiXmlDocument *xmlfile);
	BOOL CreateConfigFile();
	BOOL SaveConfigFile();
	void FixCue();
	void FixInternalCue(CString AudioFileName);
	void FixTTACue();
	BOOL SetDialogPos();
	BOOL DealFile();
	BOOL ExtractTakInternalCue(CString AudioFileName);
	BOOL ExtractFlacInternalCue(CString AudioFileName);

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
	afx_msg void OnBnClickedCheckAlwaysontop();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonTransferstring();
};
