#include "stdafx.h"
#include "BaseInfoManager.h"


CBaseInfoManager::CBaseInfoManager()
{
	InitializeBasic();
}

CBaseInfoManager::~CBaseInfoManager()
{
	ReleaseManager();
}

void CBaseInfoManager::InitializeBasic()
{
	pBasicType pType = NULL;
	for (int nIndex = 0; nIndex < BASIC_COUNT; nIndex) {
		pBasicType pType = new basicType;
		pType->nIndex = nIndex + 1;
		switch (nIndex) {
		case 1:  pType->sInput = L"소화전"; pType->sEquip = L"자탐감지기"; pType->sOutput = L"지구음향"; pType->sOutputExp = L"지구 경종"; break;
		case 2:  pType->sInput = L"발신기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L"지구음향"; pType->sOutputExp = L"지구 경종"; break;
		case 3:  pType->sInput = L"계단감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 4:  pType->sInput = L"ELEV감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 5:  pType->sInput = L"감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 6:  pType->sInput = L"AN연기"; pType->sEquip = L"열식아나로그"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 7:  pType->sInput = L"AN정온"; pType->sEquip = L"연식아나로그"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 8:  pType->sInput = L"주소형연기"; pType->sEquip = L"연식 주소형"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 9:  pType->sInput = L"주소형정온"; pType->sEquip = L"열식 주소형"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 10: pType->sInput = L"주소형차동"; pType->sEquip = L"열식 주소형"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 11: pType->sInput = L"SVP감지기A"; pType->sEquip = L"A교차감지기"; pType->sOutput = L"싸이렌"; pType->sOutputExp = L"싸이렌"; break;
		case 12: pType->sInput = L"SVP감지기B"; pType->sEquip = L"B교차감지기"; pType->sOutput = L"프리액션"; pType->sOutputExp = L"밸브"; break;
		case 13: pType->sInput = L"SVP P/S"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 14: pType->sInput = L"SVP T/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 15: pType->sInput = L"AV P/S"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 16: pType->sInput = L"AV T/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 17: pType->sInput = L"셔터연감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 18: pType->sInput = L"셔터열감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 19: pType->sInput = L"셔터1차폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"방화셔터"; pType->sOutputExp = L"1차폐쇄"; break;
		case 20: pType->sInput = L"셔터2차폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"방화셔터"; pType->sOutputExp = L"2차폐쇄"; break;
		case 21: pType->sInput = L"전실급기댐퍼"; pType->sEquip = L"일반확인"; pType->sOutput = L"전실제연"; pType->sOutputExp = L"전실제연"; break;
		case 22: pType->sInput = L"전실급기수동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 23: pType->sInput = L"전실배기댐퍼"; pType->sEquip = L"일반확인"; pType->sOutput = L"전실제연"; pType->sOutputExp = L"전실제연"; break;
		case 24: pType->sInput = L"전실배기수동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 25: pType->sInput = L"자동폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문제어"; pType->sOutputExp = L"자동폐쇄"; break;
		case 26: pType->sInput = L"창문폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"창문제어"; pType->sOutputExp = L"창문폐쇄"; break;
		case 27: pType->sInput = L"배연창"; pType->sEquip = L"일반확인"; pType->sOutput = L"창문제어"; pType->sOutputExp = L"배연창"; break;
		case 28: pType->sInput = L"방화문"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문제어"; pType->sOutputExp = L"방화문"; break;
		case 29: pType->sInput = L"P/L점등"; pType->sEquip = L"일반확인"; pType->sOutput = L"기타"; pType->sOutputExp = L"소화전 기동"; break;
		case 30: pType->sInput = L"유도등점등"; pType->sEquip = L"일반확인"; pType->sOutput = L"기타"; pType->sOutputExp = L"유도등 정지"; break;
		case 31: pType->sInput = L"가스감지기A"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 32: pType->sInput = L"가스감지기B"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 33: pType->sInput = L"가스방출확인"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 34: pType->sInput = L"가스수동기동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 35: pType->sInput = L"저수위F/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 36: pType->sInput = L"PUMP P/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 37: pType->sInput = L"PUMP T/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 38: pType->sInput = L"제연수동기동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 39: pType->sInput = L"SMD 확인"; pType->sEquip = L"일반확인"; pType->sOutput = L"상가제연"; pType->sOutputExp = L"거실댐퍼"; break;
		case 40: pType->sInput = L"MVD 확인"; pType->sEquip = L"일반확인"; pType->sOutput = L"전실제연"; pType->sOutputExp = L"복합댐퍼"; break;
		case 41: pType->sInput = L"급기팬"; pType->sEquip = L"일반확인"; pType->sOutput = L"제연팬"; pType->sOutputExp = L"급기팬"; break;
		case 42: pType->sInput = L"배기팬"; pType->sEquip = L"일반확인"; pType->sOutput = L"제연팬"; pType->sOutputExp = L"배기팬"; break;
		case 43: pType->sInput = L"저수위T/S"; pType->sEquip = L"감시비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 44: pType->sInput = L"옥상문"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문제어"; pType->sOutputExp = L"옥상문"; break;
		default: break;
		}
		m_listBasic.AddTail(pType);
		m_mapBasic.SetAt(nIndex, pType);
	}
}

void CBaseInfoManager::ReleaseManager()
{
	pBasicType pType = NULL;
	while (!m_listBasic.IsEmpty()) {
		pType = m_listBasic.RemoveHead();
		SAFE_DELETE(pType);
	}
	m_mapBasic.RemoveAll();
}

pBasicType CBaseInfoManager::GetBasicType(CString sEquip)
{
	pBasicType pType = NULL;
	POSITION pos = m_listBasic.GetHeadPosition();
	while (pos) {
		pType = m_listBasic.GetNext(pos);
		if (pType && pType->sEquip == sEquip) {
			return pType;
		}
	}
	return NULL;
}

pBasicType CBaseInfoManager::GetBasicType(int nIndex)
{
	pBasicType pType = NULL;
	if (m_mapBasic.Lookup(nIndex, pType)) {
		return pType;
	}
	return NULL;
}
