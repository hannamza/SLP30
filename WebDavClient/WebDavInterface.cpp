#include "stdafx.h"
#include "WebDavInterface.h"
#include "WebDav.h"

#pragma comment(lib, "LibNeon.lib")


CWebDavInterface::CWebDavInterface()
{
	m_nPort = 80;
}

CWebDavInterface::~CWebDavInterface()
{
}

bool CWebDavInterface::LoginInfo(std::string host, std::string user, std::string pass, std::string uri, int port)
{
	m_sHost = host;
	m_sUser = user;
	m_sPass = pass;
	m_nPort = port;
	m_sUri = uri;

	return true;
}

bool CWebDavInterface::IsExistDir(std::string dir)
{
	WebDav WebDav(m_sHost, m_nPort, m_sUser, m_sPass);
	if (WebDav.exist(dir)) {
		return true;
	}
	return false;
}

bool CWebDavInterface::MakeDirectory(std::string dir)
{
	WebDav WebDav(m_sHost, m_nPort, m_sUser, m_sPass);
	if (WebDav.exist(m_sUri + dir)) {
		return true;
	}
	if (WebDav.mkdir(m_sUri + dir)) {
		return true;
	}
	std::string str = WebDav.getLastError();
	return false;
}

bool CWebDavInterface::UploadFile(std::string uploadName, std::string filePath)
{
	WebDav WebDav(m_sHost, m_nPort, m_sUser, m_sPass);
	if (WebDav.put(m_sUri + uploadName, filePath)) {
		return true;
	}
	return false;
}

bool CWebDavInterface::DownloadFile(std::string downloadfile, std::string filePath)
{
	WebDav WebDav(m_sHost, m_nPort, m_sUser, m_sPass);
	if (WebDav.get(m_sUri + downloadfile, filePath)) {
		return true;
	}
	return false;
}

bool CWebDavInterface::DeleteDirectory(std::string dir)
{
	WebDav WebDav(m_sHost, m_nPort, m_sUser, m_sPass);
	if (!WebDav.exist(m_sUri + dir)) {
		return true;
	}
	if (WebDav.rm(m_sUri + dir)) {
		return true;
	}

	return false;
}
