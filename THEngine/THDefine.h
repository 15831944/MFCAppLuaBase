/*************************************
*
*   文件：THDefine.h
*
*   描述：宏定义文件
*
**************************************/
#pragma once

// 密钥Key
#define TH_KEY_DES		  _T("12345678")

// 最大帧数
#define TH_FRAME_MAX	  128

// buff size
#define BUF_SIZE_MAIN			1024
#define BUF_SIZE_MAINMAX		10240

// 计时器
#define TH_TIMER_UPDATE       101
#define TH_TIMER_TIMERUN	  102
#define TH_TIMER_POPWINDOWTS  103		// 启动提示框
#define TH_TIMER_POPWINDOWMSG 104		// 提示框

// 消息
#define TH_BTN_MESSAGE	  WM_USER+0
#define TH_MESSAGE		  WM_USER+1

#define TH_POP_MSG		  20000		// 窗口 消息
#define TH_POP_SOUND	  20001		// 窗口 音乐
#define TH_POP_IMG		  20002		// 窗口 图片
#define TH_POP_VIDEO	  20003		// 窗口 视频
#define TH_POP_CZLV		  20004		// 窗口 操作记录
#define TH_POP_LOAD		  20005		// 窗口 加载进度

// Menu
#define TH_MENU_EDIT	  10001		// 编辑
#define TH_MENU_DELETE	  10002		// 删除
#define TH_MENU_EXIT	  10003		// 退出
#define TH_MENU_TOMOST	  10004		// 窗口置顶

#define TH_MENU_EVKJ	  11001		// 快捷操作
#define TH_MENU_EVTM	  11002		// 定时事件
#define TH_MENU_EVACT	  11003		// 安仔动作

#define TH_MENU_FJGN	  12001		// 附加功能


// 播放器Pop
#define TH_MENUPOP_M1	  13001		// 功能1
#define TH_MENUPOP_M2	  13002		// 功能2
#define TH_MENUPOP_M3	  13003		// 功能3
#define TH_MENUPOP_M4	  13004		// 功能4


#define TH_BT_CLOSE		  100	// 关闭
#define TH_BT_PLAY		  101	// 播放视频
#define TH_BT_PLAYL		  102	// 播放上一个
#define TH_BT_PLAYR		  103	// 播放下一个
#define TH_BT_SOUND		  104	// 声音

#define TH_BT_CZLV		  105	// 操作记录
#define TH_BT_LJRUN		  106	// 立即执行
#define TH_BT_NORUN		  107	// 暂不执行

// 滑动条
#define TH_VAL_SOUND	  200	// 声音
#define TH_VAL_JINDU	  201	// 进度

// 线程消息
#define TH_THREAD_POPIMG  1000	// img

// file
#define		TEMP_FILE_TIMETM	 _T("Config/RecordSys/Time_omp.th")
#define		TEMP_FILE_TIMEPRO	 _T("Time_pro.temp")
#define		TEMP_FILE_TIMEWND	 _T("Time_wnd.temp")

#define		TH_FILE_TIMETM	 _T("Config/RecordSys/Time.th")
#define		TH_FILE_TIMETM_TMP _T("Config/RecordSys/Time_tm.th")
#define		TH_FILE_TIMEPRO_TMP _T("Config/RecordSys/Time_pro.th")
#define		TH_FILE_TIMEWND_TMP _T("Config/RecordSys/Time_wnd.th")

