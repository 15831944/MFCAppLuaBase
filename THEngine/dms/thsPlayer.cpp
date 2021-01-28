#include "stdafx.h"
#include "thsPlayer.h"
#include "../thHelper.h"
#include "thsGameManager.h"

thsPlayerBase::thsPlayerBase()
	: m_info(nullptr)
	, m_processId(0)
	, m_hwnd(0)
	, m_isReady(false)
	, m_isWindowExit(true)
	, m_delay(100)
{
	m_iCommand = cmd_note;
}

thsPlayerBase::~thsPlayerBase()
{
	stop();
	if (m_info)
	{
		m_info->release();
	}
}

bool thsPlayerBase::init(thValue* info)
{
	if (info)
	{
		info->retain();
	}
	m_info = info;
	return true;
}

void thsPlayerBase::start()
{
	if (m_strState != "")
	{
		return;
	}
	m_timDelay.Begin();
	m_timCheckWindow.Begin();
	CString gamePath = m_info->asString("gamePath").c_str();
	CString gameParame = m_info->asString("gameParame").c_str();
	_bindGMWindowBegin(gamePath, gameParame);
}

void thsPlayerBase::stop(void)
{
	// ֹͣ�����߳�
	ConcludeThread(INFINITE);
}

void thsPlayerBase::pause(void)
{

}

void thsPlayerBase::resume(void)
{

}

bool thsPlayerBase::OnEventThreadRun(void)
{
	int delay = 50;
	thHelper::ThreadDelay(delay);

	if (m_isError)
	{
		// ���ִ�����ִͣ��
		return true;
	}
	thHelper::Log_Print("OnEventThreadRun...\n");


	if (m_iCommand == cmd_start)
	{
		// ������Ϸ�����󶨴���
		auto hProcess = thHelper::ShellExecuteEx(m_gamePath, m_gameParame);
		if (hProcess == 0)
		{
			thHelper::Log_Print("[error] ShellExecuteEx: shlex");
			_postError("��Ϸ����ʧ��");
			return false;
		}
		m_hProcess = hProcess;
		auto processId = GetProcessId(HANDLE(hProcess));
		DWORD hwnd = 0;
		while (true)
		{
			hwnd = m_ths.fp_wd_fwbpid(processId, "", "��ˮ�� Version");
			if (hwnd != 0)
			{
				break;
			}
			thHelper::ThreadDelay(1000);
		}
		m_processId = processId;
		m_hwnd = hwnd;
		_bindGMWindow();
		m_isWindowExit = false;
		m_isReady = false;
		_execute(cmd_init);
		return true;
	}

	_updateBase(delay);
	return true;
}

void thsPlayerBase::_updateBase(int delay)
{
	// m_delayDel += delay;
	if (!m_timDelay.IsPassTimeMs(m_delay))
	{
		return;
	}
	m_timDelay.Reset();
	// ����������Ϸ��...
	if (m_iCommand == cmd_start)
	{
		return;
	}

	// ���󶨵Ĵ����Ƿ񱻹ر���...
	//if (m_timCheckWindow.IsPassTimeMs(1000))
	//{
	//	m_timCheckWindow.Reset();
	//	if (!thsGameManager::ms_checkWd(&m_ths, m_hwnd))
	//	{
	//		// ��Ϸ�����Ѿ����ر�
	//		_stop();
	//		// ���¿�ʼ��������
	//		m_iCommand = 0;
	//		m_isError = false;
	//		return;
	//	}
	//}
	if (!thsGameManager::ms_checkWd(&m_ths, m_hwnd))
	{
		// ��Ϸ�����Ѿ����ر�
		_stop();
		// ���¿�ʼ��������
		_execute(cmd_start);
		m_isError = false;
		thHelper::Log_Print("���µ�¼��...\n");
		return;
	}

	// �����߼�
	if (m_iCommand == cmd_init)
	{
		// ��ʼ����������
		_game_init();
		_execute(cmd_login);
	}
	else if (m_iCommand == cmd_login)
	{
		// ֪ͨ��¼
		if (_game_isLogin())
		{
			m_delay = 50;
			if (_game_login())
			{
				_execute(cmd_info);
			}
		}
		else
		{
			m_delay = 300;
		}
	}
	else if (m_iCommand == cmd_info)
	{
		// ��ȡ�˺�������Ϣ
		if (_game_info())
		{
			_execute(cmd_logic);
			m_isReady = true;
		}
	}
	
	if (!m_isReady || m_isWindowExit) return;

	// ��ʼ�����߼�
	if (m_iCommand != cmd_logic)
		return;
	_update(delay);
}

