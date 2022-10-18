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

void CCommonState::InitSelectCircuitCompInfo(int nSystem)
{
	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}
	CString sBlock, sStair, sTemp, sFloor, sCircuitName;
	int nCount = 0;

	int nBlockIndex = 0, nStairIndex = 0;
	bool bExit = false;

	for (int nIndexBlock = 0; nIndexBlock < CCircuitBasicInfo::Instance()->m_nBlock; nIndexBlock++)
	{
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sBlock = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nBlockIndex);
			sBlock += L"동";

			for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) {
				sStair.Format(L"%d계단", nIndex + 1);

				for (int nIndexBasement = CCircuitBasicInfo::Instance()->m_nBasement; nIndexBasement > 0; nIndexBasement--)
				{
					sFloor.Format(L"B%dF", nIndexBasement);

					for (int nIndexCircuitParent = 0; nIndexCircuitParent < CIRCUIT_PARENT; nIndexCircuitParent++) 
					{
						sCircuitName = g_lpszCircuitParent[nIndexCircuitParent];

						selectCircuitComp* pSCC = new selectCircuitComp;
						pSCC->sSystem = sSystem;
						pSCC->sBlock = sBlock;
						pSCC->sStair = sStair;
						pSCC->sFloor = sFloor;
						pSCC->sCircuitName = sCircuitName;
						pSCC->nCurrentCount = 0;
						pSCC->nPreviousCount = 0;

						if(nSystem == 0)
							m_vecCalSelectCircuit_0.push_back(pSCC);
						else
							m_vecCalSelectCircuit_1.push_back(pSCC);
					}
				}

				for (int nIndexFloor = 1; nIndexFloor <= CCircuitBasicInfo::Instance()->m_nFloor; nIndexFloor++)
				{
					sFloor.Format(L"%dF", nIndexFloor);

					for (int nIndexCircuitParent = 0; nIndexCircuitParent < CIRCUIT_PARENT; nIndexCircuitParent++)
					{
						sCircuitName = g_lpszCircuitParent[nIndexCircuitParent];

						selectCircuitComp* pSCC = new selectCircuitComp;
						pSCC->sSystem = sSystem;
						pSCC->sBlock = sBlock;
						pSCC->sStair = sStair;
						pSCC->sFloor = sFloor;
						pSCC->sCircuitName = sCircuitName;
						pSCC->nCurrentCount = 0;
						pSCC->nPreviousCount = 0;

						if (nSystem == 0)
							m_vecCalSelectCircuit_0.push_back(pSCC);
						else
							m_vecCalSelectCircuit_1.push_back(pSCC);
					}
				}

			}
		}
	}
}
