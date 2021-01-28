#include "pch.h"
#include "thMFCComboBox.h"
#include "../lua/bind/thscript_bind.h"

thMFCComboBox::thMFCComboBox(CComboBox* ctrl)
	: m_ctrl(ctrl)
{

}

thMFCComboBox::~thMFCComboBox()
{

}

bool thMFCComboBox::registerToLua(lua_State* luas)
{
	thClassDef<thMFCComboBox>(luas, "thMFCComboBox")
		// as func
		.funcr<int>("GetCount", thDF_CFuncConv(thMFCComboBox, GetCount))
		.funcr<int>("GetCurSel", thDF_CFuncConv(thMFCComboBox, GetCurSel))
		.func<int>("SetCurSel", thDF_CFuncConv(thMFCComboBox, SetCurSel))
		.func<const std::string&>("AddString", thDF_CFuncConv(thMFCComboBox, AddString))
		.func<int, const std::string&>("InsertString", thDF_CFuncConv(thMFCComboBox, InsertString))
		.end();

	return true;
}

thMFCComboBox* thMFCComboBox::create(CComboBox* ctrl)
{
	auto ret = new thMFCComboBox(ctrl);
	ret->autorelease();
	return ret;
}

int thMFCComboBox::GetCount() const
{
	return m_ctrl->GetCount();
}

int thMFCComboBox::GetCurSel() const
{
	return m_ctrl->GetCurSel();
}

void thMFCComboBox::SetCurSel(int nSelect)
{
	m_ctrl->SetCurSel(nSelect);
}

void thMFCComboBox::AddString(const std::string& lpszString)
{
	m_ctrl->AddString(lpszString.c_str());
}

void thMFCComboBox::InsertString(int nIndex, const std::string& lpszString)
{
	m_ctrl->InsertString(nIndex, lpszString.c_str());
}
