#pragma once

#include "StudentInfoDB.h"
#include "ZCommon.h"
#include "afxwin.h"
#include "afxdtctl.h"


#define MAINLIST_COL_Row            0           // �����б���
#define MAINLIST_COL_Name           1           // ����
#define MAINLIST_COL_Telephone      2           // �绰
#define MAINLIST_COL_Country        3           // �������
#define MAINLIST_COL_CollegeFrom    4           // �Ͷ���ѧ
#define MAINLIST_COL_CollegeTo      5           // Ŀ���ѧ
#define MAINLIST_COL_MajorFrom      6           // �Ͷ�רҵ
#define MAINLIST_COL_MajorTo        7           // Ŀ��רҵ
#define MAINLIST_COL_Class          8           // �꼶
#define MAINLIST_COL_LangScore      9           // ����
#define MAINLIST_COL_GPA            10          // GPA
#define MAINLIST_COL_Status         11          // ״̬
#define MAINLIST_COL_Important      12          // ��Ҫ����
#define MAINLIST_COL_NextVisitTime  13          // �´λط�ʱ��
#define MAINLIST_COL_Sex            14          // �Ա�
#define MAINLIST_COL_QQ             15          // QQ
#define MAINLIST_COL_Source         16          // ��Դ
#define MAINLIST_COL_InsertTime     17
#define MAINLIST_COL_UpdateTime     18
#define MAINLIST_COL_EMail          19

typedef struct 
{
	uint16_t   Col;
	uint16_t   Width;
	char*      Desc;
}MainListColDesc;


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

	CComboBox           m_comboImportant;
	CDateTimeCtrl       m_DatePick;
	CDateTimeCtrl       m_TimePick;

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
