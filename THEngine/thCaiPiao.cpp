#include "pch.h"
#include "thCaiPiao.h"

thCaiPiao* thCaiPiao::self = nullptr;

thCaiPiao::thCaiPiao(void)
	: m_lOpenTime(-1)
	, m_bOpenKj(false)
	, m_bIsKjing(false)
	, m_lStartHmTime(-1)
	, m_caiPType(cpType_cqssc)
	, m_lsJlNum(1000)
{

}

thCaiPiao* thCaiPiao::createInstance(void)
{
	if (!self)
	{
		self = new thCaiPiao();
	}
	return self;
}

void thCaiPiao::destroyInstance(void)
{
	if (self)
	{
		delete self;
	}
}

void thCaiPiao::run(int delay)
{
	// 获取历史号码
	if (self->m_lStartHmTime >= 0)
	{
		self->m_lStartHmTime -= delay;
		if (self->m_lStartHmTime <= 0)
		{
			self->m_lStartHmTime = -1;
			self->_startKaiJianHistory();
		}
	}

	// 开奖
	if (self->m_bOpenKj)
	{
		self->m_lOpenTime -= delay;
		if (self->m_lOpenTime <= 0)
		{
			self->m_lOpenTime = 0;
			self->m_bOpenKj = false;
			startKaiJian();
		}
	}

	// call
	for (auto &m: self->m_mapFunc)
	{
		auto &v = m.second;
		if (v.bOpen)
		{
			v.time -= delay;
			if (v.time <= 0)
			{
				v.bOpen = false;
				if (v.func)
				{
					v.func();
				}
			}
		}
	}
}

void thCaiPiao::setFunc_KaiJianHistory(const std::function<bool(const std::function<void(std::vector<CString>&)>&)>& func)
{
	self->m_funcKaiJianHistory = func;
}

void thCaiPiao::setFunc_KaiJian(const std::function<bool(const std::function<void(const CString& qihao, const CString& qihaoNext, const CString& hm, long time)>&)>& func)
{
	self->m_funcKaiJian = func;
}

void thCaiPiao::setFunc_Show(const std::function<void(const CString&qihao, const CString&haoma)>& func)
{
	self->m_funcShowHm = func;
}

void thCaiPiao::setFunc_ShowClear(const std::function<void(void)>& func)
{
	self->m_funcShowHmClear = func;
}

void thCaiPiao::startKaiJianHistory(int time)
{
	self->m_lStartHmTime = time;
}

void thCaiPiao::startKaiJian(void)
{
	if (self->m_funcKaiJian)
	{
		self->m_bOpenKj = false;
		self->m_bIsKjing = true;
		if (!self->m_funcKaiJian(CC_CALLBACK_4(thCaiPiao::_kaiJian, self)))
		{
			theApp.m_pMainWnd->MessageBox("获取开奖号码失败，联系作者！", "提示");
			return;
		}
	}
}

void thCaiPiao::registerFunc(int key, const std::function<void(void)>& func)
{
	self->m_mapFunc.emplace(key, thDEC_Func(func));
}

void thCaiPiao::unRegisterFunc(int key)
{
	auto iter = self->m_mapFunc.find(key);
	if (iter != self->m_mapFunc.end())
	{
		self->m_mapFunc.erase(key);
	}
}

void thCaiPiao::autoFunc(int key, int time)
{
	auto iter = self->m_mapFunc.find(key);
	if (iter != self->m_mapFunc.end())
	{
		auto &v = iter->second;
		v.bOpen = true;
		v.time = time;
	}
}

bool SortbysHm(thCaiPiao::thDEC_Hm& x, thCaiPiao::thDEC_Hm& y)
{
	return x.num > y.num;
}

CString thCaiPiao::getReMa_range(int qsNum, int hmNum/*=5*/, int idx)
{
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	return getReMa_rangeEx(self->m_vHaoma.size()-qsNum, self->m_vHaoma.size(), hmNum, true, idx);
}

