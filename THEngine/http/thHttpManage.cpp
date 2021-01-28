#include "thHttpManage.h"
#include "HttpClient.h"

#include "../lua/bind/thscript_bind.h"

using namespace network;
VectorHttpCallback thHttpManage::s_vhttpCallBack;
ccHttpCallback thHttpManage::s_callback;
std::string thHttpManage::s_buffer;
VectorString thHttpManage::s_headers;

std::unordered_map<std::string, int> thHttpManage::s_mapTag;

static int aid = 0;
static std::string secret = "";

bool thHttpManage::registerToLua(lua_State* luas)
{
	thClassDef<thHttpManage>(luas, "thHttpManage")
		// as func
		.func<const std::string&, const std::string&, bool, thLuaFunction*>("websiteGetEx", thDF_FuncConv(thHttpManage::websiteGetExLua))
		.func<const std::string&, const std::string&, const std::string&, bool, thLuaFunction*>("websitePostEx", thDF_FuncConv(thHttpManage::websitePostExLua))
		.end();

	return true;
}

ccHttpCallbackH* _activeHttpCallback(const ccHttpCallbackH& callback)
{
	if (!thHttpManage::s_vhttpCallBack.empty())
	{
		auto cb = thHttpManage::s_vhttpCallBack.back();
		thHttpManage::s_vhttpCallBack.pop_back();
		*cb = callback;
		return cb;
	}
	auto cb = new ccHttpCallbackH;
	*cb = callback;
	return cb;
}

void _freeHttpCallback(ccHttpCallbackH* callback)
{
	thHttpManage::s_vhttpCallBack.push_back(callback);
}

void callback_httpRequestCompleted(HttpClient* client, HttpResponse* response)
{
	auto httpRequest = response->getHttpRequest();
	auto callback = (ccHttpCallbackH*)httpRequest->getUserData();
	//if (!callback) callback = &thHttpManage::s_callback;
	//else _freeHttpCallback(callback);
	if (callback) _freeHttpCallback(callback);

	// 请求失败
	if (!response->isSucceed())
	{
		if ((*callback)) (*callback)(false, httpRequest->getTag(), response->getErrorBuffer());
		return;
	}
	//	int codeIndex = response->getResponseCode();

		// 请求成功
	auto buffer = response->getResponseData();
	if ((*callback)) (*callback)(true, httpRequest->getTag(), std::string(buffer->data(), buffer->size()));
}


void thHttpManage::init(int timeoutForConnect, int timeoutForRead, const std::string& cookieFile/*=""*/, const ccHttpCallback& callback/*=nullptr*/)
{
	auto http = HttpClient::getInstance();
	http->setTimeoutForConnect(timeoutForConnect);
	http->setTimeoutForRead(timeoutForRead);
	if (!cookieFile.empty())
	{
		http->enableCookies(cookieFile.c_str());
	}
	s_callback = callback;
}

void thHttpManage::unInit(void)
{
}

void thHttpManage::setHeaders(VectorString& headers)
{
	s_headers = headers;
}

void thHttpManage::websiteGet(const std::string& tag, const std::string& url, bool useHeaders/*=false*/, const ccHttpCallbackH& callback /*= nullptr*/)
{
	// 网络异步连接
	network::HttpRequest* postRequest = new network::HttpRequest();
	postRequest->setTag(tag.c_str());
	// 设置发送类型
	postRequest->setRequestType(network::HttpRequest::Type::GET);
	// 回调函数，处理接收到的信息
	postRequest->setResponseCallback(callback_httpRequestCompleted);

	// headers
// 	std::vector<std::string> vHeaders;
// 	vHeaders.push_back("User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko");
	if (useHeaders) postRequest->setHeaders(s_headers);

	// 设置网址
	postRequest->setUrl(url.c_str());

	// call
	if (callback)
	{
		auto httpCallback = _activeHttpCallback(callback);
		postRequest->setUserData(httpCallback);
	}

	// 设置接收数据类型
	network::HttpClient::getInstance()->send(postRequest);
	// 释放
	postRequest->release();
}

