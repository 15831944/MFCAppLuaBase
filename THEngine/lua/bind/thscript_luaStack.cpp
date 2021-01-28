/****************************************************************************
 Copyright (c) 2011-2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "thscript_luaStack.h"
#include "thscript_toluafix.h"
#include "thscript_luaEngine.h"
#include "thscript_value.h"
#include <string.h>
#include "../xxtea/xxtea.h"
#include "../lua/lua.h"
#include "../tolua/tolua++.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"


namespace {
    int get_string_for_print(lua_State * L, std::string* out)
    {
        int n = lua_gettop(L);  /* number of arguments */
        int i;

        lua_getglobal(L, "tostring");
        for (i=1; i<=n; i++) {
            const char *s;
            lua_pushvalue(L, -1);  /* function to be called */
            lua_pushvalue(L, i);   /* value to print */
            lua_call(L, 1, 1);
            size_t sz;
            s = lua_tolstring(L, -1, &sz);  /* get result */
            if (s == NULL)
                return luaL_error(L, LUA_QL("tostring") " must return a string to "
                                  LUA_QL("print"));
            if (i>1) out->append("\t");
            out->append(s, sz);
            lua_pop(L, 1);  /* pop result */
        }
        return 0;
    }

    int lua_print(lua_State * L)
    {
        std::string t;
        get_string_for_print(L, &t);
		__thDF_LOG("[LUA-print] %s\n", t.c_str());

        return 0;
    }

    int lua_release_print(lua_State * L)
    {
        std::string t;
        get_string_for_print(L, &t);
		__thDF_LOG("[LUA-print] %s", t.c_str());

        return 0;
    }

	int cocos2dx_lua_loader(lua_State *L)
	{
		static const std::string BYTECODE_FILE_EXT = ".luac";
		static const std::string NOT_BYTECODE_FILE_EXT = ".lua";

		std::string filename(luaL_checkstring(L, 1));
		size_t pos = filename.rfind(BYTECODE_FILE_EXT);
		if (pos != std::string::npos && pos == filename.length() - BYTECODE_FILE_EXT.length())
			filename = filename.substr(0, pos);
		else
		{
			pos = filename.rfind(NOT_BYTECODE_FILE_EXT);
			if (pos != std::string::npos && pos == filename.length() - NOT_BYTECODE_FILE_EXT.length())
				filename = filename.substr(0, pos);
		}

		pos = filename.find_first_of('.');
		while (pos != std::string::npos)
		{
			filename.replace(pos, 1, "/");
			pos = filename.find_first_of('.');
		}

		// search file in package.path
		std::string chunk;
		std::string chunkName;

		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path");
		std::string searchpath(lua_tostring(L, -1));
		lua_pop(L, 1);
		size_t begin = 0;
		size_t next = searchpath.find_first_of(';', 0);

		do
		{
			if (next == std::string::npos)
				next = searchpath.length();
			std::string prefix = searchpath.substr(begin, next - begin);
			if (prefix[0] == '.' && prefix[1] == '/')
				prefix = prefix.substr(2);

			pos = prefix.rfind(BYTECODE_FILE_EXT);
			if (pos != std::string::npos && pos == prefix.length() - BYTECODE_FILE_EXT.length())
			{
				prefix = prefix.substr(0, pos);
			}
			else
			{
				pos = prefix.rfind(NOT_BYTECODE_FILE_EXT);
				if (pos != std::string::npos && pos == prefix.length() - NOT_BYTECODE_FILE_EXT.length())
					prefix = prefix.substr(0, pos);
			}
			pos = prefix.find_first_of('?', 0);
			while (pos != std::string::npos)
			{
				prefix.replace(pos, 1, filename);
				pos = prefix.find_first_of('?', pos + filename.length() + 1);
			}
			chunkName = prefix + BYTECODE_FILE_EXT;
			chunk = thLuaEngine::getInstance()->loadFileToString(chunkName);
			if (!chunk.empty()) // && !utils->isDirectoryExist(chunkName))
			{
				break;
			}
			else
			{
				chunkName = prefix + NOT_BYTECODE_FILE_EXT;
				chunk = thLuaEngine::getInstance()->loadFileToString(chunkName);
				if (!chunk.empty()) //&& !utils->isDirectoryExist(chunkName))
				{
					break;
				}
				else
				{
					chunkName = prefix;
					chunk = thLuaEngine::getInstance()->loadFileToString(chunkName);
					if (!chunk.empty()) // && !utils->isDirectoryExist(chunkName))
					{
						break;
					}
				}
			}

			begin = next + 1;
			next = searchpath.find_first_of(';', begin);
		} while (begin < searchpath.length());
		if (chunk.length() > 0)
		{
			thLuaStack* stack = thLuaEngine::getInstance()->getLuaStack();
			stack->luaLoadBuffer(L, reinterpret_cast<const char*>(chunk.data()),
				static_cast<int>(chunk.length()), chunkName.c_str());
		}
		else
		{
			__thDF_LOG("can not get file data of %s", chunkName.c_str());
			return 0;
		}

		return 1;
	}
}

