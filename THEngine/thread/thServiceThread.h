#ifndef __TH_SERVICETHREAD_H__
#define __TH_SERVICETHREAD_H__

#pragma once
#define TH_INLINE __forceinline 

//////////////////////////////////////////////////////////////////////////

// �ٽ���ͬ����
class thCriticalSection
{
	// ��������
public:
	TH_INLINE thCriticalSection(void) { ::InitializeCriticalSection(&m_csLock); }
	TH_INLINE ~thCriticalSection(void) { ::DeleteCriticalSection(&m_csLock); }

	// ���ܺ���
public:
	// ��������
	TH_INLINE void Lock(void) { ::EnterCriticalSection(&m_csLock); }
	// �������� 
	TH_INLINE void UnLock(void) { ::LeaveCriticalSection(&m_csLock); }

	// ��������
private:
	CRITICAL_SECTION	m_csLock;			// �ٽ����
};

// ��Դ����
class thThreadLock
{

	// ��������
public:
	// ���캯��
	thThreadLock(thCriticalSection & CriticalSection, bool bLockAtOnce=true);
	// ��������
	virtual ~thThreadLock(void);

	// ��������
public:
	// ��������
	void Lock(void);
	// �������� 
	void UnLock(void);

	// ״̬����
public:
	// ��������
	TH_INLINE INT GetLockCount(void) { return m_nLockCount; }

	// ��������
private:
	INT								m_nLockCount;					// ��������
	thCriticalSection &				m_CriticalSection;				// ��������
};

//////////////////////////////////////////////////////////////////////////

// �߳�ִ��״̬
enum emThreadState
{
	Thread_Note,		// ��ʼ��
	Thread_Suspend,		// ����
	Thread_Runing,		// ����
};

// �̶߳���
class thServiceThread
{

	// ��������
protected:
	// ���캯��
	thServiceThread(void);
	// ��������
	virtual ~thServiceThread(void);

	// �ӿں���
public:
	// ���б�־
	virtual bool IsRuning(void);
	// �����߳�
	virtual bool StartThread(void);
	// ��ֹ�߳�
	virtual bool ConcludeThread(DWORD dwMillSeconds);
	// ��������״̬
	virtual	bool SetThreadState(emThreadState state);

	// ���ܺ���
public:
	// ��ȡ״̬
	emThreadState GetThreadState(void) { return m_emState; }
	// ��ȡ��ʶ
	UINT GetThreadID(void) { return m_uThreadID; }
	// ��ȡ���
	HANDLE GetThreadHandle(void) { return m_hThreadHandle; }
	// Ͷ����Ϣ
	LRESULT PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

	// �¼�����
protected:
	// �����¼�
	virtual bool OnEventThreadRun(void) { return true; }
	// ��ʼ�¼�
	virtual bool OnEventThreadStrat(void) { return true; }
	// ��ֹ�¼�
	virtual bool OnEventThreadConclude(void) { return true; }

	// �ڲ�����
private:
	// �̺߳���
	static unsigned __stdcall ThreadFunction(LPVOID pThreadData);

	// ״̬����
private:
	volatile bool			m_bRun;							// ���б�־

	// �̱߳���
private:
	emThreadState			m_emState;						// �߳�״̬
	UINT					m_uThreadID;					// �̱߳�ʶ
	HANDLE					m_hThreadHandle;				// �߳̾��
};

//////////////////////////////////////////////////////////////////////////

#endif /*__TH_SERVICETHREAD_H__*/