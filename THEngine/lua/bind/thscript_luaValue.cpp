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

#include "thscript_luaValue.h"


const thLuaValue thLuaValue::intValue(const int intValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeInt;
    value._field.intValue = intValue;
    return value;
}

const thLuaValue thLuaValue::floatValue(const float floatValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeFloat;
    value._field.floatValue = floatValue;
    return value;
}

const thLuaValue thLuaValue::booleanValue(const bool booleanValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeBoolean;
    value._field.booleanValue = booleanValue;
    return value;
}

const thLuaValue thLuaValue::stringValue(const char* stringValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeString;
    value._field.stringValue = new std::string(stringValue ? stringValue : "");
    return value;
}

const thLuaValue thLuaValue::stringValue(const std::string& stringValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeString;
    value._field.stringValue = new std::string(stringValue);
    return value;
}

const thLuaValue thLuaValue::dictValue(const thLuaValueDict& dictValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeDict;
    value._field.dictValue = new (std::nothrow) thLuaValueDict(dictValue);
    return value;
}

const thLuaValue thLuaValue::arrayValue(const thLuaValueArray& arrayValue)
{
    thLuaValue value;
    value._type = thLuaValueTypeArray;
    value._field.arrayValue = new (std::nothrow) thLuaValueArray(arrayValue);
    return value;
}

const thLuaValue thLuaValue::ccobjectValue(thRef* ccobjectValue, const char* objectTypename)
{
    thLuaValue value;
    value._type = thLuaValueTypeObject;
    value._field.ccobjectValue = ccobjectValue;
    ccobjectValue->retain();
    value._ccobjectType = new std::string(objectTypename);
    return value;
}

const thLuaValue thLuaValue::ccobjectValue(thRef* ccobjectValue, const std::string& objectTypename)
{
    return thLuaValue::ccobjectValue(ccobjectValue, objectTypename.c_str());
}

thLuaValue::thLuaValue(const thLuaValue& rhs)
{
    copy(rhs);
}

thLuaValue& thLuaValue::operator=(const thLuaValue& rhs)
{
    if (this != &rhs) copy(rhs);
    return *this;
}

thLuaValue::~thLuaValue(void)
{
    if (_type == thLuaValueTypeString)
    {
        delete _field.stringValue;
    }
    else if (_type == thLuaValueTypeDict)
    {
        delete _field.dictValue;
    }
    else if (_type == thLuaValueTypeArray)
    {
        delete _field.arrayValue;
    }
    else if (_type == thLuaValueTypeObject)
    {
        _field.ccobjectValue->release();
        delete _ccobjectType;
    }
}

void thLuaValue::copy(const thLuaValue& rhs)
{
    memcpy(&_field, &rhs._field, sizeof(_field));
    _type = rhs._type;
    if (_type == thLuaValueTypeString)
    {
        _field.stringValue = new std::string(*rhs._field.stringValue);
    }
    else if (_type == thLuaValueTypeDict)
    {
        _field.dictValue = new (std::nothrow) thLuaValueDict(*rhs._field.dictValue);
    }
    else if (_type == thLuaValueTypeArray)
    {
        _field.arrayValue = new (std::nothrow) thLuaValueArray(*rhs._field.arrayValue);
    }
    else if (_type == thLuaValueTypeObject)
    {
        _field.ccobjectValue = rhs._field.ccobjectValue;
        _field.ccobjectValue->retain();
        _ccobjectType = new std::string(*rhs._ccobjectType);
    }
}
