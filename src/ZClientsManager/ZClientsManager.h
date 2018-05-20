
// ZClientsManager.h : ZClientsManager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "ZMemoryData.h"

#include "ZAppConfig.h"


class CMainFrame;

extern ZMemoryData		g_MemData;
extern ZNetCommBase*	g_pNetComm;
extern CMainFrame*		g_pMainFrame;

extern ztl_log_t*		g_Logger;

extern ZAppConfigs		g_AppConfig;


// CZClientsManagerApp:
// 有关此类的实现，请参阅 ZClientsManager.cpp
//

class CZClientsManagerApp : public CWinAppEx
{
public:
	CZClientsManagerApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

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
