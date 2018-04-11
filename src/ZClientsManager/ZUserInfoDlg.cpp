// ZUserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZUserInfoDlg.h"
#include "afxdialogex.h"


// ZUserInfoDlg dialog

IMPLEMENT_DYNAMIC(ZUserInfoDlg, CDialogEx)

ZUserInfoDlg::ZUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USERINFO, pParent)
{

}

ZUserInfoDlg::~ZUserInfoDlg()
{
}

void ZUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &ZUserInfoDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// ZUserInfoDlg message handlers


void ZUserInfoDlg::OnBnClickedBtnSave()
{
	CString lName, lString;
	GetDlgItemText(IDC_EDIT_NAME, lName);
	
	// todo: update user info into db
}
