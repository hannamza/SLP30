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

	//�� ������ ����
	ReleaseCircuitCompInfo(0);
	ReleaseCircuitCompInfo(1);
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
	CString sSystem = L"0 ����";
	if (nSystem == 1) {
		sSystem = L"1 ����";
	}
	CString sBlock, sStair, sTemp, sFloor, sCircuitName;
	int nCount = 0;
	bool bExit = false;

	for (int nIndexBlock = 0; nIndexBlock < CCircuitBasicInfo::Instance()->m_nBlock; nIndexBlock++)
	{
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sBlock = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndexBlock);
			sBlock += L"��";

			for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) 
			{
				sStair.Format(L"%d���", nIndex + 1);

				for (int nIndexBasement = CCircuitBasicInfo::Instance()->m_nBasement; nIndexBasement > 0; nIndexBasement--)
				{
					sFloor.Format(L"B%dF", nIndexBasement);

					//��������� ������ ����� ��������� ��������� ȸ�θ����� �ִ´�.
					for (int nIndexCircuitParent = 0; nIndexCircuitParent < CIRCUIT_PARENT; nIndexCircuitParent++) 
					{
						sCircuitName = g_lpszCircuitParent[nIndexCircuitParent];

						if (nIndexCircuitParent != CIRCUIT_PARENT - 3)
						{
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
						else //�������
						{
							CStringArray strArr;
							strArr.RemoveAll();

							CCircuitBasicInfo::Instance()->GetCircuitChild(sCircuitName, strArr);

							for (int i = 0; i < strArr.GetSize(); i++)
							{
								selectCircuitComp* pSCC = new selectCircuitComp;
								pSCC->sSystem = sSystem;
								pSCC->sBlock = sBlock;
								pSCC->sStair = sStair;
								pSCC->sFloor = sFloor;
								pSCC->sCircuitName = strArr.GetAt(i);
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

				for (int nIndexFloor = 1; nIndexFloor <= CCircuitBasicInfo::Instance()->m_nFloor; nIndexFloor++)
				{
					sFloor.Format(L"%dF", nIndexFloor);

					//��������� ������ ����� ��������� ��������� ȸ�θ����� �ִ´�.
					for (int nIndexCircuitParent = 0; nIndexCircuitParent < CIRCUIT_PARENT; nIndexCircuitParent++)
					{
						sCircuitName = g_lpszCircuitParent[nIndexCircuitParent];

						if (nIndexCircuitParent != CIRCUIT_PARENT - 3)
						{
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
						else //�������
						{
							CStringArray strArr;
							strArr.RemoveAll();

							CCircuitBasicInfo::Instance()->GetCircuitChild(sCircuitName, strArr);

							for (int i = 0; i < strArr.GetSize(); i++)
							{
								selectCircuitComp* pSCC = new selectCircuitComp;
								pSCC->sSystem = sSystem;
								pSCC->sBlock = sBlock;
								pSCC->sStair = sStair;
								pSCC->sFloor = sFloor;
								pSCC->sCircuitName = strArr.GetAt(i);
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
	}
}

int CCommonState::CalculateCircuitAddressCount(CString strCircuitName)
{
	int nRet = 0;

	int nCircuitIndex = -1;

	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++)
	{
		if (strCircuitName.Compare(g_lpszCircuitName[nIndex]) == 0)
		{
			nCircuitIndex = nIndex;
			break;
		}
	}

	switch (nCircuitIndex)
	{
	case �����_��ȭ��:
	case �����_�߽ű�:
	case �����_�ð��溸:
	case �����_�迬â:
	case �����_��ȭ��:
	case �����_â�����:
	case �����_�ڵ����:
	case �����_P_L����:
	case �����_����:
	case �����_�ޱ���:
	case �����_�����:
	case �����_��ܰ�����:
	case �����_ELEV������:
	case �����_TSȮ��:
	case �����_�����_�ǳ���ٸ�Ȯ��:
		nRet = 2;
		break;
	default:
		nRet = 1;
		break;
	}

	return nRet;
}

int CCommonState::CalculateTotalCircuitCount(int nSystem)
{
	int nRet = 0;
	POSITION pos;
	pSelectCircuit pSC;
	CStringArray strArrCircuitName;
	bool bDetector;

	if (nSystem == 0)
	{
		pos = CCommonState::ie()->m_selectCircuit_0.GetHeadPosition();
		while (pos)
		{
			pSC = CCommonState::ie()->m_selectCircuit_0.GetNext(pos);

			//���� ���õǾ�� ������ 0�̸� ȸ�ι�ȣ ������� ����
			if(pSC->nCount == 0)
				continue;

			bDetector = false;
			bDetector = CCircuitBasicInfo::Instance()->IsDetector(pSC->sCircuitName);
			if (!bDetector)
			{
				for (int nCnt = 0; nCnt < pSC->nCount; nCnt++)
				{
					strArrCircuitName.RemoveAll();
					CString sCircuitName = pSC->sCircuitName;
					CCircuitBasicInfo::Instance()->GetCircuitChild(sCircuitName, strArrCircuitName);
					int nArrCount = 0;
					nArrCount = strArrCircuitName.GetSize();
					for (int i = 0; i < nArrCount; i++)
					{
						CString str = strArrCircuitName.GetAt(i);
						int n = CalculateCircuitAddressCount(str);
						nRet += n;
					}
				}
			}
			else
			{
				for (int nCnt = 0; nCnt < pSC->nCount; nCnt++)
				{
					int n = CalculateCircuitAddressCount(pSC->sCircuitName);
					nRet += n;
				}
			}

			
		}
	}
	else
	{
		pos = CCommonState::ie()->m_selectCircuit_1.GetHeadPosition();
		while (pos)
		{
			pSC = CCommonState::ie()->m_selectCircuit_1.GetNext(pos);

			//���� ���õǾ�� ������ 0�̸� ȸ�ι�ȣ ������� ����
			if (pSC->nCount == 0)
				continue;

			bDetector = false;
			bDetector = CCircuitBasicInfo::Instance()->IsDetector(pSC->sCircuitName);
			if (!bDetector)
			{
				for (int nCnt = 0; nCnt < pSC->nCount; nCnt++)
				{
					strArrCircuitName.RemoveAll();
					CString sCircuitName = pSC->sCircuitName;
					CCircuitBasicInfo::Instance()->GetCircuitChild(sCircuitName, strArrCircuitName);
					int nArrCount = 0;
					nArrCount = strArrCircuitName.GetSize();
					for (int i = 0; i < nArrCount; i++)
					{
						CString str = strArrCircuitName.GetAt(i);
						int n = CalculateCircuitAddressCount(str);
						nRet += n;
					}
				}
			}
			else
			{
				for (int nCnt = 0; nCnt < pSC->nCount; nCnt++)
				{
					int n = CalculateCircuitAddressCount(pSC->sCircuitName);
					nRet += n;
				}
			}
		}
	}

	return nRet;
}

void CCommonState::ReleaseCircuitCompInfo(int nSystem)
{
	std::vector<selectCircuitComp*>::iterator iter;
	if (nSystem == 0)
	{
		iter = m_vecCalSelectCircuit_0.begin();
		for (; iter != m_vecCalSelectCircuit_0.end(); iter++)
		{
			delete *iter;
		}

		m_vecCalSelectCircuit_0.clear();
	}
	else
	{
		iter = m_vecCalSelectCircuit_1.begin();
		for (; iter != m_vecCalSelectCircuit_1.end(); iter++)
		{
			delete *iter;
		}

		m_vecCalSelectCircuit_1.clear();
	}
}