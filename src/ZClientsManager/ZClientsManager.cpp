
// ZClientsManager.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ZClientsManager.h"
#include "MainFrm.h"

#include "ZLoginDlg.h"

#include "ZUtility.h"

#include <ZToolLib/ztl_win32_stacktrace.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ZNetCommBase*   g_pNetComm = NULL;
ZMemoryData     g_MemData;

ztl_log_t*      g_Logger = NULL;

ZAppConfigs     g_AppConfig;

extern BOOL     g_DoSyncStuRequest;

/* 收到网络消息 */
static void _ZOnNetMessage(void* apUserData, ZNetMessage* apMessage)
{
	ZNetHead*   lpNetHead;
	ZMsgDesc*   lpMsgHead;
	void*       lpRawMessage;

    if (g_pNetComm && !g_pNetComm->IsRunning()) {
        return;
    }

	ZNetProtocol::ExtractNetMessage(apMessage, &lpNetHead, &lpMsgHead, &lpRawMessage);

	// got unrecognized message
	if (!ZNetProtocol::IsValidHead(lpNetHead))
	{
		return;
	}

	if (lpNetHead->m_Type == ZNET_T_PublishAdd || lpNetHead->m_Type == ZNET_T_PublishUpdate)
	{
		/* got published message */

		ZDebug("_ZOnNetMessage got Publish NetType:%d, MsgType:%d\n", lpNetHead->m_Type, lpMsgHead->m_MsgType);

		if (lpMsgHead->m_MsgType == ZNET_MSG_UserInfo)
		{
			ZUserInfo lUserInfo = { 0 };
			ZFixString2UserInfo((char*)lpRawMessage, lpMsgHead->m_DataSize, ZNetProtocol::NetMessagePreSize(), &lUserInfo);
			g_MemData.AddOrUpdateUserInfo(lpNetHead->m_Type, &lUserInfo, FALSE);
		}
		else if (lpMsgHead->m_MsgType == ZNET_MSG_StuInfo)
		{
			ZStudentInfo lStuInfo = { 0 };
			ZFixString2StuInfo((char*)lpRawMessage, lpMsgHead->m_DataSize, ZNetProtocol::NetMessagePreSize(), &lStuInfo);
			g_MemData.AddOrUpdateStuInfo(lpNetHead->m_Type, &lStuInfo, FALSE);
		}
	}
	else if (lpNetHead->m_Type == ZNET_T_Query)
	{
		ZDebug("_ZOnNetMessage got T_Query request\n");

		/* got query request, publish the data */
		char lBuffer[4000] = "";
		int  lLength = 0;
		ZNetMessage* lpMessageToSend;

		vector<ZStudentInfo*> lVec = g_MemData.QueryAllStudents();
		for (size_t i = 0; i < lVec.size(); ++i)
		{
			memset(lBuffer, 0, sizeof(lBuffer));
			lLength = ZStuInfoFixString(lVec[i], lBuffer, 0);
			lpMessageToSend = ZNetProtocol::MakeNetMessage(ZNET_T_QueryRsp, ZNET_MSG_StuInfo, lBuffer, lLength);

			ZNetHead* lpNetHead = ZNetProtocol::GetMessageNetHead(lpMessageToSend);
			if (i == lVec.size() - 1)
			{
				lpNetHead->m_Flag |= ZNET_FLAT_IsLast;
			}

			g_pNetComm->DirectSend(lpMessageToSend->GetRawBegin(), lpMessageToSend->Size());

			ZNetMessage::Release(lpMessageToSend);
		}
	}
	else if (lpNetHead->m_Type == ZNET_T_QueryRsp)
	{
		/* got query response */
		if (lpMsgHead->m_MsgType == ZNET_MSG_StuInfo && g_DoSyncStuRequest)
		{
			// mark as false when the last query responsed
			ZNetHead* lpNetHead = ZNetProtocol::GetMessageNetHead(apMessage);
			if (lpNetHead->m_Flag & ZNET_FLAT_IsLast)
			{
				g_DoSyncStuRequest = FALSE;
			}

			ZStudentInfo lStuInfo = {};
			ZFixString2StuInfo(apMessage->GetRawBegin(), apMessage->Size(), ZNetProtocol::NetMessagePreSize(), &lStuInfo);
			g_MemData.AddOrUpdateStuInfo(lpNetHead->m_Type, &lStuInfo, FALSE);
		}
	}

	ZNetMessage::Release(apMessage);
}

