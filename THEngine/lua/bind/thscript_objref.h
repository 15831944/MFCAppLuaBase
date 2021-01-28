
#ifndef __THSCRIPT_OBJREF_H_
#define __THSCRIPT_OBJREF_H_

#include <string>

// ������Ҫע�ᵽ�ű��Ķ������
class thRef
{
public:
	// ref
	void retain(void);
	void release(void);

	// �²�ʵ��true return
	virtual bool _release(void) { return true; }

	// �Զ�����
	thRef* autorelease(void);

	// ��ȡ���ü���
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

// lua�ű�������
// ���ڱ���ű����� ��c++����
// ���಻�ܷ��ظ�lua����ʹ��
class thLuaFunction : public thRef
{
public:
	thLuaFunction(unsigned int luafuncHandler);
	// ����
	static thLuaFunction* create(unsigned int luafuncHandler);
	// ref
	bool _release(void);

	// ���� isClassPtr=true ���arg��Ϊָ�봫�룬����չ��Ϊ����
	void call(thValue* arg = nullptr, bool isClassPtr=false);
	int call_i(thValue* arg = nullptr, bool isClassPtr = false);
	std::string call_s(thValue* arg = nullptr, bool isClassPtr = false);
	thValue* call_v(thValue* arg = nullptr, bool isClassPtr = false);


	// ֱ�Ӹ������ֵ���
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
