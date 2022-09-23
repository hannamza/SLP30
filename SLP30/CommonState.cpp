#include "stdafx.h"
#include "CommonState.h"


CCommonState::CCommonState()
{
	m_pWnd = NULL;
	m_pEditWnd = NULL;
	m_bLogin = false;
	m_bMax = false;
	m_bMenu = false;
	m_bInitCircuit = true;

	// test++
	//m_sUserID = L"gfsadmin";
}

CCommonState::~CCommonState()
{
	ReleaseCircuit(0);
	ReleaseCircuit(1);
}

void CCommonState::ReleaseCircuit(int nType)
{
	pSelectCircuit pCircuit = NULL;
	if (nType == 0) {
		while (!m_selectCircuit_0.IsEmpty()) {
			pCircuit = m_selectCircuit_0.RemoveHead();
			SAFE_DELETE(pCircuit);
		}
	}
	else {
		while (!m_selectCircuit_1.IsEmpty()) {
			pCircuit = m_selectCircuit_1.RemoveHead();
			SAFE_DELETE(pCircuit);
		}
	}
}

void CCommonState::SetResult(int nIndex, bool bValue)
{
	m_bResult[nIndex] = bValue;
}

bool CCommonState::GetResult(int nIndex)
{
	return m_bResult[nIndex];
}
