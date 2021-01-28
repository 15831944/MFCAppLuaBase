#pragma once

//Ver 1.0
//��;������ƽ�����ֺ�һЩ��Ч����

class CTextDrawer
{
public:
	static SIZE GetDrawSize(HDC hDc,LPTSTR lpszText,UINT uFormat,bool& bHaveOffsety,short& yOffset);

	static short CalcOffsety(HDC hDc); //���� j�ĸ߶�-b�ĸ߶ȵĸ߶Ȳ�,���ڶ����ַ����
	static bool IsYOffset(UINT uChar);	//���Ƶ�ʱ����Ҫ����ƫ�Ƶ��ַ�(jJgy,false)
	static bool IsDoubleByteChar(UINT uChar);

	// ��������
	static void SmoothDrawTextEx(LPTSTR lpszText,CRect rc,COLORREF clr1,COLORREF clr2,
		UINT nFontSize=14,DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);
	// ������Ӱ
	static void SmoothShadow(LPTSTR lpszText,CRect rc,COLORREF clrText,UINT nFontSize=14,
		DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);

	static short s_cxSpacing; //�ַ����
	static short s_cwBlank; //�ո���Ŀ��(GetGlyphOutline��õĿ�ȸ߶���Ϊ1)

	static BYTE* s_pBits;
	static DWORD s_dwEffWidth;
};
