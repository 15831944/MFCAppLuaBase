#pragma once
#include "thStruct.h"
// #include "bass/THSMManager.h"
#include <ShlObj.h> 
#include <functional>

typedef BOOL (WINAPI *LayeredWindow)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD); 
// time
enum
{
	TM_SysTime,	// ϵͳʱ��
	TM_Year,	// ��
	TM_Month,	// ��
	TM_Day,		// ��
	TM_Hour,	// Сʱ
	TM_Minute,	// ����
	TM_Second,	// ��
};

// exitwindow
enum
{
	ET_SHUTDOWN, // �����ػ�
	ET_POWEROFF, // ǿ�ƹػ�
	ET_REBOOT,	 // ����
	ET_LOGOFF,	 // ע��
	ET_IDLE,	 // ����
	ET_OFF,		 // ����
	ET_SUOPING,	 // ����
	ET_MONITORPOWER,// �ر���ʾ��
	ET_SCREENSAVE,	// ��������
};

// ��ͼ
enum
{
	PS_FULLSCREEN,	// ȫ����ͼ
	PS_WINDOW,		// ���ڽ�ͼ
};

// ��ȡ�ļ�
enum
{
	LV_ALL,	// ��ȡ�����ļ�
	LV_IMG,	// ��ȡͼƬ�ļ�
	LV_VIDEO,// ��Ƶ�ļ�
	LV_SOUND,// ��Ƶ�ļ�
	LV_SWF,// SWF�ļ�
};

// ���״̬
enum
{
	CR_MOVE,	// �ƶ�״̬
	CR_STOP,	// ֹͣ
	CR_INCTRL,	// ����ؼ�״̬
};

#define GRAY_AVERAGE		0
#define GRAY_MAX			1
#define GRAY_WEIGHTAVERAGE	2

// ���ܲ�������
class CxImage;
class THPopWindow;
class thHelper
{
public:
	thHelper(void);
	// Sound,Musicʵ��
	// static THSMManager* SM(void) {return &(s_lpTH->m_SMManager);}
	// ��ʱ����
	// static THTimeBS*	BS(void) {return &(s_lpTH->m_TimeBS);}

public:
	// �жϵ�ǰ�����Ƿ��Ѿ�������. ��ֹ�࿪
	static bool		IsAppExist(void);
	// ʵ����
	static void		AppInit(void);
	// ����
	static void		AppRun(int delay);
	// �˳�ʱ������� 
	static void		AppExit(void);

	// �ռ�
	static void		Log_Create(const char *filename);
	static void		Log_Write(const char *szFormat, ...);
	static void		Log_WriteTime(const char *szFormat, ...);
	static void		Log_Print(const char *szFormat, ...);
	// ·��ö��
	static CString	MakePath_ASCII(const char *filename=0);
	static wchar_t* MakePath_UNICODE(const char *filename=0);
	// ö�ٻ���·��
	static CString	MakeTempPath(const char *filename=0);
	// ȥ��·��
	static CString	AnalyPath(const char *Pathfile=0);
	// ����ͼƬGdi+ �ڴ�
	static Bitmap*	LoadImgFromFile(const char *filename, DWORD size=0);
	static Bitmap*	LoadImgFromMem(const char *data, DWORD size);
	// ����ͼƬCxImageת����GDI+Bitmap ����֧����ʾCxImage��ȫ��ͼƬ
	static Bitmap*	LoadImgFromCxImg(const TCHAR * filename, DWORD imagetype=0);
	// ת��RGB
	static void		RGB2ARGB(Bitmap **pbm);
	// GDI+bitmap ת CxImage
	static BOOL		Bitmap2Ximage(Bitmap *pBitmap, CxImage &xImage);
	// CxImage ת GDI+bitmap
	static Bitmap*  Ximage2Bitmap(CxImage *pXimage);
	// ��ȡͼƬ֡�� �����ж��Ƿ���GifͼƬ
	static UINT		GetFrameCount(Bitmap* pimage);
	// ��ȡCLSID
	static bool		GetEncoderCLSID(const WCHAR *format,CLSID *pClsid);
	// ͼƬ����
	//static bool		SaveImg(const char* lpFile,Image *lpImg,int iImg=IMG_PNG);
	static bool		SaveImg(const char* lpFile,Bitmap *lpImg);
	static CString	GetDlgPath(HWND hWnd);
	// ͼ������
	static CRect	ConstantRatioWH(long lNewW,long lNewH,long lSrcW,long lSrcH,BOOL bCenter);
	// �ַ�ת��
	static const char*	ToASCII(const wchar_t *pwIn);
	static wchar_t* ToUNICODE(const char *pszIn);
	static const char*	toGB2312(const char *pszIn);
	// �ַ���ʽ��
	static const char*	sprintf_s(LPCSTR fmt,...);
	static wchar_t* sprintf_w(LPCSTR fmt,...);
	static int		strleng(LPSTR buf);
	static int		strleng(wchar_t* buf);
	// ��ȡ�ַ��� ���� ���������С
	static int		CompLengFt(CString strBuf,int iFontSize);
	// �ڴ�ת��
	static bool		mem_to_global(const void *buf, size_t size,HGLOBAL global);
	static bool	    stream_to_mem(IStream *stream, void **outbuf, size_t *size);
	// �Ƚ�����
	static int		CompareInt(int iLeft,int iRight);
	/// �ļ�����
	// ȡ�ļ�����
	static int  GetFileLength(const char *name);	
	// �ж��ļ��Ƿ����
	static bool CheckFile(const char *name);
	// ���ļ�����ȡ�ļ�·�� �ļ���+·��
	static const char *GetFilePath(const char *filename);
	// ���ļ�����·��������ļ�������·��
	static const char* GetFileName(const char* filename);
	// ��ȡ��׺��
	static CString GetFileExt(const char* filename);
	// ��ȡ����
	static CString GetFileTitle(const char* filename);
	// ��ȡĿ¼�µ������ļ�
	static bool	 GetFileByPath(const char* path,FileMapPtr &mapFile,int iLv=LV_ALL);

