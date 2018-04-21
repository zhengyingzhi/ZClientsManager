
// ZClientsManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ZClientsManager.h"
#include "MainFrm.h"

#include "ZLoginDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ZNetCommBase*	g_pNetComm = NULL;
ZMemoryData		g_MemData;

ZAppConfigs		g_AppConfig;

/* �յ�������Ϣ */
static void _ZOnNetMessage(void* apUserData, ZNetMessage* apMessage)
{
	// todo: operations
	ZNetHead*	lpNetHead;
	ZMsgHead*	lpMsgHead;
	void*		lpRawMessage;

	ZNetProtocol::ExtractNetMessage(apMessage, &lpNetHead, &lpMsgHead, &lpRawMessage);

	// got unrecognized message
	if (!ZNetProtocol::IsValidHead(lpNetHead))
	{
		return;
	}

	if (lpNetHead->m_Type == ZNET_T_PublishAdd || lpNetHead->m_Type == ZNET_T_PublishUpdate)
	{
		if (lpMsgHead->m_MsgType == ZNET_MSG_UserInfo)
		{
			g_MemData.AddOrUpdateUserInfo((ZUserInfo*)lpRawMessage);
		}
		else if (lpMsgHead->m_MsgType == ZNET_MSG_StuInfo)
		{
			g_MemData.AddOrUpdateStuInfo((ZStudentInfo*)lpRawMessage);
		}
	}

	ZNetMessage::Release(apMessage);
}

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

	// load netlib
	net_init();

	// load config "ZAppConfig.txt"
	g_AppConfig.ReadAppConfig(ZAPP_CONFIG_NAME);

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

	g_pMainFrame = pFrame;

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


BOOL CZClientsManagerApp::DoLoginDlg()
{
	/* database */
	if (g_MemData.GetUserDB() == NULL)
	{
		if (g_MemData.OpenUserDB(g_AppConfig.m_UserDBName, g_AppConfig.m_CastIP, , g_AppConfig.m_MainPort) != 0)
		{
			AfxMessageBox(_T("���˻���Ϣ���ݿ�ʧ��"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}

		// if have no any user at firstly, add a default root user
		if (g_MemData.UserCount() == 0)
		{
			ZUserInfo lUserInfo = { 0 };
			strcpy(lUserInfo.UserName, ZUSER_DEFAULT_RootName);
			strcpy(lUserInfo.Password, ZUSER_DEFAULT_RootPasswd);
			strcpy(lUserInfo.Telephone, "00");
			lUserInfo.Number = 1;
			lUserInfo.Role = ZUSER_ROLE_Root;
			lUserInfo.Cipher = 0;	// ZUSER_CIPHER_Simple
			strcpy(lUserInfo.Comment, "    ��������Ա�˻���������߹���Ȩ�ޣ��ɹ���һ�������˻���Ϣ��ѧ����Դ��Ϣ��");

			g_MemData.GetUserDB()->Insert(&lUserInfo, sizeof(lUserInfo));
			g_MemData.AddUserInfo(&lUserInfo);
		}
	}

	/* network */
	if (g_pNetComm == NULL)
	{
		ZNetConfig lNetConf;
		lNetConf.m_Type			= g_AppConfig.m_NetType;
		lNetConf.m_IsBroadcast	= 0;
		lNetConf.m_PeerAddr		= 0;
		lNetConf.m_PeerPort		= 0;
		lNetConf.m_BindPort		= g_AppConfig.m_MainPort;
		lNetConf.m_BindAddr		= string_to_inetaddr(ZNET_DEFAULT_ANYIP);
		lNetConf.m_GroupAddr	= string_to_inetaddr(g_AppConfig.m_CastIP);

		// todo: add callback
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
			AfxMessageBox(_T("δ�ҵ����û���Ϣ"), MB_OK | MB_ICONWARNING);
			continue;
		}
		lpUserInfo = lVec[0];

		if (lUserID.Compare(lpUserInfo->UserName) == 0 &&
			lPasswd.Compare(lpUserInfo->Password) == 0)
		{
			// memory the logined user info
			m_LoginUser = lUserID;
			break;
		}
		else
		{
			AfxMessageBox(_T("�û������������"), MB_OK | MB_ICONWARNING);
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
