// MainListView.cpp : implementation file
//

#include "stdafx.h"
#include "ZClientsManager.h"
#include "MainListView.h"

#include "ZInfoDesc.h"
#include "ZUtility.h"


//////////////////////////////////////////////////////////////////////////
// list control operates
MainListColDesc MLColums[] = {
	{ MAINLIST_COL_Row,         40,  _T("���") },
	{ MAINLIST_COL_Name,        60,  _T("����") },
	{ MAINLIST_COL_Telephone,   100, _T("�绰") },
	{ MAINLIST_COL_Country,     80,  _T("����") },
	{ MAINLIST_COL_CollegeFrom, 110, _T("�Ͷ���ѧ") },
	{ MAINLIST_COL_CollegeTo,   110, _T("Ŀ���ѧ") },
	{ MAINLIST_COL_MajorFrom,   140, _T("�Ͷ�רҵ") },
	{ MAINLIST_COL_MajorTo,     140, _T("Ŀ��רҵ") },
	{ MAINLIST_COL_Class,       60,  _T("�꼶") },
	{ MAINLIST_COL_LangScore,   60,  _T("����") },
	{ MAINLIST_COL_GPA,         60,  _T("GPA") },
	{ MAINLIST_COL_Status,      100, _T("״̬") },
	{ MAINLIST_COL_Important,   60,  _T("��Ҫ����") },
	{ MAINLIST_COL_NextVisitTime, 130, _T("�ط�ʱ��") },
	{ MAINLIST_COL_Sex,         40,  _T("�Ա�") },
	{ MAINLIST_COL_QQ,          80,  _T("QQ") },
	{ MAINLIST_COL_Source,      160, _T("��Դ") },
	{ MAINLIST_COL_InsertTime,  130, _T("����ʱ��") },
	{ MAINLIST_COL_UpdateTime,  130, _T("����ʱ��") },
	{ MAINLIST_COL_EMail,       120, _T("����") },
	{ 0, 0, 0 }
};

