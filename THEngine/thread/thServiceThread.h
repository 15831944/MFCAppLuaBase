#ifndef __TH_SERVICETHREAD_H__
#define __TH_SERVICETHREAD_H__

#pragma once
#define TH_INLINE __forceinline 

//////////////////////////////////////////////////////////////////////////

// 临界区同步类
class thCriticalSection
{
	// 函数定义
public:
	TH_INLINE thCriticalSection(void) { ::InitializeCriticalSection(&m_csLock); }
	TH_INLINE ~thCriticalSection(void) { ::DeleteCriticalSection(&m_csLock); }

	// 功能函数
public:
	// 锁定函数
	TH_INLINE void Lock(void) { ::EnterCriticalSection(&m_csLock); }
	// 解锁函数 
	TH_INLINE void UnLock(void) { ::LeaveCriticalSection(&m_csLock); }

	// 变量定义
private:
	CRITICAL_SECTION	m_csLock;			// 临界变量
};

// 资源锁定
class thThreadLock
{

	// 函数定义
public:
	// 构造函数
	thThreadLock(thCriticalSection & CriticalSection, bool bLockAtOnce=true);
	// 析构函数
	virtual ~thThreadLock(void);

	// 操作函数
public:
	// 锁定函数
	void Lock(void);
	// 解锁函数 
	void UnLock(void);

	// 状态函数
public:
	// 锁定次数
	TH_INLINE INT GetLockCount(void) { return m_nLockCount; }

	// 变量定义
private:
	INT								m_nLockCount;					// 锁定计数
	thCriticalSection &				m_CriticalSection;				// 锁定对象
};

//////////////////////////////////////////////////////////////////////////

// 线程执行状态
enum emThreadState
{
	Thread_Note,		// 初始化
	Thread_Suspend,		// 挂起
	Thread_Runing,		// 运行
};

// 线程对象
class thServiceThread
{

	// 函数定义
protected:
	// 构造函数
	thServiceThread(void);
	// 析构函数
	virtual ~thServiceThread(void);

	// 接口函数
public:
	// 运行标志
	virtual bool IsRuning(void);
	// 启动线程
	virtual bool StartThread(void);
	// 终止线程
	virtual bool ConcludeThread(DWORD dwMillSeconds);
	// 设置运行状态
	virtual	bool SetThreadState(emThreadState state);

	// 功能函数
public:
	// 获取状态
	emThreadState GetThreadState(void) { return m_emState; }
	// 获取标识
	UINT GetThreadID(void) { return m_uThreadID; }
	// 获取句柄
	HANDLE GetThreadHandle(void) { return m_hThreadHandle; }
	// 投递消息
	LRESULT PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

	// 事件函数
protected:
	// 运行事件
	virtual bool OnEventThreadRun(void) { return true; }
	// 开始事件
	virtual bool OnEventThreadStrat(void) { return true; }
	// 终止事件
	virtual bool OnEventThreadConclude(void) { return true; }

	// 内部函数
private:
	// 线程函数
	static unsigned __stdcall ThreadFunction(LPVOID pThreadData);

	// 状态变量
private:
	volatile bool			m_bRun;							// 运行标志

	// 线程变量
private:
	emThreadState			m_emState;						// 线程状态
	UINT					m_uThreadID;					// 线程标识
	HANDLE					m_hThreadHandle;				// 线程句柄
};

//////////////////////////////////////////////////////////////////////////

#endif /*__TH_SERVICETHREAD_H__*/