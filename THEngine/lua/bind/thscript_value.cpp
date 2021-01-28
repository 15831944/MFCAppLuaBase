#include "thscript_value.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "thscript_bind.h"
#include "thscript_allocator.h"

// 缓存value key字符串
typedef std::map<size_t, std::string> HashString;
HashString g_mapHashString;			// hash string

void str_hashCache(size_t hash, const std::string& str)
{
	g_mapHashString.emplace(hash, str);
}

std::string* str_getHashCache(size_t hash)
{
	auto iter = g_mapHashString.find(hash);
	if (iter != g_mapHashString.end())
	{
		return &iter->second;
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
const thDEC_Value thDEC_Value::Null;

thDEC_Value::thDEC_Value(thValue* v)
	: _type(Type::THVALUE)
{
	v->retain();
	_field.value = v;
}


thDEC_Value::thDEC_Value(const char* v)
	: _type(Type::STRING)
{
	_field.strVal = thAllocator::allocateType<std::string>(v);
}


thDEC_Value::thDEC_Value(const std::string& v)
	: _type(Type::STRING)
{
	_field.strVal = thAllocator::allocateType<std::string>(v);
}

thDEC_Value& thDEC_Value::operator=(const thDEC_Value& other)
{
	if (this != &other)
	{
		reset(other._type);

		switch (other._type)
		{
		case Type::INTEGER:
			_field.intVal = other._field.intVal;
			break;
		case Type::UNSIGNED:
			_field.unsignedVal = other._field.unsignedVal;
			break;
		case Type::FLOAT:
			_field.floatVal = other._field.floatVal;
			break;
		case Type::BOOLEAN:
			_field.boolVal = other._field.boolVal;
			break;
		case Type::DOUBLE:
			_field.doubleVal = other._field.doubleVal;
			break;
		case Type::STRING:
			if (_field.strVal)
			{
				*_field.strVal = *other._field.strVal;
				break;
			}
			_field.strVal = thAllocator::allocateType<std::string>(*other._field.strVal);
			break;
		case Type::THVALUE:
			if (_field.value) _field.value->release();
			_field.value = other._field.value;
			if (_field.value) _field.value->retain();
			break;
		case Type::OBJECT:
			if (_field.object) _field.object->release();
			_field.object = other._field.object;
			if (_field.object) _field.object->retain();
			break;
		case Type::LUA_FUNC:
			if (_field.luafunc) _field.luafunc->release();
			_field.luafunc = other._field.luafunc;
			if (_field.luafunc) _field.luafunc->retain();
			break;
		default:
			break;
		}
	}
	return *this;
}

thDEC_Value& thDEC_Value::operator=(thDEC_Value&& other)
{
	if (this != &other)
	{
		clear();
		switch (other._type)
		{
		case Type::INTEGER:
			_field.intVal = other._field.intVal;
			break;
		case Type::UNSIGNED:
			_field.unsignedVal = other._field.unsignedVal;
			break;
		case Type::FLOAT:
			_field.floatVal = other._field.floatVal;
			break;
		case Type::BOOLEAN:
			_field.boolVal = other._field.boolVal;
			break;
		case Type::DOUBLE:
			_field.doubleVal = other._field.doubleVal;
			break;
		case Type::STRING:
			_field.strVal = other._field.strVal;
			break;
		case Type::THVALUE:
			_field.value = other._field.value;
			break;
		case Type::OBJECT:
			_field.object = other._field.object;
			break;
		case Type::LUA_FUNC:
			_field.luafunc = other._field.luafunc;
			break;
		default:
			break;
		}
		_type = other._type;

		memset(&other._field, 0, sizeof(other._field));
		other._type = Type::NONE;
	}

	return *this;
}

void thDEC_Value::clear(void)
{
	// Free memory the old value allocated
	switch (_type)
	{
	case Type::INTEGER:
		_field.intVal = 0;
		break;
	case Type::UNSIGNED:
		_field.unsignedVal = 0u;
		break;
	case Type::FLOAT:
		_field.floatVal = 0.0f;
		break;
	case Type::DOUBLE:
		_field.doubleVal = 0.0;
		break;
	case Type::BOOLEAN:
		_field.boolVal = false;
		break;
	case Type::ULONGLONG:
		_field.ullVal = 0ll;
		break;
	case Type::STRING:
		thAllocator::freeType<std::string>(_field.strVal);
		_field.strVal = nullptr;
		break;
	case Type::THVALUE:
		SAFE_RELEASE_NULL(_field.value);
		break;
	case Type::OBJECT:
		SAFE_RELEASE_NULL(_field.object);
		break;
	case Type::LUA_FUNC:
		SAFE_RELEASE_NULL(_field.luafunc);
		break;
	default:
		break;
	}

	memset(&_field, 0, sizeof(_field));
	_type = Type::NONE;
}

void thDEC_Value::reset(Type type)
{
	if (_type == type)
		return;

	clear();

	//// Allocate memory for the new value
	//switch (type)
	//{
	//case Type::STRING:
	//	_field.strVal = new (std::nothrow) std::string();
	//	break;
	//default:
	//	break;
	//}

	_type = type;
}

int thDEC_Value::asInt(void) const
{
	if (_type == Type::NONE)
	{
		return 0;
	}

	if (_type == Type::INTEGER)
	{
		return _field.intVal;
	}

	if (_type == Type::UNSIGNED)
	{
		__thDF_assert(_field.unsignedVal < INT_MAX, "Can only convert values < INT_MAX");
		return (int)_field.unsignedVal;
	}

	if (_type == Type::STRING)
	{
		return atoi(_field.strVal->c_str());
	}

	if (_type == Type::FLOAT)
	{
		return static_cast<int>(_field.floatVal);
	}

	if (_type == Type::DOUBLE)
	{
		return static_cast<int>(_field.doubleVal);
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal ? 1 : 0;
	}

	return 0;
}

unsigned int thDEC_Value::asUnsignedInt(void) const
{
	if (_type == Type::NONE)
	{
		return 0u;
	}

	if (_type == Type::UNSIGNED)
	{
		return _field.unsignedVal;
	}

	if (_type == Type::INTEGER)
	{
		__thDF_assert(_field.intVal >= 0, "Only values >= 0 can be converted to unsigned");
		return static_cast<unsigned int>(_field.intVal);
	}

	if (_type == Type::STRING)
	{
		// NOTE: strtoul is required (need to augment on unsupported platforms)
		return static_cast<unsigned int>(strtoul(_field.strVal->c_str(), nullptr, 10));
	}

	if (_type == Type::FLOAT)
	{
		return static_cast<unsigned int>(_field.floatVal);
	}

	if (_type == Type::DOUBLE)
	{
		return static_cast<unsigned int>(_field.doubleVal);
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal ? 1u : 0u;
	}

	return 0u;
}

float thDEC_Value::asFloat(void) const
{
	if (_type == Type::NONE)
	{
		return 0.0f;
	}

	if (_type == Type::FLOAT)
	{
		return _field.floatVal;
	}

	if (_type == Type::STRING)
	{
		return (float)atof(_field.strVal->c_str());
	}

	if (_type == Type::INTEGER)
	{
		return static_cast<float>(_field.intVal);
	}

	if (_type == Type::UNSIGNED)
	{
		return static_cast<float>(_field.unsignedVal);
	}

	if (_type == Type::DOUBLE)
	{
		return static_cast<float>(_field.doubleVal);
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal ? 1.0f : 0.0f;
	}

	return 0.0f;
}

double thDEC_Value::asDouble(void) const
{
	if (_type == Type::NONE)
	{
		return 0.0;
	}

	if (_type == Type::DOUBLE)
	{
		return _field.doubleVal;
	}

	if (_type == Type::STRING)
	{
		return static_cast<double>(atof(_field.strVal->c_str()));
	}

	if (_type == Type::INTEGER)
	{
		return static_cast<double>(_field.intVal);
	}

	if (_type == Type::UNSIGNED)
	{
		return static_cast<double>(_field.unsignedVal);
	}

	if (_type == Type::FLOAT)
	{
		return static_cast<double>(_field.floatVal);
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal ? 1.0 : 0.0;
	}

	return 0.0;
}

bool thDEC_Value::asBool(void) const
{
	if (_type == Type::NONE)
	{
		return false;
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal;
	}

	if (_type == Type::STRING)
	{
		return (*_field.strVal == "0" || *_field.strVal == "false") ? false : true;
	}

	if (_type == Type::INTEGER)
	{
		return _field.intVal == 0 ? false : true;
	}

	if (_type == Type::UNSIGNED)
	{
		return _field.unsignedVal == 0 ? false : true;
	}

	if (_type == Type::FLOAT)
	{
		return _field.floatVal == 0.0f ? false : true;
	}

	if (_type == Type::DOUBLE)
	{
		return _field.doubleVal == 0.0 ? false : true;
	}

	return false;
}


unsigned long long thDEC_Value::asULongLong(void) const
{
	if (_type == Type::NONE)
	{
		return 0ll;
	}

	if (_type == Type::ULONGLONG)
	{
		return _field.ullVal;
	}

	if (_type == Type::DOUBLE)
	{
		return static_cast<unsigned long long>(_field.doubleVal);
	}

	if (_type == Type::STRING)
	{
		return static_cast<unsigned long long>(atof(_field.strVal->c_str()));
	}

	if (_type == Type::INTEGER)
	{
		return static_cast<unsigned long long>(_field.intVal);
	}

	if (_type == Type::UNSIGNED)
	{
		return static_cast<unsigned long long>(_field.unsignedVal);
	}

	if (_type == Type::FLOAT)
	{
		return static_cast<unsigned long long>(_field.floatVal);
	}

	if (_type == Type::BOOLEAN)
	{
		return _field.boolVal ? 1 : 0;
	}

	return 0ll;
}

std::string thDEC_Value::asString(void) const
{
	if (_type == Type::NONE)
	{
		return "";
	}
	if (_type == Type::STRING)
	{
		return *_field.strVal;
	}

	std::stringstream ret;

	switch (_type)
	{
	case Type::INTEGER:
		ret << _field.intVal;
		break;
	case Type::UNSIGNED:
		ret << _field.unsignedVal;
		break;
	case Type::ULONGLONG:
		ret << _field.ullVal;
		break;
	case Type::FLOAT:
		ret << std::fixed << std::setprecision(7) << _field.floatVal;
		break;
	case Type::DOUBLE:
		ret << std::fixed << std::setprecision(16) << _field.doubleVal;
		break;
	case Type::BOOLEAN:
		ret << (_field.boolVal ? "true" : "false");
		break;
	default:
		break;
	}

	return ret.str();
}

std::string* thDEC_Value::asStringPtr(void) const
{
	if (_type == Type::STRING)
	{
		return _field.strVal;
	}

	return nullptr;
}

thValue* thDEC_Value::asValue(void) const
{
	if (_type == Type::THVALUE)
	{
		return _field.value;
	}

	return nullptr;
}

thRef* thDEC_Value::asObject(void) const
{
	if (_type == Type::OBJECT)
	{
		return _field.object;
	}

	return nullptr;
}

thLuaFunction* thDEC_Value::asLuaFunction(void) const
{
	if (_type == Type::LUA_FUNC)
	{
		return _field.luafunc;
	}

	return nullptr;
}

void thDEC_Value::retValueToLua(lua_State* luas)
{
	switch (_type)
	{
	case thDEC_Value::Type::INTEGER:
	{
		auto _val = asInt();
		lua_pushinteger(luas, _val);
		break;
	}
	case thDEC_Value::Type::THVALUE:
	{
		auto _val = asValue();
		_val->retValueToLua(luas, true);
		break;
	}
	case thDEC_Value::Type::BOOLEAN:
	{
		auto _val = asBool();
		lua_pushboolean(luas, _val);
		break;
	}
	case thDEC_Value::Type::FLOAT:
	{
		auto _val = asFloat();
		lua_pushnumber(luas, _val);
		break;
	}
	case thDEC_Value::Type::UNSIGNED:
	{
		auto _val = asUnsignedInt();
		lua_pushnumber(luas, _val);
		break;
	}
	case thDEC_Value::Type::STRING:
	{
		auto _val = asString();
		lua_pushlstring(luas, _val.c_str(), _val.length());
		break;
	}
	case thDEC_Value::Type::DOUBLE:
	{
		auto _val = asDouble();
		lua_pushnumber(luas, _val);
		break;
	}
	case thDEC_Value::Type::ULONGLONG:
	{
		auto _val = asULongLong();
		char szfmt[100];
		snprintf(szfmt, 100, "%llu", _val);
		lua_pushstring(luas, szfmt);
		break;
	}
	case thDEC_Value::Type::OBJECT:
	{
		auto _val = asObject();
		toluafix_pushusertype_ccobject(luas, _val->_ID, &_val->_luaID, _val, getClassType(typeid(*_val).name()).c_str());
		break;
	}
	default:
		lua_pushnil(luas);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////


// lua func
int createFromLuaArg_(lua_State* luas, thValue* obj, int argc)
{
	thValue* ret = thValue::create();
	if (argc > 0)
	{
		int lo = 2;
		for (int i = 0; i < argc; ++i)
		{
			ret->readLuaValue(luas, lo);
			++lo;
		}
	}
	object_to_luaval<thValue>(luas, "thValue", (thValue*)ret);
	return 1;
}
int createFromTable_(lua_State* luas, thValue* obj, int argc)
{
	thValue* ret = thValue::create();
	if (argc > 0)
	{
		ret->readLuaValueTable(luas);
	}
	object_to_luaval<thValue>(luas, "thValue", (thValue*)ret);
	return 1;
}

int push_lua_(lua_State* luas, thValue* obj, int argc)
{
	if (!obj) return 0;
	if (argc > 0)
	{
		thValue* val = thValue::create();
		std::string name;
		if (!luaval_to_std_string(luas, 2, &name))
		{
			return 0;
		}
		val->readLuaValue(luas, 3);
		obj->push_value(name, val);
		lua_settop(luas, 1);
		return 0;
	}
	luaL_error(luas, "%s has wrong number of arguments: %d, was expecting %d \n", "thValue:push_lua_", argc, 0);
	return 0;
}

int pushArg_lua_(lua_State* luas, thValue* obj, int argc)
{
	if (!obj) return 0;
	if (argc > 0)
	{
		thValue* val = thValue::create();
		val->readLuaValue(luas, 2);
		obj->pushArg_value(val);
		lua_settop(luas, 1);
		return 0;
	}
	luaL_error(luas, "%s has wrong number of arguments: %d, was expecting %d \n", "thValue:pushArg_lua_", argc, 0);
	return 0;
}

int getLuaTable_(lua_State* luas, thValue* obj, int argc)
{
	if (!obj) return 0;
	if (argc == 0)
	{
		return obj->retValueToLua(luas, true);
	}
	luaL_error(luas, "%s has wrong number of arguments: %d, was expecting %d \n", "thValue:getLuaTable_", argc, 0);
	return 0;
}
int getLuaArg_(lua_State* luas, thValue* obj, int argc)
{
	if (!obj) return 0;
	if (argc == 0)
	{
		return obj->retValueToLua(luas);
	}
	luaL_error(luas, "%s has wrong number of arguments: %d, was expecting %d \n", "thValue:getLuaArg_", argc, 0);
	return 0;
}


// 注册到lua
bool thValue::registerToLua(lua_State* luas)
{
	thClassDef<thValue>(luas, "thValue")
		// create
		.funcr<thValue*, bool>("create", thDF_FuncConv(thValue::create), 1, true)
		// as func
		.func<const std::string&>("push_null", thDF_CFuncConv(thValue, push_null))
		.func<const std::string&, bool>("push_bool", thDF_CFuncConv(thValue, push_bool))
		.func<const std::string&, int>("push_int", thDF_CFuncConv(thValue, push_int))
		.func<const std::string&, int>("push_uint", thDF_CFuncConv(thValue, push_uint))
		.func<const std::string&, float>("push_float", thDF_CFuncConv(thValue, push_float))
		.func<const std::string&, double>("push_double", thDF_CFuncConv(thValue, push_double))
		.func<const std::string&, const std::string&>("push_ulonglong", thDF_CFuncConv(thValue, push_ulonglong))
		.func<const std::string&, const std::string&>("push_string", thDF_CFuncConv(thValue, push_string))
		.func<const std::string&, thValue*>("push_value", thDF_CFuncConv(thValue, push_value))
		.func<const std::string&, thRef*>("push_object", thDF_CFuncConv(thValue, push_object))
		.func<const std::string&, thLuaFunction*>("push_luafunc", thDF_CFuncConv(thValue, push_luafunc))
		.func<const std::string&>("remove_value", thDF_CFuncConv(thValue, remove_value))
		.func<int>("remove_valueById", thDF_CFuncConv(thValue, remove_valueById))
		.func<bool>("setArray", thDF_CFuncConv(thValue, setArray))
		.funcr<bool, const std::string&>("isValue", thDF_CFuncConv(thValue, isValue))
		.funcr<bool>("isObject", thDF_CFuncConv(thValue, isObject))
		.funcr<bool>("isArray", thDF_CFuncConv(thValue, isArray))
		.funcr<bool>("isChange", thDF_CFuncConv(thValue, isChange))
		.funcr<int>("size", thDF_CFuncConv(thValue, size))
		// as
		.funcr<bool, const std::string&>("asBool", thDF_CFuncConv(thValue, asBool))
		.funcr<int, const std::string&>("asInt", thDF_CFuncConv(thValue, asInt))
		.funcr<unsigned int, const std::string&>("asUInt", thDF_CFuncConv(thValue, asUInt))
		.funcr<float, const std::string&>("asFloat", thDF_CFuncConv(thValue, asFloat))
		.funcr<double, const std::string&>("asDouble", thDF_CFuncConv(thValue, asDouble))
		.funcr<std::string, const std::string&>("asULongLong", thDF_CFuncConv(thValue, asULongLong))
		.funcr<std::string, const std::string&>("asString", thDF_CFuncConv(thValue, asString))
		.funcr<thValue*, const std::string&>("asValue", thDF_CFuncConv(thValue, asValue))
		.funcr<thRef*, const std::string&>("asObject", thDF_CFuncConv(thValue, asObject))
		// arg func
		.func<int>("pushArg_null", thDF_CFuncConv(thValue, pushArg_null), 1, -1)
		.func<bool, int>("pushArg_bool", thDF_CFuncConv(thValue, pushArg_bool), 1, -1)
		.func<int, int>("pushArg_int", thDF_CFuncConv(thValue, pushArg_int), 1, -1)
		.func<int, int>("pushArg_uint", thDF_CFuncConv(thValue, pushArg_uint), 1, -1)
		.func<float, int>("pushArg_float", thDF_CFuncConv(thValue, pushArg_float), 1, -1)
		.func<double, int>("pushArg_double", thDF_CFuncConv(thValue, pushArg_double), 1, -1)
		.func<const std::string&, int>("pushArg_ulonglong", thDF_CFuncConv(thValue, pushArg_ulonglong), 1, -1)
		.func<const std::string&, int>("pushArg_string", thDF_CFuncConv(thValue, pushArg_string), 1, -1)
		.func<thValue*, int>("pushArg_value", thDF_CFuncConv(thValue, pushArg_value), 1, -1)
		.func<thRef*, int>("pushArg_object", thDF_CFuncConv(thValue, pushArg_object), 1, -1)
		.func<thLuaFunction*, int>("pushArg_luafunc", thDF_CFuncConv(thValue, pushArg_luafunc), 1, -1)
		// arg
		.funcr<bool, int>("argBool", thDF_CFuncConv(thValue, argBool))
		.funcr<int, int>("argInt", thDF_CFuncConv(thValue, argInt))
		.funcr<unsigned int, int>("argUInt", thDF_CFuncConv(thValue, argUInt))
		.funcr<float, int>("argFloat", thDF_CFuncConv(thValue, argFloat))
		.funcr<double, int>("argDouble", thDF_CFuncConv(thValue, argDouble))
		.funcr<std::string, int>("argULongLong", thDF_CFuncConv(thValue, argULongLong))
		.funcr<std::string, int>("argString", thDF_CFuncConv(thValue, argString))
		.funcr<thValue*, int>("argValue", thDF_CFuncConv(thValue, argValue))
		.funcr<thRef*, int>("argObject", thDF_CFuncConv(thValue, argObject))
		// lua 专属操作函数
		.funcCall("createFromLuaArg", thDF_FuncConv(createFromLuaArg_), true)
		.funcCall("createFromTable", thDF_FuncConv(createFromTable_), true)
		.funcCall("push_lua", thDF_FuncConv(push_lua_))
		.funcCall("pushArg_lua", thDF_FuncConv(pushArg_lua_))
		.funcCall("getLuaTable", thDF_FuncConv(getLuaTable_))
		.funcCall("getLuaArg", thDF_FuncConv(getLuaArg_))
		.end();


	return true;
}

//////////////////////////////////////////////////////////////////////////
// dec cache
inline thDEC_Value* _createDECValue(void)
{
	return thAllocator::allocateType<thDEC_Value>();
}

template<class T>
inline thDEC_Value* _createDECValue(T v)
{
	return thAllocator::allocateType<thDEC_Value>(v);
}

// 回收
inline void _freeDECValue(thDEC_Value* v)
{
	if (v)
	{
		v->~thDEC_Value();
		thAllocator::freeType(v);
	}
}

// create
thValue* thValue::create(bool bSaveKeyStr)
{
	thValue* val = new(thAllocator::allocate(sizeof(thValue))) thValue();
	if (!val) return nullptr;
	// if (size > 0) val->_mapValue.reserve(size);
	val->_bSaveKeyStr = bSaveKeyStr;
	val->_isArray = false;
	val->_isChange = true;
	val->autorelease();
	return val;
}

bool thValue::_release(void)
{
	if (_referenceCount == 0)
	{
		// 删除
		for (auto &v : _mapValue)
		{
			_freeDECValue(v.second);
		}
		_mapValue.clear();
		this->~thValue();
		thAllocator::freeType(this);
	}
	return false;
}

std::string thValue::asULongLong(const std::string& name) const
{
	auto ll = getValue(name)->asULongLong();
	char szfmt[100];
	snprintf(szfmt, 100, "%llu", ll);
	return szfmt;
}

void thValue::setAsBool(const std::string& name, bool value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::BOOLEAN)
	{
		iter->second->_field.boolVal = value;
		return;
	}
}

void thValue::setAsInt(const std::string& name, int value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::INTEGER)
	{
		iter->second->_field.intVal = value;
		return;
	}
}

void thValue::setAsUInt(const std::string& name, unsigned int value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::UNSIGNED)
	{
		iter->second->_field.unsignedVal = value;
		return;
	}
}

void thValue::setAsFloat(const std::string& name, float value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::FLOAT)
	{
		iter->second->_field.floatVal = value;
		return;
	}
}

