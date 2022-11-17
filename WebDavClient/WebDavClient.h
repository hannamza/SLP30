#pragma once

#include "resource.h"

typedef struct UPLOAD_INFO
{
	WCHAR szUserID[32]; // user id
	WCHAR szFolder[64]; // 프로젝트 명
	WCHAR szYear[16]; // 년도
	WCHAR szDate[32]; // 날짜
	WCHAR szFileName[64]; // 파일명
	WCHAR szLocalPath[2048];
}uploadInfo, *pUploadInfo;