	// ��ȡ�򿪵�Ŀ¼
	static bool  OpenFilePath(HWND hWnd,CString &strPath,CString &strFolder);

	/// ���ݷ��ŷָ��ַ���
	static CString analyStringFind(const CString& str, const char* beginfd, const char* endfd, int& of);
	static CString analyString(CString &str, const char *tp);
	static int   analyInt(CString &str, const char *tp);
	static DWORD   analyDword(CString &str, const char *tp);

	static std::string analyStringFind(const std::string& str, const char* beginfd, const char* endfd, int& of);
	static std::string analyString(std::string &str, const char* tp);
	static int   analyInt(std::string &str, const char *tp);

	/// �ָ�����[] �ɶ� 
	static CString analySign(CString &str,char *fmt,int &iMid);

	// vector�ַ��� ����
	static bool	 SortbyString(const CString& x, const CString& y);

	/// ע���ȼ�
	static int	 RegisterHotKey(HWND hWnd,int id,UINT fsMod1,UINT fsMod2,UINT vk,BOOL bSys=true);
	/// ע��
	static void	 UnregisterHotKey(HWND hWnd,int id,BOOL bSys=true);
	/// ע��ȫ��
	static void	 UnregisterHotKeyAll(void);
	/// �����ȼ����
	static void	 RunHotKey(UINT vk);
	/// �����ַ�����ȡ��ݼ�
	static int	 GetHotKey(CString &strKey);
	/// ����ID��ȡ��ݼ�
	static const char* GetHotKey(int vk);

	/// GDI+��Ⱦ���� ·����Ⱦ
	static void	 DrawString(HDC hDC,WCHAR *lpText,RectF &rect,Color col,WCHAR *lpFont,int iSize,INT style=FontStyleRegular);
	// gdi �ڿռ�picture�ϻ�ͼ
	static void DrawImage(CWnd* self, int id, const CString& res);

	// WIndowͼ�����
	static HANDLE	LoadImage(LPCSTR name,UINT type);
	// ��ȡ��ǰϵͳʱ��
	template< class _TYPE >
	static _TYPE	Time(int iTM=TM_SysTime);
	// �꣬�£��գ��뵱ǰʱ��Ƚ�
	static bool		CompTime(WORD wYear,WORD wMonth,WORD wDay);
	// Сʱ�����ӣ��뵱ǰʱ��Ƚ�
	static bool		CompTime(WORD wHour,WORD wMinute);
	// ת������	
	static const char*	AnalyTime(int iDayOfWeek);
	// ͼƬ�Ҷȴ���
	static Bitmap*	GrayImg(Bitmap *lpImg,int algo=GRAY_WEIGHTAVERAGE);

	// ����������
	static void		HideTaskBar(BOOL bHide);
	// �л�����
	static void		ToggleDesktop(void);
	// ���߿�
	static void		XorBorder(HWND hWnd);
	// ��ʾDlgֻ����������
	static BOOL		ModifyDlgNumer(CWnd *lpWnd, bool bNumer);
	// �����ؼ����
	static void		ResetCtrlWidth(CComboBox *lpCtrl, int off=10);

