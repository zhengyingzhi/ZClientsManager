#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"

#include "TaskDB.h"

#include <thread>
#include <vector>

class ZTaskInfoDBText;

// ZTaskManager dialog

class ZTaskManager : public CDialogEx
{
	DECLARE_DYNAMIC(ZTaskManager)

public:
	ZTaskManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZTaskManager();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Reminder };
#endif

public:
	void TryExpireTimers();
	void ShowOnListCtrl(ZTaskInfo* apTaskInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	BOOL InitTaskDB();
	void ClearCaches();
	void EraseOneTask(const ZTaskInfo* apTaskInfo);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnDeltask();
	afx_msg void OnNMClickListTasks(NMHDR *pNMHDR, LRESULT *pResult);

	int m_SelectedRow;
	CListCtrl m_ListTask;
	CDateTimeCtrl m_DateCtrl;
	CDateTimeCtrl m_TimeCtrl;
	ZTaskInfoDBText* m_pTaskDB;
	std::unique_ptr<std::thread> m_pThread;
	std::vector<ZTaskInfo*> m_CacheTasks;
};
