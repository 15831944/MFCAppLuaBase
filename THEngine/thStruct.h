/*************************************
*
*   �ļ���THStruct.h
*
*   �������ṹ�� ���ݶ����ļ�
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
// ��Ч
struct stSound 
{
	DWORD	sound;
	thTimer	thTime;
	DWORD   chn;
};
typedef std::map<CString,stSound> SoundMapPtr;

// hwnd
typedef std::vector<HWND>	HwndMapPtr;

// Dlg���
class THDialog;
struct stDlgZJ 
{
	stDlgZJ(){memset(this,0,sizeof(*this));}
	THDialog *pdlg;
	bool	 bDelete;
};
typedef	std::map<int,stDlgZJ> DlgMapPtr;

// �߳�
struct thByteStream;
struct stThread 
{
	HANDLE		 _ThrHand;	 // ���
	unsigned int _dwThreadID;// �߳�ID
	int			 _iTdID;	 // �̱߳�ʶ
	THDialog	 *_lpWnd;	 // ����ָ��
	thByteStream _thParams;	 // �����̲߳���
};
typedef	std::map<int,stThread> ThreadMapPtr;
typedef	std::vector<int>	   ClearMapPtr;

// ��Դ����
struct stReLoad 
{
	stReLoad(){memset(this,0,sizeof(*this));}
	char	szLoadPath[MAX_PATH];
	int		iLoadType;
	int		iResType;
};

// ��¼ʱ��
struct stRecord
{
	stRecord(){memset(this,0,sizeof(*this));}
	int	 iYear;		// ��
	int	 iMonth;	// ��
	int	 iDay;		// ��

	int	 iHour;		// Сʱ
	int	 iMinute;	// ����

	bool bProcess;  // ����
	bool bHwnd;		// ����
	//char szProcess[1024];	// ����
	//char szHwnd[1024];		// ����
};
typedef	std::vector<stRecord> RecordMapPtr;

// ���̴�����Ϣ
struct stProcess 
{
	//stProcess(){memset(this,0,sizeof(*this));}
	HwndMapPtr	mapHwnd;			// ���ھ��
	DWORD		th32ProcessID;		// ����id
	char		szExePath[MAX_PATH];// ����EXE·��
};
typedef std::map<CString,stProcess> ProcessMapPtr;

// �򿪴����߳���Ϣ
struct stPopThread 
{
	stPopThread(){memset(this,0,sizeof(*this));}
	int  iType;
	int  iWDType;
	char szPath[MAX_PATH];
};

// �ȼ���Ϣ
struct stHotKey
{
	stHotKey(){memset(this,0,sizeof(*this));}
	HWND	hWnd;		// ����
	UINT	nGroupkey1;	// �ȼ� ��ϼ�1
	UINT	nGroupkey2;	// �ȼ� ��ϼ�2
	UINT	nHotkey;	// �ȼ�
	//int		iId;		// ��ϢID

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

// ʱ��
struct stTime 
{
	stTime(){memset(this,0,sizeof(*this));}
	// ʱ��
	WORD wYear;
	WORD wMonth;
	WORD wDay;
	// ��ʼʱ��
	WORD wHourSt;
	WORD wMinuteSt;
	// ����ʱ��
	WORD wHourEd;
	WORD wMinuteEd;
	//WORD wSecond;
	// ����1-7 0-ÿ��
	BOOL bDayOfWeek[8];
};
// �¼�
struct stEvent
{
	stEvent(){memset(this,0,sizeof(*this));iState=1;}
	char	szTsMsg[1024];	// ��ʾ��Ϣ
	int		iState;			// ״̬ 1-�ȴ� 2-���� 3-��ͻ

	// ����ʱ������
	int		ptType;			// 0-ָ�����ڣ�1-ÿ�죬����ѭ������
	stTime	ptTime;
};
// �������� Begin
//struct stActions : public stEvent
//{
//	stActions(){memset(this,0,sizeof(*this));}
//	THAnimation ani;		// ��������
//};
//// ���ж����б�
//typedef	std::vector<stActions> ActionsMapPtr;

// �������� End


// ��ʱ�¼� Begin
// 1 - ������Ϣ��ʾ
// 2 - ������ʾ
//	1 - �м䴰��
//  2 - ���½�
//  3 - ���½�
// 3 - ��ǰ����
//  1 - �� ����
//  2 - ��˸ ���� 
//  3 - ��С�� ����

// �¼�
struct stEventTM
{
	stEventTM(){memset(this,0,sizeof(*this));}
	char	szTsMsg[1024];	// ��ʾ��Ϣ
	char	szTsSound[256];	// ��ʾ����
	int		mapTs[2];		// ��ʾ�¼�
	int		iEvent;			// ��Ӧ�¼�
	int		iType;			// �¼�����

	// ����ʱ������
	int		ptType;			// -1����� 0-ָ�����ڣ�1-ÿ�죬����ѭ������ 
	int     ptFanShi;		// 0-ָ��ʱ�� 1-������� 2-�������� 3-�������
	stTime	ptTime;
};

typedef	std::vector<stEventTM*> EventTMMapPtr;

// ���㱨ʱ
// 1 - ���� ��ʱ
// 2 - ָ��ʱ�� ��ʱ
// ��ʱ�ػ�
// 1 - ָ��ʱ��ػ� һ��
// 2 - ָ��ʱ��ػ� ÿ��
struct stEventTM_Type : public stEventTM
{
	stEventTM_Type(){memset(this,0,sizeof(*this));}
};

// ��ʱ����(���г�����ⲿ�������Ϣ��ʾ)
// 1 - �����ⲿ��������ҳ
// 2 - ��ָֹ������
struct stEventTM_EXEWD : public stEventTM
{
	stEventTM_EXEWD(){memset(this,0,sizeof(*this));}
	char	szEXE[256];	// exe����
	char	szWnd[256]; // ���ڱ���
	HWND	hWnd;		// ���ھ��
};

// ������ֽ	(��Ҫѡ���ֽ�����Ŀ¼)
// 1 - ָ��ʱ�� ����
struct stEventTM_Paper : public stEventTM
{
	stEventTM_Paper(){memset(this,0,sizeof(*this));}
	char	szPath[256];	// ·��
	int		iPaper;			// ��ֽ���
};

// ��ͼ	(��Ҫѡ���ͼ�����Ŀ¼)
// 1 - ȫ����ͼ 
// 2 - ָ�����ڽ�ͼ
// 3 - ָ��EXE��ͼ
struct stEventTM_JieTu : public stEventTM
{
	stEventTM_JieTu(){memset(this,0,sizeof(*this));}
	bool	bJiaQian;	 // �Ƿ��ǿ��ͼ
	char	szPath[256]; // ����Ŀ¼
	char	szEXE[256];	 // exe����
	char	szWnd[256]; // ���ڱ���
	HWND	hWnd;		// ���ھ��
};

// �߼����� ��������(�������ڱ༭�����)
// 1 - ���ѱ������� ����ָ����exe����
// 2 - ���ѱ������� ָ��ʱ������
// 3 - ���ѱ������� �������
// 4 - ��exe���� ���� ��ʾ
// 4 - ��exe���� ���� ��ֹ
struct stEventTM_Save : public stEventTM
{
	stEventTM_Save(){memset(this,0,sizeof(*this));}

};

// ��ʱ�¼� End


// ��ݲ��� Begin
// 1 - ע���ݼ� ȫ��
// 2 - ע���ݼ� ������
// 3 - ִ���¼�
//	   1 - ָ������򴰿� ��ֹ/��
//	   2 - ָ������򴰿� ����/��ʾ 
//     3 - ����ҳ�����
//     4 - ��ʾ����
//     5 - ����/��ʾ ���д���
//     6 - ���� ȫ��/����
//	   7 - ���� ��ǰ
//	   8 - ���� �ö�
//	   9 - ��ͼ	1ȫ����ͼ 2ָ�����ڽ�ͼ	(��Ҫѡ���ͼ�����Ŀ¼)
//    10 - �ػ�
//    11 - �����¼�
/*	  11 - ��ʾ������ �ݲ����*/
struct stEventKJ
{
	stEventKJ(){memset(this,0,sizeof(*this));}
	UINT	nGroupkey1;	// �ȼ� ��ϼ�1
	UINT	nGroupkey2;	// �ȼ� ��ϼ�2
	UINT	nHotkey;	// �ȼ�
	BOOL	bAll;		// ȫ��/�ֲ�
	int		iEvent;		// ִ���¼�
	char	szWnd[256]; // ���ڱ���
	HWND	hWnd;		// ���ھ��
};
typedef	std::vector<stEventKJ*>	EvtKJMapPtr;

