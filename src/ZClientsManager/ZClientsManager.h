
// ZClientsManager.h : ZClientsManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "StudentInfoDB.h"
#include "UserInfoDB.h"


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
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CZClientsManagerApp theApp;
