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

}

ZLoginDlg::~ZLoginDlg()
{
}

void ZLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ZLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ZLoginDlg message handlers



CString ZLoginDlg::GetUserID()
{
	return m_UserID;
}

CString ZLoginDlg::GetPasswd()
{
	return m_Passwd;
}

bool ZLoginDlg::IsRememberPasswd()
{
	return m_RememberPasswd == 1 ? true : false;
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
