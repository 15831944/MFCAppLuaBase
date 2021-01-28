#include "pch.h"
#include "thServiceThread.h"
#include <Process.h>

//////////////////////////////////////////////////////////////////////////
// �ṹ����

// ��������
struct tagThreadParameter
{
	bool							bSuccess;							// �Ƿ����
	HANDLE							hEventFinish;						// �¼����
	thServiceThread	*				pServiceThread;						// �߳�ָ��
};

//////////////////////////////////////////////////////////////////////////

// ���캯��
thThreadLock::thThreadLock(thCriticalSection & CriticalSection, bool bLockAtOnce) 
	: m_CriticalSection(CriticalSection)
	, m_nLockCount(0)
{
	// ��������
	if (true==bLockAtOnce) Lock();
}

// ��������
thThreadLock::~thThreadLock(void)
{
	// �������
	while (m_nLockCount>0)
	{
		UnLock();
	}
}

// ��������
void thThreadLock::Lock(void)
{
	// ��������
	++m_nLockCount;
	m_CriticalSection.Lock();

	return;
}

// ��������
void thThreadLock::UnLock(void)
{
	// Ч��״̬
	assert(m_nLockCount>0);
	if (0==m_nLockCount) return;

	// �������
	--m_nLockCount;
	m_CriticalSection.UnLock();

	return;
}

//////////////////////////////////////////////////////////////////////////

// ���캯��
thServiceThread::thServiceThread(void)
: m_bRun(false)
, m_uThreadID(0)
, m_emState(Thread_Note)
, m_hThreadHandle(NULL)
{
}

// ��������
thServiceThread::~thServiceThread(void)
{
	// ֹͣ�߳�
	ConcludeThread(INFINITE);

	return;
}

// ״̬�ж�
bool thServiceThread::IsRuning(void)
{
	// ���м��
	if (NULL==m_hThreadHandle) return false;
	if (WaitForSingleObject(m_hThreadHandle,0)!=WAIT_TIMEOUT) return false;

	return true;
}

// �����߳�
bool thServiceThread::StartThread(void)
{
	// Ч��״̬
	assert(false==IsRuning());
	if (true==IsRuning()) return false;

	// �������
	if (m_hThreadHandle!=NULL) 
	{
		// �رվ��
		CloseHandle(m_hThreadHandle);

		// ���ñ���
		m_uThreadID=0;
		m_hThreadHandle=NULL;
	}

	// ��������
	tagThreadParameter ThreadParameter;
	ZeroMemory(&ThreadParameter,sizeof(ThreadParameter));

	// ���ñ���
	ThreadParameter.bSuccess		= false;
	ThreadParameter.pServiceThread	= this;
	ThreadParameter.hEventFinish	= CreateEvent(NULL,FALSE,FALSE,NULL);

	// Ч��״̬
	assert(ThreadParameter.hEventFinish!=NULL);
	if (NULL==ThreadParameter.hEventFinish) return false;

	// �����߳�
	m_bRun = true;
	m_emState = Thread_Runing;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	// �����ж�
	if (INVALID_HANDLE_VALUE==m_hThreadHandle)
	{
		CloseHandle(ThreadParameter.hEventFinish);
		return false;
	}

	// �ȴ��¼�
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);
	CloseHandle(ThreadParameter.hEventFinish);

	// �жϴ���
	if (false==ThreadParameter.bSuccess)
	{
		ConcludeThread(INFINITE);
		return false;
	}

	return true;
}

// ֹͣ�߳�
bool thServiceThread::ConcludeThread(DWORD dwMillSeconds)
{
	// ֹͣ�߳�
	if (true==IsRuning())
	{
		// ���ñ���
		m_bRun = false;
		this->SetThreadState(Thread_Runing);
		// ֹͣ�ȴ�
		if (WaitForSingleObject(m_hThreadHandle,dwMillSeconds)==WAIT_TIMEOUT)
		{
			return false;
		}
	}

	// ���ñ���
	if (m_hThreadHandle!=NULL)
	{
		// �رվ��
		CloseHandle(m_hThreadHandle);

		// ���ñ���
		m_uThreadID = 0;
		m_hThreadHandle = NULL;
	}

	return true;
}

// ��������״̬
bool thServiceThread::SetThreadState( emThreadState state )
{
	if (Thread_Note==m_emState) return false;

	switch (state)
	{
	case Thread_Suspend:	// ����
		{
			if (Thread_Suspend==m_emState) return true;
			m_emState = Thread_Suspend;
			SuspendThread(m_hThreadHandle);
		}
		break;
	case Thread_Runing:		// ����
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

// Ͷ����Ϣ
LRESULT thServiceThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// ״̬Ч��
	assert((m_uThreadID!=0)&&(m_hThreadHandle!=NULL));
	if ((0==m_uThreadID)||(NULL==m_hThreadHandle)) return false;

	// Ͷ����Ϣ
	if (FALSE==::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam))
	{
		DWORD dwLastError = GetLastError();
		return dwLastError;
	}

	return 0L;
}

// �̺߳���
unsigned thServiceThread::ThreadFunction(LPVOID pThreadData)
{
	// Ч�����
	assert(pThreadData!=NULL);

	// ��������
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	thServiceThread * pServiceThread=pThreadParameter->pServiceThread;

	// ��������
	srand((DWORD)time(NULL));

	// ����֪ͨ
	try
	{
		pThreadParameter->bSuccess=pServiceThread->OnEventThreadStrat(); 
	} 
	catch (...)
	{
		// ���ñ���
		assert(FALSE);
		pThreadParameter->bSuccess=false;
	}

	// �����¼�
	bool bSuccess=pThreadParameter->bSuccess;
	assert(pThreadParameter->hEventFinish!=NULL);
	if (pThreadParameter->hEventFinish!=NULL) SetEvent(pThreadParameter->hEventFinish);

	// �̴߳���
	if (true==bSuccess)
	{
		// �߳�����
		while (pServiceThread->m_bRun)
		{
#ifndef _DEBUG
			//���а汾
			try
			{
				if (false==pServiceThread->OnEventThreadRun())
				{
					break;
				}
			}
			catch (...)	{ }
#else
			//���԰汾
			if (false==pServiceThread->OnEventThreadRun())
			{
				break;
			}
#endif
		}

		//ֹ֪ͣͨ
		try
		{ 
			pServiceThread->OnEventThreadConclude();
		} 
		catch (...)	{ assert(false); }
	}

	////��ֹ�߳�
	//_endthreadex(0L);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////
