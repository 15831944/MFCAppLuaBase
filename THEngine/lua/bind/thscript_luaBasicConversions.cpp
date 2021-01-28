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

#include "thscript_luaBasicConversions.h"
#include "thscript_toluafix.h"

std::unordered_map<std::string, std::string>  g_luaType;
std::unordered_map<std::string, std::string>  g_typeCast;

#if _DEBUG >=1

extern std::string getClassType(const std::string& className)
{
	std::string name = className;
	auto ist = int(name.find("*"));
	if (ist != -1) name.erase(name.begin()+ist-1, name.end());

	auto iter = g_luaType.find(name);
	if (g_luaType.end() != iter)
		name = iter->second.c_str();
	else
	{
		name.erase(name.begin(), name.begin() + strlen("class "));
	}
	return name;
}

void luaval_to_native_err(lua_State* L,const char* msg,tolua_Error* err, const char* funcName)
{
    if (NULL == L || NULL == err || NULL == msg || 0 == strlen(msg))
        return;

    if (msg[0] == '#')
    {
        const char* expected = err->type;
        const char* provided = tolua_typename(L,err->index);
        if (msg[1]=='f')
        {
            int narg = err->index;
            if (err->array)
                __thDF_LOG("%s\n     %s argument #%d is array of '%s'; array of '%s' expected.\n",msg+2,funcName,narg,provided,expected);
            else
                __thDF_LOG("%s\n     %s argument #%d is '%s'; '%s' expected.\n",msg+2,funcName,narg,provided,expected);
        }
        else if (msg[1]=='v')
        {
            if (err->array)
                __thDF_LOG("%s\n     %s value is array of '%s'; array of '%s' expected.\n",funcName,msg+2,provided,expected);
            else
                __thDF_LOG("%s\n     %s value is '%s'; '%s' expected.\n",msg+2,funcName,provided,expected);
        }
    }
}
#endif

extern int lua_isusertype (lua_State* L, int lo, const char* type);

bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def)
{
    if (def && lua_gettop(L)<std::abs(lo))
        return true;

    if (lua_isnil(L,lo) || lua_isusertype(L,lo,type))
        return true;

    return false;
}

bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue, const char* funcName)
{
    if (nullptr == L || nullptr == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (unsigned short)tolua_tonumber(L, lo, 0);
    }

    return ok;
}


bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        /**
         When we want to convert the number value from the Lua to int, we would call lua_tonumber to implement.It would
         experience two phase conversion: int -> double, double->int.But,for the 0x80000000 which the min value of int, the
         int cast may return an undefined result,like 0x7fffffff.So we must use the (int)(unsigned int)lua_tonumber() to get
         predictable results for 0x80000000.In this place,we didn't use lua_tointeger, because it may produce different results
         depending on the compiler,e.g:for iPhone4s,it also get wrong value for 0x80000000.
         */
        unsigned int estimateValue = (unsigned int)lua_tonumber(L, lo);
        if (estimateValue == std::numeric_limits<int>::min())
        {
            *outValue = (int)estimateValue;
        }
        else
        {
            *outValue = (int)lua_tonumber(L, lo);
        }
    }

    return ok;
}

bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (unsigned int)tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (unsigned char)tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_boolean(lua_State* L,int lo,bool* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isboolean(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (bool)tolua_toboolean(L, lo, 0);
    }

    return ok;
}

bool luaval_to_number(lua_State* L,int lo,double* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_long_long(lua_State* L,int lo,long long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (long long)tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_iscppstring(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t size;
        auto rawString = lua_tolstring(L,lo,&size);
        *outValue = std::string(rawString, size);
    }

    return ok;
}

bool luaval_to_vec2(lua_State* L,int lo, thVec2* outValue, const char* funcName)
{
    if (nullptr == L || nullptr == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }


    if (ok)
    {
        lua_pushstring(L, "x");
        lua_gettable(L, lo);
        outValue->x = lua_isnil(L, -1) ? 0.0f : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "y");
        lua_gettable(L, lo);
        outValue->y = lua_isnil(L, -1) ? 0.0f : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    return ok;
}

bool luaval_to_vec3(lua_State* L,int lo, thVec3* outValue, const char* funcName)
{
    if (nullptr == L || nullptr == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }


    if (ok)
    {
        lua_pushstring(L, "x");
        lua_gettable(L, lo);
        outValue->x = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "y");
        lua_gettable(L, lo);
        outValue->y = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "z");
        lua_gettable(L, lo);
        outValue->z = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    return ok;
}

