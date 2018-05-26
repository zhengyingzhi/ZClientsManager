#pragma once

#include "StudentInfo.h"
#include "afxdtctl.h"

// ZStuQryDlg dialog

class CMainFrame;

class ZStuQryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZStuQryDlg)

public:
	ZStuQryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZStuQryDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QUERY };
#endif

public:
	void SetStuInfo(const ZStudentInfo& aStuInfo);
	void SetMainFrame(CMainFrame* apMainFrame);

public:
	CMainFrame*    m_pMainFrame;
	ZStudentInfo   m_StuInfo;
	CDateTimeCtrl  m_DTCtrl;
	BOOL           m_DateTimeChanged;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnDtnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
};
