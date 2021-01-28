#pragma once

// mfc管理类
class thMFCManager : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCManager* create();

	// 添加控件
	static void addListCtrl(const std::string& name, CListCtrl& ctrl);

	// 获取控件
	static thRef* getListCtrl(const std::string& name);


protected:
	thMFCManager();
public:
	~thMFCManager();

protected:
	static thMFCManager* self;
	static thValue* s_value;
};