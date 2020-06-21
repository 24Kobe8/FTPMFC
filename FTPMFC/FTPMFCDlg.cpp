// FTPMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTPMFC.h"
#include "FTPMFCDlg.h"
#include "afxdialogex.h"
char szLocDirectory[MAX_PATH]; //���ص�ǰ·����
char szAppDir[MAX_PATH];        //�ó�������·����
CString szSrvDir;   //������·����
int ReNameIndex;
CString strName;
#ifndef __AFXINET_H__
#include <afxinet.h>
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFTPMFCDlg �Ի���



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
	DDX_Control(pDX, ID_BTN_CONNECT, m_BtnConnect);//���ӷ�������ť
	DDX_Control(pDX, IDC_COMBOBOXEX2, m_ctrlLocal);//������������һҳ��ť
	DDX_Control(pDX, IDC_COMBOBOXEX3, m_ctrlSrv);//m_ctrlSrv������·��
	DDX_Control(pDX, IDC_IPADDRESS1, m_Ip);//ip
	DDX_Control(pDX, IDC_LIST1, m_ctrlLocalFile);//m_ctrlLocalFile�ͻ����ļ��б�
	DDX_Control(pDX, IDC_LIST2, m_ctrlServerFile);//m_ctrlServerFile�������ļ��б�
	DDX_Text(pDX, IDC_EDIT1, m_User);//�û���
	DDX_Text(pDX, IDC_EDIT2, m_Pwd);//����
	DDX_Text(pDX, IDC_EDIT3, m_Message);//��Ӧ��Ϣ
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


// CFTPMFCDlg ��Ϣ�������

BOOL CFTPMFCDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&m_Menu);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BACKBITMAP));//λͼ��ԴIDΪIDB_BACKBITMAP
	m_ctBack.SetBitmap(hBitmap);//m_ctBack���˰�ť������λͼ

	//�������ļ�ͼ��
	m_ServerImageList.Detach();
	m_ServerImageList.Create(IDB_FILE_BITMAP, 16, 3, RGB(0, 255, 0));
	m_ctrlServerFile.SetImageList(&m_ServerImageList, LVSIL_SMALL);

	////////////////////////////////////////////////////
	m_Ip.SetAddress(127, 0, 0, 1);//����Ĭ������ip
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText("anonymous");//����Ĭ���û���anonymous
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pEdit->SetWindowText("");//Ĭ������Ϊ��

	//�Զ����ڴ��ڴ�С
	//GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	///////////////////////////
	//��ȡ������������Ϣ 
	HIMAGELIST hImageList;
	//�����ļ���Ϣ����
	SHFILEINFO shFi;
	//��ȡ�ļ�ͼ����Ϣ
	hImageList = (HIMAGELIST)SHGetFileInfo("C:\\", 0, &shFi, sizeof(shFi),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	//���ļ�ͼ���б����m_ctImageList
	m_ctImageList.m_hImageList = hImageList;

	SetFileColumns(&m_ctrlLocalFile);//�趨�ͻ����ļ���ʾ�б��
	SetServerFileColumns(&m_ctrlServerFile);//�趨���������ļ���ʾ�б��

	SetImageList(&m_ctrlLocalFile, &m_ctrlLocal);//�趨�ͻ����ļ�ͼ��
	//��ÿͻ��˴���·��
	GetLocDir();

	//����ͻ���·���б����Ϊc���򷵻�0
	int i = m_ctrlLocal.FindStringExact(-1, "C:\\");

	m_ctrlLocal.SetCurSel(i);//�б��Ĭ����ʾC��
	SetCurrentDirectory("C:\\");
	//��ÿͻ���·���б���·��
	GetLocCurrentDirectory();
	//���ҿͻ����ļ�
	BeginFindLocFile();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFTPMFCDlg::OnPaint()
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
HCURSOR CFTPMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
// �����ļ��б����Ϣ
void CFTPMFCDlg::SetFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[] = {
		_T("�ļ���"),LVCFMT_LEFT,
		_T("����"),LVCFMT_CENTER,
		_T("�ֽ���"),LVCFMT_CENTER
	};
	for (int i = 0, j = 100; i<sizeof(columns) / sizeof(columns[0]); i++, j -= 10)
		pListCtrl->InsertColumn(i, columns[i].pszText, columns[i].uiFormat, j);
}

