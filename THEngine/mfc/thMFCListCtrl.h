#pragma once

// CListCtrl
class thMFCListCtrl : public thRef
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// create
	static thMFCListCtrl* create(CListCtrl* ctrl);


	// ��ȡ�ؼ�����չ��ʽ
	int GetExtendedStyle();
	// �����ض��ڿؼ�����չ��ʽλ��
	int SetExtendedStyle(int dwNewStyle);

	// �����������
	int InsertColumn(int nCol, const std::string& lpszColumnHeading, int nWidth);

	// ��ȡitem����
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