	// ini �����ļ�����
	static bool			Ini_SetFile(const char *file);
	// �����ļ����Ƿ����
	static bool			Ini_Check(const char *section, const char *name);
	// д�ļ�
	static void			Ini_Wirte(const char *szFormat, ...);
	// ��д��Int
	static void			Ini_SetInt(const char *section, const char *name, int value);
	static int 			Ini_GetInt(const char *section, const char *name);
	// ��д��Float
	static void			Ini_SetFloat(const char *section, const char *name, float value);
	static float		Ini_GetFloat(const char *section, const char *name);
	// ��д���ı�
	static void			Ini_SetString(const char *section, const char *name, const char *value);
	static const char*		Ini_GetString(const char *section, const char *name);
	// ��д��Vector int-������Vector��
	template< class _TYPE >
	static void			Ini_SetVector(const char *section, const char *name, _TYPE &Vtor, int iVtor=2);
	template< class _TYPE >
	static _TYPE		Ini_GetVector(const char *section, const char *name, int iVtor=2);
	// ��д��VectorArry ��
	static void			Ini_SetVectorArry(const char *section, const char *name, IniMapPtr& mapIni);
	static IniMapPtr&	Ini_GetVectorArry(const char *section, const char *name);

	// ���Dlg���
	static void			Dlg_Push(int iDD,THDialog *dlg);
	// ��ȡ
	static THDialog*	Dlg_Get(int iDD);
	// ��ʾ
	static void			Dlg_Show(int iDD);
	// �ͷ�
	static void			Dlg_Release(int iDD=-1);

	// �������� ��ʾGif����
	static THPopWindow* Dlg_GifTs(int iDD,CWnd *lpParent);

	/// ���Ӳ���

	// ������ ���� 1024:768 CSize=4:3
	//static CSize		WindowWHRatio(long lW,long lH);
	// ��ȡ��ǰ�ֱ���
	static CSize		WindowScreenFmt(void);
	// �������״̬
	static int			CursorState(int iCr=CR_MOVE);
	static int			GetCursorState(void);
	// ����Ƿ�ֹ ���ʱ��
	static bool			CursorSuspend(DWORD dwTime);
	// ��꼤�� ��Ծ��
	static bool			CursorForce(void);
	// ��ȡ���洰��
	static HWND			FindDesktopWnd(void); 
	// �Ƿ���ǰ̨����
	static bool			ForegroundWindow(HWND hWnd);
	// �Ѵ��ڶ�������
	static void			PegWindowDesk(HWND hWnd);
	// �Ƿ���ǰ̨���� ������,id
	static bool			ForegroundWindow(LPCTSTR strName);
	static bool			ForegroundWindow(DWORD dwProcessId);
	// ������ֽ
	static bool			BeginDesktop(void);
	static bool			SetWallPaper(CString strPaper,DWORD dwStyle);
	static bool			EndDesktop(void);

	// ö�ٴ��� ������,id bShow �ɼ�����
	static HwndMapPtr&  EnumWindows(LPCTSTR strName,bool bShow=true);
	static HwndMapPtr&  EnumWindows(DWORD dwProcessId,bool bShow=true);
	// ��ʾ����
	static bool			AppShowWindow(int iCmd);
	// ��¼����
	static bool			RecordWindow(HWND hWnd);
	static bool			RecordEmpty(void);
	// �Ƿ��¼�����Լ�����
	static bool			RecordMe(HWND hWnd);

	// �����ⲿ����
	static	bool		ShellExecute(const CString& open, const CString& param="");
	// ����Ŀ����̾��hProcess���˳���Ҫ�ر�
	static	DWORD		ShellExecuteEx(const CString& open, const CString& param = "");
	// ����/��ʾ���򴰿� ���ݽ������򴰿ھ��
	static  bool		ShowWindow(LPCTSTR strName,int iCmd);
	static  bool		ShowWindow(HWND hWnd,int iCmd);

	/// ���̴��ڲ���

	// ͨ����������id,�õ������ھ��.
	static HwndMapPtr&	GetProcessHWND(LPCTSTR strName);
	static HwndMapPtr&	GetProcessHWND(DWORD dwProcessId);
	// ͨ����������ID�ҵ� ���ھ��
	static HWND			GetHwndByTitleID(const char*lpTitile,DWORD dwProcessId);
	// ͨ����������id,�������߳�ID
	static DWORD		GetThreadId(LPCTSTR strName);
	static DWORD		GetThreadId(DWORD dwProcessId);
	// ��ȡ����ID ͨ��������
	static DWORD		GetProcessId(LPCTSTR strName);
	// ͨ�����ڻ����id��ȡ����·��
	static const char*		GetFileExe(HWND hWnd);
	static const char*		GetFileExe(DWORD dwProcessId);
	// ��ȡ���н�����Ϣ
	static ProcessMapPtr& BeginProcess(void);
	static void			EndProcess(void);
	// ��ȡ���л�Ĵ���
	static HwndMapPtr&	BeginHwnd(bool bFilter=true);
	static void			EndHwnd(void);
	// �����Ƿ���� ͨ����������id
	static bool			IsProcessExist(LPCTSTR strName);
	static bool			IsProcessExist(DWORD dwProcessId);
	// �������� ���ݽ�������id
	static bool			ExitProcess(LPCTSTR strName);
	static bool			ExitProcess(DWORD dwProcessId);
	// ���ݴ��ڽ���
	static bool			ExitProcess(HWND hWnd);

