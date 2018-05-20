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
		SetDlgItemText(IDC_EDIT_TELEPHONE, m_StuInfo.Telehone);
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

	CString lName, lTelephone, lCollge, lQQ, lStatus, lSource, lEMail;
	GetDlgItemText(IDC_EDIT_NAME, lName);
	GetDlgItemText(IDC_EDIT_TELEPHONE, lTelephone);
	GetDlgItemText(IDC_EDIT_COLLEGE, lCollge);
	GetDlgItemText(IDC_EDIT_QQ, lQQ);
	GetDlgItemText(IDC_EDIT_STATUS, lStatus);
	GetDlgItemText(IDC_EDIT_SOURCE, lSource);
	GetDlgItemText(IDC_EDIT_EMAIL, lEMail);

	strncpy(lStuInfo.Name, (char*)(LPCSTR)lName, sizeof(lStuInfo.Name) - 1);
	strncpy(lStuInfo.Telehone, (char*)(LPCSTR)lTelephone, sizeof(lStuInfo.Telehone) - 1);
	strncpy(lStuInfo.CollegeFrom, (char*)(LPCSTR)lCollge, sizeof(lStuInfo.CollegeFrom) - 1);
	strncpy(lStuInfo.QQ, (char*)(LPCSTR)lQQ, sizeof(lStuInfo.QQ) - 1);
	strncpy(lStuInfo.Status, (char*)(LPCSTR)lStatus, sizeof(lStuInfo.Status) - 1);
	strncpy(lStuInfo.Source, (char*)(LPCSTR)lSource, sizeof(lStuInfo.Source) - 1);
	strncpy(lStuInfo.EMail, (char*)(LPCSTR)lEMail, sizeof(lStuInfo.Source) - 1);

	vector<ZStudentInfo*> lVec;
	if (!lName.IsEmpty() || !lTelephone.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareNameAndTel, 0);
	else if (!lCollge.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareCollege, 0);
	else if (!lQQ.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareStuQQ, 0);
	else if (!lEMail.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareEMail, 0);
	else if (!lStatus.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareStatus, 0);
	else if (!lSource.IsEmpty())
		lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareSource, 0);

	// todo: 支持按录入时间查询

	if (lVec.empty())
	{
		AfxMessageBox(_T("未查询到相关数据"), MB_OK);
	}
	else
	{
		if (m_pMainFrame)
			m_pMainFrame->UpdateStuToMainListView(lVec, FALSE);
		CDialogEx::OnOK();
	}

}


void ZStuQryDlg::OnBnClickedBtnClear()
{
	SetDlgItemText(IDC_EDIT_NAME, "");
	SetDlgItemText(IDC_EDIT_TELEPHONE, "");
	SetDlgItemText(IDC_EDIT_COUNTRY, "");
	SetDlgItemText(IDC_EDIT_EMAIL, "");
}
