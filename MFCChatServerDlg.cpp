
// MFCChatServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"
#include "afxdialogex.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	TRACE(_T(" "), GetLastError());

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCChatServerDlg 对话框



CMFCChatServerDlg::CMFCChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_server = nullptr;		//主套接字变量
	m_chat = nullptr;		//阻塞连接套接字
	m_buf[0] = 0;			//字符串

	m_WordColorCombo;		//字体颜色变量
	m_tm = 0;				//实时时间变量
	m_list;					//示例框变量
	m_input;				//发送消息编辑框变量
}

void CMFCChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOUR_COMBO, m_WordColorCombo);
	DDX_Control(pDX, IDC_MSG_LIST, m_list);
	DDX_Control(pDX, IDC_SEND_EDIT, m_input);
}

BEGIN_MESSAGE_MAP(CMFCChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BTN, &CMFCChatServerDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_SEND_BTN, &CMFCChatServerDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, &CMFCChatServerDlg::OnBnClickedClearBtn)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STOP_BTN, &CMFCChatServerDlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDC_MAIL_BTN, &CMFCChatServerDlg::OnBnClickedMailBtn)
	ON_BN_CLICKED(IDC_CAL_BTN, &CMFCChatServerDlg::OnBnClickedCalBtn)
	ON_BN_CLICKED(IDC_QQ_BTN, &CMFCChatServerDlg::OnBnClickedQqBtn)
	ON_BN_CLICKED(IDC_BAIDU_BTN, &CMFCChatServerDlg::OnBnClickedBaiduBtn)
END_MESSAGE_MAP()


// CMFCChatServerDlg 消息处理程序

BOOL CMFCChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);		 //启动按钮
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);	 //停止按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);		//消息发送按钮
	GetDlgItem(IDC_CLEAR_BTN)->EnableWindow(FALSE);	//清除历史消息
	GetDlgItem(IDC_MAIL_BTN)->EnableWindow(FALSE);	//邮箱
	GetDlgItem(IDC_CAL_BTN)->EnableWindow(FALSE);	//计算器
	GetDlgItem(IDC_QQ_BTN)->EnableWindow(FALSE);	//QQ
	GetDlgItem(IDC_BAIDU_BTN)->EnableWindow(FALSE);	//百度一下

	//初始化字体颜色
	m_WordColorCombo.AddString(_T("黑色"));
	m_WordColorCombo.AddString(_T("红色"));
	m_WordColorCombo.AddString(_T("蓝色"));
	m_WordColorCombo.AddString(_T("绿色"));
	//设置默认字体颜色
	m_WordColorCombo.SetCurSel(0); //黑色
	SetDlgItemText(IDC_COLOUR_COMBO, _T("黑色"));

	//设置默认端口值
	GetDlgItem(IDC_PORT_EDIT)->SetWindowText(_T("5000"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//绘制背景图片
		CPaintDC dc(this); // 用于绘制的设备上下文
		CRect rect;
		GetClientRect(&rect);
		CDC dcBmp;
		dcBmp.CreateCompatibleDC(&dcBmp);
		CBitmap bmpBackGround;
		bmpBackGround.LoadBitmap(IDB_BMP1_BITMAP);
		BITMAP bBitmap;
		bmpBackGround.GetBitmap(&bBitmap);
		CBitmap* pbmpOld = dcBmp.SelectObject(&bmpBackGround);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, bBitmap.bmWidth, bBitmap.bmHeight, SRCCOPY);
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//消息合并
CString CMFCChatServerDlg::CatShowString(CString strInfo, CString strMsg)
{
	CString strTime;
	CTime tmNow;
	tmNow = CTime::GetCurrentTime();
	strTime = tmNow.Format("%X ");

	CString strShow;
	strShow = strTime + strShow;
	strShow += strInfo;
	strShow += strMsg;

	return strShow;
}

//启动
void CMFCChatServerDlg::OnBnClickedStartBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_START_BTN)->EnableWindow(FALSE);		//启动按钮
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(TRUE);	 //停止按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(TRUE);	//消息发送按钮
	GetDlgItem(IDC_CLEAR_BTN)->EnableWindow(TRUE);	//清除历史消息
	GetDlgItem(IDC_MAIL_BTN)->EnableWindow(TRUE);	//邮箱
	GetDlgItem(IDC_CAL_BTN)->EnableWindow(TRUE);	//计算器
	GetDlgItem(IDC_QQ_BTN)->EnableWindow(TRUE);		//QQ
	GetDlgItem(IDC_BAIDU_BTN)->EnableWindow(TRUE);	//百度一下

	//从控件里面获取端口
	CString strPort;
	GetDlgItem(IDC_PORT_EDIT)->GetWindowText(strPort);

	//CString转char*
	USES_CONVERSION;
	LPCSTR szPort = (LPCSTR)T2A(strPort);

	int iPort = _ttoi(strPort);

	//创建套接字对象
	m_server = new CServerSocket;

	//创建套接字
	if (!m_server->Create(iPort)) return;

	//阻塞,等待连接
	if (!m_server->Listen()) return;

	//更新示例框显示
	/*
	CString str;
	m_tm.Format("%X ");
	str += _T("聊天服务端建立服务!");
	*/
	CString strShow;
	CString str = _T("聊天服务端启动服务!");
	strShow = CatShowString(_T(""), str);
	m_list.AddString(strShow);
	UpdateData(FALSE);
}

