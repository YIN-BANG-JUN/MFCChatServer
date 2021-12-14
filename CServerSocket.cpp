#include "pch.h"
#include "CServerSocket.h"

#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}

//重写OnAccept函数 - 回调
void CServerSocket::OnAccept(int nErrorCode)
{
	CMFCChatServerDlg* dlg = (CMFCChatServerDlg*)AfxGetApp()->GetMainWnd();
	//创建客服端套接字
	dlg->m_chat = new CChatSocket;
	//开始接收并连接
	Accept(*(dlg->m_chat));
	/*
	CString str;
	dlg->m_tm = CTime::GetCurrentTime();
	str = dlg->m_tm.Format("%X ");
	str += _T("客户端连接成功!");
	*/
	CString strShow;
	CString strInfo = _T("与客户端连接成功!");
	//CString strMsg = _T("");
	strShow = dlg->CatShowString(strInfo, _T(""));

	dlg->m_list.AddString(strShow);
	dlg->m_list.UpdateData(FALSE);

	CAsyncSocket::OnAccept(nErrorCode);
}
