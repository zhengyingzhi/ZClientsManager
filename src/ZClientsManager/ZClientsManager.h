
// ZClientsManager.h : ZClientsManager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "ZMemoryData.h"

class CMainFrame;

extern ZMemoryData		g_MemData;
extern ZNetCommBase*	g_pNetComm;
extern CMainFrame*		g_pMainFrame;

#define ZAPP_CONFIG_NAME    "ZAppConfig.txt"
typedef struct
{
	char    m_UserID[24];
}ZAppConfigs;

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
	void ReadConfigs(ZAppConfigs& aAppConfigs);

public:
	CString GetLoginUserID() { return m_LoginUser; }
	CString m_LoginUser;

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CZClientsManagerApp theApp;
