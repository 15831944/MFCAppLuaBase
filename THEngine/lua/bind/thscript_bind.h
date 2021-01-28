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


// ���� ��ʱֻ֧�ֵ�8������,��֧�ָ�����������չ__callArg����


// ȫ�ֺ���ע��
class thGlobalDef
{
public:
	// namespaceName��������֣����õ�ʱ��ǰ�����test --> test.func()
	// namespaceName��ǰֻ�Ժ�����Ч
	thGlobalDef(lua_State* luas, const char* namespaceName="");
	~thGlobalDef(void) {}

	// ���� - ע�������Ҫ����
	void end(void);

	// ע�ắ��
	// *defargĬ�ϲ������� - ���˻����������ͣ�����class���ͱ���Ϊָ�봫ֵ*
	template<typename... Args>
	thGlobalDef& func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);
	// ע�ắ�� - �з���ֵ
	template<typename result_type, typename... Args>
	thGlobalDef& funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);

	// �����Ƿ����
	bool isFunc(const char* funcName);


	// ע�᳣��, enum
	thGlobalDef& def(const char *name, int value);


protected:
	thGlobalDef(void) {}

private:
	lua_State* m_luas;
	std::string m_name;
	std::vector<luaL_Reg> m_vReg;
};

// ��ע��
// ���Ҫ����
// 1.����̳�thRef��
// 2.����Ҫ��static TClass::create ��������
template<typename TClass>
class thClassDef
{
public:
	thClassDef(lua_State* luas, const char* className, const char* classBase="thRef");
	~thClassDef(void) {}

	// ���� - ע�������Ҫ����
	void end(void);

	// ע�ắ��
	// *defargĬ�ϲ������� - ���˻����������ͣ�����class���ͱ���Ϊָ�봫ֵ*
	template<typename... Args>
	thClassDef& func(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);
	// ע�ắ�� - �з���ֵ
	template<typename result_type, typename... Args>
	thClassDef& funcr(const char* funcName, const thDEC_FuncPtr& funcref, int defarg=0, ...);


	// ����lua��̬�������� - �󶨵�c++��������ʹ��thValue --> ret func(thValue* arg)
	thClassDef& funcAuto(const char* funcName, const thDEC_FuncPtr& funcref);
	template<typename result_type>
	thClassDef& funcrAuto(const char* funcName, const thDEC_FuncPtr& funcref);

	// �ⲿʵ�ֽ�������
	// ��ʽ --> int func(lua_State* luas, TClass* obj, int argc)
	thClassDef& funcCall(const char* funcName, const thDEC_FuncPtr& funcref, bool isStatic=false);

	// �����Ƿ����
	bool isFunc(const char* funcName);


	// ע�����var
	template<typename VarType>
	thClassDef& var(const char* varName, VarType TClass::* pvar);
	//// ע����� - thLValueר��
	//thClassDef& varLValue(const char* varName, thLValue TClass::* pvar);

	// ע�᳣��, enum
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