void thValue::setAsDouble(const std::string& name, double value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::DOUBLE)
	{
		iter->second->_field.doubleVal = value;
		return;
	}
}

void thValue::setAsLongLong(const std::string& name, const std::string& value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::ULONGLONG)
	{
		unsigned long long _value = strtoull(value.c_str(), nullptr, 10);
		iter->second->_field.ullVal = _value;
		return;
	}
}

void thValue::setAsString(const std::string& name, const std::string& value)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::STRING)
	{
		if (iter->second->_field.strVal)
		{
			*iter->second->_field.strVal = value;
			return;
		}
		iter->second->_field.strVal = thAllocator::allocateType<std::string>(value);
		return;
	}
}
void thValue::push_null(const std::string& name)
{
	if (isValue(name))
	{
		remove_value(name);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue());
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_bool(const std::string& name, bool value)
{
	if (isValue(name))
	{
		setAsBool(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_int(const std::string& name, int value)
{
	if (isValue(name))
	{
		setAsInt(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_uint(const std::string& name, unsigned int value)
{
	if (isValue(name))
	{
		setAsUInt(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_float(const std::string& name, float value)
{
	if (isValue(name))
	{
		setAsFloat(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_double(const std::string& name, double value)
{
	if (isValue(name))
	{
		setAsDouble(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_ulonglong(const std::string& name, const std::string& value)
{
	if (isValue(name))
	{
		setAsLongLong(name, value);
		return;
	}
	unsigned long long _value = strtoull(value.c_str(), nullptr, 10);

	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(_value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	};
}

void thValue::push_string(const std::string& name, const std::string& value)
{
	if (isValue(name))
	{
		setAsString(name, value);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_value(const std::string& name, thValue* value)
{
	if (!value)
	{
		remove_value(name);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_object(const std::string& name, thRef* value)
{
	if (!value)
	{
		remove_value(name);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

void thValue::push_luafunc(const std::string& name, thLuaFunction *value)
{
	if (!value)
	{
		remove_value(name);
		return;
	}
	auto hash = std::hash<std::string>()(name);
	push_valueDec(hash, _createDECValue(value));
	if (_bSaveKeyStr)
	{
		str_hashCache(hash, name);
	}
}

bool thValue::push_valueDec(const std::string& name, thDEC_Value* value)
{
	return push_valueDec(std::hash<std::string>()(name), value);
}

bool thValue::push_valueDec(size_t hash, thDEC_Value* value)
{
	_isChange = true;
	auto iter = _mapValue.find(hash);
	if (iter != _mapValue.end())
	{
		_freeDECValue(iter->second);
		iter->second = value;
		return true;
	}

	_mapValue.emplace(hash, value);
	return true;
}

bool thValue::isValue(const std::string& name)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end())
	{
		return true;
	}

	return false;
}

void thValue::remove_value(const std::string& name)
{
	auto iter = _mapValue.find(std::hash<std::string>()(name));
	if (iter != _mapValue.end())
	{
		_isChange = true;
		_freeDECValue(iter->second);
		_mapValue.erase(iter);
	}
}

void thValue::remove_valueById(int idx)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end())
	{
		_isChange = true;
		_freeDECValue(iter->second);
		if (!isArray())
		{
			_mapValue.erase(iter);
		}
		else
		{
			if (_mapValue.size() == 1)
			{
				_mapValue.erase(iter);
				return;
			}
			else
			{
				// 0 1 2 3
				int size = (int)_mapValue.size();
				for (int i = idx; i < size - 1; ++i)
				{
					_mapValue[i] = _mapValue[i + 1];
					_mapValue[i + 1] = nullptr;
				}
				_mapValue.erase(size - 1);
			}
		}
	}
}


std::string* thValue::_getValueKey(int idx)
{
	return str_getHashCache(idx);
}

void thValue::pushArg_null(int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue());
}

void thValue::pushArg_bool(bool value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_int(int value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_uint(unsigned int value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_float(float value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_double(double value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_ulonglong(const std::string& value, int idx)
{
	setArray(true);
	unsigned long long _value = strtoull(value.c_str(), nullptr, 10);

	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(_value));
}

void thValue::pushArg_string(const std::string& value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_value(thValue* value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	if (!value)
	{
		push_valueDec(idx, _createDECValue());
		return;
	}
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_object(thRef* value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	if (!value)
	{
		push_valueDec(idx, _createDECValue());
		return;
	}
	push_valueDec(idx, _createDECValue(value));
}

void thValue::pushArg_luafunc(thLuaFunction* value, int idx)
{
	setArray(true);
	if (idx < 0) idx = _mapValue.size();
	if (!value)
	{
		push_valueDec(idx, _createDECValue());
		return;
	}
	push_valueDec(idx, _createDECValue(value));
}

std::string thValue::argULongLong(int idx) const
{
	auto ll = getValue(idx)->asULongLong();
	char szfmt[100];
	snprintf(szfmt, 100, "%llu", ll);
	return szfmt;
}

void thValue::setArgBool(int idx, bool value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::BOOLEAN)
	{
		iter->second->_field.boolVal = value;
		return;
	}
}

void thValue::setArgInt(int idx, int value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::INTEGER)
	{
		iter->second->_field.intVal = value;
		return;
	}
}

void thValue::setArgUInt(int idx, unsigned int value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::UNSIGNED)
	{
		iter->second->_field.unsignedVal = value;
		return;
	}
}

void thValue::setArgFloat(int idx, float value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::FLOAT)
	{
		iter->second->_field.floatVal = value;
		return;
	}
}

void thValue::setArgDouble(int idx, double value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::DOUBLE)
	{
		iter->second->_field.doubleVal = value;
		return;
	}
}

void thValue::setArgLongLong(int idx, const std::string& value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::ULONGLONG)
	{
		unsigned long long _value = strtoull(value.c_str(), nullptr, 10);
		iter->second->_field.ullVal = _value;
		return;
	}
}

void thValue::setArgString(int idx, const std::string& value)
{
	auto iter = _mapValue.find(idx);
	if (iter != _mapValue.end() && iter->second->getType() == thDEC_Value::Type::BOOLEAN)
	{
		if (iter->second->_field.strVal && *iter->second->_field.strVal == value)
		{
			return;
		}
		if (iter->second->_field.strVal)
		{
			*iter->second->_field.strVal = value;
			return;
		}
		iter->second->_field.strVal = thAllocator::allocateType<std::string>(value);
		return;
	}
}

void thValue::readLuaValue(lua_State *luas, int lo, int index)
{
	int valueType = lua_type(luas, lo);

	if (valueType == LUA_TNIL)
	{
		pushArg_null(index);
	}
	else if (valueType == LUA_TNUMBER)
	{
		auto val = (int)tolua_tonumber(luas, lo, 0);
		pushArg_int(val, index);
	}
	else if (valueType == LUA_TBOOLEAN)
	{
		auto val = tolua_toboolean(luas, lo, 0) != 0;
		pushArg_bool(val, index);
	}
	else if (valueType == LUA_TSTRING)
	{
		auto val = tolua_tocppstring(luas, lo, "");
		pushArg_string(val, index);
	}
	else if (valueType == LUA_TUSERDATA)
	{
		auto val = (thRef*)tolua_tousertype(luas, lo, 0);
		pushArg_object(val, index);
	}
	else if (valueType == LUA_TFUNCTION)
	{
		// lua 函数
		LUA_FUNCTION handler = toluafix_ref_function(luas, lo, 0);
		pushArg_luafunc(thLuaFunction::create(handler));
	}
	else if (valueType == LUA_TTABLE)
	{
		auto val = thValue::create();
		val->readLuaValueTable(luas);
		pushArg_value(val, index);
	}
	else
	{
		pushArg_null(index);
	}
}

void thValue::readLuaValueTable(lua_State *luas)
{
	try
	{
		int isType = 0;
		size_t keyId = 0;
		std::string keyStr = "";

		// 取 table 索引值 
		auto lo = lua_gettop(luas);
		// push nil
		lua_pushnil(luas);
		while (lua_next(luas, lo) != 0)
		{
			int keyType = lua_type(luas, -2);

			size_t _keyId = 0;
			keyStr = "";
			if (keyType == LUA_TNUMBER)
			{
				if (isType == 0) isType = LUA_TNUMBER;
				keyId = (size_t)lua_tonumber(luas, -2) - 1;
				if (isType == LUA_TSTRING)
				{
					keyStr = std::to_string(keyId);
				}
				else _keyId = keyId;
			}
			else if (keyType == LUA_TSTRING)
			{
				if (isType == 0) isType = LUA_TSTRING;
				std::string idstring = lua_tostring(luas, -2);
				if (isType == LUA_TNUMBER)
				{
					keyId = atoi(idstring.c_str());
					_keyId = keyId;
				}
				else
				{
					keyStr = idstring;
					// keyId = std::hash<std::string>()(idstring);
				}
			}
			else
			{
				_keyId = keyId;
				++keyId;
			}

			int valueType = lua_type(luas, -1);
			switch (valueType) {
			case LUA_TNIL:
			{
				if (isType == LUA_TNUMBER) pushArg_null(_keyId);
				else push_null(keyStr);
				break;
			}
			case LUA_TBOOLEAN:
			{
				auto val = lua_toboolean(luas, -1) != 0;
				if (isType == LUA_TNUMBER) pushArg_bool(val, _keyId);
				else push_bool(keyStr, val);
				break;
			}
			case LUA_TNUMBER:
			{
				auto val = (int)lua_tonumber(luas, -1);
				if (isType == LUA_TNUMBER) pushArg_int(val, _keyId);
				else push_int(keyStr, val);
				break;
			}
			case LUA_TSTRING:
			{
				auto val = lua_tostring(luas, -1);
				if (isType == LUA_TNUMBER) pushArg_string(val, _keyId);
				else push_string(keyStr, val);
				break;
			}
			case LUA_TUSERDATA:
			{
				auto val = (thRef*)tolua_tousertype(luas, -1, 0);
				if (isType == LUA_TNUMBER) pushArg_object(val, _keyId);
				else push_object(keyStr, val);
				break;
			}
			case LUA_TFUNCTION:
			{
				// lua 函数
				LUA_FUNCTION handler = toluafix_ref_function(luas, -1, 0);
				auto val = thLuaFunction::create(handler);
				if (isType == LUA_TNUMBER) pushArg_luafunc(val, _keyId);
				else push_luafunc(keyStr, val);
				break;
			}
			case LUA_TTABLE:
			{
				auto val = thValue::create();
				val->readLuaValueTable(luas);
				if (isType == LUA_TNUMBER) pushArg_value(val, _keyId);
				else push_value(keyStr, val);
				break;
			}
			default:
			{
				if (isType == LUA_TNUMBER) pushArg_null(_keyId);
				else push_null(keyStr);
				break;
			}
			}
			lua_pop(luas, 1);
		}
	}
	catch (const char* s)
	{
		lua_pop(luas, 1);
		tolua_error(luas, s, nullptr);
	}
	catch (std::exception& e)
	{
		const char* errMsg = e.what();
		lua_pop(luas, 1);
		tolua_error(luas, errMsg, nullptr);
	}
	catch (...)
	{
		const char* errMsg = lua_tostring(luas, -1);
		lua_pop(luas, 1);
		tolua_error(luas, errMsg, nullptr);
	}
}

int thValue::retValueToLua(lua_State *luas, bool isTable/*=false*/)
{
	int retcnt = 0;
	if (isTable)
	{
		// L: table
		lua_newtable(luas);
	}

	if (_mapValue.empty())
	{
		return retcnt;
	}


	// array
	auto decVal = _mapValue.begin();
	auto cache = _getValueKey(decVal->first);
	if (_isArray
		|| cache == nullptr)
	{
		std::vector<int> vId;
		for (auto &v : _mapValue)
		{
			vId.push_back(v.first);
		}
		std::sort(vId.begin(), vId.end());

		int idx = 1;
		for (auto &v : vId)
		{
			auto val = (thDEC_Value*)getValue(v);
			if (val->isNull())
			{
				break;
			}
			val->retValueToLua(luas);
			if (isTable)
			{
				// table[index] = value, L: table
				lua_rawseti(luas, -2, idx);
			}
			++idx;
			++retcnt;
		}
	}
	else
	{
		// map
		for (auto &v : _mapValue)
		{
			// add key
			if (isTable)
			{
				auto strkey = _getValueKey(v.first);
				if (!strkey) break;
				lua_pushlstring(luas, strkey->c_str(), strkey->length());
			}
			// value
			v.second->retValueToLua(luas);

			if (isTable)
			{
				// table.key = value, L: table
				lua_rawset(luas, -3);
			}
			++retcnt;
		}
	}
	return isTable?1:retcnt;
}