#include "pch.h"
#include "thMFCListCtrl.h"
#include "../lua/bind/thscript_bind.h"

thMFCListCtrl::thMFCListCtrl(CListCtrl* ctrl)
	: m_ctrl(ctrl)
{

}

thMFCListCtrl::~thMFCListCtrl()
{

}

bool thMFCListCtrl::registerToLua(lua_State* luas)
{
	thClassDef<thMFCListCtrl>(luas, "thMFCListCtrl")
		// as func
		.funcr<int>("GetExtendedStyle", thDF_CFuncConv(thMFCListCtrl, GetExtendedStyle))
		.funcr<int, int>("SetExtendedStyle", thDF_CFuncConv(thMFCListCtrl, SetExtendedStyle))
		.funcr<int, int, const std::string&, int>("InsertColumn", thDF_CFuncConv(thMFCListCtrl, InsertColumn))
		.funcr<int>("GetItemCount", thDF_CFuncConv(thMFCListCtrl, GetItemCount))
		.funcr<int, const std::string&>("InsertItem", thDF_CFuncConv(thMFCListCtrl, InsertItem))
		.funcr<int, int, const std::string&>("SetItemText", thDF_CFuncConv(thMFCListCtrl, SetItemText))
		.funcr<const std::string&, int, int>("GetItemText", thDF_CFuncConv(thMFCListCtrl, GetItemText))
		.func("DeleteAllItems", thDF_CFuncConv(thMFCListCtrl, DeleteAllItems))
		.end();

	return true;
}

thMFCListCtrl* thMFCListCtrl::create(CListCtrl* ctrl)
{
	auto ret = new thMFCListCtrl(ctrl);
	ret->autorelease();
	return ret;
}

int thMFCListCtrl::GetExtendedStyle()
{
	return m_ctrl->GetExtendedStyle();
}

int thMFCListCtrl::SetExtendedStyle(int dwNewStyle)
{
	return m_ctrl->SetExtendedStyle(dwNewStyle);
}

int thMFCListCtrl::InsertColumn(int nCol, const std::string& lpszColumnHeading, int nWidth)
{
	return m_ctrl->InsertColumn(nCol, thHelper::toGB2312(lpszColumnHeading.c_str()), LVCFMT_CENTER, nWidth);
}

int thMFCListCtrl::GetItemCount()
{
	return m_ctrl->GetItemCount();
}

int thMFCListCtrl::InsertItem(int nItem, const std::string& lpszItem)
{
	return m_ctrl->InsertItem(nItem, lpszItem.c_str());
}

bool thMFCListCtrl::SetItemText(int nItem, int nSubItem, const std::string& lpszText)
{
	return m_ctrl->SetItemText(nItem, nSubItem, thHelper::toGB2312(lpszText.c_str()));
}

std::string thMFCListCtrl::GetItemText(int nItem, int nSubItem) const
{
	auto str = m_ctrl->GetItemText(nItem, nSubItem);
	return str.GetString();
}

void thMFCListCtrl::DeleteAllItems()
{
	m_ctrl->DeleteAllItems();
}
