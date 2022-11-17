#pragma once

#include "TSingleton.h"
class CWebDavInterface : public TSingleton<CWebDavInterface>
{
public:
	CWebDavInterface();
	~CWebDavInterface();

public:
	bool LoginInfo(std::string host, std::string user, std::string pass, std::string uri, int port);
	bool IsExistDir(std::string dir);
	bool MakeDirectory(std::string dir);
	bool UploadFile(std::string uploadName, std::string filePath);
	bool DownloadFile(std::string downloadfile, std::string filePath);
	bool DeleteDirectory(std::string dir);

private:
	std::string m_sHost;
	std::string m_sUser;
	std::string m_sPass;
	std::string m_sUri;
	int m_nPort;
};

