
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "MainListView.h"

#include "ZStuQryDlg.h"

#include "ZTaskManager.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CMFCCaptionBar    m_wndCaptionBar;
	CMainListView*    m_pMainView;

	CComboBox         m_comboBox;
	CString           m_comboText;

protected:
	ZStuQryDlg        m_StuQryDlg;
	ZTaskManager      m_TaskManager;

public:
	void UpdateStuToMainListView(vector<ZStudentInfo*>& aStuVec, BOOL aAppend);
	void UpdateStuToMainListView(ZStudentInfo* apStuInfo, BOOL aAppend);

	void VagueFind(const char* apFindStr);

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	BOOL CreateCaptionBar();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnEditInsert();
	afx_msg void OnEditFind();
	afx_msg void OnEditDelete();
	afx_msg void OnEditManager();
	afx_msg void OnSysClose();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonFind();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditSync();
	afx_msg void OnButtonReset();
	afx_msg void OnEditTaskman();
};


