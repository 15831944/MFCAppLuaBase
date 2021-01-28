/****************************************************************************
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
#ifndef __THSCRIPT_LUABAISCCONVERSIONS_H__
#define __THSCRIPT_LUABAISCCONVERSIONS_H__

#include <unordered_map>
#include <string>
#include <map>

#include "../lua/lua.h"
#include "../tolua/tolua++.h"
#include "thscript_dec.h"
#include "thscript_toluafix.h"
#include "thscript_objref.h"

extern std::unordered_map<std::string, std::string>  g_luaType;
extern std::unordered_map<std::string, std::string>  g_typeCast;

// 注册类型
template<class TClass>
extern void registerClassType(const std::string& className)
{
	std::string typeName = typeid(TClass).name();
	g_luaType[typeName] = className;
	g_typeCast[className] = className;
}

// 获取class类型
extern std::string getClassType(const std::string& className);


#if _DEBUG >=1
void luaval_to_native_err(lua_State* L,const char* msg,tolua_Error* err, const char* funcName = "");
#endif

struct thVec2
{
	thVec2() :x(0), y(0) {}
	thVec2(float _x, float _y):x(_x), y(_y){}
	float x;
	float y;
};

struct thVec3
{
	thVec3() :x(0), y(0), z(0) {}
	thVec3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
	float x;
	float y;
	float z;
};

struct thVec4
{
	thVec4() :x(0), y(0), z(0), w(0) {}
	thVec4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
	float x;
	float y;
	float z;
	float w;
};

struct thSize
{
	thSize():width(0),height(0) {}
	thSize(float _w, float _h) :width(_w), height(_h) {}
	float width;
	float height;
};

struct thRect
{
	thRect() {}
	thRect(const thVec2& _v, const thSize& _s)
	{ 
		origin.x = _v.x;
		origin.y = _v.y;
		size.width = _s.width;
		size.height = _s.height;
	}
	thVec2 origin;
	thSize size;
};

struct thColor3B
{
	thColor3B() :r(0), g(0), b(0) {}
	thColor3B(uint8_t _r, uint8_t _g, uint8_t _b) :r(_r), g(_g), b(_b) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct thColor4B
{
	thColor4B() :r(0), g(0), b(0), a(0) {}
	thColor4B(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) :r(_r), g(_g), b(_b), a(_a) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

/**
 * @addtogroup lua
 * @{
 */

/**
 * If the typename of userdata at the given acceptable index of stack is equal to type it return true, otherwise return false.
 * If def != 0, lo could greater than the top index of stack, return value is true.
 * If the value of the given index is nil, return value also is true.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param type the typename used to judge.
 * @param def whether has default value.
 * @return Return true if the typename of userdata at the given acceptable index of stack is equal to type, otherwise return false.
 */
extern bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def);
// to native

/**
 * @name luaval_to_native
 * The following function are all used to convert the Lua values at the given acceptable index to the corresponding c++ values.
 * If the Lua values can be converted the return value is true, otherwise return false.
 * If it happens error during the conversion process, it outputs the error msg in the console to provide information about the name of calling function, the typename of value at the given acceptable index, and so on.
 * @{
 **/


/**
 * Get a unsigned long value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_ulong(lua_State* L,int lo, unsigned long* outValue, const char* funcName="");

/**
 * Get a unsigned short value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the unsigned short value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue, const char* funcName = "");

/**
 * Get a int value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the int value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName = "");

/**
 * Get a unsigned int value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the unsigned int value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName = "");

/**
 * Get a uint16_t value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the uint16_t value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue, const char* funcName = "");

/**
 * Get a boolean value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack has type boolean it returns true, otherwise returns false.
 * Any Lua value is different from false and nil, the value of conversion is true, otherwise the value is false.
 * If the lo is non-valid index, the value of conversion also is false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the boolean value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_boolean(lua_State* L,int lo,bool* outValue, const char* funcName = "");

/**
 * Get a double value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the double value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_number(lua_State* L,int lo,double* outValue, const char* funcName = "");

/**
 * Get a long long value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the long long value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_long_long(lua_State* L,int lo,long long* outValue, const char* funcName = "");

/**
 * Get a std::string value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a string or a number convertible to a string it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store std::string value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a string or a number convertible to a string, otherwise return false.
 */
