#pragma once

#include "resource.h"

typedef struct UPLOAD_INFO
{
	WCHAR szUserID[32]; // user id
	WCHAR szFolder[64]; // ������Ʈ ��
	WCHAR szYear[16]; // �⵵
	WCHAR szDate[32]; // ��¥
	WCHAR szFileName[64]; // ���ϸ�
	WCHAR szLocalPath[2048];
}uploadInfo, *pUploadInfo;