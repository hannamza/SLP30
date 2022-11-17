#include "stdafx.h"
#include "Server.h"
#include "Client.h"
#include "Packet.h"
#include "IOEvent.h"

#include "Log.h"
#include "Network.h"
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CALLBACK Server::WorkerProcessRecvPacket(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context)
{
	Packet* packet = static_cast<Packet*>(Context);
	assert(packet);

	BYTE* buffer = packet->GetData(), *pData = NULL;

	USHORT command = 6;
	UINT size = 0;
	memcpy_s(&command, sizeof(USHORT), buffer, sizeof(USHORT));
	memcpy_s(&size, sizeof(UINT), &buffer[sizeof(USHORT)], sizeof(UINT));
	if (size > 0)
	{
		pData = buffer;
	}
	if (command < ProtocolHeader::Keep_Alive || command >= ProtocolHeader::DefineEndProtocol)
	{
		return;
	}

	switch (command)
	{
	case ProtocolHeader::Keep_Alive:
		Server::Instance()->Echo(packet);
		//trace0("[%d] KEEP ALIVE Packet.", GetCurrentThreadId());
		break;
	case ProtocolHeader::RequestLogin:				// login(manager)
		ProcessProtocolRequestLogin(pData, packet);
		break;
	case ProtocolHeader::RequestUserLogin:
		ProcessProtocolRequestUserLogin(pData, packet);
		break;
	case ProtocolHeader::RequestUserAdd:
		ProcessProtocolRequestUserAdd(pData, packet);
		break;
	case ProtocolHeader::RequestUserMod:
		ProcessProtocolRequestUserMod(pData, packet);
		break;
	case ProtocolHeader::RequestUserDel:
		ProcessProtocolRequestUserDel(pData, packet);
		break;
	case ProtocolHeader::RequestUserList:
		ProcessProtocolRequestUserList(pData, packet);
		break;
	case ProtocolHeader::RequestHistoryAdd:
		ProcessProtocolRequestHistoryAdd(pData, packet);
		break;
	case ProtocolHeader::RequestHistoryList:
		break;
	case ProtocolHeader::RequestHistoryUserList:
		break;
	default:
		Server::Instance()->Echo(packet);
		break;
	}
}

void Server::ProcessProtocolRequestLogin(BYTE* pData, Packet* packet)	// login(manager)
{
	CEncDec::Instance()->PacketDecode(pData, sizeof(ProtocolHeader), sizeof(ProtocolRequestLogin) - sizeof(ProtocolHeader));
	ProtocolRequestLogin* pReq = (ProtocolRequestLogin*)pData;
	ProtocolResponse res;

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];
	MYSQL_ROW  ContentRow;

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;

	sprintf_s(szQuery, 2048, "select seq from admin_user where id='%s' and pw=PASSWORD('%s')", pReq->szID, pReq->szPW);
	pRes = pManager->MysqlSelectQuery(szQuery);

	// 존재 하면 성공
	if (pRes && (UINT)mysql_num_rows(pRes) == 1)
	{
		TRACE("success\n");
		while (ContentRow = mysql_fetch_row(pRes))
		{
			res.nResult = CCommonFunc::GetAtoUI((char*)ContentRow[0]);
		}
	}
	else
	{
		res.nResult = 0;
	}
	pManager->ReleaseSelectQuery(pRes);

	CDBPool::Instance()->ReturnDBManager(pManager);

	res.protocol = ProtocolHeader::ResponseLogin;

	trace0("ProcessProtocolRequestLogin- user: %s, result: %d", pReq->szID, res.nResult);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));

}

void Server::ProcessProtocolRequestUserLogin(BYTE* pData, Packet* packet)
{
	CEncDec::Instance()->PacketDecode(pData, sizeof(ProtocolHeader), sizeof(ProtocolRequestLogin) - sizeof(ProtocolHeader));
	ProtocolRequestLogin* pReq = (ProtocolRequestLogin*)pData;
	ProtocolResponse res;

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];
	MYSQL_ROW  ContentRow;

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;

	sprintf_s(szQuery, 2048, "select seq from user_account where id='%s' and pw=PASSWORD('%s')", pReq->szID, pReq->szPW);
	pRes = pManager->MysqlSelectQuery(szQuery);

	// 존재 하면 성공
	if (pRes && (UINT)mysql_num_rows(pRes) == 1)
	{
		TRACE("success\n");
		while (ContentRow = mysql_fetch_row(pRes))
		{
			res.nResult = CCommonFunc::GetAtoUI((char*)ContentRow[0]);
		}
	}
	else
	{
		res.nResult = 0;
	}
	pManager->ReleaseSelectQuery(pRes);

	CDBPool::Instance()->ReturnDBManager(pManager);

	res.protocol = ProtocolHeader::ResponseUserLogin;

	trace0("ProcessProtocolRequestUserLogin- user: %s, result: %d", pReq->szID, res.nResult);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
}