thLuaStack::~thLuaStack()
{
    if (nullptr != _state)
    {
        lua_close(_state);
    }
}

thLuaStack *thLuaStack::create(void)
{
    thLuaStack *stack = new (std::nothrow) thLuaStack();
    stack->init();
    stack->autorelease();
    return stack;
}

thLuaStack *thLuaStack::attach(lua_State *L)
{
    thLuaStack *stack = new (std::nothrow) thLuaStack();
    stack->initWithLuaState(L);
    stack->autorelease();
    return stack;
}

bool thLuaStack::init(void)
{
    _state = lua_open();
    luaL_openlibs(_state);
    toluafix_open(_state);

    // Register our version of the global "print" function
    const luaL_Reg global_functions [] = {
        {"print", lua_print},
        {"release_print",lua_release_print},
        {nullptr, nullptr}
    };
    luaL_register(_state, "_G", global_functions);

    g_luaType.clear();

    // add cocos2dx loader
    addLuaLoader(cocos2dx_lua_loader);

	thValue::registerToLua(_state);

    return true;
}

bool thLuaStack::initWithLuaState(lua_State *L)
{
    _state = L;
    return true;
}

void thLuaStack::addSearchPath(const char* path)
{
    lua_getglobal(_state, "package");                                  /* L: package */
    lua_getfield(_state, -1, "path");                /* get package.path, L: package path */
    const char* cur_path =  lua_tostring(_state, -1);
    lua_pushfstring(_state, "%s;%s/?.lua", cur_path, path);            /* L: package path newpath */
    lua_setfield(_state, -3, "path");          /* package.path = newpath, L: package path */
    lua_pop(_state, 2);                                                /* L: - */
}

void thLuaStack::addLuaLoader(lua_CFunction func)
{
    if (!func) return;

    // stack content after the invoking of the function
    // get loader table
    lua_getglobal(_state, "package");                                  /* L: package */
    lua_getfield(_state, -1, "loaders");                               /* L: package, loaders */

    // insert loader into index 2
    lua_pushcfunction(_state, func);                                   /* L: package, loaders, func */
    for (int i = (int)(lua_objlen(_state, -2) + 1); i > 2; --i)
    {
        lua_rawgeti(_state, -2, i - 1);                                /* L: package, loaders, func, function */
        // we call lua_rawgeti, so the loader table now is at -3
        lua_rawseti(_state, -3, i);                                    /* L: package, loaders, func */
    }
    lua_rawseti(_state, -2, 2);                                        /* L: package, loaders */

    // set loaders into package
    lua_setfield(_state, -2, "loaders");                               /* L: package */

    lua_pop(_state, 1);
}


void thLuaStack::removeScriptObjectByObject(thRef* pObj)
{
    toluafix_remove_ccobject_by_refid(_state, pObj->_luaID);
}

void thLuaStack::removeScriptHandler(int nHandler)
{
	toluafix_remove_function_by_refid(_state, nHandler);
}

int thLuaStack::executeString(const char *codes)
{
    luaL_loadstring(_state, codes);
    return executeFunction(0);
}

