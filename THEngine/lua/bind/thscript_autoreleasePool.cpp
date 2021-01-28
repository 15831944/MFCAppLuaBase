/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
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
#include "thscript_autoreleasePool.h"
#include "thscript_dec.h"

thAutoreleasePool::thAutoreleasePool()
: _name("")
{
    _managedObjectArray.reserve(150);
    thPoolManager::getInstance()->push(this);
}

thAutoreleasePool::thAutoreleasePool(const std::string &name)
: _name(name)
{
    _managedObjectArray.reserve(150);
    thPoolManager::getInstance()->push(this);
}

thAutoreleasePool::~thAutoreleasePool()
{
    __thDF_LOG("deallocing thAutoreleasePool: %p", this);
    clear();
    
    thPoolManager::getInstance()->pop();
}

void thAutoreleasePool::addObject(thRef* object)
{
    _managedObjectArray.emplace_back(object);
}

void thAutoreleasePool::clear()
{
	if (!_managedObjectArray.empty())
	{
		std::vector<thRef*> releasings;
		releasings.swap(_managedObjectArray);
		for (const auto &obj : releasings)
		{
			obj->release();
		}
	}
}

bool thAutoreleasePool::contains(thRef* object) const
{
    for (const auto& obj : _managedObjectArray)
    {
        if (obj == object)
            return true;
    }
    return false;
}

void thAutoreleasePool::dump()
{
    __thDF_LOG("autorelease pool: %s, number of managed object %d\n", _name.c_str(), static_cast<int>(_managedObjectArray.size()));
    __thDF_LOG("%20s%20s%20s", "Object pointer", "Object id", "reference count");
    for (const auto &obj : _managedObjectArray)
    {
        __thDF_LOG("%20p%20u\n", obj, obj->getReferenceCount());
    }
}


//--------------------------------------------------------------------
//
// thPoolManager
//
//--------------------------------------------------------------------

thPoolManager* thPoolManager::s_singleInstance = nullptr;

thPoolManager* thPoolManager::getInstance()
{
    if (s_singleInstance == nullptr)
    {
        s_singleInstance = new (std::nothrow) thPoolManager();
        // Add the first auto release pool
        new thAutoreleasePool("autorelease pool");
    }
    return s_singleInstance;
}

void thPoolManager::destroyInstance()
{
    delete s_singleInstance;
    s_singleInstance = nullptr;
}

thPoolManager::thPoolManager()
{
    _releasePoolStack.reserve(10);
}

thPoolManager::~thPoolManager()
{
    __thDF_LOG("deallocing thPoolManager: %p", this);
    
    while (!_releasePoolStack.empty())
    {
        thAutoreleasePool* pool = _releasePoolStack.back();
        
        delete pool;
    }
}


thAutoreleasePool* thPoolManager::getCurrentPool() const
{
    return _releasePoolStack.back();
}

bool thPoolManager::isObjectInPools(thRef* obj) const
{
    for (const auto& pool : _releasePoolStack)
    {
        if (pool->contains(obj))
            return true;
    }
    return false;
}

void thPoolManager::push(thAutoreleasePool *pool)
{
    _releasePoolStack.emplace_back(pool);
}

void thPoolManager::pop()
{
    __thDF_assert(!_releasePoolStack.empty(), "thPoolManager::pop");
    _releasePoolStack.pop_back();
}