bool luaval_to_vec4(lua_State* L,int lo, thVec4* outValue, const char* funcName)
{
    if (nullptr == L || nullptr == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }


    if (ok)
    {
        lua_pushstring(L, "x");
        lua_gettable(L, lo);
        outValue->x = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "y");
        lua_gettable(L, lo);
        outValue->y = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "z");
        lua_gettable(L, lo);
        outValue->z = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "w");
        lua_gettable(L, lo);
        outValue->w = lua_isnil(L, -1) ? 0 : (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    return ok;
}

bool luaval_to_ssize(lua_State* L,int lo, size_t* outValue, const char* funcName)
{
    return luaval_to_long(L, lo, reinterpret_cast<long*>(outValue));
}

bool luaval_to_long(lua_State* L,int lo, long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (long)tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_ulong(lua_State* L,int lo, unsigned long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        *outValue = (unsigned long)tolua_tonumber(L, lo, 0);
    }

    return ok;
}

bool luaval_to_size(lua_State* L,int lo,thSize* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        lua_pushstring(L, "width");  /* L: paramStack key */
        lua_gettable(L,lo);/* L: paramStack paramStack[lo][key] */
        outValue->width = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);/* L: paramStack*/

        lua_pushstring(L, "height");
        lua_gettable(L,lo);
        outValue->height = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);
    }

    return ok;
}

bool luaval_to_rect(lua_State* L,int lo, thRect* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        lua_pushstring(L, "x");
        lua_gettable(L,lo);
        outValue->origin.x = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "y");
        lua_gettable(L,lo);
        outValue->origin.y = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "width");
        lua_gettable(L,lo);
        outValue->size.width = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "height");
        lua_gettable(L,lo);
        outValue->size.height = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
        lua_pop(L,1);
    }

    return ok;
}

bool luaval_to_color4b(lua_State* L,int lo, thColor4B* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if(ok)
    {
        lua_pushstring(L, "r");
        lua_gettable(L,lo);
        outValue->r = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "g");
        lua_gettable(L,lo);
        outValue->g = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "b");
        lua_gettable(L,lo);
        outValue->b = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "a");
        lua_gettable(L,lo);
        outValue->a = lua_isnil(L,-1) ? 255 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);
    }

    return ok;
}

bool luaval_to_color3b(lua_State* L,int lo,thColor3B* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;

    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        lua_pushstring(L, "r");
        lua_gettable(L,lo);
        outValue->r = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "g");
        lua_gettable(L,lo);
        outValue->g = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);

        lua_pushstring(L, "b");
        lua_gettable(L,lo);
        outValue->b = lua_isnil(L,-1) ? 0 : (uint8_t)lua_tonumber(L,-1);
        lua_pop(L,1);
    }

    return ok;
}

bool luaval_to_array_of_vec2(lua_State* L,int lo,thVec2 **points, int *numPoints, const char* funcName)
{
    if (NULL == L)
        return false;

    bool ok = true;

    tolua_Error tolua_err;

    if (!tolua_istable(L, lo, 0, &tolua_err) )
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        if (len > 0)
        {
            thVec2* array = (thVec2*) new thVec2[len];
            if (NULL == array)
                return false;
            for (size_t i = 0; i < len; ++i)
            {
                lua_pushnumber(L,i + 1);
                lua_gettable(L,lo);
                if (!tolua_istable(L,-1, 0, &tolua_err))
                {
#if _DEBUG >=1
                    luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
                    lua_pop(L, 1);
                    SAFE_DELETE_ARRAY(array);
                    return false;
                }
                ok &= luaval_to_vec2(L, lua_gettop(L), &array[i]);
                if (!ok)
                {
                    lua_pop(L, 1);
                    SAFE_DELETE_ARRAY(array);
                    return false;
                }
                lua_pop(L, 1);
            }

            *numPoints = (int)len;
            *points    = array;
        }
    }
    return ok;
}

