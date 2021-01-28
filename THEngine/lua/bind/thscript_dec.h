#ifndef __THSCRIPT_DEC_H__
#define __THSCRIPT_DEC_H__
#include <stdint.h>
#include <string>
#include <assert.h>
#include <stdarg.h>
#include "thscript_value.h"

#define __thDF_API
#define __thDF_inline inline

// ref C++11
#include <atomic>
#define __thDF_refDef						mutable std::atomic<uint32_t>

// arg conv
#define __thDF_Arg_int64(t,b)				*((t *)((int64_t*)&b))
#define __thDF_Arg_uint64(t,b)				*((t *)((uint64_t*)&b))

// ref
#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)

// log
#ifdef _DEBUG
void __print(const char * format, ...);
#define __thDF_LOG(format, ...)		__print(format, ##__VA_ARGS__)
#define __thDF_assert(cond, msg) do {                              \
          if (!(cond)) {                                          \
              __print("Assert failed: %s", msg);				\
			  assert(cond);                                      \
          } \
        } while (0)
#else
#define __thDF_LOG(...)			do {} while (0)
#define __thDF_assert(cond, msg)
#endif


// 函数注册 begin


// 函数类型
enum thEM_FuncFlag
{
	thFlag_UnknownFunc = 0,			// 未知函数	
	thFlag_GlabalFunc = 1,			// 全局函数
	thFlag_MethodFunc = 2			// 类函数
};

// 解析:
// thDF_FuncConv		全局或static函数指针
// func  -- 函数
#define thDF_FuncConv							thFC_ConvFuncPtr
#define thDF_FuncConvEx(func,par,ret)			thFC_ConvFuncPtr((void (*)())(static_cast<ret (*)par>(func)))

// 解析：
// thDF_CFuncConv		类函数指针
// cls   -- class 类
// func  -- 类函数
#define thDF_CFuncConv(cls,func)				thDEC_ClassFuncPtr<sizeof(void (cls::*)())>::convert((void (cls::*)())(&cls::func))
// 可填入具体参数，多态函数可使用到
#define thDF_CFuncConvEx(cls,func,par,ret)		thDEC_ClassFuncPtr<sizeof(void (cls::*)())>::convert(static_cast<ret (cls::*)par>(&cls::func))


// 函数注册 end


// 函数辅助结构 begin

// 全局函数
typedef void(*thTF_Function)(void);

// 声明一个伪类,这样我们可以确定一个简单的方法指针的大小
class thUnknownClass {};
typedef void (thUnknownClass::*thTF_ClassFunc)();
const int32_t TH_SINGLE_PTR_SIZE = sizeof(thTF_ClassFunc);

// 函数指针
struct thDEC_FuncPtr
{
	thDEC_FuncPtr(thEM_FuncFlag fg)
		: flag(fg)
	{
		for(size_t i = 0; i < sizeof(ptr.dummy); ++i)
			ptr.dummy[i] = 0;
	}

	void copyFuncPtr(const void *mthdPtr, size_t size)
	{
		for(size_t i = 0; i < size; ++i)
			ptr.dummy[i] = reinterpret_cast<const char *>(mthdPtr)[i];
	}

	union
	{
		// 最大的已知的方法点是20个字节（MSVC64位），
		// 但与8字节对齐，这成为24个字节。因此，我们需要
		// 能够存储至少那么多。
		char  dummy[25]; 
		struct {thTF_ClassFunc  func; char dummy[25-sizeof(thTF_ClassFunc)];} cfunc;
		struct {thTF_Function	func; char dummy[25-sizeof(thTF_Function)];}  gfunc;
	}ptr;

	// 函数类型
	uint8_t flag; // 0 = 全局函数, 1 = 类函数
};

// 全局函数定义
// 模板函数来捕捉所有全局函数
template <class FUNC>
__thDF_inline thDEC_FuncPtr thFC_ConvFuncPtr(FUNC func)
{
	thDEC_FuncPtr __func_ptr(thFlag_GlabalFunc);

#ifdef __thDF_win64
	// The size_t cast is to avoid a compiler warning with thTF_Function(0) 
	// on 64bit, as 0 is interpreted as a 32bit TH::int32 value
	__func_ptr.ptr.gfunc.func = reinterpret_cast<thTF_Function>(size_t(func));
#else
	// MSVC6 doesn't like the size_t cast above so I
	// solved this with a separate code for 32bit.
	__func_ptr.ptr.gfunc.func = reinterpret_cast<thTF_Function>(func);
#endif // __thDF_win64

	return __func_ptr;
}


// 类函数 模板定义

