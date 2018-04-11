#pragma once

#include "UserInfoDB.h"
#include "ZCommon.h"


// ZUserInfoDlg dialog

class ZUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZUserInfoDlg)

public:
	ZUserInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZUserInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USERINFO };
#endif

	ZEnumOperateType    m_OperateType;   // ²Ù×÷ÀàÐÍ

public:
	void SetOperateType(ZEnumOperateType aOperateType) { m_OperateType = aOperateType; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};
