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

//��дOnAccept���� - �ص�
void CServerSocket::OnAccept(int nErrorCode)
{
	CMFCChatServerDlg* dlg = (CMFCChatServerDlg*)AfxGetApp()->GetMainWnd();
	//�����ͷ����׽���
	dlg->m_chat = new CChatSocket;
	//��ʼ���ղ�����
	Accept(*(dlg->m_chat));
	/*
	CString str;
	dlg->m_tm = CTime::GetCurrentTime();
	str = dlg->m_tm.Format("%X ");
	str += _T("�ͻ������ӳɹ�!");
	*/
	CString strShow;
	CString strInfo = _T("��ͻ������ӳɹ�!");
	//CString strMsg = _T("");
	strShow = dlg->CatShowString(strInfo, _T(""));

	dlg->m_list.AddString(strShow);
	dlg->m_list.UpdateData(FALSE);

	CAsyncSocket::OnAccept(nErrorCode);
}
