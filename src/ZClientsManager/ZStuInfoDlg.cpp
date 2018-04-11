// ZStuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZStuInfoDlg.h"
#include "afxdialogex.h"


// ZStuInfoDlg dialog

IMPLEMENT_DYNAMIC(ZStuInfoDlg, CDialogEx)

ZStuInfoDlg::ZStuInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STUINFO, pParent)
{

}

ZStuInfoDlg::~ZStuInfoDlg()
{
}

void ZStuInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZStuInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &ZStuInfoDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// ZStuInfoDlg message handlers


void ZStuInfoDlg::OnBnClickedBtnSave()
{
	ZStudentInfo lStuInfo;
	memset(&lStuInfo, 0, sizeof(lStuInfo));

	CString lName, lString;
	GetDlgItemText(IDC_EDIT_NAME, lName);

	// todo: save into db
}