void Server::ProcessProtocolRequestUserAdd(BYTE* pData, Packet* packet)
{
	ProtocolRequestUserAdd* pReq = (ProtocolRequestUserAdd*)pData;
	ProtocolResponse res;
	res.protocol = ProtocolHeader::ResponseUserAdd;
	res.size = sizeof(ProtocolResponse);

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;
	sprintf_s(szQuery, 2048, "SELECT seq FROM user_account WHERE id='%s' and pw=PASSWORD('%s')", pReq->szId, pReq->szPw);
	pRes = pManager->MysqlSelectQuery(szQuery);
	if (pRes)
	{
		if (pRes && mysql_num_rows(pRes) > 0)
		{
			pManager->ReleaseSelectQuery(pRes);

			res.nResult = -1; // exist
			Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
			trace0("ProcessProtocolRequestUserAdd - exist no: %s", pReq->szId);

			CDBPool::Instance()->ReturnDBManager(pManager);
			return;
		}
		pManager->ReleaseSelectQuery(pRes);
	}

	sprintf_s(szQuery, 2048, "INSERT INTO user_account(name, phone, id, pw, etc, secusb) VALUES('%s', '%s', '%s', PASSWORD('%s'), '%s', '%d')",
		pReq->szName, pReq->szPhone, pReq->szId, pReq->szPw, pReq->szEtc, pReq->nSecUsb);
	if (pManager->MysqlExcuteQuery(szQuery))
	{
		res.nResult = 0; // success
	}
	else
	{
		res.nResult = -2; // fail
	}
	CDBPool::Instance()->ReturnDBManager(pManager);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
	trace0("ProcessProtocolRequestUserAdd - name: %s, id: %s", pReq->szName, pReq->szId);
}

void Server::ProcessProtocolRequestUserMod(BYTE* pData, Packet* packet)
{
	ProtocolRequestUserAdd* pReq = (ProtocolRequestUserAdd*)pData;
	ProtocolResponse res;

	res.protocol = ProtocolHeader::ResponseUserMod;
	res.size = sizeof(ProtocolResponse);

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;
	sprintf_s(szQuery, 2048, "SELECT seq FROM user_account WHERE id='%s' and pw=PASSWORD('%s')", pReq->szId, pReq->szPw);
	pRes = pManager->MysqlSelectQuery(szQuery);
	if (pRes)
	{
		if (pRes && mysql_num_rows(pRes) > 0)
		{
			pManager->ReleaseSelectQuery(pRes);

			res.nResult = -1; // exist
			Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
			trace0("ProcessProtocolRequestModManager - exist manager id: %s", pReq->szId);

			CDBPool::Instance()->ReturnDBManager(pManager);
			return;
		}
		pManager->ReleaseSelectQuery(pRes);
	}

	if (strlen(pReq->szPw) > 0) {
		sprintf_s(szQuery, 2048, "UPDATE user_account SET pw=PASSWORD('%s'), phone='%s', name='%s', etc='%s', secusb='%d' WHERE seq=%d"
			, pReq->szPw, pReq->szPhone, pReq->szName, pReq->szEtc, pReq->nSecUsb, pReq->nSeq);
	}
	else {
		sprintf_s(szQuery, 2048, "UPDATE user_account SET name='%s', phone='%s', etc='%s', secusb=%d WHERE seq=%d"
			, pReq->szName, pReq->szPhone, pReq->szEtc, pReq->nSecUsb, pReq->nSeq);
	}

	if (pManager->MysqlExcuteQuery(szQuery))
	{
		res.nResult = 1; // success
	}
	else
	{
		res.nResult = 0; // fail
	}
	CDBPool::Instance()->ReturnDBManager(pManager);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
	trace0("ProcessProtocolRequestUserMod - name: %s, id: %s, etc: %s, seq: %d", pReq->szName, pReq->szId, pReq->szEtc, pReq->nSeq);
}