CString thCaiPiao::getReMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum /*= 5*/, bool isRe, int idx)
{
	if (hmNum > 10)
	{
		return "";
	}
	int qsNum = qsNumEnd - qsNumBegin;
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	int of = 0;
	int cnt = 10;
	if (self->m_caiPType == cpType_pk10)
	{
		of = 1;
		// cnt = 10;
	}
	if (self->m_vReMa.size() != cnt)
	{
		self->m_vReMa.clear();
		for (int i = 0; i < cnt; ++i)
		{
			self->m_vReMa.push_back(thDEC_Hm(i + of, 0));
		}
	}
	for (int i = 0; i < cnt; ++i)
	{
		self->m_vReMa[i].hm = i + of;
		self->m_vReMa[i].num = 0;
	}
	CString strNum;
	for (int i = qsNumBegin; i < qsNumEnd; ++i)
	{
		self->m_strTemp = self->m_vHaoma[i];
		auto qihao = thHelper::analyString(self->m_strTemp, "|");
		int haoma1 = 1;
		if (self->m_strTemp.Find(',')!=-1)
		{
			for (int ix = 0; ix < idx; ++ix)
			{
				haoma1 = thHelper::analyInt(self->m_strTemp, ",");
			}
		}
		else
		{
			strNum = self->m_strTemp[idx-1];
			haoma1 = atoi(strNum);
		}
		++self->m_vReMa[haoma1 - of].num;
	}
	std::sort(self->m_vReMa.begin(), self->m_vReMa.end(), SortbysHm);
	self->m_strTemp = "";
	const char* strTp = "%d ";
	if (self->m_caiPType == cpType_pk10)
	{
		strTp = "%02d ";
	}
	if (isRe)
	{
		for (int i = 0; i < hmNum; ++i)
		{
			self->m_strTemp.AppendFormat(strTp, self->m_vReMa[i].hm);
		}
	}
	else
	{
		int iA = cnt - hmNum;
		for (int i = iA; i < cnt; ++i)
		{
			self->m_strTemp.AppendFormat(strTp, self->m_vReMa[i].hm);
		}
	}
	
	return self->m_strTemp;
}

CString thCaiPiao::getLengMa_range(int qsNum, int hmNum /*= 5*/, int idx)
{
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	return getReMa_rangeEx(self->m_vHaoma.size() - qsNum, self->m_vHaoma.size(), hmNum, false, idx);
}

CString thCaiPiao::getLengMa_rangeEx(int qsNumBegin, int qsNumEnd, int hmNum /*= 5*/, int idx)
{
	return getReMa_rangeEx(qsNumBegin, qsNumEnd, hmNum, false, idx);
}

CString thCaiPiao::getReMa_rangeD(int qsNum, int hmNum /*= 5*/, int wz /*= 5*/, bool isQ/*=true*/)
{
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	return getReMa_rangeDEx(self->m_vHaoma.size() - qsNum, self->m_vHaoma.size(), hmNum, true, wz, isQ);
}

CString thCaiPiao::getReMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum /*= 5*/, bool isRe /*= true*/, int wz /*= 5*/, bool isQ /*= true*/)
{
	if (hmNum > 10)
	{
		return "";
	}
	int qsNum = qsNumEnd - qsNumBegin;
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	int of = 0;
	int cnt = 10;
	int iHmnm = 5;
	int iB = 0;
	int iE = wz;
	if (self->m_caiPType == cpType_pk10)
	{
		of = 1;
		iHmnm = 10;
	}
	if (!isQ && iHmnm != wz)
	{
		iB = iHmnm - wz;
		iE = iHmnm;
	}
	if (self->m_vReMa.size() != cnt)
	{
		self->m_vReMa.clear();
		for (int i = 0; i < cnt; ++i)
		{
			self->m_vReMa.push_back(thDEC_Hm(i + of, 0));
		}
	}
	for (int i = 0; i < cnt; ++i)
	{
		self->m_vReMa[i].hm = i + of;
		self->m_vReMa[i].num = 0;
	}
	CString strNum;

	for (int i = qsNumBegin; i < qsNumEnd; ++i)
	{
		self->m_strTemp = self->m_vHaoma[i];
		auto qihao = thHelper::analyString(self->m_strTemp, "|");
		int haoma = 1;
		if (self->m_strTemp.Find(',') != -1)
		{
			for (int ix = iB; ix < iE; ++ix)
			{
				haoma = thHelper::analyInt(self->m_strTemp, ",");
				++self->m_vReMa[haoma - of].num;
			}
		}
		else
		{
			for (int ix = iB; ix < iE; ++ix)
			{
				strNum = self->m_strTemp[ix];
				haoma = atoi(strNum);
				++self->m_vReMa[haoma - of].num;
			}
			
		}
	}
	std::sort(self->m_vReMa.begin(), self->m_vReMa.end(), SortbysHm);
	self->m_strTemp = "";
	const char* strTp = "%d ";
	if (self->m_caiPType == cpType_pk10)
	{
		strTp = "%02d ";
	}
	if (isRe)
	{
		for (int i = 0; i < hmNum; ++i)
		{
			self->m_strTemp.AppendFormat(strTp, self->m_vReMa[i].hm);
		}
	}
	else
	{
		int iA = cnt - hmNum;
		for (int i = iA; i < cnt; ++i)
		{
			self->m_strTemp.AppendFormat(strTp, self->m_vReMa[i].hm);
		}
	}

	return self->m_strTemp;
}

