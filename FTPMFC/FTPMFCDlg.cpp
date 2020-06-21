// FTPMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPMFC.h"
#include "FTPMFCDlg.h"
#include "afxdialogex.h"
char szLocDirectory[MAX_PATH]; //本地当前路径。
char szAppDir[MAX_PATH];        //该程序所在路径。
CString szSrvDir;   //服务器路径。
int ReNameIndex;
CString strName;
#ifndef __AFXINET_H__
#include <afxinet.h>
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTPMFCDlg 对话框



CFTPMFCDlg::CFTPMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTPMFC_DIALOG, pParent)
	//, m_User(_T(""))
	//, m_Pwd(_T(""))
	//, m_Message(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_strRemoteParentRoot = _T("");
	//m_strRemoteCurrentRoot = _T("");
	//czUploadFileName = _T("");
	//czUploadFileFullName = _T("");
	//czDeleteServerFileName = _T("");
	//czDeleteServerFileFullName = _T("");
	//czDownloadServerFileName = _T("");
	//czDownloadServerFileFullName = _T("");
	//LocalDir = _T("");
	//m_iDirCount = 0;

	m_User = _T("");
	m_Pwd = _T("");
	m_Message = _T("");
	m_bConnected = false;
	m_pFtpConnection = NULL;

	::GetCurrentDirectory(-1, szAppDir);
}

void CFTPMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BACK, m_ctBack);
	DDX_Control(pDX, ID_BTN_CONNECT, m_BtnConnect);//连接服务器按钮
	DDX_Control(pDX, IDC_COMBOBOXEX2, m_ctrlLocal);//服务器返回上一页按钮
	DDX_Control(pDX, IDC_COMBOBOXEX3, m_ctrlSrv);//m_ctrlSrv服务器路径
	DDX_Control(pDX, IDC_IPADDRESS1, m_Ip);//ip
	DDX_Control(pDX, IDC_LIST1, m_ctrlLocalFile);//m_ctrlLocalFile客户端文件列表
	DDX_Control(pDX, IDC_LIST2, m_ctrlServerFile);//m_ctrlServerFile服务器文件列表
	DDX_Text(pDX, IDC_EDIT1, m_User);//用户名
	DDX_Text(pDX, IDC_EDIT2, m_Pwd);//密码
	DDX_Text(pDX, IDC_EDIT3, m_Message);//响应信息
}

BEGIN_MESSAGE_MAP(CFTPMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BACK, &CFTPMFCDlg::OnBack)
	ON_BN_CLICKED(ID_BTN_CONNECT, &CFTPMFCDlg::OnBtnConnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFTPMFCDlg::OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CFTPMFCDlg::OnDblclkList2)
	ON_COMMAND(ID_DELETE_SRV_FILE, &CFTPMFCDlg::OnDeleteSrvFile)
	ON_COMMAND(ID_QUICK_WEBTOLOCAL, &CFTPMFCDlg::OnQuickWebtolocal)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFTPMFCDlg::OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CFTPMFCDlg::OnRclickList2)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX2, &CFTPMFCDlg::OnSelchangeComboboxex2)
	ON_UPDATE_COMMAND_UI(ID_QUICK_WEBTOLOCAL, &CFTPMFCDlg::OnUpdateQuickWebtolocal)
	ON_COMMAND(ID_UPLOADFILE, &CFTPMFCDlg::OnUploadFile)
	ON_UPDATE_COMMAND_UI(ID_UPLOADFILE, &CFTPMFCDlg::OnUpdateUploadfile)
	ON_MESSAGE(WM_SETREDRAWFLAG, &CFTPMFCDlg::OnSetLocRedrawFlag)
	ON_MESSAGE(WM_SETFILE, &CFTPMFCDlg::OnInsertLocFile)
	ON_COMMAND(ID_BUTTON_EXIT,&CFTPMFCDlg::OnbtnClose)
	ON_COMMAND(ID_CTL_BREAK, &CFTPMFCDlg::OnCtlBreak)
	ON_UPDATE_COMMAND_UI(ID_CTL_BREAK, &CFTPMFCDlg::OnUpdateCtlBreak)
	ON_UPDATE_COMMAND_UI(ID_BTN_CONNECT, &CFTPMFCDlg::OnUpdateBtnConnect)
	ON_COMMAND(ID_QUICK_WEBTOLOCAL, &CFTPMFCDlg::OnQuickWebtolocal)

	ON_BN_CLICKED(ID_CTL_BREAK, &CFTPMFCDlg::OnBnClickedCtlBreak)
	ON_COMMAND(ID_ReName, &CFTPMFCDlg::OnRenName)
