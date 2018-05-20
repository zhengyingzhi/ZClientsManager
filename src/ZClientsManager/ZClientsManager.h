
// ZClientsManager.h : ZClientsManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "ZMemoryData.h"

#include "ZAppConfig.h"


class CMainFrame;

extern ZMemoryData		g_MemData;
extern ZNetCommBase*	g_pNetComm;
extern CMainFrame*		g_pMainFrame;

extern ztl_log_t*		g_Logger;

extern ZAppConfigs		g_AppConfig;


// CZClientsManagerApp:
// �йش����ʵ�֣������ ZClientsManager.cpp
//

class CZClientsManagerApp : public CWinAppEx
{
public:
	CZClientsManagerApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	BOOL DoLoginDlg();

public:
	BOOL HaveExportRight();
	CString GetLoginUserID() { return m_LoginUser.UserName; }

	ZUserInfo m_LoginUser;

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CZClientsManagerApp theApp;
