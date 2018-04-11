#pragma once

#include "StudentInfoDB.h"
#include "ZCommon.h"


#define MAINLIST_COL_Row            0           // �����б���
#define MAINLIST_COL_Name           1           // ����
#define MAINLIST_COL_Telephone      2           // �绰
#define MAINLIST_COL_Country        3           // �������
#define MAINLIST_COL_College        4           // ��ѧ
#define MAINLIST_COL_Major          5           // רҵ
#define MAINLIST_COL_Class          6           // �꼶
#define MAINLIST_COL_LangScore      7           // ����
#define MAINLIST_COL_Sex            8           // �Ա�
#define MAINLIST_COL_QQ             9           // QQ
#define MAINLIST_COL_Source         10          // ��Դ
#define MAINLIST_COL_Status         11          // ״̬
#define MAINLIST_COL_InsertTime     12          // ����ʱ��
#define MAINLIST_COL_UpdateTime     13          // ����ʱ��
#define MAINLIST_COL_NextVisitTime  14          // �´λط�ʱ��
#define MAINLIST_COL_Important      15          // ��Ҫ����


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

	ZEnumOperateType    m_OperateType;   // ��������
	ZStudentInfo        m_StuInfo;

public:
	void SetOperateType(ZEnumOperateType aOperateType) { m_OperateType = aOperateType; }

	void SetStudentInfo(ZStudentInfo* apStuInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};
