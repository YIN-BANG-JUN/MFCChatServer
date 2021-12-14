
// MFCChatServerDlg.h: 头文件
//

#pragma once

#include "CServerSocket.h"
#include "CChatSocket.h"

#define MAX_SERVER_BUF 1024

// CMFCChatServerDlg 对话框
class CMFCChatServerDlg : public CDialogEx
{
// 构造
public:
	CMFCChatServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCHATSERVER_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	CServerSocket* m_server;		//主套接字变量
	CChatSocket* m_chat;			//阻塞连接套接字
	char m_buf[MAX_SERVER_BUF];		//字符串

	CComboBox m_WordColorCombo;		//字体颜色变量
	CTime m_tm;						//实时时间变量
	CListBox m_list;				//示例框变量
	CEdit m_input;					//发送消息编辑框变量

	afx_msg CString CatShowString(CString strInfo, CString strMsg);		//消息合并
	afx_msg void OnBnClickedStartBtn();		//启动
	afx_msg void OnBnClickedSendBtn();		//发送消息
	afx_msg void OnBnClickedClearBtn();		//清除历史消息
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//设置字体颜色
	virtual BOOL PreTranslateMessage(MSG* pMsg);	//规避回车键
	afx_msg void OnBnClickedStopBtn();		//停止
	afx_msg void OnBnClickedMailBtn();		//邮箱
	afx_msg void OnBnClickedCalBtn();		//计算器
	afx_msg void OnBnClickedQqBtn();		//QQ
	afx_msg void OnBnClickedBaiduBtn();		//百度一下
};
