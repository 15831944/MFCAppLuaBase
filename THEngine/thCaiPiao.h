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

// 彩票管理类
class thCaiPiao
{
public:
	thCaiPiao(void);

	// 创建
	static thCaiPiao* createInstance(void);
	static void destroyInstance(void);
	// 运行
	static void run(int delay);

public:
	// 设置获取历史奖号接口
	static void setFunc_KaiJianHistory(const std::function<bool(const std::function<void(std::vector<CString>&vHm)>&)>& func);
	// 设置开奖接口
	static void setFunc_KaiJian(const std::function<bool(const std::function<void(const CString& qihao, const CString& qihaoNext, const CString& hm, long time)>&)>& func);

	// 设置号码显示接口
	static void setFunc_Show(const std::function<void(const CString&qihao, const CString&haoma)>& func);
	// 清空号码显示
	static void setFunc_ShowClear(const std::function<void(void)>& func);


	// 开始获取奖号
	static void startKaiJianHistory(int time=0);
	// 获取当前期号码
	static void startKaiJian(void);
	// 获取开奖倒计时
	static long getKaiJianTime(void)
	{
		return self->m_lOpenTime;
	}
	// 是否正在开奖
	static bool isKaiJianing(void)
	{
		return self->m_bIsKjing;
	}
	// 获取当前期号
	static const CString& getKaiJianQiHao(void)
	{
		return self->m_qihao;
	}
	// 获取下一期期号
	static const CString& getKaiJianQiHaoNext(void)
	{
		return self->m_qihaoNew;
	}
	// 获取全部号码
	static std::vector<CString>& getKaiJianAll(void)
	{
		return self->m_vHaoma;
	}

public:
	// 定时驱动接口
	static void registerFunc(int key, const std::function<void(void)>& func);
	static void unRegisterFunc(int key);
	// 开始驱动
	static void autoFunc(int key, int time);

	// 功能外放函数
public:
	// 设置彩票类型
	static void setCaiPType(int type) { self->m_caiPType = type; }
	static int getCaiPType(void) { return self->m_caiPType; }
	// 获取一段区间里面的热码(qsNum期数，hmNum返回码数,wz位置,isRe正序)
	static CString getReMa_range(int qsNum, int hmNum=5,int wz=1);
	static CString getReMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, bool isRe=true, int wz = 1);
	// 冷吗
	static CString getLengMa_range(int qsNum, int hmNum = 5, int wz = 1);
	static CString getLengMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, int wz = 1);

	// 计算多个号码里面的热码(qsNum期数，hmNum返回码数,wz位置前5，isQ前后,isRe正序)
	static CString getReMa_rangeD(int qsNum, int hmNum = 5, int wz = 5, bool isQ=true);
	static CString getReMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, bool isRe = true, int wz = 5, bool isQ = true);
	// 冷吗
	static CString getLengMa_rangeD(int qsNum, int hmNum = 5, int wz = 5, bool isQ = true);
	static CString getLengMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum = 5, int wz = 5, bool isQ = true);


	// 计算合
	static int getNumHe(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 = -1, int hm5 = -1, int hm6 = -1, int hm7 = -1, int hm8 = -1, int hm9 = -1, int hm10 = -1);
	// 计算跨
	static int getNumKua(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 = -1, int hm5 = -1, int hm6 = -1, int hm7 = -1, int hm8 = -1, int hm9 = -1, int hm10 = -1);

	// 获取彩票类型号码数目
	static int getCaiPHaomaNum(void);
	// 定位胆号码取反
	static CString getDanMa_F(const CString& haoma);
	// 分割单个号码
	static void getHaomaFg(const CString& haoma, int hm[],int count);
	// 号码空格隔开
	static void setHaomaGk(CString& haoma);


	// 直选
public:
	// 获取具体期号
	static int getQihaoJt(const CString& strQihao);
	// 计算下一期
	static int getQihaoNext(int qihao, int add);
	static CString getQihaoNextE(const CString& strQihao, int add);
	// 获取当前在第几期
	static int getJhQihaoIndex(const CString& strQihao);

	// 获取号码注数
	static int getHama_num(const CString& haoma);
	// 号码+1
	static int getHamaAdd(int haoma, int add=1);


	// 3星直选
public:
	// 获取直选号码
	static CString getHaomaXing3(void);
	// 是否是3连
	static bool isHama_lian3(const CString& haoma, int wz = 3);
	// 是否是组3
	static bool isHaoma_zu3(const CString& haoma, int wz = 3);

	// 杀掉3连
	static void shHaoma3_lian3(CString& haoma);
	// 杀掉组3
	static void shHaoma3_zu3(CString& haoma);
	// 杀掉组6
	static void shHaoma3_zu6(CString& haoma);

	// 杀个位定码
	static void shHaoma3_dmG(CString& haoma, const CString& dama);
	// 杀十位定码
	static void shHaoma3_dmS(CString& haoma, const CString& dama);
	// 杀百位定码
	static void shHaoma3_dmB(CString& haoma, const CString& dama);

	// 杀掉 垃圾大底，36注
	static void shHaoma_dd1(CString& haoma);

	// 加上豹子
	static void jhHaoma_baozi(CString& haoma);
	// 定胆，必须在定胆里面(0369)
	static void dmHaoma_dm(CString& haoma, const CString& dama);


	// 内部函数
protected:
	// 开始获取奖号
	void _startKaiJianHistory();
	void _kaiJianHistory(std::vector<CString>& vHm);
	void _kaiJian(const CString& qihao, const CString& qihaoNext, const CString& hm, long time);

	// 杀号码
	void _shHaoma(CString& haoma, CString& sh);
	// 杀定码
	void _shHaoma_dm(CString& haoma, const CString& dama, int wz);


	// 数据变量
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
	// 开奖号码
	std::vector<CString> m_vHaoma;
	CString m_strTemp;
	CString m_strTemp1;
	char m_cbBuffer[1024];
	CString m_qihao;
	CString m_qihaoNew;
	CString m_haoma;
	bool m_bOpenKj;
	bool m_bIsKjing;	// 是否开奖中
	long m_lOpenTime;
	long m_lStartHmTime; // 获取历史号码

	struct thDEC_Hm
	{
		thDEC_Hm(int _hm, int _num) :hm(_hm), num(_num) {}
		int hm;
		int num;
	};
	std::vector<thDEC_Hm> m_vReMa;
};