/* ��ʼ������Ϣ */
static void _InitMainListCtrl(CListCtrl& aList)
{
	DWORD dwStyle;
	aList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	dwStyle = aList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	aList.SetExtendedStyle(dwStyle);

#if 0
	aList.InsertColumn(MAINLIST_COL_Row,        _T("���"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_Name,       _T("����"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Telephone,  _T("�绰"),     LVCFMT_LEFT, 100);
	aList.InsertColumn(MAINLIST_COL_Country,    _T("�������"), LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_College,    _T("ѧУ"),     LVCFMT_LEFT, 110);
	aList.InsertColumn(MAINLIST_COL_Major,      _T("רҵ"),     LVCFMT_LEFT, 140);
	aList.InsertColumn(MAINLIST_COL_Class,      _T("�꼶"),     LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_LangScore,  _T("���Գɼ�"), LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_Sex,        _T("�Ա�"),     LVCFMT_LEFT, 40);
	aList.InsertColumn(MAINLIST_COL_QQ,         _T("QQ"),       LVCFMT_LEFT, 80);
	aList.InsertColumn(MAINLIST_COL_Status,     _T("״̬"),     LVCFMT_LEFT, 100);
	aList.InsertColumn(MAINLIST_COL_Important,  _T("��Ҫ����"), LVCFMT_LEFT, 60);
	aList.InsertColumn(MAINLIST_COL_NextVisitTime, _T("�ط�ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_InsertTime, _T("����ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_UpdateTime, _T("����ʱ��"), LVCFMT_LEFT, 120);
	aList.InsertColumn(MAINLIST_COL_Source,     _T("��Դ"),     LVCFMT_LEFT, 160);
#else
	for (uint32_t i = 0; MLColums[i].Desc; ++i)
	{
		aList.InsertColumn(MLColums[i].Col, MLColums[i].Desc, LVCFMT_LEFT, MLColums[i].Width);
	}
#endif//0
}

/* ������Ϣ��ָ������ */
static void _UpdateMainListCtrl(int aRow, CListCtrl& aList, ZStudentInfo* apStuInfo)
{
	CString lString;
	std::string lStdString;
	if (aRow == -1)
	{
		aRow = aList.GetItemCount();
	}

	aList.InsertItem(aRow, _T(""));

	//lString.Format("%d", aRow + 1);
	lString.Format("%d", apStuInfo->Number);
	aList.SetItemText(aRow, MAINLIST_COL_Row,         lString);
	aList.SetItemText(aRow, MAINLIST_COL_Name,        apStuInfo->Name);
	aList.SetItemText(aRow, MAINLIST_COL_Telephone,   apStuInfo->Telehone);
	aList.SetItemText(aRow, MAINLIST_COL_Country,     apStuInfo->Country);
	aList.SetItemText(aRow, MAINLIST_COL_CollegeFrom, apStuInfo->CollegeFrom);
	aList.SetItemText(aRow, MAINLIST_COL_CollegeTo,   apStuInfo->CollegeTo);
	aList.SetItemText(aRow, MAINLIST_COL_MajorFrom,   apStuInfo->MajorFrom);
	aList.SetItemText(aRow, MAINLIST_COL_MajorTo,     apStuInfo->MajorTo);
	aList.SetItemText(aRow, MAINLIST_COL_Class,       apStuInfo->Class);

	lString.Format("%.1lf", double(apStuInfo->LanguageScore / 10.0));
	aList.SetItemText(aRow, MAINLIST_COL_LangScore, lString);
	lString.Format("%.1lf", double(apStuInfo->GPA / 10.0));
	aList.SetItemText(aRow, MAINLIST_COL_GPA, lString);

	aList.SetItemText(aRow, MAINLIST_COL_Status,   apStuInfo->Status);
	aList.SetItemText(aRow, MAINLIST_COL_Important, ZStuImportantDesc(apStuInfo->ImportantLevel));

	lStdString = ZConvStdTimeStr(apStuInfo->NextVisitTime);
	aList.SetItemText(aRow, MAINLIST_COL_NextVisitTime, lStdString.c_str());

	aList.SetItemText(aRow, MAINLIST_COL_Sex, ZStuSexDesc(apStuInfo->Sex));
	aList.SetItemText(aRow, MAINLIST_COL_QQ, apStuInfo->QQ);
	aList.SetItemText(aRow, MAINLIST_COL_Source, apStuInfo->Source);

	lStdString = ZConvStdTimeStr(apStuInfo->InsertTime);
	aList.SetItemText(aRow, MAINLIST_COL_InsertTime, lStdString.c_str());

	lStdString = ZConvStdTimeStr(apStuInfo->UpdateTime);
	aList.SetItemText(aRow, MAINLIST_COL_UpdateTime, lStdString.c_str());

	aList.SetItemText(aRow, MAINLIST_COL_EMail, apStuInfo->EMail);
}

static uint32_t _GetMainListColNum()
{
	return sizeof(MLColums) / sizeof(MLColums[0]) - 1;
}

/* ��ȡͷ������Ϣ */
static CString _GetMainListHeaderLine()
{
	CString lString;

	for (uint32_t i = 0; MLColums[i].Desc; ++i)
	{
		lString.Append(MLColums[i].Desc);
		lString.Append(",");
	}
	return lString;
}

/* ��ȡһ����Ϣ */
static CString _GetMainListCtrlLine(int aRow, CListCtrl& aList)
{
	if (aRow < 0) {
		return "";
	}

	CString lString;

	for (uint32_t i = 0; MLColums[i].Desc; ++i)
	{
		lString.Append(aList.GetItemText(aRow, i));
		lString.Append(",");
	}
	return lString;
}


// CMainListView

IMPLEMENT_DYNCREATE(CMainListView, CListView)

CMainListView::CMainListView()
	: m_list(GetListCtrl())
{
	m_Row = -1;
	m_Col = -1;
}

CMainListView::~CMainListView()
{
}

BEGIN_MESSAGE_MAP(CMainListView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMainListView::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMainListView::OnNMRClick)
	ON_COMMAND(ID_EDIT_MODIFY, &CMainListView::OnEditModify)
	ON_COMMAND(ID_EDIT_DELETE, &CMainListView::OnEditDelete)
	ON_COMMAND(ID_EDIT_RESETMAIN, &CMainListView::OnEditResetmain)
	ON_COMMAND(ID_EDIT_EXPORT, &CMainListView::OnEditExport)
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
		if (g_MemData.OpenStuDB(g_AppConfig.m_StuDBName, g_AppConfig.m_CastIP, g_AppConfig.m_MainPort) != 0)
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



// ����ѧ����Ϣ����ListView��
void CMainListView::UpdateStuToListView(vector<ZStudentInfo*>& aStuVec, BOOL aAppend)
{
	if (!aAppend) {
		m_list.DeleteAllItems();
	}
	else
	{
		// ���Ż���Ӧ���и��ط����������Ϣ�����������ǿ���g_MemData��������ÿ�α�������������
		for (size_t k = 0; k < aStuVec.size(); ++k)
		{
			ZStudentInfo* lpCurStu = aStuVec[k];

			BOOL lFind = FALSE;
			int row = 0;
			CString lName, lTelephone;
			for (row = 0; row < m_list.GetItemCount(); ++row)
			{
				lName = m_list.GetItemText(row, MAINLIST_COL_Name);
				lTelephone = m_list.GetItemText(row, MAINLIST_COL_Telephone);
				if (lName == lpCurStu->Name && lTelephone == lpCurStu->Telehone)
				{
					lFind = TRUE;
					break;
				}
			}

			if (lFind)
			{
				ZDebug("UpdateStuToListView update %d,%s", lpCurStu->Number, lpCurStu->Name);
				_UpdateMainListCtrl(row, m_list, lpCurStu);
			
				// mark as null
				aStuVec[k] = NULL;
			}
		}
	}
	//int lRowOff = m_list.GetItemCount();

	for (size_t i = 0; i < aStuVec.size(); ++i)
	{
		if (!aStuVec[i]) {
			continue;
		}

		//_UpdateMainListCtrl(lRowOff + i, m_list, aStuVec[i]);
		//_UpdateMainListCtrl(lRowOff + i, m_list, aStuVec[aStuVec.size() - i - 1]);
		_UpdateMainListCtrl(0, m_list, aStuVec[i]);
	}
}


// ������˫���¼�
void CMainListView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_Row = pNMItemActivate->iItem;
	m_Col = pNMItemActivate->iSubItem;

	// ˫��ʱ������ѧ����Ϣ�ĶԻ���
	OnEditModify();

	*pResult = 0;
}

// ����Ҽ������¼�
void CMainListView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_Row = pNMItemActivate->iItem;
	m_Col = pNMItemActivate->iSubItem;

	if (m_Row >= 0 || 1)
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

		//��ָ�� λ����ʾ�����˵�
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, lPoint.x, lPoint.y, this);
	}

	*pResult = 0;
}


