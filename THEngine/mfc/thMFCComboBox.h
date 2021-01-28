#pragma once

// CComboBox
class thMFCComboBox : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCComboBox* create(CComboBox* ctrl);

	int GetCount() const;
	int GetCurSel() const;
	void SetCurSel(int nSelect);

	void AddString(const std::string& lpszString);
	void InsertString(int nIndex, const std::string& lpszString);


protected:
	thMFCComboBox(CComboBox* ctrl);
public:
	~thMFCComboBox();

protected:
	CComboBox* m_ctrl;
};