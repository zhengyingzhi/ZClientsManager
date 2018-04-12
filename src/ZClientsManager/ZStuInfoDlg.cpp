// ZStuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "ZClientsManager.h"
#include "ZStuInfoDlg.h"
#include "ZInfoDesc.h"

extern ZDataBase* g_pStuDB;

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

		double lScore = (m_StuInfo.LanguageScore / 10);
		if (m_StuInfo.LanguageScore > 2000)
			lString.Format("%.0f", lScore);
		else
			lString.Format("%.1f", lScore);
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

void ZStuInfoDlg::GetDlgItemValue(int nID, char aBuffer[], int aBufSize)
{
	CString lString;
	GetDlgItemText(nID, lString);
	strncpy(aBuffer, (char*)(LPCSTR)lString, aBufSize - 1);
}

void ZStuInfoDlg::GetDlgItemValue(int nID, int32_t& aValue)
{
	CString lString;
	GetDlgItemText(nID, lString);
	aValue = atoi((char*)(LPCSTR)lString);
}

void ZStuInfoDlg::GetDlgItemValue(int nID, uint32_t& aValue)
{
	CString lString;
	GetDlgItemText(nID, lString);
	aValue = atoi((char*)(LPCSTR)lString);
}

void ZStuInfoDlg::GetDlgItemValue(int nID, int64_t& aValue)
{
	CString lString;
	GetDlgItemText(nID, lString);
	aValue = atoll((char*)(LPCSTR)lString);
}

void ZStuInfoDlg::OnBnClickedBtnSave()
{
	ZStudentInfo lStuInfo;
	memset(&lStuInfo, 0, sizeof(lStuInfo));

	GetDlgItemValue(IDC_EDIT_NAME, lStuInfo.Name, sizeof(lStuInfo.Name));
	GetDlgItemValue(IDC_EDIT_TELEPHONE, lStuInfo.Telehone, sizeof(lStuInfo.Telehone));
	GetDlgItemValue(IDC_EDIT_QQ, lStuInfo.QQ, sizeof(lStuInfo.QQ));
	GetDlgItemValue(IDC_EDIT_CLASS, lStuInfo.Class, sizeof(lStuInfo.Class));
	GetDlgItemValue(IDC_EDIT_COLLEGE, lStuInfo.College, sizeof(lStuInfo.College));
	GetDlgItemValue(IDC_EDIT_MAJOR, lStuInfo.Major, sizeof(lStuInfo.Major));
	GetDlgItemValue(IDC_EDIT_COUNTRY, lStuInfo.Country, sizeof(lStuInfo.Country));
	GetDlgItemValue(IDC_EDIT_SOURCE, lStuInfo.Source, sizeof(lStuInfo.Source));
	GetDlgItemValue(IDC_EDIT_STATUS, lStuInfo.Status, sizeof(lStuInfo.Status));
	GetDlgItemValue(IDC_EDIT_COMMENT, lStuInfo.Comments, sizeof(lStuInfo.Comments));

	CString lString;
	GetDlgItemText(IDC_EDIT_LANGSCORE, lString);
	double lScore = atof((char*)(LPCSTR)lString);
	lStuInfo.LanguageScore = uint32_t(lScore * 10);

	CButton* lpBtn = (CButton*)GetDlgItem(IDC_RADIO_BOY);
	lStuInfo.Sex = lpBtn->GetCheck() ? SSEX_Boy : SSEX_Girl;

	if (memcpy(&m_StuInfo, &lStuInfo, sizeof(ZStudentInfo)) == 0)
	{
		return;
	}

	if (strcmp(m_StuInfo.Name, lStuInfo.Name) == 0 &&
		strcmp(m_StuInfo.Telehone, lStuInfo.Telehone) == 0)
	{
		// update
		g_pStuDB->Update(&lStuInfo, sizeof(lStuInfo));
	}
	else
	{
		// insert
		g_pStuDB->Insert(&lStuInfo, sizeof(lStuInfo));
	}

}
