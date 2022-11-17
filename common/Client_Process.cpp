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
	case ProtocolHeader::ResponseLogin:		// 응답: 관리자 로그인
		ProcessResponseManagerLogin(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAdminLogin\n");
		break;
	case ProtocolHeader::ResponseUserList:		// 응답: 사용자 목록 요청
		ProcessResponseUserList(pData, pPacket);
		TRACE("ProtocolHeader::ResponseUserList\n");
		break;
	case ProtocolHeader::ResponseUserAdd:			// 응답: 사용자 추가
		ProcessResponseAddUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAddUser\n");
		break;
	case ProtocolHeader::ResponseUserMod:			// 응답: 사용자 수정
		ProcessResponseModUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseModUser\n");
		break;
	case ProtocolHeader::ResponseUserDel:			// 응답: 사용자 삭제
		ProcessResponseDelUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseDelUser\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseManagerLogin(BYTE* pData, BYTE* pPacket)		// 응답: 관리자 로그인
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult > 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestLogin, true);
		CCommonState::Instance()->m_nIdx = pRes->nResult;
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::RequestLogin + 100, 0);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestLogin, false);
	}
	CCommonState::Instance()->m_bLoginResult = true;
}

void Client::ProcessResponseUserList(BYTE* pData, BYTE* pPacket)		// 응답: 사용자 목록 요청
{
	ProtocolResponseUserList* pList = (ProtocolResponseUserList*)pData;
	int nCount = pList->nCount;
	if (nCount == 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserList, true);
		CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 0, 0);
		return;
	}

	CString sValue = CCommonFunc::CharToWCHAR(pList->pValue);
	CString sTemp;

	CUserManager::Instance()->ReleaseList();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		AfxExtractSubString(sTemp, sValue, nIndex, L'☆');
		CUserManager::Instance()->InsertUser(sTemp.GetBuffer(0));
	}
	CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserList, true);

	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 0, 0);
}

void Client::ProcessResponseAddUser(BYTE* pData, BYTE* pPacket)	 		// 응답: 사용자 추가
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserAdd, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserAdd, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseUserAdd + 100, pRes->nResult);
}

void Client::ProcessResponseModUser(BYTE* pData, BYTE* pPacket)	 		// 응답: 사용자 수정
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserMod, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserMod, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseUserMod + 100, pRes->nResult);
}

void Client::ProcessResponseDelUser(BYTE* pData, BYTE* pPacket) 		// 응답: 사용자 삭제
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserDel, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseUserDel, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseUserDel + 100, pRes->nResult);
}
