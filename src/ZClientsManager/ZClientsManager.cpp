
// ZClientsManager.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ZClientsManager.h"
#include "MainFrm.h"

#include "ZLoginDlg.h"

#include <ZToolLib/ztl_config.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ZMemoryData g_MemData;

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

	if (!DoLoginDlg())
	{
		return FALSE;
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



void CZClientsManagerApp::ReadConfigs(ZAppConfigs& aAppConfigs)
{
	ztl_config_t* zconf;
	zconf = ztl_config_open(ZAPP_CONFIG_NAME, '#', '=');
	if (!zconf)
	{
		return;
	}

	int lLength = 0;
	char* lpOutUserID = nullptr;
	if (ztl_config_read_str(zconf, "UserID", &lpOutUserID, &lLength))
	{
		strncpy(aAppConfigs.m_UserID, lpOutUserID, sizeof(aAppConfigs.m_UserID) - 1);
	}

	ztl_config_close(zconf);
}

BOOL CZClientsManagerApp::DoLoginDlg()
{
	if (g_MemData.GetUserDB() == NULL)
	{
		if (g_MemData.OpenUserDB(USERINFO_DB_DEFAULT_NAME, "127.0.0.1", 0) != 0)
		{
			AfxMessageBox(_T("打开账户信息数据库失败"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}

	ZAppConfigs lAppConfigs = {};
	ReadConfigs(lAppConfigs);

	ZUserInfo*      lpUserInfo;

	CString   lUserID, lPasswd;
	ZLoginDlg lLoginDlg;
	if (lAppConfigs.m_UserID[0])
	{
		lLoginDlg.SetUserID(lAppConfigs.m_UserID);
	}

#if defined(DEBUG) || defined(_DEBUG)
	lPasswd = _T("123456");
	lLoginDlg.SetPassword(lPasswd);
#endif // DEBUG


	do
	{
		if (IDCANCEL == lLoginDlg.DoModal())
		{
			return FALSE;
		}

		lUserID = lLoginDlg.GetUserID();
		lPasswd = lLoginDlg.GetPasswd();

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
			break;
		}
		else
		{
			AfxMessageBox(_T("用户名或密码错误"), MB_OK | MB_ICONWARNING);
		}
	} while (TRUE);

	if (lUserID.Compare(lAppConfigs.m_UserID) != 0)
	{
		char lConfBuffer[1024] = "";
		sprintf(lConfBuffer, "UserID=%s", (char*)(LPCSTR)lUserID);
		FILE* fp = fopen(ZAPP_CONFIG_NAME, "w");
		if (fp)
		{
			fputs(lConfBuffer, fp);
			fflush(fp);
			fclose(fp);
		}
	}

	return TRUE;
}
