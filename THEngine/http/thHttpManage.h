#ifndef __TH_HTTPMANAGE_H__
#define __TH_HTTPMANAGE_H__

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


// 返回code
enum emCode
{
	code_none,
	code_error,
	code_timeOut,						// 超时
	code_ok = 200,						// ok
	code_404 = 404,
};

// 发起请求处理状态
enum emHttpState
{
	state_note,
	state_ok,							// 处理完成
	state_error,						// 出现错误
	state_loading,						// 处理中...
	state_refresh,						// 包裹刷新状态
	state_timeout,						// 包裹超时
};

class thValue;
class thLuaFunction;
struct lua_State;

typedef std::function<void(bool bSucceed, const std::string& tag, const std::string& info)> ccHttpCallbackH;
typedef std::function<void(int code, const std::string& tag, thValue* value)> ccHttpCallback;
typedef std::function<void(bool bOk)> ccHttpCallbackPlayQ;
typedef std::vector<ccHttpCallbackH*> VectorHttpCallback;
typedef std::vector<std::string> VectorString;

// http管理类
class thHttpManage
{
public:
	static bool registerToLua(lua_State* luas);

public:
	// 设置回调
	static void init(int timeoutForConnect, int timeoutForRead, const std::string& cookieFile = "", const ccHttpCallback& callback = nullptr);
	static void unInit(void);

	// 设置headers
	static void setHeaders(VectorString& headers);

	// 模拟对网站get消息
	static void websiteGet(const std::string& tag, const std::string& url, bool useHeaders = false, const ccHttpCallbackH& callback = nullptr);
	static void websiteGetEx(const std::string& tag, const std::string& url, bool useHeaders = false, const ccHttpCallback& callback = nullptr);

	// 模拟对网站post消息
	static void websitePost(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders = false, const ccHttpCallbackH& callback = nullptr);
	static void websitePostEx(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders = false, const ccHttpCallback& callback = nullptr);


	// 获取请求状态
public:
	// 获取
	static int getWebsiteState(const std::string& tag);
	// 设置状态
	static void setWebsiteState(const std::string& tag, int state);

	// lua
public:
	static void websiteGetExLua(const std::string& tag, const std::string& url, bool useHeaders, thLuaFunction* callback);
	static void websitePostExLua(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders, thLuaFunction* callback);


protected:
	// 解析数据
	static void _parsData(bool bSucceed, const std::string& tag, const std::string& info, const ccHttpCallback& callback = nullptr);


public:
	static VectorHttpCallback s_vhttpCallBack;
	static ccHttpCallback s_callback;
	static std::string s_buffer;
	static VectorString s_headers;

	// 包裹状态
	static std::unordered_map<std::string, int> s_mapTag;
};


#endif /*__TH_HTTPMANAGE_H__*/