// CZClientsManagerApp

BEGIN_MESSAGE_MAP(CZClientsManagerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CZClientsManagerApp::OnAppAbout)
END_MESSAGE_MAP()


// CZClientsManagerApp 构造

CZClientsManagerApp::CZClientsManagerApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ZClientsManager.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CZClientsManagerApp 对象

CZClientsManagerApp theApp;

static char theDumpName[512] = "";

// CZClientsManagerApp 初始化

BOOL CZClientsManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// load netlib
	net_init();

	// win32 dump init
	//sprintf(theDumpName, "AoruiCore_%d.dmp", GetCurrentProcessId());
	//ztl_stack_trace_init(theDumpName);

	// load config "ZAppConfig.txt"
	g_AppConfig.ReadAppConfig(ZAPP_CONFIG_NAME);

	if (!DoLoginDlg())
	{
		return FALSE;
	}

	// login success
	g_Logger = ztl_log_create(g_AppConfig.m_LogName, ZTL_WritFile, false);
	if (!g_Logger) {
		ZDebug("Create log %s failed", g_AppConfig.m_LogName);
	}
	else {
		ztl_log_set_level(g_Logger, g_AppConfig.m_LogLevel);
	}


	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	g_pMainFrame = pFrame;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CZClientsManagerApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CZClientsManagerApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CZClientsManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CZClientsManagerApp 自定义加载/保存方法

void CZClientsManagerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CZClientsManagerApp::LoadCustomState()
{
}

void CZClientsManagerApp::SaveCustomState()
{
}

// CZClientsManagerApp 消息处理程序


