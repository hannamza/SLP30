#pragma once

#include "TSingleton.h"

typedef struct PATTERN_INFO
{
	int nSystem;
	int nFloor;
	int nStair;
	CUIntArray arrayPattern1;
	CUIntArray arrayPattern2;
}patternInfo, *pPatternInfo;

enum {
	pattentype_normal, // 층, 계단별 전층
	pattentype_base, // 층, 계단별 지하
	pattentype_up, // 층, 계단별 지상

	pattentype_exceptalarm, // (경종, 시각경보, 비상방송)제외 층, 계단별
	pattentype_exceptdoor, // (경종, 시각경보, 비상방송)제외 층, 계단별
	pattentype_exceptalarmdoor, // (경종, 시각경보, 비상방송)제외 층, 계단별

	pattentype_alarmnormal,// (경종, 시각경보, 비상방송)포함 층, 계단별 전층
	pattentype_alarmup, // (경종, 시각경보, 비상방송)포함 층, 계단별 지상
	pattentype_alarmdown, // (경종, 시각경보, 비상방송)포함 층, 계단별 지하

	pattentype_doornormal, // 패턴(자동폐쇄, 창문폐쇄, 옥상개폐) 포함 층, 계단별 전층
	pattentype_doorup, // 패턴(자동폐쇄, 창문폐쇄, 옥상개폐) 층, 계단별 지상
	pattentype_doordown, // 패턴(자동폐쇄, 창문폐쇄, 옥상개폐) 층, 계단별 지하
};
class CPatternManager : public TSingleton<CPatternManager>
{
public:
	CPatternManager();
	~CPatternManager();

public:
	void AddPatternInfo(SYSTEM_INFO_* pInfo);
	void AddPatternInfoExceptAlarm(SYSTEM_INFO_* pInfo);
	void AddPatternInfoExceptDoor(SYSTEM_INFO_* pInfo);
	void AddPatternInfoExceptAlarmDoor(SYSTEM_INFO_* pInfo);
	void AddPatternInfo(BC_INFO* pInfo, int nCircuitIndex);
	pPatternInfo GetPatternAlarm(int nSystem, int nStair, int nFloor, int nType);

	int  GetPatternIndex(int nType, int nSystem, int nStair, int nFloor);
	int  GetPatternList(int nType, int nSystem, int nStair, int nFloor, CUIntArray & system, CUIntArray & broad);
	int  GetPatternList(int nStair, int nFloor, CString sPattern, CUIntArray & pattern, CUIntArray & system);

	void AddPatternList(CList<pPatternInfo, pPatternInfo>* pList);
	void MakePatternList();
	int  GetPatternCount();
	pPatternInfo GetPattern(int nIndex);

	void ReleasePattern();

private:
	int  GetPatternIndex(int nType, int nSystem, int nStair, int nFloor, CUIntArray & system, CUIntArray & broad);
	void ReleaseList(CList<pPatternInfo, pPatternInfo> * pList);
	int CommonPattern(int nStair, int nFloor, CUIntArray & pattern);
	int  GetPattern(int nStair, int nFloor, int nType, bool bAll, CUIntArray & pattern);
	int  GetSystemList(int nStair, int nFloor, CString sSystem, CUIntArray & system);

private:
	CList<pPatternInfo, pPatternInfo> m_listPattern;
	CList<pPatternInfo, pPatternInfo> m_listBasePattern;	// 지하패턴
	CList<pPatternInfo, pPatternInfo> m_listUpPattern;		// 지상패턴
	CList<pPatternInfo, pPatternInfo> m_listPattern1;		// 경종, 시각경보, 비상방송 제외 층, 계단별
	CList<pPatternInfo, pPatternInfo> m_listPattern2;		// 패턴(자동폐쇄, 창문폐쇄, 옥상개폐 제외)
	CList<pPatternInfo, pPatternInfo> m_listPattern3;		// 패턴(경종, 시각경보, 비상방송, 자동폐쇄, 창문폐쇄, 옥상개폐 제외)
	patternInfo m_alarmPattern;		// 전층(경종, 시각경보, 비상방송)
	patternInfo m_alarmPatternUp;	// 지상 전층(경종, 시각경보, 비상방송)
	patternInfo m_alarmPatternDown;	// 지하 전층(경종, 시각경보, 비상방송)

	patternInfo m_doorPattern;		// 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	patternInfo m_doorPatternUp;	// 지상 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	patternInfo m_doorPatternDown;	// 지하 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)

	CList<pPatternInfo, pPatternInfo> m_patternTotal;
};

