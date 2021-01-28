#pragma once

#include "../lua/bind/thscript_bind.h"
#include "thMFCManager.h"
#include "thMFCListCtrl.h"
#include "thMFCComboBox.h"
#include "thMFCEdit.h"
#include "thMFCStatic.h"

inline bool registerMFCToLua(lua_State* luas)
{
	thMFCListCtrl::registerToLua(luas);
	thMFCStatic::registerToLua(luas);
	thMFCEdit::registerToLua(luas);
	thMFCComboBox::registerToLua(luas);
	thMFCManager::registerToLua(luas);
	return true;
}