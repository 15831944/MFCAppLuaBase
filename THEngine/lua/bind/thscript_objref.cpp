
#include "thscript_objref.h"
#include "thscript_luaEngine.h"
#include "../tolua/tolua++.h"
#include "thscript_toluafix.h"
#include "thscript_value.h"
#include "thscript_autoreleasePool.h"
#include "thscript_allocator.h"

thRef::thRef()
	: _referenceCount(1) // when the Ref is created, the reference count of it is 1
	, _luaID(0)
{
	static unsigned int uObjectCount = 0;
	_ID = ++uObjectCount;

}

thRef::~thRef()
{
	thLuaEngine* pEngine = thLuaEngine::getInstance();
	if (pEngine != nullptr && _luaID)
	{
		// if the object is referenced by Lua engine, remove it
		pEngine->removeScriptObjectByObject(this);
	}
}

void thRef::retain()
{
	__thDF_assert(_referenceCount > 0, "reference count should be greater than 0");
	++_referenceCount;
}

void thRef::release()
{
	__thDF_assert(_referenceCount > 0, "reference count should be greater than 0");
	--_referenceCount;

	// 下层逻辑处理
	bool bRemove = true;
	if (!_release())
	{
		// 已经删除了
		bRemove = false;
	}

	if (_referenceCount == 0)
	{
		if (bRemove)
		{
			delete this;
		}
	}
}

thRef* thRef::autorelease()
{
	thPoolManager::getInstance()->getCurrentPool()->addObject(this);
	return this;
}

unsigned int thRef::getReferenceCount() const
{
	return _referenceCount;
}


//////////////////////////////////////////////////////////////////////////

// 读取返回值到thLValue
thValue* _readRetValue(lua_State* luas, int argc)
{
	int index = argc-1;
	thValue* value = thValue::create();
	for (int i = 0; i < argc; ++i)
	{
		int lo = -1;
		value->readLuaValue(luas, lo, index);
		index -= 1;
		lua_pop(luas, 1);
	}

	return value;
}

thLuaFunction::thLuaFunction()
	: _luafuncHandler(0)
{

}

thLuaFunction::thLuaFunction(unsigned int luafuncHandler)
	: _luafuncHandler(luafuncHandler)
{

}

thLuaFunction::~thLuaFunction()
{
	if (_luafuncHandler != 0)
		thLuaEngine::getInstance()->removeScriptHandler(_luafuncHandler);
}

thLuaFunction* thLuaFunction::create(unsigned int luafuncHandler)
{
	auto lf = thAllocator::allocateType<thLuaFunction>(luafuncHandler);
	lf->autorelease();
	return lf;
}

bool thLuaFunction::_release(void)
{
	if (_referenceCount == 0)
	{
		thAllocator::freeType(this);
	}
	return false;
}

void thLuaFunction::call(thValue* arg, bool isClassPtr)
{
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeFunctionByHandlerArg(_luafuncHandler, arg, isClassPtr);
	stack->clean();
}

int thLuaFunction::call_i(thValue* arg, bool isClassPtr)
{
	int ret = 0;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeFunctionByHandlerArg(_luafuncHandler, arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = (int)tolua_tonumber(luas, -1, 0);
		lua_pop(luas, 1);
	});
	stack->clean();
	return ret;
}

std::string thLuaFunction::call_s(thValue* arg, bool isClassPtr)
{
	std::string ret;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeFunctionByHandlerArg(_luafuncHandler, arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = tolua_tocppstring(luas, -1, "");
		lua_pop(luas, 1);
	});
	stack->clean();
	return ret;
}

thValue* thLuaFunction::call_v(thValue* arg, bool isClassPtr)
{
	thValue* ret = nullptr;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeFunctionByHandlerArg(_luafuncHandler, arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = _readRetValue(luas, numResults);
	});
	stack->clean();
	return ret;
}

void thLuaFunction::call(const std::string& funcName, thValue* arg, bool isClassPtr)
{
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeGlobalFunctionByNameArg(funcName.c_str(), arg, isClassPtr);
	stack->clean();
}

int thLuaFunction::call_i(const std::string& funcName, thValue* arg, bool isClassPtr)
{
	int ret = 0;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeGlobalFunctionByNameArg(funcName.c_str(), arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = (int)tolua_tonumber(luas, -1, 0);
		lua_pop(luas, 1);
	});
	stack->clean();
	return ret;
}

std::string thLuaFunction::call_s(const std::string& funcName, thValue* arg, bool isClassPtr)
{
	std::string ret;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeGlobalFunctionByNameArg(funcName.c_str(), arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = tolua_tocppstring(luas, -1, "");
		lua_pop(luas, 1);
	});
	stack->clean();
	return ret;
}

thValue* thLuaFunction::call_v(const std::string& funcName, thValue* arg, bool isClassPtr)
{
	thValue* ret = nullptr;
	thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
	stack->executeGlobalFunctionByNameArg(funcName.c_str(), arg, isClassPtr, [&ret](lua_State* luas, int numResults)
	{
		ret = _readRetValue(luas, numResults);
	});
	stack->clean();
	return ret;
}