extern bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue, const char* funcName = "");

/**
 * Get a long value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the long value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_long(lua_State* L,int lo, long* outValue, const char* funcName = "");

/**
 * Get a ssize_t value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a number or a string convertible to a number it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to store the ssize_t value converted from the Lua value.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a number or a string convertible to a number, otherwise return false.
 */
extern bool luaval_to_ssize(lua_State* L,int lo, size_t* outValue, const char* funcName = "");

/**
 * Get a Size object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `width` and `height` key and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Size object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_size(lua_State* L,int lo, thSize* outValue, const char* funcName = "");

/**
 * Get a Rect object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `x`,`y`,`width` and `height` keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Rect object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_rect(lua_State* L,int lo, thRect* outValue, const char* funcName = "");

/**
 * Get a Color3B object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `r`,`g` and `b` keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Color3B object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_color3b(lua_State* L,int lo, thColor3B* outValue, const char* funcName = "");

/**
 * Get a Color4B object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `r`,`g`, `b` and 'a' keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Color4B object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_color4b(lua_State* L,int lo, thColor4B* outValue, const char* funcName = "");

/**
 * Get a array of Vec2 object from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param points the pointer to a array of Vec2 object which stores the values from the Lua table.
 * @param numPoints a int pointer to store the size of a Vec2 object array.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_array_of_vec2(lua_State* L,int lo, thVec2 **points, int *numPoints, const char* funcName = "");


/**
 * Get a Vec2 object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `x`,and 'y' keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Vec2 object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_vec2(lua_State* L,int lo, thVec2* outValue, const char* funcName = "");

/**
 * Get a Vec3 object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `x`, 'y' and `z` keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Vec3 object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_vec3(lua_State* L,int lo, thVec3* outValue, const char* funcName = "");

/**
 * Get a Vec4 object value from the given acceptable index of stack.
 * If the value at the given acceptable index of stack is a table it returns true, otherwise returns false.
 * If the table has the `x`, 'y', `z` and `w` keys and the corresponding values are not nil, this function would assign the values to the corresponding members of outValue. Otherwise, the value of members of outValue would be 0.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param outValue the pointer to a Vec4 object which stores the values from the Lua table.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_vec4(lua_State* L,int lo, thVec4* outValue, const char* funcName = "");

/**
 * Get a Vec2 object value from the given acceptable index of stack.
 * At current, the Point is typedef of Vec2.
 * @see luaval_to_vec2
 */
static inline bool luaval_to_point(lua_State* L,int lo, thVec2* outValue, const char* funcName = "")
{
    return luaval_to_vec2(L, lo, outValue);
}

/**
 * Get a cocos2d::Vector of type T objects by the argc numbers of Lua values in the stack.
 *
 * @param L the current lua_State.
 * @param argc the numbers of Lua values in the stack.
 * @param ret a cocos2d::Vector of type T objects.
 * @return Return false if argc equal to 0 , L equal to nullptr or the Lua userdata at the index isn't `cc.Ref` type, otherwise return true.
 */
template <class T>
bool luavals_variadic_to_ccvector( lua_State* L, int argc, std::vector<T>* ret)
{
    if (nullptr == L || argc == 0 )
        return false;

    bool ok = true;

    for (int i = 0; i < argc; i++)
    {
        if (lua_isuserdata(L, i + 2))
        {
            tolua_Error err;

            if (!tolua_isusertype(L, i + 2, "cc.Ref", 0, &err))
            {
                ok = false;
                break;
            }
            T obj = static_cast<T>(tolua_tousertype(L, i + 2, nullptr));
            ret->push_back(obj);
        }
    }

    return ok;
}


