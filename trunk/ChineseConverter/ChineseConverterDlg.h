/************************************************************************/
/*  ��ת�� 1.0                                                        */
/*  kuyur (kuyur@kuyur.info)  -->twitter: @kuyur                        */
/*  http://kuyur.info/blog  http://code.google.com/p/unicue             */
/*  Distributed under GPLv3                                             */
/************************************************************************/

// ChineseConverterDlg.h : ͷ�ļ�
//

#pragma once


// CChineseConverterDlg �Ի���
class CChineseConverterDlg : public CDialogEx
{
// ����
public:
	CChineseConverterDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CChineseConverterDlg();

// �Ի�������
	enum { IDD = IDD_CHINESECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON    m_hIcon;
	wchar_t* m_RawString;           // ԭʼ�ַ��������ı���ȡ����BOM��
	UINT     m_RawStringLength;     // ԭʼ�ַ����ĳ��ȣ����ı���ȡ����BOM����)
	wchar_t* m_String;              // �ַ���������BOM��
	UINT     m_StringLength;        // �ַ����ĳ��ȣ�����BOM��
	wchar_t* m_UnicodeString;       // Unicode�ַ���
	UINT     m_UnicodeLength;       // Unicode�ַ����ĳ���
	CString  m_FilePathName;        // �ı��ļ�·��
	CC4Context* m_context;          // converting context
	BOOL DealFile();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonSaveas();
};
