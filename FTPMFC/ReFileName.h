#pragma once


// ReFileName 对话框

class ReFileName : public CDialogEx
{
	DECLARE_DYNAMIC(ReFileName)

public:
	ReFileName(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ReFileName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ReName;
	CButton m_Sure;
	CButton m_Close;
	bool flag;
	afx_msg void OnBnClickedBtnsure();
	afx_msg void OnBnClickedBtnclose();
	CString m_type;
	afx_msg void OnStnClickedType();
};
