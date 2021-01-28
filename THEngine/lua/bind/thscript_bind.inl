/// ===================================================================================
/// inline
/// ===================================================================================

/// ===================================================================================
//// 辅助 转换值到thLValue
//template<typename P>
//inline thLValue __getConvValue(P val)
//{
//	thLValue value;
//	auto& tp = typeid(P);
//	std::string name = tp.name();
//	auto fd = int(name.find("class"));
//	if (fd != -1)
//	{
//		// 类
//		if (tp == typeid(std::string))
//		{
//			value = *(std::string*)&val;
//		}
//		else if (tp == typeid(std::string*))
//		{
//			auto ptr = *(std::string**)&val;
//			if (!ptr)
//			{
//				throw "__getConvValue: std::string* ptr is nil";
//			}
//			value = *ptr;
//		}
//		if (tp == typeid(thLValue))
//		{
//			value = *(thLValue*)&val;
//		}
//		else if (tp == typeid(thLValue*))
//		{
//			auto ptr = *(thLValue**)&value;
//			if (!ptr)
//			{
//				throw "__getConvValue: thLValue* ptr is nil";
//			}
//			value = *ptr;
//		}
//		else if (tp == typeid(thLuaFunction*))
//		{
//			auto ptr = *(thLuaFunction**)&value;
//			if (!ptr)
//			{
//				throw "__getConvValue: thLuaFunction* ptr is nil";
//			}
//			value = *ptr;
//		}
//		else
//		{
//			value = val;
//		}
//	}
//	else if (tp == typeid(bool))
//	{
//		value = *(bool*)&val;
//	}
//	else if (tp == typeid(unsigned char))
//	{
//		value = *(unsigned char*)&val;
//	}
//	else if (tp == typeid(int16_t))
//	{
//		value = *(int*)&val;
//	}
//	else if (tp == typeid(int32_t))
//	{
//		value = *(int*)&val;
//	}
//	else if (tp == typeid(uint32_t))
//	{
//		value = *(uint32_t*)&val;
//	}
//	else if (tp == typeid(float))
//	{
//		value = *(float*)&val;
//	}
//	else if (tp == typeid(double))
//	{
//		value = *(double*)&val;
//	}
//	return value;
//}

///// ===================================================================================
//// 辅助 返回var到lua 根据R类型
//template<typename R>
//inline int __retArgLuaValueByR(lua_State* luas, thLValue& value)
//{
//	auto& tp = typeid(R);
//	std::string name = tp.name();
//	auto fd = int(name.find("class"));
//	if (fd != -1)
//	{
//		// 类
//		if (tp == typeid(std::string))
//		{
//			std::string& str = *value.asStringPtr();
//			tolua_pushcppstring(luas, str);
//		}
//		else if (tp == typeid(thLValue))
//		{
//			return __retArgLuaValue(luas, value);
//		}
//		else if (tp == typeid(thLValue*))
//		{
//			return __retArgLuaValue(luas, value);
//		}
//		else if (tp == typeid(thLuaFunction*))
//		{
//			lua_settop(luas, 1);
//		}
//		else
//		{
//			auto className = getClassType(name);
//			object_to_luaval<thRef>(luas, className.c_str(), (thRef*)value.asObject());
//		}
//	}
//	else if (tp == typeid(bool))
//	{
//		tolua_pushboolean(luas, value.asBool());
//	}
//	else if (tp == typeid(int16_t))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asInt());
//	}
//	else if (tp == typeid(int32_t))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asInt());
//	}
//	else if (tp == typeid(uint32_t))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asUnsignedInt());
//	}
//	else if (tp == typeid(float))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asFloat());
//	}
//	else if (tp == typeid(double))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asDouble());
//	}
//	else if (tp == typeid(long long))
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asDouble());
//	}
//	else
//	{
//		lua_settop(luas, 1);
//	}
//
//	return 1;
//}

///// ===================================================================================
//// 辅助 返回var到lua
//inline int __retArgLuaValue(lua_State* luas, thLValue& value)
//{
//	auto vtp = value.getType();
//	if (vtp == thLValue::Type::OBJECT)
//	{
//		auto object = value.asObject();
//		auto& tp = typeid(object);
//		std::string name = tp.name();
//		auto className = getClassType(name);
//		object_to_luaval<thRef>(luas, className.c_str(), (thRef*)object);
//	}
//	else if (vtp == thLValue::Type::STRING)
//	{
//		std::string& str = *value.asStringPtr();
//		tolua_pushcppstring(luas, str);
//	}
//	else if (vtp == thLValue::Type::BYTE
//		|| vtp == thLValue::Type::INTEGER)
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asInt());
//	}
//	else if (vtp == thLValue::Type::BOOLEAN)
//	{
//		tolua_pushboolean(luas, value.asBool());
//	}
//	else if (vtp == thLValue::Type::UNSIGNED)
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asUnsignedInt());
//	}
//	else if (vtp == thLValue::Type::FLOAT)
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asFloat());
//	}
//	else if (vtp == thLValue::Type::DOUBLE)
//	{
//		tolua_pushnumber(luas, (lua_Number)value.asDouble());
//	}
//	else
//	{
//		lua_settop(luas, 1);
//	}
//
//	return 1;
//}

