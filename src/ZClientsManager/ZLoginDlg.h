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
	BOOL    IsRememberPasswd();

	void SetUserID(const CString& aUserID);
	void SetPassword(const CString& aPassword);
	void SetRememberPwd(BOOL aOn);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString m_UserID;
	CString m_Passwd;
	BOOL    m_RememberPasswd;
};
