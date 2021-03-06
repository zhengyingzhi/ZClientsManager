#pragma once


#include <afxcview.h>

#include "StudentInfoDB.h"
#include "ZStuInfoDlg.h"
#include "ZUserInfoDlg.h"


// CMainListView view

class CMainListView : public CListView
{
	DECLARE_DYNCREATE(CMainListView)

protected:
	CMainListView();           // protected constructor used by dynamic creation
	virtual ~CMainListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void UpdateStuToListView(vector<ZStudentInfo*>& aStuVec, BOOL aAppend);

protected:
	CListCtrl& m_list;
	int m_Row;
	int m_Col;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditModify();
	afx_msg void OnEditDelete();
	afx_msg void OnEditResetmain();
	afx_msg void OnEditExport();
};


