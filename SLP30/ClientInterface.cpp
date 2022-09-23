#include "stdafx.h"
#include "ClientInterface.h"

CClientInterface::CClientInterface()
{
	m_bConnecting = false;
	m_dwLastTime = 0;
	m_dwLastConnected = 0;
	InitializeCriticalSection(&mConnectionCheck);

	CEncDec::New();
}

CClientInterface::~CClientInterface()
{
	CEncDec::Delete();
}

void CClientInterface::Destroy()
{
	DeleteCriticalSection(&mConnectionCheck);
}

bool CClientInterface::TryConnection(const char* ip, u_short port)
{
	EnterCriticalSection(&mConnectionCheck);

	strcpy_s(m_ip, 32, ip);
	m_port = port;

	if (!m_bConnecting)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::ie()->AddClients(1);
		ClientMan::ie()->ConnectClients(ip, port);
	}

	LeaveCriticalSection(&mConnectionCheck);
	return true;
}

bool CClientInterface::TryShutdown()
{
	EnterCriticalSection(&mConnectionCheck);

	if (m_bConnecting)
	{
		m_bConnecting = false;

		ClientMan::ie()->RemoveClients();
		ClientMan::Delete();
		Network::Deinitialize();

		m_dwLastConnected = GetTickCount();
	}
	LeaveCriticalSection(&mConnectionCheck);

	return true;
}

bool CClientInterface::IsConnected()
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::ie()->GetClientState() == Client::CONNECTED)
	{
		LeaveCriticalSection(&mConnectionCheck);
		return true;
	}
	LeaveCriticalSection(&mConnectionCheck);
	return false;
}

bool CClientInterface::Send(const char* msg)
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::ie()->GetNumClients() > 0)
	{
		ClientMan::ie()->Send(msg);
	}
	else if(GetTickCount() - m_dwLastConnected >= 3000)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::ie()->AddClients(1);
		ClientMan::ie()->ConnectClients(m_ip, m_port);

		LeaveCriticalSection(&mConnectionCheck);
		return false;
	}

	LeaveCriticalSection(&mConnectionCheck);

	return true;
}

bool CClientInterface::Send(BYTE* buffer, int nSize)
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::ie()->GetNumClients() > 0)
	{
		ClientMan::ie()->Send(buffer, nSize);
	}
	else if (GetTickCount() - m_dwLastConnected >= 3000)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::ie()->AddClients(1);
		ClientMan::ie()->ConnectClients(m_ip, m_port);

		LeaveCriticalSection(&mConnectionCheck);
		return false;
	}

	LeaveCriticalSection(&mConnectionCheck);
	return true;
}

// 시스템, 매니저, 사용자 목록 요청
void CClientInterface::ProcessCommonRequest(USHORT protocol, int nValue)
{
	ProtocolRequest req;
	req.protocol = protocol;
	req.nValue = nValue;

	//Log::Trace("ProcessCommonRequest: protocol: %d / value: %d", protocol, nValue);

	Send((BYTE*)&req, sizeof(ProtocolRequest));
}

void CClientInterface::ProcessRequestLogin(WCHAR* szID, WCHAR* szPW)
{
	ProtocolRequestLogin login;
	login.protocol = ProtocolHeader::RequestUserLogin;
	strcpy_s(login.szID, CCommonFunc::WcharToUtf8(szID));
	strcpy_s(login.szPW, CCommonFunc::WcharToUtf8(szPW));

	CEncDec::ie()->PacketEncode((BYTE*)&login, sizeof(ProtocolHeader), sizeof(ProtocolRequestLogin) - sizeof(ProtocolHeader));
	Send((BYTE*)&login, sizeof(ProtocolRequestLogin));
}

void CClientInterface::ProcessRequestHistoryAdd(WCHAR* szID, WCHAR* szHistory)
{
	ProtocolRequestHistoryAdd req;
	strcpy_s(req.szID, CCommonFunc::WcharToUtf8(szID));
	strcpy_s(req.szLog, CCommonFunc::WcharToUtf8(szHistory));

	Send((BYTE*)&req, sizeof(ProtocolRequestHistoryAdd));
}
