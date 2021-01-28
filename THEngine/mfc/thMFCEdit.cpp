#include "pch.h"
#include "thMFCEdit.h"
#include "../lua/bind/thscript_bind.h"

thMFCEdit::thMFCEdit(CEdit* ctrl)
	: m_ctrl(ctrl)
{

}

thMFCEdit::~thMFCEdit()
{

}

bool thMFCEdit::registerToLua(lua_State* luas)
{
	thClassDef<thMFCEdit>(luas, "thMFCEdit")
		// as func
		.end();

	return true;
}

thMFCEdit* thMFCEdit::create(CEdit* ctrl)
{
	auto ret = new thMFCEdit(ctrl);
	ret->autorelease();
	return ret;
}

