#ifndef __THSCRIPT_BIND_H__
#define __THSCRIPT_BIND_H__

#include "thscript_dec.h"
#include "thscript_objref.h"
#include "thscript_luaBasicConversions.h"
#include "thscript_luaEngine.h"

#include "../lua/lua.h"
#include "../tolua/tolua++.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

#include <map>
#include <functional>


// 函数 暂时只支持到8个参数,想支持更多请自行扩展__callArg函数


// 全局函数注册
class thGlobalDef
{
public:
	// namespaceName如果填名字，调用的时候前面加上test --> test.func()
	// namespaceName当前只对函数有效
	thGlobalDef(lua_State* luas, const char* namespaceName="");
	~thGlobalDef(void) {}

	// 结束 - 注册结束需要调用
	void end(void);

	// 注册函数
	// *defarg默认参数个数 - 除了基本数据类型，其他class类型必须为指针传值*
	template<typename... Args>
	thGlobalDef& func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);
	// 注册函数 - 有返回值
	template<typename result_type, typename... Args>
	thGlobalDef& funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);

	// 函数是否存在
	bool isFunc(const char* funcName);


	// 注册常量, enum
	thGlobalDef& def(const char *name, int value);


protected:
	thGlobalDef(void) {}

private:
	lua_State* m_luas;
	std::string m_name;
	std::vector<luaL_Reg> m_vReg;
};

// 类注册
// 类必要条件
// 1.必须继承thRef类
// 2.必须要有static TClass::create 创建函数
template<typename TClass>
class thClassDef
{
public:
	thClassDef(lua_State* luas, const char* className, const char* classBase="thRef");
	~thClassDef(void) {}

	// 结束 - 注册结束需要调用
	void end(void);

	// 注册函数
	// *defarg默认参数个数 - 除了基本数据类型，其他class类型必须为指针传值*
	template<typename... Args>
	thClassDef& func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);
	// 注册函数 - 有返回值
	template<typename result_type, typename... Args>
	thClassDef& funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);


	// 根据lua动态参数类型 - 绑定的c++函数参数使用thValue --> ret func(thValue* arg)
	thClassDef& funcAuto(const char* funcName, const thDEC_FuncPtr& funcref);
	template<typename result_type>
	thClassDef& funcrAuto(const char* funcName, const thDEC_FuncPtr& funcref);

	// 外部实现解析函数
	// 格式 --> int func(lua_State* luas, TClass* obj, int argc)
	thClassDef& funcCall(const char* funcName, const thDEC_FuncPtr& funcref, bool isStatic=false);

	// 函数是否存在
	bool isFunc(const char* funcName);


	// 注册变量var
	template<typename VarType>
	thClassDef& var(const char* varName, VarType TClass::* pvar);
	//// 注册变量 - thLValue专用
	//thClassDef& varLValue(const char* varName, thLValue TClass::* pvar);

	// 注册常量, enum
	thClassDef&	def(const char *name, int value);

protected:
	thClassDef(void) {}

private:
	lua_State* m_luas;
	std::string m_name;
	std::vector<std::string> m_vFunc;
};


#include "thscript_bind.inl"

#endif // __THSCRIPT_BIND_H__