bool luaval_to_std_vector_string(lua_State* L, int lo, std::vector<std::string>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        std::string value = "";
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isstring(L, -1))
            {
                ok = luaval_to_std_string(L, -1, &value);
                if(ok)
                    ret->push_back(value);
            }
            else
            {
                __thDF_assert(false, "string type is needed");
            }

            lua_pop(L, 1);
        }
    }

    return ok;
}

bool luaval_to_std_vector_int(lua_State* L, int lo, std::vector<int>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((int)tolua_tonumber(L, -1, 0));
            }
            else
            {
                __thDF_assert(false, "int type is needed");
            }

            lua_pop(L, 1);
        }
    }

    return ok;
}

bool luaval_to_std_vector_float(lua_State* L, int lo, std::vector<float>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;

    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((float)tolua_tonumber(L, -1, 0));
            }
            else
            {
                __thDF_assert(false, "float type is needed");
            }

            lua_pop(L, 1);
        }
    }

    return ok;
}


bool luaval_to_std_vector_ushort(lua_State* L, int lo, std::vector<unsigned short>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;

    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((unsigned short)tolua_tonumber(L, -1, 0));
            }
            else
            {
                __thDF_assert(false, "unsigned short type is needed");
            }

            lua_pop(L, 1);
        }
    }

    return ok;
}

bool luaval_to_std_vector_vec2(lua_State* L, int lo, std::vector<thVec2>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;

    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        thVec2 value;
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if (lua_istable(L, lua_gettop(L)))
            {
                ok &= luaval_to_vec2(L, lua_gettop(L), &value);
                if (ok)
                {
                    ret->push_back(value);
                }
            }
            else
            {
                __thDF_assert(false, "vec2 type is needed");
            }
            lua_pop(L, 1);
        }
    }

    return ok;
}

bool luaval_to_std_vector_vec3(lua_State* L, int lo, std::vector<thVec3>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;

    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        thVec3 value;
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if (lua_istable(L, lua_gettop(L)))
            {
                ok &= luaval_to_vec3(L, lua_gettop(L), &value);
                if (ok)
                {
                    ret->push_back(value);
                }
            }
            else
            {
                __thDF_assert(false, "vec3 type is needed");
            }
            lua_pop(L, 1);
        }
    }

    return ok;
}

void vec2_array_to_luaval(lua_State* L,const thVec2* points, int count)
{
    if (NULL  == L)
        return;
    lua_newtable(L);
    for (int i = 1; i <= count; ++i)
    {
        lua_pushnumber(L, i);
        vec2_to_luaval(L, points[i-1]);
        lua_rawset(L, -3);
    }
}

void vec2_to_luaval(lua_State* L,const thVec2& vec2)
{
    if (NULL  == L)
        return;
    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "x");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec2.x);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "y");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec2.y);               /* L: table key value*/
    lua_rawset(L, -3);

}

void vec3_to_luaval(lua_State* L,const thVec3& vec3)
{
    if (NULL  == L)
        return;

    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "x");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec3.x);             /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "y");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec3.y);             /* L: table key value*/
    lua_rawset(L, -3);
    lua_pushstring(L, "z");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec3.z);             /* L: table key value*/
    lua_rawset(L, -3);
}

void vec4_to_luaval(lua_State* L,const thVec4& vec4)
{
    if (NULL  == L)
        return;

    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "x");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec4.x);             /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "y");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec4.y);             /* L: table key value*/
    lua_rawset(L, -3);
    lua_pushstring(L, "z");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec4.z);             /* L: table key value*/
    lua_rawset(L, -3);
    lua_pushstring(L, "w");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) vec4.w);             /* L: table key value*/
    lua_rawset(L, -3);
}