/// ===================================================================================
// 辅助 返回var到lua
template<typename TClass, typename VarType>
inline int __retArgLuaVar(lua_State* luas, TClass* objptr, VarType TClass::* pvar)
{
	auto& tp = typeid(VarType);
	std::string name = tp.name();
	auto fd = int(name.find("class"));
	if (fd != -1)
	{
		// 类
		if (tp == typeid(std::string))
		{
			std::string& val = *(std::string*)&(objptr->*pvar);
			tolua_pushcppstring(luas, val);
		}
		else if (tp == typeid(std::string*))
		{
			auto ptr = *(std::string**)&(objptr->*pvar);
			if (!ptr)
			{
				throw "__retArgLuaVar: std::string* ptr is nil";
			}
			std::string& val = *ptr;
			tolua_pushcppstring(luas, val);
		}
		else if (tp == typeid(thValue))
		{
			lua_settop(luas, 1);
		}
		else if (tp == typeid(thLuaFunction*))
		{
			lua_settop(luas, 1);
		}
		else
		{
			auto className = getClassType(name);
			auto val = *(thRef**)&(objptr->*pvar);
			object_to_luaval<thRef>(luas, className.c_str(), (thRef*)val);
		}
	}
	else if (tp == typeid(bool))
	{
		auto val = *(bool*)&(objptr->*pvar);
		tolua_pushboolean(luas, val);
	}
	else if (tp == typeid(int16_t))
	{
		auto val = *(int16_t*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(int32_t))
	{
		auto val = *(int32_t*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(uint32_t))
	{
		auto val = *(uint32_t*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(float))
	{
		auto val = *(float*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(double))
	{
		auto val = *(double*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(long long))
	{
		auto val = *(long long*)&(objptr->*pvar);
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else
	{
		lua_settop(luas, 1);
	}

	return 1;
}

/// ===================================================================================
// 辅助 返回值到lua
template<typename R>
inline int __retArgLua(lua_State* luas, R value)
{
	auto& tp = typeid(R);
	std::string name = tp.name();
	auto fd = int(name.find("class"));
	if (fd != -1)
	{
		// 类
		if (tp == typeid(std::string))
		{
			std::string& val = *(std::string*)&value;
			tolua_pushcppstring(luas, val);
		}
		else if (tp == typeid(std::string*))
		{
			auto ptr = *(std::string**)&value;
			if (!ptr)
			{
				throw "__retArgLua: std::string* ptr is nil";
			}
			std::string& val = *ptr;
			tolua_pushcppstring(luas, val);
		}
		else if (tp == typeid(thValue))
		{
			lua_settop(luas, 1);
		}
		else if (tp == typeid(thLuaFunction*))
		{
			lua_settop(luas, 1);
		}
		else
		{
			auto className = getClassType(name);
			auto ptr = *(thRef**)&value;
			object_to_luaval<thRef>(luas, className.c_str(), (thRef*)ptr);
		}
	}
	else if (tp == typeid(bool))
	{
		auto val = *(bool*)&value;
		tolua_pushboolean(luas, val);
	}
	else if (tp == typeid(int16_t))
	{
		auto val = *(int16_t*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(int32_t))
	{
		auto val = *(int32_t*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(uint32_t))
	{
		auto val = *(uint32_t*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(float))
	{
		auto val = *(float*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(double))
	{
		auto val = *(double*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else if (tp == typeid(long long))
	{
		auto val = *(long long*)&value;
		tolua_pushnumber(luas, (lua_Number)val);
	}
	else
	{
		lua_settop(luas, 1);
	}

	return 1;
}

/// ===================================================================================
// 获取luo返回值赋值到thValue
inline bool __getLuaRetLValue(lua_State* luas, int lo, thValue* value)
{
	int valueType = lua_type(luas, lo);

	if (valueType == LUA_TNIL)
	{
		return true;
	}
	if (valueType == LUA_TNUMBER)
	{
		auto val = (int)tolua_tonumber(luas, lo, 0);
		value->pushArg_int(val);
		return true;
	}
	if (valueType == LUA_TBOOLEAN)
	{
		auto val = tolua_toboolean(luas, lo, 0) != 0;
		value->pushArg_bool(val);
		return true;
	}
	if (valueType == LUA_TSTRING)
	{
		auto val = tolua_tocppstring(luas, lo, "");
		value->pushArg_string(val);
		return true;
	}
	if (valueType == LUA_TUSERDATA)
	{
		auto val = (thRef*)tolua_tousertype(luas, lo, 0);
		value->pushArg_object(val);
		return true;
	}
	if (valueType == LUA_TFUNCTION)
	{
		// lua 函数
		LUA_FUNCTION handler = (toluafix_ref_function(luas, lo, 0));
		// 保存记录
		auto val = thLuaFunction::create(handler);
		value->pushArg_luafunc(val);
		return true;
	}

	return false;
}

/// ===================================================================================
// 读取luo参数赋值到thValue 根据P类型
template<typename P>
inline bool __readArgLua(lua_State* luas, int lo, thValue* value, int idx=-1)
{
	auto& tp = typeid(P);
	std::string name = tp.name();
	auto fd = int(name.find("class"));
	if (fd != -1)
	{
		// 类
		if (tp == typeid(std::string))
		{
			std::string val;
			if (!luaval_to_std_string(luas, lo, &val))
			{
				return false;
			}
			value->pushArg_string(val, idx);
		}
		else if (tp == typeid(thValue))
		{
			value->pushArg_null(idx);
		}
		else if (tp == typeid(thLuaFunction*))
		{
			// lua 函数
			LUA_FUNCTION handler = (toluafix_ref_function(luas, lo, 0));
			value->pushArg_luafunc(thLuaFunction::create(handler), idx);
		}
		else
		{
			auto className = getClassType(name);
			thRef* val = nullptr;
			if (!luaval_to_object<thRef>(luas, lo, className.c_str(), &val))
			{
				return false;
			}
			value->pushArg_object(val, idx);
		}
	}
	else if (tp == typeid(char*)
		|| tp == typeid(const char*))
	{
		std::string val;
		if (!luaval_to_std_string(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_string(val, idx);
	}
	else if (tp == typeid(bool))
	{
		bool val;
		if (!luaval_to_boolean(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_bool(val, idx);
	}
	else if (tp == typeid(unsigned char))
	{
		int val;
		if (!luaval_to_int32(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_int(val, idx);
	}
	else if (tp == typeid(int16_t))
	{
		int val;
		if (!luaval_to_int32(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_int(val, idx);
	}
	else if (tp == typeid(int32_t))
	{
		int val;
		if (!luaval_to_int32(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_int(val, idx);
	}
	else if (tp == typeid(uint32_t))
	{
		uint32_t val;
		if (!luaval_to_uint32(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_uint(val, idx);
	}
	else if (tp == typeid(float))
	{
		double val;
		if (!luaval_to_number(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_float((float)val, idx);
	}
	else if (tp == typeid(double))
	{
		double val;
		if (!luaval_to_number(luas, lo, &val))
		{
			return false;
		}
		value->pushArg_double(val, idx);
	}
	return true;
}

/// ===================================================================================
// 读取luo参数赋值到thValue 根据lua类型
inline bool __readArgLuaByLuaType(lua_State* luas, int lo, thValue* value)
{
	value->readLuaValue(luas, lo);
	return true;
}

/// ===================================================================================
// 赋值 类pvar = thLValue
template<typename TClass, typename VarType>
inline void __setVarByLValue(thValue* value, TClass* objptr, VarType TClass::* pvar)
{
	auto& tp = typeid(VarType);
	std::string name = tp.name();
	auto fd = int(name.find("class"));
	if (fd != -1)
	{
		// 类
		if (tp == typeid(std::string))
		{
			*(std::string*)&(objptr->*pvar) = value->argString(0);
		}
		else if (tp == typeid(std::string*))
		{
			auto ptr = *(std::string**)&(objptr->*pvar);
			if (!ptr)
			{
				throw "__setVarByLValue: std::string* ptr is nil";
			}
			*ptr = value->argString(0);
		}
		else if (tp == typeid(thValue))
		{
			
		}
		else if (tp == typeid(thLuaFunction*))
		{
			auto ptr = (thLuaFunction**)&(objptr->*pvar);
			*ptr = value->argLuaFunction(0);
		}
		else
		{
			*(thRef**)&(objptr->*pvar) = (thRef*)value->argObject(0);
		}
	}
	else if (tp == typeid(bool))
	{
		*(bool*)&(objptr->*pvar) = value->argBool(0);
	}
	else if (tp == typeid(int16_t))
	{
		*(int16_t*)&(objptr->*pvar) = value->argInt(0);
	}
	else if (tp == typeid(int32_t))
	{
		*(int32_t*)&(objptr->*pvar) = value->argInt(0);
	}
	else if (tp == typeid(uint32_t))
	{
		*(uint32_t*)&(objptr->*pvar) = value->argUInt(0);
	}
	else if (tp == typeid(float))
	{
		*(float*)&(objptr->*pvar) = value->argFloat(0);
	}
	else if (tp == typeid(double))
	{
		*(double*)&(objptr->*pvar) = value->argDouble(0);
	}
}

/// ===================================================================================
// 添加参数到value

// 辅助 转换参数到thValue
template<typename P>
inline void __parArgType(thValue* ret, va_list& params, int idx=-1)
{
	auto& tp = typeid(P);
	std::string name = tp.name();
	auto fd = int(name.find("class"));
	if (fd != -1)
	{
		// 类
		if (tp == typeid(std::string))
		{
			auto val = (char*)va_arg(params, char*);
			ret->pushArg_string(val, idx);
		}
		else if (tp == typeid(std::string*))
		{
			auto val = (char*)va_arg(params, char*);
			ret->pushArg_string(val, idx);
		}
	}
	else if (tp == typeid(bool))
	{
		auto val = (bool)va_arg(params, bool);
		ret->pushArg_bool(val, idx);
	}
	else if (tp == typeid(int32_t))
	{
		auto val = (int)va_arg(params, int);
		ret->pushArg_int(val, idx);
	}
	else if (tp == typeid(uint32_t))
	{
		auto val = (unsigned int)va_arg(params, unsigned int);
		ret->pushArg_uint(val, idx);
	}
	else if (tp == typeid(float))
	{
		auto val = (float)va_arg(params, double);
		ret->pushArg_float(val, idx);
	}
	else if (tp == typeid(double))
	{
		auto val = (double)va_arg(params, double);
		ret->pushArg_double(val, idx);
	}
}

/// ===================================================================================
// 解析函数默认参数
template<typename result_type>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	return arg;
}
// 展开函数
template <typename result_type, class P1>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 1);
	return arg;
}
template <typename result_type, class P1, class P2>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	else if (defargcount == 1) return __parArg<result_type, P2>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 2);
	__parArgType<P2>(arg, params, argcount - 1);
	return arg;
}
template <typename result_type, class P1, class P2, class P3>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	else if (defargcount == 1) return __parArg<result_type, P3>(arg, argcount, defargcount, params);
	else if (defargcount == 2) return __parArg<result_type, P2, P3>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 3);
	__parArgType<P2>(arg, params, argcount - 2);
	__parArgType<P3>(arg, params, argcount - 1);
	return arg;
}
template <typename result_type, class P1, class P2, class P3, class P4>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	else if (defargcount == 1) return __parArg<result_type, P4>(arg, argcount, defargcount, params);
	else if (defargcount == 2) return __parArg<result_type, P3, P4>(arg, argcount, defargcount, params);
	else if (defargcount == 3) return __parArg<result_type, P2, P3, P4>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 4);
	__parArgType<P2>(arg, params, argcount - 3);
	__parArgType<P3>(arg, params, argcount - 2);
	__parArgType<P4>(arg, params, argcount - 1);
	return arg;
}
template <typename result_type, class P1, class P2, class P3, class P4, class P5>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	else if (defargcount == 1) return __parArg<result_type, P5>(arg, argcount, defargcount, params);
	else if (defargcount == 2) return __parArg<result_type, P4, P5>(arg, argcount, defargcount, params);
	else if (defargcount == 3) return __parArg<result_type, P3, P4, P5>(arg, argcount, defargcount, params);
	else if (defargcount == 4) return __parArg<result_type, P2, P3, P4, P5>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 5);
	__parArgType<P2>(arg, params, argcount - 4);
	__parArgType<P3>(arg, params, argcount - 3);
	__parArgType<P4>(arg, params, argcount - 2);
	__parArgType<P5>(arg, params, argcount - 1);
	return arg;
}
template <typename result_type, class P1, class P2, class P3, class P4, class P5, class P6>
inline thValue* __parArg(thValue* arg, int argcount, int defargcount, va_list params) {
	if (defargcount == 0) return __parArg<result_type>(arg, argcount, defargcount, params);
	else if (defargcount == 1) return __parArg<result_type, P6>(arg, argcount, defargcount, params);
	else if (defargcount == 2) return __parArg<result_type, P5, P6>(arg, argcount, defargcount, params);
	else if (defargcount == 3) return __parArg<result_type, P4, P5, P6>(arg, argcount, defargcount, params);
	else if (defargcount == 4) return __parArg<result_type, P3, P4, P5, P6>(arg, argcount, defargcount, params);
	else if (defargcount == 5) return __parArg<result_type, P2, P3, P4, P5, P6>(arg, argcount, defargcount, params);
	if (!arg) arg = thValue::create();
	__parArgType<P1>(arg, params, argcount - 6);
	__parArgType<P2>(arg, params, argcount - 5);
	__parArgType<P3>(arg, params, argcount - 4);
	__parArgType<P4>(arg, params, argcount - 3);
	__parArgType<P5>(arg, params, argcount - 2);
	__parArgType<P6>(arg, params, argcount - 1);
	return arg;
}

/// ===================================================================================
// call
template<typename result_type>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg=nullptr)
{
	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)() = (result_type(*)())(funcref.ptr.gfunc.func);
		return f();
	}
	else
	{
		result_type(thUnknownClass::*f)() = (result_type(thUnknownClass::*)())funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)();
	}
}

/// ===================================================================================
template<typename result_type, typename P1>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		if (argc > 0)
		{
			int argidx = 2;
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1) = (result_type(*)(P1))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1) = (result_type(thUnknownClass::*)(P1))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2) = (result_type(*)(P1, P2))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2) = (result_type(thUnknownClass::*)(P1, P2))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3) = (result_type(*)(P1, P2, P3))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3) = (result_type(thUnknownClass::*)(P1, P2, P3))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3, typename P4>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
		if (argc > 3)
		{
			if (!__readArgLua<P4>(luas, argidx + 3, arg, 3))
				throw "__callArg: error: arg4";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3, P4) = (result_type(*)(P1, P2, P3, P4))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2)), thConvType<P4>::convValue(arg->argValueDec(3)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3, P4) = (result_type(thUnknownClass::*)(P1, P2, P3, P4))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2)), thConvType<P4>::convValue(arg->argValueDec(3)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3, typename P4, typename P5>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
		if (argc > 3)
		{
			if (!__readArgLua<P4>(luas, argidx + 3, arg, 3))
				throw "__callArg: error: arg4";
		}
		if (argc > 4)
		{
			if (!__readArgLua<P5>(luas, argidx + 4, arg, 4))
				throw "__callArg: error: arg5";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3, P4, P5) = (result_type(*)(P1, P2, P3, P4, P5))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3, P4, P5) = (result_type(thUnknownClass::*)(P1, P2, P3, P4, P5))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
		if (argc > 3)
		{
			if (!__readArgLua<P4>(luas, argidx + 3, arg, 3))
				throw "__callArg: error: arg4";
		}
		if (argc > 4)
		{
			if (!__readArgLua<P5>(luas, argidx + 4, arg, 4))
				throw "__callArg: error: arg5";
		}
		if (argc > 5)
		{
			if (!__readArgLua<P6>(luas, argidx + 5, arg, 5))
				throw "__callArg: error: arg6";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3, P4, P5, P6) = (result_type(*)(P1, P2, P3, P4, P5, P6))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3, P4, P5, P6) = (result_type(thUnknownClass::*)(P1, P2, P3, P4, P5, P6))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
		if (argc > 3)
		{
			if (!__readArgLua<P4>(luas, argidx + 3, arg, 3))
				throw "__callArg: error: arg4";
		}
		if (argc > 4)
		{
			if (!__readArgLua<P5>(luas, argidx + 4, arg, 4))
				throw "__callArg: error: arg5";
		}
		if (argc > 5)
		{
			if (!__readArgLua<P6>(luas, argidx + 5, arg, 5))
				throw "__callArg: error: arg6";
		}
		if (argc > 6)
		{
			if (!__readArgLua<P7>(luas, argidx + 6, arg, 6))
				throw "__callArg: error: arg7";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3, P4, P5, P6, P7) = (result_type(*)(P1, P2, P3, P4, P5, P6, P7))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)), thConvType<P7>::convValue(arg->argValueDec(6)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3, P4, P5, P6, P7) = (result_type(thUnknownClass::*)(P1, P2, P3, P4, P5, P6, P7))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)), thConvType<P7>::convValue(arg->argValueDec(6)));
	}
}

