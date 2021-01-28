#pragma once

// CListCtrl
class thMFCListCtrl : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCListCtrl* create(CListCtrl* ctrl);


	// 获取控件的扩展样式
	int GetExtendedStyle();
	// 设置特定于控件的扩展样式位。
	int SetExtendedStyle(int dwNewStyle);

	// 插入标题数据
	int InsertColumn(int nCol, const std::string& lpszColumnHeading, int nWidth);

	// 获取item数量
	int GetItemCount();

	// Adds an item to the control.
	int InsertItem(int nItem, const std::string& lpszItem);
	// Sets the text associated with a particular item.
	bool SetItemText(int nItem, int nSubItem, const std::string& lpszText);
	std::string GetItemText(int nItem, int nSubItem) const;

	// remove all items
	void DeleteAllItems();


protected:
	thMFCListCtrl(CListCtrl* ctrl);
public:
	~thMFCListCtrl();

protected:
	CListCtrl* m_ctrl;
};