static const std::string BYTECODE_FILE_EXT    = ".luac";
static const std::string NOT_BYTECODE_FILE_EXT = ".lua";

int thLuaStack::executeScriptFile(const char* filename)
{
    __thDF_assert(filename, "thLuaStack::executeScriptFile() - invalid filename");

    std::string buf(filename);
    //
    // remove .lua or .luac
    //
    size_t pos = buf.rfind(BYTECODE_FILE_EXT);
    if (pos != std::string::npos)
    {
        buf = buf.substr(0, pos);
    }
    else
    {
        pos = buf.rfind(NOT_BYTECODE_FILE_EXT);
        if (pos == buf.length() - NOT_BYTECODE_FILE_EXT.length())
        {
            buf = buf.substr(0, pos);
        }
    }

    //
    // 1. check .luac suffix
    // 2. check .lua suffix
    //
    std::string tmpfilename = buf + BYTECODE_FILE_EXT;
    if (thLuaEngine::getInstance()->isFileExist(tmpfilename))
    {
        buf = tmpfilename;
    }
    else
    {
        tmpfilename = buf + NOT_BYTECODE_FILE_EXT;
        if (thLuaEngine::getInstance()->isFileExist(tmpfilename))
        {
            buf = tmpfilename;
        }
    }

	std::string data = thLuaEngine::getInstance()->loadFileToString(buf);
    int rn = 0;
    if (!data.empty())
    {
		auto fullpath = thLuaEngine::getInstance()->fullPathForFilename(buf);
        if (luaLoadBuffer(_state, (const char*)data.data(), (int)data.length(), fullpath.c_str()) == 0)
        {
            rn = executeFunction(0);
        }
    }
    return rn;
}

int thLuaStack::executeGlobalFunction(const char* functionName)
{
    lua_getglobal(_state, functionName);       /* query function by name, stack: function */
    if (!lua_isfunction(_state, -1))
    {
        __thDF_LOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
        lua_pop(_state, 1);
        return 0;
    }
    return executeFunction(0);
}


int thLuaStack::executeGlobalFunction(const char* functionName, int argsNumber, const std::function<void(lua_State*, int)>& func)
{
	lua_getglobal(_state, functionName);
	if (lua_isfunction(_state, -1))
	{
		if (argsNumber > 0)
		{
			lua_insert(_state, -(argsNumber + 1));
		}

		int top = lua_gettop(_state);

		int traceback = 0;
		lua_getglobal(_state, "__G__TRACKBACK__");
		if (!lua_isfunction(_state, -1))
		{
			lua_pop(_state, 1);
		}
		else
		{
			traceback = -(argsNumber + 2);
			lua_insert(_state, traceback);
		}

		int error = 0;
		error = lua_pcall(_state, argsNumber, -1, traceback);
		if (error)
		{
			if (traceback == 0)
			{
				__thDF_LOG("[LUA ERROR] %s", lua_tostring(_state, -1));
				lua_pop(_state, 1); // remove error message from stack
			}
			return 0;
		}

		// 动态获取返回参数需要numResults填-1
		auto numResults = lua_gettop(_state) - top + argsNumber;

		// get return value
		int ret = 0;
		//if (lua_isnumber(_state, -1))
		//{
		//	ret = lua_tointeger(_state, -1);
		//}
		//else if (lua_isboolean(_state, -1))
		//{
		//	ret = lua_toboolean(_state, -1);
		//}
		do {

			if (numResults <= 0 || nullptr == func)
				break;

			func(_state, numResults);

		} while (0);

		if (traceback) lua_pop(_state, 1); // remove return value from stack
		return ret;
	}
	else
	{
		__thDF_LOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(_state, 1);
		lua_pop(_state, argsNumber); // remove args from stack
		return 0;
	}
}

