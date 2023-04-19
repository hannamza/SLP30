#pragma once

#include "TSingleton.h"

typedef struct PATTERN_INFO
{
	PATTERN_INFO() {
		bBroad = false;
		nFloor = 0;
	}
	bool bBroad;
	int nFloor;
	int nStair;
	CString sBlock;
	CString sPatternName;
	CStringArray arrayPattern;
	CStringArray arrayBroad;
}patternInfo, *pPatternInfo;

// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair
enum {
	패턴_알람지하 = 1,	//1
	패턴_알람, 
	패턴_시각경보,		//3
	패턴_알람층별,
	패턴_문전층,		//5
	패턴_문계단별전층, 
	패턴_댐퍼전층,		//7
	패턴_댐퍼계단별,	//8
};

enum {
	생성_동_전층 = 0, 
	생성_동_계단별, 
	생성_동_계단_층,
};

#define		FIXED_PATTERN		1

class CPatternManager : public TSingleton<CPatternManager>
{
public:
	CPatternManager();
	~CPatternManager();

public:
	// 패턴에 추가
	void AddPatternInfo(SYSTEM_INFO_* pInfo);
	// 설비별 연동 목록
	void GetPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern, CStringArray & system);
	void GetPattern(SYSTEM_INFO_* pInfo, CUIntArray & pattern, CUIntArray & system);
	// 패턴의 설비 목록 (1부터 시작)
	void GetPatternList(int nPatternIndex, CStringArray & system, CStringArray & broad);
	// 패턴의 설비 목록 (문자, P1... )
	void GetPatternList(CString sPattern, CStringArray & system, CStringArray & broad);
	// 패턴 카운트
	int  GetPatternCount();
	// 패턴 정보
	pPatternInfo GetPatternInfo(int nPatternIndex);

	// 기동램프(소화전기동) 전층
	void GetLamp(CStringArray & system);
	void GetLamp(CUIntArray & system);

	void ReleasePattern();

private:
	int GetFixedPatternCount();
	int ChangeFixedIndex();
	int GetVeriatyIndex(int nIndex);
	int GetPatternIndex(int nIndex);

	// 전층(경종)
	void AddAlarmPattern(SYSTEM_INFO_* pInfo);
	// 전층(시각경보, 비상방송)
	void AddVisualAlarmPattern(SYSTEM_INFO_* pInfo);
	// 계단별 전층(경종, 시각경보, 비상방송)
	void AddAlarmPatternUp(SYSTEM_INFO_* pInfo);
	// 지하 전층(경종, 시각경보, 비상방송)
	void AddAlarmPatternDown(SYSTEM_INFO_* pInfo);

	// 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	void AddDoorPattern(SYSTEM_INFO_* pInfo);
	// 계단별 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	void AddDoorPatternStair(SYSTEM_INFO_* pInfo);

	// 전층 급기댐퍼
	void AddDamperPattern(SYSTEM_INFO_* pInfo);
	// 계단별 급기댐퍼
	void AddDamperPatternStair(SYSTEM_INFO_* pInfo);

	// 층별 배연창
	//void AddSmokeScreenFloor(SYSTEM_INFO_* pInfo);
	// 계단/층별 배연창
	//void AddSmokeScreenStairF(SYSTEM_INFO_* pInfo);

	// 패턴 체크 및 추가
	void CheckAddPatternSystem(SYSTEM_INFO_* pInfo, int nType, CList<pPatternInfo, pPatternInfo> & pattern, CString sPatternName);
	// 패턴명 생성
	CString MakePatternName(SYSTEM_INFO_* pInfo, int nType, CString sPattern);

	void ReleaseList(CList<pPatternInfo, pPatternInfo> * pList);
	int  CheckType(int nType, SYSTEM_INFO_* pInfo);
	//void GetSystem(int nStair, int nFloor, CString sCircuitName, CStringArray & system);
	// -99 : 전계단, 전층  |  nType: (0: 해당층, 1:직상층 포함, 2: 지하층, 3: 지하층+1층)
	bool GetSystem(CString sBlock, int nStair, int nFloor, int nType, CStringArray & sCircuitName, CStringArray & system);
	bool GetSystem(CString sBlock, int nStair, int nFloor, CString sRoom, CString sCircuitName, CStringArray & system);
	void CheckAddPatternSystem(CString sValue, CStringArray & list);

	bool CheckCompareName(CStringArray & listName, CString sName);

	// 연동데이터
	void GetAlarmBroadPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern);// 경종, 비상방송, 시각경보 패턴
	void GetValve(SYSTEM_INFO_* pInfo, CStringArray & system);// 싸이렌 | 교차회로 밸브 값 / 아날로그 감지기 밸브 값
	void GetSiren(SYSTEM_INFO_* pInfo, CStringArray & system);// 싸이렌 동작확인 시 연동(svp ps, av p/s)
	void GetDoorPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern); // 자동 폐쇄, 창문 폐쇄, 옥상문, 비상등, 로비폰, 자동문 패턴
	void GetSmokeScreen(SYSTEM_INFO_* pInfo, CStringArray & system); // 배연창
	void GetAirDamper(SYSTEM_INFO_* pInfo, CStringArray & pattern); // 전실급기댐퍼
	void GetExDamper(SYSTEM_INFO_* pInfo, CStringArray & system); // 전실배기댐퍼
	void GetAirFan(SYSTEM_INFO_* pInfo, CStringArray & system); //급기팬
	void GetExFan(SYSTEM_INFO_* pInfo, CStringArray & system); //배기팬
	void GetSutterSmoke(SYSTEM_INFO_* pInfo, CStringArray & system); // 셔터 1차/2차 폐쇄 (연감지기)
	void GetSutterFire(SYSTEM_INFO_* pInfo, CStringArray & system); // 셔터 1차/2차 폐쇄 (열감지기)

private:
	patternInfo m_alarmPatternDown;	// 지하 전층(경종, 시각경보, 비상방송)

	CList<pPatternInfo, pPatternInfo> m_alarmPattern;		// 동별 전층(경종)
	CList<pPatternInfo, pPatternInfo> m_visualalarmPattern; // 동별 전층(시각경보, 비상방송)
	CList<pPatternInfo, pPatternInfo> m_alarmPatternUp;		// 계단별 지상 층별(경종, 시각경보, 비상방송)

	CList<pPatternInfo, pPatternInfo> m_doorPattern;		// 동별 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	CList<pPatternInfo, pPatternInfo> m_doorPatternStair;	// 계단별 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)

	CList<pPatternInfo, pPatternInfo> m_damperPatternAll;	// 동별 전층 급기댐퍼
	CList<pPatternInfo, pPatternInfo> m_damperPatternStair;	// 계단별 급기댐퍼
};

