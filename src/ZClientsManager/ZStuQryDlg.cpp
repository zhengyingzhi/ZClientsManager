// ZStuQryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZStuQryDlg.h"
#include "afxdialogex.h"


// ZStuQryDlg dialog

IMPLEMENT_DYNAMIC(ZStuQryDlg, CDialogEx)

ZStuQryDlg::ZStuQryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_QUERY, pParent)
{
	memset(&m_StuInfo, 0, sizeof(m_StuInfo));
}

ZStuQryDlg::~ZStuQryDlg()
{
}

void ZStuQryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL ZStuQryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_StuInfo.Name)
	{
		SetDlgItemText(IDC_EDIT_NAME, m_StuInfo.Name);
	}
	if (m_StuInfo.Telehone)
	{
		SetDlgItemText(IDC_EDIT_NAME, m_StuInfo.Telehone);
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(ZStuQryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_QUERY, &ZStuQryDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &ZStuQryDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// ZStuQryDlg message handlers



void ZStuQryDlg::SetStuInfo(const ZStudentInfo& aStuInfo)
{
	memcpy(&m_StuInfo, &aStuInfo, sizeof(ZStudentInfo));
}

void ZStuQryDlg::OnBnClickedBtnQuery()
{
	// TODO: Add your control notification handler code here
}


void ZStuQryDlg::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
}
