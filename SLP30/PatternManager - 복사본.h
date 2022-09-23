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
	pattentype_normal, // ��, ��ܺ� ����
	pattentype_base, // ��, ��ܺ� ����
	pattentype_up, // ��, ��ܺ� ����

	pattentype_exceptalarm, // (����, �ð��溸, �����)���� ��, ��ܺ�
	pattentype_exceptdoor, // (����, �ð��溸, �����)���� ��, ��ܺ�
	pattentype_exceptalarmdoor, // (����, �ð��溸, �����)���� ��, ��ܺ�

	pattentype_alarmnormal,// (����, �ð��溸, �����)���� ��, ��ܺ� ����
	pattentype_alarmup, // (����, �ð��溸, �����)���� ��, ��ܺ� ����
	pattentype_alarmdown, // (����, �ð��溸, �����)���� ��, ��ܺ� ����

	pattentype_doornormal, // ����(�ڵ����, â�����, ������) ���� ��, ��ܺ� ����
	pattentype_doorup, // ����(�ڵ����, â�����, ������) ��, ��ܺ� ����
	pattentype_doordown, // ����(�ڵ����, â�����, ������) ��, ��ܺ� ����
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
	CList<pPatternInfo, pPatternInfo> m_listBasePattern;	// ��������
	CList<pPatternInfo, pPatternInfo> m_listUpPattern;		// ��������
	CList<pPatternInfo, pPatternInfo> m_listPattern1;		// ����, �ð��溸, ����� ���� ��, ��ܺ�
	CList<pPatternInfo, pPatternInfo> m_listPattern2;		// ����(�ڵ����, â�����, ������ ����)
	CList<pPatternInfo, pPatternInfo> m_listPattern3;		// ����(����, �ð��溸, �����, �ڵ����, â�����, ������ ����)
	patternInfo m_alarmPattern;		// ����(����, �ð��溸, �����)
	patternInfo m_alarmPatternUp;	// ���� ����(����, �ð��溸, �����)
	patternInfo m_alarmPatternDown;	// ���� ����(����, �ð��溸, �����)

	patternInfo m_doorPattern;		// ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)
	patternInfo m_doorPatternUp;	// ���� ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)
	patternInfo m_doorPatternDown;	// ���� ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)

	CList<pPatternInfo, pPatternInfo> m_patternTotal;
};

