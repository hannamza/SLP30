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
		case 1:  pType->sInput = L"소화전"; pType->sEquip = L"자탐감지기"; pType->sOutput = L"지구 경종"; pType->sOutputExp = L"지구 경종"; break;
		case 2:  pType->sInput = L"발신기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L"지구 경종"; pType->sOutputExp = L"지구 경종"; break;
		case 3:  pType->sInput = L"감지기 (계단)"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 4:  pType->sInput = L"감지기 (ELEV)"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 5:  pType->sInput = L"감지기"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 6:  pType->sInput = L"아날로그 광전"; pType->sEquip = L"아날로그 정온"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 7:  pType->sInput = L"아날로그 정온"; pType->sEquip = L"아날로그 광전"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 8:  pType->sInput = L"주소형 광전"; pType->sEquip = L"주소형 광전"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 9:  pType->sInput = L"주소형 정온"; pType->sEquip = L"주소형 정온"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 10: pType->sInput = L"주소형 차동"; pType->sEquip = L"주소형 차동"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 11: pType->sInput = L"감지기 A (SVP)"; pType->sEquip = L"A교차감지기"; pType->sOutput = L"싸이렌"; pType->sOutputExp = L"싸이렌"; break;
		case 12: pType->sInput = L"감지기 B (SVP)"; pType->sEquip = L"B교차감지기"; pType->sOutput = L"프리액션"; pType->sOutputExp = L"밸브"; break;
		case 13: pType->sInput = L"P/S 확인 (SVP)"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 14: pType->sInput = L"T/S 확인 (SVP)"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 15: pType->sInput = L"알람밸브 P/S"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 16: pType->sInput = L"알람벨브 T/S"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 17: pType->sInput = L"광전 (셔터)"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 18: pType->sInput = L"정온 (셔터)"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 19: pType->sInput = L"셔터 1차 폐쇄"; pType->sEquip = L"일반 확인"; pType->sOutput = L"방화 셔터"; pType->sOutputExp = L"1차 폐쇄"; break;
		case 20: pType->sInput = L"셔터 2차 폐쇄"; pType->sEquip = L"일반 확인"; pType->sOutput = L"방화 셔터"; pType->sOutputExp = L"2차 폐쇄"; break;
		case 21: pType->sInput = L"전실 급기댐퍼"; pType->sEquip = L"일반 확인"; pType->sOutput = L"전실 제연"; pType->sOutputExp = L"전실 제연"; break;
		case 22: pType->sInput = L"전실 급기수동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 23: pType->sInput = L"전실 배기댐퍼"; pType->sEquip = L"일반확인"; pType->sOutput = L"전실 제연"; pType->sOutputExp = L"전실 제연"; break;
		case 24: pType->sInput = L"전실 배기수동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 25: pType->sInput = L"자동 폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문 제어"; pType->sOutputExp = L"자동 폐쇄"; break;
		case 26: pType->sInput = L"창문 폐쇄"; pType->sEquip = L"일반확인"; pType->sOutput = L"창문 제어"; pType->sOutputExp = L"창문 폐쇄"; break;
		case 27: pType->sInput = L"배연창"; pType->sEquip = L"일반확인"; pType->sOutput = L"창문 제어"; pType->sOutputExp = L"배연창"; break;
		case 28: pType->sInput = L"방화문"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문 제어"; pType->sOutputExp = L"방화문"; break;
		case 29: pType->sInput = L"P/L 점등"; pType->sEquip = L"일반확인"; pType->sOutput = L"기타"; pType->sOutputExp = L"소화전 기동"; break;
		case 30: pType->sInput = L"유도등 점등"; pType->sEquip = L"일반확인"; pType->sOutput = L"기타"; pType->sOutputExp = L"유도등 정지"; break;
		case 31: pType->sInput = L"가스감지기A"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 32: pType->sInput = L"가스감지기B"; pType->sEquip = L"자탐감지기"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 33: pType->sInput = L"가스방출확인"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 34: pType->sInput = L"가스수동기동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 35: pType->sInput = L"저수위 F/S"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 36: pType->sInput = L"P/S 확인 (PUMP)"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 37: pType->sInput = L"T/S 확인 (PUMP)"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 38: pType->sInput = L"제연 수동기동"; pType->sEquip = L"화재용접점"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 39: pType->sInput = L"상가댐퍼 확인"; pType->sEquip = L"일반확인"; pType->sOutput = L"상가 제연"; pType->sOutputExp = L"거실댐퍼"; break;
		case 40: pType->sInput = L"복합댐퍼 확인"; pType->sEquip = L"일반확인"; pType->sOutput = L"전실 제연"; pType->sOutputExp = L"복합댐퍼"; break;
		case 41: pType->sInput = L"급기팬"; pType->sEquip = L"일반확인"; pType->sOutput = L"제연팬"; pType->sOutputExp = L"급기팬"; break;
		case 42: pType->sInput = L"배기팬"; pType->sEquip = L"일반확인"; pType->sOutput = L"제연팬"; pType->sOutputExp = L"배기팬"; break;
		case 43: pType->sInput = L"저수위 T/S"; pType->sEquip = L"감시 비지속"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 44: pType->sInput = L"옥상문"; pType->sEquip = L"일반확인"; pType->sOutput = L"출입문 제어"; pType->sOutputExp = L"옥상문"; break;
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