void Server::ProcessProtocolRequestUserDel(BYTE* pData, Packet* packet)
{
	ProtocolRequestUserAdd* pReq = (ProtocolRequestUserAdd*)pData;
	ProtocolResponse res;

	res.protocol = ProtocolHeader::ResponseUserDel;
	res.size = sizeof(ProtocolResponse);

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;
	sprintf_s(szQuery, 2048, "DELETE FROM user_account where seq=%d", pReq->nSeq);
	if (pManager->MysqlExcuteQuery(szQuery))
	{
		res.nResult = 1; // success
	}
	else
	{
		res.nResult = 0; // fail
	}
	CDBPool::Instance()->ReturnDBManager(pManager);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
	trace0("ProcessProtocolRequestUserDel - seq: %d", pReq->nSeq);
}

void Server::ProcessProtocolRequestUserList(BYTE* pData, Packet* packet)
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	//ProtocolResponseUserList* pRes = NULL;
	BYTE* pResult = NULL;

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];
	MYSQL_ROW  ContentRow;
	CStringA sValue;

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;
	sprintf_s(szQuery, 2048, "SELECT seq, name, phone, id, etc FROM user_account");
	pRes = pManager->MysqlSelectQuery(szQuery);
	int nCount = 0;
	int nSize = 0;
	if (pRes)
	{
		nCount = mysql_num_rows(pRes);
		while (ContentRow = mysql_fetch_row(pRes)) {
			sValue += (char*)ContentRow[0];
			sValue += "§";
			sValue += CCommonFunc::Utf8ToChar((char*)ContentRow[1]);
			sValue += "§";
			sValue += (char*)ContentRow[2];
			sValue += "§";
			sValue += (char*)ContentRow[3];
			sValue += "§";
			sValue += CCommonFunc::Utf8ToChar((char*)ContentRow[4]);
			sValue += "☆";
		}

		nSize = sizeof(ProtocolResponseUserList) + sValue.GetLength() + 1;
		pResult = new BYTE[nSize];
		memset(pResult, 0x00, nSize);
		((ProtocolResponseUserList*)pResult)->size = nSize;
		((ProtocolResponseUserList*)pResult)->protocol = ProtocolHeader::ResponseUserList;
		((ProtocolResponseUserList*)pResult)->nCount = nCount;
		strcpy(((ProtocolResponseUserList*)pResult)->pValue, sValue.GetBuffer(0));

		pManager->ReleaseSelectQuery(pRes);
	}
	else
	{
		nSize = sizeof(ProtocolResponseUserList);
		pResult = new BYTE[nSize];
		memset(pResult, 0x00, nSize);
		((ProtocolResponseUserList*)pResult)->size = nSize;
		((ProtocolResponseUserList*)pResult)->protocol = ProtocolHeader::ResponseUserList;
		((ProtocolResponseUserList*)pResult)->nCount = 0;
	}
	CDBPool::Instance()->ReturnDBManager(pManager);

	trace0("ProcessProtocolRequestGetUserList");

	Server::Instance()->Send(packet, (BYTE*)pResult, nSize);

	SAFE_DELETE(pResult);
}

void Server::ProcessProtocolRequestHistoryAdd(BYTE* pData, Packet* packet)
{
	ProtocolRequestHistoryAdd* pReq = (ProtocolRequestHistoryAdd*)pData;
	ProtocolResponse res;

	res.protocol = ProtocolHeader::ResponseUserAdd;
	res.size = sizeof(ProtocolResponse);

	MYSQL_RES* pRes = NULL;
	CHAR szQuery[2048];

	CDBConnectionManager* pManager = CDBPool::Instance()->GetDbManager(); if (!pManager) return;

	sprintf_s(szQuery, 2048, "INSERT INTO history(id, log) VALUES('%s', '%s')", pReq->szID, pReq->szLog);
	if (pManager->MysqlExcuteQuery(szQuery))
	{
		res.nResult = 0; // success
	}
	else
	{
		res.nResult = -1; // fail
	}
	CDBPool::Instance()->ReturnDBManager(pManager);

	Server::Instance()->Send(packet, (BYTE*)&res, sizeof(ProtocolResponse));
	trace0("ProcessProtocolRequestHistoryAdd - id: %s, log: %s", pReq->szID, pReq->szLog);
}
