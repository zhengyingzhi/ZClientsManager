// ZLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZLoginDlg.h"
#include "afxdialogex.h"


// ZLoginDlg dialog

IMPLEMENT_DYNAMIC(ZLoginDlg, CDialogEx)

ZLoginDlg::ZLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{
	m_RememberPasswd = FALSE;
}

ZLoginDlg::~ZLoginDlg()
{
}

void ZLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL ZLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_UserID.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_User, m_UserID);
	}
	if (!m_Passwd.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_Password, m_Passwd);
	}

	GetDlgItem(IDC_CHK_MEMORY_PWD)->EnableWindow(m_RememberPasswd);

	return TRUE;
}


BEGIN_MESSAGE_MAP(ZLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ZLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ZLoginDlg message handlers



CString ZLoginDlg::GetUserID()
{
	return m_UserID;
}

void ZLoginDlg::SetUserID(const CString& aUserID)
{
	m_UserID = aUserID;
}

void ZLoginDlg::SetPassword(const CString& aPassword)
{
	m_Passwd = aPassword;
}

void ZLoginDlg::SetRememberPwd(BOOL aOn)
{
	m_RememberPasswd = aOn;
}

CString ZLoginDlg::GetPasswd()
{
	return m_Passwd;
}

BOOL ZLoginDlg::IsRememberPasswd()
{
	return m_RememberPasswd;
}

void ZLoginDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_User, m_UserID);
	GetDlgItemText(IDC_EDIT_Password, m_Passwd);

	CButton* lpBtn = (CButton*)GetDlgItem(IDC_CHK_MEMORY_PWD);
	if (lpBtn->GetCheck() == 1) {
		m_RememberPasswd = 1;
	}

	CDialogEx::OnOK();
}