// �Ҽ��˵�-�޸�
void CMainListView::OnEditModify()
{
	// ��ȡ�������ݣ�������Dialog�У�������Ӧ����
	if (m_Row < 0) {
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		if (pos)
			m_Row = m_list.GetNextSelectedItem(pos);
	}
	int lRow = m_Row;
	if (lRow < 0) {
		return;
	}

	CString lName, lTelephone;
	lName = m_list.GetItemText(lRow, MAINLIST_COL_Name);
	lTelephone = m_list.GetItemText(lRow, MAINLIST_COL_Telephone);

	ZStudentInfo lQryCond = {};
	strncpy(lQryCond.Name, (char*)(LPCSTR)lName, sizeof(lQryCond.Name));
	strncpy(lQryCond.Telehone, (char*)(LPCSTR)lTelephone, sizeof(lQryCond.Telehone));

	vector<ZStudentInfo*> lVec;
	lVec = g_MemData.QueryStuInfo(&lQryCond, ZQueryCompareNameAndTel, 0);
	if (!lVec.empty())
	{
		ZStuInfoDlg lSIDlg;
		lSIDlg.SetStudentInfo(lVec[0]);

		lSIDlg.DoModal();
	}
}

// �Ҽ��˵�-ɾ��
void CMainListView::OnEditDelete()
{
	// ����������ɾ��ָ��������
	POSITION pos;
	while (pos = m_list.GetFirstSelectedItemPosition())
	{
		int lSelItem = m_list.GetNextSelectedItem(pos);
		if (lSelItem >= 0 && lSelItem < m_list.GetItemCount())
		{
			m_list.DeleteItem(lSelItem);
		}

	}
}

