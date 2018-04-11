#pragma once


// ZLoginDlg dialog

class ZLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZLoginDlg)

public:
	ZLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZLoginDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

public:
	CString GetUserID();
	CString GetPasswd();
	bool    IsRememberPasswd();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString m_UserID;
	CString m_Passwd;
	int     m_RememberPasswd;
};