/**
 * Get a cocos2d::Vector of type T objects from a Lua table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a cocos2d::Vector of type T objects.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
template <class T>
bool luaval_to_ccvector(lua_State* L, int lo , std::vector<T>* ret, const char* funcName = "")
{
    if (nullptr == L || nullptr == ret)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
        ok = false;

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L, lo);

            if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }


            T cobj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
            if (NULL != cobj)
                ret->push_back(cobj);

            lua_pop(L, 1);
        }
    }

    return ok;
}

/**
 * Get a pointer points to a std::vector<std::string> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<std::string>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
bool   luaval_to_std_vector_string(lua_State* L, int lo, std::vector<std::string>* ret, const char* funcName = "");

/**
 * Get a pointer points to a std::vector<std::int> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<int>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
bool luaval_to_std_vector_int(lua_State* L, int lo, std::vector<int>* ret, const char* funcName = "");


/**
 * Get a pointer points to a cocos2d::Map whose key/value pair is string value and T object from a Lua hash-map table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a cocos2d::Map whose key/value pair is string value and T object.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
template <class T>
bool luaval_to_ccmap_string_key(lua_State* L, int lo, std::map<std::string, T>* ret, const char* funcName = "")
{
    if(nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
		luaval_to_native_err(L, "#ferror:", &tolua_err);
#endif
        ok = false;
    }

    if (ok)
    {
        std::string stringKey = "";
        lua_pushnil(L);                                             /* first key L: lotable ..... nil */
        while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
        {
            if (!lua_isstring(L, -2))
            {
                lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
                continue;
            }

            if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }

            luaval_to_std_string(L, -2, &stringKey);
            T obj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
            if (nullptr != obj)
                ret->emplace(stringKey, obj);

            lua_pop(L, 1);                                          /* L: lotable ..... key */
        }
    }

    return ok;
}

/**
 * Get a Type T object from the given acceptable index of stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param type a string pointer points to the type name.
 * @param ret the pointer points to a Type T object.
 * @return Return true if the type of Lua value at the index is a Lua userdata, otherwise return false.
 */
template <class T>
bool luaval_to_object(lua_State* L, int lo, const char* type, T** ret, const char* funcName = "")
{
    if(nullptr == L || lua_gettop(L) < lo)
        return false;

    if (!luaval_is_usertype(L, lo, type, 0))
        return false;

    *ret = static_cast<T*>(tolua_tousertype(L, lo, 0));

    if (nullptr == *ret)
		__thDF_LOG("Warning: %s argument %d is invalid native object(nullptr)", funcName, lo);

    return true;
}

/**
 * Get a pointer points to a std::vector<float> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<float>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_std_vector_float(lua_State* L, int lo, std::vector<float>* ret, const char* funcName = "");

/**
 * Get a pointer points to a std::vector<unsigned shortt> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<unsigned short>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_std_vector_ushort(lua_State* L, int lo, std::vector<unsigned short>* ret, const char* funcName = "");

/**
 * Get a pointer points to a std::vector<cocos2d::Vec2> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<cocos2d::Vec2>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_std_vector_vec2(lua_State* L, int lo, std::vector<thVec2>* ret, const char* funcName = "");

/**
 * Get a pointer points to a std::vector<cocos2d::Vec3> from a Lua array table in the stack.
 *
 * @param L the current lua_State.
 * @param lo the given acceptable index of stack.
 * @param ret a pointer points to a std::vector<cocos2d::Vec3>.
 * @param funcName the name of calling function, it is used for error output in the debug model.
 * @return Return true if the value at the given acceptable index of stack is a table, otherwise return false.
 */
extern bool luaval_to_std_vector_vec3(lua_State* L, int lo, std::vector<thVec3>* ret, const char* funcName = "");

extern bool luaval_to_std_map_string_string(lua_State* L, int lo, std::map<std::string, std::string>* ret, const char* funcName);

/**@}**/

// from native

/**
 * @name native_to_luaval
 * The following function are all used to convert native c++ values to the corresponding Lua values, then push it into the Lua stack.
 *
 * @{
 **/

/**
 * Push a table converted from a cocos2d::Vec2 object into the Lua stack.
 * The format of table as follows: {x=numberValue1, y=numberValue2}
 *
 * @param L the current lua_State.
 * @param vec2  a cocos2d::Vec2 object.
 */
extern void vec2_to_luaval(lua_State* L,const thVec2& vec2);

/**
 * Push a table converted from a cocos2d::Vec3 object into the Lua stack.
 * The format of table as follows: {x=numberValue1, y=numberValue2, z=numberValue3}
 *
 * @param L the current lua_State.
 * @param vec3  a cocos2d::Vec3 object.
 */
extern void vec3_to_luaval(lua_State* L,const thVec3& vec3);

/**
 * Push a table converted from a cocos2d::Vec4 object into the Lua stack.
 * The format of table as follows: {x=numberValue1, y=numberValue2, z=numberValue3, w=numberValue4}
 *
 * @param L the current lua_State.
 * @param vec4  a cocos2d::Vec4 object.
 */
