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

#include "thscript_luaEngine.h"
#include "thscript_toluafix.h"
#include "thscript_autoreleasePool.h"

std::string g_path;

thLuaEngine* thLuaEngine::_defaultEngine = nullptr;

thLuaEngine* thLuaEngine::getInstance(void)
{
    if (!_defaultEngine)
    {
        _defaultEngine = new (std::nothrow) thLuaEngine();
        _defaultEngine->init();
    }
    return _defaultEngine;
}

thLuaEngine::~thLuaEngine(void)
{
	removeObjectAll();
    SAFE_RELEASE(_stack);
    _defaultEngine = nullptr;
}

bool thLuaEngine::init(void)
{
    _stack = thLuaStack::create();
    _stack->retain();
    return true;
}

void thLuaEngine::clear(void)
{
	thPoolManager::getInstance()->getCurrentPool()->clear();
}

void thLuaEngine::addSearchPath(const char* path)
{
    _stack->addSearchPath(path);
}

void thLuaEngine::addLuaLoader(lua_CFunction func)
{
    _stack->addLuaLoader(func);
}

void thLuaEngine::removeScriptObjectByObject(thRef* pObj)
{
    _stack->removeScriptObjectByObject(pObj);
    // ScriptHandlerMgr::getInstance()->removeObjectAllHandlers(pObj);
}

void thLuaEngine::removeScriptHandler(int nHandler)
{
	_stack->removeScriptHandler(nHandler);
}

int thLuaEngine::executeString(const char *codes)
{
    int ret = _stack->executeString(codes);
    _stack->clean();
    return ret;
}

int thLuaEngine::executeScriptFile(const char* filename)
{
    int ret = _stack->executeScriptFile(filename);
    _stack->clean();
    return ret;
}

int thLuaEngine::executeGlobalFunction(const char* functionName)
{
    int ret = _stack->executeGlobalFunction(functionName);
    _stack->clean();
    return ret;
}


int thLuaEngine::executeGlobalFunction(const char* functionName, int argsNum)
{
	int ret = _stack->executeGlobalFunction(functionName, argsNum);
	_stack->clean();
	return ret;
}

bool thLuaEngine::handleAssert(const char *msg)
{
    bool ret = _stack->handleAssert(msg);
    _stack->clean();
    return ret;
}

void thLuaEngine::addObject(thRef* obj)
{
	if (!obj) return;
	for (auto &v : _vObject)
	{
		if (v == obj) return;
	}
	_vObject.emplace_back(obj);
	obj->retain();
}

void thLuaEngine::removeObject(thRef* obj)
{
	if (!obj) return;
	int i = 0;
	for (auto &v : _vObject)
	{
		if (v == obj)
		{
			v->release();
			_vObject.erase(_vObject.begin() + i);
			return;
		}
		++i;
	}
}

void thLuaEngine::removeObjectAll(void)
{
	for (auto &v: _vObject)
	{
		v->release();
	}
	_vObject.clear();
}

int thLuaEngine::reallocateScriptHandler(int nHandler)
{    
    int nRet = _stack->reallocateScriptHandler(nHandler);
    _stack->clean();
    return nRet;
}

int thLuaEngine::reload(const char* moduleFileName)
{
    return _stack->reload(moduleFileName);
}

void thLuaEngine::setAppPath(const std::string& path)
{
	g_path = path;
	if (g_path.length() > 0 && g_path[g_path.length()-1] != '/'
		&& g_path[g_path.length() - 1] != '\\')
	{
		g_path.push_back('/');
	}
}

bool thLuaEngine::isFileExist(const std::string& file)
{
	auto fpath = g_path + file;
	auto fp = fopen(fpath.c_str(), "r");
	if (!fp)
	{
		return false;
	}
	fclose(fp);
	return true;
}

std::string thLuaEngine::fullPathForFilename(const std::string& file)
{
	return g_path + file;
}

std::string thLuaEngine::loadFileToString(const std::string& file)
{
	auto fpath = g_path + file;
	auto fp = fopen(fpath.c_str(), "rb");
	if (!fp)
	{
		return "";
	}
	fseek(fp, 0, SEEK_END);
	auto len = (int)ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::string buff;
	buff.resize(len);
	auto rlen = fread((void*)buff.data(),sizeof(char), len, fp);
	return buff;
}