void thHttpManage::websiteGetEx(const std::string& tag, const std::string& url, bool useHeaders, const ccHttpCallback& callback)
{
	if (tag.empty())
	{
		return;
	}
	// 检测包裹状态
	auto st = getWebsiteState(tag);
	if (st == state_loading) return;
	setWebsiteState(tag, state_loading);
	// get
	websiteGet(tag, url, useHeaders, [=](bool bSucceed, const std::string& tag, const std::string& info)
	{
		_parsData(bSucceed, tag, info, callback);
	});
}

void thHttpManage::websitePost(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders /*= false*/, const ccHttpCallbackH& callback/*=nullptr*/)
{
	// 网络异步连接
	network::HttpRequest* postRequest = new network::HttpRequest();
	postRequest->setTag(tag.c_str());
	// 设置发送类型
	postRequest->setRequestType(network::HttpRequest::Type::POST);
	// 回调函数，处理接收到的信息
	postRequest->setResponseCallback(callback_httpRequestCompleted);

	// headers
	if (useHeaders) postRequest->setHeaders(s_headers);

	// 设置网址
	postRequest->setUrl(url.c_str());
	if (!postData.empty()) postRequest->setRequestData(postData.c_str(), postData.length());

	// call
	if (callback)
	{
		auto httpCallback = _activeHttpCallback(callback);
		postRequest->setUserData(httpCallback);
	}

	// 设置接收数据类型
	network::HttpClient::getInstance()->send(postRequest);
	// 释放
	postRequest->release();
}

void thHttpManage::websitePostEx(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders, const ccHttpCallback& callback)
{
	if (tag.empty())
	{
		return;
	}
	// 检测包裹状态
	auto st = getWebsiteState(tag);
	if (st == state_loading) return;
	setWebsiteState(tag, state_loading);
	// post
	websitePost(tag, url, postData, useHeaders, [=](bool bSucceed, const std::string& tag, const std::string& info)
	{
		_parsData(bSucceed, tag, info, callback);
	});
}

int thHttpManage::getWebsiteState(const std::string& tag)
{
	auto iter = s_mapTag.find(tag);
	if (iter != s_mapTag.end())
	{
		return iter->second;
	}
	return state_note;
}

void thHttpManage::setWebsiteState(const std::string& tag, int state)
{
	if (tag.empty())
	{
		return;
	}
	s_mapTag[tag] = state;
}

void thHttpManage::websiteGetExLua(const std::string& tag, const std::string& url, bool useHeaders, thLuaFunction* callback)
{
	if (callback) callback->retain();
	websiteGetEx(tag, url, useHeaders, [=](int code, const std::string& tag, thValue* value)
	{
		if (callback)
		{
			callback->call(value, true);
			callback->release();
		}
	});
}

void thHttpManage::websitePostExLua(const std::string& tag, const std::string& url, const std::string& postData, bool useHeaders, thLuaFunction* callback)
{
	if (callback) callback->retain();
	websitePostEx(tag, url, postData, useHeaders, [=](int code, const std::string& tag, thValue* value)
	{
		if (callback)
		{
			callback->call(value, true);
			callback->release();
		}
	});
}

void thHttpManage::_parsData(bool bSucceed, const std::string& tag, const std::string& info, const ccHttpCallback& callback /*= nullptr*/)
{
	if (bSucceed)
	{
		setWebsiteState(tag, state_ok);
		auto par = thValue::create();
		par->push_int("code", code_ok);
		par->push_string("data", info);
		if (callback)
		{
			callback(code_timeOut, tag, par);
		}
	}
	else
	{
		int code = code_error;
		if (int(info.find("Operation timed out after")) != -1
			|| int(info.find("Connection timed out after")) != -1
			|| int(info.find("Failed to connect to")) != -1)
		{
			code = code_timeOut;
			setWebsiteState(tag, state_timeout);
		}
		else
		{
			setWebsiteState(tag, state_error);
		}
		if (callback)
		{
			callback(code, tag, nullptr);
		}
	}
}