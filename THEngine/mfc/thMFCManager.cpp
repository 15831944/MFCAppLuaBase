#include "pch.h"
#include "thMFCManager.h"
#include "thMFCHeader.h"

thMFCManager* thMFCManager::self;
thValue* thMFCManager::s_value;
thMFCManager::thMFCManager()
{
	s_value = thValue::create();
}

thMFCManager::~thMFCManager()
{
}

bool thMFCManager::registerToLua(lua_State* luas)
{
	thClassDef<thMFCManager>(luas, "thMFCManager")
		// as func
		.funcr<thMFCListCtrl*, const std::string&>("getListCtrl", thDF_FuncConv(thMFCManager::getListCtrl))
		.end();

	return true;
}

thMFCManager* thMFCManager::create()
{
	self = new thMFCManager();
	return self;
}

void thMFCManager::addListCtrl(const std::string& name, CListCtrl& ctrl)
{
	s_value->push_object(name, thMFCListCtrl::create(&ctrl));
}

thRef* thMFCManager::getListCtrl(const std::string& name)
{
	return s_value->asObject(name);
}
