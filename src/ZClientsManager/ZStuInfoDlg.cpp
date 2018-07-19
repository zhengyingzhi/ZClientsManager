// ZStuInfoDlg.cpp : implementation file
//

#include <time.h>
#include <assert.h>

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
	m_OperateType = ZOT_Show;
}

ZStuInfoDlg::~ZStuInfoDlg()
{
}

void ZStuInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMPORTANT, m_comboImportant);
	DDX_Control(pDX, IDC_LIST_COMMENTS, m_ListComments);
}

BOOL ZStuInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboImportant.InsertString(SIM_Normal, _T("普通"));      // 0
	m_comboImportant.InsertString(SIM_Important, _T("重要"));   // 1
	m_comboImportant.InsertString(SIM_Emergency, _T("紧急"));   // 2
	m_comboImportant.SetCurSel(0);

	DWORD dwStyle;
	m_ListComments.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = m_ListComments.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	m_ListComments.SetExtendedStyle(dwStyle);

	m_ListComments.InsertColumn(COMMENTSLIST_COL_Row, _T("编号"), LVCFMT_LEFT, 40);
	m_ListComments.InsertColumn(COMMENTSLIST_COL_ByName, _T("备注人"), LVCFMT_LEFT, 60);
	m_ListComments.InsertColumn(COMMENTSLIST_COL_Time, _T("时间"), LVCFMT_LEFT, 120);
	m_ListComments.InsertColumn(COMMENTSLIST_COL_Content, _T("内容"), LVCFMT_LEFT, 300);

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
		SetDlgItemText(IDC_EDIT_ADDTIME, lTimeStr.c_str());
		lTimeStr = ZConvStdTimeStr((time_t)m_StuInfo.UpdateTime);
		SetDlgItemText(IDC_EDIT_UPDATETIME, lTimeStr.c_str());

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

		// split comments by \r\n
		ZComments lCommVec(m_StuInfo.Comments, "\r\n");
		for (int i = 0; i < lCommVec.GetCount(); ++i)
		{
			assert(lCommVec[i]);
			ZCommentLine& lCommLine = *lCommVec[i];

			int lRow = i;
			m_ListComments.InsertItem(lRow, _T(""));
			m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Row, lCommLine[0].c_str());
			m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_ByName, lCommLine[1].c_str());
			m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Time, lCommLine[2].c_str());
			m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Content, lCommLine[3].c_str());
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
	ON_BN_CLICKED(IDC_BTN_AddComment, &ZStuInfoDlg::OnBnClickedBtnAddcomment)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMMENTS, &ZStuInfoDlg::OnNMClickListComments)
	ON_BN_CLICKED(IDC_BTN_FINISH, &ZStuInfoDlg::OnBnClickedBtnFinish)
END_MESSAGE_MAP()


// ZStuInfoDlg message handlers

void ZStuInfoDlg::SetStudentInfo(ZStudentInfo* apStuInfo)
{
	if (apStuInfo)
		memcpy(&m_StuInfo, apStuInfo, sizeof(m_StuInfo));
	else
		memset(&m_StuInfo, 0, sizeof(m_StuInfo));

	if (m_StuInfo.Name[0] && m_StuInfo.Telehone[0])
	{
		m_OperateType = ZOT_Update;
	}
}

