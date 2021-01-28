
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

#ifndef __THSCRIPT_LUAVALUE_H_
#define __THSCRIPT_LUAVALUE_H_

#include <string>
#include <map>
#include <list>

#include "../lua/lua.h"
#include "thscript_objref.h"

/**
 * @addtogroup lua
 * @{
 */

typedef int LUA_FUNCTION;
typedef int LUA_TABLE;
typedef int LUA_STRING;

class thLuaValue;

typedef std::map<std::string, thLuaValue>   thLuaValueDict;
typedef thLuaValueDict::const_iterator      thLuaValueDictIterator;
typedef std::list<thLuaValue>               thLuaValueArray;
typedef thLuaValueArray::const_iterator     thLuaValueArrayIterator;

/// @cond
typedef enum {
    thLuaValueTypeInt,
    thLuaValueTypeFloat,
    thLuaValueTypeBoolean,
    thLuaValueTypeString,
    thLuaValueTypeDict,
    thLuaValueTypeArray,
    thLuaValueTypeObject
} thLuaValueType;
/// @endcond

/// @cond
typedef union {
    int                 intValue;
    float               floatValue;
    bool                booleanValue;
    std::string*        stringValue;
    thLuaValueDict*     dictValue;
    thLuaValueArray*    arrayValue;
    thRef*           ccobjectValue;
} thLuaValueField;
/// @endcond

/**
 * Wrap different general types of data into a same specific type named thLuaValue.
 * The general types supported as follows:int,float,bool,std::string,const char*,thLuaValueDict,thLuaValueArray,Ref.
 *
 * @lua NA
 * @js NA
 */
class thLuaValue
{
public:
    
    /**
     * Construct a thLuaValue object by a int value.
     *
     * @param intValue a int value.
     * @return a thLuaValue object.
     */
    static const thLuaValue intValue(const int intValue);
    
    /**
     * Construct a thLuaValue object by a float value.
     *
     * @param floatValue a float value.
     * @return a thLuaValue object.
     */
    static const thLuaValue floatValue(const float floatValue);
    
    /**
     * Construct a thLuaValue object by a boolean value.
     *
     * @param booleanValue a bool value.
     * @return a thLuaValue object.
     */
    static const thLuaValue booleanValue(const bool booleanValue);
    
    /**
     * Construct a thLuaValue object by a string pointer.
     *
     * @param stringValue a string pointer.
     * @return a thLuaValue object.
     */
    static const thLuaValue stringValue(const char* stringValue);
    
    /**
     * Construct a thLuaValue object by a std::string object.
     *
     * @param stringValue a std::string object.
     * @return a thLuaValue object.
     */
    static const thLuaValue stringValue(const std::string& stringValue);
    
    /**
     * Construct a thLuaValue object by a thLuaValueDict value.
     *
     * @param dictValue a thLuaValueDict object.
     * @return a thLuaValue object.
     */
    static const thLuaValue dictValue(const thLuaValueDict& dictValue);
    
    /**
     * Construct a thLuaValue object by a thLuaValueArray value.
     *
     * @param arrayValue a thLuaValueArray object.
     * @return a thLuaValue object.
     */
    static const thLuaValue arrayValue(const thLuaValueArray& arrayValue);
    
    /**
     * Construct a thLuaValue object by a Ref object.
     *
     * @param ccobjectValue a Ref object.
     * @param objectTypename a string pointer point to the typename of object.
     * @return a thLuaValue object.
     */
    static const thLuaValue ccobjectValue(thRef* ccobjectValue, const char* objectTypename);
    
    /**
     * Construct a thLuaValue object by a Ref object.
     *
     * @param ccobjectValue a Ref object.
     * @param objectTypename a std::string object represent the typename of object.
     * @return a thLuaValue object.
     */
    static const thLuaValue ccobjectValue(thRef* ccobjectValue, const std::string& objectTypename);
    
    
    /**
     * Default constructor of thLuaValue.
     * Set the default value for _type(thLuaValueTypeInt) and _ccobjectType(nullptr),and init the _field.
     */
    thLuaValue(void)
    : _type(thLuaValueTypeInt)
    , _ccobjectType(nullptr)
    {
        memset(&_field, 0, sizeof(_field));
    }
    
    /**
     * Copy constructor of Data.
     */
    thLuaValue(const thLuaValue& rhs);
    
    /**
     * Override of operator= .
     */
    thLuaValue& operator=(const thLuaValue& rhs);
    
    /**
     * Destructor.
     */
    ~thLuaValue(void);
    
    /**
     * Get the type of thLuaValue object.
     * 
     * @return the type of thLuaValue object.
     */
    thLuaValueType getType() const {
        return _type;
    }
    
    /**
     * Get the typename of the Ref object.
     *
     * @return the reference of _ccobjectType.
     */
    const std::string& getObjectTypename(void) const {
        return *_ccobjectType;
    }
    
    /**
     * Get the int value of thLuaValue object.
     *
     * @return the int value.
     */
    int intValue(void) const {
        return _field.intValue;
    }
    
    /**
     * Get the float value of thLuaValue object.
     *
     * @return the float value.
     */
    float floatValue(void) const {
        return _field.floatValue;
    }
    
    /**
     * Get the boolean value of thLuaValue object.
     *
     * @return the boolean value.
     */
    bool booleanValue(void) const {
        return _field.booleanValue;
    }
    
    /**
     * Get the boolean value of thLuaValue object.
     *
     * @return the reference about string value.
     */
    const std::string& stringValue(void) const {
        return *_field.stringValue;
    }
    
    /**
     * Get the thLuaValueDict value of thLuaValue object.
     *
     * @return the thLuaValueDict value.
     */
    const thLuaValueDict& dictValue(void) const {
        return *_field.dictValue;
    }
    
    /**
     * Get the thLuaValueArray value of thLuaValue object.
     *
     * @return the thLuaValueArray value.
     */
    const thLuaValueArray& arrayValue(void) const {
        return *_field.arrayValue;
    }
    
    /**
     * Get the Ref object of thLuaValue object.
     *
     * @return the pointer point to a Ref object.
     */
	thRef* ccobjectValue(void) const {
        return _field.ccobjectValue;
    }
    
private:
    thLuaValueField _field;
    thLuaValueType  _type;
    std::string*    _ccobjectType;
    
    void copy(const thLuaValue& rhs);
};

/// @}
#endif // __THSCRIPT_LUAVALUE_H_
