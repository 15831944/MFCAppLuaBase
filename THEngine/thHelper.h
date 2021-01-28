#pragma once
#include "thStruct.h"
// #include "bass/THSMManager.h"
#include <ShlObj.h> 
#include <functional>

typedef BOOL (WINAPI *LayeredWindow)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD); 
// time
enum
{
	TM_SysTime,	// 系统时间
	TM_Year,	// 年
	TM_Month,	// 月
	TM_Day,		// 日
	TM_Hour,	// 小时
	TM_Minute,	// 分钟
	TM_Second,	// 秒
};

// exitwindow
enum
{
	ET_SHUTDOWN, // 正常关机
	ET_POWEROFF, // 强制关机
	ET_REBOOT,	 // 重启
	ET_LOGOFF,	 // 注销
	ET_IDLE,	 // 待机
	ET_OFF,		 // 休眠
	ET_SUOPING,	 // 锁屏
	ET_MONITORPOWER,// 关闭显示器
	ET_SCREENSAVE,	// 启动屏保
};

// 截图
enum
{
	PS_FULLSCREEN,	// 全屏截图
	PS_WINDOW,		// 窗口截图
};

// 获取文件
enum
{
	LV_ALL,	// 获取所有文件
	LV_IMG,	// 获取图片文件
	LV_VIDEO,// 视频文件
	LV_SOUND,// 音频文件
	LV_SWF,// SWF文件
};

// 鼠标状态
enum
{
	CR_MOVE,	// 移动状态
	CR_STOP,	// 停止
	CR_INCTRL,	// 进入控件状态
};

#define GRAY_AVERAGE		0
#define GRAY_MAX			1
#define GRAY_WEIGHTAVERAGE	2

// 功能补助函数
class CxImage;
class THPopWindow;
class thHelper
{
public:
	thHelper(void);
	// Sound,Music实例
	// static THSMManager* SM(void) {return &(s_lpTH->m_SMManager);}
	// 报时程序
	// static THTimeBS*	BS(void) {return &(s_lpTH->m_TimeBS);}

public:
	// 判断当前程序是否已经在运行. 防止多开
	static bool		IsAppExist(void);
	// 实例化
	static void		AppInit(void);
	// 运行
	static void		AppRun(int delay);
	// 退出时调用清除 
	static void		AppExit(void);