// �������ļ��б����Ϣ
void CFTPMFCDlg::SetServerFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[] = {
		_T("�ļ���"),LVCFMT_LEFT,
		_T("����"),LVCFMT_CENTER,
		_T("�ֽ���"),LVCFMT_CENTER
	};
	for (int i = 0, j = 100; i<sizeof(columns) / sizeof(columns[0]); i++, j -= 10)
		pListCtrl->InsertColumn(i, columns[i].pszText, columns[i].uiFormat, j);
}

//��ȡͼ��
BOOL CFTPMFCDlg::SetImageList(CListCtrl* pListCtrl, CComboBoxEx* pCombo)
{
	pListCtrl->SetImageList(&m_ctImageList, LVSIL_SMALL);
	if (!pCombo)
		return TRUE;
	pCombo->SetImageList(&m_ctImageList);
	return TRUE;
}

// ��ÿͻ��˴���·��
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

// ����ļ�ͼ��
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
	COMBOBOXEXITEM Combo;//COMBOBOXEXITEM�ṹ��
	Combo.mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
	Combo.iImage = iIcon;//δѡ��״̬�£�ͼ���б��е�ͼ������
	Combo.iSelectedImage = iIconSel;//ѡ��״̬�£�ͼ���б��е�ͼ������
	Combo.pszText = (LPTSTR)(LPCTSTR)str;
	Combo.iItem = nItem;
	m_ctrlLocal.InsertItem(&Combo);
	return nItem;
}

BOOL CFTPMFCDlg::BeginFindLocFile()//��ʼ���ҿͻ����ļ�
{
	m_ctrlLocalFile.DeleteAllItems();

	FindLocFile();
	return TRUE;
}

void CFTPMFCDlg::FindLocFile()//���ҿͻ����ļ�
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
		m_ctrlLocalFile.InsertItem(0, str, iIcon);//����..
		m_ctrlLocalFile.SetItemData(0, DIRECTORYICON);//��������
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

// �ڿͻ����ļ��б������ҵ����ļ���
void CFTPMFCDlg::OnInsertFile(CListCtrl* pListCtrl, LPVOID pIn, LPARAM lParam)
{
	FILE_FTP_INFO*pInfo = (FILE_FTP_INFO*)pIn;//FILE_FTP_INFO�ṹ�壬�����ļ��������ڡ���С������
	LVITEM	lvIt;//LVITEM�ṹ��
	int iIcon, nItem;
	if (pInfo->nType == DIRECTORYICON)
		nItem = 0;//����������ļ��У�nItem=0
	else
		nItem = pListCtrl->GetItemCount();//���򣬷����������ļ��б�
	CString fileName = pInfo->szFileName;;//�ļ���
	if ((int)lParam == LOCFILE)//����ǿͻ����ļ�����ȡ�ļ�����ͼ��
		GetFileIcon(fileName, &iIcon);
	else
		iIcon = pInfo->nType;//���򣬷�������

	lvIt.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;//˵���ṹ����Щ��Ա��Ч
	lvIt.iImage = iIcon;//����ͼ���б���ָ��ͼ�������ֵ
	lvIt.lParam = pInfo->nType;//�������32λ����
	lvIt.pszText = pInfo->szFileName;//��������������
	lvIt.iSubItem = 0;//���������ֵ(������Ϊ�к�)��0��ʼ
	lvIt.iItem = nItem;//��Ŀ������ֵ(������Ϊ�к�)��0��ʼ
	int iPos = pListCtrl->InsertItem(&lvIt);//��ivIt�����б�
	lvIt.mask = LVIF_TEXT;
	lvIt.iItem = iPos;
	lvIt.pszText = pInfo->szFileDate;//��������
	lvIt.iSubItem = 1;
	pListCtrl->SetItem(&lvIt);


	lvIt.pszText = pInfo->szFileSize;//�����ļ���С
	lvIt.iSubItem = 2;
	pListCtrl->SetItem(&lvIt);
}