// �Ҽ��˵�-����
void CMainListView::OnEditExport()
{
	/* ��¼�������û������е������ݵ�Ȩ�� */
	if (!theApp.HaveExportRight())
	{
		CString lNote;
		lNote.Format("�������е������ݵ�Ȩ��");
		AfxMessageBox(lNote, MB_OK | MB_ICONERROR);
		return;
	}

	//if (m_list.GetSelectedCount() <= 0)
    if (m_list.GetItemCount() == 0)
	{
		CString lNote;
		lNote.Format("��ѡ��Ҫ����������");
		AfxMessageBox(lNote, MB_OK | MB_ICONWARNING);
		return;
	}

	// open file dialog
	BOOL    lIsOpen      = FALSE;
	CString lDefaultDir  = _T(".");
	CString lDefaultFile = _T("���ѧ����Ϣ.csv");
	CString lFilter      = _T("�ļ���*.csv; *.txt��|*.csv; *.txt||");
	CString lFilePath    = lDefaultDir + "\\" + lDefaultFile;

	CFileDialog lOpenFileDlg(lIsOpen, lDefaultDir, lDefaultFile,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lFilter, NULL);

	lOpenFileDlg.GetOFN().lpstrInitialDir = _T(".\\���ѧ����Ϣ.csv");

	INT_PTR lResult    = lOpenFileDlg.DoModal();

	if (lResult != IDOK)
	{
		return;
	}

	lFilePath = lOpenFileDlg.GetPathName();

	CString lLine;
	POSITION pos;

	FILE* fp = fopen((char*)(LPCSTR)lFilePath, "wb");
	if (fp == NULL)
	{
		lLine.Format("�������ݵ��ļ�[%s]��ʧ��:%d", lFilePath, GetLastError());
		AfxMessageBox(lLine, MB_OK | MB_ICONERROR);
		return;
	}

    // TODO: ������¼�Ƿ�Ҫ���� Comments �еļ�¼���˴�δ����
	lLine = _GetMainListHeaderLine();
	lLine.Append("\r\n");
	fwrite((char*)(LPCSTR)lLine, lLine.GetLength(), 1, fp);

	pos = m_list.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        for (int row = 0; row < m_list.GetItemCount(); ++row)
        {		// get data
            lLine = _GetMainListCtrlLine(row, m_list);
            if (lLine.IsEmpty()) {
                continue;
            }

            lLine.Append("\r\n");
            fwrite((char*)(LPCSTR)lLine, lLine.GetLength(), 1, fp);
        }
    }
    else
    {
        // get selected data and save into file
        while (pos)
        {
            int lSelItem = m_list.GetNextSelectedItem(pos);
            if (lSelItem < 0 || lSelItem >= m_list.GetItemCount()) {
                break;
            }

            // get data
            lLine = _GetMainListCtrlLine(lSelItem, m_list);
            if (lLine.IsEmpty()) {
                continue;
            }

            lLine.Append("\r\n");
            fwrite((char*)(LPCSTR)lLine, lLine.GetLength(), 1, fp);
        }
    }

	fflush(fp);
	fclose(fp);
}


// ������������ʾ��Ϣ
void CMainListView::OnEditResetmain()
{
	// �����������ѯ���
	vector<ZStudentInfo*> lVec;
	lVec = g_MemData.QueryAllStudents();
	UpdateStuToListView(lVec, FALSE);
}

