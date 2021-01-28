#ifndef __THSCRIPT_VALUE_H__
#define __THSCRIPT_VALUE_H__

#include <string>
#include <vector>
#include <unordered_map>
#include "thscript_objref.h"

/**
 * @addtogroup base
 * @{
 */

class thValue;
struct lua_State;

struct thDEC_Value
{
	static const thDEC_Value Null;

	enum class Type
	{
		/// no value is wrapped, an empty Value
		NONE = 0,
		/// wrap integer
		INTEGER,
		/// wrap unsigned integer
		UNSIGNED,
		/// wrap float
		FLOAT,
		/// wrap double
		DOUBLE,
		/// wrap bool
		BOOLEAN,
		/// wrap string
		STRING,
		/// unsigned long long
		ULONGLONG,
		/// value
		THVALUE,
		/// thReft
		OBJECT,
		/// lua function
		LUA_FUNC
	};
	union
	{
		unsigned char byteVal;
		int intVal;
		unsigned int unsignedVal;
		float floatVal;
		double doubleVal;
		bool boolVal;
		unsigned long long ullVal;

		std::string* strVal;
		thValue* value;
		thRef* object;
		thLuaFunction* luafunc;
	}_field;

	Type _type;

	thDEC_Value(void) : _type(Type::NONE) { memset(&_field, 0, sizeof(_field)); }
	thDEC_Value(bool v) : _type(Type::BOOLEAN) { _field.boolVal = v; }
	thDEC_Value(int v) : _type(Type::INTEGER) { _field.intVal = v; }
	thDEC_Value(unsigned int v) : _type(Type::UNSIGNED) { _field.unsignedVal = v; }
	thDEC_Value(float v) : _type(Type::FLOAT) { _field.floatVal = v; }
	thDEC_Value(double v) : _type(Type::DOUBLE) { _field.doubleVal = v; }
	thDEC_Value(unsigned long long v) : _type(Type::ULONGLONG) { _field.ullVal = v; }
	thDEC_Value(const char* v);
	thDEC_Value(const std::string& v);
	thDEC_Value(thValue* v);
	thDEC_Value(thRef* v) : _type(Type::OBJECT) { v->retain(); _field.object = v; }
	thDEC_Value(thLuaFunction* v) : _type(Type::LUA_FUNC) { v->retain(); _field.luafunc = v; }
	thDEC_Value(const thDEC_Value& other) : _type(Type::NONE) { *this = other; }
	thDEC_Value(thDEC_Value&& other) : _type(Type::NONE) { *this = std::move(other); }
	~thDEC_Value(void) { clear(); }

	/** Assignment operator, assign from Value to Value. */
	thDEC_Value& operator= (const thDEC_Value& other);
	/** Assignment operator, assign from Value to Value. It will use std::move internally. */
	thDEC_Value& operator= (thDEC_Value&& other);

	void clear(void);
	void reset(Type type);

	inline bool isNull(void) const { return _type == Type::NONE; }
	inline Type getType(void) const { return _type; }

	// get value
	int asInt(void) const;
	unsigned int asUnsignedInt(void) const;
	float asFloat(void) const;
	double asDouble(void) const;
	bool asBool(void) const;
	unsigned long long asULongLong(void) const;
	std::string asString(void) const;
	std::string* asStringPtr(void) const;
	thValue* asValue(void) const;
	thRef* asObject(void) const;
	thLuaFunction* asLuaFunction(void) const;

	// value
	template<class result>
	void* asValuePtr(void)
	{
		auto& tp = typeid(result);
		std::string name = tp.name();
		auto fd = int(name.find("class"));
		if (fd != -1)
		{
			// class
			if (tp == typeid(std::string))
			{
				return _field.strVal;
			}
			else if (tp == typeid(thLuaFunction*))
			{
				return _field.luafunc;
			}
			else
			{
				return _field.object;
			}
		}
		else if (tp == typeid(char))
		{
			if (_type == Type::STRING)
			{
				return (void*)_field.strVal->data();
			}
			else
			{
				return &_field.intVal;
			}
		}
		else if (tp == typeid(bool))
		{
			return &_field.boolVal;
		}
		else if (tp == typeid(unsigned char))
		{
			return &_field.byteVal;
		}
		else if (tp == typeid(int16_t))
		{
			return &_field.intVal;
		}
		else if (tp == typeid(int32_t))
		{
			return &_field.intVal;
		}
		else if (tp == typeid(uint32_t))
		{
			return &_field.unsignedVal;
		}
		else if (tp == typeid(float))
		{
			return &_field.floatVal;
		}
		else if (tp == typeid(double))
		{
			return &_field.doubleVal;
		}
		
		return nullptr;
	}
	template<class result>
	void* asValuePtr2(void)
	{
		return &asValuePtr<result>();
	}

	// 返回到lua
	void retValueToLua(lua_State* luas);

};
typedef std::unordered_map <size_t, thDEC_Value*> MapHashValue;

// value数据包裹类
class thValue : public thRef
{
public:
	~thValue(void) {}
	// 创建value
	static thValue* create(bool bSaveKeyStr = true);
	// ref
	bool _release(void);

public:
	bool asBool(const std::string& name) const { return getValue(name)->asBool(); }
	int asInt(const std::string& name) const { return getValue(name)->asInt(); }
	unsigned int asUInt(const std::string& name) const { return getValue(name)->asUnsignedInt(); }
	float asFloat(const std::string& name) const { return getValue(name)->asFloat(); }
	double asDouble(const std::string& name) const { return getValue(name)->asDouble(); }
	std::string asULongLong(const std::string& name) const;
	std::string asString(const std::string& name) const { return getValue(name)->asString(); }
	thValue* asValue(const std::string& name) const { return getValue(name)->asValue(); }
	thRef* asObject(const std::string& name) const { return getValue(name)->asObject(); }
	thLuaFunction* asLuaFunction(const std::string& name) const { return getValue(name)->asLuaFunction(); }