void thsPlayerBase::_bindGMWindowBegin(const CString& gamePath, const CString& gameParame)
{
	// ��¼��Ϸ����ȡ����
	if (isReady() && !isWindowExit())
	{
		// �Ѿ���¼����
		return;
	}
	_postState("��¼��Ϸ��");
	m_isReady = false;
	//if (!value)
	//{
	//	thHelper::Log_Print("[error] handleEvents: ...name=", name);
	//	return;
	//}
	//std::thread thread(&thsPlayerBase::_asyncThread_getHwnd, this, gamePath, gameParame);
	//thread.detach();
	m_gamePath = gamePath;
	m_gameParame = gameParame;
	_execute(cmd_start);
	StartThread();
}

void thsPlayerBase::_bindGMWindow()
{
	if (!thsGameManager::ms_bdWd(&m_ths, m_hwnd))
	{
		thHelper::Log_Print("[error] _bindGMWindow: no %d", m_hwnd);
		_postError("��Ϸ���ڰ�ʧ��");
	}
}

void thsPlayerBase::_execute(emCommand cmd)
{
	m_iCommand = cmd;
}

void thsPlayerBase::_stop()
{
	m_isReady = false;
	m_isWindowExit = true;
	CloseHandle(HANDLE(m_hProcess));
	thsGameManager::ms_unbdWd(&m_ths);
	_game_stop();
}

//
//void thsPlayerBase::_asyncThread_getHwnd(CString gamePath, CString gameParame)
//{
//	thsPlug ths;
//	SHELLEXECUTEINFO ShExecInfo;
//	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
//	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
//	ShExecInfo.hwnd = NULL;
//	ShExecInfo.lpVerb = NULL;
//	ShExecInfo.lpFile = gamePath;// "D:\\NetEase\\��ˮ��\\program\\bin\\Release64\\GacRunnerNG.exe";
//	ShExecInfo.lpParameters = gameParame;// "42.186.115.17:23056@0[����ɽ��] __RESERVE__={1}__CREATE_FACE__={0}__MD5__={}";
//	ShExecInfo.lpDirectory = NULL;
//	ShExecInfo.nShow = SW_SHOWMINIMIZED;
//	ShExecInfo.hInstApp = NULL;
//	BOOL ret = ShellExecuteEx(&ShExecInfo);
//	if (!ret)
//	{
//		thHelper::Log_Print("[error] _asyncThread_login: shlex");
//		m_isReady = true;
//		m_isWindowExit = true;
//		CloseHandle(ShExecInfo.hProcess);
//		return;
//	}
//	//auto s = ths.fp_wd_ep("GacRunnerNG.exe");
//
//	auto processId = GetProcessId(ShExecInfo.hProcess);
//	//auto f1 = ths.fp_wd_gwpid(DWORD(ShExecInfo.hProcess));
//
//	//auto hwnd = FindMainWindow(f);
//
//	DWORD hwnd = 0;
//	while (true)
//	{
//		hwnd = ths.fp_wd_fwbpid(processId, "", "��ˮ�� Version");
//		if (hwnd != 0)
//		{
//			break;
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//	}
//	m_processId = processId;
//	m_hwnd = hwnd;
//	_bindGMWindow();
//	m_isWindowExit = false;
//	m_isReady = false;
//	m_iCommand = 1;
//
//
//	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
//	CloseHandle(ShExecInfo.hProcess);
//
//	thsGameManager::ms_unbdWd(&m_ths);
//	m_isWindowExit = true;
//}

void thsPlayerBase::_postError(const CString& msg)
{
	// m_iCommand = -1;
	m_isError = true;
	_postState(msg);
}

void thsPlayerBase::_postState(const CString& msg)
{
	m_strState = msg;
}
