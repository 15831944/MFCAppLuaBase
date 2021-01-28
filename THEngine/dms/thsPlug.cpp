#include "stdafx.h"
#include "atlbase.h"
#include "thsPlug.h"
#include "../thHelper.h"

#define DEBUG_CHECK_S(i,s) CString s1,s2,s3,s4; int s##fs = 0;int d=i;m_strTemp.Format("%d",d);\
while (s##fs<d){s##fs++;if(s##fs>10)break;}s1.LoadString(IDS_STRINGUD);s2.LoadString(IDS_STRINGFD);s3.LoadString(IDS_STRINGID);s4.LoadString(IDS_STRINGMD);\
while(true){break;}\
spDisp.GetIDOfName(thHelper::ToUNICODE(fp_wj_ripwd(thHelper::Base64DecodeStr(s1), m_strTemp, thHelper::MakeTempPath(thHelper::Base64DecodeStr(s3)), thHelper::Base64DecodeStr(s4))),&dispatch_id);

// #define DEBUG_CHECK_S(i,s) CString fc = __FUNCTION__;thHelper::analyString(fc, "::"); CString s1,s2,s3,s4; int s##fs = 0;int d=i;\
// while (s##fs<d){s##fs++;if(s##fs>10)break;}s1.LoadString(IDS_STRINGUD);s2.LoadString(IDS_STRINGFD);s3.LoadString(IDS_STRINGID);s4.LoadString(IDS_STRINGMD);\
// while(true){break;}\
// spDisp.GetIDOfName(thHelper::ToUNICODE(fp_wj_ripwd(thHelper::Base64DecodeStr(s1), fc, thHelper::MakeTempPath(thHelper::Base64DecodeStr(s3)), thHelper::Base64DecodeStr(s4))),&dispatch_id);

class MyDispatchDriver
{
public:
    IDispatch* p;
    MyDispatchDriver()
    {
        p = NULL;
    }
    MyDispatchDriver(IDispatch* lp)
    {
        if ((p = lp) != NULL)
            p->AddRef();
    }
    ~MyDispatchDriver() { if (p) p->Release(); }
    HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
    {
        HRESULT hr = -1;
        if (p == NULL) return hr;
        return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
    }
    HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPPARAMS dispparams = { NULL, NULL, 0, 0};
        if (p == NULL) return hr;
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
        if (p == NULL) return hr;
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
};

thsPlug* thsPlug::s_ths = nullptr;
CString thsPlug::s_sp;

thsPlug::thsPlug()
{
	this->init(s_sp);
}

thsPlug::thsPlug(bool bIsPtr)
{

}

thsPlug::~thsPlug()
{
   if (obj) obj->Release();
}

bool thsPlug::init(const CString& sft)
{
	CLSID clsid;
	IUnknown * pUnknown = NULL;
	HRESULT hr;

	obj = NULL;
	hr = ::CLSIDFromProgID(thHelper::ToUNICODE(sft.GetString()), &clsid);
	if (FAILED(hr))
	{
		return false;
	}

	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&pUnknown);
	if (FAILED(hr))
	{
		return false;
	}

	pUnknown->QueryInterface(IID_IDispatch, (void **)&obj);
	if (pUnknown) pUnknown->Release();
	return true;
}

