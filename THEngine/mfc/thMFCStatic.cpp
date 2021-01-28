#include "pch.h"
#include "thMFCStatic.h"
#include "../lua/bind/thscript_bind.h"

thMFCStatic::thMFCStatic(CStatic* ctrl)
	: m_ctrl(ctrl)
{

}

thMFCStatic::~thMFCStatic()
{

}

bool thMFCStatic::registerToLua(lua_State* luas)
{
	thClassDef<thMFCStatic>(luas, "thMFCStatic")
		// as func
		.func<const std::string&>("SetWindowText", thDF_CFuncConv(thMFCStatic, SetWindowText))
		.funcr<std::string>("GetWindowText", thDF_CFuncConv(thMFCStatic, GetWindowText))
		.end();

	return true;
}

thMFCStatic* thMFCStatic::create(CStatic* ctrl)
{
	auto ret = new thMFCStatic(ctrl);
	ret->autorelease();
	return ret;
}

void thMFCStatic::SetWindowText(const std::string& str)
{
	m_ctrl->SetWindowText(str.c_str());
}

std::string thMFCStatic::GetWindowText()
{
	CString str;
	m_ctrl->GetWindowText(str);
	return str.GetString();
}
