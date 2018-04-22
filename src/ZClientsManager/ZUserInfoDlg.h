#pragma once

#include "UserInfoDB.h"
#include "ZCommon.h"
#include "afxcmn.h"


#define USERLIST_COL_Row            0           // �����б���
#define USERLIST_COL_Name           1           // ����
#define USERLIST_COL_Password       2           // ����
#define USERLIST_COL_Telephone      3           // �绰
#define USERLIST_COL_QQ             4           // QQ
#define USERLIST_COL_Role           5           // Ȩ��
#define USERLIST_COL_InsertTime     6           // ����ʱ��
#define USERLIST_COL_UpdateTime     7           // ����ʱ��
#define USERLIST_COL_Status         8           // ״̬���Ƿ���ɾ����
#define USERLIST_COL_IDNumber       9           // ���֤��


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

	ZEnumOperateType    m_OperateType;   // ��������
	CListCtrl m_UserList;

public:
	void SetOperateType(ZEnumOperateType aOperateType) { m_OperateType = aOperateType; }

	void UpdateUserInfoToDlg(vector<ZUserInfo*>& aVec, BOOL aAppend);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnPassword();
	afx_msg void OnNMClickListUserinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnReset();
};
