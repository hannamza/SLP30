#include "stdafx.h"
#include "CircuitManager.h"


CCircuitManager::CCircuitManager()
{
	m_nBIndex = 0;
	m_nFIndex = 0;
	m_nSIndex = 0;
	m_nRIndex = 0;
}

CCircuitManager::~CCircuitManager()
{
	m_mapBuilding.RemoveAll();
	ReleaseMap(m_mapStairName);
	ReleaseMap(m_mapFloorName);
	ReleaseMap(m_mapRoomName);

	pCommonCircuit pTemp;
	while (!m_releaseCircuit.IsEmpty()) {
		pTemp = m_releaseCircuit.RemoveHead();
		SAFE_DELETE(pTemp);
	}
	pCommonInfo pInfo;
	while (!m_releaseInfo.IsEmpty()) {
		pInfo = m_releaseInfo.RemoveHead();
		SAFE_DELETE(pInfo);
	}
}

void CCircuitManager::ReleaseMap(MAP_INT & map)
{
	MAP_INT::CPair* pair;
	POSITION pos = map.GetStartPosition();
	while (pos) {
		pair = map.GetNext(pos);
		if (pair && pair->m_value) {
			SAFE_DELETE(pair->m_value);
		}
	}
	map.RemoveAll();
}

bool CCircuitManager::AddItem(pCommonMap pMap, CString sName, int nIndex)
{
	int nValue = 0;
	if (pMap->map.Lookup(sName, nValue)) {
		return false;
	}
	pMap->map.SetAt(sName, nIndex);
	return true;
}

bool CCircuitManager::AddItem(MAP_INT & map, int nParentIndex, CString sName, int nIndex)
{
	pCommonMap pMap = NULL;
	if (!map.Lookup(nParentIndex, pMap)) {
		pCommonMap pMap = new commonMap;
		pMap->nNameIndex = nParentIndex;
		map.SetAt(nParentIndex, pMap);
	}
	return AddItem(pMap, sName, nIndex);
}

int CCircuitManager::GetItemIndex(pCommonMap pMap, CString sName)
{
	int nIndex = 0;
	if (pMap->map.Lookup(sName, nIndex)) {
		return nIndex;
	}
	return -1;
}

int CCircuitManager::GetItemIndex(MAP_INT & map, int nParentIndex, CString sName)
{
	pCommonMap pMap;
	if (!map.Lookup(nParentIndex, pMap)) {
		return -1;
	}
	int nSIndex = GetItemIndex(pMap, sName);
	return nSIndex;
}

int CCircuitManager::SetBuildingName(CString sName)
{
	int nIndex = GetBuildingIndex(sName);
	if (nIndex != -1)
	{
		return nIndex;
	}
	mBSync.Enter();
	m_mapBuilding.SetAt(sName, m_nBIndex++);
	mBSync.Leave();

	return m_nBIndex - 1;
}
int CCircuitManager::GetBuildingIndex(CString sName)
{
	mBSync.Enter();
	int nIndex = -1;
	if (m_mapBuilding.Lookup(sName, nIndex)) {
		mBSync.Leave();
		return nIndex;
	}
	mBSync.Leave();
	return -1;
}

int CCircuitManager::SetStairName(CString sBuildingName, CString sName)
{
	int nBIndex = GetBuildingIndex(sBuildingName);
	if (nBIndex == -1) {
		nBIndex = SetBuildingName(sBuildingName);
	}
	mSSync.Enter();
	if (!AddItem(m_mapStairName, nBIndex, sName, m_nSIndex++)) {
		mSSync.Leave();
		return -1;
	}
	mSSync.Leave();

	return m_nSIndex - 1;
}
int CCircuitManager::SetStairName(int nBIndex, CString sName)
{
	mSSync.Enter();
	if (!AddItem(m_mapStairName, nBIndex, sName, m_nSIndex++)) {
		mSSync.Leave();
		return -1;
	}
	mSSync.Leave();

	return m_nSIndex - 1;
}
int CCircuitManager::GetStairIndex(CString sBuildingName, CString sName, int nBIndex /*= -1*/)
{
	if (nBIndex == -1) {
		nBIndex = GetBuildingIndex(sBuildingName);
	}
	if (nBIndex == -1) {
		return -1;
	}
	mSSync.Enter();
	int nSIndex = GetItemIndex(m_mapStairName, nBIndex, sName);
	mSSync.Leave();

	return nSIndex;
}

int CCircuitManager::SetFloorName(CString sBuildingName, CString sStairName, CString sName)
{
	int nSIndex = GetStairIndex(sBuildingName, sStairName);
	if (nSIndex == -1) {
		nSIndex = SetStairName(sBuildingName, sStairName);
	}
	mFSync.Enter();
	if (!AddItem(m_mapFloorName, nSIndex, sName, m_nFIndex++)) {
		mFSync.Leave();
		return -1;
	}
	mFSync.Leave();

	return m_nFIndex - 1;
}
int CCircuitManager::SetFloorName(int nSIndex, CString sName)
{
	mFSync.Enter();
	if (!AddItem(m_mapFloorName, nSIndex, sName, m_nFIndex++)) {
		mFSync.Leave();
		return -1;
	}
	mFSync.Leave();

	return m_nFIndex - 1;
}
int CCircuitManager::GetFloorIndex(CString sBuildingName, CString sStairName, CString & sName)
{
	int nSIndex = GetStairIndex(sBuildingName, sStairName);
	if (nSIndex == -1) {
		return -1;
	}
	mFSync.Enter();
	int nFIndex = GetItemIndex(m_mapFloorName, nSIndex, sName);
	mFSync.Leave();
	return nFIndex;
}

