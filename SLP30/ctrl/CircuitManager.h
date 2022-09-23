#pragma once

#include <atlcoll.h>

typedef struct COMMON_CIRCUIT
{
	int nCircuitNo;
	int nCircuitType;
	CString sName;
}commonCircuit, *pCommonCircuit;
typedef CList<pCommonCircuit, pCommonCircuit> LIST_CIRCUIT;

typedef struct COMMON_INFO
{
	int nIndex;
	LIST_CIRCUIT list;
}commonInfo, *pCommonInfo;
typedef CList<pCommonInfo, pCommonInfo> LIST_INFO;

typedef CAtlMap<int, pCommonInfo> MAP_CIRCUIT;

//--------------------------------------------------

typedef struct COMMON_MAP
{
	int nNameIndex;
	CAtlMap<CAtlString, int> map;
}commonMap, *pCommonMap;

typedef CAtlMap<int, pCommonMap> MAP_INT;
typedef CAtlMap<CAtlString, int> MAP_STRING;

class CCircuitManager
{
public:
	CCircuitManager();
	~CCircuitManager();

public:
	int  SetBuildingName(CString sName);
	int  GetBuildingIndex(CString sName);

	int  SetStairName(CString sBuildingName, CString sName);
	int  SetStairName(int nBIndex, CString sName);
	int  GetStairIndex(CString sBuildingName, CString sName, int nBIndex = -1);

	int  SetFloorName(CString sBuildingName, CString sStairName, CString sName);
	int  SetFloorName(int nSIndex, CString sName);
	int  GetFloorIndex(CString sBuildingName, CString sStairName, CString & sName);

	int  SetRoomName(CString sBuildingName, CString sStairName, CString sFloorName, CString sName);
	int  SetRoomName(int nFIndex, CString sName);
	int  GetRoomIndex(CString sBuildingName, CString sStairName, CString sFloorName, CString & sName);

	// 전체 리스트 관리
	int  SetCircuitInfo(int nCircuitNo, int nCircuitType, CString sCircuitName, CString sBuildingName, CString sStairName, CString sFloorName, CString sRoomName);
	// nType: 0-건물, 1-계단, 2-층, 3-실 
	int  GetCircuitList(LIST_CIRCUIT & list, int nType, CString sBuildingName = L"", CString sStairName = L"", CString sFloorName = L"", CString sRoomName = L"");

private:
	bool AddItem(pCommonMap pMap, CString sName, int nIndex);
	bool AddItem(MAP_INT & map, int nParentIndex, CString sName, int nIndex);
	int  GetItemIndex(pCommonMap pMap, CString sName);
	int  GetItemIndex(MAP_INT & map, int nParentIndex, CString sName);
	bool AddCircuit(int nIndex, MAP_CIRCUIT & map, pCommonCircuit pCircuit);
	void ReleaseMap(MAP_INT & map);
	void GetMapList(int nIndex, MAP_CIRCUIT & map, LIST_CIRCUIT & list);

private:
	MAP_STRING m_mapBuilding;
	MAP_INT m_mapStairName;
	MAP_INT m_mapFloorName;
	MAP_INT m_mapRoomName;

	MAP_CIRCUIT m_mapBuildingCircuit;
	MAP_CIRCUIT m_mapStairNameCircuit;
	MAP_CIRCUIT m_mapFloorNameCircuit;
	MAP_CIRCUIT m_mapRoomNameCircuit;

	LIST_CIRCUIT m_releaseCircuit;
	LIST_INFO m_releaseInfo;

	int m_nBIndex, m_nSIndex, m_nFIndex, m_nRIndex;

	CCriticalSectionEx mBSync, mSSync, mFSync, mRSync;
};

