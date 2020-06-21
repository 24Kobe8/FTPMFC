// ReName.cpp: 实现文件
//

#include "FTPMFC.h"
#include "ReName.h"
#include "afxdialogex.h"


// ReName 对话框

IMPLEMENT_DYNAMIC(ReName, CDialogEx)

ReName::ReName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ReName::~ReName()
{
}

void ReName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ReName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ReName::OnBnClickedButton1)
END_MESSAGE_MAP()


// ReName 消息处理程序


void ReName::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