BOOL CZClientsManagerApp::HaveExportRight()
{
	if (m_LoginUser.Role == ZUSER_ROLE_Visitor || m_LoginUser.Role == ZUSER_ROLE_Normal)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CZClientsManagerApp::DoLoginDlg()
{
	/* database */
	if (g_MemData.GetUserDB() == NULL)
	{
		// 注意：仅要求有该数据库文件
		FILE* fp = fopen(g_AppConfig.m_UserDBName, "r");
		if (fp == NULL)
		{
			AfxMessageBox(_T("未找到账户信息数据库"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
		fclose(fp);

		// 打开并访问数据库
		if (g_MemData.OpenUserDB(g_AppConfig.m_UserDBName, g_AppConfig.m_CastIP, g_AppConfig.m_MainPort) != 0)
		{
			AfxMessageBox(_T("打开账户信息数据库失败"), MB_OK | MB_ICONERROR);
			return FALSE;
		}

		// 如果是第一次，则创建默认账户
		if (g_MemData.UserCount() == 0)
		{
			// add a root user
			ZUserInfo lUserInfo = { 0 };
			strcpy(lUserInfo.UserName, ZUSER_DEFAULT_RootName);
			strcpy(lUserInfo.Password, ZUSER_DEFAULT_RootPasswd);
			strcpy(lUserInfo.Telephone, "00");
			lUserInfo.Number = 1;
			lUserInfo.Role = ZUSER_ROLE_Root;
			lUserInfo.Cipher = 0;	// ZUSER_CIPHER_Simple
			strcpy(lUserInfo.Comment, "    超级管理员账户，具有最高管理权限，可管理一切其它账户信息和学生资源信息。");

			g_MemData.GetUserDB()->Insert(&lUserInfo, sizeof(lUserInfo));
			g_MemData.AddOrUpdateUserInfo(0, &lUserInfo, FALSE);

			// add a normal user which can only browse data
			strcpy(lUserInfo.UserName, "Guess");
			strcpy(lUserInfo.Password, "123456");
			strcpy(lUserInfo.Telephone, "01");
			lUserInfo.Number = 1;
			lUserInfo.Role = ZUSER_ROLE_Normal;
			lUserInfo.Cipher = 0;	// ZUSER_CIPHER_Simple
			strcpy(lUserInfo.Comment, "    普通账户，只具有浏览学生资源信息的权限，不可管理其它账户信息和学生资源信息。");

			g_MemData.GetUserDB()->Insert(&lUserInfo, sizeof(lUserInfo));
			g_MemData.AddOrUpdateUserInfo(0, &lUserInfo, FALSE);
		}
	}

	/* network */
	if (g_pNetComm == NULL)
	{
		ZNetConfig lNetConf     = {};
		lNetConf.m_Type         = g_AppConfig.m_NetType;
		lNetConf.m_IsBroadcast  = 0;
		lNetConf.m_PeerAddr     = 0;
		lNetConf.m_PeerPort     = g_AppConfig.m_MainPort;
		lNetConf.m_BindPort     = g_AppConfig.m_MainPort;
		lNetConf.m_BindAddr     = string_to_inetaddr(ZNET_DEFAULT_ANYIP);
		lNetConf.m_GroupAddr    = string_to_inetaddr(g_AppConfig.m_CastIP);

		if (strstr(g_AppConfig.m_CastIP, "255.255."))
		{
			lNetConf.m_IsBroadcast = 1;
			//lNetConf.m_PeerAddr = string_to_inetaddr(g_AppConfig.m_CastIP);
			lNetConf.m_PeerAddr = htonl(INADDR_BROADCAST);
			lNetConf.m_GroupAddr = 0;
		}

		// add callback
		lNetConf.m_pFunc = _ZOnNetMessage;
		lNetConf.m_pUserData = this;

		g_pNetComm = new ZUdpComm();
		g_pNetComm->Init(lNetConf);

		g_pNetComm->Start();
	}

	/* login user */
	ZUserInfo* lpUserInfo;

	CString  lUserID, lPasswd;
	uint32_t lSavePasswd = 0;

	ZLoginDlg lLoginDlg;
	if (g_AppConfig.m_UserID[0])
	{
		lLoginDlg.SetUserID(g_AppConfig.m_UserID);
	}
	if (g_AppConfig.m_Password[0])
	{
		lLoginDlg.SetPassword(g_AppConfig.m_Password);
	}
	lLoginDlg.SetRememberPwd(g_AppConfig.m_SavePasswd);

	/* show login dialog and verify userid & passwd */
	do
	{
		if (IDCANCEL == lLoginDlg.DoModal())
		{
			return FALSE;
		}

		lUserID = lLoginDlg.GetUserID();
		lPasswd = lLoginDlg.GetPasswd();
		lSavePasswd = lLoginDlg.IsRememberPasswd();

		ZUserInfo lQryCond = {};
		strncpy(lQryCond.UserName, (char*)(LPCSTR)lUserID, sizeof(lQryCond.UserName) - 1);

		vector<ZUserInfo*> lVec;
		lVec = g_MemData.QueryUserInfo(&lQryCond, ZQueryCompareUserName);
		if (lVec.empty())
		{
			AfxMessageBox(_T("未找到此用户信息"), MB_OK | MB_ICONWARNING);
			continue;
		}
		lpUserInfo = lVec[0];

		if (lUserID.Compare(lpUserInfo->UserName) == 0 &&
			lPasswd.Compare(lpUserInfo->Password) == 0)
		{
			// memory the logined user info
			m_LoginUser = *(lVec[0]);
			break;
		}
		else
		{
			AfxMessageBox(_T("用户名或密码错误"), MB_OK | MB_ICONWARNING);
		}
	} while (TRUE);

	/* write config back to app conf name */
	if (lUserID.Compare(g_AppConfig.m_UserID) != 0 || lPasswd.Compare(g_AppConfig.m_Password) != 0)
	{
		g_AppConfig.SetUserID((char*)(LPCSTR)lUserID);
		g_AppConfig.SetPassword((char*)(LPCSTR)lPasswd);
		g_AppConfig.SetSavePasswd(lSavePasswd);

		g_AppConfig.WriteAppConfig(ZAPP_CONFIG_NAME);
	}

	return TRUE;
}
