// ZUserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "ZClientsManager.h"
#include "ZUserInfoDlg.h"
#include "ZInfoDesc.h"

static void _InitUserListCtrl(CListCtrl& aList)
{
	DWORD dwStyle;
	aList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = aList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	aList.SetExtendedStyle(dwStyle);

	aList.InsertColumn(USERLIST_COL_Row,        _T("编号"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(USERLIST_COL_Name,       _T("姓名"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(USERLIST_COL_Password,   _T("密码"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(USERLIST_COL_Telephone,  _T("电话"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(USERLIST_COL_QQ,         _T("QQ"),       LVCFMT_LEFT, 80);
	aList.InsertColumn(USERLIST_COL_Role,       _T("角色"),     LVCFMT_LEFT, 80);
}

static void _UpdateUserListCtrl(int aRow, CListCtrl& aList, ZUserInfo* apUserInfo)
{
	if (!apUserInfo)
		return;

	CString lString;
	if (aRow == -1)
	{
		aRow = aList.GetItemCount();
	}

	aList.InsertItem(aRow, _T(""));

	lString.Format("%d", aRow + 1);
	aList.SetItemText(aRow, USERLIST_COL_Row,      lString);
	aList.SetItemText(aRow, USERLIST_COL_Name,     apUserInfo->UserName);
	aList.SetItemText(aRow, USERLIST_COL_Password, apUserInfo->Password);
	aList.SetItemText(aRow, USERLIST_COL_Telephone,apUserInfo->Telephone);
	aList.SetItemText(aRow, USERLIST_COL_QQ,       apUserInfo->QQ);
	aList.SetItemText(aRow, USERLIST_COL_Role,     ZUserRoleDesc(apUserInfo->Role));
	
}


// ZUserInfoDlg dialog

IMPLEMENT_DYNAMIC(ZUserInfoDlg, CDialogEx)

ZUserInfoDlg::ZUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USERINFO, pParent)
{

}

ZUserInfoDlg::~ZUserInfoDlg()
{
}

void ZUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERINFO, m_UserList);
}

BOOL ZUserInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CListCtrl& lList = *(CListCtrl*)GetDlgItem(IDC_LIST_USERINFO);
	_InitUserListCtrl(lList);

	CComboBox* lpCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ROLE);
	lpCombo->InsertString(0, _T("超级管理员"));
	lpCombo->InsertString(0, _T("访问者"));
	lpCombo->InsertString(0, _T("普通用户"));
	lpCombo->InsertString(0, _T("普通管理员"));
	lpCombo->SetCurSel(0);

	// query db
	OnBnClickedBtnReset();

	return TRUE;
}


BEGIN_MESSAGE_MAP(ZUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &ZUserInfoDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_PASSWORD, &ZUserInfoDlg::OnBnClickedBtnPassword)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USERINFO, &ZUserInfoDlg::OnNMClickListUserinfo)
	ON_BN_CLICKED(IDC_BTN_FIND, &ZUserInfoDlg::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_RESET, &ZUserInfoDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// ZUserInfoDlg message handlers


void ZUserInfoDlg::OnBnClickedBtnSave()
{
	CString lName, lPassword, lTelephone, lQQ, lString;
	GetDlgItemText(IDC_EDIT_NAME, lName);
	GetDlgItemText(IDC_EDIT_Password, lPassword);
	GetDlgItemText(IDC_EDIT_TELEPHONE, lTelephone);
	GetDlgItemText(IDC_EDIT_QQ, lQQ);

	ZUserInfo lUserInfo = { 0 };
	strncpy(lUserInfo.UserName, (char*)(LPCSTR)lName, sizeof(lUserInfo.UserName) - 1);
	strncpy(lUserInfo.Password, (char*)(LPCSTR)lPassword, sizeof(lUserInfo.Password) - 1);
	strncpy(lUserInfo.Telephone, (char*)(LPCSTR)lTelephone, sizeof(lUserInfo.Telephone) - 1);
	strncpy(lUserInfo.QQ, (char*)(LPCSTR)lQQ, sizeof(lUserInfo.QQ) - 1);

	CComboBox* lpCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ROLE);
	int lSel = lpCombo->GetCurSel();
	switch (lSel)
	{
	case 0:
		lUserInfo.Role = ZUSER_ROLE_Admin;
		break;
	case 1:
		lUserInfo.Role = ZUSER_ROLE_Normal;
		break;
	case 2:
		lUserInfo.Role = ZUSER_ROLE_Visitor;
		break;
	case 3:
		lUserInfo.Role = ZUSER_ROLE_Root;
		break;
	default:
		lUserInfo.Role = ZUSER_ROLE_Normal;
		break;
	}

	if (IDYES !=  AfxMessageBox(_T("确定插入该用户数据"), MB_YESNOCANCEL))
	{
		return;
	}

	// update user info into db
	if (0 != g_MemData.GetUserDB()->Insert(&lUserInfo, sizeof(lUserInfo)))
	{
		AfxMessageBox(_T("插入账户数据失败"), MB_OK | MB_ICONWARNING);
		return;
	}

	_UpdateUserListCtrl(-1, m_UserList, &lUserInfo);

	g_MemData.AddUserInfo(&lUserInfo);
}


void ZUserInfoDlg::OnBnClickedBtnPassword()
{
	// TODO: Add your control notification handler code here
}


void ZUserInfoDlg::OnNMClickListUserinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int lRow = pNMItemActivate->iItem;
	if (lRow >= 0)
	{
		CString lName, lPassword, lTelephone, lQQ, lRole, lString;

		lName = m_UserList.GetItemText(lRow, USERLIST_COL_Name);
		lTelephone = m_UserList.GetItemText(lRow, USERLIST_COL_Telephone);
		lPassword = m_UserList.GetItemText(lRow, USERLIST_COL_Password);
		lQQ = m_UserList.GetItemText(lRow, USERLIST_COL_QQ);
		lRole = m_UserList.GetItemText(lRow, USERLIST_COL_Role);

		CComboBox* lpCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ROLE);
		if (lRole == _T("超级管理员")) {
			lpCombo->SetCurSel(3);
		}
		else if (lRole == _T("访问者")) {
			lpCombo->SetCurSel(2);
		}
		else if (lRole == _T("普通用户")) {
			lpCombo->SetCurSel(1);
		}
		else {
			lpCombo->SetCurSel(0);
		}

		SetDlgItemText(IDC_EDIT_NAME, lName);
		SetDlgItemText(IDC_EDIT_Password, lPassword);
		SetDlgItemText(IDC_EDIT_TELEPHONE, lTelephone);
		SetDlgItemText(IDC_EDIT_QQ, lQQ);
	}

	*pResult = 0;
}


