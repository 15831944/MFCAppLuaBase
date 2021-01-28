/*************************************
*
*   �ļ���THSMManager.h
*
*   ��������Ч�����֣�������
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

// ������
typedef std::map<DWORD,void*> StreamMapPtr;
// ���ݴ洢
typedef std::map<DWORD, int> SaveMapPtr;

class TH_PORT THSMManager
{
public:
	THSMManager(void);
	// �ⲿ��������
public:
	void		Input_Resource_Load(Resource_Load lpFunc){Resource_Load = lpFunc;}
	void		Input_Resource_Free(Resource_Free lpFunc){Resource_Free = lpFunc;}
	void		Input_Log_Write(Log_Write lpFunc){Log_Write = lpFunc;}

	// �ڲ�����
public:
	// װ��
	bool		Initialize(HWND hWnd);
	// �ͷ�
	void		Release(void);
	// ������
	HSTREAM		Stream_Load(const char *file, DWORD size=0);
	void		Stream_Free(HSTREAM stream);
	HCHANNEL	Stream_Play(HSTREAM stream, bool loop, int volume=100);

	// ��Ч
	HSOUND		Sound_Load(const char *file, DWORD size=0);
	void		Sound_Free(HSOUND eff);
	HCHANNEL	Sound_Play(HSOUND eff);
	HCHANNEL	Sound_PlayEx(HSOUND eff, int volume=100, int pan=0, float pitch=1.0f, bool loop=false);

	// ����
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

	// ���ſ�����
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
	// ��ʼ��
	bool		_SoundInit(HWND hWnd);
	// �ͷ�
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

	// ���ݼ���
	Resource_Load		Resource_Load;
	// �����ͷ�
	Resource_Free		Resource_Free;
	// �ռ�
	Log_Write			Log_Write;

	// ���ݽṹ
	StreamMapPtr		m_mapStream;	// ��
	SaveMapPtr			m_mapSound;		// ��Ч
	SaveMapPtr			m_mapMusic;		// ����
};

#endif /*__THSMMANAGER_H__*/
