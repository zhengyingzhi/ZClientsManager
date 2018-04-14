// ZConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZConfirmDlg.h"
#include "afxdialogex.h"


// ZConfirmDlg dialog

IMPLEMENT_DYNAMIC(ZConfirmDlg, CDialogEx)

ZConfirmDlg::ZConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIRM, pParent)
{

}

ZConfirmDlg::~ZConfirmDlg()
{
}

void ZConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZConfirmDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ZConfirmDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ZConfirmDlg message handlers


void ZConfirmDlg::OnBnClickedOk()
{
	CString lInput;
	GetDlgItemText(IDC_EDIT_YESNO, lInput);

	if (lInput == "yes" || lInput == "Yes" || lInput == "YES")
		CDialogEx::OnOK();
	else
		CDialogEx::OnCancel();
}
