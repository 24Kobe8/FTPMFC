#pragma once


// ReName 对话框

class ReName : public CDialogEx
{
	DECLARE_DYNAMIC(ReName)

public:
	ReName(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ReName();
	bool flag;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
