#include "stdafx.h"
#include "ReadWriteState.h"

CReadWriteState::CReadWriteState(void)
{
}

CReadWriteState::~CReadWriteState(void)
{
}

void CReadWriteState::SetFileName(CString sFileName, int nType/* = 0*/)
{
	m_sFileName = sFileName;

	if (nType == 0) {
		CString sPath;
		TCHAR szPath[2048];
		GetModuleFileName(NULL, szPath, 2048);
		PathRemoveFileSpec(szPath);

		m_sFullPath.Format(_T("%s\\%s"), szPath, m_sFileName);
	}
	else if (nType == 1) {
		WCHAR szDocPath[2048], szPath[2048];
		SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS, FALSE);
		swprintf_s(szPath, L"%s\\%s\\", szDocPath, L"SLP30");
		if (!::PathIsDirectory(szPath)) {
			CCommonFunc::CreateDirectory(szPath);
		}
		m_sFullPath.Format(_T("%s\\%s"), szPath, m_sFileName);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CReadWriteState::WriteValue(CString sSection, CString sKey, CString sValue)
{
	BOOL bResult = WritePrivateProfileString(sSection, sKey, sValue, m_sFullPath);

	return bResult;
}
BOOL CReadWriteState::ReadValue(CString sSection, CString sKey, CString & sValue)
{
	TCHAR szValue[2048];
	DWORD nSize = GetPrivateProfileString(sSection, sKey, _T(""), szValue, 2048, m_sFullPath);

	if(nSize > 0)
		sValue = szValue;
	else return FALSE;

	return TRUE;
}
BOOL CReadWriteState::UpdateValue(CString sSection, CString sKey, CString & sValue)
{
	TCHAR szValue[2048];
	DWORD nSize = GetPrivateProfileString(sSection, sKey, _T(""), szValue, 2048, m_sFullPath);

	if(nSize > 0)
		sValue = szValue;
	else return FALSE;

	return TRUE;
}
BOOL CReadWriteState::WriteUpdate(CString sSection, CString sKey, CString sValue)
{
	BOOL bResult = WritePrivateProfileString(sSection, sKey, sValue, m_sFullPath);

	return bResult;
}
BOOL CReadWriteState::WriteUpdate(CString sSection, CString sKey, int nValue)
{
	CString sValue;
	sValue.Format(_T("%d"), nValue);
	return WriteUpdate(sSection, sKey, sValue);
}
//////////////////////////////////////////////////////////////////////////////////////////////////


// WRITE /////////////////////////////////////////////////////////////////////////////////////////
BOOL CReadWriteState::WriteState(CString sSection, CString sKey, CString sValue)
{
	return WriteValue(sSection, sKey, sValue);
}
BOOL CReadWriteState::WriteState(CString sSection, CString sKey, int nValue)
{
	CString sValue;
	sValue.Format(_T("%d"), nValue);
	return WriteValue(sSection, sKey, sValue);
}
BOOL CReadWriteState::WriteState(CString sSection, CString sKey, float fValue)
{
	CString sValue;
	sValue.Format(_T("%f"), fValue);
	return WriteValue(sSection, sKey, sValue);
}


// READ //////////////////////////////////////////////////////////////////////////////////////////
BOOL CReadWriteState::ReadState(CString sSection, CString sKey, CString & sValue)
{
	return ReadValue(sSection, sKey, sValue);
}
BOOL CReadWriteState::ReadState(CString sSection, CString sKey, int & nValue)
{
	CString sValue;
	if(ReadValue(sSection, sKey, sValue))
	{
		nValue = _ttoi(sValue.GetBuffer(0));
		sValue.ReleaseBuffer();
		return TRUE;
	}

	return FALSE;
}
BOOL CReadWriteState::ReadState(CString sSection, CString sKey, float & fValue)
{
	CString sValue;
	if(ReadValue(sSection, sKey, sValue))
	{
		fValue = (float)_ttof(sValue.GetBuffer(0));
		sValue.ReleaseBuffer();
		return TRUE;
	}

	return FALSE;
}
BOOL CReadWriteState::ReadState(CString sSection, CString sKey, double & dValue)
{
	CString sValue;
	if(ReadValue(sSection, sKey, sValue))
	{
		dValue = _ttof(sValue.GetBuffer(0));
		sValue.ReleaseBuffer();
		return TRUE;
	}

	return FALSE;
}
BOOL CReadWriteState::ReadUpdate(CString sSection, CString sKey, int & nValue)
{
	CString sValue;
	if(UpdateValue(sSection, sKey, sValue))
	{
		nValue = _ttoi(sValue.GetBuffer(0));
		sValue.ReleaseBuffer();
		return TRUE;
	}

	return FALSE;
}
BOOL CReadWriteState::ReadText(CString sSection, CString sKey, CString & sText)
{
	CString sValue;
	if(UpdateValue(sSection, sKey, sText))
	{
		return TRUE;
	}

	return FALSE;
}