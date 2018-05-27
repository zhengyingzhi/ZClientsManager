// ZTaskManager.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "ZTaskManager.h"
#include "afxdialogex.h"

#include "ZNetwork.h"

#include <time.h>

#include <ZToolLib/ztl_utils.h>

#define TASKLIST_COL_Time       0
#define TASKLIST_COL_Status     2
#define TASKLIST_COL_Name       3
#define TASKLIST_COL_Content    4
#define TASKLIST_COL_TimeInt    5

// ZTaskManager dialog
extern ZNetCommBase*	g_pNetComm;

static void _NetLoopOnce(void* apUserData)
{
	ZTaskManager* lpTaskMgr = (ZTaskManager*)apUserData;
	if (lpTaskMgr)
		lpTaskMgr->TryExpireTimers();
}

void ZTaskManager::TryExpireTimers()
{
	// 有线程安全问题，但不考虑
	time_t lNow = time(0);
	ZTaskInfo* lpTask;
	for (size_t i = 0; i < m_CacheTasks.size(); ++i)
	{
		lpTask = m_CacheTasks[i];
		if (lNow > lpTask->TaskTime && lpTask->TaskTime)
		{
			CString lNote;
			lNote.Format(_T("%ld\n%s\n%s"), (long)lpTask->TaskTime,
				lpTask->TaskName, lpTask->TaskContent);
			AfxMessageBox(lNote, MB_OK);

			// mark as deleted
			lpTask->Flag = 1;
		}
	}
}

void ZTaskManager::ShowOnListCtrl(ZTaskInfo* apTaskInfo)
{
	int lRow = m_ListTask.GetItemCount();
	m_ListTask.InsertItem(lRow, "");

	time_t lNow = time(0);
	CString lDateTime;
	struct tm* ptm = localtime((time_t*)&apTaskInfo->TaskTime);

	lDateTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	CString lStrExpired = apTaskInfo->TaskTime > lNow ? "未到期" : "到期";
	CString lTimeIntr;
	lTimeIntr.Format("%ld", (long)apTaskInfo->TaskTime);

	m_ListTask.SetItemText(lRow, TASKLIST_COL_Time, lDateTime);
	m_ListTask.SetItemText(lRow, TASKLIST_COL_Status, lStrExpired);
	m_ListTask.SetItemText(lRow, TASKLIST_COL_Name, apTaskInfo->TaskName);
	m_ListTask.SetItemText(lRow, TASKLIST_COL_Content, apTaskInfo->TaskContent);
	m_ListTask.SetItemText(lRow, TASKLIST_COL_TimeInt, lTimeIntr);
}

IMPLEMENT_DYNAMIC(ZTaskManager, CDialogEx)

ZTaskManager::ZTaskManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Reminder, pParent)
	, m_pTaskDB()
	, m_pThread()
{
	if (g_pNetComm)
	{
		g_pNetComm->SetLoopOnceFunc(_NetLoopOnce, this);
	}
}

ZTaskManager::~ZTaskManager()
{
}

void ZTaskManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASKS, m_ListTask);
	DDX_Control(pDX, IDC_DTPICKER_Date, m_DateCtrl);
	DDX_Control(pDX, IDC_DTPICKER_Time, m_TimeCtrl);
}

BOOL ZTaskManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle;
	m_ListTask.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = m_ListTask.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	m_ListTask.SetExtendedStyle(dwStyle);
	m_ListTask.InsertColumn(TASKLIST_COL_Time, _T("时间"), LVCFMT_LEFT, 130);
	m_ListTask.InsertColumn(TASKLIST_COL_Status, _T("状态"), LVCFMT_LEFT, 60);
	m_ListTask.InsertColumn(TASKLIST_COL_Name, _T("名称"), LVCFMT_LEFT, 80);
	m_ListTask.InsertColumn(TASKLIST_COL_Content, _T("内容"), LVCFMT_LEFT, 200);
	m_ListTask.InsertColumn(TASKLIST_COL_Content, _T("Intr"), LVCFMT_LEFT, 60);

	m_SelectedRow = -1;

	// TODO: load task from file
	if (!InitTaskDB())
	{
		AfxMessageBox(_T("打开任务管理数据库失败"), MB_ICONERROR);
		return TRUE;
	}

	if (!m_CacheTasks.empty())
	{
		uint32_t lCount = 0;
		time_t lNow = time(0);
		ZTaskInfo* lpCurTask;
		for (size_t i = 0; i < m_CacheTasks.size(); ++i)
		{
			lpCurTask = m_CacheTasks[i];
			if (lNow > lpCurTask->TaskTime)
			{
				lCount++;
			}

			// show on dialog
			ShowOnListCtrl(lpCurTask);
		}

		if (lCount > 0)
		{
			CString lNote;
			lNote.Format(_T("您有 [%d] 个定时任务到期，请查看"), lCount);
			AfxMessageBox(lNote, MB_ICONWARNING);
		}
	}

	return TRUE;
}

BOOL ZTaskManager::InitTaskDB()
{
	int rv;
	if (m_pTaskDB == NULL)
	{
		m_pTaskDB = new ZTaskInfoDBText();
		rv = m_pTaskDB->Open(TASKINFO_DB_DEFAULT_NAME, "", 0);
		if (rv != 0)
		{
			return FALSE;
		}
	}
	else
	{
		// no need query any more
		return TRUE;
	}

	ClearCaches();

	ZTaskInfo lTaskInfo = { 0 };
	ZQueryResult* lpQryRs = m_pTaskDB->Query(&lTaskInfo, ZQueryCompareTaskAll, 0);

	if (lpQryRs)
	{
		ZTaskInfo *lpSrcData, *lpDstData;
		for (uint32_t i = 0; i < lpQryRs->RsCount(); ++i)
		{
			lpSrcData = lpQryRs->RsAtAsType<ZTaskInfo>(i);
			if (!lpSrcData)
			{
				continue;
			}

			lpDstData = (ZTaskInfo*)malloc(sizeof(ZTaskInfo));
			memcpy(lpDstData, lpSrcData, sizeof(ZTaskInfo));
			m_CacheTasks.push_back(lpDstData);
		}

		m_pTaskDB->FreeQueryRs(lpQryRs);
	}

	return TRUE;
}

