/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <vector>
#include <assert.h>

 // new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

// safe
#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

/**
 * @addtogroup network
 * @{
 */

namespace network {

class HttpClient;
class HttpResponse;

typedef std::function<void(HttpClient* client, HttpResponse* response)> ccHttpRequestCallback;

/** 
 * Defines the object which users must packed for HttpClient::send(HttpRequest*) method.
 * Please refer to tests/test-cpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 * @since v2.0.2
 *
 * @lua NA
 */

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
// #ifdef DELETE
// #undef DELETE
// #endif
// #endif

class HttpRequest
{
public:
    /**
     * The HttpRequest type enum used in the HttpRequest::setRequestType.
     */
    enum class Type
    {
        GET = 0,
        POST,
        PUT,
        DELETE_,
        UNKNOWN,
    };
    
    /** 
     *  Constructor.
     *   Because HttpRequest object will be used between UI thread and network thread,
         requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
         new/retain/release still works, which means you need to release it manually
         Please refer to HttpRequestTest.cpp to find its usage.
     */
    HttpRequest()
    {
        _requestType = Type::UNKNOWN;
		_referenceCount = 1;
        _url.clear();
        _requestData.clear();
        _tag.clear();
        _pCallback = nullptr;
        _pUserData = nullptr;
    };
    
    /** Destructor. */
    virtual ~HttpRequest()
    {
    };

	void retain()
	{
		++_referenceCount;
	}

	void release()
	{
		--_referenceCount;
		if (_referenceCount == 0)
		{
			delete this;
		}
	}
    
    /** 
     * Override autorelease method to avoid developers to call it.
     * If this function was called, it would trigger assert in debug mode
     *
     * @return Ref* always return nullptr.
     */
            
    // setter/getters for properties
     
    /** 
     * Set request type of HttpRequest object before being sent,now it support the enum value of HttpRequest::Type.
     *
     * @param type the request type.
     */
    inline void setRequestType(Type type)
    {
        _requestType = type;
    };
    /** 
     * Get the request type of HttpRequest object.
     *
     * @return HttpRequest::Type.
     */
    inline Type getRequestType()
    {
        return _requestType;
    };
    
    /** 
     * Set the url address of HttpRequest object.
     * The url value could be like these: "http://httpbin.org/ip" or "https://httpbin.org/get"
     *
     * @param url the string pointer.
     */
    inline void setUrl(const char* url)
    {
        _url = url;
    };
    /** 
     * Get the url address of HttpRequest object.
     *
     * @return const char* the pointer of _url.
     */
    inline const char* getUrl()
    {
        return _url.c_str();
    };
    
    /** 
     * Set the request data of HttpRequest object.
     *
     * @param buffer the buffer of request data, it support binary data.
     * @param len    the size of request data.
     */
    inline void setRequestData(const char* buffer, size_t len)
    {
        _requestData.assign(buffer, buffer + len);
    };
    /** 
     * Get the request data pointer of HttpRequest object.
     *
     * @return char* the request data pointer.
     */
    inline char* getRequestData()
    {
        if(_requestData.size() != 0)
            return &(_requestData.front());

        return nullptr;
    }
    /** 
     * Get the size of request data
     *
     * @return ssize_t the size of request data
     */
    inline size_t getRequestDataSize()
    {
        return _requestData.size();
    }
    
    /** 
     * Set a string tag to identify your request.
     * This tag can be found in HttpResponse->getHttpRequest->getTag().
     *
     * @param tag the string pointer
     */
    inline void setTag(const char* tag)
    {
        _tag = tag;
    };
    /** 
     * Get the string tag to identify the request.
     * The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback.
     *
     * @return const char* the pointer of _tag
     */
    inline const char* getTag()
    {
        return _tag.c_str();
    };
    
    /**
     * Set user-customed data of HttpRequest object.
     * You can attach a customed data in each request, and get it back in response callback.
     * But you need to new/delete the data pointer manually.
     *
     * @param pUserData the string pointer
     */
    inline void setUserData(void* pUserData)
    {
        _pUserData = pUserData;
    };
    /** 
     * Get the user-customed data pointer which were pre-setted.
     * Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer.
     *
     * @return void* the pointer of user-customed data.
     */
    inline void* getUserData()
    {
        return _pUserData;
    };
    
    /**
     * Set the target and related callback selector.
     * When response come back, it would call (pTarget->*pSelector) to process something.
     *
     * @param pTarget the target object pointer.
     * @param pSelector the callback function.
     */
    
    /**
     * Set the target and related callback selector of HttpRequest object.
     * When response come back, we would call (pTarget->*pSelector) to process response data.
     *
     * @param pTarget the target object pointer.
     * @param pSelector the SEL_HttpResponse function.
     */
   
    /**
     * Set response callback function of HttpRequest object.
     * When response come back, we would call _pCallback to process response data.
     *
     * @param callback the ccHttpRequestCallback function.
     */
    inline void setResponseCallback(const ccHttpRequestCallback& callback)
    {
        _pCallback = callback;
    }
    
    /** 
     * Get the target of callback selector function, mainly used by HttpClient.
     *
     * @return Ref* the target of callback selector function
     */

    /**
     * This sub class is just for migration SEL_CallFuncND to SEL_HttpResponse,someday this way will be removed.
     * 
     * @lua NA
     */
    
    /** 
     * Get _prxy object by the _pSelector.
     *
     * @return _prxy the _prxy object
     */
    
    /**
     * Get ccHttpRequestCallback callback function.
     *
     * @return const ccHttpRequestCallback& ccHttpRequestCallback callback function.
     */
    inline const ccHttpRequestCallback& getCallback()
    {
        return _pCallback;
    }
    
    /** 
     * Set custom-defined headers.
     *
     * @param pHeaders the string vector of custom-defined headers.
     */
    inline void setHeaders(std::vector<std::string> pHeaders)
   	{
   		_headers=pHeaders;
   	}
   
    /** 
     * Get custom headers.
     *
     * @return std::vector<std::string> the string vector of custom-defined headers.
     */
   	inline std::vector<std::string> getHeaders()
   	{
   		return _headers;
   	}

protected:
    // properties
	unsigned int				_referenceCount;
    Type                        _requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
    std::string                 _url;            /// target url that this request is sent to
    std::vector<char>           _requestData;    /// used for POST
    std::string                 _tag;            /// user defined tag, to identify different requests in response callback
    ccHttpRequestCallback       _pCallback;      /// C++11 style callbacks
    void*                       _pUserData;      /// You can add your customed data here 
    std::vector<std::string>    _headers;		      /// custom http headers
};

}

// end group
/// @}

#endif //__HTTP_REQUEST_H__
