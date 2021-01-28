/*************************************
*
*   文件：THStruct.h
*
*   描述：结构体 数据定义文件
*
**************************************/
#pragma once

#include <vector>
#include <functional>
#include <map>

typedef std::function<void(void)> FuncVoid;

// Ini
typedef	std::vector<float>	IniMapPtr;
// file
typedef std::vector<CString> FileMapPtr;
// 声效
struct stSound 
{
	DWORD	sound;
	thTimer	thTime;
	DWORD   chn;
};
typedef std::map<CString,stSound> SoundMapPtr;

// hwnd
typedef std::vector<HWND>	HwndMapPtr;

// Dlg组件
class THDialog;
struct stDlgZJ 
{
	stDlgZJ(){memset(this,0,sizeof(*this));}
	THDialog *pdlg;
	bool	 bDelete;
};
typedef	std::map<int,stDlgZJ> DlgMapPtr;

// 线程
struct thByteStream;
struct stThread 
{
	HANDLE		 _ThrHand;	 // 句柄
	unsigned int _dwThreadID;// 线程ID
	int			 _iTdID;	 // 线程标识
	THDialog	 *_lpWnd;	 // 窗口指针
	thByteStream _thParams;	 // 传递线程参数
};
typedef	std::map<int,stThread> ThreadMapPtr;
typedef	std::vector<int>	   ClearMapPtr;

// 资源加载
struct stReLoad 
{
	stReLoad(){memset(this,0,sizeof(*this));}
	char	szLoadPath[MAX_PATH];
	int		iLoadType;
	int		iResType;
};

// 记录时间
struct stRecord
{
	stRecord(){memset(this,0,sizeof(*this));}
	int	 iYear;		// 年
	int	 iMonth;	// 月
	int	 iDay;		// 日

	int	 iHour;		// 小时
	int	 iMinute;	// 分钟

	bool bProcess;  // 进程
	bool bHwnd;		// 窗口
	//char szProcess[1024];	// 进程
	//char szHwnd[1024];		// 窗口
};
typedef	std::vector<stRecord> RecordMapPtr;

// 进程窗口信息
struct stProcess 
{
	//stProcess(){memset(this,0,sizeof(*this));}
	HwndMapPtr	mapHwnd;			// 窗口句柄
	DWORD		th32ProcessID;		// 进程id
	char		szExePath[MAX_PATH];// 进程EXE路径
};
typedef std::map<CString,stProcess> ProcessMapPtr;

// 打开窗口线程消息
struct stPopThread 
{
	stPopThread(){memset(this,0,sizeof(*this));}
	int  iType;
	int  iWDType;
	char szPath[MAX_PATH];
};

// 热键消息
struct stHotKey
{
	stHotKey(){memset(this,0,sizeof(*this));}
	HWND	hWnd;		// 窗口
	UINT	nGroupkey1;	// 热键 组合键1
	UINT	nGroupkey2;	// 热键 组合键2
	UINT	nHotkey;	// 热键
	//int		iId;		// 消息ID

	UINT	GetVKKey(UINT nkey)
	{
		if (nkey==MOD_CONTROL) return VK_CONTROL;
		if (nkey==MOD_ALT) return VK_MENU;
		if (nkey==MOD_SHIFT) return VK_SHIFT;
		if (nkey==MOD_WIN) return 0x5B;
		return 0;
	}
};
typedef std::map<int,stHotKey>	HotKeyMapPtr;

// 时间
struct stTime 
{
	stTime(){memset(this,0,sizeof(*this));}
	// 时间
	WORD wYear;
	WORD wMonth;
	WORD wDay;
	// 开始时间
	WORD wHourSt;
	WORD wMinuteSt;
	// 结束时间
	WORD wHourEd;
	WORD wMinuteEd;
	//WORD wSecond;
	// 星期1-7 0-每天
	BOOL bDayOfWeek[8];
};
// 事件
struct stEvent
{
	stEvent(){memset(this,0,sizeof(*this));iState=1;}
	char	szTsMsg[1024];	// 提示信息
	int		iState;			// 状态 1-等待 2-运行 3-冲突

	// 播放时间类型
	int		ptType;			// 0-指定日期，1-每天，天数循环播放
	stTime	ptTime;
};
// 动作序列 Begin
//struct stActions : public stEvent
//{
//	stActions(){memset(this,0,sizeof(*this));}
//	THAnimation ani;		// 动画播放
//};
//// 所有动作列表
//typedef	std::vector<stActions> ActionsMapPtr;

// 动作序列 End


// 定时事件 Begin
// 1 - 安仔消息提示
// 2 - 弹窗提示
//	1 - 中间窗口
//  2 - 右下角
//  3 - 左下角
// 3 - 当前窗口
//  1 - 震动 窗口
//  2 - 闪烁 窗口 
//  3 - 最小化 窗口

// 事件
struct stEventTM
{
	stEventTM(){memset(this,0,sizeof(*this));}
	char	szTsMsg[1024];	// 提示信息
	char	szTsSound[256];	// 提示音乐
	int		mapTs[2];		// 提示事件
	int		iEvent;			// 响应事件
	int		iType;			// 事件类型

	// 播放时间类型
	int		ptType;			// -1不添加 0-指定日期，1-每天，天数循环播放 
	int     ptFanShi;		// 0-指定时间 1-间隔提醒 2-整点提醒 3-半点提醒
	stTime	ptTime;
};

