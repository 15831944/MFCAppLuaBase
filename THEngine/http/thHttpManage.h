#ifndef __TH_HTTPMANAGE_H__
#define __TH_HTTPMANAGE_H__

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


// ����code
enum emCode
{
	code_none,
	code_error,
	code_timeOut,						// ��ʱ
	code_ok = 200,						// ok
	code_404 = 404,
};

// ����������״̬
enum emHttpState
{
	state_note,
	state_ok,							// �������
	state_error,						// ���ִ���
	state_loading,						// ������...
	state_refresh,						// ����ˢ��״̬
	state_timeout,						// ������ʱ
};

class thValue;
class thLuaFunction;
struct lua_State;

typedef std::function<void(bool bSucceed, const std::string& tag, const std::string& info)> ccHttpCallbackH;
typedef std::function<void(int code, const std::string& tag, thValue* value)> ccHttpCallback;
typedef std::function<void(bool bOk)> ccHttpCallbackPlayQ;
typedef std::vector<ccHttpCallbackH*> VectorHttpCallback;
typedef std::vector<std::string> VectorString;

// http������
class thHttpManage
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// ���ûص�
	static void init(int timeoutForConnect, int timeoutForRead, const std::string& cookieFile = "", const ccHttpCallback& callback = nullptr);
	static void unInit(void);

	// ����headers
	static void setHeaders(VectorString& headers);

	// ģ�����վget��Ϣ
	static void websiteGet(const std::string& tag, const std::string& url, bool useHeaders = false, const ccHttpCallbackH& callback = nullptr);
	static void websiteGetEx(const std::string& tag, const std::string& url, bool useHeaders = false, const ccHttpCallback& callback = nullptr);

	// ģ�����վpost��Ϣ
	static void websitePost(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders = false, const ccHttpCallbackH& callback = nullptr);
	static void websitePostEx(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders = false, const ccHttpCallback& callback = nullptr);


	// ��ȡ����״̬
public:
	// ��ȡ
	static int getWebsiteState(const std::string& tag);
	// ����״̬
	static void setWebsiteState(const std::string& tag, int state);

	// lua
public:
	static void websiteGetExLua(const std::string& tag, const std::string& url, bool useHeaders, thLuaFunction* callback);
	static void websitePostExLua(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders, thLuaFunction* callback);


protected:
	// ��������
	static void _parsData(bool bSucceed, const std::string& tag, const std::string& info, const ccHttpCallback& callback = nullptr);


public:
	static VectorHttpCallback s_vhttpCallBack;
	static ccHttpCallback s_callback;
	static std::string s_buffer;
	static VectorString s_headers;

	// ����״̬
	static std::unordered_map<std::string, int> s_mapTag;
};


#endif /*__TH_HTTPMANAGE_H__*/