//     1 - ָ������򴰿� ��ֹ/��
//     2 - ָ������򴰿� ����/��ʾ
//	   3 - ����ҳ�����
struct stEventKJ_OCWD : public stEventKJ
{
	//stEventKJ_OCWD(){memset(this,0,sizeof(*this));}
	char		szEXE[256];	// exe����
	HwndMapPtr  mapHwnd;	// ���ڼ�¼
};

//     10 - �ػ�
//	   11 - �����¼�
struct stEventKJ_Type : public stEventKJ
{
	stEventKJ_Type(){memset(this,0,sizeof(*this));}
	int		iType;
};

//	   9 - ��ͼ	(��Ҫѡ���ͼ�����Ŀ¼)
//      1 - ȫ����ͼ 
//      2 - ָ�����ڽ�ͼ
//		3 - ָ��EXE��ͼ
struct stEventKJ_JieTu : public stEventKJ
{
	//stEventKJ_JieTu(){memset(this,0,sizeof(*this));}
	int		iType;		 // ��ͼ����
	bool	bJiaQian;	 // �Ƿ��ǿ��ͼ
	char	szPath[256]; // ����Ŀ¼
	char	szEXE[256];	 // exe����
	HwndMapPtr  mapHwnd; // ���ڼ�¼
};

//     6 - ���� ȫ��/����
struct stEventKJ_Full : public stEventKJ
{
	stEventKJ_Full(){memset(this,0,sizeof(*this));}
	CRect		rect;		// ���ڴ�С
	LONG		lStyle;		// ������ʽ
};

//     4 - ��ʾ����
//     5 - ����/��ʾ ���д���
//	   7 - ���� ��ǰ
//	   8 - ���� �ö�
struct stEventKJ_Window : public stEventKJ
{
	//stEventKJ_Window(){memset(this,0,sizeof(*this));}
	CRect		rect;		// ���ڴ�С
	HwndMapPtr  mapHwnd;	// ���ڼ�¼
};

// ��ݲ��� End