	// ��ǰ�߳�����delay
	static void			ThreadDelay(int delay);
	static void			ThreadDelayRandom(int max);

	/// ϵͳ����
	// �ж��Ƿ����Լ�����
	static bool			IsWindowMe(HWND hWnd);
	// ����Ȩ��
	static bool			AdjustPrivileges(LPCSTR lpNameSE);
	// �ػ�
	static bool			ExitWindow(int iEt);
	// ��ͼ
	static Image*		PrintScreen(HWND hWnd,bool bDesktop,bool bJQ,const char* lpFile=NULL);


	// ��ע����ȡ���� ·��
	static CString		GetExePathByReg(const char* valueName);


	// �ӽ���
public:
	// Base64
	// �ַ�������
	static CString		Base64EncodeStr(LPCTSTR lpszSrc);
	// ����
	static CString		Base64DecodeStr(LPCTSTR lpszSrc);

	// �Զ������
	static CString		EncodeString(const CString& str, const CString& key);
	static CString		DecodeString(const CString& str, const CString& key);

	// xxtea
	static CString		XxteaEncodeStr(const CString& str, const CString& key = "img");
	static CString		XxteaDecodeStr(const CString& str, const CString& key = "img");
	static void			XxteaEncodeByte(thByteStream& sbyte, const CString& key = "img");
	static void			XxteaDecodeByte(thByteStream& sbyte, const CString& key = "img");


	// �ⲿ���ú���
public:
	// ʱ���ʽ��
	static CString		TimeFormat(float fPos,float fLeng);
	/// �������ݵ��ڴ� begin
	static void*		Resource_Load(const char *filename, DWORD *size=0);
	static void			Resource_Free(void *res);
	/// �������ݵ��ڴ� end
	/// �������ݵ�����thByteStream
	static thByteStream*		ResourceByte_Load(const char *filename, DWORD *size=0);
	static void			ResourceByte_Free(thByteStream *res);
	/// �������ݵ�����CString
	static CString		ResourceString_Load(const char *filename, DWORD *size = 0);
	// �����ļ���
	static void			CreateDirectory(const char *path);
	/// �����ļ�
	static bool			CreateFile(const char *filename);
	/// ����д���ļ�
	static void			ByteToFile(const char *filename,thByteStream *lpData,const char * _Mode/*="wb��ab"*/);
	// ��������
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

	// time ��ʱ��
public:
	static  void		timeCall(int time, const std::function<void(void)>& call);


public:
	// ���
	static int			random_int(int min, int max);
	static float		random_float(float min, float max);


	// Exe����е�����Դ
public:
	static thByteStream&		exeResToByte(int resId, const CString& resType);
	static void			exeResToFile(int resId, const CString& resType, const CString& file);

	// �˳���Ҫɾ���ı����ļ�
public:
	// ���
	static void			pushExitTempFile(const CString& file);
	// ����
	static void			exitTempFile(void);


protected:
	static thHelper*	s_lpTH;
	// ini
	static CString		s_strIniFile;
	// �ռ�
	static CString		s_strLogFile;
	// ·��
	static CString		s_strAppPath;
	static CString		s_strTemp;
	static char			s_szChar[BUF_SIZE_MAINMAX];
	static wchar_t		s_wChar[BUF_SIZE_MAINMAX];
	static thByteStream		s_tempByte;
	// ������
	static HANDLE		s_hMutex;	
	// �������
//	static IActiveDesktop* s_lpDesktop;
	thTimer				m_thTimer;	// ʱ�������
	int					m_iCursorSt;// ���״̬
	CPoint				m_ptCurPos;	// ��¼���λ��

	// �˳�ɾ���ļ��б�
	static std::vector<CString> s_exitFile;
protected:
	IniMapPtr	m_mapIni;		// ini
	// hWnd
	HwndMapPtr	m_mapAppHwnd;
	HwndMapPtr	m_mapHwnd;
	HwndMapPtr	m_mapHwndRec;	// ��¼����
	SoundMapPtr	m_mapSound;		// sound
	DlgMapPtr	m_mapDlg;		// Dlg���
	HotKeyMapPtr  m_mapHotKey;  // hotkey
	ProcessMapPtr m_mapProcess; // process

	// ��ʱ��
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
	// ��ʱ
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

