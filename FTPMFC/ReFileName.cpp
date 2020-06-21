// ReFileName.cpp: 实现文件
//

#include "stdafx.h"
#include "FTPMFC.h"
#include "ReFileName.h"
#include "afxdialogex.h"


// ReFileName 对话框

IMPLEMENT_DYNAMIC(ReFileName, CDialogEx)

ReFileName::ReFileName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_ReName(_T(""))
	, m_type(_T(""))
{
	m_ReName = "";
	flag = false;
}

ReFileName::~ReFileName()
{
}

void ReFileName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITName, m_ReName);
	DDX_Control(pDX, IDC_BtnSure, m_Sure);
	DDX_Control(pDX, IDC_BtnClose, m_Close);
	DDX_Text(pDX, IDC_TYPE, m_type);
}


BEGIN_MESSAGE_MAP(ReFileName, CDialogEx)
	ON_BN_CLICKED(IDC_BtnSure, &ReFileName::OnBnClickedBtnsure)
	ON_BN_CLICKED(IDC_BtnClose, &ReFileName::OnBnClickedBtnclose)
	ON_STN_CLICKED(IDC_TYPE, &ReFileName::OnStnClickedType)
END_MESSAGE_MAP()


// ReFileName 消息处理程序


void ReFileName::OnBnClickedBtnsure()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = true;

	ReFileName::OnOK();
}


void ReFileName::OnBnClickedBtnclose()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = false;
	ReFileName::OnCancel();
}



 
void ReFileName::OnStnClickedType()
{
	// TODO: 在此添加控件通知处理程序代码
}
