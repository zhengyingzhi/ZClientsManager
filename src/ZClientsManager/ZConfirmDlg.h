#pragma once


// ZConfirmDlg dialog

class ZConfirmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZConfirmDlg)

public:
	ZConfirmDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZConfirmDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
