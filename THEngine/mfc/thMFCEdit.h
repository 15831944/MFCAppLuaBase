#pragma once

// thMFCEdit
class thMFCEdit : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCEdit* create(CEdit* ctrl);


protected:
	thMFCEdit(CEdit* ctrl);
public:
	~thMFCEdit();

protected:
	CEdit* m_ctrl;
};