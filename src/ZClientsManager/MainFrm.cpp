
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ZClientsManager.h"

#include "MainFrm.h"

#include "ZStuInfoDlg.h"
#include "ZUserInfoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
CMainFrame* g_pMainFrame = NULL;

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_EDIT_INSERT, &CMainFrame::OnEditInsert)
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_COMMAND(ID_EDIT_DELETE, &CMainFrame::OnEditDelete)
	ON_COMMAND(ID_EDIT_MANAGER, &CMainFrame::OnEditManager)
	ON_COMMAND(ID_SYS_CLOSE, &CMainFrame::OnSysClose)
	ON_COMMAND(ID_BUTTON_Add, &CMainFrame::OnButtonAdd)
	ON_COMMAND(ID_BUTTON_Del, &CMainFrame::OnButtonDel)
	ON_COMMAND(ID_BUTTON_Modify, &CMainFrame::OnButtonModify)
	ON_COMMAND(ID_BUTTON_Find, &CMainFrame::OnButtonFind)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static bool _IsNumericString(const char* apFindStr, int aLength)
{
	const char* lpCur = apFindStr;
	while (lpCur < (apFindStr + aLength))
	{
		if (*lpCur < '0' || *lpCur > '9') {
			return false;
		}
		++lpCur;
	}
	return true;
}

static bool _IsScoreString(const char* apFindStr)
{
	int lLength = strlen(apFindStr);
	if (lLength > 4) {
		return false;
	}

	const char* lpTemp = strchr(apFindStr, '.');
	if (lpTemp)
	{
		if (_IsNumericString(apFindStr, int(lpTemp - apFindStr))) {
			return true;
		}
		return false;
	}

	if (_IsNumericString(apFindStr, strlen(apFindStr))) {
		return true;
	}
	return false;
}

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	m_pMainView = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("δ�ܴ�����ͼ����\n");
	//	return -1;
	//}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		//!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_My))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������: 
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// ����Զ���ComboBox����������
	// ��ȡCombo��λ��
	int index = 0;
	RECT rect;
	while (m_wndToolBar.GetItemID(index) != ID_BUTTON_Combo)
		index++;

	// ����ComboBox���ԣ�����ʾ����������
	m_wndToolBar.SetButtonInfo(index, ID_BUTTON_Combo, TBBS_SEPARATOR, 60);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left += 60;
	rect.top  += 20;
	rect.right += 160;
	rect.bottom += 100;
	m_comboBox.Create(CBS_DROPDOWN | WS_VISIBLE | WS_TABSTOP | CBS_AUTOHSCROLL, rect,
		&m_wndToolBar, ID_BUTTON_Combo);
	m_comboBox.InsertString(0, "");
	m_comboBox.SetCurSel(0);
	m_comboBox.ShowWindow(SW_SHOW);


	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ����������: 
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			//CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_AORUIAPP);
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	CString lTitle;
	lTitle.Format(_T("����ʽ��� - %s"), theApp.GetLoginUserID());
	SetWindowText(lTitle);

	return 0;
}

void CMainFrame::OnClose()
{
	if (IDYES != AfxMessageBox(_T("�Ƿ��˳�"), MB_YESNO))
	{
		return;
	}

	// save data
	g_AppConfig.WriteAppConfig(ZAPP_CONFIG_NAME);

	g_MemData.CloseUserDB();
	g_MemData.CloseStuDB();

	CFrameWndEx::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	// temprary comment out by yizhe when create project
	//m_wndCaptionBar.ToggleAutoHide();
	m_wndCaptionBar.ShowWindow(SW_HIDE);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	if (m_pMainView)
		m_pMainView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_pMainView && (nCode == WM_KEYDOWN))
	{

	}

	// ����ͼ��һ�γ��Ը�����
	if (m_pMainView && m_pMainView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// ר���ڿͻ������ڴ���
	CCreateContext cc;
	cc.m_pNewViewClass = RUNTIME_CLASS(CMainListView);
	m_pMainView = (CMainListView *)CreateView(&cc);//CWnd::Create

	return TRUE;
	//return CFrameWndEx::OnCreateClient(lpcs, pContext);
}



