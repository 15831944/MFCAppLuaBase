// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装器类

// 注意:  不要修改此文件的内容。  如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。


#include "pch.h"
#include "explorer_url1.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1

IMPLEMENT_DYNCREATE(CExplorer_url1, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1 属性

/////////////////////////////////////////////////////////////////////////////
// CExplorer_url1 操作


BOOL CExplorer_url1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	// TODO: 在此添加专用代码和/或调用基类
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

	// TODO: 在此处添加消息处理程序代码
	//SetScrollRange(SB_VERT | SB_HORZ, 0, 0);

	//ShowScrollBar(SB_VERT, FALSE);
	//ShowScrollBar(SB_BOTH, FALSE);
}
