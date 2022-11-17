#include "stdafx.h"
#include "SharedMemoryManager.h"

#define		SM_NAME		L"SLP30_SM"
#define		SM_SIZE		8192

CSharedMemoryManager::CSharedMemoryManager()
{
	m_hHandle = NULL;
	m_piMemory = NULL;
}

CSharedMemoryManager::~CSharedMemoryManager()
{
	CloseSharedMemory();
}

bool CSharedMemoryManager::CreateSharedMemory()
{
	if (!m_hHandle) {
		m_hHandle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SM_SIZE, SM_NAME);
		if (NULL == m_hHandle) {
			return false;
		}
	}
	return true;
}

bool CSharedMemoryManager::OpenSharedMemory()
{
	if (NULL == m_hHandle) {
		//20221117 GBM start - SLP30에서 공유메모리 이름을 SLP30_SM을 사용하므로 수정 -> 다른 프로젝트까지 확인 필요
		m_hHandle = ::OpenFileMapping(FILE_MAP_READ, NULL, L"SLP30_SM");
		//m_hHandle = ::OpenFileMapping(FILE_MAP_READ, NULL, L"SLP30_SM_MANAGER");
		//20221117 GBM end
		if (NULL == m_hHandle) {
			return false;
		}
	}
	if (NULL != m_hHandle) {
		m_piMemory = (int*)::MapViewOfFile(m_hHandle, FILE_MAP_READ, 0, 0, SM_SIZE);
		if (NULL == m_piMemory) {
			return false;
		}
	}
	return true;
}

bool CSharedMemoryManager::ReadSharedMemory(BYTE* pValue, int nSize)
{
	if (NULL != m_piMemory) {
		memcpy(pValue, m_piMemory, nSize);
	}
	else {
		return false;
	}
	return true;
}

bool CSharedMemoryManager::WriteSharedMemory(BYTE* pValue, int nSize)
{
	if (NULL != m_piMemory) {
		memcpy(m_piMemory, pValue, nSize);
	}
	else {
		return false;
	}
	return true;
}

bool CSharedMemoryManager::CloseSharedMemory()
{
	if (NULL != m_piMemory) {
		BOOL bUnMap = ::UnmapViewOfFile(m_piMemory);
		if (bUnMap) {
			m_piMemory = NULL;
		}
		else {
			return false;
		}
	}
	if (NULL != m_hHandle) {
		BOOL bClose = ::CloseHandle(m_hHandle);
		if (bClose) {
			m_hHandle = NULL;
		}
		else {
			return false;
		}
	}
	return false;
}