/// ===================================================================================
template<typename result_type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline result_type __callArg(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc, thValue* arg = nullptr)
{
	// 解析参数
	if (!arg) arg = thValue::create();
	if (arg)
	{
		int argidx = 2;
		if (argc > 0)
		{
			if (!__readArgLua<P1>(luas, argidx, arg, 0))
				throw "__callArg: error: arg1";
		}
		if (argc > 1)
		{
			if (!__readArgLua<P2>(luas, argidx + 1, arg, 1))
				throw "__callArg: error: arg2";
		}
		if (argc > 2)
		{
			if (!__readArgLua<P3>(luas, argidx + 2, arg, 2))
				throw "__callArg: error: arg3";
		}
		if (argc > 3)
		{
			if (!__readArgLua<P4>(luas, argidx + 3, arg, 3))
				throw "__callArg: error: arg4";
		}
		if (argc > 4)
		{
			if (!__readArgLua<P5>(luas, argidx + 4, arg, 4))
				throw "__callArg: error: arg5";
		}
		if (argc > 5)
		{
			if (!__readArgLua<P6>(luas, argidx + 5, arg, 5))
				throw "__callArg: error: arg6";
		}
		if (argc > 6)
		{
			if (!__readArgLua<P7>(luas, argidx + 6, arg, 6))
				throw "__callArg: error: arg7";
		}
		if (argc > 7)
		{
			if (!__readArgLua<P8>(luas, argidx + 7, arg, 7))
				throw "__callArg: error: arg8";
		}
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(P1, P2, P3, P4, P5, P6, P7, P8) = (result_type(*)(P1, P2, P3, P4, P5, P6, P7, P8))(funcref.ptr.gfunc.func);
		return f(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)), thConvType<P7>::convValue(arg->argValueDec(6)), thConvType<P8>::convValue(arg->argValueDec(7)));
	}
	else
	{
		result_type(thUnknownClass::*f)(P1, P2, P3, P4, P5, P6, P7, P8) = (result_type(thUnknownClass::*)(P1, P2, P3, P4, P5, P6, P7, P8))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(thConvType<P1>::convValue(arg->argValueDec(0)), thConvType<P2>::convValue(arg->argValueDec(1)), thConvType<P3>::convValue(arg->argValueDec(2))
			, thConvType<P4>::convValue(arg->argValueDec(3)), thConvType<P5>::convValue(arg->argValueDec(4))
			, thConvType<P6>::convValue(arg->argValueDec(5)), thConvType<P7>::convValue(arg->argValueDec(6)), thConvType<P8>::convValue(arg->argValueDec(7)));
	}
}