	// 日记
	static void		Log_Create(const char *filename);
	static void		Log_Write(const char *szFormat, ...);
	static void		Log_WriteTime(const char *szFormat, ...);
	static void		Log_Print(const char *szFormat, ...);
	// 路径枚举
	static CString	MakePath_ASCII(const char *filename=0);
	static wchar_t* MakePath_UNICODE(const char *filename=0);
	// 枚举缓存路径
	static CString	MakeTempPath(const char *filename=0);
	// 去掉路径
	static CString	AnalyPath(const char *Pathfile=0);
	// 加载图片Gdi+ 内存
	static Bitmap*	LoadImgFromFile(const char *filename, DWORD size=0);
	static Bitmap*	LoadImgFromMem(const char *data, DWORD size);
	// 加载图片CxImage转换到GDI+Bitmap 可以支持显示CxImage的全部图片
	static Bitmap*	LoadImgFromCxImg(const TCHAR * filename, DWORD imagetype=0);
	// 转换RGB
	static void		RGB2ARGB(Bitmap **pbm);
	// GDI+bitmap 转 CxImage
	static BOOL		Bitmap2Ximage(Bitmap *pBitmap, CxImage &xImage);
	// CxImage 转 GDI+bitmap
	static Bitmap*  Ximage2Bitmap(CxImage *pXimage);
	// 获取图片帧数 可以判断是否是Gif图片
	static UINT		GetFrameCount(Bitmap* pimage);
	// 获取CLSID
	static bool		GetEncoderCLSID(const WCHAR *format,CLSID *pClsid);
	// 图片保存
	//static bool		SaveImg(const char* lpFile,Image *lpImg,int iImg=IMG_PNG);
	static bool		SaveImg(const char* lpFile,Bitmap *lpImg);
	static CString	GetDlgPath(HWND hWnd);
	// 图像缩放
	static CRect	ConstantRatioWH(long lNewW,long lNewH,long lSrcW,long lSrcH,BOOL bCenter);
	// 字符转换
	static const char*	ToASCII(const wchar_t *pwIn);
	static wchar_t* ToUNICODE(const char *pszIn);
	static const char*	toGB2312(const char *pszIn);
	// 字符格式化
	static const char*	sprintf_s(LPCSTR fmt,...);
	static wchar_t* sprintf_w(LPCSTR fmt,...);
	static int		strleng(LPSTR buf);
	static int		strleng(wchar_t* buf);
	// 获取字符串 长度 根据字体大小
	static int		CompLengFt(CString strBuf,int iFontSize);
	// 内存转换
	static bool		mem_to_global(const void *buf, size_t size,HGLOBAL global);
	static bool	    stream_to_mem(IStream *stream, void **outbuf, size_t *size);
	// 比较数据
	static int		CompareInt(int iLeft,int iRight);
	/// 文件操作
	// 取文件长度
	static int  GetFileLength(const char *name);	
	// 判断文件是否存在
	static bool CheckFile(const char *name);
	// 由文件名获取文件路径 文件名+路径
	static const char *GetFilePath(const char *filename);
	// 从文件名含路径分离出文件名不含路径
	static const char* GetFileName(const char* filename);
	// 获取后缀名
	static CString GetFileExt(const char* filename);
	// 获取标题
	static CString GetFileTitle(const char* filename);
	// 获取目录下的所有文件
	static bool	 GetFileByPath(const char* path,FileMapPtr &mapFile,int iLv=LV_ALL);

	// 获取打开的目录
	static bool  OpenFilePath(HWND hWnd,CString &strPath,CString &strFolder);

	/// 根据符号分割字符串
	static CString analyStringFind(const CString& str, const char* beginfd, const char* endfd, int& of);
	static CString analyString(CString &str, const char *tp);
	static int   analyInt(CString &str, const char *tp);
	static DWORD   analyDword(CString &str, const char *tp);

	static std::string analyStringFind(const std::string& str, const char* beginfd, const char* endfd, int& of);
	static std::string analyString(std::string &str, const char* tp);
	static int   analyInt(std::string &str, const char *tp);

	/// 分割括号[] 成对 
	static CString analySign(CString &str,char *fmt,int &iMid);

	// vector字符串 排序
	static bool	 SortbyString(const CString& x, const CString& y);

	/// 注册热键
	static int	 RegisterHotKey(HWND hWnd,int id,UINT fsMod1,UINT fsMod2,UINT vk,BOOL bSys=true);
	/// 注销
	static void	 UnregisterHotKey(HWND hWnd,int id,BOOL bSys=true);
	/// 注销全部
	static void	 UnregisterHotKeyAll(void);
	/// 运行热键检测
	static void	 RunHotKey(UINT vk);
	/// 根据字符串获取快捷键
	static int	 GetHotKey(CString &strKey);
	/// 根据ID获取快捷键
	static const char* GetHotKey(int vk);

	/// GDI+渲染字体 路径渲染
	static void	 DrawString(HDC hDC,WCHAR *lpText,RectF &rect,Color col,WCHAR *lpFont,int iSize,INT style=FontStyleRegular);
	// gdi 在空间picture上画图
	static void DrawImage(CWnd* self, int id, const CString& res);

