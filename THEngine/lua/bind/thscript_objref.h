
#ifndef __THSCRIPT_OBJREF_H_
#define __THSCRIPT_OBJREF_H_

#include <string>

// 所有需要注册到脚本的对象基类
class thRef
{
public:
	// ref
	void retain(void);
	void release(void);

	// 下层实现true return
	virtual bool _release(void) { return true; }

	// 自动回收
	thRef* autorelease(void);

	// 获取引用计数
	unsigned int getReferenceCount(void) const;

protected:
	thRef(void);

public:
	virtual ~thRef(void);

protected:
	/// count of references
	unsigned int _referenceCount;

public:
	/// object id, ScriptSupport need public _ID
	unsigned int  _ID;
	/// Lua reference id
	int          _luaID;
};

class thValue;

// lua脚本函数类
// 用于保存脚本函数 供c++调用
// 该类不能返回给lua里面使用
class thLuaFunction : public thRef
{
public:
	thLuaFunction(unsigned int luafuncHandler);
	// 创建
	static thLuaFunction* create(unsigned int luafuncHandler);
	// ref
	bool _release(void);

	// 调用 isClassPtr=true 会把arg作为指针传入，否则展开为参数
	void call(thValue* arg = nullptr, bool isClassPtr=false);
	int call_i(thValue* arg = nullptr, bool isClassPtr = false);
	std::string call_s(thValue* arg = nullptr, bool isClassPtr = false);
	thValue* call_v(thValue* arg = nullptr, bool isClassPtr = false);


	// 直接根据名字调用
	static void call(const std::string& funcName, thValue* arg = nullptr, bool isClassPtr = false);
	static int call_i(const std::string& funcName, thValue* arg = nullptr, bool isClassPtr = false);
	static std::string call_s(const std::string& funcName, thValue* arg = nullptr, bool isClassPtr = false);
	static thValue* call_v(const std::string& funcName, thValue* arg = nullptr, bool isClassPtr = false);


protected:
	thLuaFunction(void);

public:
	virtual ~thLuaFunction(void);


protected:
	/// lua function handler
	unsigned int _luafuncHandler;
};


#endif // __THSCRIPT_OBJREF_H_
