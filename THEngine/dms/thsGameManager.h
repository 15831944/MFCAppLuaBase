#pragma once

#include "thsPlayer.h"
#include <map>

// 玩家 账号 游戏管理器
class thsGameManager
{
public:
	thsGameManager();
	~thsGameManager();

	// 创建
	static thsGameManager* createInstance(void);
	static void destroyInstance(void);
	inline static thsGameManager* get(void) { return self; }

public:
	// 游戏启动路径
	void setGameRunPath(const CString& path) { m_gamePath = path; }
	const CString& getGameRunPath() { return m_gamePath; }

	// 最小正常分辨率 - 游戏能缩放到的最小窗口大小
	void setGameWdMinSize(int width, int height);

	// 设置游戏分辨率
	void setGameWdSize(int width, int height);
	int getGameWdWidth(void) { return m_width; }
	int getGameWdHeight(void) { return m_height; }

	// 获取分辨率缩放比例 - 相对正常最小分辨率
	float getGameWdWidthEps(void) { return m_widthEps; }
	float getGameWdHeightEps(void) { return m_heightEps; }


	// 根据服务器名 获取启动参数
	CString getGameRunParames(const CString& serverName);


public:
	// 更新数据
	void update(int delay);

	// 注册玩家
	void registerPlayer(const CString& key, thsPlayerBase* player);
	void unregisterPlayer(const CString& key);

	// 获取玩家
	thsPlayerBase* getPlayer(const CString& key);

	// 触发事件 - 后续处理...
	bool postEvent(const CString& name, thValue* value = nullptr, const std::function<void(const CString& name)>&callfunc = nullptr);
	// 触发事件 - 立马返回
	bool sendEvent(const CString& name, thValue* value = nullptr);

protected:
	// 更新事件队列
	void _updateEvent(int delay);


	// 功能事件
public:
	// 获取随机毫秒
	static int ms_randomMs(int max);
	// 延迟随机毫秒时间
	static void ms_delay(int delay);
	static void ms_delayRandom(int max);

	// 绑定窗口
	static bool ms_bdWd(thsPlug* ths, DWORD hwnd);
	// 解绑
	static void ms_unbdWd(thsPlug* ths);
	// 检查窗口是否存在
	static bool ms_checkWd(thsPlug* ths, DWORD hwnd);


	// 鼠标移动到目标位置，并点击左键
	static void ms_mvPosClickLeft(thsPlug* ths, int x, int y);

	// 全选字符 shift+end
	static void ms_selectAllTxt(thsPlug* ths);
	// 退格，删除输入字符
	static void ms_bkBackspace(thsPlug* ths, int count);
	// 输入文本
	static void ms_inputStr(thsPlug* ths, DWORD hwnd, const CString& str);

	// 查找字符串
	static bool ms_fdstrfst(thsPlug* ths, long x1, long y1, long x2, long y2, const char* str, const char* color, long * x, long * y, double sim=0.85);


protected:
	static thsGameManager* self;
	CString m_gamePath;		// 游戏启动路径
	int m_widthOr;		// 最小分辨率
	int m_heightOr;
	int m_width;		// 当前设置的分辨率
	int m_height;
	float m_widthEps;	// 分辨率缩放比例
	float m_heightEps;	

	// 玩家队列
	std::map<CString,thsPlayerBase*> m_mapPlayer;

	// 事件队列
	struct sEventS
	{
		sEventS(const CString& _name, thValue* _value, const std::function<void(const CString&)>& func) :name(_name), value(_value) { callfunc = func; }
		CString name;
		thValue* value;
		std::function<void(const CString&)> callfunc;
	};
	std::vector<sEventS> m_vEvent;
};

