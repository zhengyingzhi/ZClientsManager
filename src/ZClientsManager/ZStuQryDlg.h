#pragma once

#include "StudentInfo.h"

// ZStuQryDlg dialog

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

public:
	ZStudentInfo   m_StuInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnBnClickedBtnClear();
};
