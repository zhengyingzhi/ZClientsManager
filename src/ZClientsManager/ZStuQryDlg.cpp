// ZStuQryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "MainFrm.h"
#include "ZStuQryDlg.h"
#include "afxdialogex.h"


// ZStuQryDlg dialog

IMPLEMENT_DYNAMIC(ZStuQryDlg, CDialogEx)

ZStuQryDlg::ZStuQryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_QUERY, pParent)
{
	m_pMainFrame = NULL;
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


void ZStuQryDlg::SetMainFrame(CMainFrame* apMainFrame)
{
	m_pMainFrame = apMainFrame;
}

void ZStuQryDlg::SetStuInfo(const ZStudentInfo& aStuInfo)
{
	memcpy(&m_StuInfo, &aStuInfo, sizeof(ZStudentInfo));
}

void ZStuQryDlg::OnBnClickedBtnQuery()
{
	ZStudentInfo lStuInfo = {};

	CString lString;
	GetDlgItemText(IDC_EDIT_NAME, lString);
	strncpy(lStuInfo.Name, (char*)(LPCSTR)lString, sizeof(lStuInfo.Name) - 1);

	vector<ZStudentInfo*> lVec;
	lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareUserName, 0);
	if (lVec.empty())
	{
		AfxMessageBox(_T("未查询到相关数据"), MB_OK);
	}
	else
	{
		if (m_pMainFrame)
			m_pMainFrame->UpdateStuToMainListView(lVec);
		CDialogEx::OnOK();
	}

}


void ZStuQryDlg::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
}