END_MESSAGE_MAP()


// CFTPMFCDlg 消息处理程序

BOOL CFTPMFCDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	m_Menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&m_Menu);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BACKBITMAP));//位图资源ID为IDB_BACKBITMAP
	m_ctBack.SetBitmap(hBitmap);//m_ctBack后退按钮，设置位图

	//服务器文件图标
	m_ServerImageList.Detach();
	m_ServerImageList.Create(IDB_FILE_BITMAP, 16, 3, RGB(0, 255, 0));
	m_ctrlServerFile.SetImageList(&m_ServerImageList, LVSIL_SMALL);

	////////////////////////////////////////////////////
	m_Ip.SetAddress(127, 0, 0, 1);//设置默认连接ip
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText("anonymous");//设置默认用户名anonymous
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pEdit->SetWindowText("");//默认密码为空

	//自动调节窗口大小
	//GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	///////////////////////////
	//获取本地驱动器信息 
	HIMAGELIST hImageList;
	//定义文件信息变量
	SHFILEINFO shFi;
	//获取文件图标信息
	hImageList = (HIMAGELIST)SHGetFileInfo("C:\\", 0, &shFi, sizeof(shFi),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	//将文件图象列表存入m_ctImageList
	m_ctImageList.m_hImageList = hImageList;

	SetFileColumns(&m_ctrlLocalFile);//设定客户端文件显示列表框
	SetServerFileColumns(&m_ctrlServerFile);//设定服务器端文件显示列表框

	SetImageList(&m_ctrlLocalFile, &m_ctrlLocal);//设定客户端文件图标
	//获得客户端磁盘路径
	GetLocDir();

	//如果客户端路径列表框含有为c盘则返回0
	int i = m_ctrlLocal.FindStringExact(-1, "C:\\");

	m_ctrlLocal.SetCurSel(i);//列表框默认显示C盘
	SetCurrentDirectory("C:\\");
	//获得客户端路径列表框的路径
	GetLocCurrentDirectory();
	//查找客户端文件
	BeginFindLocFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFTPMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFTPMFCDlg::OnPaint()
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
HCURSOR CFTPMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
// 本地文件列表框信息
void CFTPMFCDlg::SetFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[] = {
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("字节数"),LVCFMT_CENTER
	};
	for (int i = 0, j = 100; i<sizeof(columns) / sizeof(columns[0]); i++, j -= 10)
		pListCtrl->InsertColumn(i, columns[i].pszText, columns[i].uiFormat, j);
}

// 服务器文件列表框信息
void CFTPMFCDlg::SetServerFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[] = {
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("字节数"),LVCFMT_CENTER
	};
	for (int i = 0, j = 100; i<sizeof(columns) / sizeof(columns[0]); i++, j -= 10)
		pListCtrl->InsertColumn(i, columns[i].pszText, columns[i].uiFormat, j);
}

//获取图标
BOOL CFTPMFCDlg::SetImageList(CListCtrl* pListCtrl, CComboBoxEx* pCombo)
{
	pListCtrl->SetImageList(&m_ctImageList, LVSIL_SMALL);
	if (!pCombo)
		return TRUE;
	pCombo->SetImageList(&m_ctImageList);
	return TRUE;
}

