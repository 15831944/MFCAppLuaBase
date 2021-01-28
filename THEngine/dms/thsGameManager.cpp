#include "stdafx.h"
#include "thsGameManager.h"
#include "../data/thBlockAllocator.h"
#include "../thscript/thscript.h"

thsGameManager* thsGameManager::self = nullptr;

thsGameManager::thsGameManager()
{
	m_widthOr = 1280;
	m_heightOr = 720;
	m_widthEps = 1.0f;
	m_heightEps = 1.0f;
}


thsGameManager::~thsGameManager()
{
	for (auto &v : m_vEvent)
	{
		if (v.value)
		{
			v.value->release();
		}
	}
}

thsGameManager* thsGameManager::createInstance(void)
{
	thBlockAllocator::createInstance();
	if (self)
	{
		return self;
	}
	self = new thsGameManager();
	return self;
}

void thsGameManager::destroyInstance(void)
{
	thBlockAllocator::destroyInstance();
	if (self)
	{
		delete self;
		self = nullptr;
	}
}

void thsGameManager::setGameWdMinSize(int width, int height)
{
	m_widthOr = width;
	m_heightOr = height;
}

void thsGameManager::setGameWdSize(int width, int height)
{
	m_width = width;
	m_height = height;
	// 设置比例
	m_widthEps = m_width / float(m_widthOr);
	m_heightEps = m_height / float(m_heightOr);
}

CString thsGameManager::getGameRunParames(const CString& serverName)
{
	if (serverName == "剑履山河")
	{
		return "42.186.115.17:23056@0[剑履山河] __RESERVE__={1}__CREATE_FACE__={0}__MD5__={}";
	}
	return "";
}

void thsGameManager::update(int delay)
{
	_updateEvent(delay);
	for (auto &v : m_mapPlayer)
	{
		auto player = v.second;
		if (player)
		{
			// player->update(delay);
		}
	}
}

void thsGameManager::registerPlayer(const CString& key, thsPlayerBase* player)
{
	m_mapPlayer.emplace(key, player);
}

void thsGameManager::unregisterPlayer(const CString& key)
{
	auto iter = m_mapPlayer.find(key);
	if (iter != m_mapPlayer.end())
	{
		if (iter->second)
		{
			delete iter->second;
		}
		m_mapPlayer.erase(iter);
	}
}

thsPlayerBase* thsGameManager::getPlayer(const CString& key)
{
	auto iter = m_mapPlayer.find(key);
	if (iter != m_mapPlayer.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool thsGameManager::postEvent(const CString& name, thValue* value /*= nullptr*/, const std::function<void(const CString&)>&callfunc /*= nullptr*/)
{
	if (value) value->retain();
	for (auto &v : m_vEvent)
	{
		if (v.name == name)
		{
			if (v.value)
			{
				v.value->release();
			}
			v.value = value;
			return true;
		}
	}
	m_vEvent.emplace_back(name, value, callfunc);
	return true;
}

bool thsGameManager::sendEvent(const CString& name, thValue* value /*= nullptr*/)
{
	if (value) value->retain();
	for (auto &v: m_mapPlayer)
	{
		auto player = v.second;
		if (player)
		{
			player->_handleEvents(name, value);
		}
	}
	if (value) value->release();
	return true;
}

void thsGameManager::_updateEvent(int delay)
{
	if (!m_vEvent.empty())
	{
		auto &evt = m_vEvent[0];
		for (auto &v : m_mapPlayer)
		{
			auto player = v.second;
			if (player)
			{
				player->_handleEvents(evt.name, evt.value);
			}
		}
		if (evt.value) evt.value->release();
		if (evt.callfunc)
		{
			evt.callfunc(evt.name);
		}
		m_vEvent.erase(m_vEvent.begin());
	}
}

int thsGameManager::ms_randomMs(int max)
{
	return thHelper::random_int(20, max);
}

void thsGameManager::ms_delay(int delay)
{
	//ths->fp_xt_delay(delay);
	thHelper::ThreadDelay(delay);
}

void thsGameManager::ms_delayRandom(int max)
{
	//ths->fp_xt_delay(ms_randomMs(max));
	// Sleep(ms_randomMs(max));
	thHelper::ThreadDelayRandom(max);
}

bool thsGameManager::ms_bdWd(thsPlug* ths, DWORD hwnd)
{
	auto r = ths->fp_wd_bwex(hwnd, "dx2", "dx.mouse.position.lock.api|dx.mouse.api|dx.mouse.cursor", "dx.keypad.api", "dx.public.active.api|dx.public.hide.dll|dx.public.graphic.protect|dx.public.down.cpu", 101);
	return r == 1;
}

void thsGameManager::ms_unbdWd(thsPlug* ths)
{
	ths->fp_ht_ubw();
}

bool thsGameManager::ms_checkWd(thsPlug* ths, DWORD hwnd)
{
	auto s = ths->fp_wd_isbd(hwnd);
	return s == 1;
}

void thsGameManager::ms_mvPosClickLeft(thsPlug* ths, int x, int y)
{
	// 缩放比例
	auto wEps = thsGameManager::get()->getGameWdWidthEps();
	auto hEps = thsGameManager::get()->getGameWdHeightEps();

	// 移动数据点击接受服务条款
	x = int(x* wEps);
	y = int(y* hEps);
	ths->fp_aj_mt(x,y);
	// 延迟50毫秒
	ms_delayRandom(30);
	// 点击
	ths->fp_aj_lck();
	ms_delayRandom(120);
}

void thsGameManager::ms_selectAllTxt(thsPlug* ths)
{
	ths->fp_wd_kp(36);
	ms_delayRandom(200);
	ths->fp_aj_kd(16);
	ths->fp_wd_kp(35);
	ms_delayRandom(200);
	ths->fp_aj_ku(16);
	ms_delayRandom(30);
}

void thsGameManager::ms_bkBackspace(thsPlug* ths, int count)
{
	for (int i=0; i<count; ++i)
	{
		ths->fp_wd_kp(8);
		ms_delayRandom(50);
	}
}

void thsGameManager::ms_inputStr(thsPlug* ths, DWORD hwnd, const CString& str)
{
	ms_delayRandom(50);
	ths->fp_dt_ss(hwnd, str);
	ms_delay(500);
}

bool thsGameManager::ms_fdstrfst(thsPlug* ths, long x1, long y1, long x2, long y2, const char* str, const char* color, long * x, long * y, double sim/*=0.85*/)
{
	auto ret = ths->fp_nc_fsft(x1, y1, x2, y2, str, color, sim, x, y);
	if (ret == 0)
	{
		return true;
	}
	return false;
}