thsPlug* thsPlug::createThs(const CString& sft, const CString& sftdl, const CString& sftpar)
{
	if (s_ths)
	{
		return s_ths;
	}

	// 注册调用
	auto regidl = [=]()->bool
	{
		LRESULT(CALLBACK* lpDllEntryPoint)();
		HINSTANCE hLib = LoadLibrary(sftdl);
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			return FALSE;
		}
		(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
		BOOL bRet = FALSE;
		if (lpDllEntryPoint != NULL)
		{
			HRESULT hr = (*lpDllEntryPoint)();
			bRet = SUCCEEDED(hr);
			if (FAILED(hr))
			{
				// MessageBoxA(nullptr, "注册失败！", "", 0);
			}
		}
		FreeLibrary(hLib);
		return bRet;
	};
	auto regidlto = [=]()->bool
	{
		//CString sftpar = thHelper::MakeTempPath("360\\wmp3\\tmr.df");
		//CString sftdl = thHelper::MakeTempPath("360\\wmp3\\tm.df");
		HMODULE hmod = LoadLibrary(sftpar);//加载dll 参数DLL路径
		DWORD dlladdr = (DWORD)GetProcAddress(hmod, thHelper::Base64DecodeStr("U2V0RGxsUGF0aEE="));
		DWORD dmdlladdr = (DWORD)(char*)sftdl.GetString();
		_asm
		{
			push 0;
			push dmdlladdr;
			call dlladdr;
		}
		auto s = FreeLibrary(hmod);
		return true;
	};

	if (sftpar.IsEmpty())
	{
		CLSID clsid;
		auto hr = ::CLSIDFromProgID(thHelper::ToUNICODE(sft.GetString()), &clsid);
		if (FAILED(hr))
		{
			if (!regidl())
			{
				return nullptr;
			}
		}
	}
	else
	{
		if (!regidlto())
		{
			return nullptr;
		}
	}
	s_sp = sft;
	s_ths = new thsPlug(true);
	if (s_ths->init(sft))
	{
		return s_ths;
	}
	delete s_ths;
	s_ths = nullptr;
	return nullptr;
}

void thsPlug::releaseThs(void)
{
	if (s_ths)
	{
		delete s_ths;
	}
	s_ths = nullptr;
}

