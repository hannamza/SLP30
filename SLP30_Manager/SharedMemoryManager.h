#pragma once

class CSharedMemoryManager : public TSingleton<CSharedMemoryManager>
{
public:
	CSharedMemoryManager();
	~CSharedMemoryManager();

public:
	bool CreateSharedMemory();
	bool OpenSharedMemory();
	bool ReadSharedMemory(BYTE* pValue, int nSize);
	bool WriteSharedMemory(BYTE* pValue, int nSize);
	bool CloseSharedMemory();

private:
	HANDLE m_hHandle;
	int* m_piMemory;
};

