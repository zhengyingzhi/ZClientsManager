// ZStuInfoDlg.cpp : implementation file
//

#include <time.h>

#include "stdafx.h"
#include "afxdialogex.h"

#include "ZClientsManager.h"
#include "MainFrm.h"

#include "ZStuInfoDlg.h"
#include "ZInfoDesc.h"

#include "ZAppConfig.h"

#include "ZUtility.h"

#include <ZToolLib/ztl_logger.h>
extern ztl_log_t* g_Logger;


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
	DDX_Control(pDX, IDC_COMBO_IMPORTANT, m_comboImportant);
	DDX_Control(pDX, IDC_DTPICKER_Date, m_DatePick);
	DDX_Control(pDX, IDC_DTPICKER_Time, m_TimePick);
}

BOOL ZStuInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboImportant.InsertString(SIM_Normal, _T("普通"));      // 0
	m_comboImportant.InsertString(SIM_Important, _T("重要"));   // 1
	m_comboImportant.InsertString(SIM_Emergency, _T("紧急"));   // 2
	m_comboImportant.SetCurSel(0);

	CString lText;
	switch (m_OperateType)
	{
	case ZOT_Update:
		lText = _T("更新");
		break;
	case ZOT_Delete:
		lText = _T("删除");
		break;
	default:
		lText = _T("保存");
		break;
	}
	GetDlgItem(IDC_BTN_SAVE)->SetWindowText(lText);

	CString lString;
	if (m_StuInfo.Name[0])
	{
		//lString.Format(_T("傲睿国际教育 - %s"), m_StuInfo.Name);
		//SetWindowText(lString);

		lString.Format("%d", m_StuInfo.Number);
		SetDlgItemText(IDC_EDIT_NUMBER, lString);

		SetDlgItemText(IDC_EDIT_NAME, m_StuInfo.Name);
		SetDlgItemText(IDC_EDIT_TELEPHONE, m_StuInfo.Telehone);
		SetDlgItemText(IDC_EDIT_QQ, m_StuInfo.QQ);
		SetDlgItemText(IDC_EDIT_COLLEGE, m_StuInfo.CollegeFrom);
		SetDlgItemText(IDC_EDIT_COLLEGE_TO, m_StuInfo.CollegeTo);
		SetDlgItemText(IDC_EDIT_MAJOR, m_StuInfo.MajorFrom);
		SetDlgItemText(IDC_EDIT_MAJOR_TO, m_StuInfo.MajorTo);
		SetDlgItemText(IDC_EDIT_CLASS, m_StuInfo.Class);
		SetDlgItemText(IDC_EDIT_COUNTRY, m_StuInfo.Country);
		SetDlgItemText(IDC_EDIT_IDNumber, m_StuInfo.IDNumber);

		std::string lTimeStr;
		lTimeStr = ZConvStdTimeStr((time_t)m_StuInfo.InsertTime);
		SetDlgItemText(IDC_EDIT_INSERTTIME, lTimeStr.c_str());
		lTimeStr = ZConvStdTimeStr((time_t)m_StuInfo.UpdateTime);
		SetDlgItemText(IDC_EDIT_UPDATETIME, lTimeStr.c_str());

		CTime lVisitTime((time_t)m_StuInfo.NextVisitTime);
		m_DatePick.SetTime(&lVisitTime);
		m_TimePick.SetTime(&lVisitTime);

		double lScore = (m_StuInfo.LanguageScore / 10);
		if (m_StuInfo.LanguageScore > 2000)
			lString.Format("%.0f", lScore);
		else
			lString.Format("%.1f", lScore);
		SetDlgItemText(IDC_EDIT_LANGSCORE, lString);

		double lGPA = (m_StuInfo.GPA / 10);
		lString.Format("%.1f", lGPA);
		SetDlgItemText(IDC_EDIT_GPA, lString);

		//SetDlgItemText(IDC_COMBO_IMPORTANT, ZStuImportantDesc(m_StuInfo.ImportantLevel));
		SetDlgItemText(IDC_EDIT_SOURCE, m_StuInfo.Source);
		SetDlgItemText(IDC_EDIT_STATUS, m_StuInfo.Status);
		SetDlgItemText(IDC_EDIT_EMAIL, m_StuInfo.EMail);
		SetDlgItemText(IDC_EDIT_COMMENT, m_StuInfo.Comments);

		m_comboImportant.SetCurSel(m_StuInfo.ImportantLevel);

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
	else
	{
// 		time_t lNow = time(0) + (30 * 60);
// 		CTime lVisitTime(lNow);
// 		m_DatePick.SetTime(&lVisitTime);
// 		m_TimePick.SetTime(&lVisitTime);
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(ZStuInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &ZStuInfoDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// ZStuInfoDlg message handlers

void ZStuInfoDlg::SetStudentInfo(ZStudentInfo* apStuInfo)
{
	if (apStuInfo)
		memcpy(&m_StuInfo, apStuInfo, sizeof(m_StuInfo));
	else
		memset(&m_StuInfo, 0, sizeof(m_StuInfo));
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

// 点击保存按钮事件
void ZStuInfoDlg::OnBnClickedBtnSave()
{
	/* TODO: 每更新一条消息时，应持久化该操作到一个文件中，便于查找历史 */

	ZStudentInfo lStuInfo;
	memset(&lStuInfo, 0, sizeof(lStuInfo));

	GetDlgItemValue(IDC_EDIT_NUMBER, lStuInfo.Number);
	GetDlgItemValue(IDC_EDIT_NAME, lStuInfo.Name, sizeof(lStuInfo.Name));
	GetDlgItemValue(IDC_EDIT_TELEPHONE, lStuInfo.Telehone, sizeof(lStuInfo.Telehone));
	GetDlgItemValue(IDC_EDIT_QQ, lStuInfo.QQ, sizeof(lStuInfo.QQ));
	GetDlgItemValue(IDC_EDIT_CLASS, lStuInfo.Class, sizeof(lStuInfo.Class));
	GetDlgItemValue(IDC_EDIT_COLLEGE, lStuInfo.CollegeFrom, sizeof(lStuInfo.CollegeFrom));
	GetDlgItemValue(IDC_EDIT_COLLEGE_TO, lStuInfo.CollegeTo, sizeof(lStuInfo.CollegeTo));
	GetDlgItemValue(IDC_EDIT_MAJOR, lStuInfo.MajorFrom, sizeof(lStuInfo.MajorFrom));
	GetDlgItemValue(IDC_EDIT_MAJOR_TO, lStuInfo.MajorTo, sizeof(lStuInfo.MajorTo));
	GetDlgItemValue(IDC_EDIT_COUNTRY, lStuInfo.Country, sizeof(lStuInfo.Country));
	GetDlgItemValue(IDC_EDIT_SOURCE, lStuInfo.Source, sizeof(lStuInfo.Source));
	GetDlgItemValue(IDC_EDIT_STATUS, lStuInfo.Status, sizeof(lStuInfo.Status));
	GetDlgItemValue(IDC_EDIT_IDNumber, lStuInfo.IDNumber, sizeof(lStuInfo.IDNumber));
	GetDlgItemValue(IDC_EDIT_EMAIL, lStuInfo.EMail, sizeof(lStuInfo.EMail));
	GetDlgItemValue(IDC_EDIT_COMMENT, lStuInfo.Comments, sizeof(lStuInfo.Comments));

	lStuInfo.ImportantLevel = (StuImportant)m_comboImportant.GetCurSel();

	CString lString;
	double lScore;
	GetDlgItemText(IDC_EDIT_LANGSCORE, lString);
	lScore = atof((char*)(LPCSTR)lString);
	lStuInfo.LanguageScore = uint32_t(lScore * 10);

	GetDlgItemText(IDC_EDIT_GPA, lString);
	lScore = atof((char*)(LPCSTR)lString);
	lStuInfo.GPA = uint32_t(lScore * 10);

	CButton* lpBtn = (CButton*)GetDlgItem(IDC_RADIO_BOY);
	lStuInfo.Sex = lpBtn->GetCheck() ? SSEX_Boy : SSEX_Girl;

	// 比较数据
	if (ZStuInfoEqual(&m_StuInfo, &lStuInfo))
	{
		AfxMessageBox(_T("未更新任何数据"), MB_OK | MB_ICONWARNING);
		return;
	}

	DWORD lRet = 0;
	if (m_OperateType == ZOT_Update) {
		lRet = AfxMessageBox(_T("确定更新数据?"), MB_YESNO);
	}
	if (IDNO == lRet) {
		return;
	}

	char lFixString[4096] = "";
	ZStuInfoFixString(&lStuInfo, lFixString, 0);
	ztl_log_error(g_Logger, ZTL_LOG_INFO, lFixString);

	if (strcmp(m_StuInfo.Name, lStuInfo.Name) == 0 &&
		strcmp(m_StuInfo.Telehone, lStuInfo.Telehone) == 0)
	{
		lStuInfo.UpdateTime = time(0);

		// update
		lRet = g_MemData.GetStuDB()->Update(&lStuInfo, sizeof(lStuInfo));
		if (lRet != 0)
		{
			AfxMessageBox(_T("插入学生信息数据失败"), MB_OK | MB_ICONWARNING);
			return;
		}
	}
	else
	{
		lStuInfo.InsertTime = time(0);

		// insert
		lRet = g_MemData.GetStuDB()->Insert(&lStuInfo, sizeof(lStuInfo));
		if (lRet != 0)
		{
			AfxMessageBox(_T("插入学生信息数据失败"), MB_OK | MB_ICONWARNING);
			return;
		}

		vector<ZStudentInfo*> lVec;
		lVec.push_back(&lStuInfo);
		g_pMainFrame->UpdateStuToMainListView(lVec, TRUE);
	}
	memcpy(&m_StuInfo, &lStuInfo, sizeof(ZStudentInfo));

	g_MemData.AddOrUpdateStuInfo(0, &m_StuInfo);

	g_pMainFrame->UpdateStuToMainListView(&m_StuInfo, FALSE);

	CDialogEx::OnOK();
}
