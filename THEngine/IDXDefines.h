#pragma once

#include <windows.h>
#include <stdio.h>
#include <MMSystem.h>
#include <atlstr.h>
#include <assert.h>
#include <process.h>
//#include <string>
#include <vector>
#include <map>
#include <functional>

//#pragma warning (push)
#pragma warning(disable : 4996)	/* Ignore 4996 warning */

#ifndef TH_NOVTABLE
#if !defined(_DEBUG) /* Streamline code */
#define TH_NOVTABLE __declspec(novtable)
#else
#define TH_NOVTABLE
#endif /* !defined(_DEBUG) */
#endif /* TH_NOVTABLE */

#ifdef TH_DLL
#define TH_API  __declspec(dllexport)
#else
#define TH_API
#endif /*TH_DLL*/

#define TH_CALL  __stdcall

#define TH_INLINE __forceinline		/* inline */

// release
#define TH_SAFE_RELEASE(p) { if(p) {	\
	(p)->Release();					\
	(p) = NULL;	}					\
}
// delete
#define TH_SAFE_DELETE(p) { if(p) {	\
	delete (p);						\
	(p) = NULL; }					\
}
// delete array []
#define TH_SAFE_DELETE_ARY(p) { if(p) {\
	delete [] (p);					 \
	(p) = NULL;	}					 \
}
// Free memory
#define TH_SAFE_FREE(p) { if(p) {	\
	free(p);						\
	(p) = NULL; }					\
}
// 
#define TH_SAFE_DELETEOBJ(p){ if(p){ \
	DeleteObject(p);}				\
}

// 颜色调整
#define TH_ARGB(a,r,g,b)	((DWORD(a)<<24) | (DWORD(r)<<16) | (DWORD(g)<<8) | DWORD(b))
#define TH_GETA(col)		((col)>>24)
#define TH_GETR(col)		(((col)>>16) & 0xFF)
#define TH_GETG(col)		(((col)>>8) & 0xFF)
#define TH_GETB(col)		((col) & 0xFF)
#define TH_SETA(col,a)		(((col) & 0x00FFFFFF) | (DWORD(a)<<24))
#define TH_SETR(col,r)		(((col) & 0xFF00FFFF) | (DWORD(r)<<16))
#define TH_SETG(col,g)		(((col) & 0xFFFF00FF) | (DWORD(g)<<8))
#define TH_SETB(col,b)		(((col) & 0xFFFFFF00) | DWORD(b))

// 数组长度
#define ARRAY_COUNT(ar)		sizeof(ar) / sizeof(ar[0])


// 绑定函数 c++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
