// ������������� Microsoft Visual C++ ������ IDispatch ��װ����

// ע��:  ��Ҫ�޸Ĵ��ļ������ݡ�  ���������
//  Microsoft Visual C++ �������ɣ������޸Ľ������ǡ�


#include "pch.h"
#include "explorer_url1.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1

IMPLEMENT_DYNCREATE(CExplorer_url1, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1 ����

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1 ����


BOOL CExplorer_url1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_RBUTTONDOWN == pMsg->message
		|| WM_RBUTTONDBLCLK == pMsg->message
		|| WM_DROPFILES == pMsg->message)
	{
		return TRUE;
	}

	static bool bLBD = false;
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		bLBD = true;
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		bLBD = false;
	}
	else if (bLBD)
	{
		if (pMsg->message == WM_MOUSEMOVE)
		{
			return TRUE;
			//CRect rect;
			//GetWindowRect(&rect);
			//LONG posY = rect.top+rect.Height()-35;
			//if (pMsg->pt.y >= posY)
			//{
			//	return TRUE;
			//}
			//CString strFmt;
			//strFmt.Format("%d-%d, %d\n", pMsg->pt.x, pMsg->pt.y, posY);
			//	OutputDebugStringA(strFmt);
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}
BEGIN_MESSAGE_MAP(CExplorer_url1, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CExplorer_url1::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//SetScrollRange(SB_VERT | SB_HORZ, 0, 0);

	//ShowScrollBar(SB_VERT, FALSE);
	//ShowScrollBar(SB_BOTH, FALSE);
}