CString thCaiPiao::getLengMa_rangeD(int qsNum, int hmNum /*= 5*/, int wz /*= 5*/, bool isQ /*= true*/)
{
	if ((int)self->m_vHaoma.size() < qsNum)
	{
		return "";
	}
	return getReMa_rangeDEx(self->m_vHaoma.size() - qsNum, self->m_vHaoma.size(), hmNum, false , wz, isQ);
}

CString thCaiPiao::getLengMa_rangeDEx(int qsNumBegin, int qsNumEnd, int hmNum /*= 5*/, int wz /*= 5*/, bool isQ /*= true*/)
{
	return getReMa_rangeDEx(qsNumBegin, qsNumEnd, hmNum, false, wz, isQ);
}

CString thCaiPiao::getDanMa_F(const CString& haoma)
{
	CString strHaoma;
	CString strTemp;
	if (self->m_caiPType == cpType_pk10)
	{
		for (int i = 1; i < 11; ++i)
		{
			strTemp.Format("%02d", i);
			if (haoma.Find(strTemp) == -1)
			{
				strHaoma.AppendFormat("%02d ", i);
			}
		}
	}
	else
	{
		for (int i = 0; i < 10; ++i)
		{
			strTemp.Format("%d", i);
			if (haoma.Find(strTemp) == -1)
			{
				strHaoma.AppendFormat("%d ", i);
			}
		}
	}
	return strHaoma;
}

int thCaiPiao::getQihaoNext(int qihao, int add)
{
	auto num = qihao + add;
	if (self->m_caiPType == cpType_pk10)
	{
		if (num > 999)
		{
			return num - 1000;
		}
	}
	else if (self->m_caiPType == cpType_txffc)
	{
		if (num == 1440)
		{
			return 1;
		}
		if (num > 1440)
		{
			return num - 1440;
		}
	}
	else if (self->m_caiPType == cpType_cqssc)
	{
		if (num == 120)
		{
			return 1;
		}
		if (num > 120)
		{
			return num - 120;
		}
	}
	return num;
}

CString thCaiPiao::getQihaoNextE(const CString& strQihao, int add)
{
	auto qihao = getQihaoJt(strQihao);
	if (self->getCaiPType() == cpType_txffc)
	{
		self->m_strTemp.Format("%04d", qihao);
	}
	else
	{
		self->m_strTemp.Format("%03d", qihao);
	}
	self->m_strTemp1 = strQihao;
	self->m_strTemp1.Replace(self->m_strTemp,"");
	auto qihaoNext = getQihaoNext(qihao, add);
	if (self->getCaiPType() == cpType_txffc)
	{
		self->m_strTemp1.AppendFormat("%04d", qihaoNext);
	}
	else
	{
		self->m_strTemp1.AppendFormat("%03d", qihaoNext);
	}
	return self->m_strTemp1;
}

void thCaiPiao::getHaomaFg(const CString& haoma, int hm[], int count)
{
	CString strTemp;
	if (haoma.Find(',')!=-1)
	{
		strTemp = haoma;
		for (int i = 0; i < count; ++i)
		{
			hm[i] = thHelper::analyInt(strTemp, ",");
		}
	}
	else
	{
		for (int i = 0; i < count; ++i)
		{
			strTemp = haoma[i];
			hm[i] = atoi(strTemp);
		}
	}
}