	// setval
	void setAsBool(const std::string& name, bool value);
	void setAsInt(const std::string& name, int value);
	void setAsUInt(const std::string& name, unsigned int value);
	void setAsFloat(const std::string& name, float value);
	void setAsDouble(const std::string& name, double value);
	void setAsLongLong(const std::string& name, const std::string& value);
	void setAsString(const std::string& name, const std::string& value);

public:
	void push_null(const std::string& name);
	void push_bool(const std::string& name, bool value);
	void push_int(const std::string& name, int value);
	void push_uint(const std::string& name, unsigned int value);
	void push_float(const std::string& name, float value);
	void push_double(const std::string& name, double value);
	void push_ulonglong(const std::string& name, const std::string& value);
	void push_string(const std::string& name, const std::string& value);
	void push_value(const std::string& name, thValue* value);
	void push_object(const std::string& name, thRef* value);
	void push_luafunc(const std::string& name, thLuaFunction *value);
	bool push_valueDec(const std::string& name, thDEC_Value* value);
	bool push_valueDec(size_t hash, thDEC_Value* value);

	// remove
	void remove_value(const std::string& name);
	void remove_valueById(int idx);

	// 状态判断.
	bool isValue(const std::string& name);
	void setArray(bool isArray) { _isArray = isArray; }
	bool isObject(void) { return !_isArray; }
	bool isArray(void) { return _isArray; }
	bool isChange(void) { return _isChange; }
	size_t size(void) { return _mapValue.size(); }

	// get
	const thDEC_Value* getValue(const std::string& name) const
	{
		auto iter = _mapValue.find(std::hash<std::string>()(name));
		if (iter != _mapValue.end())
		{
			return iter->second;
		}
		return &thDEC_Value::Null;
	}
	const thDEC_Value* getValue(int idx) const
	{
		auto iter = _mapValue.find(idx);
		if (iter != _mapValue.end())
		{
			return iter->second;
		}
		return &thDEC_Value::Null;
	}
	thDEC_Value* _getValue(int idx)
	{
		auto iter = _mapValue.find(idx);
		if (iter != _mapValue.end())
		{
			return iter->second;
		}
		return nullptr;
	}
	// get key
	std::string* _getValueKey(int idx);
	// all
	MapHashValue& _getMapValue()
	{
		return _mapValue;
	}

	// 下标快速调用
public:
	void pushArg_null(int idx = -1);
	void pushArg_bool(bool value, int idx = -1);
	void pushArg_int(int value, int idx = -1);
	void pushArg_uint(unsigned int value, int idx = -1);
	void pushArg_float(float value, int idx = -1);
	void pushArg_double(double value, int idx = -1);
	void pushArg_ulonglong(const std::string& value, int idx = -1);
	void pushArg_string(const std::string& value, int idx = -1);
	void pushArg_value(thValue* value, int idx = -1);
	void pushArg_object(thRef* value, int idx = -1);
	void pushArg_luafunc(thLuaFunction* value, int idx = -1);

	// 根据下标获取值
	bool argBool(int idx) const { return getValue(idx)->asBool(); }
	int argInt(int idx) const { return getValue(idx)->asInt(); }
	unsigned int argUInt(int idx) const { return getValue(idx)->asUnsignedInt(); }
	float argFloat(int idx) const { return getValue(idx)->asFloat(); }
	double argDouble(int idx) const { return getValue(idx)->asDouble(); }
	std::string argULongLong(int idx) const;
	std::string argString(int idx) const { return getValue(idx)->asString(); }
	thValue* argValue(int idx) const { return getValue(idx)->asValue(); }
	thDEC_Value* argValueDec(int idx) const { return (thDEC_Value*)getValue(idx); }
	thRef* argObject(int idx) const { return getValue(idx)->asObject(); }
	thLuaFunction* argLuaFunction(int idx) const { return getValue(idx)->asLuaFunction(); }

	// setval
	void setArgBool(int idx, bool value);
	void setArgInt(int idx, int value);
	void setArgUInt(int idx, unsigned int value);
	void setArgFloat(int idx, float value);
	void setArgDouble(int idx, double value);
	void setArgLongLong(int idx, const std::string& value);
	void setArgString(int idx, const std::string& value);

	// c++ lua操作
public:
	// 注册
	static bool registerToLua(lua_State* luas);

	// 读取lua单个值
	void readLuaValue(lua_State *luas, int lo, int idx = -1);
	// 读取lua table值
	void readLuaValueTable(lua_State *luas);

	// 返回value 到lua
	int retValueToLua(lua_State *luas, bool isTable=false);


	// lua 中调用特殊处理
public:
	// lua arg

	// 创建value createFromLua(arg...)
	static thValue* createFromLuaArg(void) { return nullptr; }
	// 创建value createFromTable(table)
	static thValue* createFromTable(void) { return nullptr; }

	// push lua arg  void push_luaArg(name, arg...)
	void push_lua(const std::string& name) {}
	// void pushArg_luaArg(arg...)
	void pushArg_lua(void) {}

	// 整体格式化到lua table getLuaTable(void)
	void getLuaTable(void) {}
	// 整体格式化到lcoal a,b,c,d,... getLuaArg(void)
	void getLuaArg(void) {}

protected:
	thValue(void) {}

public:
	MapHashValue _mapValue;
	bool _bSaveKeyStr;
	bool _isArray;
	bool _isChange;
};

typedef std::vector<thValue*> VectorTHValue;
typedef std::unordered_map <size_t, thValue*> MapKeyValue;

#endif // __THSCRIPT_VALUE_H__