// 定义默认模板 
// 未知函数大小 不应该被使用到
template <int32_t FuncSize>
struct thDEC_ClassFuncPtr
{
	template<class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		// This version of the function should never be executed, nor compiled,
		// as it would mean that the size of the method pointer cannot be determined.

		int32_t ERROR_UnsupportedMethodPtr[FuncSize-100];

		thDEC_FuncPtr __func_ptr(thFlag_UnknownFunc);
		return __func_ptr;
	}
};

// Template specialization
template <>
struct thDEC_ClassFuncPtr<TH_SINGLE_PTR_SIZE>
{
	template<class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		thDEC_FuncPtr __func_ptr(thFlag_MethodFunc);
		__func_ptr.copyFuncPtr(&Mthd, TH_SINGLE_PTR_SIZE);
		return __func_ptr;
	}
};

#if defined(_MSC_VER) && !defined(__MWERKS__)

// MSVC and Intel uses different sizes for different class method pointers
template <>
struct thDEC_ClassFuncPtr<TH_SINGLE_PTR_SIZE+1*sizeof(int32_t)>
{
	template <class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		thDEC_FuncPtr __func_ptr(thFlag_MethodFunc);
		__func_ptr.copyFuncPtr(&Mthd, TH_SINGLE_PTR_SIZE+sizeof(int32_t));
		return __func_ptr;
	}
};

template <>
struct thDEC_ClassFuncPtr<TH_SINGLE_PTR_SIZE+2*sizeof(int32_t)>
{
	template <class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		// On 32bit platforms with is where a class with virtual inheritance falls.
		// On 64bit platforms we can also fall here if 8uint8_t data alignments is used.

		thDEC_FuncPtr __func_ptr(thFlag_MethodFunc);
		__func_ptr.copyFuncPtr(&Mthd, TH_SINGLE_PTR_SIZE+2*sizeof(int32_t));

		// Microsoft has a terrible optimization on class methods with virtual inheritance.
		// They are hardcoding an important offset, which is not coming in the method pointer.

#if defined(_MSC_VER) && !defined(__thDF_win64)
		// Method pointers for virtual inheritance is not supported,
		// as it requires the location of the vbase table, which is 
		// only available to the C++ compiler, but not in the method
		// pointer. 

		// You can get around this by forward declaring the class and
		// storing the sizeof its method pointer in a constant. Example:

		// class ClassWithVirtualInheritance;
		// const TH::int32 ClassWithVirtualInheritance_workaround = sizeof(void ClassWithVirtualInheritance::*());

		// This will force the compiler to use the unknown type
		// for the class, which falls under the next case


		// Copy the virtual table index to the 4th dword so that AngelScript
		// can properly detect and deny the use of methods with virtual inheritance.
		*(reinterpret_cast<uint32_t*>(&p)+3) = *(reinterpret_cast<uint32_t*>(&__func_ptr)+2);
#endif // defined(_MSC_VER) && !defined(__thDF_win64)

		return __func_ptr;
	}
};

template <>
struct thDEC_ClassFuncPtr<TH_SINGLE_PTR_SIZE+3*sizeof(int32_t)>
{
	template <class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		thDEC_FuncPtr __func_ptr(thFlag_MethodFunc);
		__func_ptr.copyFuncPtr(&Mthd, TH_SINGLE_PTR_SIZE+3*sizeof(int32_t));
		return __func_ptr;
	}
};

template <>
struct thDEC_ClassFuncPtr<TH_SINGLE_PTR_SIZE+4*sizeof(int32_t)>
{
	template <class FUNC>
	static thDEC_FuncPtr convert(FUNC Mthd)
	{
		// On 64bit platforms with 8uint8_t data alignment
		// the unknown class method pointers will come here.

		thDEC_FuncPtr __func_ptr(thFlag_MethodFunc);
		__func_ptr.copyFuncPtr(&Mthd, TH_SINGLE_PTR_SIZE+4*sizeof(int32_t));
		return __func_ptr;
	}
};

#endif // defined(_MSC_VER) && !defined(__MWERKS__)

// conv val obj
template<typename _Ty>
struct thConvType
{
	static size_t size(void)
	{
		return sizeof(_Ty);
	}
	static _Ty conv(void* ptr)
	{
		return *(_Ty*)ptr;
	}
	static _Ty convValue(thDEC_Value* ptr)
	{
		return *(_Ty*)ptr->asValuePtr<_Ty>();
	}
	static void convToPtr(void* toptr, _Ty in)
	{
		new(toptr) _Ty(in);
	}
	static void convToVal(uint64_t& toval, _Ty in)
	{
		__thDF_Arg_uint64(_Ty, toval) = in;
	}
};