//发送消息
void CMFCChatServerDlg::OnBnClickedSendBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取编辑框内容
	CString strTmpMsg;
	GetDlgItem(IDC_SEND_EDIT)->GetWindowTextW(strTmpMsg);

	USES_CONVERSION;
	char* szSendBuf = T2A(strTmpMsg);

	//发送给服务端
	m_chat->Send(szSendBuf, MAX_SERVER_BUF, 0);

	//消息显示到列表框
	/*
	CString strShow = _T("服务端 : ");
	CString strTime;
	m_tm = CTime::GetCurrentTime();
	strTime = m_tm.Format("%X ");

	strShow = strTime + strShow;
	strShow += strTmpMsg;
	*/
	CString strShow;
	CString strInfo = _T("服务端 : ");
	strShow = CatShowString(strInfo, strTmpMsg);

	m_list.AddString(strShow);
	UpdateData(FALSE);

	GetDlgItem(IDC_SEND_EDIT)->SetWindowTextW(_T(" "));
}

//清除历史消息
void CMFCChatServerDlg::OnBnClickedClearBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//清除历史记录数据
	m_list.ResetContent();
}

//设置字体颜色
HBRUSH CMFCChatServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	//获取字体颜色控件变量
	CString strColor;
	m_WordColorCombo.GetWindowTextW(strColor);

	if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SEND_EDIT == pWnd->GetDlgCtrlID())
	{
		if (strColor == _T("黑色"))
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (strColor == _T("红色"))
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if (strColor == _T("蓝色"))
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
		else if (strColor == _T("绿色"))
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}
	/*
	if (strColor == _T("黑色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
	}
	else if (strColor == _T("红色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}
	else if (strColor == _T("蓝色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}
	else if (strColor == _T("绿色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}
	*/
	return hbr;
}

//规避回车键
BOOL CMFCChatServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//按下键盘-回车键
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

	//按下键盘-空格键
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;

	//添加快捷键 如Ctrl + 按键
	//按下键盘且Ctrl且按键
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			if (pMsg->wParam == 'X' || pMsg->wParam == 'x')
			{
				CDialog::OnOK();
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//停止
void CMFCChatServerDlg::OnBnClickedStopBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_START_BTN)->EnableWindow(TRUE);		//启动按钮
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);	 //停止按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);	//消息发送按钮
	GetDlgItem(IDC_CLEAR_BTN)->EnableWindow(FALSE);	//清除历史消息
	GetDlgItem(IDC_MAIL_BTN)->EnableWindow(FALSE);	//邮箱
	GetDlgItem(IDC_CAL_BTN)->EnableWindow(FALSE);	//计算器
	GetDlgItem(IDC_QQ_BTN)->EnableWindow(FALSE);		//QQ
	GetDlgItem(IDC_BAIDU_BTN)->EnableWindow(FALSE);	//百度一下

	//处理回收资源
	m_server->Close();
	if (m_server != NULL)
	{
		delete m_server;
		m_server = NULL;
	}

	if (m_chat != NULL)
	{
		delete m_chat;
		m_chat = NULL;
	}

	//显示到列表框
	CString strShow;
	strShow = CatShowString(_T(""), _T("聊天服务端停止服务!"));

	m_list.AddString(strShow);
	UpdateData(FALSE);
}

//邮箱
void CMFCChatServerDlg::OnBnClickedMailBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	ShellExecute(NULL, _T("open"), _T("https://mail.qq.com"), NULL, NULL, SW_SHOWNORMAL); 
}

//计算器
void CMFCChatServerDlg::OnBnClickedCalBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	ShellExecute(NULL, _T("open"), _T("calc.exe"), NULL, NULL, SW_SHOWNORMAL);
}

//QQ
void CMFCChatServerDlg::OnBnClickedQqBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//各用户QQ路径不同,需更改
	/*
	ShellExecute(NULL, _T("open"), _T("D:\\QQ\\Bin\\QQScLauncher.exe"), NULL, NULL, SW_SHOWNORMAL);
	*/
}

//百度一下
void CMFCChatServerDlg::OnBnClickedBaiduBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	ShellExecute(NULL, _T("open"), _T("https://www.baidu.com"), NULL, NULL, SW_SHOWNORMAL);
}