	// WIndow图标加载
	static HANDLE	LoadImage(LPCSTR name,UINT type);
	// 获取当前系统时间
	template< class _TYPE >
	static _TYPE	Time(int iTM=TM_SysTime);
	// 年，月，日，与当前时间比较
	static bool		CompTime(WORD wYear,WORD wMonth,WORD wDay);
	// 小时，分钟，与当前时间比较
	static bool		CompTime(WORD wHour,WORD wMinute);
	// 转换日期	
	static const char*	AnalyTime(int iDayOfWeek);
	// 图片灰度处理
	static Bitmap*	GrayImg(Bitmap *lpImg,int algo=GRAY_WEIGHTAVERAGE);

	// 隐藏任务栏
	static void		HideTaskBar(BOOL bHide);
	// 切换桌面
	static void		ToggleDesktop(void);
	// 画边框
	static void		XorBorder(HWND hWnd);
	// 显示Dlg只能输入数字
	static BOOL		ModifyDlgNumer(CWnd *lpWnd, bool bNumer);
	// 调整控件宽度
	static void		ResetCtrlWidth(CComboBox *lpCtrl, int off=10);

	// ini 配置文件加载
	static bool			Ini_SetFile(const char *file);
	// 查找文件中是否存在
	static bool			Ini_Check(const char *section, const char *name);
	// 写文件
	static void			Ini_Wirte(const char *szFormat, ...);
	// 读写，Int
	static void			Ini_SetInt(const char *section, const char *name, int value);
	static int 			Ini_GetInt(const char *section, const char *name);
	// 读写，Float
	static void			Ini_SetFloat(const char *section, const char *name, float value);
	static float		Ini_GetFloat(const char *section, const char *name);
	// 读写，文本
	static void			Ini_SetString(const char *section, const char *name, const char *value);
	static const char*		Ini_GetString(const char *section, const char *name);
	// 读写，Vector int-代表是Vector几
	template< class _TYPE >
	static void			Ini_SetVector(const char *section, const char *name, _TYPE &Vtor, int iVtor=2);
	template< class _TYPE >
	static _TYPE		Ini_GetVector(const char *section, const char *name, int iVtor=2);
	// 读写，VectorArry 组
	static void			Ini_SetVectorArry(const char *section, const char *name, IniMapPtr& mapIni);
	static IniMapPtr&	Ini_GetVectorArry(const char *section, const char *name);

	// 添加Dlg组件
	static void			Dlg_Push(int iDD,THDialog *dlg);
	// 获取
	static THDialog*	Dlg_Get(int iDD);
	// 显示
	static void			Dlg_Show(int iDD);
	// 释放
	static void			Dlg_Release(int iDD=-1);

	// 创建加载 提示Gif动画
	static THPopWindow* Dlg_GifTs(int iDD,CWnd *lpParent);

	/// 附加操作

	// 计算宽高 比例 1024:768 CSize=4:3
	//static CSize		WindowWHRatio(long lW,long lH);
	// 获取当前分辨率
	static CSize		WindowScreenFmt(void);
	// 设置鼠标状态
	static int			CursorState(int iCr=CR_MOVE);
	static int			GetCursorState(void);
	// 鼠标是否静止 多久时间
	static bool			CursorSuspend(DWORD dwTime);
	// 鼠标激活 活跃的
	static bool			CursorForce(void);
	// 获取桌面窗口
	static HWND			FindDesktopWnd(void); 
	// 是否是前台窗口
	static bool			ForegroundWindow(HWND hWnd);
	// 把窗口钉在桌面
	static void			PegWindowDesk(HWND hWnd);
	// 是否是前台窗口 进程名,id
	static bool			ForegroundWindow(LPCTSTR strName);
	static bool			ForegroundWindow(DWORD dwProcessId);
	// 更换壁纸
	static bool			BeginDesktop(void);
	static bool			SetWallPaper(CString strPaper,DWORD dwStyle);
	static bool			EndDesktop(void);