/// ===================================================================================
template<typename result_type>
inline result_type __callArgAuto(lua_State* luas, const thDEC_FuncPtr& funcref, void* objptr, int argc)
{
	// 解析参数
	thValue* arg = thValue::create();
	int argidx = 2;
	for (int i=0; i< argc; ++i)
	{
		if (!__readArgLuaByLuaType(luas, argidx + i, arg))
			throw "__callArgAuto: error: arg";
	}

	if (funcref.flag == thFlag_GlabalFunc)
	{
		result_type(*f)(thValue*) = (result_type(*)(thValue*))(funcref.ptr.gfunc.func);
		return f(arg);
	}
	else
	{
		result_type(thUnknownClass::*f)(thValue*) = (result_type(thUnknownClass::*)(thValue*))funcref.ptr.cfunc.func;
		return (((thUnknownClass*)objptr)->*f)(arg);
	}
}


/// ===================================================================================
// thGlobalDef
inline thGlobalDef::thGlobalDef(lua_State* luas, const char* namespaceName/*=""*/)
{
	m_luas = luas;
	m_name = namespaceName;
	m_vReg.clear();
	if (m_name.empty())
	{
		m_name = "_G";	// 全局
	}
}

/// ===================================================================================
inline void thGlobalDef::end(void)
{
	if (m_luas && !m_vReg.empty())
	{
		m_vReg.emplace_back();
		luaL_register(m_luas, m_name.c_str(), m_vReg.data());
	}
}

