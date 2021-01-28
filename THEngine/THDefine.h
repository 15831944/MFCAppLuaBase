/*************************************
*
*   �ļ���THDefine.h
*
*   �������궨���ļ�
*
**************************************/
#pragma once

// ��ԿKey
#define TH_KEY_DES		  _T("12345678")

// ���֡��
#define TH_FRAME_MAX	  128

// buff size
#define BUF_SIZE_MAIN			1024
#define BUF_SIZE_MAINMAX		10240

// ��ʱ��
#define TH_TIMER_UPDATE       101
#define TH_TIMER_TIMERUN	  102
#define TH_TIMER_POPWINDOWTS  103		// ������ʾ��
#define TH_TIMER_POPWINDOWMSG 104		// ��ʾ��

// ��Ϣ
#define TH_BTN_MESSAGE	  WM_USER+0
#define TH_MESSAGE		  WM_USER+1

#define TH_POP_MSG		  20000		// ���� ��Ϣ
#define TH_POP_SOUND	  20001		// ���� ����
#define TH_POP_IMG		  20002		// ���� ͼƬ
#define TH_POP_VIDEO	  20003		// ���� ��Ƶ
#define TH_POP_CZLV		  20004		// ���� ������¼
#define TH_POP_LOAD		  20005		// ���� ���ؽ���

// Menu
#define TH_MENU_EDIT	  10001		// �༭
#define TH_MENU_DELETE	  10002		// ɾ��
#define TH_MENU_EXIT	  10003		// �˳�
#define TH_MENU_TOMOST	  10004		// �����ö�

#define TH_MENU_EVKJ	  11001		// ��ݲ���
#define TH_MENU_EVTM	  11002		// ��ʱ�¼�
#define TH_MENU_EVACT	  11003		// ���ж���

#define TH_MENU_FJGN	  12001		// ���ӹ���


// ������Pop
#define TH_MENUPOP_M1	  13001		// ����1
#define TH_MENUPOP_M2	  13002		// ����2
#define TH_MENUPOP_M3	  13003		// ����3
#define TH_MENUPOP_M4	  13004		// ����4


#define TH_BT_CLOSE		  100	// �ر�
#define TH_BT_PLAY		  101	// ������Ƶ
#define TH_BT_PLAYL		  102	// ������һ��
#define TH_BT_PLAYR		  103	// ������һ��
#define TH_BT_SOUND		  104	// ����

#define TH_BT_CZLV		  105	// ������¼
#define TH_BT_LJRUN		  106	// ����ִ��
#define TH_BT_NORUN		  107	// �ݲ�ִ��

// ������
#define TH_VAL_SOUND	  200	// ����
#define TH_VAL_JINDU	  201	// ����

// �߳���Ϣ
#define TH_THREAD_POPIMG  1000	// img

// file
#define		TEMP_FILE_TIMETM	 _T("Config/RecordSys/Time_omp.th")
#define		TEMP_FILE_TIMEPRO	 _T("Time_pro.temp")
#define		TEMP_FILE_TIMEWND	 _T("Time_wnd.temp")

#define		TH_FILE_TIMETM	 _T("Config/RecordSys/Time.th")
#define		TH_FILE_TIMETM_TMP _T("Config/RecordSys/Time_tm.th")
#define		TH_FILE_TIMEPRO_TMP _T("Config/RecordSys/Time_pro.th")
#define		TH_FILE_TIMEWND_TMP _T("Config/RecordSys/Time_wnd.th")

