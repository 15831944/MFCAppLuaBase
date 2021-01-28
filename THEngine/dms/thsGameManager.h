#pragma once

#include "thsPlayer.h"
#include <map>

// ��� �˺� ��Ϸ������
class thsGameManager
{
public:
	thsGameManager();
	~thsGameManager();

	// ����
	static thsGameManager* createInstance(void);
	static void destroyInstance(void);
	inline static thsGameManager* get(void) { return self; }

public:
	// ��Ϸ����·��
	void setGameRunPath(const CString& path) { m_gamePath = path; }
	const CString& getGameRunPath() { return m_gamePath; }

	// ��С�����ֱ��� - ��Ϸ�����ŵ�����С���ڴ�С
	void setGameWdMinSize(int width, int height);

	// ������Ϸ�ֱ���
	void setGameWdSize(int width, int height);
	int getGameWdWidth(void) { return m_width; }
	int getGameWdHeight(void) { return m_height; }

	// ��ȡ�ֱ������ű��� - ���������С�ֱ���
	float getGameWdWidthEps(void) { return m_widthEps; }
	float getGameWdHeightEps(void) { return m_heightEps; }


	// ���ݷ������� ��ȡ��������
	CString getGameRunParames(const CString& serverName);


public:
	// ��������
	void update(int delay);

	// ע�����
	void registerPlayer(const CString& key, thsPlayerBase* player);
	void unregisterPlayer(const CString& key);

	// ��ȡ���
	thsPlayerBase* getPlayer(const CString& key);

	// �����¼� - ��������...
	bool postEvent(const CString& name, thValue* value = nullptr, const std::function<void(const CString& name)>&callfunc = nullptr);
	// �����¼� - ������
	bool sendEvent(const CString& name, thValue* value = nullptr);

protected:
	// �����¼�����
	void _updateEvent(int delay);


	// �����¼�
public:
	// ��ȡ�������
	static int ms_randomMs(int max);
	// �ӳ��������ʱ��
	static void ms_delay(int delay);
	static void ms_delayRandom(int max);

	// �󶨴���
	static bool ms_bdWd(thsPlug* ths, DWORD hwnd);
	// ���
	static void ms_unbdWd(thsPlug* ths);
	// ��鴰���Ƿ����
	static bool ms_checkWd(thsPlug* ths, DWORD hwnd);


	// ����ƶ���Ŀ��λ�ã���������
	static void ms_mvPosClickLeft(thsPlug* ths, int x, int y);

	// ȫѡ�ַ� shift+end
	static void ms_selectAllTxt(thsPlug* ths);
	// �˸�ɾ�������ַ�
	static void ms_bkBackspace(thsPlug* ths, int count);
	// �����ı�
	static void ms_inputStr(thsPlug* ths, DWORD hwnd, const CString& str);

	// �����ַ���
	static bool ms_fdstrfst(thsPlug* ths, long x1, long y1, long x2, long y2, const char* str, const char* color, long * x, long * y, double sim=0.85);


protected:
	static thsGameManager* self;
	CString m_gamePath;		// ��Ϸ����·��
	int m_widthOr;		// ��С�ֱ���
	int m_heightOr;
	int m_width;		// ��ǰ���õķֱ���
	int m_height;
	float m_widthEps;	// �ֱ������ű���
	float m_heightEps;	

	// ��Ҷ���
	std::map<CString,thsPlayerBase*> m_mapPlayer;

	// �¼�����
	struct sEventS
	{
		sEventS(const CString& _name, thValue* _value, const std::function<void(const CString&)>& func) :name(_name), value(_value) { callfunc = func; }
		CString name;
		thValue* value;
		std::function<void(const CString&)> callfunc;
	};
	std::vector<sEventS> m_vEvent;
};