void ZTaskManager::ClearCaches()
{
	for (size_t i = 0; i < m_CacheTasks.size(); ++i)
	{
		free(m_CacheTasks[i]);
	}
	m_CacheTasks.clear();
}

void ZTaskManager::EraseOneTask(const ZTaskInfo* apTaskInfo)
{
	if (m_CacheTasks.empty())
	{
		return;
	}

	std::vector<ZTaskInfo*>::iterator iter = m_CacheTasks.begin();
	for (; iter != m_CacheTasks.end(); ++iter)
	{
		ZTaskInfo* lpTask = *iter;
		if (lpTask->TaskTime == apTaskInfo->TaskTime)
		{
			m_CacheTasks.erase(iter);
			free(lpTask);
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(ZTaskManager, CDialogEx)
	ON_BN_CLICKED(IDOK, &ZTaskManager::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TASKS, &ZTaskManager::OnNMClickListTasks)
	ON_BN_CLICKED(IDC_BTN_DELTASK, &ZTaskManager::OnBnClickedBtnDeltask)
END_MESSAGE_MAP()


// ZTaskManager message handlers


void ZTaskManager::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString lName, lContent;
	GetDlgItemText(IDC_EDIT_TASKNAME, lName);
	GetDlgItemText(IDC_EDIT_TASKCONTENT, lContent);

	if (lName.IsEmpty() || lContent.IsEmpty())
	{
		AfxMessageBox(_T("名称或内容为空"), MB_ICONWARNING);
		return;
	}

	struct tm ltm = { 0 };

	CTime lDate, lTime;
	m_DateCtrl.GetTime(lDate);
	m_TimeCtrl.GetTime(lTime);

	ltm.tm_year = lDate.GetYear() - 1900;
	ltm.tm_mon = lDate.GetMonth() - 1;
	ltm.tm_mday = lDate.GetDay();
	ltm.tm_hour = lTime.GetHour();
	ltm.tm_min = lTime.GetMinute();
	ltm.tm_sec = lTime.GetSecond();

	time_t lTheTime = mktime(&ltm);

	time_t lNow = time(0);
	if (lTheTime < lNow + 60)
	{
		AfxMessageBox(_T("请选择1分钟后的时间"), MB_ICONWARNING);
		return;
	}

	ZTaskInfo* lpTaskInfo = (ZTaskInfo*)calloc(1, sizeof(ZTaskInfo));
	lpTaskInfo->TaskTime = (uint32_t)lTheTime;
	strncpy(lpTaskInfo->TaskName, (char*)(LPCSTR)lName, sizeof(lpTaskInfo->TaskName) - 1);
	strncpy(lpTaskInfo->TaskContent, (char*)(LPCSTR)lContent, sizeof(lpTaskInfo->TaskContent) - 1);
	m_CacheTasks.push_back(lpTaskInfo);

	m_pTaskDB->Insert(lpTaskInfo, sizeof(ZTaskInfo));

	ShowOnListCtrl(lpTaskInfo);

	// CDialogEx::OnOK();
}


void ZTaskManager::OnNMClickListTasks(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_SelectedRow = pNMItemActivate->iItem;
	if (m_SelectedRow >= 0)
	{
		CString lTaskName, lTaskContent, lTimeIntr;

		time_t lTheTime = atol((char*)(LPCSTR)lTimeIntr);
		CTime lCTm(lTheTime);
		m_DateCtrl.SetTime(&lCTm);
		m_TimeCtrl.SetTime(&lCTm);

		lTaskName = m_ListTask.GetItemText(m_SelectedRow, TASKLIST_COL_Name);
		lTaskContent = m_ListTask.GetItemText(m_SelectedRow, TASKLIST_COL_Content);

		SetDlgItemText(IDC_EDIT_TASKNAME, lTaskName);
		SetDlgItemText(IDC_EDIT_TASKCONTENT, lTaskContent);
	}

	*pResult = 0;
}


void ZTaskManager::OnBnClickedBtnDeltask()
{
	if (m_SelectedRow < 0)
	{
		AfxMessageBox(_T("请选择待删除的任务"), MB_OK);
		return;
	}

	if (m_SelectedRow >= m_ListTask.GetItemCount())
	{
		return;
	}

	CString lTaskName, lNote, lTimeIntr;
	m_ListTask.GetItemText(m_SelectedRow, TASKLIST_COL_Name);
	m_ListTask.GetItemText(m_SelectedRow, TASKLIST_COL_TimeInt);
	lNote.Format(_T("是否删除该任务\n%s"), lTaskName);
	int rv = AfxMessageBox(_T("请选择待删除的任务"), MB_YESNOCANCEL);
	if (rv == IDOK)
	{
		// remove from db and cache
		ZTaskInfo lTaskInfo = { 0 };
		lTaskInfo.TaskTime = atol((char*)(LPCSTR)lTimeIntr);
		m_pTaskDB->Delete(&lTaskInfo, sizeof(lTaskInfo));

		EraseOneTask(&lTaskInfo);

		SetDlgItemText(IDC_EDIT_TASKNAME, "");
		SetDlgItemText(IDC_EDIT_TASKCONTENT, "");

		m_ListTask.DeleteItem(m_SelectedRow);
	}
}