void thCaiPiao::setHaomaGk(CString& haoma)
{
	if (haoma.Find(',') != -1)
	{
		haoma.Replace(",", " ");
	}
	else
	{
		const char* cpTp = "%d ";
		if (self->m_caiPType == cpType_pk10)
		{
			cpTp = "%02d ";
		}
		CString strTemp,num;
		int count = haoma.GetLength();
		for (int i=0; i<count; ++i)
		{
			num = haoma[i];
			strTemp.AppendFormat(cpTp, atoi(num));
		}
		haoma = strTemp;
	}
}

int thCaiPiao::getQihaoJt(const CString& strQihao)
{
	CString strTemp;
	if (self->getCaiPType() == cpType_txffc)
	{
		strTemp = strQihao.Mid(strQihao.GetLength() - 4);
	}
	else
	{
		strTemp = strQihao.Mid(strQihao.GetLength() - 3);
	}
	return atoi(strTemp);
}

int thCaiPiao::getJhQihaoIndex(const CString& strQihao)
{
	CString strTemp;
	auto qihao = strQihao;
	auto qihaoI = thHelper::analyInt(qihao, "-");
	auto qihaoE = thHelper::analyInt(qihao, "-");
	strTemp = thCaiPiao::getKaiJianQiHaoNext();
	if (self->getCaiPType() == cpType_txffc)
	{
		strTemp = strTemp.Mid(strTemp.GetLength() - 4);
	}
	else
	{
		strTemp = strTemp.Mid(strTemp.GetLength() - 3);
	}
	auto qihaoNew = atoi(strTemp);
	for (int i = 0; i < 10; ++i)
	{
		if (qihaoI + i > qihaoE)
		{
			break;
		}
		if (qihaoNew == qihaoI + i)
		{
			return i + 1;
		}
	}
	return 1;
}

int thCaiPiao::getNumHe(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 /*= -1*/, int hm5 /*= 0*/, int hm6 /*= 0*/, int hm7 /*= 0*/, int hm8 /*= 0*/, int hm9 /*= 0*/, int hm10 /*= 0*/)
{
	int count = 0;
	int hmNum[10] = { hm1,hm2,hm3,hm4,hm5,hm6,hm7,hm8,hm9,hm10 };
	for (int i = 0; i < 10; ++i)
	{
		if (hmNum[i] == -1)
		{
			break;
		}
		++count;
	}
	if (num < 0 || num >count)
	{
		num = 0;
	}

	int he = 0;
	int i = 0;
	int iEnd = num;
	if (!isQ)
	{
		i = count - num;
		num += i;
	}
	for (; i < num; ++i)
	{
		he += hmNum[i];
	}
	
	return he % 10;
}

int thCaiPiao::getNumKua(bool isQ, int num, int hm1, int hm2, int hm3, int hm4 /*= -1*/, int hm5 /*= 0*/, int hm6 /*= 0*/, int hm7 /*= 0*/, int hm8 /*= 0*/, int hm9 /*= 0*/, int hm10 /*= 0*/)
{
	int count = 0;
	int hmNum[10] = { hm1,hm2,hm3,hm4,hm5,hm6,hm7,hm8,hm9,hm10 };
	for (int i = 0; i < 10; ++i)
	{
		if (hmNum[i] == -1)
		{
			break;
		}
		++count;
	}
	if (num < 0 || num >count)
	{
		num = 0;
	}

	int kMax = 0;
	int kMin = 100;
	int i = 0;
	int iEnd = num;
	if (!isQ)
	{
		i = count - num;
		num += i;
	}

	for (; i < num; ++i)
	{
		if (hmNum[i] > kMax)
		{
			kMax = hmNum[i];
		}
		if (hmNum[i] < kMin)
		{
			kMin = hmNum[i];
		}
	}
	return kMax-kMin;
}

int thCaiPiao::getCaiPHaomaNum(void)
{
	if (self->m_caiPType == cpType_pk10)
	{
		return 10;
	}

	return 5;
}

int thCaiPiao::getHama_num(const CString& haoma)
{
	int count = 0;
	self->m_strTemp = haoma;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		++count;
	}
	return count;
}

