// Big5toUnicodeDlg.h : ͷ�ļ�
//

#pragma once


// CBig5toUnicodeDlg �Ի���
class CBig5toUnicodeDlg : public CDialog
{
// ����
public:
	CBig5toUnicodeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CBig5toUnicodeDlg();

// �Ի�������
	enum { IDD = IDD_BIG5TOUNICODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_menu;                //�˵�
	UINT  m_big5Length;          //big5�ַ����ĳ���
	int   m_unicodeLength;       //unicode�ַ����ĳ���
	char* m_big5Str;

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
};