/// ===================================================================================
template<typename...Args>
inline thGlobalDef& thGlobalDef::func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg, ...)
{
	if (m_luas)
	{
		if (funcref.flag != thFlag_GlabalFunc) { __thDF_LOG("thGlobalDef::func func is type error method", funcName); return *this; }
		if (isFunc(funcName)) { __thDF_LOG("thGlobalDef::func name:%s have existed", funcName); return *this; }
		auto count = (int)sizeof...(Args);
		if (defarg > count)
		{
			__thDF_LOG("thGlobalDef::func name:%s defargcount > argcount", funcName);
			return *this;
		}
		// 默认参数
		thValue* defargval = nullptr;
		if (defarg > 0)
		{
			va_list params;
			va_start(params, defarg);
			defargval = __parArg<void, Args...>(nullptr, count, defarg, params);
			va_end(params);
			if (defargval) thLuaEngine::getInstance()->addObject(defargval);
		}
		m_vReg.emplace_back(funcName, [=](lua_State* luas)
		{
			auto argc = lua_gettop(luas);
			if (argc == count
				|| argc == count - defarg
				|| argc < count && argc > count - defarg)
			{
				try
				{
					__callArg<void, Args...>(luas, funcref, nullptr, argc, defargval);
				}
				catch (const char* err)
				{
					__thDF_LOG("thGlobalDef::func name:%s __callArg error: %s", funcName, err);
				}
				lua_settop(luas, 1);
				return 0;
			}
			else
			{
				__thDF_LOG("thGlobalDef::func name:%s error funcargc != luaarc: %d-%d", funcName, count, argc);
			}
			return 0;
		});
	}
	return *this;
}

