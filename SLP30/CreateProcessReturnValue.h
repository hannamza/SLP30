#pragma once

class CCreateProcessReturnValue
{
public:
	CCreateProcessReturnValue(void);
	~CCreateProcessReturnValue(void);

private:
	INT			m_nIndex;
	INT			m_nReturnValue;
	CString		m_sPath, m_sArgs;
	CWnd*		m_pWnd;

public:
	void StartCreateProcess(INT nIndex, CString sPath, CString sArgs, CWnd* pWnd);
	bool SendDataToWebdav(CString sLocalPath, CString sRemoteFileName, CString sID);

public:
	void CreateProcessReturnValueStart();
	void CreateProcessReturnValueStop();
	void CreateProcessReturnValue();

	HANDLE		mEventHandle;
	HANDLE		m_hThreadHandle;
	BOOL		m_bThread, m_bTerminate;

};