void size_to_luaval(lua_State* L,const thSize& sz)
{
    if (NULL  == L)
        return;
    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "width");                         /* L: table key */
    lua_pushnumber(L, (lua_Number) sz.width);           /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "height");                        /* L: table key */
    lua_pushnumber(L, (lua_Number) sz.height);          /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
}

void rect_to_luaval(lua_State* L,const thRect& rt)
{
    if (NULL  == L)
        return;
    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "x");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) rt.origin.x);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "y");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) rt.origin.y);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "width");                         /* L: table key */
    lua_pushnumber(L, (lua_Number) rt.size.width);           /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "height");                        /* L: table key */
    lua_pushnumber(L, (lua_Number) rt.size.height);          /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
}

void color4b_to_luaval(lua_State* L,const thColor4B& cc)
{
    if (NULL  == L)
        return;
    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "r");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.r);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "g");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.g);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "b");                         /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.b);           /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "a");                        /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.a);          /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
}

void color3b_to_luaval(lua_State* L,const thColor3B& cc)
{
    if (NULL  == L)
        return;
    lua_newtable(L);                                    /* L: table */
    lua_pushstring(L, "r");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.r);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "g");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.g);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
    lua_pushstring(L, "b");                             /* L: table key */
    lua_pushnumber(L, (lua_Number) cc.b);               /* L: table key value*/
    lua_rawset(L, -3);                                  /* table[key] = value, L: table */
}

void ccvector_std_string_to_luaval(lua_State* L, const std::vector<std::string>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    int index = 1;

    for (const std::string& value : inValue)
    {
        lua_pushnumber(L, (lua_Number)index);
        lua_pushstring(L, value.c_str());
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_int_to_luaval(lua_State* L, const std::vector<int>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    int index = 1;
    for (const int value : inValue)
    {
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_float_to_luaval(lua_State* L, const std::vector<float>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    int index = 1;
    for (const float value : inValue)
    {
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_ushort_to_luaval(lua_State* L, const std::vector<unsigned short>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    int index = 1;
    for (const unsigned short value : inValue)
    {
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

void std_vector_vec3_to_luaval(lua_State* L, const std::vector<thVec3>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    int index = 1;
    for (const thVec3& value : inValue)
    {
        lua_pushnumber(L, (lua_Number)index);
        vec3_to_luaval(L, value);
        lua_rawset(L, -3);
        ++index;
    }
}

void std_map_string_string_to_luaval(lua_State* L, const std::map<std::string, std::string>& inValue)
{
    if (nullptr == L)
        return;

    lua_newtable(L);

    for (auto iter = inValue.begin(); iter != inValue.end(); ++iter)
    {
        lua_pushstring(L, iter->first.c_str());
        lua_pushstring(L, iter->second.c_str());
        lua_rawset(L, -3);
    }
}

bool luaval_to_std_map_string_string(lua_State* L, int lo, std::map<std::string, std::string>* ret, const char* funcName)
{
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if _DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }

    if (!ok)
        return ok;

    lua_pushnil(L);
    std::string key;
    std::string value;
    while (lua_next(L, lo) != 0)
    {
        if (lua_isstring(L, -2) && lua_isstring(L, -1))
        {
            if (luaval_to_std_string(L, -2, &key) && luaval_to_std_string(L, -1, &value))
            {
                (*ret)[key] = value;
            }
        }
        else
        {
            __thDF_assert(false, "string type is needed");
        }

        lua_pop(L, 1);
    }

    return ok;
}

bool luaval_to_node(lua_State* L, int lo, const char* type, thRef** node)
{
    return luaval_to_object<thRef>(L, lo, type, node);
}

void node_to_luaval(lua_State* L, const char* type, thRef* node)
{
    object_to_luaval<thRef>(L, type, node);
}

void std_vector_vec2_to_luaval(lua_State* L, const std::vector<thVec2>& inValue)
{
	if (nullptr == L)
		return;

	lua_newtable(L);

	int index = 1;
	for (const thVec2& value : inValue)
	{
		lua_pushnumber(L, (lua_Number)index);
		vec2_to_luaval(L, value);
		lua_rawset(L, -3);
		++index;
	}
}