void ZStuInfoDlg::GetStuInfoFromDlg(ZStudentInfo* apStuInfo)
{
	ZStudentInfo& lStuInfo = *apStuInfo;
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

	CString lAddTime, lUpdateTime;
	GetDlgItemText(IDC_EDIT_ADDTIME, lAddTime);
	GetDlgItemText(IDC_EDIT_UPDATETIME, lUpdateTime);
	lStuInfo.InsertTime = ZConvStr2StdTime((char*)(LPCSTR)lAddTime);
	lStuInfo.UpdateTime = ZConvStr2StdTime((char*)(LPCSTR)lUpdateTime);

	CString lComments;
	for (int row = 0; row < m_ListComments.GetItemCount(); ++row)
	{
		CString lData;
		lData.Format("%s|%s|%s|%s",
			m_ListComments.GetItemText(row, COMMENTSLIST_COL_Row),
			m_ListComments.GetItemText(row, COMMENTSLIST_COL_ByName),
			m_ListComments.GetItemText(row, COMMENTSLIST_COL_Time),
			m_ListComments.GetItemText(row, COMMENTSLIST_COL_Content));

		lComments.Append(lData);
	}
	strncpy(lStuInfo.Comments, (char*)(LPCSTR)lComments, sizeof(lStuInfo.Comments) - 1);

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
	GetStuInfoFromDlg(&lStuInfo);
	if (!lStuInfo.Name[0] || !lStuInfo.Telehone[0])
	{
		AfxMessageBox(_T("用户名和电话为必填项"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (strchr(lStuInfo.Name, ':') || strchr(lStuInfo.QQ, ':') || strchr(lStuInfo.EMail, ':'))
	{
		AfxMessageBox(_T("用户名、QQ、邮箱中不能包含':'字符"), MB_OK | MB_ICONWARNING);
		return;
	}

	std::vector<ZStudentInfo*> lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareNameAndTel, 0);
	if (!lVec.empty() && !m_StuInfo.Name[0])
	{
		AfxMessageBox(_T("已存在该用户名和其电话，请检查"), MB_OK | MB_ICONWARNING);
		return;
	}

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

		// update (will write to db and network)
		g_MemData.AddOrUpdateStuInfo(ZMD_DATA_Update, &lStuInfo);
	}
	else
	{
		lStuInfo.InsertTime = time(0);

		// insert
		g_MemData.AddOrUpdateStuInfo(ZMD_DATA_Add, &lStuInfo);
	}
	memcpy(&m_StuInfo, &lStuInfo, sizeof(ZStudentInfo));

	// update to main list view
	g_pMainFrame->UpdateStuToMainListView(&lStuInfo, TRUE);

	AfxMessageBox(_T("添加或更新数据成功"), MB_OK | MB_ICONINFORMATION);

	// exit this dialog or show a message box
	//CDialogEx::OnOK();
}

/* 添加一行备注 */
void ZStuInfoDlg::OnBnClickedBtnAddcomment()
{
	CString lOneComment;
	GetDlgItemText(IDC_EDIT_COMMENT, lOneComment);
	if (lOneComment.IsEmpty())
	{
		AfxMessageBox(_T("添加备注不能为空"), MB_ICONWARNING);
		return;
	}

	if (lOneComment.Find("\r\n", 0) >= 0)
	{
		AfxMessageBox(_T("备注内容中不能输入多行内容，请检查"), MB_ICONERROR);
		return;
	}

	// auto append line feed
	lOneComment.Append("\r\n");
	if (lOneComment == m_LastOneComment)
	{
		AfxMessageBox(_T("未编辑新备注内容，不能添加"), MB_ICONWARNING);
		return;
	}

	if (lOneComment.Find('|', 0) >= 0)
	{
		AfxMessageBox(_T("备注内容不能包含字符: '|' ，请替换其它字符"), MB_ICONERROR);
		return;
	}
	m_LastOneComment = lOneComment;

	time_t lNow = time(0);
	std::string lTimeString = ZConvStdTimeStr(lNow);

	//int lRow = m_ListComments.GetItemCount();
	int lRow = 0;   // add to first row
	CString lRowString;
	lRowString.Format("%d", m_ListComments.GetItemCount() + 1);

	m_ListComments.InsertItem(lRow, _T(""));
	m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Row, lRowString);
	m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_ByName, g_AppConfig.m_UserID);
	m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Time,  lTimeString.c_str());
	m_ListComments.SetItemText(lRow, COMMENTSLIST_COL_Content, lOneComment);
}



void ZStuInfoDlg::OnNMClickListComments(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int lRow = pNMItemActivate->iItem;
	if (lRow > 0)
	{
		m_LastOneComment = m_ListComments.GetItemText(lRow, COMMENTSLIST_COL_Content);

		SetDlgItemText(IDC_EDIT_COMMENT, m_LastOneComment);
	}

	*pResult = 0;
}


void ZStuInfoDlg::OnBnClickedBtnFinish()
{
	ZStudentInfo lStuInfo;
	memset(&lStuInfo, 0, sizeof(lStuInfo));
	GetStuInfoFromDlg(&lStuInfo);

	if (!lStuInfo.Name[0] || !lStuInfo.Telehone[0])
	{
		AfxMessageBox(_T("用户名或电话为空"), MB_OK | MB_ICONWARNING);
		return;
	}

	std::vector<ZStudentInfo*> lVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareNameAndTel, 0);
	if (lVec.empty())
	{
		AfxMessageBox(_T("数据库中未找到该用户名和其电话信息，请检查"), MB_OK | MB_ICONWARNING);
		return;
	}

	lStuInfo.UpdateTime = time(0);
	lStuInfo.Flag |= ZSI_FLAG_Deleted;
	g_MemData.AddOrUpdateStuInfo(0, &lStuInfo, TRUE);
}