	// 枚举窗口 进程名,id bShow 可见窗口
	static HwndMapPtr&  EnumWindows(LPCTSTR strName,bool bShow=true);
	static HwndMapPtr&  EnumWindows(DWORD dwProcessId,bool bShow=true);
	// 显示窗口
	static bool			AppShowWindow(int iCmd);
	// 记录窗口
	static bool			RecordWindow(HWND hWnd);
	static bool			RecordEmpty(void);
	// 是否记录的是自己窗口
	static bool			RecordMe(HWND hWnd);

	// 运行外部程序
	static	bool		ShellExecute(const CString& open, const CString& param="");
	// 返回目标进程句柄hProcess，退出需要关闭
	static	DWORD		ShellExecuteEx(const CString& open, const CString& param = "");
	// 隐藏/显示程序窗口 根据进程名或窗口句柄
	static  bool		ShowWindow(LPCTSTR strName,int iCmd);
	static  bool		ShowWindow(HWND hWnd,int iCmd);

	/// 进程窗口操作

	// 通过进程名或id,得到主窗口句柄.
	static HwndMapPtr&	GetProcessHWND(LPCTSTR strName);
	static HwndMapPtr&	GetProcessHWND(DWORD dwProcessId);
	// 通过标题或进程ID找到 窗口句柄
	static HWND			GetHwndByTitleID(const char*lpTitile,DWORD dwProcessId);
	// 通过进程名或id,返回主线程ID
	static DWORD		GetThreadId(LPCTSTR strName);
	static DWORD		GetThreadId(DWORD dwProcessId);
	// 获取进程ID 通过进程名
	static DWORD		GetProcessId(LPCTSTR strName);
	// 通过窗口或进程id获取程序路径
	static const char*		GetFileExe(HWND hWnd);
	static const char*		GetFileExe(DWORD dwProcessId);
	// 获取所有进程信息
	static ProcessMapPtr& BeginProcess(void);
	static void			EndProcess(void);
	// 获取所有活动的窗口
	static HwndMapPtr&	BeginHwnd(bool bFilter=true);
	static void			EndHwnd(void);
	// 进程是否存在 通过进程名或id
	static bool			IsProcessExist(LPCTSTR strName);
	static bool			IsProcessExist(DWORD dwProcessId);
	// 结束进程 根据进程名或id
	static bool			ExitProcess(LPCTSTR strName);
	static bool			ExitProcess(DWORD dwProcessId);
	// 根据窗口结束
	static bool			ExitProcess(HWND hWnd);

	// 当前线程休眠delay
	static void			ThreadDelay(int delay);
	static void			ThreadDelayRandom(int max);

	/// 系统操作
	// 判断是否是自己窗口
	static bool			IsWindowMe(HWND hWnd);
	// 提升权限
	static bool			AdjustPrivileges(LPCSTR lpNameSE);
	// 关机
	static bool			ExitWindow(int iEt);
	// 截图
	static Image*		PrintScreen(HWND hWnd,bool bDesktop,bool bJQ,const char* lpFile=NULL);


	// 从注册表获取程序 路径
	static CString		GetExePathByReg(const char* valueName);


	// 加解密
public:
	// Base64
	// 字符串加密
	static CString		Base64EncodeStr(LPCTSTR lpszSrc);
	// 解密
	static CString		Base64DecodeStr(LPCTSTR lpszSrc);

	// 自定义加密
	static CString		EncodeString(const CString& str, const CString& key);
	static CString		DecodeString(const CString& str, const CString& key);

	// xxtea
	static CString		XxteaEncodeStr(const CString& str, const CString& key = "img");
	static CString		XxteaDecodeStr(const CString& str, const CString& key = "img");
	static void			XxteaEncodeByte(thByteStream& sbyte, const CString& key = "img");
	static void			XxteaDecodeByte(thByteStream& sbyte, const CString& key = "img");