int thLuaStack::executeGlobalFunctionByNameArg(const char* functionName, thValue* arg, bool isClassPtr, const std::function<void(lua_State*, int)>& func /*= nullptr*/)
{
	if (!arg) return executeGlobalFunction(functionName, 0);
	else
	{
		if (isClassPtr)
		{
			pushObject(arg, "thValue");
			return executeGlobalFunction(functionName, 1, func);
		}
		arg->retValueToLua(_state);
		return executeGlobalFunction(functionName, arg->size(), func);
	}
}

void thLuaStack::clean(void)
{
    lua_settop(_state, 0);
}
//
//void thLuaStack::pushValueDec(thDEC_Value* val)
//{
//	if (!val) return;
//
//	switch (val->getType())
//	{
//	case thDEC_Value::Type::INTEGER:
//	{
//		auto _val = val->asInt();
//		pushInt(_val);
//		break;
//	}
//	case thDEC_Value::Type::THVALUE:
//	{
//		auto _val = val->asValue();
//		pushValue(_val, true);
//		break;
//	}
//	case thDEC_Value::Type::BOOLEAN:
//	{
//		auto _val = val->asBool();
//		pushBoolean(_val);
//		break;
//	}
//	case thDEC_Value::Type::FLOAT:
//	{
//		auto _val = val->asFloat();
//		pushFloat(_val);
//		break;
//	}
//	case thDEC_Value::Type::UNSIGNED:
//	{
//		auto _val = val->asUnsignedInt();
//		pushLong(_val);
//		break;
//	}
//	case thDEC_Value::Type::STRING:
//	{
//		auto _val = val->asString();
//		pushString(_val.c_str(), _val.length());
//		break;
//	}
//	case thDEC_Value::Type::DOUBLE:
//	{
//		auto _val = val->asDouble();
//		pushDouble(_val);
//		break;
//	}
//	case thDEC_Value::Type::ULONGLONG:
//	{
//		auto _val = val->asULongLong();
//		char szfmt[100];
//		snprintf(szfmt, 100, "%llu", _val);
//		pushString(szfmt);
//		break;
//	}
//	case thDEC_Value::Type::OBJECT:
//	{
//		auto _val = val->asObject();
//		pushObject(_val, getClassType(typeid(*_val).name()).c_str());
//		break;
//	}
//	default:
//		pushNil();
//		break;
//	}
//}
//
//void thLuaStack::pushValue(thValue* value, bool isTable)
//{
//	if (value)
//	{
//		auto isArray = value->isArray();
//		if (isTable)
//		{
//			// L: table
//			lua_newtable(_state);
//		}
//		
//		// array
//		if (isArray)
//		{
//			int idx = 0;
//			while (true)
//			{
//				// value
//				auto val = (thDEC_Value*)value->getValue(idx);
//				if (val->isNull())
//				{
//					break;
//				}
//				pushValueDec(val);
//				if (isTable)
//				{
//					// table[index] = value, L: table
//					lua_rawseti(_state, -2, idx+1);
//				}
//				++idx;
//			}
//		}
//		else
//		{
//			// map
//			auto& map = value->_getMapValue();
//			for (auto &v: map)
//			{
//				// add key
//				if (isTable)
//				{
//					auto strkey = value->_getValueKey(v.first);
//					if (!strkey) break;
//					lua_pushlstring(_state, strkey->c_str(), strkey->length());
//				}
//				// value
//				pushValueDec(v.second);
//
//				if (isTable)
//				{
//					// table.key = value, L: table
//					lua_rawset(_state, -3);
//				}
//			}
//		}
//		
//	}
//}

void thLuaStack::pushInt(int intValue)
{
    lua_pushinteger(_state, intValue);
}

void thLuaStack::pushFloat(float floatValue)
{
    lua_pushnumber(_state, floatValue);
}

void thLuaStack::pushDouble(double value)
{
	lua_pushnumber(_state, value);
}

void thLuaStack::pushLong(long longValue)
{
    lua_pushnumber(_state, longValue);
}

void thLuaStack::pushBoolean(bool boolValue)
{
    lua_pushboolean(_state, boolValue);
}

void thLuaStack::pushString(const char* stringValue)
{
    lua_pushstring(_state, stringValue);
}

