#ifndef __INTHSDPLAYER_H__
#define __INTHSDPLAYER_H__

#include "../data/thValue.h"
#include "thsPlug.h"
#include <functional>
#include <vector>

// 玩家基本数据
//struct stPlayerInfo 
//{
//	CString account;		// 账号
//	CString password;		// 密码
//	CString server;			// 服务器
//};

#include "../thread/thServiceThread.h"

enum emCommand
{
	cmd_note = -1,
	cmd_start = 0,	// 启动游戏，并绑定窗口
	cmd_init = 1,	// 初始化引擎数据
	cmd_login = 2,	// 开始模拟账号登录
	cmd_info = 3,	// 获取游戏账号信息
	cmd_logic = 4,	// 处理逻辑
};


// 账号玩家基类 通用
// 后面其他游戏开发 继承这个 

class thsPlayerBase : public thServiceThread,public thRef
{
public:
	virtual ~thsPlayerBase();

public:

	// 开始执行
	virtual void start(void);
	// 停止执行
	virtual void stop(void);
	// 暂停执行
	virtual void pause(void);
	// 继续执行
	virtual void resume(void);

public:
	


public:
	// 是否准备完成
	// 所有数据和逻辑处理，请在准备完成后
	virtual bool isReady(void) { return m_isReady; }

	// 绑定窗口已经退出。。。暂停所有处理
	virtual bool isWindowExit(void) { return m_isWindowExit; }
	// 错误
	virtual bool isError(void) { return m_isError; }

	// 获取进程id
	virtual DWORD getProcessId() { return m_processId; }
	// 获取游戏窗口句柄
	virtual DWORD getHwnd() { return m_hwnd; }
	// 获取状态
	virtual CString& getState() { return m_strState; }

public:
	// 事件处理
	virtual void _handleEvents(const CString& name, thValue* value = nullptr) { return; }

protected:
	// 更新数据
	void _updateBase(int delay);
	// 绑定游戏窗口
	void _bindGMWindowBegin(const CString& gamePath, const CString& gameParame);
	void _bindGMWindow();

	// 执行指令
	void  _execute(emCommand cmd);
	// 停止运行
	void _stop();


	// 下层处理 -- 逻辑
protected:

	// 更新数据
	virtual void _update(int delay) {}

	// 设置大漠一些属性
	virtual void _game_init(void) {}
	// 游戏停止
	virtual void _game_stop(void) {}

	// 检测游戏是否可以登录了
	virtual bool _game_isLogin(void) { return false; }

	// 开始登录游戏
	virtual bool _game_login(void) { return true; }
	// 登录成功后 获取账号数据信息
	virtual bool _game_info(void) { return true; }


protected:
	// 投递错误信息
	void _postError(const CString& msg);
	// 投递状态信息
	void _postState(const CString& msg);


	// 重载线程函数
private:
	// 运行事件
	bool OnEventThreadRun(void);
	// 开始事件
	bool OnEventThreadStrat(void) { return true; }
	// 终止事件
	bool OnEventThreadConclude(void) { return true; }

protected:
	thsPlayerBase();
	// 初始化数据
	virtual bool init(thValue* info);

protected:
	thsPlug m_ths;		// 大漠实例
	thValue* m_info;	// 数据信息
	bool	m_isHeader;		// 是否做队长
	int		m_delayDel;	// 逻辑处理延迟
	int		m_delay;	// 逻辑处理延迟

	DWORD	m_hProcess;		// 进程句柄
	DWORD	m_processId;	// 进程id
	DWORD	m_hwnd;			// 游戏窗口

	bool	m_isReady;		// 数据是否准备ok
	bool	m_isWindowExit; // 对应游戏窗口是否已经退出
	bool	m_isError;		// 出现错误
	CString m_strState;		// 当前状态

	CString m_retEvents;	// 事件处理成功消息

	CString m_gamePath;
	CString m_gameParame;

	// time
	thTimer m_timDelay;		// 逻辑处理延迟
	thTimer m_timCheckWindow;	// 检测窗口是否退出

private:
	int	m_iCommand;			// 处理步骤
	
};

#endif