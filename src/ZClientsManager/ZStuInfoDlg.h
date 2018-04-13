#pragma once

#include "StudentInfoDB.h"
#include "ZCommon.h"


#define MAINLIST_COL_Row            0           // 仅是列表编号
#define MAINLIST_COL_Name           1           // 名字
#define MAINLIST_COL_Telephone      2           // 电话
#define MAINLIST_COL_Country        3           // 意向国家
#define MAINLIST_COL_College        4           // 大学
#define MAINLIST_COL_Major          5           // 专业
#define MAINLIST_COL_Class          6           // 年级
#define MAINLIST_COL_LangScore      7           // 分数
#define MAINLIST_COL_Sex            8           // 性别
#define MAINLIST_COL_QQ             9           // QQ
#define MAINLIST_COL_Status         10          // 状态
#define MAINLIST_COL_Important      11          // 重要级别
#define MAINLIST_COL_NextVisitTime  12          // 下次回访时间
#define MAINLIST_COL_InsertTime     13          // 插入时间
#define MAINLIST_COL_UpdateTime     14          // 更新时间
#define MAINLIST_COL_Source         15          // 来源


// ZStuInfoDlg dialog

class ZStuInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZStuInfoDlg)

public:
	ZStuInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZStuInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STUINFO };
#endif

	ZEnumOperateType    m_OperateType;   // 操作类型
	ZStudentInfo        m_StuInfo;

public:
	void SetOperateType(ZEnumOperateType aOperateType) { m_OperateType = aOperateType; }

	void SetStudentInfo(ZStudentInfo* apStuInfo);

	void GetDlgItemValue(int nID, char aBuffer[], int aBufSize);
	void GetDlgItemValue(int nID, int32_t& aValue);
	void GetDlgItemValue(int nID, uint32_t& aValue);
	void GetDlgItemValue(int nID, int64_t& aValue);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};