/// ===================================================================================
template<typename result_type, typename...Args>
inline thGlobalDef& thGlobalDef::funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg, ...)
{
	if (m_luas)
	{
		if (funcref.flag != thFlag_GlabalFunc) { __thDF_LOG("thGlobalDef::funcr func is type error method", funcName); return *this; }
		if (isFunc(funcName)) { __thDF_LOG("thGlobalDef::funcr name:%s have existed", funcName); return *this; }
		auto count = (int)sizeof...(Args);
		if (defarg > count)
		{
			__thDF_LOG("thGlobalDef::funcr name:%s defargcount > argcount", funcName);
			return *this;
		}
		// 默认参数
		thValue* defargval = nullptr;
		if (defarg > 0)
		{
			va_list params;
			va_start(params, defarg);
			defargval = __parArg<void, Args...>(nullptr, count, defarg, params);
			va_end(params);
			if (defargval) thLuaEngine::getInstance()->addObject(defargval);
		}
		m_vReg.emplace_back({ funcName, [=](lua_State* luas)
		{
			auto argc = lua_gettop(luas) - 1;
			if (argc == count
				|| argc == count - defarg
				|| argc < count && argc > count - defarg)
			{
				try
				{
					return __retArgLua<result_type>(luas, __callArg<result_type, Args...>(luas, funcref, nullptr, argc, defargval));
				}
				catch (const char* err)
				{
					__thDF_LOG("thGlobalDef::funcr name:%s __callArg error: %s", funcName, err);
				}
			}
			else
			{
				__thDF_LOG("thGlobalDef::funcr name:%s error funcargc != luaarc: %d-%d", funcName, count, argc);
			}
			lua_settop(luas, 1);
			return 0;
		} });
	}
	return *this;
}

/// ===================================================================================
inline bool thGlobalDef::isFunc(const char* funcName)
{
	std::string funcn = funcName;
	for (auto &v : m_vReg)
	{
		if (v.name == funcn)
		{
			return true;
		}
	}
	return false;
}

/// ===================================================================================
inline thGlobalDef& thGlobalDef::def(const char *name, int value)
{
	if (m_luas) tolua_constant(m_luas, name, value);
	return *this;
}