// 获得客户端磁盘路径
void CFTPMFCDlg::GetLocDir()
{
	int iIcon, iIconSel;
	char lpBuffer[MAX_PATH];
	char* ptr;
	CString str;
	m_iDirCount = 0;
	::GetLogicalDriveStrings(MAX_PATH, lpBuffer);
	ptr = lpBuffer;
	while (*ptr)
	{
		m_iDirCount++;
		str = ptr;
		GetFileIcon(str, &iIcon, &iIconSel);
		InsertLocDir(str, iIcon, iIconSel);
		ptr += strlen(ptr) + 1;
	}
}

// 获得文件图标
void CFTPMFCDlg::GetFileIcon(CString& fileName, int* iIcon, int* iIconSel)
{
	CString str = fileName;
	if (str.Right(1) != "\\")
		str += "\\";
	SHFILEINFO shFi;
	if (!SHGetFileInfo(str, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_DISPLAYNAME))
	{
		AfxMessageBox("111");
		return;
	}
	*iIcon = shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if (iIconSel)
	{
		if (!SHGetFileInfo(str, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OPENICON | SHGFI_DISPLAYNAME))
		{
			AfxMessageBox("222");
			return;
		}
		*iIconSel = shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
	return;
}

int CFTPMFCDlg::InsertLocDir(CString& str, int iIcon, int iIconSel)
{
	int nItem = m_ctrlLocal.GetCount();
	COMBOBOXEXITEM Combo;//COMBOBOXEXITEM结构体
	Combo.mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
	Combo.iImage = iIcon;//未选定状态下，图像列表中的图像索引
	Combo.iSelectedImage = iIconSel;//选定状态下，图像列表中的图像索引
	Combo.pszText = (LPTSTR)(LPCTSTR)str;
	Combo.iItem = nItem;
	m_ctrlLocal.InsertItem(&Combo);
	return nItem;
}

BOOL CFTPMFCDlg::BeginFindLocFile()//开始查找客户端文件
{
	m_ctrlLocalFile.DeleteAllItems();

	FindLocFile();
	return TRUE;
}

void CFTPMFCDlg::FindLocFile()//查找客户端文件
{
	FILE_FTP_INFO* pInfo = new FILE_FTP_INFO;
	CFileFind fileFind;
	UINT i = 0;
	CTime time;
	BOOL bContinue;
	SetCurrentDirectory(szLocDirectory);
	::PostMessage(this->m_hWnd, WM_SETDIRECTORYDISPLAY, 0, 0);
	bContinue = fileFind.FindFile("*.*");
	CString str;
	while (bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if (fileFind.IsHidden())continue;
		str = fileFind.GetFileName();
		strcpy(pInfo->szFileName, str);
		fileFind.GetLastWriteTime(time);
		str = time.Format("%x");
		strcpy(pInfo->szFileDate, str);
		if (fileFind.IsDirectory())
		{
			strcpy(pInfo->szFileSize, "");
			pInfo->nType = DIRECTORYICON;
			::SendMessage(this->m_hWnd, WM_SETFILE, (WPARAM)pInfo,
				(LPARAM)LOCFILE);
		}
		else
		{
			int i = fileFind.GetLength();
			if (i>1024)
			{
				str.Format("%d", i / 1024);
				str += "KB";
			}
			else
				str.Format("%d", i);
			strcpy(pInfo->szFileSize, str);
			pInfo->nType = FILEICON;
			::SendMessage(this->m_hWnd, WM_SETFILE, (WPARAM)pInfo,
				(LPARAM)LOCFILE);
		}
	}
	::SendMessage(this->m_hWnd, WM_SETREDRAWFLAG, 0, 0);
	delete pInfo;
	fileFind.Close();
}

LRESULT CFTPMFCDlg::OnSetLocRedrawFlag(WPARAM wParam, LPARAM lParam)
{
	if (m_bHaveDotFlag)//
	{
		CString str = "..";
		int iIcon = DIRECTORYICON;
		m_ctrlLocalFile.InsertItem(0, str, iIcon);//插入..
		m_ctrlLocalFile.SetItemData(0, DIRECTORYICON);//设置日期
		m_bHaveDotFlag = FALSE;
	}
	m_ctrlLocalFile.Invalidate();
	m_ctrlLocalFile.SetRedraw();
	return 0L;
}

LRESULT CFTPMFCDlg::OnSetLocDirectoryDisplay(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str = szLocDirectory;
	m_ctrlLocal.SetWindowText(str);
	m_ctrlLocal.Invalidate();
	return 0L;
}

LRESULT CFTPMFCDlg::OnInsertLocFile(WPARAM wParam, LPARAM lParam)
{
	FILE_FTP_INFO* pInfo = (FILE_FTP_INFO*)wParam;
	CString str = pInfo->szFileName;
	if (str == "." || str == "..")
	{
		m_bHaveDotFlag = TRUE;
		return 0L;
	}
	OnInsertFile(&m_ctrlLocalFile, pInfo, lParam);
	return 0L;
}

// 在客户端文件列表插入查找到的文件名
void CFTPMFCDlg::OnInsertFile(CListCtrl* pListCtrl, LPVOID pIn, LPARAM lParam)
{
	FILE_FTP_INFO*pInfo = (FILE_FTP_INFO*)pIn;//FILE_FTP_INFO结构体，包含文件名、日期、大小、类型
	LVITEM	lvIt;//LVITEM结构体
	int iIcon, nItem;
	if (pInfo->nType == DIRECTORYICON)
		nItem = 0;//如果类型是文件夹，nItem=0
	else
		nItem = pListCtrl->GetItemCount();//否则，返回行数（文件列表）
	CString fileName = pInfo->szFileName;;//文件名
	if ((int)lParam == LOCFILE)//如果是客户端文件，获取文件名和图标
		GetFileIcon(fileName, &iIcon);
	else
		iIcon = pInfo->nType;//否则，返回类型

	lvIt.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;//说明结构中哪些成员有效
	lvIt.iImage = iIcon;//关联图像列表中指定图像的索引值
	lvIt.lParam = pInfo->nType;//程序定义的32位参数
	lvIt.pszText = pInfo->szFileName;//主项或子项的名称
	lvIt.iSubItem = 0;//子项的索引值(可以视为行号)从0开始
	lvIt.iItem = nItem;//项目的索引值(可以视为行号)从0开始
	int iPos = pListCtrl->InsertItem(&lvIt);//将ivIt加入列表
	lvIt.mask = LVIF_TEXT;
	lvIt.iItem = iPos;
	lvIt.pszText = pInfo->szFileDate;//设置日期
	lvIt.iSubItem = 1;
	pListCtrl->SetItem(&lvIt);


	lvIt.pszText = pInfo->szFileSize;//设置文件大小
	lvIt.iSubItem = 2;
	pListCtrl->SetItem(&lvIt);
}

// 获得客户端当前路径
void CFTPMFCDlg::GetLocCurrentDirectory()
{
	CString str;
	m_ctrlLocal.GetWindowText(str);
	strcpy(szLocDirectory, str);
}

//连接服务器
void CFTPMFCDlg::OnBtnConnect()
{
	CString IP;
	BYTE f1, f2, f3, f4;
	TCHAR temp[10];
	UpdateData(TRUE);
	m_Ip.GetAddress(f1, f2, f3, f4);
	IP = itoa(f1, temp, 10);
	IP += ".";
	IP += itoa(f2, temp, 10);
	IP += ".";
	IP += itoa(f3, temp, 10);
	IP += ".";
	IP += itoa(f4, temp, 10);
	if (IsValid() == false)
		return;

	m_Message = m_Message + "连接服务器:" + IP + "\r\n";
	m_Message = m_Message + "登录用户名:" + m_User + "\r\n";


	//新建对话
	m_pInetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try
	{
		//新建连接对象
		m_pFtpConnection = m_pInetSession->GetFtpConnection(IP, m_User,
			m_Pwd);

	}
	catch (CInternetException *pEx)
	{
		//获取错误
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox("There was an exception");
		pEx->Delete();
		m_pFtpConnection = NULL;

		return;
	}
	m_Message = m_Message + "登陆成功!" + "\r\n";
	UpdateData(false);
	m_pRemoteFinder = new CFtpFileFind(m_pFtpConnection);
	BrowseDir("", &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	m_BtnConnect.EnableWindow(m_pFtpConnection == NULL);
	m_ctrlSrv.SetWindowText("\/");
}

void CFTPMFCDlg::BrowseDir(CString strDir, CListCtrl* pLstCtrl, CFileFind* pFinder, CArray<FILEITEM, FILEITEM&>* pFilesArr)//文件浏览器
{
	//清空列表框
	pLstCtrl->DeleteAllItems();
	pFilesArr->RemoveAll();

	int nIndex = 0;

	CString szDir = strDir;
	if (szDir.Right(1) != "\/")
		szDir += "\/";
	szDir += "*.*";
	CTime time; 
	CString str;
	//查找目录下的文件
	BOOL res = pFinder->FindFile(szDir);
	while (res)
	{
		res = pFinder->FindNextFile();

		//是目录
		if (pFinder->IsDirectory() && !pFinder->IsDots())
		{
			FILEITEM fileitem;
			fileitem.nItem = nIndex;
			fileitem.bDir = TRUE;
			fileitem.strFileName = pFinder->GetFileName();
			pFilesArr->Add(fileitem);
			AddItem(pLstCtrl, nIndex, TRUE, pFinder->GetFileName());
			pLstCtrl->SetItemText(nIndex, 2, "文件夹");
		}
		//是文件
		else if (!pFinder->IsDirectory() && !pFinder->IsDots())
		{
			FILEITEM fileitem;
			fileitem.nItem = nIndex;
			fileitem.bDir = FALSE;
			fileitem.strFileName = pFinder->GetFileName();
			pFilesArr->Add(fileitem);

			AddItem(pLstCtrl, nIndex, FALSE, pFinder->GetFileName());
			DWORD len = pFinder->GetLength();
			str.Format("%dKB", len / 1024);
			pLstCtrl->SetItemText(nIndex, 2, str);
		}
		pFinder->GetLastWriteTime(time);
		str = time.Format("%x");
		pLstCtrl->SetItemText(nIndex, 1, str);
		nIndex++;
	}
}

void CFTPMFCDlg::AddItem(CListCtrl* pLstCtrl, int nIndex, bool bDir, CString strText)//判断是否文件或文件夹
{
	LV_ITEM lvitem;
	int iActualItem;
	for (int iSubItem = 0; iSubItem<2; iSubItem++)//第一次循环是当前列表，第二次是子列表
	{
		lvitem.mask = LVIF_TEXT | (iSubItem == 0 ? LVIF_IMAGE : 0) | LVIF_PARAM;//isubitem = 0时，表达式为LVIF_IMAGE,否则为0;mask说明成员中哪些成员有效
																				//lvitem.mask=LVIF_TEXT|;
		lvitem.iItem = (iSubItem == 0) ? nIndex : iActualItem;//isubitem = 0时，表达式为nIndex,否则为iActualItem；iItem为项目的索引值
		lvitem.iSubItem = iSubItem;
		switch (iSubItem)
		{
		case 0:
			if (bDir)//如果是文件夹
			{
				lvitem.iImage = 1;
				lvitem.pszText = (LPTSTR)(LPCTSTR)strText;

			}
			else
			{
				lvitem.iImage = 0;
				lvitem.pszText = (LPTSTR)(LPCTSTR)strText;

			}
			break;
		case 1:
			if (bDir)
			{

				lvitem.pszText = "文件夹";
			}
			else
			{

				lvitem.pszText = "文件";
			}
			break;
		}
		if (iSubItem == 0)
			iActualItem = pLstCtrl->InsertItem(&lvitem);
		else
			pLstCtrl->SetItem(&lvitem);
	}
}

BOOL CFTPMFCDlg::IsValid()
{
	UpdateData(TRUE);
	BYTE f1, f2, f3, f4;
	if (m_Ip.IsBlank())
	{
		MessageBox(_T("请输入FTP服务器的地址."),
			_T("错误"),
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//不合理的IP
	if (m_Ip.GetAddress(f1, f2, f3, f4) < 4)
	{
		MessageBox(_T("输入的FTP服务器地址非法."),
			_T("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (m_User == "")
	{
		MessageBox(_T("用户名不能为空"), "错误", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

//上传
void CFTPMFCDlg::OnUploadFile()
{
	if (m_pFtpConnection == NULL)
		return;
	CString str, str1, filename, buf;
	int index = m_ctrlLocalFile.GetNextItem(-1, LVNI_SELECTED);
	if (index<0)
		return;
	str = m_ctrlLocalFile.GetItemText(index, 0);
	str1 = szLocDirectory;
	czUploadFileName = str;
	if (str1.Right(1) != "\\")
	{
		str1 = str1 + "\\" + str;
	}
	else
		str1 = szLocDirectory + str;
	m_Message = m_Message + "开始上传文件" + str1 + "\r\n";
	UpdateData(false);
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	pEdit->LineScroll(pEdit->GetLineCount(), 0);


	////////////////////////////////////////////如果上传的是单个文件
	CFileFind ff;
	ff.FindFile(str1);
	ff.FindNextFile();
	if (!ff.IsDirectory() && !ff.IsDots())
	{
		czUploadFileFullName = str1;
		CWinThread *pThread = AfxBeginThread(UpLoad, this);
		return;
	}
	else////////////////////////////////////////////如果上传的是文件夹
	{
		AfxMessageBox("本程序暂时不支持上传文件夹!");
		return;
	}
}

void CFTPMFCDlg::StartSendFile(CString FileName)
{
	CFileFind ff, ff1;
	CString szDir = FileName;
	if (szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";
	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//如果是一个子目录，用递归继续往深一层找
			StartSendFile(ff.GetFilePath());
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件
			CString str;
			czUploadFileFullName = ff.GetFilePath();
			//::CRITICAL_SECTION
			CWinThread *pThread = AfxBeginThread(UpLoad, this);
			Sleep(30);
		}
		//Sleep(3000);
	}
	ff.Close();//关闭
}

//上传
UINT CFTPMFCDlg::UpLoad(LPVOID para)
{
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;
	CString str;
	pThis->m_ctrlSrv.GetWindowText(str);
	pThis->m_pFtpConnection->PutFile(pThis->czUploadFileFullName, str+"\/"+ pThis->czUploadFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czUploadFileName + "上传完成\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	pThis->BrowseDir(str + "\/", &(pThis->m_ctrlServerFile), pThis->m_pRemoteFinder, &(pThis->m_arrRemoteFiles));
	return 0L;
}

//删除服务端文件
void CFTPMFCDlg::OnDeleteSrvFile()
{
	int index = m_ctrlServerFile.GetNextItem(-1, LVNI_SELECTED);
	CString str = m_pRemoteFinder->GetRoot() + "\/" + m_ctrlServerFile.GetItemText(index, 0);
	czDeleteServerFileName = m_ctrlServerFile.GetItemText(index, 0);
	m_ctrlServerFile.DeleteItem(index);
	czDeleteServerFileFullName = str;

	CWinThread *pThread = AfxBeginThread(DelServerFile, this);

	BrowseDir(m_pRemoteFinder->GetRoot(), &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
}

//删除服务端文件
UINT CFTPMFCDlg::DelServerFile(LPVOID para)
{
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;
	CString szServerFileName = pThis->czDeleteServerFileFullName;

	pThis->m_pFtpConnection->Remove(szServerFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czDeleteServerFileName + "删除完成\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);

	return 0L;
}

//下载
void CFTPMFCDlg::OnQuickWebtolocal()
{
	int index = m_ctrlServerFile.GetNextItem(-1, LVNI_SELECTED);//查找服务端文件的下一项，成功返回下一项的索引，否则返回-1
	CString str = m_strRemoteCurrentRoot + "\/" + m_ctrlServerFile.GetItemText(index, 0);//文件路径
	czDownloadServerFileName = m_ctrlServerFile.GetItemText(index, 0);//文件名
	czDownloadServerFileFullName = str;//文件路径
	if (index<0)
		return;
	CWinThread *pThread = AfxBeginThread(DownloadServerFile, this);//下载文件
																   //////////////////////////////////////////////////
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);//当前客户端路径
	str = szLocDirectory;

	CString buf = szLocDirectory;
	if (str.Right(1) != "\/")
		str += "\/";
	str += szLocDirectory;//文件路径
	int iIcon, iIconSel;
	GetFileIcon(buf, &iIcon, &iIconSel);//获得文件图标
	int i = InsertLocDir(str, iIcon, iIconSel);//插入文件信息
	m_ctrlLocal.SetCurSel(i);//选中当前选项
	::SetCurrentDirectory(str);
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);
	BeginFindLocFile();	//遍历一遍本地文件，并刷新
}

UINT CFTPMFCDlg::DownloadServerFile(LPVOID para)//从服务端下载文件
{
	CString strLocalFile, strRemoteFile;
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;

	CString szServerFileName = pThis->czDownloadServerFileName;
	CString szServerFileFullName = pThis->czDownloadServerFileFullName;
	CString szLocalDir = szLocDirectory;
	szLocalDir = szLocalDir + "\/";
	pThis->m_pFtpConnection->GetFile(szServerFileFullName, szLocalDir + szServerFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czDownloadServerFileName + "下载完成\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	return 0L;
}

//服务端右键
void CFTPMFCDlg::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (m_pFtpConnection == NULL)return;
	SetPopSrvMenu(nItem);

	*pResult = 0;
}

//设置服务端弹出框
void CFTPMFCDlg::SetPopSrvMenu(int nItem)
{
	CMenu dMenu;
	if (nItem == -1)
	{
		if (!dMenu.LoadMenu(IDR_CREAT_SVR_DIR))
			AfxThrowResourceException();
	}
	else
	{
		if (!dMenu.LoadMenu(IDR_SRV_MENU))
			AfxThrowResourceException();
	}
	CMenu* pPopMenu = dMenu.GetSubMenu(0);
	ASSERT(pPopMenu != NULL);
	POINT pt;
	::GetCursorPos(&pt);
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, AfxGetMainWnd());
}

//客户端双击文件夹
void CFTPMFCDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO* pLocInfo = (NMLVDISPINFO*)pNMHDR;
	CString str, str1, filename, buf;
	int iIcon, iIconSel;
	str.Format("%d", pLocInfo->item);
	if (str == "-1")
		return;
	int nItem = atoi(str);

	DWORD dwFlag = m_ctrlLocalFile.GetItemData(nItem);
	if (dwFlag == FILEICON)
	{
		//OnLocFileOpen();
		return;
	}
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);
	str = szLocDirectory;
	m_ctrlLocalFile.GetItemText(nItem, 0, szLocDirectory, MAX_PATH);
	buf = szLocDirectory;
	if (str.Right(1) != "\\")
		str += "\\";
	str += szLocDirectory;

	GetFileIcon(buf, &iIcon, &iIconSel);
	int i = InsertLocDir(str, iIcon, iIconSel);
	m_ctrlLocal.SetCurSel(i);
	::SetCurrentDirectory(str);
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);
	BeginFindLocFile();

	*pResult = 0;
}

//设置客户端列表框
void CFTPMFCDlg::OnSelchangeComboboxex2()
{
	CString str;
	char lpBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, lpBuffer);
	int i = m_ctrlLocal.GetCurSel();//获取当前光标的位置
	m_ctrlLocal.GetLBText(i, (LPTSTR)(LPCTSTR)str);//把索引号为i的数据放到str变量中
	i = 0;
	while (1)
	{
		if (!SetCurrentDirectory(str))//将当前工作目录切换到str，如果切换不了，则执行下面
		{
			i++;
			if (i<2)//此时i=1
			{
				AfxMessageBox("请插入磁盘再试");
				continue;
			}
			str = lpBuffer;
			if ((i = m_ctrlLocal.FindStringExact(-1, str)) != -1)
				m_ctrlLocal.SetCurSel(i);
			else
				m_ctrlLocal.SetWindowText(str);
		}
		else
		{
			::SetCurrentDirectory(str);
			GetLocCurrentDirectory();
			BeginFindLocFile();
		}
		break;

	}
}

//客户端列表地右键
void CFTPMFCDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (nItem == -1)return;
	SetPopLocalMenu();
	*pResult = 0;
}

//设置客户端弹出框
void CFTPMFCDlg::SetPopLocalMenu()
{
	CMenu dMenu;
	if (!dMenu.LoadMenu(IDR_LOC_MENU))
		AfxThrowResourceException();
	CMenu* pPopMenu = dMenu.GetSubMenu(0);
	ASSERT(pPopMenu != NULL);
	POINT pt;
	::GetCursorPos(&pt);
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, AfxGetMainWnd());
}

// 断开服务器
void CFTPMFCDlg::OnCtlBreak()
{
	if (m_pFtpConnection == NULL)
		return;
	m_pInetSession->Close();
	m_pFtpConnection = NULL;
	m_Message = m_Message + "已断开服务器!\r\n";
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	UpdateData(false);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	m_BtnConnect.EnableWindow(m_pFtpConnection == NULL);
	m_ctrlSrv.ResetContent();
	m_ctrlServerFile.DeleteAllItems();
	
}

//使断开按钮变灰
void CFTPMFCDlg::OnUpdateCtlBreak(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//使连接按钮变灰
void CFTPMFCDlg::OnUpdateBtnConnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection == NULL);
}

//使下载按钮变灰
void CFTPMFCDlg::OnUpdateQuickWebtolocal(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//使上传按钮变灰
void CFTPMFCDlg::OnUpdateUploadfile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//服务器端双击
void CFTPMFCDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_ctrlServerFile.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = m_ctrlServerFile.GetNextSelectedItem(pos);
		FILEITEM fileItem = m_arrRemoteFiles.GetAt(nItem);
		if (fileItem.bDir == TRUE)
		{
			CString strDir;
			strDir.Format("%s\/%s", m_pRemoteFinder->GetRoot(), fileItem.strFileName);
			m_strRemoteParentRoot = m_pRemoteFinder->GetRoot();
			BrowseDir(strDir, &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
			m_strRemoteCurrentRoot = strDir;
			m_ctrlSrv.SetWindowText(strDir);
		}
	}

	*pResult = 0;
}

void CFTPMFCDlg::OnBack()//服务端返回按钮
{
	if (m_pFtpConnection == NULL)
		return;
	if (m_strRemoteCurrentRoot == "\/")
		return;
	int t = m_strRemoteCurrentRoot.ReverseFind('\/');
	m_strRemoteCurrentRoot = m_strRemoteCurrentRoot.Left(t);
	BrowseDir(m_strRemoteCurrentRoot, &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);

	m_ctrlSrv.SetWindowText(m_strRemoteCurrentRoot);
}

void CFTPMFCDlg::OnbtnClose()
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void CFTPMFCDlg::OnBnClickedCtlBreak()
{
	// TODO: 在此添加控件通知处理程序代码
}



//服务器重命名
void CFTPMFCDlg::OnRenName()
{
	// TODO: 在此添加命令处理程序代码
	ReNameIndex = m_ctrlServerFile.GetNextItem(-1, LVNI_SELECTED);
	strName = m_strRemoteCurrentRoot + "\/" + m_ctrlServerFile.GetItemText(ReNameIndex, 0);
	rename.m_type = m_ctrlServerFile.GetItemText(ReNameIndex, 0);
	rename.DoModal();

	if (rename.flag == true)
	{
		czReName = m_strRemoteCurrentRoot + "\/" + rename.m_ReName;
		if (m_pFtpConnection->Rename(strName, czReName))
		{
			m_Message = m_Message + rename.m_ReName + "修改完成\r\n";
			CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
			pEdit->SetWindowText(m_Message);
			pEdit->LineScroll(pEdit->GetLineCount(), 0);
			BrowseDir(m_strRemoteCurrentRoot, &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
		}
		else
		{
			
			MessageBox("命名失败");
		}

	}
}
