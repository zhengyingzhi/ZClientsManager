// MainListView.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "MainListView.h"

#include "ZInfoDesc.h"
#include "ZUtility.h"

/* database object */
ZStudentInfoDB* g_pDB = NULL;

//////////////////////////////////////////////////////////////////////////
// list control operates
static void _InitMainListCtrl(CListCtrl& aList)
{
	DWORD dwStyle;
	aList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = aList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	aList.SetExtendedStyle(dwStyle);

	aList.InsertColumn(MAINLIST_COL_Row,        _T("编号"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_Name,       _T("姓名"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Telephone,  _T("电话"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Country,    _T("意向国家"), LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_College,    _T("学校"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Major,      _T("专业"),     LVCFMT_LEFT, 100);
	aList.InsertColumn(MAINLIST_COL_Class,      _T("年级"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_LangScore,  _T("语言成绩"), LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Sex,        _T("性别"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_QQ,         _T("QQ"),       LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Source,     _T("来源"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Status,     _T("状态"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_InsertTime, _T("插入时间"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_UpdateTime, _T("更新时间"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_NextVisitTime, _T("回访时间"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_Important,  _T("重要级别"), LVCFMT_LEFT, 80);
}

static void _UpdateMainListCtrl(int aRow, CListCtrl& aList, ZStudentInfo* apStuInfo)
{
	CString lString;
	if (aRow == -1)
	{
		aRow = aList.GetItemCount();
	}

	aList.InsertItem(aRow, _T(""));

	lString.Format("%d", aRow);
	aList.SetItemText(aRow, MAINLIST_COL_Row,      lString);
	aList.SetItemText(aRow, MAINLIST_COL_Name,     apStuInfo->Name);
	aList.SetItemText(aRow, MAINLIST_COL_Telephone,apStuInfo->Telehone);
	aList.SetItemText(aRow, MAINLIST_COL_Country,  apStuInfo->Country);
	aList.SetItemText(aRow, MAINLIST_COL_College,  apStuInfo->College);
	aList.SetItemText(aRow, MAINLIST_COL_Major,    apStuInfo->Major);
	aList.SetItemText(aRow, MAINLIST_COL_Class,    apStuInfo->Class);

	lString.Format("%.1lf", apStuInfo->LanguageScore);
	aList.SetItemText(aRow, MAINLIST_COL_LangScore, lString);

	aList.SetItemText(aRow, MAINLIST_COL_Sex,      ZStuSexDesc(apStuInfo->Sex));
	aList.SetItemText(aRow, MAINLIST_COL_QQ,       apStuInfo->QQ);
	aList.SetItemText(aRow, MAINLIST_COL_Source,   apStuInfo->Source);
	aList.SetItemText(aRow, MAINLIST_COL_Status,   apStuInfo->Status);

	aList.SetItemText(aRow, MAINLIST_COL_InsertTime, ZConvDateTimeStr(apStuInfo->InsertTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_UpdateTime, ZConvDateTimeStr(apStuInfo->UpdateTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_NextVisitTime, ZConvDateTimeStr(apStuInfo->NextVisitTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_Important, ZStuImportantDesc(apStuInfo->ImportantLevel));

}


// CMainListView

IMPLEMENT_DYNCREATE(CMainListView, CListView)

CMainListView::CMainListView()
	: m_list(GetListCtrl())
{

}

CMainListView::~CMainListView()
{
}

BEGIN_MESSAGE_MAP(CMainListView, CListView)
END_MESSAGE_MAP()


// CMainListView diagnostics

#ifdef _DEBUG
void CMainListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainListView message handlers


void CMainListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	_InitMainListCtrl(m_list);
}