void thLuaStack::pushString(const char* stringValue, int length)
{
    lua_pushlstring(_state, stringValue, length);
}

void thLuaStack::pushNil(void)
{
    lua_pushnil(_state);
}

void thLuaStack::pushObject(thRef* objectValue, const char* typeName)
{
    toluafix_pushusertype_ccobject(_state, objectValue->_ID, &objectValue->_luaID, objectValue, typeName);
}

void thLuaStack::pushLuaValue(const thLuaValue& value)
{
    const thLuaValueType type = value.getType();
    if (type == thLuaValueTypeInt)
    {
        return pushInt(value.intValue());
    }
    else if (type == thLuaValueTypeFloat)
    {
        return pushFloat(value.floatValue());
    }
    else if (type == thLuaValueTypeBoolean)
    {
        return pushBoolean(value.booleanValue());
    }
    else if (type == thLuaValueTypeString)
    {
        return pushString(value.stringValue().c_str());
    }
    else if (type == thLuaValueTypeDict)
    {
        pushLuaValueDict(value.dictValue());
    }
    else if (type == thLuaValueTypeArray)
    {
        pushLuaValueArray(value.arrayValue());
    }
    else if (type == thLuaValueTypeObject)
    {
        pushObject(value.ccobjectValue(), value.getObjectTypename().c_str());
    }
}

void thLuaStack::pushLuaValueDict(const thLuaValueDict& dict)
{
    lua_newtable(_state);                                              /* L: table */
    for (thLuaValueDictIterator it = dict.begin(); it != dict.end(); ++it)
    {
        lua_pushstring(_state, it->first.c_str());                     /* L: table key */
        pushLuaValue(it->second);                                     /* L: table key value */
        lua_rawset(_state, -3);                     /* table.key = value, L: table */
    }
}

void thLuaStack::pushLuaValueArray(const thLuaValueArray& array)
{
    lua_newtable(_state);                                              /* L: table */
    int index = 1;
    for (thLuaValueArrayIterator it = array.begin(); it != array.end(); ++it)
    {
        pushLuaValue(*it);                                            /* L: table value */
        lua_rawseti(_state, -2, index);          /* table[index] = value, L: table */
        ++index;
    }
}

bool thLuaStack::pushFunctionByHandler(int nHandler)
{
    toluafix_get_function_by_refid(_state, nHandler);                  /* L: ... func */
    if (!lua_isfunction(_state, -1))
    {
        __thDF_LOG("[LUA ERROR] function refid '%d' does not reference a Lua function", nHandler);
        lua_pop(_state, 1);
        return false;
    }
    return true;
}