/// ===================================================================================
// thClassDef
template<typename TClass>
thClassDef<TClass>::thClassDef(lua_State* luas, const char* className, const char* classBase)
{
	m_luas = luas;
	m_name = className;
	if (m_luas)
	{
		tolua_open(m_luas);
		tolua_module(m_luas, nullptr, 0);
		tolua_beginmodule(m_luas, nullptr);
		// class
		tolua_usertype(m_luas, m_name.c_str());
		tolua_cclass(m_luas, m_name.c_str(), m_name.c_str(), classBase, nullptr);
		tolua_beginmodule(m_luas, m_name.c_str());
		//// 创建create
		//tolua_function(m_luas, "create", [](lua_State* luas)
		//{
		//	auto ret = TClass::create();
		//	// tolua_pushusertype(luas, (void*)ret, m_name.c_str());
		//	object_to_luaval<TClass>(luas, typeid(TClass).name(), ret);
		//	return 1;
		//});
		registerClassType<TClass>(m_name);
	}
}

/// ===================================================================================
template<typename TClass>
void thClassDef<TClass>::end(void)
{
	if (m_luas)
	{
		tolua_endmodule(m_luas);
		tolua_endmodule(m_luas);
	}
}


/// ===================================================================================
template<typename TClass>
template<typename... Args>
thClassDef<TClass>& thClassDef<TClass>::func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg, ...)
{
	if (m_luas)
	{
		if (isFunc(funcName)) { __thDF_LOG("thClassDef::func name:%s have existed", funcName); return *this; }
		auto count = (int)sizeof...(Args);
		if (defarg > count)
		{
			__thDF_LOG("thClassDef::func name:%s defargcount > argcount", funcName);
			return *this;
		}
		// 默认参数
		thValue* defargval = nullptr;
		if (defarg > 0)
		{
			va_list params;
			va_start(params, defarg);
			defargval = __parArg<void,Args...>(nullptr, count, defarg, params);
			va_end(params);
			if (defargval) thLuaEngine::getInstance()->addObject(defargval);
		}
		m_vFunc.emplace_back(funcName);
		tolua_function(m_luas, funcName, [=](lua_State* luas)
		{
			TClass* cobj = nullptr;
			if (funcref.flag == thFlag_MethodFunc)
			{
				cobj = (TClass*)tolua_tousertype(luas, 1, 0);
				if (!cobj)
				{
					return 0;
				}
			}

			auto argc = lua_gettop(luas) - 1;
			if (argc == count
				|| argc == count - defarg
				|| argc < count && argc > count - defarg)
			{
				try
				{
					__callArg<void, Args...>(luas, funcref, cobj, argc, defargval);
				}
				catch (const char* err)
				{
					__thDF_LOG("thClassDef::func name:%s __callArg error: %s", funcName, err);
				}
			}
			else
			{
				__thDF_LOG("thClassDef::func name:%s error funcargc != luaarc: %d-%d", funcName, count, argc);
			}
			lua_settop(luas, 1);
			return 0;
		});
	}
	return *this;
}


/// ===================================================================================
template<typename TClass>
template<typename result_type, typename... Args>
thClassDef<TClass>& thClassDef<TClass>::funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg, ...)
{
	if (m_luas)
	{
		if (isFunc(funcName)) { __thDF_LOG("thClassDef::funcr name:%s have existed", funcName); return *this; }
		auto count = (int)sizeof...(Args);
		if (defarg > count)
		{
			__thDF_LOG("thClassDef::funcr name:%s defargcount > argcount", funcName); 
			return *this;
		}
		// 默认参数
		thValue* defargval = nullptr;
		if (defarg > 0)
		{
			va_list params;
			va_start(params, defarg);
			defargval = __parArg<result_type,Args...>(nullptr, count, defarg, params);
			va_end(params);
			if (defargval) thLuaEngine::getInstance()->addObject(defargval);
		}
		m_vFunc.emplace_back(funcName);
		tolua_function(m_luas, funcName, [=](lua_State* luas)
		{
			TClass* cobj = nullptr;
			if (funcref.flag == thFlag_MethodFunc)
			{
				cobj = (TClass*)tolua_tousertype(luas, 1, 0);
				if (!cobj)
				{
					return 0;
				}
			}

			int ret = 0;
			auto argc = lua_gettop(luas) - 1;
			if (argc == count
				|| argc == count - defarg
				|| argc < count && argc > count - defarg)
			{
				try
				{
					ret = __retArgLua<result_type>(luas, __callArg<result_type, Args...>(luas, funcref, cobj, argc, defargval));
				}
				catch (const char* err)
				{
					__thDF_LOG("thClassDef::funcr name:%s __callArg error: %s", funcName, err);
					lua_settop(luas, 1);
				}
			}
			else
			{
				__thDF_LOG("thClassDef::funcr name:%s error funcargc != luaarc: %d-%d", funcName, count, argc);
				lua_settop(luas, 1);
			}
			return ret;
		});
	}
	return *this;
}

/// ===================================================================================
template<typename TClass>
inline thClassDef<TClass>& thClassDef<TClass>::funcAuto(const char* funcName, const thDEC_FuncPtr& funcref)
{
	if (m_luas)
	{
		if (isFunc(funcName)) { __thDF_LOG("thClassDef::funcAuto name:%s have existed", funcName); return *this; }
		m_vFunc.emplace_back(funcName);
		tolua_function(m_luas, funcName, [=](lua_State* luas)
		{
			TClass* cobj = nullptr;
			if (funcref.flag == thFlag_MethodFunc)
			{
				cobj = (TClass*)tolua_tousertype(luas, 1, 0);
				if (!cobj)
				{
					return 0;
				}
			}

			auto argc = lua_gettop(luas) - 1;
			try
			{
				__callArgAuto<void>(luas, funcref, cobj, argc);
			}
			catch (const char* err)
			{
				__thDF_LOG("thClassDef::funcAuto name:%s __callArg error: %s", funcName, err);
			}
			lua_settop(luas, 1);
			return 0;
		});
	}
	return *this;
}