int thCaiPiao::getHamaAdd(int haoma, int add/*=1*/)
{
	auto num = haoma + add;
	if (self->m_caiPType == cpType_pk10)
	{
		if (num > 10)
		{
			return num - 10;
		}
		return num;
	}
	else
	{
		if (num > 9)
		{
			return num - 9;
		}
		return num;
	}
}

static CString g_xing3 = "000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 021 022 023 024 025 026 027 028 029 030 031 032 033 034 035 036 037 038 039 040 041 042 043 044 045 046 047 048 049 050 051 052 053 054 055 056 057 058 059 060 061 062 063 064 065 066 067 068 069 070 071 072 073 074 075 076 077 078 079 080 081 082 083 084 085 086 087 088 089 090 091 092 093 094 095 096 097 098 099 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 ";
CString thCaiPiao::getHaomaXing3(void)
{
	return g_xing3;
}

static CString g_xing3_lian3 = "012 019 021 089 091 098 102 109 120 123 132 190 201 210 213 231 234 243 312 321 324 342 345 354 423 432 435 453 456 465 534 543 546 564 567 576 645 654 657 675 678 687 756 765 768 786 789 798 809 867 876 879 890 897 901 908 910 978 980 987 ";
bool thCaiPiao::isHama_lian3(const CString& haoma, int wz)
{
	int hm[5];
	getHaomaFg(haoma, hm, 5);
	if (wz == 1)
	{
		self->m_strTemp.Format("%d%d%d", hm[0], hm[1], hm[2]);
	}
	else if (wz == 2)
	{
		self->m_strTemp.Format("%d%d%d", hm[1], hm[2], hm[3]);
	}
	else
	{
		self->m_strTemp.Format("%d%d%d", hm[2], hm[3], hm[4]);
	}
	if (g_xing3_lian3.Find(self->m_strTemp) != -1)
	{
		return true;
	}
	return false;
}

bool thCaiPiao::isHaoma_zu3(const CString& haoma, int wz)
{
	int hm[5];
	getHaomaFg(haoma, hm, 5);
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	if (wz == 1)
	{
		num1 = hm[0]; num2 = hm[1]; num3 = hm[2];
	}
	else if (wz == 2)
	{
		num1 = hm[1]; num2 = hm[2]; num3 = hm[3];
	}
	else
	{
		num1 = hm[2]; num2 = hm[3]; num3 = hm[4];
	}
	if (num1 == num2 && num1 != num3 || num1 == num3 && num1 != num2)
	{
		return true;
	}
	return false;
}

void thCaiPiao::shHaoma3_lian3(CString& haoma)
{
	self->_shHaoma(haoma, g_xing3_lian3);
}

static CString g_xing3_zu3 = "001 002 003 004 005 006 007 008 009 010 011 020 022 030 033 040 044 050 055 060 066 070 077 080 088 090 099 100 101 110 112 113 114 115 116 117 118 119 121 122 131 133 141 144 151 155 161 166 171 177 181 188 191 199 200 202 211 212 220 221 223 224 225 226 227 228 229 232 233 242 244 252 255 262 266 272 277 282 288 292 299 300 303 311 313 322 323 330 331 332 334 335 336 337 338 339 343 344 353 355 363 366 373 377 383 388 393 399 400 404 411 414 422 424 433 434 440 441 442 443 445 446 447 448 449 454 455 464 466 474 477 484 488 494 499 500 505 511 515 522 525 533 535 544 545 550 551 552 553 554 556 557 558 559 565 566 575 577 585 588 595 599 600 606 611 616 622 626 633 636 644 646 655 656 660 661 662 663 664 665 667 668 669 676 677 686 688 696 699 700 707 711 717 722 727 733 737 744 747 755 757 766 767 770 771 772 773 774 775 776 778 779 787 788 797 799 800 808 811 818 822 828 833 838 844 848 855 858 866 868 877 878 880 881 882 883 884 885 886 887 889 898 899 900 909 911 919 922 929 933 939 944 949 955 959 966 969 977 979 988 989 990 991 992 993 994 995 996 997 998 ";
void thCaiPiao::shHaoma3_zu3(CString& haoma)
{
	self->_shHaoma(haoma, g_xing3_zu3);
}

