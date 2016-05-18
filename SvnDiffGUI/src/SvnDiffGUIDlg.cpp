
// SvnDiffGUIDlg.cpp : 实现文件
//

#include "UpdatePackage.h"
#include "stdafx.h"

#include "SvnDiffGUI.h"
#include "SvnDiffGUIDlg.h"
#include "afxdialogex.h"
#include "afx.h"
#include "afxdlgs.h"
#include "Conversion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSvnDiffGUIDlg 对话框



CSvnDiffGUIDlg::CSvnDiffGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSvnDiffGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSvnDiffGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_FILE_PATH, m_filepath);
	DDX_Control(pDX, ID_VERSION_1, m_version1);
	DDX_Control(pDX, ID_VERSION_2, m_version2);
	DDX_Control(pDX, ID_FILE_PATH2, m_savepath);
	DDX_Control(pDX, ID_VERSION_4, m_version);
	DDX_Control(pDX, ID_VERSION_3, m_fixversion);
	DDX_Control(pDX, ID_FILE_PATH3, m_remoteVersion);
	DDX_Control(pDX, ID_FILE_PATH4, m_remotemanifest);
}

BEGIN_MESSAGE_MAP(CSvnDiffGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSvnDiffGUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BTN_SELECTFILE, &CSvnDiffGUIDlg::OnBnClickedBtnSelectfile)
	ON_BN_CLICKED(ID_BTN_PACKAGE, &CSvnDiffGUIDlg::OnBnClickedBtnPackage)
	ON_BN_CLICKED(ID_BTN_SELECT_SAVE_PATH, &CSvnDiffGUIDlg::OnBnClickedBtnSelectSavePath)
END_MESSAGE_MAP()


// CSvnDiffGUIDlg 消息处理程序

BOOL CSvnDiffGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_version1.SetWindowText(_T("0"));
	m_version2.SetWindowText(_T("0"));
	m_version.SetWindowText(_T("1.0.0.0"));
	m_fixversion.SetWindowText(_T("1.0.0.0"));
	m_filepath.SetWindowText(_T("E:/svnworkspace/projectmax/thelastday/projects/TheLastDay/trunk/TheLastDay/res/version/project.manifest"));
	
	initData();
	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSvnDiffGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSvnDiffGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSvnDiffGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSvnDiffGUIDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CSvnDiffGUIDlg::OnBnClickedBtnSelectfile()
{
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("所有文件|*||"));
	if (dlg.DoModal() == IDOK) {
		this->m_filepath.SetWindowText(dlg.GetPathName());
	}
}


void CSvnDiffGUIDlg::initData()
{
	if (!UpdatePackage::getInstance()->getVersion())
	{
		AfxMessageBox(_T("获取版本失败"));
		return;
	}
	size_t versionBegin = UpdatePackage::getInstance()->getNewVersionBegin();
	size_t versionEnd = UpdatePackage::getInstance()->getNewVersionEnd();

	m_version1.SetWindowText(int2cstring(versionBegin));
	m_version2.SetWindowText(int2cstring(versionEnd));
}

void CSvnDiffGUIDlg::OnBnClickedBtnPackage()
{
	CString versionBegin, versionEnd,packageUrl,version,fixversion,savepath,remoteManifestUrl,remoteVersionUrl;
	m_version1.GetWindowText(versionBegin);
	m_version2.GetWindowText(versionEnd);
	m_filepath.GetWindowText(packageUrl);
	m_remotemanifest.GetWindowText(remoteManifestUrl);
	m_remoteVersion.GetWindowText(remoteVersionUrl);
	


	m_version.GetWindowText(version);
	m_fixversion.GetWindowText(fixversion);
	m_savepath.GetWindowText(savepath);

	UpdatePackage::getInstance()->setPackageUrl(cstring2string(packageUrl));
	UpdatePackage::getInstance()->setVersion(cstring2int(versionBegin),cstring2int(versionEnd));
	UpdatePackage::getInstance()->setThisVersion(cstring2string(version),cstring2string(fixversion));
	UpdatePackage::getInstance()->setUpdatePackagePath(cstring2string(savepath));
	UpdatePackage::getInstance()->setRemoteInfo(cstring2string(remoteVersionUrl), cstring2string(remoteManifestUrl));

	std::string reason;
	if (UpdatePackage::getInstance()->startWork(reason))
	{
		AfxMessageBox(_T("打包成功"));
	}
	else{
		CString str;
		str.Format(_T("打包失败 原因：%s", reason.c_str()));
		AfxMessageBox(str);
	}

}


void CSvnDiffGUIDlg::OnBnClickedBtnSelectSavePath()
{
	wchar_t szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择需要打包的目录：");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		m_savepath.SetWindowText(szPath);
	}
	else
		AfxMessageBox(_T("无效的目录，请重新选择"));
}