long thsPlug::fp_wz_srgnd(long row_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(row_gap);
	
    if (dispatch_id == -1)
    {
		DEBUG_CHECK_S(1, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_fh_abe(LONGLONG base_addr,long is_64bit)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(is_64bit);

    if (dispatch_id == -1)
    {
		DEBUG_CHECK_S(2, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
		auto r = CString(vResult.bstrVal);
        return  r;
    }
    return  _T("");
}

long thsPlug::fp_ht_ems(long en,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
		DEBUG_CHECK_S(3, e);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ht_lmrt(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(4, e);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_sf_spde(const TCHAR * all_pos,long tpe,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(5, sf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_gnts()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
       DEBUG_CHECK_S(6, g);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_ht_dcu(long rate)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(rate);

    if (dispatch_id == -1)
    {
       DEBUG_CHECK_S(7, g);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ht_ubw()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(8, d);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_zx_ici()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(9, s);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_zx_cimd(long hwnd,const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(10, f);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

double thsPlug::fp_nc_rd(long hwnd,const TCHAR * addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(11, alf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

long thsPlug::fp_ht_sddy(long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(12, alf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_mf(const TCHAR * src_file,const TCHAR * dst_file)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(src_file);
    pn[0] = COleVariant(dst_file);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(13, alf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gwpp(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(14, mf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_nc_vqex(long hwnd,LONGLONG addr,long pmbi)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(pmbi);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(15, mf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_ws(long hwnd,const TCHAR * addr,long tpe,const TCHAR * v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(16, mf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gws(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(17, mf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_fcff(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(18, mf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_play(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(19, psy);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_fcs(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(20, psy);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_dt_fsd(const TCHAR * server,long handle,long request_type,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(21, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_dt_fmcex(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(22, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_jb_sewd(long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(23, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ss(long width,long height,long depth)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(width);
    pn[1] = COleVariant(height);
    pn[0] = COleVariant(depth);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(24, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wz_gdt(long index,long font_index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(font_index);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(25, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_ts_fpme(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(26, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_dfe(const TCHAR * url,const TCHAR * save_file,long timeout)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(url);
    pn[1] = COleVariant(save_file);
    pn[0] = COleVariant(timeout);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(27, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_delay(long mis)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mis);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(28, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ts_egcbc(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(29, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_ffrt(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(30, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_eos(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(31, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_ftd(float float_value)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(float_value);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(32, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_ts_brtrb(const TCHAR * bgr_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(bgr_color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(33, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_wdfb(long hwnd,const TCHAR * addr,long data,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(data);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(34, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_rdd(long hwnd,const TCHAR * addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(35, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_ftr(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(36, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_rdatb(long hwnd,LONGLONG addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(37, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wz_fsr(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(38, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wz_seor(long exact_ocr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(exact_ocr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(39, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_gfl(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(40, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wz_fsrs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(41, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_ht_ekp(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(42, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

double thsPlug::fp_nc_rda(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(43, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

long thsPlug::fp_fb_fbuk(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(44, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wz_gwnd(long x1,long y1,long x2,long y2,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(45, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_ht_ekm(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(46, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ts_lpb(long addr,long size,const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(47, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ts_ser(long tpe,const TCHAR * info)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(info);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(48, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wde(long hwnd,const TCHAR * addr,double v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(49, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fwbps(const TCHAR * process_name,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(process_name);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(50, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_ef(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(51, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ht_smdy(const TCHAR * tpe,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(52, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_smhapid(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(53, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ht_wdn()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(54, gta);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_sbw(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(55, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_cfcm(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic);
    pn[1] = COleVariant(trans_color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(56, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_dps()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(57, gta);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_delays(long min_s,long max_s)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(min_s);
    pn[0] = COleVariant(max_s);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(58, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_cdt(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(59, gta);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gsht()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(60, usfgot);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gwrps(const TCHAR * str,long index,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(61, usfgot);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fw(const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(62, usfgot);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gwrc(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(63, usfgot);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_df(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(64, usfgot);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_ssim2(long hwnd,const TCHAR * str,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(str);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(65, usfgot);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_isbd(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(66, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_cpe(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(67, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG thsPlug::fp_nc_ri(long hwnd,const TCHAR * addr,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(68, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_wd_gfww()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(69, s);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_swgnd(long word_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(word_gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(70, f);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fsexs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(71, df);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_nc_fi(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr_range);
    pn[2] = COleVariant(int_value_min);
    pn[1] = COleVariant(int_value_max);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(72, sd);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_cts(long hwnd,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(73, df);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_efae(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(74, df);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wda(long hwnd,LONGLONG addr,double v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(75, fd);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gcl(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(76, a);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_fcb(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[11];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[10] = COleVariant(x1);
    pn[9] = COleVariant(y1);
    pn[8] = COleVariant(x2);
    pn[7] = COleVariant(y2);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(count);
    pn[3] = COleVariant(width);
    pn[2] = COleVariant(height);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(77, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,11,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_kps(const TCHAR * key_str,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(key_str);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(78, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_sdpDelay(long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(79, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_ssim(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(80, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fd_xt_esdx(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(81, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ldy(long locks)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(locks);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(82, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_grps(const TCHAR * str,long index,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(83, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_fsft(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(84, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_itd(LONGLONG int_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(int_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(85, sp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_dtd(double double_value)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(double_value);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(86, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gcrt(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;
    CComVariant t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(87, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbsts(long hwnd,long trans,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(trans);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(88, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbptt(long hwnd,const TCHAR * text,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(text);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(89, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fswfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(90, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_fpm(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(91, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_cc(long x,long y,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(92, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_sdm(long index,long addr,long size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(93, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_ku(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(94, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gpw(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(95, dis);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_eie(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(96, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_eww(long parent,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(parent);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(97, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_gp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(98, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_gsse()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(99, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_dt_fgs(long handle)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(handle);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(100, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_mr(long rx,long ry)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(rx);
    pn[0] = COleVariant(ry);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(101, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_hb_acr()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(102, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fsfex(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(103, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_dt_fipd()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(104, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fpe(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(105, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gsdbmp(long x1,long y1,long x2,long y2,long * data,long * size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(106, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        *data = t0.lVal;
        *size = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_fp(const TCHAR * server,long handle,long request_type,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(107, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_swtprt(long hwnd,long v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(108, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_swsz(long hwnd,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(109, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fsfs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(110, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_sdyipt(const TCHAR * mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(111, d);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fsex(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(112, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_fmcr(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(first_color);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(113, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_kuc(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(114, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fd(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(double_value_min);
    pn[0] = COleVariant(double_value_max);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(115, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_ife(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(116, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_sd(long index,const TCHAR * dict_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(117, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_gdc(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(118, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_fc(long x1,long y1,long x2,long y2,long quality,long delay,long time)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(quality);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(119, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_glerror()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(120, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_cfrr(long hwnd,long x,long y,long w,long h,long rw,long rh)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(x);
    pn[4] = COleVariant(y);
    pn[3] = COleVariant(w);
    pn[2] = COleVariant(h);
    pn[1] = COleVariant(rw);
    pn[0] = COleVariant(rh);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(121, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_ebd(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(122, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_ssmsg(long x1,long y1,long x2,long y2,const TCHAR * msg,const TCHAR * color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(msg);
    pn[0] = COleVariant(color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(123, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_hb_gdct()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(124, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_mtex(long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(125, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

LONGLONG thsPlug::fp_hb_acex(long hwnd,long mode,const TCHAR * base_addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(mode);
    pn[0] = COleVariant(base_addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(126, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_wd_kp(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(127, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_ep(const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(128, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gbw()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(129, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gcss()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(130, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_smcg(long col_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(col_gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(131, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ft_cfs()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(132, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_getID()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(133, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_ewbpid(long pid,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(pid);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(134, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wd_gcbgr(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(135, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_tpss(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(136, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_emacy(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(137, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_std(const TCHAR * string_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(138, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_gps(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(139, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_dt_ffh()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(140, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_pc_apad(const TCHAR * pic_info,long addr,long size)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(pic_info);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(141, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_ini_dpwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(142, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_ah_gah(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(143, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_fsd(long handle)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(handle);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(144, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gcboard()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(145, abp);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gcpos(long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(146, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_efsh()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(147, abp);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_hb_aad(const TCHAR * asm_ins)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(asm_ins);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(148, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_ld()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(149, abp);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gsdh()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(150, abp);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xs_sdacr(long level)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(level);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(151, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_scboard(const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(152, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_rd()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(153, abp);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fdex(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(double_value_min);
    pn[3] = COleVariant(double_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(154, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_fdle(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color,long style,long width)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(style);
    pn[0] = COleVariant(width);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(155, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_smsd(long speed)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(speed);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(156, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wdafb(long hwnd,LONGLONG addr,long data,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(data);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(157, abp);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbdt(long hwnd,long x,long y,long w,long h,const TCHAR * text,const TCHAR * color,long align)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x);
    pn[5] = COleVariant(y);
    pn[4] = COleVariant(w);
    pn[3] = COleVariant(h);
    pn[2] = COleVariant(text);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(align);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(158, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fmce(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(159, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_rs(long hwnd,const TCHAR * addr,long tpe,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(160, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_aj_lu()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(161, gh);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_hd_dgd(long en,const TCHAR * tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(162, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_getFps()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(163, gh);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_wf(long hwnd,const TCHAR * addr,float v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(164, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_st_orex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(165, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_aj_gkst(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(166, gh);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_gmpw()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(167, gh);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gmcnm()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(168, mc);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_rf(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(169, mc);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wd_ewsr(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2,long sort)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(spec1);
    pn[5] = COleVariant(flag1);
    pn[4] = COleVariant(type1);
    pn[3] = COleVariant(spec2);
    pn[2] = COleVariant(flag2);
    pn[1] = COleVariant(type2);
    pn[0] = COleVariant(sort);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(170, mc);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

float thsPlug::fp_wj_rft(long hwnd,const TCHAR * addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(171, mc);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

long thsPlug::fp_xt_sed(long index,const TCHAR * dict_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(172, mc);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_aim(long hwnd,const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(173, mc);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gdif(const TCHAR * str,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(174, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_rrf()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(175, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gwpid(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(176, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_swlh(long line_height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(line_height);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(177, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fubw(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(178, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_gmsd()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(179, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_strstr(const TCHAR * s,const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(s);
    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(180, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_st_rnm(const TCHAR * code,const TCHAR * Ver)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(Ver);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(181, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_fimd(const TCHAR * id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(182, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_lpc(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(183, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_setd(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(184, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_cfolder(const TCHAR * folder_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(185, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_smrgp(long row_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(row_gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(186, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ud(long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(187, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG thsPlug::fp_wj_gmba(long hwnd,const TCHAR * module_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(module_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(188, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_xt_iti32(LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(189, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gsw()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(190, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_scsz(long hwnd,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(191, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_ri(const TCHAR * section,const TCHAR * key,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(192, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_aj_kd(long vk)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(vk);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(193, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_ad(long index,const TCHAR * dict_info)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_info);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(194, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbute(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(195, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_wsadr(long hwnd,LONGLONG addr,long tpe,const TCHAR * v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(196, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_is64bit()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(197, ig);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fwser(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(spec1);
    pn[4] = COleVariant(flag1);
    pn[3] = COleVariant(type1);
    pn[2] = COleVariant(spec2);
    pn[1] = COleVariant(flag2);
    pn[0] = COleVariant(type2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(198, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_fpex(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(199, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_fpexs(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(200, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wd_gwt(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(201, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_rdadr(long hwnd,LONGLONG addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(202, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_ripwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        spDisp.GetIDOfName(thHelper::ToUNICODE(thHelper::Base64DecodeStr("UmVhZEluaVB3ZA==")),&dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_esd(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(204, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gc(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(205, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_cpng(long x1,long y1,long x2,long y2,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(206, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_mpn(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(207, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_sev(long index,const TCHAR * name,const TCHAR * value)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(name);
    pn[0] = COleVariant(value);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(208, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gsdat(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(209, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gchv(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(210, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_sptop()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(211, ig);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_getVersion()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(212, ig);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_swtxt(long hwnd,const TCHAR * text)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(text);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(213, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_rfd(const TCHAR * file_name,long start_pos,long end_pos)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(start_pos);
    pn[0] = COleVariant(end_pos);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(214, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_fsex(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(215, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_fsgex(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(string_value);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(216, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_gwds(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(217, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_md5(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(218, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_dfder(const TCHAR * folder_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(219, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_fswf(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[12];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[11] = COleVariant(x1);
    pn[10] = COleVariant(y1);
    pn[9] = COleVariant(x2);
    pn[8] = COleVariant(y2);
    pn[7] = COleVariant(str);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(font_name);
    pn[3] = COleVariant(font_size);
    pn[2] = COleVariant(flag);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(220, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,12,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_mdd(long dx,long dy)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(dx);
    pn[0] = COleVariant(dy);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(221, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fmc(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(222, ig);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_ru()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(223, tf);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_bwex(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,const TCHAR * public_desc,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(display);
    pn[3] = COleVariant(mouse);
    pn[2] = COleVariant(keypad);
    pn[1] = COleVariant(public_desc);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(224, tf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

float thsPlug::fp_wj_rfadr(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(225, tf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

long thsPlug::fp_xs_edd(long enable_debug)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(enable_debug);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(226, tf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_mm_smfrtf(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(227, tf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_getDir(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(228, tf);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_sdy()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(229, dr);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_suc(long uac)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(uac);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(230, dr);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_fcl()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(231, dr);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_ewbps(const TCHAR * process_name,const TCHAR * title,const TCHAR * class_name,long filter)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(process_name);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(232, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_sp(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(233, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_fiex(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(addr_range);
    pn[5] = COleVariant(int_value_min);
    pn[4] = COleVariant(int_value_max);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(234, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_getDPI()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(235, s);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_i_lit(long locks)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(locks);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(236, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_beep(long fre,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(fre);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(237, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbsgif(long hwnd,long x,long y,const TCHAR * pic_name,long repeat_limit,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(repeat_limit);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(238, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wdadr(long hwnd,LONGLONG addr,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(239, s);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG thsPlug::fp_hb_ac(long hwnd,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(240, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_fb_cfe(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(241, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_st_fp(const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(242, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_st_or(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(243, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

LONGLONG thsPlug::fp_nc_riadr(long hwnd,LONGLONG addr,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(244, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbtlg(long hwnd,long gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(245, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fcbex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(count);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(246, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_df(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(247, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_ss(long hwnd,const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(248, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_ini_eikpwd(const TCHAR * section,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(249, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xs_gdi()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(250, ol);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_fbsgf(long hwnd,long x,long y,const TCHAR * pic_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(x);
    pn[1] = COleVariant(y);
    pn[0] = COleVariant(pic_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(251, ol);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xs_dcdas()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(252, ol);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_weu()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(253, ol);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_frlse(long handle)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(handle);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(254, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_ini_wi(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(v);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(255, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ssero(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(256, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_dfsh()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(257, t);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_wk(long key_code,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(key_code);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(258, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_spath(const TCHAR * path)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(path);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(259, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_dev(long index,const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(260, t);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_vfex(long hwnd,LONGLONG addr)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(261, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_stct(long hwnd,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(262, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_rck()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(263, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_sf()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(264, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_wiadr(long hwnd,LONGLONG addr,long tpe,LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(265, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gargb(long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(266, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_cfbrt(long hwnd,long x,long y,long w,long h)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(267, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_cfle(const TCHAR * src_file,const TCHAR * dst_file,long over)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(src_file);
    pn[1] = COleVariant(dst_file);
    pn[0] = COleVariant(over);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(268, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gwrsr(const TCHAR * str,long index)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(str);
    pn[0] = COleVariant(index);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(269, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_gcutpe()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(270, v);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_st_cjpg(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(271, v);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbctt(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(272, f);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_st_fpc(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(273, f);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_dss()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(274, f);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gntime()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(275, f);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gw(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(276, f);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_eci()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(277, f);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_ffex(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(float_value_min);
    pn[3] = COleVariant(float_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(278, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_sdpwd(const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(279, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_log(const TCHAR * info)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(info);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(280, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_gtmac()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(281, fe);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_grct(const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(282, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_erm(long en,long mousedelay,long mousestep)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(en);
    pn[1] = COleVariant(mousedelay);
    pn[0] = COleVariant(mousestep);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(283, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_ini_eispwd(const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(284, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_fcr(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(285, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_eps(const TCHAR * all_pos,long tpe,long x1,long y1,long x2,long y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(all_pos);
    pn[4] = COleVariant(tpe);
    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(286, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_fwbpid(long process_id,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(process_id);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(287, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_skdy(const TCHAR * tpe,long delay)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(288, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gcnm(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(289, fe);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_bdw(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(display);
    pn[2] = COleVariant(mouse);
    pn[1] = COleVariant(keypad);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(290, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gev(long index,const TCHAR * name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(291, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_fpmex(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    CComVariant vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(292, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_sd(long index,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(293, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_sl()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(294, bw);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_fswfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(295, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_xt_fsfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(296, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_epce(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(297, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_lck()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(298, bw);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_stop(long id)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(id);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(299, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_kpc(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(300, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_fspe(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(301, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_ldck()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(302, bw);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbdpc(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(pic);
    pn[0] = COleVariant(trans_color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(303, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fdtex(long hwnd,const TCHAR * addr_range,const TCHAR * data,long steps,long multi_thread,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(addr_range);
    pn[3] = COleVariant(data);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(304, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gwrt(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;
    CComVariant t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(305, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_dable(const TCHAR * asm_code,LONGLONG base_addr,long is_64bit)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(asm_code);
    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(is_64bit);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(306, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_wfle(const TCHAR * file_name,const TCHAR * content)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(content);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(307, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_rg(const TCHAR * code,const TCHAR * Ver)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(Ver);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(308, bw);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gwc(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(309, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_gnd()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(310, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_vpex(long hwnd,LONGLONG addr,long size,long tpe,long old_protect)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr);
    pn[2] = COleVariant(size);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(old_protect);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(311, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gpi(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(312, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_ini_eis(const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(313, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_fpm(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_info);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(314, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_eik(const TCHAR * section,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(section);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(315, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_fspee(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(316, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wj_rdtb(long hwnd,const TCHAR * addr,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(317, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_rsadr(long hwnd,LONGLONG addr,long tpe,long length)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(318, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_wj_fsfexs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(319, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_ssm(long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(320, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_st_orexo(long x1,long y1,long x2,long y2,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(321, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_gsw(long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(322, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_fnps(const TCHAR * all_pos,long tpe,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(323, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_fbss(long hwnd,const TCHAR * file_name,long en,const TCHAR * header)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(en);
    pn[0] = COleVariant(header);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(324, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fblk(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(325, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ops(long pid)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pid);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(326, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_emm(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(327, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_dt_ss2(long hwnd,const TCHAR * str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(328, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbsf(long hwnd,const TCHAR * font_name,long size,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(329, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_mw(long hwnd,long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(330, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_st_orifl(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(331, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_nc_fcex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(332, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_gffs()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(333, i);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_wipwd(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name,const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(section);
    pn[3] = COleVariant(key);
    pn[2] = COleVariant(v);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
		while (true){break;}
		spDisp.GetIDOfName(thHelper::ToUNICODE(thHelper::Base64DecodeStr("V3JpdGVJbmlQd2Q=")), &dispatch_id);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_rap(const TCHAR * path,long mode)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(path);
    pn[0] = COleVariant(mode);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(335, i);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wd_gcsex(long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(336, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_nc_fss(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(337, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

CString thsPlug::fp_nc_fdd(long hwnd,const TCHAR * addr_range,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr_range);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(338, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_cgif(long x1,long y1,long x2,long y2,const TCHAR * file_name,long delay,long time)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(339, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_eks(long en,long time_out)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(340, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_ff(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(float_value_min);
    pn[0] = COleVariant(float_value_max);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(341, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_nc_wfa(long hwnd,LONGLONG addr,float v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(342, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_imgtobmp(const TCHAR * pic_name,const TCHAR * bmp_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(pic_name);
    pn[0] = COleVariant(bmp_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(343, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_cr_lc()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(344, ex);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_spp(const TCHAR * pwd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(pwd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(345, ex);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fce(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
       DEBUG_CHECK_S(346, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_gost()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(347, di);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_st_rgexnmc(const TCHAR * code,const TCHAR * Ver,const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(Ver);
    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(348, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_swlhnd(long line_height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(line_height);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(349, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wi(long hwnd,const TCHAR * addr,long tpe,LONGLONG v)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(350, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_scgnd(long col_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(col_gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(351, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_swgp(long word_gap)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(word_gap);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(352, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_dini(const TCHAR * section,const TCHAR * key,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(353, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_st_fps(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(354, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_st_rgex(const TCHAR * code,const TCHAR * Ver,const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(Ver);
    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(355, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_scd(const TCHAR * cmd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(cmd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(356, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_mt(long x,long y)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(357, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

LONGLONG thsPlug::fp_nc_vaex(long hwnd,LONGLONG addr,long size,long tpe)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    CComVariant vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(tpe);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(358, di);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.llVal;
    }
    return 0;
}

long thsPlug::fp_xt_ssem(long show)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(show);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(359, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_cpe(long x1,long y1,long x2,long y2,const TCHAR * file_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(360, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_kdc(const TCHAR * key_str)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(key_str);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(361, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_fw(long x1,long y1,long x2,long y2,const TCHAR * color,const TCHAR * word)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    CComVariant vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(word);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(362, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_aj_mck()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(363, mg);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_nc_fse(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    CComVariant vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(364, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_xt_gle()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(365, mg);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_nc_wd(long hwnd,const TCHAR * addr,const TCHAR * data)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(366, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_ckuac()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(367, mg);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_wj_gdsl()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(368, mg);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_swstate(long hwnd,long flag)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(369, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_fb_fbtpdir(long hwnd,long dir)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    CComVariant vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(dir);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(370, mg);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_mu()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(371, u);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_gcsz(long hwnd,long * width,long * height)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;
    CComVariant t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(372, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *width = t0.lVal;
        *height = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wd_fwex(long parent,const TCHAR * class_name,const TCHAR * title_name)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    CComVariant vResult;

    pn[2] = COleVariant(parent);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(373, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_wj_ifet(const TCHAR * folder)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(folder);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(374, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_rbtbr(const TCHAR * rgb_color)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(rgb_color);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(375, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_fb_fce(long hwnd)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(hwnd);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(376, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_xt_gttime()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(377, u);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long thsPlug::fp_aj_erkd(long en)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(en);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(378, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gntbip(const TCHAR * ip)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    CComVariant vResult;

    pn[0] = COleVariant(ip);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(379, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_wd_idydd(long x1,long y1,long x2,long y2,long t)
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    CComVariant vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(t);

    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(380, u);
    }

    hr = spDisp.InvokeN(dispatch_id,pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gmc()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(381, u);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}

long thsPlug::fp_aj_md()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(382, u);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString thsPlug::fp_xt_gbp()
{
    static DISPID dispatch_id = -1;
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    CComVariant vResult;


    if (dispatch_id == -1)
    {
        DEBUG_CHECK_S(383, df);
    }

    hr = spDisp.Invoke0(dispatch_id,&vResult);
    if (SUCCEEDED(hr))
    {
        return  CString(vResult.bstrVal);
    }
    return  _T("");
}