/// ===================================================================================
template<typename TClass>
template<typename result_type>
inline thClassDef<TClass>& thClassDef<TClass>::funcrAuto(const char* funcName, const thDEC_FuncPtr& funcref)
{
	if (m_luas)
	{
		if (isFunc(funcName)) { __thDF_LOG("thClassDef::funcrAuto name:%s have existed", funcName); return *this; }
		m_vFunc.emplace_back(funcName);
		tolua_function(m_luas, funcName, [=](lua_State* luas)
		{
			TClass* cobj = nullptr;
			if (funcref.flag == thFlag_MethodFunc)
			{
				cobj = (TClass*)tolua_tousertype(luas, 1, 0);
				if (!cobj)
				{
					return 0;
				}
			}

			auto argc = lua_gettop(luas) - 1;
			try
			{
				return __retArgLua<result_type>(luas, __callArgAuto<result_type>(luas, funcref, cobj, argc));
			}
			catch (const char* err)
			{
				__thDF_LOG("thClassDef::funcrAuto name:%s __callArg error: %s", funcName, err);
			}
			lua_settop(luas, 1);
			return 0;
		});
	}
	return *this;
}

/// ===================================================================================
template<typename TClass>
thClassDef<TClass>& thClassDef<TClass>::funcCall(const char* funcName, const thDEC_FuncPtr& funcref, bool isStatic)
{
	if (m_luas)
	{
		if (isFunc(funcName)) { __thDF_LOG("thClassDef::funcCall name:%s have existed", funcName); return *this; }
		m_vFunc.emplace_back(funcName);
		tolua_function(m_luas, funcName, [=](lua_State* luas)
		{
			TClass* cobj = nullptr;
			if (!isStatic)
			{
				cobj = (TClass*)tolua_tousertype(luas, 1, 0);
				if (!cobj)
				{
					return 0;
				}
			}

			auto argc = lua_gettop(luas) - 1;
			int(*f)(lua_State*, TClass*, int) = (int(*)(lua_State*, TClass*, int))(funcref.ptr.gfunc.func);
			return f(luas, cobj, argc);
		});
	}
	return *this;
}

/// ===================================================================================
template<typename TClass>
bool thClassDef<TClass>::isFunc(const char* funcName)
{
	std::string funcn = funcName;
	for (auto &v: m_vFunc)
	{
		if (v == funcn)
		{
			return true;
		}
	}
	return false;
}

/// ===================================================================================
template<typename TClass>
template<typename VarType>
thClassDef<TClass>& thClassDef<TClass>::var(const char* varName, VarType TClass::* pvar)
{
	if (m_luas)
	{
		tolua_variable(m_luas, varName,
			[=](lua_State* luas)	// get
		{
			auto cobj = (TClass*)tolua_tousertype(luas, 1, 0);
			if (!cobj)
			{
				return 0;
			}
			return __retArgLuaVar<TClass, VarType>(luas, cobj, pvar);
		},
			[=](lua_State* luas)	// set
		{
			auto cobj = (TClass*)tolua_tousertype(luas, 1, 0);
			if (!cobj)
			{
				return 0;
			}
			auto argc = lua_gettop(luas) - 1;
			if (1 == argc)
			{
				thValue* value = thValue::create();
				__getLuaRetLValue(luas, 2, value);
				__setVarByLValue(value, cobj, pvar);
				lua_settop(luas, 1);
				return 0;
			}

			return 0;
		});
	}
	return *this;
}

///// ===================================================================================
//template<typename TClass>
//thClassDef<TClass>& thClassDef<TClass>::varLValue(const char* varName, thLValue TClass::* pvar)
//{
//	if (m_luas)
//	{
//		tolua_variable(m_luas, varName,
//			[=](lua_State* luas)	// get
//		{
//			auto cobj = (TClass*)tolua_tousertype(luas, 1, 0);
//			if (!cobj)
//			{
//				return 0;
//			}
//			return __retArgLuaVar<TClass, thLValue>(luas, cobj, pvar);
//		},
//			[=](lua_State* luas)	// set
//		{
//			auto cobj = (TClass*)tolua_tousertype(luas, 1, 0);
//			if (!cobj)
//			{
//				return 0;
//			}
//			auto argc = lua_gettop(luas) - 1;
//			if (1 == argc)
//			{
//				__getLuaRetLValue(luas, 2, cobj->*pvar);
//				lua_settop(luas, 1);
//				return 0;
//			}
//
//			return 0;
//		});
//	}
//	return *this;
//}

/// ===================================================================================
template<typename TClass>
thClassDef<TClass>& thClassDef<TClass>::def(const char *name, int value)
{
	if (m_luas) tolua_constant(m_luas, name, value);
	return *this;
}


/// ===================================================================================