typedef	std::vector<stEventTM*> EventTMMapPtr;

// 定点报时
// 1 - 到点 报时
// 2 - 指定时间 报时
// 定时关机
// 1 - 指定时间关机 一次
// 2 - 指定时间关机 每天
struct stEventTM_Type : public stEventTM
{
	stEventTM_Type(){memset(this,0,sizeof(*this));}
};

// 定时提醒(运行程序或外部处理或消息提示)
// 1 - 运行外部程序或打开网页
// 2 - 终止指定程序
struct stEventTM_EXEWD : public stEventTM
{
	stEventTM_EXEWD(){memset(this,0,sizeof(*this));}
	char	szEXE[256];	// exe程序
	char	szWnd[256]; // 窗口标题
	HWND	hWnd;		// 窗口句柄
};

// 更换壁纸	(需要选择壁纸保存的目录)
// 1 - 指定时间 更换
struct stEventTM_Paper : public stEventTM
{
	stEventTM_Paper(){memset(this,0,sizeof(*this));}
	char	szPath[256];	// 路径
	int		iPaper;			// 壁纸序号
};

// 截图	(需要选择截图保存的目录)
// 1 - 全屏截图 
// 2 - 指定窗口截图
// 3 - 指定EXE截图
struct stEventTM_JieTu : public stEventTM
{
	stEventTM_JieTu(){memset(this,0,sizeof(*this));}
	bool	bJiaQian;	 // 是否加强截图
	char	szPath[256]; // 保存目录
	char	szEXE[256];	 // exe程序
	char	szWnd[256]; // 窗口标题
	HWND	hWnd;		// 窗口句柄
};

// 高级提醒 保存数据(提醒正在编辑的玩家)
// 1 - 提醒保存数据 根据指定的exe程序
// 2 - 提醒保存数据 指定时间提醒
// 3 - 提醒保存数据 间隔提醒
// 4 - 有exe程序 运行 提示
// 4 - 有exe程序 运行 终止
struct stEventTM_Save : public stEventTM
{
	stEventTM_Save(){memset(this,0,sizeof(*this));}

};

// 定时事件 End


// 快捷操作 Begin
// 1 - 注册快捷键 全局
// 2 - 注册快捷键 本程序
// 3 - 执行事件
//	   1 - 指定程序或窗口 终止/打开
//	   2 - 指定程序或窗口 隐藏/显示 
//     3 - 打开网页或程序
//     4 - 显示桌面
//     5 - 隐藏/显示 所有窗口
//     6 - 窗口 全屏/窗口
//	   7 - 窗口 提前
//	   8 - 窗口 置顶
//	   9 - 截图	1全屏截图 2指定窗口截图	(需要选择截图保存的目录)
//    10 - 关机
//    11 - 其他事件
/*	  11 - 显示万年历 暂不添加*/
struct stEventKJ
{
	stEventKJ(){memset(this,0,sizeof(*this));}
	UINT	nGroupkey1;	// 热键 组合键1
	UINT	nGroupkey2;	// 热键 组合键2
	UINT	nHotkey;	// 热键
	BOOL	bAll;		// 全局/局部
	int		iEvent;		// 执行事件
	char	szWnd[256]; // 窗口标题
	HWND	hWnd;		// 窗口句柄
};
typedef	std::vector<stEventKJ*>	EvtKJMapPtr;

//     1 - 指定程序或窗口 终止/打开
//     2 - 指定程序或窗口 隐藏/显示
//	   3 - 打开网页或程序
struct stEventKJ_OCWD : public stEventKJ
{
	//stEventKJ_OCWD(){memset(this,0,sizeof(*this));}
	char		szEXE[256];	// exe程序
	HwndMapPtr  mapHwnd;	// 窗口记录
};

//     10 - 关机
//	   11 - 其他事件
struct stEventKJ_Type : public stEventKJ
{
	stEventKJ_Type(){memset(this,0,sizeof(*this));}
	int		iType;
};

//	   9 - 截图	(需要选择截图保存的目录)
//      1 - 全屏截图 
//      2 - 指定窗口截图
//		3 - 指定EXE截图
struct stEventKJ_JieTu : public stEventKJ
{
	//stEventKJ_JieTu(){memset(this,0,sizeof(*this));}
	int		iType;		 // 截图类型
	bool	bJiaQian;	 // 是否加强截图
	char	szPath[256]; // 保存目录
	char	szEXE[256];	 // exe程序
	HwndMapPtr  mapHwnd; // 窗口记录
};

//     6 - 窗口 全屏/窗口
struct stEventKJ_Full : public stEventKJ
{
	stEventKJ_Full(){memset(this,0,sizeof(*this));}
	CRect		rect;		// 窗口大小
	LONG		lStyle;		// 窗口样式
};

//     4 - 显示桌面
//     5 - 隐藏/显示 所有窗口
//	   7 - 窗口 提前
//	   8 - 窗口 置顶
struct stEventKJ_Window : public stEventKJ
{
	//stEventKJ_Window(){memset(this,0,sizeof(*this));}
	CRect		rect;		// 窗口大小
	HwndMapPtr  mapHwnd;	// 窗口记录
};

// 快捷操作 End