static CString g_xing3_zu6 = "012 013 014 015 016 017 018 019 021 023 024 025 026 027 028 029 031 032 034 035 036 037 038 039 041 042 043 045 046 047 048 049 051 052 053 054 056 057 058 059 061 062 063 064 065 067 068 069 071 072 073 074 075 076 078 079 081 082 083 084 085 086 087 089 091 092 093 094 095 096 097 098 102 103 104 105 106 107 108 109 120 123 124 125 126 127 128 129 130 132 134 135 136 137 138 139 140 142 143 145 146 147 148 149 150 152 153 154 156 157 158 159 160 162 163 164 165 167 168 169 170 172 173 174 175 176 178 179 180 182 183 184 185 186 187 189 190 192 193 194 195 196 197 198 201 203 204 205 206 207 208 209 210 213 214 215 216 217 218 219 230 231 234 235 236 237 238 239 240 241 243 245 246 247 248 249 250 251 253 254 256 257 258 259 260 261 263 264 265 267 268 269 270 271 273 274 275 276 278 279 280 281 283 284 285 286 287 289 290 291 293 294 295 296 297 298 301 302 304 305 306 307 308 309 310 312 314 315 316 317 318 319 320 321 324 325 326 327 328 329 340 341 342 345 346 347 348 349 350 351 352 354 356 357 358 359 360 361 362 364 365 367 368 369 370 371 372 374 375 376 378 379 380 381 382 384 385 386 387 389 390 391 392 394 395 396 397 398 401 402 403 405 406 407 408 409 410 412 413 415 416 417 418 419 420 421 423 425 426 427 428 429 430 431 432 435 436 437 438 439 450 451 452 453 456 457 458 459 460 461 462 463 465 467 468 469 470 471 472 473 475 476 478 479 480 481 482 483 485 486 487 489 490 491 492 493 495 496 497 498 501 502 503 504 506 507 508 509 510 512 513 514 516 517 518 519 520 521 523 524 526 527 528 529 530 531 532 534 536 537 538 539 540 541 542 543 546 547 548 549 560 561 562 563 564 567 568 569 570 571 572 573 574 576 578 579 580 581 582 583 584 586 587 589 590 591 592 593 594 596 597 598 601 602 603 604 605 607 608 609 610 612 613 614 615 617 618 619 620 621 623 624 625 627 628 629 630 631 632 634 635 637 638 639 640 641 642 643 645 647 648 649 650 651 652 653 654 657 658 659 670 671 672 673 674 675 678 679 680 681 682 683 684 685 687 689 690 691 692 693 694 695 697 698 701 702 703 704 705 706 708 709 710 712 713 714 715 716 718 719 720 721 723 724 725 726 728 729 730 731 732 734 735 736 738 739 740 741 742 743 745 746 748 749 750 751 752 753 754 756 758 759 760 761 762 763 764 765 768 769 780 781 782 783 784 785 786 789 790 791 792 793 794 795 796 798 801 802 803 804 805 806 807 809 810 812 813 814 815 816 817 819 820 821 823 824 825 826 827 829 830 831 832 834 835 836 837 839 840 841 842 843 845 846 847 849 850 851 852 853 854 856 857 859 860 861 862 863 864 865 867 869 870 871 872 873 874 875 876 879 890 891 892 893 894 895 896 897 901 902 903 904 905 906 907 908 910 912 913 914 915 916 917 918 920 921 923 924 925 926 927 928 930 931 932 934 935 936 937 938 940 941 942 943 945 946 947 948 950 951 952 953 954 956 957 958 960 961 962 963 964 965 967 968 970 971 972 973 974 975 976 978 980 981 982 983 984 985 986 987 ";
void thCaiPiao::shHaoma3_zu6(CString& haoma)
{
	self->_shHaoma(haoma, g_xing3_zu6);
}

void thCaiPiao::shHaoma3_dmG(CString& haoma, const CString& dama)
{
	self->_shHaoma_dm(haoma, dama, 3);
}

void thCaiPiao::shHaoma3_dmS(CString& haoma, const CString& dama)
{
	self->_shHaoma_dm(haoma, dama, 2);
}

void thCaiPiao::shHaoma3_dmB(CString& haoma, const CString& dama)
{
	self->_shHaoma_dm(haoma, dama, 1);
}


