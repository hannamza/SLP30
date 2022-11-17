#include "StdAfx.h"
#include <string>
//#include <unistd.h> /* close */
#include <fcntl.h> /* open */
#include <cstring> /* strncpy */

#include <ne_props.h> /* ne_simple_propfind, ne_prop_result_set */
#include <ne_uri.h> /* uri */
#include <ne_auth.h> /* ne_set_server_auth, ne_ssl_trust_default_ca */
#include <ne_basic.h> /* ne_session, ne_put, ne_get, ne_delete, ne_mkcol */

#include "Webdav.h"

#define		SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }

#pragma warning(disable:4996)

static const ne_propname fetchProps[] = {
	{ "DAV:", "resourcetype" }, 
	{ "DAV:", "getlastmodified" },
	{ "DAV:", "getcontenttype"},
	{ NULL }
};

/**
* @brief Inits webdav session and ssl authentication
*
**/
std::vector<std::string> *g_login = NULL;
WebDav::WebDav(const std::string url, const unsigned port, const std::string user, const std::string pw)
{
	ne_sock_init();
	mSession = ne_session_create("http", url.c_str(), port);
	g_login = new std::vector<std::string>();
	g_login->push_back(user);
	g_login->push_back(pw);
	ne_set_server_auth(mSession, WebDav::setLogin, g_login);

}

WebDav::~WebDav()
{
	ne_forget_auth(mSession);
	ne_session_destroy(mSession);
	ne_sock_exit();
	SAFE_DELETE(g_login);
}

/**
* @brief Callback for ne_set_server_auth to set passwort and username of ssl connection
*
**/
int WebDav::setLogin(void *userdata, const char *realm, int attempts, char *username, char *password)
{
	std::vector<std::string> *login = (std::vector<std::string>*) userdata;
	strncpy(username, login->at(0).c_str(), NE_ABUFSIZ);
	strncpy(password, login->at(1).c_str(), NE_ABUFSIZ);
	return attempts;
}

/**
* @brief Callback of ne_simple_propfind, that fills a vector(userdata) with properties of files/directory
*
**/
void WebDav::getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set)
{
	std::vector<WebdavPath> *paths = (std::vector<WebdavPath> *) userdata;

	ne_propname props[] = {
		{ "DAV:", "resourcetype" }, 
		{ "DAV:", "getlastmodified" },
		{ "DAV:", "getcontenttype"}
	};

	std::string ressourceType = ne_propset_value(set, &props[0]) ? ne_propset_value(set, &props[0]) : std::string();
	std::string lastModified  =  ne_propset_value(set, &props[1]) ?  ne_propset_value(set, &props[1]) : std::string();
	std::string contentType   = ne_propset_value(set, &props[2]) ?  ne_propset_value(set, &props[2]) : std::string();

	WebdavPath path(uri->host,
		uri->path,
		ressourceType,
		lastModified,
		contentType);

	// Push information to userdata
	paths->push_back(path);
}

/**
* @brief Checks for existance of file/diretory at
*        given uri. This can be used to reduce transfer 
*        bandwidth, so no data need to be transfered 
*        which allready exists.
*
* @param uri of file/directory on webdav-server
*
* @return true if file/directory exists
*         false if file/directory not exists
*
**/
bool WebDav::exist(std::string uri)
{
	const int depth = NE_DEPTH_ZERO; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
	std::vector<WebdavPath> * props = new std::vector<WebdavPath>;
	int res = ne_simple_propfind(mSession, uri.c_str(), depth, fetchProps, getProps, props);
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		if(mError.find("404")!= std::string::npos)
		{
			SAFE_DELETE(props);
			return false;
		}
		SAFE_DELETE(props);
		return false;
	}
	props->erase(props->begin());

	SAFE_DELETE(props);
	return true;
}

/**
* @brief Lists the directory specified with uri. 
*        Behauvior is like ls know from unix console.
* @param uri of directory on webdav-server
* 
* @return A vector of pathinfomation(Webdavpath) of
*         the listed directory.
*
**/
std::vector<WebdavPath> WebDav::ls(std::string uri)
{
	const int depth = NE_DEPTH_ONE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
	std::vector<WebdavPath> * props = new std::vector<WebdavPath>;
	int res = ne_simple_propfind(mSession, uri.c_str(), depth, fetchProps, WebDav::getProps, props);
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		SAFE_DELETE(props);
		return *props;
	}
	props->erase(props->begin());
	SAFE_DELETE(props);
	return *props;
}

/**
* @brief Pulls the whole subtree of webdav-uri
*
**/
std::vector<WebdavPath> WebDav::tree(std::string uri)
{
	std::vector<WebdavPath> *props = new std::vector<WebdavPath>;
	const int depth = NE_DEPTH_INFINITE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
	int res = ne_simple_propfind(mSession, uri.c_str(), depth, fetchProps, WebDav::getProps, props);
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		SAFE_DELETE(props);
		return *props;
	}
	SAFE_DELETE(props);
	return *props;
}

/**
* @brief Puts a file (localSource) on a directory(uri)of the
*       webdav-server.
*
* @param uri         destination on the webdav-server
*        localSource sourcefile, that will be copied
*
* @return false if can´t put file
*         true  otherwise
**/
bool WebDav::put(std::string uri, std::string localSource)
{
	int res = ne_put(mSession, uri.c_str(), (char*)localSource.c_str());
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		return false;
	}

	return true;
}

/**
* @brief Downloads a file from webdav-server(uri).
*
* @param uri              source of file to download
*        localDestination download-destination
*
* @return false if can´t download file
*         true  otherwise
**/
bool WebDav::get(std::string uri, std::string localDestination)
{
	int res = ne_get(mSession, uri.c_str(), (char*)localDestination.c_str());
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		return false;
	}
	return true;
}

/**
* @brief Creates a directory on webdav-server at uri
*
* @param uri directory to create
*
* @return false if can´t create directory
*         true  otherwise
**/
bool WebDav::mkdir(std::string uri)
{
	int res = ne_mkcol(mSession, uri.c_str());
	if(res != NE_OK)
	{
		mError = ne_get_error(mSession);
		return false;
	}
	return true;
}

/**
* @brief Delete file or directory on webdav-server at uri
*
* @param uri directory to delete
*
* @return false if can´t delete directory
*         true  otherwise
**/
bool WebDav::rm(std::string uri)
{
	int res = ne_delete(mSession, uri.c_str());
	if(res!=NE_OK)
	{
		mError = ne_get_error(mSession);
		return false;
	}
	return true;
}

std::string WebDav::getLastError()
{
	return mError;
}