	// 外部调用函数
public:
	// 时间格式化
	static CString		TimeFormat(float fPos,float fLeng);
	/// 加载数据到内存 begin
	static void*		Resource_Load(const char *filename, DWORD *size=0);
	static void			Resource_Free(void *res);
	/// 加载数据到内存 end
	/// 加载数据到缓存thByteStream
	static thByteStream*		ResourceByte_Load(const char *filename, DWORD *size=0);
	static void			ResourceByte_Free(thByteStream *res);
	/// 加载数据到缓存CString
	static CString		ResourceString_Load(const char *filename, DWORD *size = 0);
	// 创建文件夹
	static void			CreateDirectory(const char *path);
	/// 创建文件
	static bool			CreateFile(const char *filename);
	/// 数据写入文件
	static void			ByteToFile(const char *filename,thByteStream *lpData,const char * _Mode/*="wb或ab"*/);
	// 播放音乐
	static void			Sound_Play(const char* file);
	static void			Sound_PlayEx(const char* file, int volume=100, int pan=0, float pitch=1.0f, bool loop=false);
	static bool			Sound_IsPlaying(const char* file);
	static bool			Sound_IsStopped(const char* file);
	static void			Sound_Resume(const char* file);
	static void			Sound_Pause(const char* file);
	static void			Sound_Stop(const char* file);

	static float		Sound_GetLength(const char* file);
	static float		Sound_GetPos(const char* file);
	static void			Sound_SetVolume(const char* file, int volume);
	static void			Sound_SetPos(const char* file, float fSeconds);

	// time 计时器
public:
	static  void		timeCall(int time, const std::function<void(void)>& call);


public:
	// 随机
	static int			random_int(int min, int max);
	static float		random_float(float min, float max);


	// Exe封包中导出资源
public:
	static thByteStream&		exeResToByte(int resId, const CString& resType);
	static void			exeResToFile(int resId, const CString& resType, const CString& file);

	// 退出需要删除的本地文件
public:
	// 添加
	static void			pushExitTempFile(const CString& file);
	// 清理
	static void			exitTempFile(void);


protected:
	static thHelper*	s_lpTH;
	// ini
	static CString		s_strIniFile;
	// 日记
	static CString		s_strLogFile;
	// 路径
	static CString		s_strAppPath;
	static CString		s_strTemp;
	static char			s_szChar[BUF_SIZE_MAINMAX];
	static wchar_t		s_wChar[BUF_SIZE_MAINMAX];
	static thByteStream		s_tempByte;
	// 互斥量
	static HANDLE		s_hMutex;	
	// 桌面控制
//	static IActiveDesktop* s_lpDesktop;
	thTimer				m_thTimer;	// 时间控制器
	int					m_iCursorSt;// 鼠标状态
	CPoint				m_ptCurPos;	// 记录鼠标位置

	// 退出删除文件列表
	static std::vector<CString> s_exitFile;
protected:
	IniMapPtr	m_mapIni;		// ini
	// hWnd
	HwndMapPtr	m_mapAppHwnd;
	HwndMapPtr	m_mapHwnd;
	HwndMapPtr	m_mapHwndRec;	// 记录功能
	SoundMapPtr	m_mapSound;		// sound
	DlgMapPtr	m_mapDlg;		// Dlg组件
	HotKeyMapPtr  m_mapHotKey;  // hotkey
	ProcessMapPtr m_mapProcess; // process

	// 计时器
	struct thDEC_timeCall
	{
		thDEC_timeCall(int _time, const std::function<void(void)>&_call)
			:time(_time),call(_call)
		{
		}
		int time;
		std::function<void(void)> call;
	};
	std::vector<thDEC_timeCall> m_vCallTime;
	std::vector<std::function<void(void)>> m_vCallTimeC;

public:
	// sm
//	THSMManager	 m_SMManager;
	// 报时
//	THTimeBS	 m_TimeBS;
	// cursor
	static HICON s_curPt;	
	// LayeredWindow
	//static LayeredWindow UpdateLayeredWindow;
};

