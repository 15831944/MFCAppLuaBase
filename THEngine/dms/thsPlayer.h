#ifndef __INTHSDPLAYER_H__
#define __INTHSDPLAYER_H__

#include "../data/thValue.h"
#include "thsPlug.h"
#include <functional>
#include <vector>

// ��һ�������
//struct stPlayerInfo 
//{
//	CString account;		// �˺�
//	CString password;		// ����
//	CString server;			// ������
//};

#include "../thread/thServiceThread.h"

enum emCommand
{
	cmd_note = -1,
	cmd_start = 0,	// ������Ϸ�����󶨴���
	cmd_init = 1,	// ��ʼ����������
	cmd_login = 2,	// ��ʼģ���˺ŵ�¼
	cmd_info = 3,	// ��ȡ��Ϸ�˺���Ϣ
	cmd_logic = 4,	// �����߼�
};


// �˺���һ��� ͨ��
// ����������Ϸ���� �̳���� 

class thsPlayerBase : public thServiceThread,public thRef
{
public:
	virtual ~thsPlayerBase();

public:

	// ��ʼִ��
	virtual void start(void);
	// ִֹͣ��
	virtual void stop(void);
	// ��ִͣ��
	virtual void pause(void);
	// ����ִ��
	virtual void resume(void);

public:
	


public:
	// �Ƿ�׼�����
	// �������ݺ��߼���������׼����ɺ�
	virtual bool isReady(void) { return m_isReady; }

	// �󶨴����Ѿ��˳���������ͣ���д���
	virtual bool isWindowExit(void) { return m_isWindowExit; }
	// ����
	virtual bool isError(void) { return m_isError; }

	// ��ȡ����id
	virtual DWORD getProcessId() { return m_processId; }
	// ��ȡ��Ϸ���ھ��
	virtual DWORD getHwnd() { return m_hwnd; }
	// ��ȡ״̬
	virtual CString& getState() { return m_strState; }

public:
	// �¼�����
	virtual void _handleEvents(const CString& name, thValue* value = nullptr) { return; }

protected:
	// ��������
	void _updateBase(int delay);
	// ����Ϸ����
	void _bindGMWindowBegin(const CString& gamePath, const CString& gameParame);
	void _bindGMWindow();

	// ִ��ָ��
	void  _execute(emCommand cmd);
	// ֹͣ����
	void _stop();


	// �²㴦�� -- �߼�
protected:

	// ��������
	virtual void _update(int delay) {}

	// ���ô�ĮһЩ����
	virtual void _game_init(void) {}
	// ��Ϸֹͣ
	virtual void _game_stop(void) {}

	// �����Ϸ�Ƿ���Ե�¼��
	virtual bool _game_isLogin(void) { return false; }

	// ��ʼ��¼��Ϸ
	virtual bool _game_login(void) { return true; }
	// ��¼�ɹ��� ��ȡ�˺�������Ϣ
	virtual bool _game_info(void) { return true; }


protected:
	// Ͷ�ݴ�����Ϣ
	void _postError(const CString& msg);
	// Ͷ��״̬��Ϣ
	void _postState(const CString& msg);


	// �����̺߳���
private:
	// �����¼�
	bool OnEventThreadRun(void);
	// ��ʼ�¼�
	bool OnEventThreadStrat(void) { return true; }
	// ��ֹ�¼�
	bool OnEventThreadConclude(void) { return true; }

protected:
	thsPlayerBase();
	// ��ʼ������
	virtual bool init(thValue* info);

protected:
	thsPlug m_ths;		// ��Įʵ��
	thValue* m_info;	// ������Ϣ
	bool	m_isHeader;		// �Ƿ����ӳ�
	int		m_delayDel;	// �߼������ӳ�
	int		m_delay;	// �߼������ӳ�

	DWORD	m_hProcess;		// ���̾��
	DWORD	m_processId;	// ����id
	DWORD	m_hwnd;			// ��Ϸ����

	bool	m_isReady;		// �����Ƿ�׼��ok
	bool	m_isWindowExit; // ��Ӧ��Ϸ�����Ƿ��Ѿ��˳�
	bool	m_isError;		// ���ִ���
	CString m_strState;		// ��ǰ״̬

	CString m_retEvents;	// �¼�����ɹ���Ϣ

	CString m_gamePath;
	CString m_gameParame;

	// time
	thTimer m_timDelay;		// �߼������ӳ�
	thTimer m_timCheckWindow;	// ��ⴰ���Ƿ��˳�

private:
	int	m_iCommand;			// ������
	
};

#endif