// ��ÿͻ��˵�ǰ·��
void CFTPMFCDlg::GetLocCurrentDirectory()
{
	CString str;
	m_ctrlLocal.GetWindowText(str);
	strcpy(szLocDirectory, str);
}

//���ӷ�����
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

	m_Message = m_Message + "���ӷ�����:" + IP + "\r\n";
	m_Message = m_Message + "��¼�û���:" + m_User + "\r\n";


	//�½��Ի�
	m_pInetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try
	{
		//�½����Ӷ���
		m_pFtpConnection = m_pInetSession->GetFtpConnection(IP, m_User,
			m_Pwd);

	}
	catch (CInternetException *pEx)
	{
		//��ȡ����
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox("There was an exception");
		pEx->Delete();
		m_pFtpConnection = NULL;

		return;
	}
	m_Message = m_Message + "��½�ɹ�!" + "\r\n";
	UpdateData(false);
	m_pRemoteFinder = new CFtpFileFind(m_pFtpConnection);
	BrowseDir("", &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	m_BtnConnect.EnableWindow(m_pFtpConnection == NULL);
	m_ctrlSrv.SetWindowText("\/");
}

void CFTPMFCDlg::BrowseDir(CString strDir, CListCtrl* pLstCtrl, CFileFind* pFinder, CArray<FILEITEM, FILEITEM&>* pFilesArr)//�ļ������
{
	//����б��
	pLstCtrl->DeleteAllItems();
	pFilesArr->RemoveAll();

	int nIndex = 0;

	CString szDir = strDir;
	if (szDir.Right(1) != "\/")
		szDir += "\/";
	szDir += "*.*";
	CTime time; 
	CString str;
	//����Ŀ¼�µ��ļ�
	BOOL res = pFinder->FindFile(szDir);
	while (res)
	{
		res = pFinder->FindNextFile();

		//��Ŀ¼
		if (pFinder->IsDirectory() && !pFinder->IsDots())
		{
			FILEITEM fileitem;
			fileitem.nItem = nIndex;
			fileitem.bDir = TRUE;
			fileitem.strFileName = pFinder->GetFileName();
			pFilesArr->Add(fileitem);
			AddItem(pLstCtrl, nIndex, TRUE, pFinder->GetFileName());
			pLstCtrl->SetItemText(nIndex, 2, "�ļ���");
		}
		//���ļ�
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

void CFTPMFCDlg::AddItem(CListCtrl* pLstCtrl, int nIndex, bool bDir, CString strText)//�ж��Ƿ��ļ����ļ���
{
	LV_ITEM lvitem;
	int iActualItem;
	for (int iSubItem = 0; iSubItem<2; iSubItem++)//��һ��ѭ���ǵ�ǰ�б��ڶ��������б�
	{
		lvitem.mask = LVIF_TEXT | (iSubItem == 0 ? LVIF_IMAGE : 0) | LVIF_PARAM;//isubitem = 0ʱ�����ʽΪLVIF_IMAGE,����Ϊ0;mask˵����Ա����Щ��Ա��Ч
																				//lvitem.mask=LVIF_TEXT|;
		lvitem.iItem = (iSubItem == 0) ? nIndex : iActualItem;//isubitem = 0ʱ�����ʽΪnIndex,����ΪiActualItem��iItemΪ��Ŀ������ֵ
		lvitem.iSubItem = iSubItem;
		switch (iSubItem)
		{
		case 0:
			if (bDir)//������ļ���
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

				lvitem.pszText = "�ļ���";
			}
			else
			{

				lvitem.pszText = "�ļ�";
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
		MessageBox(_T("������FTP�������ĵ�ַ."),
			_T("����"),
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//�������IP
	if (m_Ip.GetAddress(f1, f2, f3, f4) < 4)
	{
		MessageBox(_T("�����FTP��������ַ�Ƿ�."),
			_T("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (m_User == "")
	{
		MessageBox(_T("�û�������Ϊ��"), "����", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

//�ϴ�
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
	m_Message = m_Message + "��ʼ�ϴ��ļ�" + str1 + "\r\n";
	UpdateData(false);
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	pEdit->LineScroll(pEdit->GetLineCount(), 0);


	////////////////////////////////////////////����ϴ����ǵ����ļ�
	CFileFind ff;
	ff.FindFile(str1);
	ff.FindNextFile();
	if (!ff.IsDirectory() && !ff.IsDots())
	{
		czUploadFileFullName = str1;
		CWinThread *pThread = AfxBeginThread(UpLoad, this);
		return;
	}
	else////////////////////////////////////////////����ϴ������ļ���
	{
		AfxMessageBox("��������ʱ��֧���ϴ��ļ���!");
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
			//�����һ����Ŀ¼���õݹ��������һ����
			StartSendFile(ff.GetFilePath());
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//��ʾ��ǰ���ʵ��ļ�
			CString str;
			czUploadFileFullName = ff.GetFilePath();
			//::CRITICAL_SECTION
			CWinThread *pThread = AfxBeginThread(UpLoad, this);
			Sleep(30);
		}
		//Sleep(3000);
	}
	ff.Close();//�ر�
}

//�ϴ�
UINT CFTPMFCDlg::UpLoad(LPVOID para)
{
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;
	CString str;
	pThis->m_ctrlSrv.GetWindowText(str);
	pThis->m_pFtpConnection->PutFile(pThis->czUploadFileFullName, str+"\/"+ pThis->czUploadFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czUploadFileName + "�ϴ����\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	pThis->BrowseDir(str + "\/", &(pThis->m_ctrlServerFile), pThis->m_pRemoteFinder, &(pThis->m_arrRemoteFiles));
	return 0L;
}

//ɾ��������ļ�
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

//ɾ��������ļ�
UINT CFTPMFCDlg::DelServerFile(LPVOID para)
{
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;
	CString szServerFileName = pThis->czDeleteServerFileFullName;

	pThis->m_pFtpConnection->Remove(szServerFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czDeleteServerFileName + "ɾ�����\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);

	return 0L;
}

//����
void CFTPMFCDlg::OnQuickWebtolocal()
{
	int index = m_ctrlServerFile.GetNextItem(-1, LVNI_SELECTED);//���ҷ�����ļ�����һ��ɹ�������һ������������򷵻�-1
	CString str = m_strRemoteCurrentRoot + "\/" + m_ctrlServerFile.GetItemText(index, 0);//�ļ�·��
	czDownloadServerFileName = m_ctrlServerFile.GetItemText(index, 0);//�ļ���
	czDownloadServerFileFullName = str;//�ļ�·��
	if (index<0)
		return;
	CWinThread *pThread = AfxBeginThread(DownloadServerFile, this);//�����ļ�
																   //////////////////////////////////////////////////
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);//��ǰ�ͻ���·��
	str = szLocDirectory;

	CString buf = szLocDirectory;
	if (str.Right(1) != "\/")
		str += "\/";
	str += szLocDirectory;//�ļ�·��
	int iIcon, iIconSel;
	GetFileIcon(buf, &iIcon, &iIconSel);//����ļ�ͼ��
	int i = InsertLocDir(str, iIcon, iIconSel);//�����ļ���Ϣ
	m_ctrlLocal.SetCurSel(i);//ѡ�е�ǰѡ��
	::SetCurrentDirectory(str);
	::GetCurrentDirectory(MAX_PATH, szLocDirectory);
	BeginFindLocFile();	//����һ�鱾���ļ�����ˢ��
}

UINT CFTPMFCDlg::DownloadServerFile(LPVOID para)//�ӷ���������ļ�
{
	CString strLocalFile, strRemoteFile;
	CFTPMFCDlg *pThis = (CFTPMFCDlg *)para;

	CString szServerFileName = pThis->czDownloadServerFileName;
	CString szServerFileFullName = pThis->czDownloadServerFileFullName;
	CString szLocalDir = szLocDirectory;
	szLocalDir = szLocalDir + "\/";
	pThis->m_pFtpConnection->GetFile(szServerFileFullName, szLocalDir + szServerFileName);
	CEdit *pEdit = (CEdit*)(pThis->GetDlgItem(IDC_EDIT3));
	pThis->m_Message = pThis->m_Message + pThis->czDownloadServerFileName + "�������\r\n";
	pEdit->SetWindowText(pThis->m_Message);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	return 0L;
}

//������Ҽ�
void CFTPMFCDlg::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (m_pFtpConnection == NULL)return;
	SetPopSrvMenu(nItem);

	*pResult = 0;
}

//���÷���˵�����
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

//�ͻ���˫���ļ���
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

//���ÿͻ����б��
void CFTPMFCDlg::OnSelchangeComboboxex2()
{
	CString str;
	char lpBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, lpBuffer);
	int i = m_ctrlLocal.GetCurSel();//��ȡ��ǰ����λ��
	m_ctrlLocal.GetLBText(i, (LPTSTR)(LPCTSTR)str);//��������Ϊi�����ݷŵ�str������
	i = 0;
	while (1)
	{
		if (!SetCurrentDirectory(str))//����ǰ����Ŀ¼�л���str������л����ˣ���ִ������
		{
			i++;
			if (i<2)//��ʱi=1
			{
				AfxMessageBox("������������");
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

//�ͻ����б���Ҽ�
void CFTPMFCDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (nItem == -1)return;
	SetPopLocalMenu();
	*pResult = 0;
}

//���ÿͻ��˵�����
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

// �Ͽ�������
void CFTPMFCDlg::OnCtlBreak()
{
	if (m_pFtpConnection == NULL)
		return;
	m_pInetSession->Close();
	m_pFtpConnection = NULL;
	m_Message = m_Message + "�ѶϿ�������!\r\n";
	CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
	UpdateData(false);
	pEdit->LineScroll(pEdit->GetLineCount(), 0);
	m_BtnConnect.EnableWindow(m_pFtpConnection == NULL);
	m_ctrlSrv.ResetContent();
	m_ctrlServerFile.DeleteAllItems();
	
}

//ʹ�Ͽ���ť���
void CFTPMFCDlg::OnUpdateCtlBreak(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//ʹ���Ӱ�ť���
void CFTPMFCDlg::OnUpdateBtnConnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection == NULL);
}

//ʹ���ذ�ť���
void CFTPMFCDlg::OnUpdateQuickWebtolocal(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//ʹ�ϴ���ť���
void CFTPMFCDlg::OnUpdateUploadfile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pFtpConnection != NULL);
}

//��������˫��
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

void CFTPMFCDlg::OnBack()//����˷��ذ�ť
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



//������������
void CFTPMFCDlg::OnRenName()
{
	// TODO: �ڴ���������������
	ReNameIndex = m_ctrlServerFile.GetNextItem(-1, LVNI_SELECTED);
	strName = m_strRemoteCurrentRoot + "\/" + m_ctrlServerFile.GetItemText(ReNameIndex, 0);
	rename.m_type = m_ctrlServerFile.GetItemText(ReNameIndex, 0);
	rename.DoModal();

	if (rename.flag == true)
	{
		czReName = m_strRemoteCurrentRoot + "\/" + rename.m_ReName;
		if (m_pFtpConnection->Rename(strName, czReName))
		{
			m_Message = m_Message + rename.m_ReName + "�޸����\r\n";
			CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT3));
			pEdit->SetWindowText(m_Message);
			pEdit->LineScroll(pEdit->GetLineCount(), 0);
			BrowseDir(m_strRemoteCurrentRoot, &m_ctrlServerFile, m_pRemoteFinder, &m_arrRemoteFiles);
		}
		else
		{
			
			MessageBox("����ʧ��");
		}

	}
}