int thLuaStack::executeFunction(int numArgs)
{
    int functionIndex = -(numArgs + 1);
    if (!lua_isfunction(_state, functionIndex))
    {
        __thDF_LOG("value at stack [%d] is not function", functionIndex);
        lua_pop(_state, numArgs + 1); // remove function and arguments
        return 0;
    }

    int traceback = 0;
    lua_getglobal(_state, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(_state, -1))
    {
        lua_pop(_state, 1);                                            /* L: ... func arg1 arg2 ... */
    }
    else
    {
        lua_insert(_state, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
        traceback = functionIndex - 1;
    }

    int error = 0;
    ++_callFromLua;
    error = lua_pcall(_state, numArgs, 1, traceback);                  /* L: ... [G] ret */
    --_callFromLua;
    if (error)
    {
        if (traceback == 0)
        {
            __thDF_LOG("[LUA ERROR] %s", lua_tostring(_state, - 1));        /* L: ... error */
            lua_pop(_state, 1); // remove error message from stack
        }
        else                                                            /* L: ... G error */
        {
            lua_pop(_state, 2); // remove __G__TRACKBACK__ and error message from stack
        }
        return 0;
    }

    // get return value
    int ret = 0;
    if (lua_isnumber(_state, -1))
    {
        ret = (int)lua_tointeger(_state, -1);
    }
    else if (lua_isboolean(_state, -1))
    {
        ret = (int)lua_toboolean(_state, -1);
    }
    // remove return value from stack
    lua_pop(_state, 1);                                                /* L: ... [G] */

    if (traceback)
    {
        lua_pop(_state, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
    }

    return ret;
}

int thLuaStack::executeFunctionByHandler(int nHandler, int numArgs)
{
    int ret = 0;
    if (pushFunctionByHandler(nHandler))                                /* L: ... arg1 arg2 ... func */
    {
        if (numArgs > 0)
        {
            lua_insert(_state, -(numArgs + 1));                        /* L: ... func arg1 arg2 ... */
        }
		ret = executeFunction(numArgs);
    }
    lua_settop(_state, 0);
    return ret;
}

int thLuaStack::executeFunctionByHandlerArg(int nHandler, thValue* arg, bool isClassPtr, const std::function<void(lua_State*, int)>& func)
{
	if (!arg) return executeFunctionByHandler(nHandler, 0);
	else
	{
		if (isClassPtr)
		{
			pushObject(arg, "thValue");
			return executeFunction(nHandler, 1, -1, func);
		}
		arg->retValueToLua(_state);
		return executeFunction(nHandler, arg->size(), -1, func);
	}
}

bool thLuaStack::handleAssert(const char *msg)
{
    if (_callFromLua == 0) return false;

    lua_pushfstring(_state, "ASSERT FAILED ON LUA EXECUTE: %s", msg ? msg : "unknown");
    lua_error(_state);
    return true;
}

int thLuaStack::reallocateScriptHandler(int nHandler)
{
    LUA_FUNCTION  nNewHandle = -1;

    if (pushFunctionByHandler(nHandler))
    {
       nNewHandle = toluafix_ref_function(_state,lua_gettop(_state),0);
    }
/*
    toluafix_get_function_by_refid(_state,nNewHandle);
    if (!lua_isfunction(_state, -1))
    {
        __thDF_LOG("Error!");
    }
    lua_settop(_state, 0);
*/
    return nNewHandle;

}

int thLuaStack::executeFunction(int handler, int numArgs, int numResults, const std::function<void(lua_State*,int)>& func)
{
    if (pushFunctionByHandler(handler))                 /* L: ... arg1 arg2 ... func */
    {

        if (numArgs > 0)
        {
            lua_insert(_state, -(numArgs + 1));                        /* L: ... func arg1 arg2 ... */
        }

        int functionIndex = -(numArgs + 1);

        if (!lua_isfunction(_state, functionIndex))
        {
            __thDF_LOG("value at stack [%d] is not function", functionIndex);
            lua_pop(_state, numArgs + 1); // remove function and arguments
            return 0;
        }

		int top = lua_gettop(_state);

        int traceCallback = 0;
        lua_getglobal(_state, "__G__TRACKBACK__");                        /* L: ... func arg1 arg2 ... G */
        if (!lua_isfunction(_state, -1))
        {
            lua_pop(_state, 1);                                           /* L: ... func arg1 arg2 ... */
        }
        else
        {
            lua_insert(_state, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
            traceCallback = functionIndex - 1;
        }


        int error = 0;
        ++_callFromLua;
        error = lua_pcall(_state, numArgs, numResults, traceCallback);     /* L: ... [G] ret1 ret2 ... retResults*/
        --_callFromLua;

		// 动态获取返回参数需要numResults填-1
		numResults = lua_gettop(_state) - top + numArgs;

        if (error)
        {
            if (traceCallback == 0)
            {
                __thDF_LOG("[LUA ERROR] %s", lua_tostring(_state, - 1));        /* L: ... error */
                lua_pop(_state, 1);                                        // remove error message from stack
            }
            else                                                           /* L: ... G error */
            {
                lua_pop(_state, 2);                                        // remove __G__TRACKBACK__ and error message from stack
            }
            return 0;
        }

        // get return value,don't pass LUA_MULTRET to numResults,
        do {

            if (numResults <= 0 || nullptr == func)
                break;

            func(_state, numResults);

        } while (0);

        if (traceCallback)
        {
            lua_pop(_state, 1);                                          // remove __G__TRACKBACK__ from stack      /* L: ... */
        }
    }

    return 1;
}

int thLuaStack::reload(const char* moduleFileName)
{
    if (nullptr == moduleFileName || strlen(moduleFileName) == 0)
    {
        __thDF_LOG("moduleFileName is null");
        return 1;
    }

    lua_getglobal(_state, "package");                         /* L: package */
    lua_getfield(_state, -1, "loaded");                       /* L: package loaded */
    lua_pushstring(_state, moduleFileName);
    lua_gettable(_state, -2);                                 /* L:package loaded module */
    if (!lua_isnil(_state, -1))
    {
        lua_pushstring(_state, moduleFileName);               /* L:package loaded module name */
        lua_pushnil(_state);                                  /* L:package loaded module name nil*/
        lua_settable(_state, -4);                             /* L:package loaded module */
    }
    lua_pop(_state, 3);

    std::string name = moduleFileName;
    std::string require = "require \'" + name + "\'";
    return executeString(require.c_str());
}

void thLuaStack::setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign, int signLen)
{
    cleanupXXTEAKeyAndSign();

    if (key && keyLen && sign && signLen)
    {
        _xxteaKey = (char*)malloc(keyLen);
        memcpy(_xxteaKey, key, keyLen);
        _xxteaKeyLen = keyLen;

        _xxteaSign = (char*)malloc(signLen);
        memcpy(_xxteaSign, sign, signLen);
        _xxteaSignLen = signLen;

        _xxteaEnabled = true;
    }
    else
    {
        _xxteaEnabled = false;
    }
}

void thLuaStack::cleanupXXTEAKeyAndSign()
{
    if (_xxteaKey)
    {
        free(_xxteaKey);
        _xxteaKey = nullptr;
        _xxteaKeyLen = 0;
    }
    if (_xxteaSign)
    {
        free(_xxteaSign);
        _xxteaSign = nullptr;
        _xxteaSignLen = 0;
    }
}

namespace {

void skipBOM(const char*& chunk, int& chunkSize)
{
    // UTF-8 BOM? skip
    if (static_cast<unsigned char>(chunk[0]) == 0xEF &&
        static_cast<unsigned char>(chunk[1]) == 0xBB &&
        static_cast<unsigned char>(chunk[2]) == 0xBF)
    {
        chunk += 3;
        chunkSize -= 3;
    }
}

} // end anonymous namespace

int thLuaStack::luaLoadBuffer(lua_State *L, const char *chunk, int chunkSize, const char *chunkName)
{
    int r = 0;

    if (_xxteaEnabled && strncmp(chunk, _xxteaSign, _xxteaSignLen) == 0)
    {
        // decrypt XXTEA
        xxtea_long len = 0;
        unsigned char* result = xxtea_decrypt((unsigned char*)chunk + _xxteaSignLen,
                                              (xxtea_long)chunkSize - _xxteaSignLen,
                                              (unsigned char*)_xxteaKey,
                                              (xxtea_long)_xxteaKeyLen,
                                              &len);
        unsigned char* content = result;
        xxtea_long contentSize = len;
        skipBOM((const char*&)content, (int&)contentSize);
        r = luaL_loadbuffer(L, (char*)content, contentSize, chunkName);
        free(result);
    }
    else
    {
        skipBOM(chunk, chunkSize);
        r = luaL_loadbuffer(L, chunk, chunkSize, chunkName);
    }

#if defined(_DEBUG)
	if (r)
	{
		switch (r)
		{
		case LUA_ERRSYNTAX:
			__thDF_LOG("[LUA ERROR] load \"%s\", error: syntax error during pre-compilation.", chunkName);
			break;

		case LUA_ERRMEM:
			__thDF_LOG("[LUA ERROR] load \"%s\", error: memory allocation error.", chunkName);
			break;

		case LUA_ERRFILE:
			__thDF_LOG("[LUA ERROR] load \"%s\", error: cannot open/read file.", chunkName);
			break;

		default:
			__thDF_LOG("[LUA ERROR] load \"%s\", error: unknown.", chunkName);
			break;
		}
	}
#endif

    return r;
}

