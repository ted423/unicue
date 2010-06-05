// Ansi2UnicodeDlg.h : ͷ�ļ�
//

#pragma once

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

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void OnCbnSelchangeComboSelectcode();
};