//////////////////////////////////////////////////////////////////////////
// inlne
template< class _TYPE >
TH_INLINE _TYPE thHelper::Time( int iTM/*=TM_SysTime*/ )
{
	_TYPE	temp;
	SYSTEMTIME	SystemTime;
	GetLocalTime( &SystemTime );
	switch (iTM)
	{
	case TM_SysTime: temp = *(_TYPE*)&SystemTime;
		break;
	case TM_Year:	temp = *(_TYPE*)&SystemTime.wYear;
		break;
	case TM_Month:	temp = *(_TYPE*)&SystemTime.wMonth;
		break;
	case TM_Day:	temp = *(_TYPE*)&SystemTime.wDay;
		break;
	case TM_Hour:	temp = *(_TYPE*)&SystemTime.wHour;
		break;
	case TM_Minute:	temp = *(_TYPE*)&SystemTime.wMinute;
		break;
	case TM_Second:	temp = *(_TYPE*)&SystemTime.wSecond;
		break;
	}
	return temp;
}

template< class _TYPE >
TH_INLINE void thHelper::Ini_SetVector( const char *section, const char *name, _TYPE &Vtor, int iVtor/*=2*/ )
{
	if (iVtor==2) s_strTemp.Format("%.3f,%.3f", Vtor.x,Vtor.y);
	else (iVtor==3) s_strTemp.Format("%.3f,%.3f,%.3f", Vtor.x,Vtor.y,Vtor.z);
	else (iVtor==4) s_strTemp.Format("%.3f,%.3f,%.3f,%.3f", Vtor.left,Vtor.top,Vtor.right,Vtor.bottom);
	else return;

	return Ini_SetString(section,name,s_strTemp);
}

template< class _TYPE >
TH_INLINE _TYPE thHelper::Ini_GetVector( const char *section, const char *name, int iVtor/*=2*/ )
{
	_TYPE _Vector;
	Ini_GetVectorArry(section,name);
	if (m_mapIni.size()==iVtor)
	{
		if (iVtor==2) _Vector.SetVector(m_mapIni[0],m_mapIni[1]);
		else (iVtor==3) _Vector.SetVector(m_mapIni[0],m_mapIni[1],m_mapIni[2]); 
		else (iVtor==4) _Vector.SetVector(m_mapIni[0],m_mapIni[1],m_mapIni[2],m_mapIni[3]); 
	}
	return _Vector;
}

//
//void CUIDFromString(CString strGuid,GUID &Guid)
//{
//	if (strGuid.IsEmpty()) return;
//	//static const GUID IID_ISimpleMath = 
//	//{0xD3D9D58B,0x45B5,0x48AB,{0xb1,0x99,0xB8,0xC4,0x05,0x60,0xAE,0xC7}};
//	//CString strGUID  = "D3D9D58B-45B5-48AB-B199-B8C40560AEC7";
//	CString strTemp = thHelper::AnalyString(strGuid,"-");
//	CString strQG;
//	_stscanf( strTemp, _T("%x"), &Guid.Data1 );
//	strTemp = thHelper::AnalyString(strGuid,"-");
//	_stscanf( strTemp, _T("%x"), &Guid.Data2 );
//	strTemp = thHelper::AnalyString(strGuid,"-");
//	_stscanf( strTemp, _T("%x"), &Guid.Data3 );
//	strTemp = thHelper::AnalyString(strGuid,"-");
//	strQG = strTemp.Mid(0,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[0] );
//	strQG = strTemp.Mid(2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[1] );
//	strTemp = thHelper::AnalyString(strGuid,"-");
//	if (strTemp.IsEmpty()) strTemp = strGuid;
//	int iOffset = 0;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[2] );
//	iOffset += 2;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[3] );
//	iOffset += 2;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[4] );
//	iOffset += 2;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[5] );
//	iOffset += 2;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[6] );
//	iOffset += 2;
//	strQG = strTemp.Mid(iOffset,2);
//	_stscanf( strQG, _T("%x"), &Guid.Data4[7] );
//}