int CCircuitManager::SetRoomName(CString sBuildingName, CString sStairName, CString sFloorName, CString sName)
{
	int nFIndex = GetFloorIndex(sBuildingName, sStairName, sFloorName);
	if (nFIndex == -1) {
		nFIndex = SetFloorName(sBuildingName, sStairName, sFloorName);
	}
	mRSync.Enter();
	if(!AddItem(m_mapRoomName, nFIndex, sName, m_nRIndex++)) {
		mRSync.Leave();
		return -1;
	}
	mRSync.Leave();

	return m_nFIndex - 1;
}
int CCircuitManager::SetRoomName(int nFIndex, CString sName)
{
	mRSync.Enter();
	if (!AddItem(m_mapRoomName, nFIndex, sName, m_nRIndex++)) {
		mRSync.Leave();
		return -1;
	}
	mRSync.Leave();

	return m_nFIndex - 1;
}
int CCircuitManager::GetRoomIndex(CString sBuildingName, CString sStairName, CString sFloorName, CString & sName)
{
	int nFIndex = GetFloorIndex(sBuildingName, sStairName, sFloorName);
	if (nFIndex == -1) {
		return -1;
	}
	mRSync.Enter();
	int nRIndex = GetItemIndex(m_mapRoomName, nFIndex, sName);
	mRSync.Leave();
	return nRIndex;
}

bool CCircuitManager::AddCircuit(int nIndex, MAP_CIRCUIT & map, pCommonCircuit pCircuit)
{
	pCommonInfo pInfo = NULL;
	if (!map.Lookup(nIndex, pInfo)) {
		pInfo = new commonInfo;
		pInfo->nIndex = nIndex;
		pInfo->list.AddTail(pCircuit);

		m_releaseInfo.AddTail(pInfo);
	}
	if (!pInfo) {
		return false;
	}
	pInfo->list.AddTail(pCircuit);

	return true;
}

int CCircuitManager::SetCircuitInfo(int nCircuitNo, int nCircuitType, CString sCircuitName, CString sBuildingName, CString sStairName, CString sFloorName, CString sRoomName)
{
	int nBIndex = SetBuildingName(sBuildingName);

	int nSIndex = SetStairName(sBuildingName, sStairName);
	if (nSIndex == -1) {
		return -1;
	}
	int nFIndex = SetFloorName(sBuildingName, sStairName, sFloorName);
	if (nFIndex != -1) {
		return -2;
	}
	int nRIndex = SetRoomName(nFIndex, sRoomName);
	if (nRIndex != -1) {
		return -3;
	}
	pCommonCircuit pCircuit = new commonCircuit;
	pCircuit->nCircuitNo = nCircuitNo;
	pCircuit->nCircuitType = nCircuitType;
	pCircuit->sName = sCircuitName;

	m_releaseCircuit.AddTail(pCircuit);

	AddCircuit(nBIndex, m_mapBuildingCircuit, pCircuit);
	AddCircuit(nBIndex, m_mapStairNameCircuit, pCircuit);
	AddCircuit(nBIndex, m_mapFloorNameCircuit, pCircuit);
	AddCircuit(nBIndex, m_mapRoomNameCircuit, pCircuit);

	return 0;
}

void CCircuitManager::GetMapList(int nIndex, MAP_CIRCUIT & map, LIST_CIRCUIT & list)
{
	pCommonInfo pInfo;
	pCommonCircuit pCircuit;
	if (map.Lookup(nIndex, pInfo) && pInfo) {
		POSITION pos = pInfo->list.GetHeadPosition();
		while (pos) {
			pCircuit = pInfo->list.GetNext(pos);
			list.AddTail(pCircuit);
		}
	}
}

// nType: 0-건물, 1-계단, 2-층, 3-실 
int CCircuitManager::GetCircuitList(LIST_CIRCUIT & list, int nType, CString sBuildingName/* = L""*/, CString sStairName/* = L""*/, CString sFloorName/* = L""*/, CString sRoomName/* = L""*/)
{
	int nIndex = -1;
	switch (nType)
	{
	case 0: // 건물
		nIndex = GetBuildingIndex(sBuildingName);
		if (nIndex == -1) {
			return -1;
		}
		GetMapList(nIndex, m_mapBuildingCircuit, list);
		break;
	case 1: // 계단
		nIndex = GetStairIndex(sBuildingName, sStairName);
		if (nIndex == -1) {
			return -2;
		}
		GetMapList(nIndex, m_mapStairNameCircuit, list);
		break;
	case 2: // 층
		nIndex = GetFloorIndex(sBuildingName, sStairName, sFloorName);
		if (nIndex == -1) {
			return -3;
		}
		GetMapList(nIndex, m_mapFloorNameCircuit, list);
		break;
	case 3: // 실
		nIndex = GetRoomIndex(sBuildingName, sStairName, sFloorName, sRoomName);
		if (nIndex == -1) {
			return -4;
		}
		GetMapList(nIndex, m_mapRoomNameCircuit, list);
		break;
	default:
		return -5;
		break;
	}
	return 0;
}