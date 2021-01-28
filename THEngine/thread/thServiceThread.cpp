#include "pch.h"
#include "thServiceThread.h"
#include <Process.h>

//////////////////////////////////////////////////////////////////////////
// 结构定义

// 启动参数
struct tagThreadParameter
{
	bool							bSuccess;							// 是否错误
	HANDLE							hEventFinish;						// 事件句柄
	thServiceThread	*				pServiceThread;						// 线程指针
};

//////////////////////////////////////////////////////////////////////////

// 构造函数
thThreadLock::thThreadLock(thCriticalSection & CriticalSection, bool bLockAtOnce) 
	: m_CriticalSection(CriticalSection)
	, m_nLockCount(0)
{
	// 锁定对象
	if (true==bLockAtOnce) Lock();
}

// 析构函数
thThreadLock::~thThreadLock(void)
{
	// 解除锁定
	while (m_nLockCount>0)
	{
		UnLock();
	}
}

// 锁定函数
void thThreadLock::Lock(void)
{
	// 锁定对象
	++m_nLockCount;
	m_CriticalSection.Lock();

	return;
}

// 解锁函数
void thThreadLock::UnLock(void)
{
	// 效验状态
	assert(m_nLockCount>0);
	if (0==m_nLockCount) return;

	// 解除锁定
	--m_nLockCount;
	m_CriticalSection.UnLock();

	return;
}

//////////////////////////////////////////////////////////////////////////

// 构造函数
thServiceThread::thServiceThread(void)
: m_bRun(false)
, m_uThreadID(0)
, m_emState(Thread_Note)
, m_hThreadHandle(NULL)
{
}

// 析构函数
thServiceThread::~thServiceThread(void)
{
	// 停止线程
	ConcludeThread(INFINITE);

	return;
}

// 状态判断
bool thServiceThread::IsRuning(void)
{
	// 运行检测
	if (NULL==m_hThreadHandle) return false;
	if (WaitForSingleObject(m_hThreadHandle,0)!=WAIT_TIMEOUT) return false;

	return true;
}

// 启动线程
bool thServiceThread::StartThread(void)
{
	// 效验状态
	assert(false==IsRuning());
	if (true==IsRuning()) return false;

	// 清理变量
	if (m_hThreadHandle!=NULL) 
	{
		// 关闭句柄
		CloseHandle(m_hThreadHandle);

		// 设置变量
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	// 变量定义
	tagThreadParameter ThreadParameter;
	ZeroMemory(&ThreadParameter,sizeof(ThreadParameter));

	// 设置变量
	ThreadParameter.bSuccess		= false;
	ThreadParameter.pServiceThread	= this;
	ThreadParameter.hEventFinish	= CreateEvent(NULL,FALSE,FALSE,NULL);

	// 效验状态
	assert(ThreadParameter.hEventFinish!=NULL);
	if (NULL==ThreadParameter.hEventFinish) return false;

	// 启动线程
	m_bRun = true;
	m_emState = Thread_Runing;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	// 错误判断
	if (INVALID_HANDLE_VALUE==m_hThreadHandle)
	{
		CloseHandle(ThreadParameter.hEventFinish);
		return false;
	}

	// 等待事件
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);
	CloseHandle(ThreadParameter.hEventFinish);

	// 判断错误
	if (false==ThreadParameter.bSuccess)
	{
		ConcludeThread(INFINITE);
		return false;
	}

	return true;
}

// 停止线程
bool thServiceThread::ConcludeThread(DWORD dwMillSeconds)
{
	// 停止线程
	if (true==IsRuning())
	{
		// 设置变量
		m_bRun = false;
		this->SetThreadState(Thread_Runing);
		// 停止等待
		if (WaitForSingleObject(m_hThreadHandle,dwMillSeconds)==WAIT_TIMEOUT)
		{
			return false;
		}
	}

	// 设置变量
	if (m_hThreadHandle!=NULL)
	{
		// 关闭句柄
		CloseHandle(m_hThreadHandle);

		// 设置变量
		m_uThreadID = 0;
		m_hThreadHandle = NULL;
	}

	return true;
}

// 设置运行状态
bool thServiceThread::SetThreadState( emThreadState state )
{
	if (Thread_Note==m_emState) return false;

	switch (state)
	{
	case Thread_Suspend:	// 挂起
		{
			if (Thread_Suspend==m_emState) return true;
			m_emState = Thread_Suspend;
			SuspendThread(m_hThreadHandle);
		}
		break;
	case Thread_Runing:		// 唤醒
		{
			if (Thread_Runing==m_emState) return true;
			m_emState = Thread_Runing;
			ResumeThread(m_hThreadHandle);
		}
		break;
	default: assert(false);
		break;
	}
	return true;
}

// 投递消息
LRESULT thServiceThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// 状态效验
	assert((m_uThreadID!=0)&&(m_hThreadHandle!=NULL));
	if ((0==m_uThreadID)||(NULL==m_hThreadHandle)) return false;

	// 投递消息
	if (FALSE==::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam))
	{
		DWORD dwLastError = GetLastError();
		return dwLastError;
	}

	return 0L;
}

// 线程函数
unsigned thServiceThread::ThreadFunction(LPVOID pThreadData)
{
	// 效验参数
	assert(pThreadData!=NULL);

	// 变量定义
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	thServiceThread * pServiceThread=pThreadParameter->pServiceThread;

	// 设置因子
	srand((DWORD)time(NULL));

	// 启动通知
	try
	{
		pThreadParameter->bSuccess=pServiceThread->OnEventThreadStrat(); 
	} 
	catch (...)
	{
		// 设置变量
		assert(FALSE);
		pThreadParameter->bSuccess=false;
	}

	// 设置事件
	bool bSuccess=pThreadParameter->bSuccess;
	assert(pThreadParameter->hEventFinish!=NULL);
	if (pThreadParameter->hEventFinish!=NULL) SetEvent(pThreadParameter->hEventFinish);

	// 线程处理
	if (true==bSuccess)
	{
		// 线程运行
		while (pServiceThread->m_bRun)
		{
#ifndef _DEBUG
			//运行版本
			try
			{
				if (false==pServiceThread->OnEventThreadRun())
				{
					break;
				}
			}
			catch (...)	{ }
#else
			//调试版本
			if (false==pServiceThread->OnEventThreadRun())
			{
				break;
			}
#endif
		}

		//停止通知
		try
		{ 
			pServiceThread->OnEventThreadConclude();
		} 
		catch (...)	{ assert(false); }
	}

	////中止线程
	//_endthreadex(0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////
