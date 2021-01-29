
// MFCAppCPDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCAppCP.h"
#include "MFCAppCPDlg.h"
#include "afxdialogex.h"

#include "../THEngine/mfc/thMFCHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMFCAppCPDlg 对话框



CMFCAppCPDlg::CMFCAppCPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPCP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorerWeb);
	DDX_Control(pDX, IDC_LISTKJ, m_listKj);
}

BEGIN_MESSAGE_MAP(CMFCAppCPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CMFCAppCPDlg 消息处理程序

BOOL CMFCAppCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 初始化
	srand((unsigned)time(NULL));
	// 初始化
	thHelper::AppInit();

	// http
	char szPath[256];
	GetTempPath(256, szPath);
	CString strPath = szPath;
	HttpClient::getInstance();
	network::HttpClient::getInstance()->setTimeoutForConnect(30);
	network::HttpClient::getInstance()->setTimeoutForRead(30);
	network::HttpClient::getInstance()->enableCookies(strPath + "ckfxm.txt");

	// lua engine
	auto engine = thLuaEngine::getInstance();
	engine->setAppPath(thHelper::MakePath_ASCII("").GetString());
	engine->getLuaStack()->setXXTEAKeyAndSign("thengine", strlen("thengine"), "thengineSign", strlen("thengineSign"));

	// init...

	// 开启定时器
	// 开启更新
	this->SetTimer(TH_TIMER_UPDATE, 10, NULL);

	// 注册热键
	registerHkey();
	// 注册类到lua
	registerToLua();

	// 初始化数据
	initData();

	// start
	startRun();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCAppCPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCAppCPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCAppCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCAppCPDlg::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CMFCAppCPDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);

	if (nKey2 == VK_F8)
	{
		// 隐藏显示窗口
		::ShowWindow(GetSafeHwnd(), IsWindowVisible()? SW_HIDE:SW_SHOW);
	}
}

void CMFCAppCPDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	// delay
	static long time_tick = GetTickCount();
	long time_delay = 0;
	auto tk = GetTickCount();
	if (tk != time_tick)
	{
		time_delay = tk - time_tick;
		time_tick = tk;
	}

	thHelper::AppRun(time_delay);
	thLuaEngine::getInstance()->clear();
}

void CMFCAppCPDlg::registerToLua()
{
	auto luas = thLuaEngine::getInstance()->getLuaState();
	::registerMFCToLua(luas);
	thHttpManage::registerToLua(luas);
}

void CMFCAppCPDlg::registerHkey()
{
	// 注册热键
	::RegisterHotKey(GetSafeHwnd(), WM_HOTKEY, 0, VK_F8);
}

void CMFCAppCPDlg::initData()
{
	// 广告
	//m_explorerWeb.put_MenuBar(FALSE);
	////	m_explorerWeb.put_StatusBar(FALSE);
	////	m_explorerWeb.put_ToolBar(FALSE);
	//m_explorerWeb.Navigate("http://nbachenxiao.ys168.com/", 0, 0, 0, 0);
	thMFCManager::create();
	// add
	thMFCManager::addListCtrl("listKj", m_listKj);

}

bool CMFCAppCPDlg::startRun()
{
	if (thLuaEngine::getInstance()->executeScriptFile("res/main.lua"))
	{
		ASSERT(false);
		return false;
	}

	// lua层
	// 控件初始化
	//thLuaFunction::call("initCtrl");

	WinExec("F:\\work\\work\\cocos\\ca\\new\\MFCAppCP\\THEngine\\lua\\luajit\\plugin_luacompile\\bin\\32bit\\luajit-win32.exe -b F:\\work\\work\\cocos\\ca\\new\\MFCAppCP\\Debug\\res\\main.lua F:\\work\\work\\cocos\\ca\\new\\MFCAppCP\\Debug\\res\\main.luac", SW_SHOW);

	return true;
}