extern void vec4_to_luaval(lua_State* L,const thVec4& vec4);

/**
 * Push a table converted from a cocos2d::Vec2 array into the Lua stack.
 * The table size is count.
 * The format of table as follows: {vec2_table1,vec2_table2,...,vec2_tableCount}.
 *
 * @param L the current lua_State.
 * @param points a pointer points to a cocos2d::Vec2 array.
 * @param count the number of cocos2d::Vec2 object should be converted to a Lua table and push into the Lua stack.
 */
extern void vec2_array_to_luaval(lua_State* L,const thVec2* points, int count);

/**
 * Push a table converted from a cocos2d::Size object into the Lua stack.
 * The format of table as follows: {width=numberValue1, height=numberValue2}
 *
 * @param L the current lua_State.
 * @param sz  a cocos2d::Size object.
 */
extern void size_to_luaval(lua_State* L,const thSize& sz);

/**
 * Push a table converted from a cocos2d::Rect object into the Lua stack.
 * The format of table as follows: {x=numberValue1, y=numberValue2, width=numberValue3, height=numberValue4}
 *
 * @param L the current lua_State.
 * @param rt  a cocos2d::Rect object.
 */
extern void rect_to_luaval(lua_State* L,const thRect& rt);

/**
 * Push a table converted from a cocos2d::Color3B object into the Lua stack.
 * The format of table as follows: {r=numberValue1, g=numberValue2, b=numberValue3}
 *
 * @param L the current lua_State.
 * @param cc  a cocos2d::Color3B object.
 */
extern void color3b_to_luaval(lua_State* L,const thColor3B& cc);

/**
 * Push a table converted from a cocos2d::Color4B object into the Lua stack.
 * The format of table as follows: {r=numberValue1, g=numberValue2, b=numberValue3, a=numberValue4}
 *
 * @param L the current lua_State.
 * @param cc a cocos2d::Color4B object.
 */
extern void color4b_to_luaval(lua_State* L,const thColor4B& cc);

/**
 * Push a table converted from a cocos2d::Vector object into the Lua stack.
 * The format of table as follows: {userdata1, userdata2, ..., userdataVectorSize}
 * The object in the cocos2d::Vector which would be pushed into the table should be Ref type.
 *
 * @param L the current lua_State.
 * @param inValue a cocos2d::Vector object.
 */
template <class T>
void ccvector_to_luaval(lua_State* L,const std::vector<T>& inValue)
{
    lua_newtable(L);

    if (nullptr == L)
        return;

    int indexTable = 1;
    for (const auto& obj : inValue)
    {
        if (nullptr == obj)
            continue;


        if (nullptr != dynamic_cast<thRef *>(obj))
        {
            std::string typeName = typeid(*obj).name();
            auto iter = g_luaType.find(typeName);
            if (g_luaType.end() != iter)
            {
                lua_pushnumber(L, (lua_Number)indexTable);
                int ID = (obj) ? (int)obj->_ID : -1;
                int* luaID = (obj) ? &obj->_luaID : NULL;
                toluafix_pushusertype_ccobject(L, ID, luaID, (void*)obj,iter->second.c_str());
                lua_rawset(L, -3);
                ++indexTable;
            }
        }
    }
}

/**
 * Push a table converted from a cocos2d::Map object into the Lua stack.
 * The format of table as follows: {name1=userdata1, name2=userdata2, ..., nameMapSize=userdataMapSize}
 * The object in the cocos2d::Map which would be pushed into the table should be Ref type.
 *
 * @param L the current lua_State.
 * @param v a cocos2d::Map object.
 */
template <class T>
void ccmap_string_key_to_luaval(lua_State* L, const std::map<std::string, T>& v)
{
    lua_newtable(L);

    if(nullptr == L)
        return;

    for (auto iter = v.begin(); iter != v.end(); ++iter)
    {
        std::string key = iter->first;
        T obj = iter->second;
        if (nullptr != dynamic_cast<thRef *>(obj))
        {
            std::string name = typeid(*obj).name();
            auto typeIter = g_luaType.find(name);
            if (g_luaType.end() != typeIter)
            {
                lua_pushstring(L, key.c_str());
                int ID = (obj) ? (int)obj->_ID : -1;
                int* luaID = (obj) ? &obj->_luaID : NULL;
                toluafix_pushusertype_ccobject(L, ID, luaID, (void*)obj,typeIter->second.c_str());
                lua_rawset(L, -3);
            }
        }
    }
}

