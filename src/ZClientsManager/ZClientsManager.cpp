
// ZClientsManager.cpp : ����Ӧ�ó��������Ϊ��
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


// CZClientsManagerApp ����

CZClientsManagerApp::CZClientsManagerApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ZClientsManager.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CZClientsManagerApp ����

CZClientsManagerApp theApp;


// CZClientsManagerApp ��ʼ��

BOOL CZClientsManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


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

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CZClientsManagerApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CZClientsManagerApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CZClientsManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CZClientsManagerApp �Զ������/���淽��

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

// CZClientsManagerApp ��Ϣ�������



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
			AfxMessageBox(_T("���˻���Ϣ���ݿ�ʧ��"), MB_OK | MB_ICONWARNING);
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
			AfxMessageBox(_T("δ�ҵ����û���Ϣ"), MB_OK | MB_ICONWARNING);
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
			AfxMessageBox(_T("�û������������"), MB_OK | MB_ICONWARNING);
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