void ZUserInfoDlg::OnBnClickedBtnFind()
{
	CString lName, lTelephone;
	GetDlgItemText(IDC_EDIT_NAME, lName);
	GetDlgItemText(IDC_EDIT_TELEPHONE, lTelephone);

	if (lName.IsEmpty() && lTelephone.IsEmpty())
	{
		AfxMessageBox(_T("请输入姓名或电话"), MB_OK | MB_ICONWARNING);
		return;
	}

	ZUserInfo lQryCond = {};
	if (!lName.IsEmpty())
		strncpy(lQryCond.UserName, (char*)(LPCSTR)lName, sizeof(lQryCond.UserName) - 1);
	if (!lTelephone.IsEmpty())
		strncpy(lQryCond.Telephone, (char*)(LPCSTR)lTelephone, sizeof(lQryCond.Telephone) - 1);

	vector<ZUserInfo*> lVec;
	lVec = g_MemData.QueryUserInfo(&lQryCond, ZQueryCompareUserName);
	if (lVec.empty())
	{
		AfxMessageBox(_T("未查询到相关账户信息"), MB_OK | MB_ICONWARNING);
		return;
	}

	UpdateUserInfoToDlg(lVec);
}

void ZUserInfoDlg::UpdateUserInfoToDlg(vector<ZUserInfo*>& aVec)
{
	m_UserList.DeleteAllItems();
	for (size_t i = 0; i < aVec.size(); ++i)
	{
		_UpdateUserListCtrl(i, m_UserList, aVec[i]);
	}
}



void ZUserInfoDlg::OnBnClickedBtnReset()
{
	vector<ZUserInfo*> lVec;
	lVec = g_MemData.QueryAllUser();
	UpdateUserInfoToDlg(lVec);
}