template<class _Ty>
struct thConvType<_Ty&>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static _Ty& conv(void* ptr)
	{
		return *(_Ty*)(ptr);
	}
	static _Ty& convValue(thDEC_Value* ptr)
	{
		return *(_Ty*)(ptr->asValuePtr<_Ty>());
	}
	static void convToPtr(void* toptr, _Ty& in)
	{
		(*(size_t*)toptr) = size_t((void*)&in);
	}
	static void convToVal(uint64_t& toval, _Ty& in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t((void*)&in);
	}
};

template<class _Ty>
struct thConvType<const _Ty&>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static const _Ty& conv(void* ptr)
	{
		return *(_Ty*)(ptr);
	}
	static const _Ty& convValue(thDEC_Value* ptr)
	{
		return *(_Ty*)(ptr->asValuePtr<_Ty>());
	}
	static void convToPtr(void* toptr, const _Ty& in)
	{
		(*(size_t*)toptr) = size_t((void*)&in);
	}
	static void convToVal(uint64_t& toval, const _Ty& in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t((void*)&in);
	}
};

template<class _Ty>
struct thConvType<_Ty&&>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static _Ty&& conv(void* ptr)
	{
		return *(_Ty*)(ptr);
	}
	static _Ty&& convValue(thDEC_Value* ptr)
	{
		return *(_Ty*)(ptr->asValuePtr<_Ty>());
	}
	static void convToPtr(void* toptr, _Ty&& in)
	{
		(*(size_t*)toptr) = size_t(in);
	}
	static void convToVal(uint64_t& toval, _Ty&& in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t(in);
	}
};

template<class _Ty>
struct thConvType<_Ty*>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static _Ty* conv(void* ptr)
	{
		return (_Ty*)(ptr);
	}
	static _Ty* convValue(thDEC_Value* ptr)
	{
		return (_Ty*)(ptr->asValuePtr<_Ty>());
	}
	static void convToPtr(void* toptr, _Ty* in)
	{
		(*(size_t*)toptr) = size_t(in);
	}
	static void convToVal(uint64_t& toval, _Ty* in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t(in);
	}
};

template<class _Ty>
struct thConvType<const _Ty*>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static const _Ty* conv(void* ptr)
	{
		return (_Ty*)(ptr);
	}
	static const _Ty* convValue(thDEC_Value* ptr)
	{
		return (_Ty*)(ptr->asValuePtr<_Ty>());
	}
	static void convToPtr(void* toptr, const _Ty* in)
	{
		(*(size_t*)toptr) = size_t(in);
	}
	static void convToVal(uint64_t& toval, const _Ty* in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t(in);
	}
};

template<class _Ty>
struct thConvType<_Ty**>
{
	static size_t size(void)
	{
		return sizeof(size_t);
	}
	static _Ty** conv(void* ptr)
	{
		return (_Ty**)(ptr);
	}
	static _Ty** convValue(thDEC_Value* ptr)
	{
		return (_Ty**)(ptr->asValuePtr2<_Ty>());
	}
	static void convToPtr(void* toptr, _Ty** in)
	{
		(*(size_t*)toptr) = size_t(in);
	}
	static void convToVal(uint64_t& toval, _Ty** in)
	{
		__thDF_Arg_uint64(size_t, toval) = size_t(in);
	}
};

//// 整型
//#define thConvType_define(__TYPE__)\
//template<>\
//struct thConvType<__TYPE__>\
//{\
//	static size_t size(void)\
//	{\
//		return sizeof(__TYPE__);\
//	}\
//	static __TYPE__ conv(void* ptr)\
//	{\
//		return *(__TYPE__*)ptr;\
//	}\
//	static __TYPE__ convValue(thLValue& ptr)\
//	{\
//		return *(__TYPE__*)ptr.asValuePtr<__TYPE__>();\
//	}\
//	static void convToPtr(void* toptr, __TYPE__ in)\
//	{\
//		*(__TYPE__*)toptr = in;\
//	}\
//	static void convToVal(uint64_t& toval, __TYPE__ in)\
//	{\
//		__thDF_Arg_uint64(__TYPE__, toval) = in;\
//	}\
//};
//thConvType_define(bool)
//thConvType_define(char)
//thConvType_define(uint8_t)
//thConvType_define(int16_t)
//thConvType_define(uint16_t)
//thConvType_define(int32_t)
//thConvType_define(int64_t)
//thConvType_define(uint32_t)
//thConvType_define(uint64_t)
//thConvType_define(float)
//thConvType_define(double)
//thConvType_define(std::string)
//thConvType_define(thRef*)
//thConvType_define(thLuaFunction*)



// 函数辅助结构 end

#endif // __THSCRIPT_DEC_H__
