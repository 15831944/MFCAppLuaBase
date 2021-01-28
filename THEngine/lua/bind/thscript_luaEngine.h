/****************************************************************************
 Copyright (c) 2012      cocos2d-x.org
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
#ifndef __THSCRIPT_LUAENGINE_H__
#define __THSCRIPT_LUAENGINE_H__

#include "../lua/lua.h"
#include "thscript_dec.h"
#include "thscript_luaStack.h"

// lua引擎
class thLuaEngine
{
public:
	virtual ~thLuaEngine(void);

    static thLuaEngine* getInstance(void);

	// 文件操作
public:
	// 设置应用资源路径
	void setAppPath(const std::string& path);
	// 判断文件是否存在
	bool isFileExist(const std::string& file);
	// 获取文件全路径
	std::string fullPathForFilename(const std::string& file);
	// 读取文件数据到string
	std::string loadFileToString(const std::string& file);

public:

    /**
     * Get LuaStack of the thLuaEngine.
     * All the interactive operation are all base on the LuaStack.
     *
     * @return LuaStack object.
     */
    thLuaStack* getLuaStack(void) {
        return _stack;
    }
	lua_State* getLuaState(void)
	{
		return _stack?_stack->getLuaState():nullptr;
	}

	// 外部update调用清理
	void clear(void);
    
    /**
     * Add a path to find lua files in.
     *
     * @param path to be added to the Lua path.
     */
    virtual void addSearchPath(const char* path);
    
    /**
     * Add lua loader.
     *
     * @param func a function pointer point to the loader function.
     */
    virtual void addLuaLoader(lua_CFunction func);
    
    /**
     * Reload script code corresponding to moduleFileName.
     * If value of package["loaded"][moduleFileName] is existed, it would set the value nil.Then,it calls executeString function.
     *
     * @param moduleFileName String object holding the filename of the script file that is to be executed.
     * @return 0 if the string is executed correctly or other if the string is executed wrongly.
     */
    virtual int reload(const char* moduleFileName);
    
    /**
     * Remove the related reference about the Ref object stored in the Lua table by set the value of corresponding key nil:
     * The related Lua tables are toluafix_refid_ptr_mapping,toluafix_refid_type_mapping,tolua_value_root and object_Metatable["tolua_ubox"] or tolua_ubox.
     * Meanwhile set the corresponding userdata nullptr and remove the all the lua function reference corresponding to this object.
     * 
     * In current mechanism, this function is called in the destructor of Ref object, developer don't call this functions.
     *
     * @param object the key object to remove script object.
     */
    virtual void removeScriptObjectByObject(thRef* object);
    
    /**
     * Remove Lua function reference by nHandler by setting toluafix_refid_function_mapping[nHandle] nil.
     *
     * @param nHandler the function reference index to find the corresponding Lua function pointer.
     */
    virtual void removeScriptHandler(int nHandler);
    
    /**
     * Reallocate Lua function reference index to the Lua function pointer to add reference.
     *
     * @param nHandler the function reference index to find the corresponding Lua function pointer.
     */
    virtual int reallocateScriptHandler(int nHandler);
    
    /**
     * Execute script code contained in the given string.
     *
     * @param codes holding the valid script code that should be executed.
     * @return 0 if the string is executed correctly, other if the string is executed wrongly.
     */
    virtual int executeString(const char* codes);
    
    /**
     * Execute a script file.
     *
     * @param filename String object holding the filename of the script file that is to be executed.
     * @return the return values by calling executeFunction.
     */
    virtual int executeScriptFile(const char* filename);
    
    /**
     * Execute a scripted global function.
     * The function should not take any parameters and should return an integer.
     *
     * @param functionName String object holding the name of the function, in the global script environment, that is to be executed.
     * @return The integer value returned from the script function.
     */
    virtual int executeGlobalFunction(const char* functionName);
	virtual int executeGlobalFunction(const char* functionName, int argsNum);
    /**
     * Handle the assert message.
     *
     * @return return true if current _callFromLua of LuaStack is not equal to 0 otherwise return false.
     */
    virtual bool handleAssert(const char *msg);


public:
	// 记录object
	void	addObject(thRef* obj);
	void	removeObject(thRef* obj);
	void	removeObjectAll(void);

    
private:
    thLuaEngine(void)
    : _stack(nullptr)
    {
    }
    bool init(void);
    
private:
    static thLuaEngine* _defaultEngine;
    thLuaStack *_stack;
	std::vector<thRef*> _vObject;
};


#endif // __THSCRIPT_LUAENGINE_H__
