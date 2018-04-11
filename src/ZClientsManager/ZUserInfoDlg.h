#pragma once

#include "UserInfoDB.h"
#include "ZCommon.h"


#define USERLIST_COL_Row            0           // 仅是列表编号
#define USERLIST_COL_Name           1           // 名字
#define USERLIST_COL_Password       2           // 密码
#define USERLIST_COL_Telephone      3           // 电话
#define USERLIST_COL_Role           4           // 权限
#define USERLIST_COL_InsertTime     5           // 插入时间
#define USERLIST_COL_UpdateTime     6           // 更新时间
#define USERLIST_COL_Status         7           // 状态（是否已删除）


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

	ZEnumOperateType    m_OperateType;   // 操作类型

public:
	void SetOperateType(ZEnumOperateType aOperateType) { m_OperateType = aOperateType; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnPassword();
	afx_msg void OnNMClickListUserinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
