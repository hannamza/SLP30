#include "stdafx.h"
#include "Client.h"
#include "ClientMan.h"

#include "Log.h"
#include "Network.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/pool/singleton_pool.hpp>

using namespace std;

int g_nValue = 0;
void Client::WorkerProcessRecvPacket(BYTE* pPacket, int nSize)
{
	BYTE* buffer = m_recvBuffer, *pData = NULL;

	ProtocolHeader header;
	memcpy_s(&header, sizeof(ProtocolHeader), buffer, sizeof(ProtocolHeader));
	if (header.size > nSize) {
		g_nValue += header.size;
		TRACE("%d / %d\n", header.size, nSize);
		return;
	}
	if (header.size > 0) {
		pData = buffer;
	}
	if (header.protocol < ProtocolHeader::Keep_Alive || header.protocol >= ProtocolHeader::DefineEndProtocol)
	{
		return;
	}
	switch (header.protocol)
	{
	case ProtocolHeader::ResponseUserLogin:		// 응답: 사용자 로그인
		ProcessResponseUserLogin(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAdminLogin\n");
		break;
	case ProtocolHeader::ResponseHistoryAdd:
		ProcessResponseHistory(pData, pPacket);
		TRACE("ProtocolHeader::ResponseHistoryAdd\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseUserLogin(BYTE* pData, BYTE* pPacket)		// 응답: 사용자 로그인
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult > 0) {
		CCommonState::ie()->SetResult(ProtocolHeader::RequestUserLogin, true);
		CCommonState::ie()->m_nIdx = pRes->nResult;
		CCommonState::ie()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::RequestUserLogin + 100, 0);
	}
	else {
		CCommonState::ie()->SetResult(ProtocolHeader::RequestUserLogin, false);
	}
	CCommonState::ie()->m_bLoginResult = true;
}

void Client::ProcessResponseHistory(BYTE* pData, BYTE* pPacket)
{

}