CString g_sh_wdDd1 = "001 008 009 010 011 080 090 099 100 101 110 199 355 445 446 454 455 464 535 544 545 553 554 644 800 889 898 899 900 909 919 988 989 990 991 998 ";
void thCaiPiao::shHaoma_dd1(CString& haoma)
{
	self->m_strTemp = g_sh_wdDd1;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		if (haoma.Find(self->m_strTemp1) != -1)
		{
			self->m_strTemp1.Append(" ");
			haoma.Replace(self->m_strTemp1, "");
		}
	}
}

CString g_jm_bz1 = "000 111 222 333 444 555 666 777 888 999 ";
void thCaiPiao::jhHaoma_baozi(CString& haoma)
{
	self->m_strTemp = g_jm_bz1;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		if (haoma.Find(self->m_strTemp1) == -1)
		{
			self->m_strTemp1.Append(" ");
			haoma.Append(self->m_strTemp1);
		}
	}
}

void thCaiPiao::dmHaoma_dm(CString& haoma, const CString& dama)
{
	self->m_strTemp = haoma;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		bool isFind = false;
		auto len = self->m_strTemp1.GetLength();
		for (int i=0; i<len; ++i)
		{
			if (dama.Find(self->m_strTemp1[i]) != -1)
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			self->m_strTemp1.Append(" ");
			haoma.Replace(self->m_strTemp1, "");
		}
	}
}


void thCaiPiao::_startKaiJianHistory()
{
	if (self->m_funcKaiJianHistory)
	{
		if (self->m_funcShowHmClear)
		{
			self->m_funcShowHmClear();
		}
		self->m_vHaoma.clear();
		if (!self->m_funcKaiJianHistory(CC_CALLBACK_1(thCaiPiao::_kaiJianHistory, self)))
		{
			theApp.m_pMainWnd->MessageBox("获取开奖号码失败，联系作者！", "提示");
			return;
		}
	}
}

void thCaiPiao::_kaiJianHistory(std::vector<CString>& vHm)
{
	//if (vHm.empty())
	//{
	//	theApp.m_pMainWnd->MessageBox("获取开奖号码失败，联系作者！", "提示");
	//	return;
	//}
	if (self->m_funcShowHm)
	{
		self->m_vHaoma = vHm;
		int i = 0;
		int count = vHm.size();
		for (auto v : self->m_vHaoma)
		{
			auto qihao = thHelper::analyString(v, "|");
			auto haoma = thHelper::analyString(v, "|");
			if (i == count - 1)
			{
				m_qihao = qihao;
			}
			self->m_funcShowHm(qihao, haoma);
			++i;
		}
		//CString strTemp = self->m_vHaoma[self->m_vHaoma.size() - 1];
		//m_qihao = thHelper::analyString(strTemp, "|");
		startKaiJian();
	}
}

void thCaiPiao::_kaiJian(const CString& qihao, const CString& qihaoNext, const CString& hm, long time)
{
	if (qihao.IsEmpty() || time <= 0)
	{
		m_bIsKjing = true;
		m_bOpenKj = true;
		m_lOpenTime = 2000;
		return;
	}
	m_bIsKjing = false;
	m_bOpenKj = true;
	m_qihaoNew = qihaoNext;
	m_haoma = hm;
	m_lOpenTime = time;
	if (qihao == m_qihao)
	{
		return;
	}
	m_qihao = qihao;
	if (self->m_funcShowHm)
	{
		m_strTemp.Format("%s|%s", qihao, hm);
		m_vHaoma.push_back(m_strTemp);
		self->m_funcShowHm(qihao, hm);
	}

}


void thCaiPiao::_shHaoma(CString& haoma, CString& sh)
{
	self->m_strTemp = sh;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		if (haoma.Find(self->m_strTemp1) != -1)
		{
			self->m_strTemp1.Append(" ");
			haoma.Replace(self->m_strTemp1, "");
		}
	}
}

void thCaiPiao::_shHaoma_dm(CString& haoma, const CString& dama, int wz)
{
	self->m_strTemp = haoma;
	while (true)
	{
		self->m_strTemp1 = thHelper::analyString(self->m_strTemp, " ");
		if (self->m_strTemp1.IsEmpty())
		{
			break;
		}
		if (dama.Find(self->m_strTemp1[wz - 1]) != -1)
		{
			self->m_strTemp1.Append(" ");
			haoma.Replace(self->m_strTemp1, "");
		}
	}
}
