#pragma once
#include "THStruct.h"
#include "IDXDefines.h"
#include <functional>

enum cpType
{
	cpType_cqssc,
	cpType_txffc,
	cpType_pk10,
};

// ��Ʊ������
class thCaiPiao
{
public:
	thCaiPiao(void);

	// ����
	static thCaiPiao* createInstance(void);
	static void destroyInstance(void);
	// ����
	static void run(int delay);

public:
	// ���û�ȡ��ʷ���Žӿ�
	static void setFunc_KaiJianHistory(const std::function<bool(const std::function<void(std::vector<CString>&vHm)>&)>& func);
	// ���ÿ����ӿ�
	static void setFunc_KaiJian(const std::function<bool(const std::function<void(const CString& qihao, const CString& qihaoNext, const CString& hm, long time)>&)>& func);

	// ���ú�����ʾ�ӿ�
	static void setFunc_Show(const std::function<void(const CString&qihao, const CString&haoma)>& func);
	// ��պ�����ʾ
	static void setFunc_ShowClear(const std::function<void(void)>& func);


	// ��ʼ��ȡ����
	static void startKaiJianHistory(int time=0);
	// ��ȡ��ǰ�ں���
	static void startKaiJian(void);
	// ��ȡ��������ʱ
	static long getKaiJianTime(void)
	{
		return self->m_lOpenTime;
	}
	// �Ƿ����ڿ���
	static bool isKaiJianing(void)
	{
		return self->m_bIsKjing;
	}
	// ��ȡ��ǰ�ں�
	static const CString& getKaiJianQiHao(void)
	{
		return self->m_qihao;
	}
	// ��ȡ��һ���ں�
	static const CString& getKaiJianQiHaoNext(void)
	{
		return self->m_qihaoNew;
	}
	// ��ȡȫ������
	static std::vector<CString>& getKaiJianAll(void)
	{
		return self->m_vHaoma;
	}

public:
	// ��ʱ�����ӿ�
	static void registerFunc(int key, const std::function<void(void)>& func);
	static void unRegisterFunc(int key);
	// ��ʼ����
	static void autoFunc(int key, int time);

	// ������ź���
public:
	// ���ò�Ʊ����
	static void setCaiPType(int type) { self->m_caiPType = type; }
	static int getCaiPType(void) { return self->m_caiPType; }
	// ��ȡһ���������������(qsNum������hmNum��������,wzλ��,isRe����)
	static CString getReMa_range(int qsNum, int hmNum=5,int wz=1);
	static CString getReMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, bool isRe=true, int wz = 1);
	// ����
	static CString getLengMa_range(int qsNum, int hmNum = 5, int wz = 1);
	static CString getLengMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, int wz = 1);

	// �������������������(qsNum������hmNum��������,wzλ��ǰ5��isQǰ��,isRe����)
	static CString getReMa_rangeD(int qsNum, int hmNum = 5, int wz = 5, bool isQ=true);
	static CString getReMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, bool isRe = true, int wz = 5, bool isQ = true);
	// ����
	static CString getLengMa_rangeD(int qsNum, int hmNum = 5, int wz = 5, bool isQ = true);
	static CString getLengMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, int wz = 5, bool isQ = true);


	// �����
	static int getNumHe(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 = -1, int hm5 = -1, int hm6 = -1, int hm7 = -1, int hm8 = -1, int hm9 = -1, int hm10 = -1);
	// �����
	static int getNumKua(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 = -1, int hm5 = -1, int hm6 = -1, int hm7 = -1, int hm8 = -1, int hm9 = -1, int hm10 = -1);

	// ��ȡ��Ʊ���ͺ�����Ŀ
	static int getCaiPHaomaNum(void);
	// ��λ������ȡ��
	static CString getDanMa_F(const CString& haoma);
	// �ָ������
	static void getHaomaFg(const CString& haoma, int hm[],int count);
	// ����ո����
	static void setHaomaGk(CString& haoma);


	// ֱѡ
public:
	// ��ȡ�����ں�
	static int getQihaoJt(const CString& strQihao);
	// ������һ��
	static int getQihaoNext(int qihao, int add);
	static CString getQihaoNextE(const CString& strQihao, int add);
	// ��ȡ��ǰ�ڵڼ���
	static int getJhQihaoIndex(const CString& strQihao);

	// ��ȡ����ע��
	static int getHama_num(const CString& haoma);
	// ����+1
	static int getHamaAdd(int haoma, int add=1);


	// 3��ֱѡ
public:
	// ��ȡֱѡ����
	static CString getHaomaXing3(void);
	// �Ƿ���3��
	static bool isHama_lian3(const CString& haoma, int wz = 3);
	// �Ƿ�����3
	static bool isHaoma_zu3(const CString& haoma, int wz = 3);

	// ɱ��3��
	static void shHaoma3_lian3(CString& haoma);
	// ɱ����3
	static void shHaoma3_zu3(CString& haoma);
	// ɱ����6
	static void shHaoma3_zu6(CString& haoma);

	// ɱ��λ����
	static void shHaoma3_dmG(CString& haoma, const CString& dama);
	// ɱʮλ����
	static void shHaoma3_dmS(CString& haoma, const CString& dama);
	// ɱ��λ����
	static void shHaoma3_dmB(CString& haoma, const CString& dama);

	// ɱ�� ������ף�36ע
	static void shHaoma_dd1(CString& haoma);

	// ���ϱ���
	static void jhHaoma_baozi(CString& haoma);
	// �����������ڶ�������(0369)
	static void dmHaoma_dm(CString& haoma, const CString& dama);


	// �ڲ�����
protected:
	// ��ʼ��ȡ����
	void _startKaiJianHistory();
	void _kaiJianHistory(std::vector<CString>& vHm);
	void _kaiJian(const CString& qihao, const CString& qihaoNext, const CString& hm, long time);

	// ɱ����
	void _shHaoma(CString& haoma, CString& sh);
	// ɱ����
	void _shHaoma_dm(CString& haoma, const CString& dama, int wz);


	// ���ݱ���
protected:
	static thCaiPiao* self;
	std::function<bool(const std::function<void(std::vector<CString>&)>&)> m_funcKaiJianHistory;
	std::function<bool(const std::function<void(const CString& qihao, const CString& qihaoNext, const CString& hm, long time)>&)> m_funcKaiJian;
	std::function<void(const CString&, const CString&)> m_funcShowHm;
	std::function<void(void)> m_funcShowHmClear;

	struct thDEC_Func
	{
		thDEC_Func(const std::function<void(void)>& _func)
			:func(_func), bOpen(false), time(0)
		{

		}
		bool bOpen;
		int time;
		std::function<void(void)> func;
	};
	std::map<int, thDEC_Func> m_mapFunc;

public:
	int m_caiPType;
	CString m_gjFile;
	int m_lsJlNum;
	// ��������
	std::vector<CString> m_vHaoma;
	CString m_strTemp;
	CString m_strTemp1;
	char m_cbBuffer[1024];
	CString m_qihao;
	CString m_qihaoNew;
	CString m_haoma;
	bool m_bOpenKj;
	bool m_bIsKjing;	// �Ƿ񿪽���
	long m_lOpenTime;
	long m_lStartHmTime; // ��ȡ��ʷ����

	struct thDEC_Hm
	{
		thDEC_Hm(int _hm, int _num) :hm(_hm), num(_num) {}
		int hm;
		int num;
	};
	std::vector<thDEC_Hm> m_vReMa;
};