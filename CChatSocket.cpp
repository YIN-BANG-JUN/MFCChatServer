#include "pch.h"
#include "CChatSocket.h"

#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"

CChatSocket::CChatSocket()
{
}

CChatSocket::~CChatSocket()
{
}

void CChatSocket::OnReceive(int nErrorCode)
{
	CMFCChatServerDlg* dlg = (CMFCChatServerDlg*)AfxGetApp()->GetMainWnd();

	//�������ݵ��ַ�����
	char szRecvBuf[MAX_SERVER_BUF] = { 0 };
	Receive(szRecvBuf, MAX_SERVER_BUF, 0);

	//��ʾ���յ����ַ�
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuf);
	/*
	CString strShow = _T("�ͻ��� : ");
	CString strTime;
	dlg->m_tm = CTime::GetCurrentTime();
	strTime = dlg->m_tm.Format("%X ");

	strShow = strTime + strShow;
	strShow += strRecvMsg;
	*/
	CString strShow;
	//CString strInfo = _T("");
	//CString strMsg = _T("");
	strShow = dlg->CatShowString(_T(""), strRecvMsg);

	dlg->m_list.AddString(strShow);

	CAsyncSocket::OnReceive(nErrorCode);
}