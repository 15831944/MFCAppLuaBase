#pragma once

//Ver 1.0
//用途：绘制平滑文字和一些特效文字

class CTextDrawer
{
public:
	static SIZE GetDrawSize(HDC hDc,LPTSTR lpszText,UINT uFormat,bool& bHaveOffsety,short& yOffset);

	static short CalcOffsety(HDC hDc); //计算 j的高度-b的高度的高度差,用于对齐字符输出
	static bool IsYOffset(UINT uChar);	//绘制的时候，需要向上偏移的字符(jJgy,false)
	static bool IsDoubleByteChar(UINT uChar);

	// 绘制文字
	static void SmoothDrawTextEx(LPTSTR lpszText,CRect rc,COLORREF clr1,COLORREF clr2,
		UINT nFontSize=14,DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);
	// 字体阴影
	static void SmoothShadow(LPTSTR lpszText,CRect rc,COLORREF clrText,UINT nFontSize=14,
		DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);

	static short s_cxSpacing; //字符间隔
	static short s_cwBlank; //空格键的宽度(GetGlyphOutline获得的宽度高度总为1)

	static BYTE* s_pBits;
	static DWORD s_dwEffWidth;
};
