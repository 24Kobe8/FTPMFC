
// FTPMFCDlg.h : ͷ�ļ�
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <afxinet.h>
#include <afxtempl.h>
#include "ReFileName.h"

#define WM_SETDIRECTORYDISPLAY     WM_USER +102
#define WM_SETFILE     WM_USER +100
#define WM_SETREDRAWFLAG     WM_USER +101
enum
{
	FILEICON,
	DIRECTORYICON,
	LOCFILE,
	SERVFILE
};
typedef struct FileItem
{
	int nItem;				//���б���е�����
	BOOL bDir;				//���ļ��У�
	CString strFileName;	//�ļ���
}FILEITEM;

// CFTPMFCDlg �Ի���
class CFTPMFCDlg : public CDialogEx
{
// ����
public:
	CFTPMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTPMFC_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	afx_msg void OnBack();
	afx_msg void OnBtnConnect();
	afx_msg void OnRenName();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteSrvFile();
	afx_msg void OnQuickWebtolocal();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboboxex2();
	afx_msg void OnUpdateQuickWebtolocal(CCmdUI *pCmdUI);
	afx_msg void OnUploadFile();
	afx_msg void OnUpdateUploadfile(CCmdUI *pCmdUI);
	static UINT UpLoad(LPVOID para);
	static UINT DelServerFile(LPVOID para);
	static UINT DownloadServerFile(LPVOID para);
protected:
	afx_msg LRESULT OnInsertLocFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetLocDirectoryDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetLocRedrawFlag(WPARAM wParam, LPARAM lParam);
public:
	void AddItem(CListCtrl* pLstCtrl, int nIndex, bool bDir, CString strText);
	BOOL BeginFindLocFile();
	void BrowseDir(CString strDir, CListCtrl* pLstCtrl, CFileFind* pFinder, CArray<FILEITEM, FILEITEM&>* pFilesArr);
	void FindLocFile();
	// ����ļ�ͼ��
	void GetFileIcon(CString& fileName, int* iIcon, int* iIconSel=NULL);
	// ��ÿͻ��˵�ǰ·��
	void GetLocCurrentDirectory();
	// ��ÿͻ��˴���·��
	void GetLocDir();
	int InsertLocDir(CString& str, int iIcon, int iIconSel);
	// �Ͽ�������
//	void OnCtlBreak();
	// �ڿͻ����ļ��б������ҵ����ļ���
	void OnInsertFile(CListCtrl* pListCtrl, LPVOID pIn, LPARAM lParam);
	void OnUpdateBtnConnect(CCmdUI* pCmdUI);
	BOOL SetImageList(CListCtrl* pListCtrl, CComboBoxEx* pCombo);
	void SetServerFileColumns(CListCtrl* pListCtrl);
	void StartSendFile(CString FileName);
	void OnUpdateCtlBreak(CCmdUI* pCmdUI);
	void SetFileColumns(CListCtrl* pListCtrl);
	CButton m_BtnConnect;
	CComboBoxEx m_ctrlLocal;
	CComboBoxEx m_ctrlSrv;
	CIPAddressCtrl m_Ip;
	CListCtrl m_ctrlLocalFile;
	CListCtrl m_ctrlServerFile;
	BOOL m_bConnected;
	CInternetSession* m_pInetSession;
	CFtpConnection* m_pFtpConnection;
	CFtpFileFind* m_pRemoteFinder;
	CArray<FILEITEM, FILEITEM&> m_arrLocalFiles;
	CArray<FILEITEM, FILEITEM&> m_arrRemoteFiles;
	CString m_strRemoteParentRoot;
	CString m_strRemoteCurrentRoot;
	CString czUploadFileName;
	CString czUploadFileFullName;
	CString czDeleteServerFileName;
	CString czDeleteServerFileFullName;
	CString czDownloadServerFileName;
	CString czDownloadServerFileFullName;
	CString LocalDir;
	CString czReName;
	int m_iDirCount;
	typedef struct
	{
		char szFileName[256];
		char szFileDate[20];
		char szFileSize[20];
		int nType;
	}FILE_FTP_INFO;
	
public:
	void SetPopLocalMenu();
	void SetPopSrvMenu(int nItem);
	BOOL IsValid();
	CString m_User;
	CString m_Pwd;
	CString m_Message;
	CTreeCtrl	m_site_info_tree;
	CButton	m_ctBack;
	CImageList m_ctImageList;
	CImageList m_ServerImageList;
	CMenu m_Menu;
	BOOL m_bHaveDotFlag;

	ReFileName rename;
	
	// �Ͽ�������
	void OnCtlBreak();
	// �رնԻ���
	void OnbtnClose();
	afx_msg void OnBnClickedCtlBreak();
	
	
};



