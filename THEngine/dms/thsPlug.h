#ifndef __INCLUDE_THSPLUG_H__
#define __INCLUDE_THSPLUG_H__

#if !defined(__cplusplus)
#error C++ compiler required
#endif

#include <objbase.h>

// f w d g h a j s n l t e z x i

class thsPlug
{
private:
    IDispatch * obj;
	CString m_strTemp;
	static thsPlug* s_ths;
	static CString s_sp;

public:
    thsPlug();
	thsPlug(bool bIsPtr);
    virtual ~thsPlug();
	// ´´½¨²å¼þ
	static thsPlug* createThs(const CString& sft, const CString& sftdl, const CString& sftpar="");
	static void releaseThs(void);
	static thsPlug* get() { return s_ths; }

	// SetRowGapNoDict
    virtual long fp_wz_srgnd(long row_gap);
	// Assemble
    virtual CString fp_fh_abe(LONGLONG base_addr,long is_64bit);
	// EnableMouseSync
    virtual long fp_ht_ems(long en,long time_out);
	// LockMouseRect
    virtual long fp_ht_lmrt(long x1,long y1,long x2,long y2);
	// SortPosDistance
    virtual CString fp_sf_spde(const TCHAR * all_pos,long tpe,long x,long y);
	// GetNetTimeSafe
    virtual CString fp_xt_gnts();
	// DownCpu
    virtual long fp_ht_dcu(long rate);
	// UnBindWindow
    virtual long fp_ht_ubw();
	// InitCri
    virtual long fp_zx_ici();
	// CheckInputMethod
    virtual long fp_zx_cimd(long hwnd,const TCHAR * id);
	// ReadDouble
    virtual double fp_nc_rd(long hwnd,const TCHAR * addr);
	// SetDisplayDelay
    virtual long fp_ht_sddy(long t);
	// MoveFile
    virtual long fp_wj_mf(const TCHAR * src_file,const TCHAR * dst_file);
	// GetWindowProcessPath
    virtual CString fp_wd_gwpp(long hwnd);
	// VirtualQueryEx
    virtual CString fp_nc_vqex(long hwnd,LONGLONG addr,long pmbi);
	// WriteString
    virtual long fp_nc_ws(long hwnd,const TCHAR * addr,long tpe,const TCHAR * v);
	// GetWindowState
    virtual long fp_wd_gws(long hwnd,long flag);
	// FaqCaptureFromFile
    virtual long fp_dt_fcff(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality);
	// Play
    virtual long fp_xt_play(const TCHAR * file_name);
	// FaqCaptureString
    virtual long fp_dt_fcs(const TCHAR * str);
	// FaqSend
    virtual CString fp_dt_fsd(const TCHAR * server,long handle,long request_type,long time_out);
	// FindMultiColorEx
    virtual CString fp_dt_fmcex(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir);
	// SetEnumWindowDelay
    virtual long fp_jb_sewd(long delay);
	// SetScreen
    virtual long fp_xt_ss(long width,long height,long depth);
	// GetDict
    virtual CString fp_wz_gdt(long index,long font_index);
	// FindPicMemE
    virtual CString fp_ts_fpme(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir);
	// DownloadFile
    virtual long fp_wj_dfe(const TCHAR * url,const TCHAR * save_file,long timeout);
	// delay
    virtual long fp_xt_delay(long mis);
	// EnableGetColorByCapture
    virtual long fp_ts_egcbc(long en);
	// FoobarFillRect
    virtual long fp_fb_ffrt(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color);
	// ExitOs
    virtual long fp_xt_eos(long tpe);
	// FloatToData
    virtual CString fp_nc_ftd(float float_value);
	// BGR2RGB
    virtual CString fp_ts_brtrb(const TCHAR * bgr_color);
	// WriteDataFromBin
    virtual long fp_nc_wdfb(long hwnd,const TCHAR * addr,long data,long length);
	// ReadData
    virtual CString fp_nc_rdd(long hwnd,const TCHAR * addr,long length);
	// FoobarTextRect
    virtual long fp_fb_ftr(long hwnd,long x,long y,long w,long h);
	// ReadDataAddrToBin
    virtual long fp_nc_rdatb(long hwnd,LONGLONG addr,long length);
	// FindStr
    virtual long fp_wz_fsr(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y);
	// SetExactOcr
    virtual long fp_wz_seor(long exact_ocr);
	// GetFileLength
    virtual long fp_wj_gfl(const TCHAR * file_name);
	// FindStrS
    virtual CString fp_wz_fsrs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y);
	// EnableKeypadPatch
    virtual long fp_ht_ekp(long en);
	// ReadDoubleAddr
    virtual double fp_nc_rda(long hwnd,LONGLONG addr);
	// FoobarUnlock
    virtual long fp_fb_fbuk(long hwnd);
	// GetWordsNoDict
    virtual CString fp_wz_gwnd(long x1,long y1,long x2,long y2,const TCHAR * color);
	// EnableKeypadMsg
    virtual long fp_ht_ekm(long en);
	// LoadPicByte
    virtual long fp_ts_lpb(long addr,long size,const TCHAR * name);
	// SetExcludeRegion
    virtual long fp_ts_ser(long tpe,const TCHAR * info);
	// WriteDouble
    virtual long fp_nc_wde(long hwnd,const TCHAR * addr,double v);
	// FindWindowByProcess
    virtual long fp_wd_fwbps(const TCHAR * process_name,const TCHAR * class_name,const TCHAR * title_name);
	// EncodeFile
    virtual long fp_wj_ef(const TCHAR * file_name,const TCHAR * pwd);
	// SetMouseDelay
    virtual long fp_ht_smdy(const TCHAR * tpe,long delay);
	// SetMemoryHwndAsProcessId
    virtual long fp_nc_smhapid(long en);
	// WheelDown
    virtual long fp_ht_wdn();
	// SwitchBindWindow
    virtual long fp_wd_sbw(long hwnd);
	// CreateFoobarCustom
    virtual long fp_wd_cfcm(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color,double sim);
	// DisablePowerSave
    virtual long fp_nc_dps();
	// Delays
    virtual long fp_xt_delays(long min_s,long max_s);
	// ClearDict
    virtual long fp_wj_cdt(long index);
	// GetScreenHeight
    virtual long fp_wd_gsht();
	// GetWordResultPos
    virtual long fp_wd_gwrps(const TCHAR * str,long index,long * x,long * y);
	// FindWindow
    virtual long fp_wd_fw(const TCHAR * class_name,const TCHAR * title_name);
	// GetWordResultCount
    virtual long fp_wd_gwrc(const TCHAR * str);
	// DeleteFile
    virtual long fp_xt_df(const TCHAR * file_name);
	// SendStringIme2
    virtual long fp_dt_ssim2(long hwnd,const TCHAR * str,long mode);
	// IsBind
    virtual long fp_wd_isbd(long hwnd);
	// CapturePre
    virtual long fp_wd_cpe(const TCHAR * file_name);
	// ReadInt
    virtual LONGLONG fp_nc_ri(long hwnd,const TCHAR * addr,long tpe);
	// GetForegroundWindow
    virtual long fp_wd_gfww();
	// SetWordGapNoDict
    virtual long fp_nc_swgnd(long word_gap);
	// FindStrExS
    virtual CString fp_nc_fsexs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// FindInt
    virtual CString fp_nc_fi(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe);
	// ClientToScreen
    virtual long fp_wd_cts(long hwnd,long * x,long * y);
	// EnableFakeActive
    virtual long fp_xt_efae(long en);
	// WriteDoubleAddr
    virtual long fp_nc_wda(long hwnd,LONGLONG addr,double v);
	// GetCommandLine
    virtual CString fp_xt_gcl(long hwnd);
	// FindColorBlock
    virtual long fp_nc_fcb(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height,long * x,long * y);
	// KeyPressStr
    virtual long fp_wd_kps(const TCHAR * key_str,long delay);
	// SetDisplayRefreshDelay
    virtual long fp_xt_sdpDelay(long t);
	// SendStringIme
    virtual long fp_dt_ssim(const TCHAR * str);
	// EnableSpeedDx
    virtual long fd_xt_esdx(long en);
	// LockDisplay
    virtual long fp_xt_ldy(long locks);
	// GetResultPos
    virtual long fp_xt_grps(const TCHAR * str,long index,long * x,long * y);
	// FindStrFast
    virtual long fp_nc_fsft(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y);
	// IntToData
    virtual CString fp_wj_itd(LONGLONG int_value,long tpe);
	// DoubleToData
    virtual CString fp_wj_dtd(double double_value);
	// GetClientRect
    virtual long fp_wd_gcrt(long hwnd,long * x1,long * y1,long * x2,long * y2);
	// FoobarSetTrans
    virtual long fp_fb_fbsts(long hwnd,long trans,const TCHAR * color,double sim);
	// FoobarPrintText
    virtual long fp_fb_fbptt(long hwnd,const TCHAR * text,const TCHAR * color);
	// FindStrWithFontE
    virtual CString fp_nc_fswfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag);
	// FreeProcessMemory
    virtual long fp_nc_fpm(long hwnd);
	// CmpColor
    virtual long fp_nc_cc(long x,long y,const TCHAR * color,double sim);
	// SetDictMem
    virtual long fp_xt_sdm(long index,long addr,long size);
	// KeyUp
    virtual long fp_aj_ku(long vk);
	// GetPointWindow
    virtual long fp_wd_gpw(long x,long y);
	// EnableIme
    virtual long fp_wd_eie(long en);
	// EnumWindow
    virtual CString fp_wd_eww(long parent,const TCHAR * title,const TCHAR * class_name,long filter);
	// GetPath
    virtual CString fp_wj_gp();
	// GetCursorShape
    virtual CString fp_xt_gsse();
	// FaqGetSize
    virtual long fp_dt_fgs(long handle);
	// MoveR
    virtual long fp_wd_mr(long rx,long ry);
	// AsmClear
    virtual long fp_hb_acr();
	// FindStrFastEx
    virtual CString fp_nc_fsfex(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// FaqIsPosted
    virtual long fp_dt_fipd();
	// FindPicE
    virtual CString fp_nc_fpe(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir);
	// GetScreenDataBmp
    virtual long fp_wd_gsdbmp(long x1,long y1,long x2,long y2,long * data,long * size);
	// FaqPost
    virtual long fp_dt_fp(const TCHAR * server,long handle,long request_type,long time_out);
	// SetWindowTransparent
    virtual long fp_wd_swtprt(long hwnd,long v);
	// SetWindowSize
    virtual long fp_wd_swsz(long hwnd,long width,long height);
	// FindStrFastS
    virtual CString fp_nc_fsfs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,long * x,long * y);
	// SetDisplayInput
    virtual long fp_wd_sdyipt(const TCHAR * mode);
	// FindStrEx
    virtual CString fp_nc_fsex(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// FindMultiColor
    virtual long fp_nc_fmcr(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir,long * x,long * y);
	// KeyUpChar
    virtual long fp_aj_kuc(const TCHAR * key_str);
	// FindDouble
    virtual CString fp_nc_fd(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max);
	// IsFileExist
    virtual long fp_wj_ife(const TCHAR * file_name);
	// SetDict
    virtual long fp_wj_sd(long index,const TCHAR * dict_name);
	// GetDictCount
    virtual long fp_wj_gdc(long index);
	// FaqCapture
    virtual long fp_dt_fc(long x1,long y1,long x2,long y2,long quality,long delay,long time);
	// GetLastError
    virtual long fp_xt_glerror();
	// CreateFoobarRoundRect
    virtual long fp_fb_cfrr(long hwnd,long x,long y,long w,long h,long rw,long rh);
	// EnableBind
    virtual long fp_wd_ebd(long en);
	// ShowScrMsg
    virtual long fp_wj_ssmsg(long x1,long y1,long x2,long y2,const TCHAR * msg,const TCHAR * color);
	// GetDmCount
    virtual long fp_hb_gdct();
	// MoveToEx
    virtual CString fp_wd_mtex(long x,long y,long w,long h);
	// AsmCallEx
    virtual LONGLONG fp_hb_acex(long hwnd,long mode,const TCHAR * base_addr);
	// KeyPress
    virtual long fp_wd_kp(long vk);
	// EnumProcess
    virtual CString fp_wd_ep(const TCHAR * name);
	// GetBindWindow
    virtual long fp_wd_gbw();
	// GetCursorSpot
    virtual CString fp_wd_gcss();
	// SetMinColGap
    virtual long fp_wj_smcg(long col_gap);
	// CheckFontSmooth
    virtual long fp_ft_cfs();
	// GetID
    virtual long fp_getID();
	// EnumWindowByProcessId
    virtual CString fp_wd_ewbpid(long pid,const TCHAR * title,const TCHAR * class_name,long filter);
	// GetColorBGR
    virtual CString fp_wd_gcbgr(long x,long y);
	// TerminateProcess
    virtual long fp_xt_tpss(long pid);
	// EnableMouseAccuracy
    virtual long fp_xt_emacy(long en);
	// StringToData
    virtual CString fp_xt_std(const TCHAR * string_value,long tpe);
	// GetPicSize
    virtual CString fp_xt_gps(const TCHAR * pic_name);
	// FaqFetch
    virtual CString fp_dt_ffh();
	// AppendPicAddr
    virtual CString fp_pc_apad(const TCHAR * pic_info,long addr,long size);
	// DeleteIniPwd
    virtual long fp_ini_dpwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd);
	// GetAveHSV
    virtual CString fp_ah_gah(long x1,long y1,long x2,long y2);
	// FreeScreenData
    virtual long fp_wd_fsd(long handle);
	// GetClipboard
    virtual CString fp_xt_gcboard();
	// GetCursorPos
    virtual long fp_wd_gcpos(long * x,long * y);
	// EnableFontSmooth
    virtual long fp_xt_efsh();
	// AsmAdd
    virtual long fp_hb_aad(const TCHAR * asm_ins);
	// LeftDown
    virtual long fp_aj_ld();
	// GetScreenDepth
    virtual long fp_wd_gsdh();
	// SetDisplayAcceler
    virtual long fp_xs_sdacr(long level);
	// SetClipboard
    virtual long fp_xt_scboard(const TCHAR * data);
	// RightDown
    virtual long fp_aj_rd();
	// FindDoubleEx
    virtual CString fp_nc_fdex(long hwnd,const TCHAR * addr_range,double double_value_min,double double_value_max,long steps,long multi_thread,long mode);
	// FoobarDrawLine
    virtual long fp_fb_fdle(long hwnd,long x1,long y1,long x2,long y2,const TCHAR * color,long style,long width);
	// SetMouseSpeed
    virtual long fp_aj_smsd(long speed);
	// WriteDataAddrFromBin
    virtual long fp_nc_wdafb(long hwnd,LONGLONG addr,long data,long length);
	// FoobarDrawText
    virtual long fp_fb_fbdt(long hwnd,long x,long y,long w,long h,const TCHAR * text,const TCHAR * color,long align);
	// FindMultiColorE
    virtual CString fp_nc_fmce(long x1,long y1,long x2,long y2,const TCHAR * first_color,const TCHAR * offset_color,double sim,long dir);
	// ReadString
    virtual CString fp_wj_rs(long hwnd,const TCHAR * addr,long tpe,long length);
	// LeftUp
    virtual long fp_aj_lu();
	// DmGuard
    virtual long fp_hd_dgd(long en,const TCHAR * tpe);
	// GetFps
    virtual long fp_getFps();
	// WriteFloat
    virtual long fp_wj_wf(long hwnd,const TCHAR * addr,float v);
	// OcrEx
    virtual CString fp_st_orex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// GetKeyState
    virtual long fp_aj_gkst(long vk);
	// GetMousePointWindow
    virtual long fp_aj_gmpw();
	// GetMachineCodeNoMac
    virtual CString fp_xt_gmcnm();
	// ReadFile
    virtual CString fp_wj_rf(const TCHAR * file_name);
	// EnumWindowSuper
    virtual CString fp_wd_ewsr(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2,long sort);
	// ReadFloat
    virtual float fp_wj_rft(long hwnd,const TCHAR * addr);
	// SetExportDict
    virtual long fp_xt_sed(long index,const TCHAR * dict_name);
	// ActiveInputMethod
    virtual long fp_aj_aim(long hwnd,const TCHAR * id);
	// GetDictInfo
    virtual CString fp_xt_gdif(const TCHAR * str,const TCHAR * font_name,long font_size,long flag);
	// ReleaseRef
    virtual long fp_xt_rrf();
	// GetWindowProcessId
    virtual long fp_wd_gwpid(long hwnd);
	// SetWordLineHeight
    virtual long fp_wd_swlh(long line_height);
	// ForceUnBindWindow
    virtual long fp_wd_fubw(long hwnd);
	// GetMouseSpeed
    virtual long fp_aj_gmsd();
	// StrStr
    virtual long fp_xt_strstr(const TCHAR * s,const TCHAR * str);
	// RegNoMac
    virtual long fp_st_rnm(const TCHAR * code,const TCHAR * Ver);
	// FindInputMethod
    virtual long fp_aj_fimd(const TCHAR * id);
	// LoadPic
    virtual long fp_xt_lpc(const TCHAR * pic_name);
	// SetExitThread
    virtual long fp_xt_setd(long en);
	// CreateFolder
    virtual long fp_xt_cfolder(const TCHAR * folder_name);
	// SetMinRowGap
    virtual long fp_xt_smrgp(long row_gap);
	// UseDict
    virtual long fp_xt_ud(long index);
	// GetModuleBaseAddr
    virtual LONGLONG fp_wj_gmba(long hwnd,const TCHAR * module_name);
	// Int64ToInt32
    virtual long fp_xt_iti32(LONGLONG v);
	// GetScreenWidth
    virtual long fp_wd_gsw();
	// SetClientSize
    virtual long fp_wd_scsz(long hwnd,long width,long height);
	// ReadIni
    virtual CString fp_wj_ri(const TCHAR * section,const TCHAR * key,const TCHAR * file_name);
	// KeyDown
    virtual long fp_aj_kd(long vk);
	// AddDict
    virtual long fp_wj_ad(long index,const TCHAR * dict_info);
	// FoobarUpdate
    virtual long fp_fb_fbute(long hwnd);
	// WriteStringAddr
    virtual long fp_wj_wsadr(long hwnd,LONGLONG addr,long tpe,const TCHAR * v);
	// Is64Bit
    virtual long fp_is64bit();
	// FindWindowSuper
    virtual long fp_wd_fwser(const TCHAR * spec1,long flag1,long type1,const TCHAR * spec2,long flag2,long type2);
	// FindPicEx
    virtual CString fp_wj_fpex(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir);
	// FindPicExS
    virtual CString fp_wj_fpexs(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir);
	// GetWindowTitle
    virtual CString fp_wd_gwt(long hwnd);
	// ReadDataAddr
    virtual CString fp_wj_rdadr(long hwnd,LONGLONG addr,long length);
	// ReadIniPwd
    virtual CString fp_wj_ripwd(const TCHAR * section,const TCHAR * key,const TCHAR * file_name,const TCHAR * pwd);
	// EnableShareDict
    virtual long fp_xt_esd(long en);
	// GetColor
    virtual CString fp_wd_gc(long x,long y);
	// CapturePng
    virtual long fp_wd_cpng(long x1,long y1,long x2,long y2,const TCHAR * file_name);
	// MatchPicName
    virtual CString fp_xt_mpn(const TCHAR * pic_name);
	// SetEnv
    virtual long fp_xt_sev(long index,const TCHAR * name,const TCHAR * value);
	// GetScreenData
    virtual long fp_wd_gsdat(long x1,long y1,long x2,long y2);
	// GetColorHSV
    virtual CString fp_wd_gchv(long x,long y);
	// SetParam64ToPointer
    virtual long fp_xt_sptop();
	// Ver
    virtual CString fp_getVersion();
	// SetWindowText
    virtual long fp_wd_swtxt(long hwnd,const TCHAR * text);
	// ReadFileData
    virtual CString fp_wj_rfd(const TCHAR * file_name,long start_pos,long end_pos);
	// FindShapeEx
    virtual CString fp_wj_fsex(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir);
	// FindStringEx
    virtual CString fp_wj_fsgex(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe,long steps,long multi_thread,long mode);
	// GetWords
    virtual CString fp_wj_gwds(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// Md5
    virtual CString fp_md5(const TCHAR * str);
	// DeleteFolder
    virtual long fp_xt_dfder(const TCHAR * folder_name);
	// FindStrWithFont
    virtual long fp_wj_fswf(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag,long * x,long * y);
	// MoveDD
    virtual long fp_aj_mdd(long dx,long dy);
	// FindMulColor
    virtual long fp_wd_fmc(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// RightUp
    virtual long fp_aj_ru();
	// BindWindowEx
    virtual long fp_wd_bwex(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,const TCHAR * public_desc,long mode);
	// ReadFloatAddr
    virtual float fp_wj_rfadr(long hwnd,LONGLONG addr);
	// EnableDisplayDebug
    virtual long fp_xs_edd(long enable_debug);
	// SetMemoryFindResultToFile
    virtual long fp_mm_smfrtf(const TCHAR * file_name);
	// GetDir
    virtual CString fp_xt_getDir(long tpe);
	// SelectDirectory
    virtual CString fp_xt_sdy();
	// SetUAC
    virtual long fp_xt_suc(long uac);
	// FaqCancel
    virtual long fp_dt_fcl();
	// EnumWindowByProcess
    virtual CString fp_wd_ewbps(const TCHAR * process_name,const TCHAR * title,const TCHAR * class_name,long filter);
	// SendPaste
    virtual long fp_xt_sp(long hwnd);
	// FindIntEx
    virtual CString fp_wj_fiex(long hwnd,const TCHAR * addr_range,LONGLONG int_value_min,LONGLONG int_value_max,long tpe,long steps,long multi_thread,long mode);
	// GetDPI
    virtual long fp_getDPI();
	// LockInput
    virtual long fp_i_lit(long locks);
	// Beep
    virtual long fp_beep(long fre,long delay);
	// FoobarStartGif
    virtual long fp_fb_fbsgif(long hwnd,long x,long y,const TCHAR * pic_name,long repeat_limit,long delay);
	// WriteDataAddr
    virtual long fp_nc_wdadr(long hwnd,LONGLONG addr,const TCHAR * data);
	// AsmCall
    virtual LONGLONG fp_hb_ac(long hwnd,long mode);
	// CreateFoobarEllipse
    virtual long fp_fb_cfe(long hwnd,long x,long y,long w,long h);
	// FreePic
    virtual long fp_st_fp(const TCHAR * pic_name);
	// Ocr
    virtual CString fp_st_or(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// ReadIntAddr
    virtual LONGLONG fp_nc_riadr(long hwnd,LONGLONG addr,long tpe);
	// FoobarTextLineGap
    virtual long fp_fb_fbtlg(long hwnd,long gap);
	// FindColorBlockEx
    virtual CString fp_nc_fcbex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long count,long width,long height);
	// DecodeFile
    virtual long fp_wj_df(const TCHAR * file_name,const TCHAR * pwd);
	// SendString
    virtual long fp_dt_ss(long hwnd,const TCHAR * str);
	// EnumIniKeyPwd
    virtual CString fp_ini_eikpwd(const TCHAR * section,const TCHAR * file_name,const TCHAR * pwd);
	// GetDisplayInfo
    virtual CString fp_xs_gdi();
	// FoobarStopGif
    virtual long fp_fb_fbsgf(long hwnd,long x,long y,const TCHAR * pic_name);
	// DisableCloseDisplayAndSleep
    virtual long fp_xs_dcdas();
	// WheelUp
    virtual long fp_aj_weu();
	// FaqRelease
    virtual long fp_dt_frlse(long handle);
	// WriteIni
    virtual long fp_ini_wi(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name);
	// SetAero
    virtual long fp_xt_ssero(long en);
	// DisableFontSmooth
    virtual long fp_xt_dfsh();
	// WaitKey
    virtual long fp_aj_wk(long key_code,long time_out);
	// SetPath
    virtual long fp_xt_spath(const TCHAR * path);
	// DelEnv
    virtual long fp_xt_dev(long index,const TCHAR * name);
	// VirtualFreeEx
    virtual long fp_nc_vfex(long hwnd,LONGLONG addr);
	// ScreenToClient
    virtual long fp_wd_stct(long hwnd,long * x,long * y);
	// RightClick
    virtual long fp_aj_rck();
	// SelectFile
    virtual CString fp_xt_sf();
	// WriteIntAddr
    virtual long fp_wj_wiadr(long hwnd,LONGLONG addr,long tpe,LONGLONG v);
	// GetAveRGB
    virtual CString fp_xt_gargb(long x1,long y1,long x2,long y2);
	// CreateFoobarRect
    virtual long fp_fb_cfbrt(long hwnd,long x,long y,long w,long h);
	// CopyFile
    virtual long fp_xt_cfle(const TCHAR * src_file,const TCHAR * dst_file,long over);
	// GetWordResultStr
    virtual CString fp_xt_gwrsr(const TCHAR * str,long index);
	// GetCpuType
    virtual long fp_xt_gcutpe();
	// CaptureJpg
    virtual long fp_st_cjpg(long x1,long y1,long x2,long y2,const TCHAR * file_name,long quality);
	// FoobarClearText
    virtual long fp_fb_fbctt(long hwnd);
	// FindPic
    virtual long fp_st_fpc(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y);
	// DisableScreenSave
    virtual long fp_xt_dss();
	// GetNetTime
    virtual CString fp_xt_gntime();
	// GetWindow
    virtual long fp_wd_gw(long hwnd,long flag);
	// EnterCri
    virtual long fp_xt_eci();
	// FindFloatEx
    virtual CString fp_wj_ffex(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max,long steps,long multi_thread,long mode);
	// SetDictPwd
    virtual long fp_xt_sdpwd(const TCHAR * pwd);
	// Log
    virtual long fp_log(const TCHAR * info);
	// GetMac
    virtual CString fp_gtmac();
	// GetResultCount
    virtual long fp_xt_grct(const TCHAR * str);
	// EnableRealMouse
    virtual long fp_aj_erm(long en,long mousedelay,long mousestep);
	// EnumIniSectionPwd
    virtual CString fp_ini_eispwd(const TCHAR * file_name,const TCHAR * pwd);
	// FindColor
    virtual long fp_wd_fcr(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir,long * x,long * y);
	// ExcludePos
    virtual CString fp_wd_eps(const TCHAR * all_pos,long tpe,long x1,long y1,long x2,long y2);
	// FindWindowByProcessId
    virtual long fp_wd_fwbpid(long process_id,const TCHAR * class_name,const TCHAR * title_name);
	// SetKeypadDelay
    virtual long fp_aj_skdy(const TCHAR * tpe,long delay);
	// GetColorNum
    virtual long fp_wd_gcnm(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// BindWindow
    virtual long fp_wd_bdw(long hwnd,const TCHAR * display,const TCHAR * mouse,const TCHAR * keypad,long mode);
	// GetEnv
    virtual CString fp_xt_gev(long index,const TCHAR * name);
	// FindPicMemEx
    virtual CString fp_xt_fpmex(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir);
	// SaveDict
    virtual long fp_xt_sd(long index,const TCHAR * file_name);
	// SetLocale
    virtual long fp_xt_sl();
	// FindStrWithFontEx
    virtual CString fp_xt_fswfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim,const TCHAR * font_name,long font_size,long flag);
	// FindStrFastE
    virtual CString fp_xt_fsfe(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// EnablePicCache
    virtual long fp_xt_epce(long en);
	// LeftClick
    virtual long fp_aj_lck();
	// Stop
    virtual long fp_stop(long id);
	// KeyPressChar
    virtual long fp_aj_kpc(const TCHAR * key_str);
	// FindShape
    virtual long fp_wj_fspe(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir,long * x,long * y);
	// LeftDoubleClick
    virtual long fp_aj_ldck();
	// FoobarDrawPic
    virtual long fp_fb_fbdpc(long hwnd,long x,long y,const TCHAR * pic,const TCHAR * trans_color);
	// FindDataEx
    virtual CString fp_nc_fdtex(long hwnd,const TCHAR * addr_range,const TCHAR * data,long steps,long multi_thread,long mode);
	// GetWindowRect
    virtual long fp_wd_gwrt(long hwnd,long * x1,long * y1,long * x2,long * y2);
	// DisAssemble
    virtual CString fp_xt_dable(const TCHAR * asm_code,LONGLONG base_addr,long is_64bit);
	// WriteFile
    virtual long fp_wj_wfle(const TCHAR * file_name,const TCHAR * content);
	// Reg
    virtual long fp_nc_rg(const TCHAR * code,const TCHAR * Ver);
	// GetWindowClass
    virtual CString fp_wd_gwc(long hwnd);
	// GetNowDict
    virtual long fp_xt_gnd();
	// VirtualProtectEx
    virtual long fp_nc_vpex(long hwnd,LONGLONG addr,long size,long tpe,long old_protect);
	// GetProcessInfo
    virtual CString fp_xt_gpi(long pid);
	// EnumIniSection
    virtual CString fp_ini_eis(const TCHAR * file_name);
	// FindPicMem
    virtual long fp_xt_fpm(long x1,long y1,long x2,long y2,const TCHAR * pic_info,const TCHAR * delta_color,double sim,long dir,long * x,long * y);
	// EnumIniKey
    virtual CString fp_xt_eik(const TCHAR * section,const TCHAR * file_name);
	// FindShapeE
    virtual CString fp_wj_fspee(long x1,long y1,long x2,long y2,const TCHAR * offset_color,double sim,long dir);
	// ReadDataToBin
    virtual long fp_wj_rdtb(long hwnd,const TCHAR * addr,long length);
	// ReadStringAddr
    virtual CString fp_wj_rsadr(long hwnd,LONGLONG addr,long tpe,long length);
	// FindStrFastExS
    virtual CString fp_wj_fsfexs(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// SetSimMode
    virtual long fp_xt_ssm(long mode);
	// OcrExOne
    virtual CString fp_st_orexo(long x1,long y1,long x2,long y2,const TCHAR * color,double sim);
	// GetSpecialWindow
    virtual long fp_xt_gsw(long flag);
	// FindNearestPos
    virtual CString fp_xt_fnps(const TCHAR * all_pos,long tpe,long x,long y);
	// FoobarSetSave
    virtual long fp_fb_fbss(long hwnd,const TCHAR * file_name,long en,const TCHAR * header);
	// FoobarLock
    virtual long fp_fb_fblk(long hwnd);
	// OpenProcess
    virtual long fp_xt_ops(long pid);
	// EnableMouseMsg
    virtual long fp_aj_emm(long en);
	// SendString2
    virtual long fp_dt_ss2(long hwnd,const TCHAR * str);
	// FoobarSetFont
    virtual long fp_fb_fbsf(long hwnd,const TCHAR * font_name,long size,long flag);
	// MoveWindow
    virtual long fp_wd_mw(long hwnd,long x,long y);
	// OcrInFile
    virtual CString fp_st_orifl(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * color,double sim);
	// FindColorEx
    virtual CString fp_nc_fcex(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir);
	// GetForegroundFocus
    virtual long fp_wd_gffs();
	// WriteIniPwd
    virtual long fp_xt_wipwd(const TCHAR * section,const TCHAR * key,const TCHAR * v,const TCHAR * file_name,const TCHAR * pwd);
	// RunApp
    virtual long fp_xt_rap(const TCHAR * path,long mode);
	// GetCursorShapeEx
    virtual CString fp_wd_gcsex(long tpe);
	// FindString
    virtual CString fp_nc_fss(long hwnd,const TCHAR * addr_range,const TCHAR * string_value,long tpe);
	// FindData
    virtual CString fp_nc_fdd(long hwnd,const TCHAR * addr_range,const TCHAR * data);
	// CaptureGif
    virtual long fp_xt_cgif(long x1,long y1,long x2,long y2,const TCHAR * file_name,long delay,long time);
	// EnableKeypadSync
    virtual long fp_xt_eks(long en,long time_out);
	// FindFloat
    virtual CString fp_nc_ff(long hwnd,const TCHAR * addr_range,float float_value_min,float float_value_max);
	// WriteFloatAddr
    virtual long fp_nc_wfa(long hwnd,LONGLONG addr,float v);
	// ImageToBmp
    virtual long fp_xt_imgtobmp(const TCHAR * pic_name,const TCHAR * bmp_name);
	// LeaveCri
    virtual long fp_cr_lc();
	// SetPicPwd
    virtual long fp_xt_spp(const TCHAR * pwd);
	// FindColorE
    virtual CString fp_nc_fce(long x1,long y1,long x2,long y2,const TCHAR * color,double sim,long dir);
	// GetOsType
    virtual long fp_xt_gost();
	// RegExNoMac
    virtual long fp_st_rgexnmc(const TCHAR * code,const TCHAR * Ver,const TCHAR * ip);
	// SetWordLineHeightNoDict
    virtual long fp_xt_swlhnd(long line_height);
	// WriteInt
    virtual long fp_nc_wi(long hwnd,const TCHAR * addr,long tpe,LONGLONG v);
	// SetColGapNoDict
    virtual long fp_xt_scgnd(long col_gap);
	// SetWordGap
    virtual long fp_xt_swgp(long word_gap);
	// DeleteIni
    virtual long fp_xt_dini(const TCHAR * section,const TCHAR * key,const TCHAR * file_name);
	// FindPicS
    virtual CString fp_st_fps(long x1,long y1,long x2,long y2,const TCHAR * pic_name,const TCHAR * delta_color,double sim,long dir,long * x,long * y);
	// RegEx
    virtual long fp_st_rgex(const TCHAR * code,const TCHAR * Ver,const TCHAR * ip);
	// SendCommand
    virtual long fp_xt_scd(const TCHAR * cmd);
	// MoveTo
    virtual long fp_aj_mt(long x,long y);
	// VirtualAllocEx
    virtual LONGLONG fp_nc_vaex(long hwnd,LONGLONG addr,long size,long tpe);
	// SetShowErrorMsg
    virtual long fp_xt_ssem(long show);
	// Capture
    virtual long fp_xt_cpe(long x1,long y1,long x2,long y2,const TCHAR * file_name);
	// KeyDownChar
    virtual long fp_aj_kdc(const TCHAR * key_str);
	// FetchWord
    virtual CString fp_wj_fw(long x1,long y1,long x2,long y2,const TCHAR * color,const TCHAR * word);
	// MiddleClick
    virtual long fp_aj_mck();
	// FindStrE
    virtual CString fp_nc_fse(long x1,long y1,long x2,long y2,const TCHAR * str,const TCHAR * color,double sim);
	// GetLocale
    virtual long fp_xt_gle();
	// WriteData
    virtual long fp_nc_wd(long hwnd,const TCHAR * addr,const TCHAR * data);
	// CheckUAC
    virtual long fp_xt_ckuac();
	// GetDiskSerial
    virtual CString fp_wj_gdsl();
	// SetWindowState
    virtual long fp_wd_swstate(long hwnd,long flag);
	// FoobarTextPrintDir
    virtual long fp_fb_fbtpdir(long hwnd,long dir);
	// MiddleUp
    virtual long fp_aj_mu();
	// GetClientSize
    virtual long fp_wd_gcsz(long hwnd,long * width,long * height);
	// FindWindowEx
    virtual long fp_wd_fwex(long parent,const TCHAR * class_name,const TCHAR * title_name);
	// IsFolderExist
    virtual long fp_wj_ifet(const TCHAR * folder);
	// RGB2BGR
    virtual CString fp_xt_rbtbr(const TCHAR * rgb_color);
	// FoobarClose
    virtual long fp_fb_fce(long hwnd);
	// GetTime
    virtual long fp_xt_gttime();
	// EnableRealKeypad
    virtual long fp_aj_erkd(long en);
	// GetNetTimeByIp
    virtual CString fp_xt_gntbip(const TCHAR * ip);
	// IsDisplayDead
    virtual long fp_wd_idydd(long x1,long y1,long x2,long y2,long t);
	// GetMachineCode
    virtual CString fp_xt_gmc();
	// MiddleDown
    virtual long fp_aj_md();
	// GetBasePath
    virtual CString fp_xt_gbp();

	private:
		bool init(const CString& sft);
};

#endif
