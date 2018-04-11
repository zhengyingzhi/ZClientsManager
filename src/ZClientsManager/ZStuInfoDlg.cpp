// ZStuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "ZClientsManager.h"
#include "ZStuInfoDlg.h"
#include "ZInfoDesc.h"


// ZStuInfoDlg dialog

IMPLEMENT_DYNAMIC(ZStuInfoDlg, CDialogEx)

ZStuInfoDlg::ZStuInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STUINFO, pParent)
{
	memset(&m_StuInfo, 0, sizeof(m_StuInfo));
}

ZStuInfoDlg::~ZStuInfoDlg()
{
}

void ZStuInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL ZStuInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString lString;
	if (m_StuInfo.Name[0])
	{
		SetDlgItemText(IDC_EDIT_NAME, m_StuInfo.Name);
		SetDlgItemText(IDC_EDIT_TELEPHONE, m_StuInfo.Telehone);
		SetDlgItemText(IDC_EDIT_COLLEGE, m_StuInfo.College);
		SetDlgItemText(IDC_EDIT_MAJOR, m_StuInfo.Major);
		SetDlgItemText(IDC_EDIT_CLASS, m_StuInfo.Class);
		SetDlgItemText(IDC_EDIT_COUNTRY, m_StuInfo.Country);

		lString.Format("%.1f", m_StuInfo.LanguageScore);
		SetDlgItemText(IDC_EDIT_LANGSCORE, lString);
		SetDlgItemText(IDC_EDIT_QQ, m_StuInfo.QQ);
		SetDlgItemText(IDC_COMBO_IMPORTANT, ZStuImportantDesc(m_StuInfo.ImportantLevel));
		SetDlgItemText(IDC_EDIT_SOURCE, m_StuInfo.Source);
		SetDlgItemText(IDC_EDIT_STATUS, m_StuInfo.Status);
		SetDlgItemText(IDC_EDIT_COMMENT, m_StuInfo.Comments);

		CButton* lpBtn;
		if (m_StuInfo.Sex == SSEX_Boy) {
			lpBtn = (CButton*)GetDlgItem(IDC_RADIO_BOY);
			lpBtn->SetCheck(1);
		}
		else {
			lpBtn = (CButton*)GetDlgItem(IDC_RADIO_GIRL);
			lpBtn->SetCheck(1);
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(ZStuInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &ZStuInfoDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// ZStuInfoDlg message handlers

void ZStuInfoDlg::SetStudentInfo(ZStudentInfo* apStuInfo)
{
	memcpy(&m_StuInfo, apStuInfo, sizeof(m_StuInfo));
}

void ZStuInfoDlg::OnBnClickedBtnSave()
{
	ZStudentInfo lStuInfo;
	memset(&lStuInfo, 0, sizeof(lStuInfo));

	CString lName, lString;
	GetDlgItemText(IDC_EDIT_NAME, lName);

	// todo: save into db
}
