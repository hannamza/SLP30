#pragma once

#pragma pack(push, 1)

class ProtocolHeader
{
public:
	ProtocolHeader() {protocol=0;size=0;};
	struct 
	{
		unsigned protocol : 16;
		unsigned size : 16;
	};

	enum ProtocolCommand
	{
		Keep_Alive = 1,

		RequestLogin,			// login(manager)
		ResponseLogin,

		RequestUserLogin, 
		ResponseUserLogin, 

		// manager protocol ----------------------------------------

		RequestUserAdd, 
		ResponseUserAdd, 

		RequestUserMod, 
		ResponseUserMod, 

		RequestUserDel, 
		ResponseUserDel, 

		RequestUserList,
		ResponseUserList,

		// SLP30 protocol -------------------------------------------

		RequestHistoryAdd, 
		ResponseHistoryAdd, 

		RequestHistoryList, 
		ResponseHistoryList, 

		RequestHistoryUserList, 
		ResponseHistoryUserList, 

		DefineEndProtocol
	};
};

//------------- COMMON ---------------------------------------------------

// common request, result: seq > 0 success, 0: fail
class ProtocolRequest : public ProtocolHeader
{
public:
	ProtocolRequest() 
	{
		memset(this, 0, sizeof(*this));
		protocol = 0;
		size   = sizeof(*this)-sizeof(ProtocolHeader);
	}
	int nValue;
};

// common response, result: seq > 0 success, 0: fail
class ProtocolResponse : public ProtocolHeader
{
public:
	ProtocolResponse() 
	{
		memset(this, 0, sizeof(*this));
		protocol = 0;
		size   = sizeof(*this)-sizeof(ProtocolHeader);
	}
	int nResult;
};

//------------------------------------------------------------------------

//RequestUserLogin
// login(manager) - ResponseLogin(1: success, 0: fail)
class ProtocolRequestLogin : public ProtocolHeader
{
public:
	ProtocolRequestLogin()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestLogin; // RequestUserLogin
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	CHAR szID[32];
	CHAR szPW[32];
};

//------------------------------------------------------------------------

//RequestUserAdd
//RequestUserMod
//RequestUserDel
class ProtocolRequestUserAdd : public ProtocolHeader
{
public:
	ProtocolRequestUserAdd()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestUserAdd;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	int nSeq;
	CHAR szName[32];
	CHAR szPhone[32];
	CHAR szId[32];
	CHAR szPw[32];
	CHAR szEtc[256];
	int nSecUsb;
};

//------------------------------------------------------------------------

//RequestUserList(common) , ResponseUserList
class ProtocolResponseUserList : public ProtocolHeader
{
public:
	ProtocolResponseUserList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseUserList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	int nCount;
	CHAR pValue[0]; // seq」name」phone」id」etc』seq」name」phone」id」etc』seq」name」phone」id」etc』seq」name」phone」id」etc』
};

//------------------------------------------------------------------------

//RequestHistoryAdd, ResponseHistoryAdd(common)
class ProtocolRequestHistoryAdd : public ProtocolHeader
{
public:
	ProtocolRequestHistoryAdd()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestHistoryAdd;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	CHAR szID[32];
	CHAR szLog[256];
};

//RequestHistoryList,
//ResponseHistoryList,

//RequestHistoryUserList,
//ResponseHistoryUserList,

#pragma pack(pop, 1)