// ����ѧ����Ϣ��ListView��
void CMainFrame::UpdateStuToMainListView(vector<ZStudentInfo*>& aStuVec, BOOL aAppend)
{
	if (m_pMainView && aStuVec.size())
	{
		m_pMainView->UpdateStuToListView(aStuVec, aAppend);
	}
}

// ������-ģ����ѯ
void CMainFrame::VagueFind(const char* apFindStr)
{
	int lLength = strlen(apFindStr);

	vector<ZStudentInfo*> lStuVec;
	ZStudentInfo lStuInfo = {};

	if (_IsNumericString(apFindStr, lLength) && (lLength == 4 || lLength == 11))
	{
		strcpy(lStuInfo.Telehone, apFindStr);
		lStuVec =g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareNameAndTel, 0);
	}
	else if (_IsScoreString(apFindStr))
	{
		lStuInfo.LanguageScore = uint32_t(atof(apFindStr) * 10);
		lStuVec = g_MemData.QueryStuInfo(&lStuInfo, ZQueryCompareScore, CC_Equal);
	}
	else
	{
		// ģ����ѯ ���֡�QQ�����֤����ѧ��״̬ ��
		lStuVec = g_MemData.QueryStuInfoVague(apFindStr);
	}

	if (!lStuVec.empty())
	{
		UpdateStuToMainListView(lStuVec, FALSE);
	}
	else
	{
		AfxMessageBox(_T("δ��ѯ�����ѧ����Ϣ\n��ʹ�ò�ѯ�Ի����ѯ"), MB_OK | MB_ICONWARNING);
	}
}


// ѧ����Ϣ�����¼�
void CMainFrame::OnEditInsert()
{
	ZStuInfoDlg lSIDlg;
	lSIDlg.SetOperateType(ZOT_Insert);
	lSIDlg.DoModal();
}


// ѧ����Ϣ�༭�¼�
void CMainFrame::OnEditFind()
{
	m_StuQryDlg.DoModal();
}


// ѧ����Ϣɾ���¼�
void CMainFrame::OnEditDelete()
{
	ZStuInfoDlg lSIDlg;
	lSIDlg.SetOperateType(ZOT_Delete);
	lSIDlg.DoModal();
}


// ϵͳ�ر��¼�
void CMainFrame::OnSysClose()
{
	OnClose();
}


// �˻������¼�
void CMainFrame::OnEditManager()
{
	ZUserInfoDlg lUIDlg;
	lUIDlg.DoModal();
}


// ������ - ���
void CMainFrame::OnButtonAdd()
{
	OnEditInsert();
}

// ������ - ɾ��
void CMainFrame::OnButtonDel()
{
	OnEditDelete();
}

// ������ - �޸�
void CMainFrame::OnButtonModify()
{
	m_pMainView->OnEditModify();
}

// ������ - ����
void CMainFrame::OnButtonFind()
{
	// get search content
	CString lContent;
	m_comboBox.GetWindowText(lContent);

	if (lContent.IsEmpty())
	{
		OnEditFind();
	}
	else
	{
		// if text not changed
		if (m_comboText == lContent)
		{
			return;
		}
		m_comboText = lContent;

		// try add one search condition to combobox
		if (m_comboBox.GetCount() > 10) {
			m_comboBox.DeleteString(10);
		}

		CString lString;
		int i;
		for (i = 0; i < m_comboBox.GetCount(); ++i)
		{
			lString.Empty();
			m_comboBox.GetLBText(i, lString);
			if (lString == lContent)
				break;
		}

		if (i == m_comboBox.GetCount())
		{
			m_comboBox.InsertString(0, lContent);
		}

		// ģ����ѯ��������
		VagueFind((char*)(LPCSTR)lContent);
	}
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CString lContent;
		m_comboBox.GetWindowText(lContent);
		if (m_comboBox.IsChild(GetFocus()) && !lContent.IsEmpty() && lContent != m_comboText)
		{
			m_comboText = lContent;

			// ģ����ѯ��������
			VagueFind((char*)(LPCSTR)lContent);

			m_comboBox.SetFocus();
		}
	}

	return CFrameWndEx::PreTranslateMessage(pMsg);
}
