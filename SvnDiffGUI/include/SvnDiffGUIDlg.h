
// SvnDiffGUIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSvnDiffGUIDlg �Ի���
class CSvnDiffGUIDlg : public CDialogEx
{
// ����
public:
	CSvnDiffGUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SVNDIFFGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void initData();  //ȥ��ʼ������  
	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelectfile();
private:
	CEdit m_filepath;
	CEdit m_version1;
	CEdit m_version2;
	afx_msg void OnBnClickedBtnPackage();
	afx_msg void OnBnClickedBtnSelectSavePath();
	CEdit m_savepath;
	CEdit m_version;
	CEdit m_fixversion;

	CEdit m_remoteVersion;
	CEdit m_remotemanifest;
};
