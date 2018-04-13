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
	void UpdateStuToListView(vector<ZStudentInfo*>& aStuVec);

protected:
	CListCtrl& m_list;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditModify();
};


