#pragma once

// thMFCStatic
class thMFCStatic : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCStatic* create(CStatic* ctrl);

	// text
	void SetWindowText(const std::string& str);
	std::string GetWindowText();


protected:
	thMFCStatic(CStatic* ctrl);
public:
	~thMFCStatic();

protected:
	CStatic* m_ctrl;
};