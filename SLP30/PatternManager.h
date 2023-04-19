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
	����_�˶����� = 1,	//1
	����_�˶�, 
	����_�ð��溸,		//3
	����_�˶�����,
	����_������,		//5
	����_����ܺ�����, 
	����_��������,		//7
	����_���۰�ܺ�,	//8
};

enum {
	����_��_���� = 0, 
	����_��_��ܺ�, 
	����_��_���_��,
};

#define		FIXED_PATTERN		1

class CPatternManager : public TSingleton<CPatternManager>
{
public:
	CPatternManager();
	~CPatternManager();

public:
	// ���Ͽ� �߰�
	void AddPatternInfo(SYSTEM_INFO_* pInfo);
	// ���� ���� ���
	void GetPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern, CStringArray & system);
	void GetPattern(SYSTEM_INFO_* pInfo, CUIntArray & pattern, CUIntArray & system);
	// ������ ���� ��� (1���� ����)
	void GetPatternList(int nPatternIndex, CStringArray & system, CStringArray & broad);
	// ������ ���� ��� (����, P1... )
	void GetPatternList(CString sPattern, CStringArray & system, CStringArray & broad);
	// ���� ī��Ʈ
	int  GetPatternCount();
	// ���� ����
	pPatternInfo GetPatternInfo(int nPatternIndex);

	// �⵿����(��ȭ���⵿) ����
	void GetLamp(CStringArray & system);
	void GetLamp(CUIntArray & system);

	void ReleasePattern();

private:
	int GetFixedPatternCount();
	int ChangeFixedIndex();
	int GetVeriatyIndex(int nIndex);
	int GetPatternIndex(int nIndex);

	// ����(����)
	void AddAlarmPattern(SYSTEM_INFO_* pInfo);
	// ����(�ð��溸, �����)
	void AddVisualAlarmPattern(SYSTEM_INFO_* pInfo);
	// ��ܺ� ����(����, �ð��溸, �����)
	void AddAlarmPatternUp(SYSTEM_INFO_* pInfo);
	// ���� ����(����, �ð��溸, �����)
	void AddAlarmPatternDown(SYSTEM_INFO_* pInfo);

	// ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
	void AddDoorPattern(SYSTEM_INFO_* pInfo);
	// ��ܺ� ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
	void AddDoorPatternStair(SYSTEM_INFO_* pInfo);

	// ���� �ޱ����
	void AddDamperPattern(SYSTEM_INFO_* pInfo);
	// ��ܺ� �ޱ����
	void AddDamperPatternStair(SYSTEM_INFO_* pInfo);

	// ���� �迬â
	//void AddSmokeScreenFloor(SYSTEM_INFO_* pInfo);
	// ���/���� �迬â
	//void AddSmokeScreenStairF(SYSTEM_INFO_* pInfo);

	// ���� üũ �� �߰�
	void CheckAddPatternSystem(SYSTEM_INFO_* pInfo, int nType, CList<pPatternInfo, pPatternInfo> & pattern, CString sPatternName);
	// ���ϸ� ����
	CString MakePatternName(SYSTEM_INFO_* pInfo, int nType, CString sPattern);

	void ReleaseList(CList<pPatternInfo, pPatternInfo> * pList);
	int  CheckType(int nType, SYSTEM_INFO_* pInfo);
	//void GetSystem(int nStair, int nFloor, CString sCircuitName, CStringArray & system);
	// -99 : �����, ����  |  nType: (0: �ش���, 1:������ ����, 2: ������, 3: ������+1��)
	bool GetSystem(CString sBlock, int nStair, int nFloor, int nType, CStringArray & sCircuitName, CStringArray & system);
	bool GetSystem(CString sBlock, int nStair, int nFloor, CString sRoom, CString sCircuitName, CStringArray & system);
	void CheckAddPatternSystem(CString sValue, CStringArray & list);

	bool CheckCompareName(CStringArray & listName, CString sName);

	// ����������
	void GetAlarmBroadPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern);// ����, �����, �ð��溸 ����
	void GetValve(SYSTEM_INFO_* pInfo, CStringArray & system);// ���̷� | ����ȸ�� ��� �� / �Ƴ��α� ������ ��� ��
	void GetSiren(SYSTEM_INFO_* pInfo, CStringArray & system);// ���̷� ����Ȯ�� �� ����(svp ps, av p/s)
	void GetDoorPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern); // �ڵ� ���, â�� ���, ����, ����, �κ���, �ڵ��� ����
	void GetSmokeScreen(SYSTEM_INFO_* pInfo, CStringArray & system); // �迬â
	void GetAirDamper(SYSTEM_INFO_* pInfo, CStringArray & pattern); // ���Ǳޱ����
	void GetExDamper(SYSTEM_INFO_* pInfo, CStringArray & system); // ���ǹ�����
	void GetAirFan(SYSTEM_INFO_* pInfo, CStringArray & system); //�ޱ���
	void GetExFan(SYSTEM_INFO_* pInfo, CStringArray & system); //�����
	void GetSutterSmoke(SYSTEM_INFO_* pInfo, CStringArray & system); // ���� 1��/2�� ��� (��������)
	void GetSutterFire(SYSTEM_INFO_* pInfo, CStringArray & system); // ���� 1��/2�� ��� (��������)

private:
	patternInfo m_alarmPatternDown;	// ���� ����(����, �ð��溸, �����)

	CList<pPatternInfo, pPatternInfo> m_alarmPattern;		// ���� ����(����)
	CList<pPatternInfo, pPatternInfo> m_visualalarmPattern; // ���� ����(�ð��溸, �����)
	CList<pPatternInfo, pPatternInfo> m_alarmPatternUp;		// ��ܺ� ���� ����(����, �ð��溸, �����)

	CList<pPatternInfo, pPatternInfo> m_doorPattern;		// ���� ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
	CList<pPatternInfo, pPatternInfo> m_doorPatternStair;	// ��ܺ� ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)

	CList<pPatternInfo, pPatternInfo> m_damperPatternAll;	// ���� ���� �ޱ����
	CList<pPatternInfo, pPatternInfo> m_damperPatternStair;	// ��ܺ� �ޱ����
};

