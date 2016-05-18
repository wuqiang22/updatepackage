
// SvnDiffGUIDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSvnDiffGUIDlg 对话框
class CSvnDiffGUIDlg : public CDialogEx
{
// 构造
public:
	CSvnDiffGUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SVNDIFFGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void initData();  //去初始化数据  
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
