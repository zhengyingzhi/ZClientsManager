
// ZClientsManager.h : ZClientsManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

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
