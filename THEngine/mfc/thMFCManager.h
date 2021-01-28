#pragma once

// mfc������
class thMFCManager : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCManager* create();

	// ��ӿؼ�
	static void addListCtrl(const std::string& name, CListCtrl& ctrl);

	// ��ȡ�ؼ�
	static thRef* getListCtrl(const std::string& name);


protected:
	thMFCManager();
public:
	~thMFCManager();

protected:
	static thMFCManager* self;
	static thValue* s_value;
};