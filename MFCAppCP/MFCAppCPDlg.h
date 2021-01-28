
// MFCAppCPDlg.h: 头文件
//

#pragma once
#include "../THEngine/web/explorer_url1.h"


// CMFCAppCPDlg 对话框
class CMFCAppCPDlg : public CDialogEx
{
// 构造
public:
	CMFCAppCPDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPCP_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCancel();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	// 注册到lua
	void registerToLua();
	// 注册热键
	void registerHkey();
	// 初始化数据
	void initData();

	// 开始运行
	bool startRun();

protected:
	// 网页广告
	CExplorer_url1 m_explorerWeb;
	// kj记录
	CListCtrl m_listKj;
};