/**@}**/

/**
 * Get the real typename for the specified typename.
 * Because all override functions wouldn't be bound,so we must use `typeid` to get the real class name.
 *
 * @param ret the pointer points to a type T object.
 * @param type the string pointer points to specified typename.
 * @return return the pointer points to the real typename, or nullptr.
 */
template <class T>
const char* getLuaTypeName(T* ret,const char* type)
{
    if (nullptr != ret)
    {
        std::string hashName = typeid(*ret).name();
        auto iter =  g_luaType.find(hashName);
        if(g_luaType.end() != iter)
        {
            return iter->second.c_str();
        }
        else
        {
            return type;
        }
    }

    return nullptr;
}

/**
 * Push the native object by userdata format into the Lua stack by typename.
 *
 * @param L the current lua_State.
 * @param type the string pointer points to the typename.
 * @param ret the native object pointer.
 */
template <class T>
void object_to_luaval(lua_State* L,const char* type, T* ret)
{
    if(nullptr != ret)
    {
        if (std::is_base_of<thRef, T>::value)
        {
            // use c style cast, T may not polymorphic
            thRef* dynObject = (thRef*)(ret);
            int ID = (int)(dynObject->_ID) ;
            int* luaID = &(dynObject->_luaID);
            toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,type);
        }
        else
        {
            tolua_pushusertype(L,(void*)ret,getLuaTypeName(ret, type));
        }
    }
    else
    {
        lua_pushnil(L);
    }
}

/**
 * Push a Lua array table converted from a std::vector<std::string> into the Lua stack.
 * The format of table as follows: {stringValue1, stringValue2, ..., stringVectorSize}
 *
 * @param L the current lua_State.
 * @param inValue a std::vector<std::string> value.
 */
void ccvector_std_string_to_luaval(lua_State* L, const std::vector<std::string>& inValue);

/**
 * Push a Lua array table converted from a std::vector<int> into the Lua stack.
 * The format of table as follows: {numberValue1, numberValue2, ..., numberVectorSize}
 *
 * @param L the current lua_State.
 * @param inValue a std::vector<int> value.
 */
void ccvector_int_to_luaval(lua_State* L, const std::vector<int>& inValue);

/**
 * Push a Lua array table converted from a std::vector<float> into the Lua stack.
 * The format of table as follows: {numberValue1, numberValue2, ..., numberVectorSize}
 *
 * @param L the current lua_State.
 * @param inValue a std::vector<float> value.
 */
void ccvector_float_to_luaval(lua_State* L, const std::vector<float>& inValue);

/**
 * Push a Lua array table converted from a std::vector<unsigned short> into the Lua stack.
 * The format of table as follows: {numberValue1, numberValue2, ..., numberVectorSize}
 *
 * @param L the current lua_State.
 * @param inValue a std::vector<float> value.
 */
void ccvector_ushort_to_luaval(lua_State* L, const std::vector<unsigned short>& inValue);

/**
 * Push a Lua array table converted from a std::vector<cocos2d::Vec3> into the Lua stack.
 * The format of table as follows: {vec3Value1, vec3Value2, ..., vec3ValueSize}
 *
 * @param L the current lua_State.
 * @param inValue a std::vector<cocos2d::Vec3> value.
 */
void std_vector_vec3_to_luaval(lua_State* L, const std::vector<thVec3>& inValue);

/**
 * Push a Lua dict table converted from a std::map<std::string, std::string> into the Lua stack.
 *
 * @param L the current lua_State.
 * @param inValue a std::map<std::string, std::string> value.
 */
void std_map_string_string_to_luaval(lua_State* L, const std::map<std::string, std::string>& inValue);

// Follow 2 function is added for Cocos Studio to make lua lib can be compile as dynamic library
extern bool luaval_to_node(lua_State* L, int lo, const char* type, thRef** node);
extern void node_to_luaval(lua_State* L, const char* type, thRef* node);

void std_vector_vec2_to_luaval(lua_State* L, const std::vector<thVec2>& inValue);

// end group
/// @}
#endif //__THSCRIPT_LUABAISCCONVERSIONS_H__
