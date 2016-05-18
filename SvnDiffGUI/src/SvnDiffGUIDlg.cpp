
// SvnDiffGUIDlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSvnDiffGUIDlg �Ի���



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


// CSvnDiffGUIDlg ��Ϣ�������

BOOL CSvnDiffGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_version1.SetWindowText(_T("0"));
	m_version2.SetWindowText(_T("0"));
	m_version.SetWindowText(_T("1.0.0.0"));
	m_fixversion.SetWindowText(_T("1.0.0.0"));
	m_filepath.SetWindowText(_T("E:/svnworkspace/projectmax/thelastday/projects/TheLastDay/trunk/TheLastDay/res/version/project.manifest"));
	
	initData();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSvnDiffGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSvnDiffGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSvnDiffGUIDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CSvnDiffGUIDlg::OnBnClickedBtnSelectfile()
{
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("�����ļ�|*||"));
	if (dlg.DoModal() == IDOK) {
		this->m_filepath.SetWindowText(dlg.GetPathName());
	}
}


void CSvnDiffGUIDlg::initData()
{
	if (!UpdatePackage::getInstance()->getVersion())
	{
		AfxMessageBox(_T("��ȡ�汾ʧ��"));
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
		AfxMessageBox(_T("����ɹ�"));
	}
	else{
		CString str;
		str.Format(_T("���ʧ�� ԭ��%s", reason.c_str()));
		AfxMessageBox(str);
	}

}


void CSvnDiffGUIDlg::OnBnClickedBtnSelectSavePath()
{
	wchar_t szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("��ѡ����Ҫ�����Ŀ¼��");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		m_savepath.SetWindowText(szPath);
	}
	else
		AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��"));
}
