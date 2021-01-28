/*************************************
*
*   文件：THSMManager.h
*
*   描述：音效，音乐，管理器
*
**************************************/
#pragma once
#ifndef __THSMMANAGER_H__
#define __THSMMANAGER_H__

#include <Windows.h>
#include <map>

#ifdef TH_EXPORT
#define TH_PORT __declspec(dllexport)
#else
#define TH_PORT __declspec(dllimport)
#endif

typedef void* (*Resource_Load)(const char *file,DWORD *size);
typedef void  (*Resource_Free)(void *res);
typedef void  (*Log_Write)(const char *szFormat, ...);

// sound music
typedef DWORD HSOUND;
typedef DWORD HMUSIC;
typedef DWORD HSTREAM;
typedef DWORD HCHANNEL;

// 数据流
typedef std::map<DWORD,void*> StreamMapPtr;
// 数据存储
typedef std::map<DWORD, int> SaveMapPtr;

class TH_PORT THSMManager
{
public:
	THSMManager(void);
	// 外部函数导入
public:
	void		Input_Resource_Load(Resource_Load lpFunc){Resource_Load = lpFunc;}
	void		Input_Resource_Free(Resource_Free lpFunc){Resource_Free = lpFunc;}
	void		Input_Log_Write(Log_Write lpFunc){Log_Write = lpFunc;}

	// 内部函数
public:
	// 装载
	bool		Initialize(HWND hWnd);
	// 释放
	void		Release(void);
	// 数据流
	HSTREAM		Stream_Load(const char *file, DWORD size=0);
	void		Stream_Free(HSTREAM stream);
	HCHANNEL	Stream_Play(HSTREAM stream, bool loop, int volume=100);

	// 音效
	HSOUND		Sound_Load(const char *file, DWORD size=0);
	void		Sound_Free(HSOUND eff);
	HCHANNEL	Sound_Play(HSOUND eff);
	HCHANNEL	Sound_PlayEx(HSOUND eff, int volume=100, int pan=0, float pitch=1.0f, bool loop=false);

	// 音乐
	HMUSIC		Music_Load(const char *file, DWORD size=0);
	void		Music_Free(HMUSIC mus);
	HCHANNEL	Music_Play(HMUSIC mus, bool loop, int volume = 100, int order = 0, int row = 0);
	void		Music_SetAmplification(HMUSIC music, int ampl);
	int			Music_GetAmplification(HMUSIC music);
	int			Music_GetLength(HMUSIC music);
	void		Music_SetPos(HMUSIC music, int order, int row);
	bool		Music_GetPos(HMUSIC music, int *order, int *row);
	void		Music_SetInstrVolume(HMUSIC music, int instr, int volume);
	int			Music_GetInstrVolume(HMUSIC music, int instr);
	void		Music_SetChannelVolume(HMUSIC music, int channel, int volume);
	int			Music_GetChannelVolume(HMUSIC music, int channel);

	// 播放控制器
	void		Channel_SetPanning(HCHANNEL chn, int pan);
	void		Channel_SetVolume(HCHANNEL chn, int volume);
	void		Channel_SetPitch(HCHANNEL chn, float pitch);
	void		Channel_Pause(HCHANNEL chn);
	void		Channel_Resume(HCHANNEL chn);
	void		Channel_Stop(HCHANNEL chn);
	void		Channel_PauseAll(void);
	void		Channel_ResumeAll(void);
	void		Channel_StopAll(void);
	bool		Channel_IsPlaying(HCHANNEL chn);
	bool		Channel_IsStopped(HCHANNEL chn);
	bool		Channel_IsPaused(HCHANNEL chn);
	float		Channel_GetLength(HCHANNEL chn);
	float		Channel_GetPos(HCHANNEL chn);
	void		Channel_SetPos(HCHANNEL chn, float fSeconds);
	void		Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101, float pitch = -1);
	bool		Channel_IsSliding(HCHANNEL channel);

protected:
	// 初始化
	bool		_SoundInit(HWND hWnd);
	// 释放
	void		_SoundDone();
	void		_SetMusVolume(int vol);
	void		_SetStreamVolume(int vol);
	void		_SetFXVolume(int vol);
private:
	// Audio
	HINSTANCE			m_hBass;
	bool				m_bSilent;
	int					m_nSampleRate;
	int					m_nFXVolume;
	int					m_nMusVolume;
	int					m_nStreamVolume;

	// 数据加载
	Resource_Load		Resource_Load;
	// 数据释放
	Resource_Free		Resource_Free;
	// 日记
	Log_Write			Log_Write;

	// 数据结构
	StreamMapPtr		m_mapStream;	// 流
	SaveMapPtr			m_mapSound;		// 音效
	SaveMapPtr			m_mapMusic;		// 音乐
};

#endif /*__THSMMANAGER_H__*/
