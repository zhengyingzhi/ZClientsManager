// MainListView.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "MainListView.h"

#include "ZInfoDesc.h"
#include "ZUtility.h"


//////////////////////////////////////////////////////////////////////////
// list control operates
static void _InitMainListCtrl(CListCtrl& aList)
{
	DWORD dwStyle;
	aList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = aList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	aList.SetExtendedStyle(dwStyle);

	aList.InsertColumn(MAINLIST_COL_Row,        _T("���"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_Name,       _T("����"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Telephone,  _T("�绰"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Country,    _T("�������"), LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_College,    _T("ѧУ"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Major,      _T("רҵ"),     LVCFMT_LEFT, 100);
	aList.InsertColumn(MAINLIST_COL_Class,      _T("�꼶"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_LangScore,  _T("���Գɼ�"), LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Sex,        _T("�Ա�"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_QQ,         _T("QQ"),       LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Source,     _T("��Դ"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Status,     _T("״̬"),     LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_InsertTime, _T("����ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_UpdateTime, _T("����ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_NextVisitTime, _T("�ط�ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_Important,  _T("��Ҫ����"), LVCFMT_LEFT, 80);
}

static void _UpdateMainListCtrl(int aRow, CListCtrl& aList, ZStudentInfo* apStuInfo)
{
	CString lString;
	if (aRow == -1)
	{
		aRow = aList.GetItemCount();
	}

	aList.InsertItem(aRow, _T(""));

	lString.Format("%d", aRow);
	aList.SetItemText(aRow, MAINLIST_COL_Row,      lString);
	aList.SetItemText(aRow, MAINLIST_COL_Name,     apStuInfo->Name);
	aList.SetItemText(aRow, MAINLIST_COL_Telephone,apStuInfo->Telehone);
	aList.SetItemText(aRow, MAINLIST_COL_Country,  apStuInfo->Country);
	aList.SetItemText(aRow, MAINLIST_COL_College,  apStuInfo->College);
	aList.SetItemText(aRow, MAINLIST_COL_Major,    apStuInfo->Major);
	aList.SetItemText(aRow, MAINLIST_COL_Class,    apStuInfo->Class);

	lString.Format("%.1lf", apStuInfo->LanguageScore);
	aList.SetItemText(aRow, MAINLIST_COL_LangScore, lString);

	aList.SetItemText(aRow, MAINLIST_COL_Sex,      ZStuSexDesc(apStuInfo->Sex));
	aList.SetItemText(aRow, MAINLIST_COL_QQ,       apStuInfo->QQ);
	aList.SetItemText(aRow, MAINLIST_COL_Source,   apStuInfo->Source);
	aList.SetItemText(aRow, MAINLIST_COL_Status,   apStuInfo->Status);

	aList.SetItemText(aRow, MAINLIST_COL_InsertTime, ZConvDateTimeStr(apStuInfo->InsertTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_UpdateTime, ZConvDateTimeStr(apStuInfo->UpdateTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_NextVisitTime, ZConvDateTimeStr(apStuInfo->NextVisitTime).c_str());
	aList.SetItemText(aRow, MAINLIST_COL_Important, ZStuImportantDesc(apStuInfo->ImportantLevel));

}


// CMainListView

IMPLEMENT_DYNCREATE(CMainListView, CListView)

CMainListView::CMainListView()
	: m_list(GetListCtrl())
{

}

CMainListView::~CMainListView()
{
}

BEGIN_MESSAGE_MAP(CMainListView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMainListView::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMainListView::OnNMRClick)
	ON_COMMAND(ID_EDIT_MODIFY, &CMainListView::OnEditModify)
END_MESSAGE_MAP()


// CMainListView diagnostics

#ifdef _DEBUG
void CMainListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainListView message handlers


void CMainListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	_InitMainListCtrl(m_list);

	// query students data from db
	if (g_MemData.GetStuDB() == NULL)
	{
		if (g_MemData.OpenStuDB(STUINFO_DB_DEFAULT_NAME, "127.0.0.1", 0) != 0)
		{
			AfxMessageBox(_T("��ѧ�����ݿ���Ϣʧ��"), MB_OK | MB_ICONWARNING);
			return;
		}
	}

	vector<ZStudentInfo*> lVec;
	lVec = g_MemData.QueryAllStudents();
	for (size_t i = 0; i < lVec.size(); ++i)
	{
		ZStudentInfo* lpStuInfo = lVec[i];
		_UpdateMainListCtrl(0, m_list, lpStuInfo);
	}

}


void CMainListView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int lRow = pNMItemActivate->iItem;

	*pResult = 0;
}


void CMainListView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int lRow = pNMItemActivate->iItem;
	if (lRow >= 0)
	{
		//��������Ҫ�õ��� cmenu ����
		CMenu menu, *pSubMenu;

		//װ���Զ�����Ҽ��˵�
		menu.LoadMenu(IDR_POPUP_EDIT);

		//��ȡ��һ�������˵�,���Ե�һ���˵��������� �˵�
		pSubMenu = menu.GetSubMenu(0);

		//����һ������ȷ�����λ�õ�λ��
		CPoint lPoint;

		//��ȡ��ǰ����λ��,�Ա�ʹ�ò˵��ܹ�������
		GetCursorPos(&lPoint);

		//�� istat ��ŵ�ǰѡ�����ǵڼ���
		//int istat = m_list.GetSelectionMark();

		//��ȡ��ǰ���е�����, 0�����ǵ� 0�� // pString="��ѡ������� :"+pString ;//��ʾ��ǰѡ����
		//CString pString = m_list.GetItemText(istat, 0);
		// MessageBox(pString);//��ʾ��ǰѡ�е�·��

		//��ָ�� λ����ʾ�����˵�
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, lPoint.x, lPoint.y, this);
	}

	*pResult = 0;
}


void CMainListView::OnEditModify()
{
	// TODO: ��ȡ�������ݣ�������Dialog�У�������Ӧ����
	int lRow = m_list.GetSelectionMark();

	ZStuInfoDlg lSIDlg;
	lSIDlg.DoModal();
}
